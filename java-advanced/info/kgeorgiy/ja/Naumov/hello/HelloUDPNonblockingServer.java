package info.kgeorgiy.ja.Naumov.hello;

import info.kgeorgiy.java.advanced.hello.HelloServer;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.DatagramChannel;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.stream.IntStream;

import static info.kgeorgiy.ja.Naumov.hello.Utils.*;

public class HelloUDPNonblockingServer implements HelloServer {
    private ExecutorService threads;
    private final ExecutorService listener = Executors.newSingleThreadExecutor();

    private Selector selector;
    private DatagramChannel serverChannel;
    private SelectionKey selectionKey; // effectively final

    private final NodeStack readBuffers = new NodeStack();
    private final NodeStack workBuffers = new NodeStack();
    private final NodeStack writeBuffers = new NodeStack();

    private final Runnable listenAction = () -> {
        try {
            while (selector.isOpen() && serverChannel.isOpen()) {
                selectionKey.interestOps((readBuffers.isEmpty() ? 0 : SelectionKey.OP_READ)
                        | (writeBuffers.isEmpty() ? 0 : SelectionKey.OP_WRITE));
                selector.select();
                ServerData data;
                if (selectionKey.isReadable() && (data = (ServerData) readBuffers.take()) != null) {
                    receive(data);
                }
                if (selectionKey.isWritable() && (data = (ServerData) writeBuffers.take()) != null) {
                    respond(data);
                }
            }
        } catch (IOException e) {
            System.err.println("Listening error: " + e.getMessage());
        }
    };

    private final Runnable workAction = () -> {
        final ServerData data = (ServerData) workBuffers.take();
        data.getBuffer().flip();
        writeBuffers.push(data);
        if ((selectionKey.interestOpsOr(SelectionKey.OP_WRITE) & SelectionKey.OP_WRITE) == 0) {
            selector.wakeup();
        }
    };

    private void receive(final ServerData data) {
        try {
            data.setAddress(serverChannel.receive(data.getBuffer()));
            if (data.getAddress() == null) {
                readBuffers.push(data);
                return;
            }
            workBuffers.push(data);
            threads.submit(workAction);
        } catch (IOException e) {
            readBuffers.push(data);
        }
    }

    private void respond(final ServerData data) {
        try {
            serverChannel.send(data.getBuffer(), data.getAddress());
        } catch (IOException ignored) {
        } finally {
            data.getBuffer().clear().position(SERVER_PREFIX.length);
            readBuffers.push(data);
        }
    }

    @Override
    public void start(final int port, final int threads) {
        this.threads = Executors.newFixedThreadPool(threads);

        try {
            selector = Selector.open();
            serverChannel = DatagramChannel.open();
            serverChannel.configureBlocking(false);
            serverChannel.bind(new InetSocketAddress(port));
            selectionKey = serverChannel.register(selector, SelectionKey.OP_READ);
        } catch (IOException e) {
            e.printStackTrace();
        }

        IntStream.range(0, threads)
                .mapToObj(i -> new ServerData())
                .forEach(readBuffers::push);

        listener.submit(listenAction);
    }

    @Override
    public void close() {
        try {
            serverChannel.close();
            selector.close();
        } catch (IOException e) {
            System.err.println("Closing error: " + e.getMessage());
        }

        listener.shutdownNow();
        threads.shutdown(); // Disable new tasks from being submitted
        try {
            // Wait a while for existing tasks to terminate
            if (!threads.awaitTermination(60, TimeUnit.SECONDS)) {
                threads.shutdownNow(); // Cancel currently executing tasks
                // Wait a while for tasks to respond to being cancelled
                if (!threads.awaitTermination(60, TimeUnit.SECONDS))
                    System.err.println("Pool did not terminate");
            }
        } catch (InterruptedException ie) {
            // (Re-)Cancel if current thread also interrupted
            threads.shutdownNow();
            // Preserve interrupt status
            Thread.currentThread().interrupt();
        }
    }

    public static void main(String[] args) {
        launchServer(new HelloUDPNonblockingServer(), args);
    }
}
