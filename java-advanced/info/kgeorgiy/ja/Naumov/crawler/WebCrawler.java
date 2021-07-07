package info.kgeorgiy.ja.Naumov.crawler;

import info.kgeorgiy.java.advanced.crawler.*;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.*;
import java.util.concurrent.*;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class WebCrawler implements AdvancedCrawler {
    private final Downloader downloader;
    private final ExecutorService downloaders;
    private final ExecutorService crawlers;
    private final int perHost;
    private volatile boolean closed;

    public WebCrawler(final Downloader downloader, final int downloaders, final int extractors, final int perHost) {
        if (downloader == null) {
            throw new NullPointerException("null downloader");
        }
        this.downloader = downloader;
        this.downloaders = Executors.newFixedThreadPool(downloaders);
        this.crawlers = Executors.newFixedThreadPool(extractors);
        this.perHost = perHost;
        closed = false;
    }

    private synchronized void ifClosed() {
        if (closed) {
            throw new IllegalStateException("Crawler closed");
        }
    }

    @Override
    public Result download(final String url, final int depth) {
        ifClosed();
        return crawlAndDownload(url, depth, new ConcurrentHashMap<>(), (host) -> new ConcurrentLinkedQueue<>());
    }

    @Override
    public Result download(final String url, final int depth, final List<String> hosts) {
        ifClosed();
        return crawlAndDownload(url,
                depth,
                hosts.stream()
                        .distinct()
                        .collect(Collectors.toMap(host -> host,
                                                  host -> new ConcurrentLinkedQueue<>())),
                (host) -> null);
    }

    private Result crawlAndDownload(final String root,
                                    final int depth,
                                    final Map<String, Queue<String>> hostQueues,
                                    final Function<? super String, ? extends Queue<String>> queueSupplier) {
        final Set<String> downloaded = ConcurrentHashMap.newKeySet();
        final Map<String, IOException> failed = new ConcurrentHashMap<>();
        final List<Map<String, Queue<String>>> layers = List.of(hostQueues, new ConcurrentHashMap<>(hostQueues));

        try {
            final String host = URLUtils.getHost(root);
            final Queue<String> queue = layers.get(0).computeIfAbsent(host, queueSupplier);
            if (queue != null) {
                downloaded.add(root);
                queue.add(root);
            }
        } catch (MalformedURLException e) {
            failed.put(root, e);
        }
        final Phaser phaser = new Phaser(1);
        for (int i = 0; i < depth; ++i) {
            final Map<String, Queue<String>> currentLayerMap = layers.get(i & 1);
            final Map<String, Queue<String>> nextLayerMap = layers.get((i ^ 1) & 1);

            final Consumer<Document> crawlAction = i == depth - 1 ? (document) -> {} : (document) ->
            {
                phaser.register();
                crawlers.submit(() -> {
                    try {
                        if (closed)
                            return;
                        document.extractLinks().forEach(link -> {
                            try {
                                final String host = URLUtils.getHost(link);
                                final Queue<String> queue = nextLayerMap.computeIfAbsent(host, queueSupplier);
                                if (queue != null && downloaded.add(link)) {
                                    queue.add(link);
                                }
                            } catch (MalformedURLException ignored) {
                            }
                        });
                    } catch (IOException ignored) {
                    } finally {
                        phaser.arriveAndDeregister();
                    }
                });
            };
            final Consumer<String> downloadAction = (url) ->
            {
                try {
                    if (closed)
                        return;
                    crawlAction.accept(downloader.download(url));
                } catch (IOException e) {
                    failed.put(url, e);
                } finally {
                    phaser.arriveAndDeregister();
                }
            };
            currentLayerMap.forEach((host, hostQueue) -> {
                phaser.bulkRegister(hostQueue.size());
                IntStream.range(0, Math.min(hostQueue.size(), perHost))
                        .forEach(x -> submitDownload(hostQueue, downloadAction));
            });
            phaser.arriveAndAwaitAdvance();
        }
        downloaded.removeAll(failed.keySet());
        return new Result(new ArrayList<>(downloaded), failed);
    }

    private void submitDownload(final Queue<String> hostQueue, final Consumer<String> downloadAction) {
        final String url = hostQueue.poll();
        if (url != null) {
            downloaders.submit(() -> {
                downloadAction.accept(url);
                submitDownload(hostQueue, downloadAction);
            });
        }
    }

    private void awaitTermination(final ExecutorService pool) {
        try {
            // Wait a while for existing tasks to terminate
            if (!pool.awaitTermination(60, TimeUnit.SECONDS)) {
                pool.shutdownNow(); // Cancel currently executing tasks
                // Wait a while for tasks to respond to being cancelled
                if (!pool.awaitTermination(60, TimeUnit.SECONDS))
                    System.err.println("Pool did not terminate");
            }
        } catch (InterruptedException ie) {
            // (Re-)Cancel if current thread also interrupted
            pool.shutdownNow();
            // Preserve interrupt status
            Thread.currentThread().interrupt();
        }
    }

    @Override
    public void close() {
        synchronized (this) {
            if (closed)
                return;
            closed = true;
        }
        downloaders.shutdown(); // Disable new tasks from being submitted
        crawlers.shutdown(); // Disable new tasks from being submitted
        awaitTermination(downloaders);
        awaitTermination(crawlers);
    }

    private static final int MIN_ARGS = 1;
    private static final int MAX_ARGS = 5;
    private static final int DEFAULT_PARAM = 1;

    public static void main(String[] args) {
        if (args == null
                || args.length < MIN_ARGS
                || args.length > MAX_ARGS
                || Arrays.stream(args).anyMatch(Objects::isNull)) {
            System.out.println("Usage: WebCrawler url [depth [downloads [extractors [perHost]]]]");
            return;
        }
        final int[] params;
        try {
            params = IntStream.range(MIN_ARGS, MAX_ARGS)
                    .map(i -> i < args.length ? Integer.parseInt(args[i]) : DEFAULT_PARAM)
                    .toArray();
        } catch (NumberFormatException e) {
            System.err.println("Not a number:\n" + e.getMessage());
            return;
        }
        try (final WebCrawler crawler = new WebCrawler(new CachingDownloader(), params[1], params[2], params[3])) {
            final Result result = crawler.download(args[0], params[0]);
            System.out.println("Downloaded:");
            System.out.println(result.getDownloaded());
            System.out.println("Errors:");
            System.out.println(result.getErrors());
        } catch (IOException e) {
            System.err.println("Can't create crawler:\n" + e.getMessage());
        }
    }
}
