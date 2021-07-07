package info.kgeorgiy.ja.Naumov.concurrent;

import info.kgeorgiy.java.advanced.mapper.ParallelMapper;

import java.util.*;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class ParallelMapperImpl implements ParallelMapper {
    private final Queue<Runnable> tasks;
    private final List<Thread> workers;
    private final Set<HashedIntLatch> latches;
    private volatile boolean closed;

    private static class HashedIntLatch {
        private static int unusedHash = 0;

        private int count;
        private final int hash;

        public HashedIntLatch(int count) {
            if (count < 0) {
                throw new IllegalArgumentException("Negative latch count");
            }
            this.count = count;
            hash = unusedHash++;
        }

        public synchronized void countDown() {
            if (count <= 0) {
                throw new IllegalStateException("Latch closed");
            }
            if (--count == 0) {
                notify();
            }
        }

        public synchronized void invalidate() {
            count = 0;
            notify();
        }

        public synchronized void await() throws InterruptedException {
            while (count != 0) {
                wait();
            }
        }

        @Override
        public int hashCode() {
            return hash;
        }
    }

    /**
     * Constructor from number of threads.
     *
     * @param threads amount of threads.
     */
    public ParallelMapperImpl(int threads) {
        if (threads <= 0) {
            throw new IllegalArgumentException("Threads number must be positive.");
        }
        final Runnable threadRun = () -> {
            while (!Thread.interrupted()) {
                try {
                    pollTask().run();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        };
        tasks = new ArrayDeque<>();
        workers = Stream.generate(() -> new Thread(threadRun))
                .limit(threads)
                .peek(Thread::start)
                .collect(Collectors.toList());
        latches = new HashSet<>();
        closed = false;
    }

    private void addTask(Runnable task) {
        synchronized (tasks) {
            tasks.add(task);
            tasks.notify();
        }
    }

    private Runnable pollTask() throws InterruptedException {
        Runnable task;
        synchronized (tasks) {
            while (tasks.isEmpty()) {
                tasks.wait();
            }
            task = tasks.poll();
        }
        return task;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T, R> List<R> map(Function<? super T, ? extends R> f, List<? extends T> args) throws InterruptedException {
        List<R> result = new ArrayList<>(Collections.nCopies(args.size(), null));
        HashedIntLatch remains = new HashedIntLatch(args.size());
        List<RuntimeException> errors = new ArrayList<>();
        List<Runnable> taskList = IntStream.range(0, args.size()).<Runnable>mapToObj(index -> () -> {
            try {
                result.set(index, f.apply(args.get(index)));
            } catch (RuntimeException e) {
                synchronized (errors) {
                    errors.add(e);
                }
            } finally {
                remains.countDown();
            }
        }).collect(Collectors.toList());
        synchronized (this) {
            if (closed) {
                throw new IllegalStateException("Mapper closed.");
            }
            taskList.forEach(this::addTask);
            synchronized (latches) {
                latches.add(remains);
            }
        }
        remains.await();
        synchronized (latches) {
            latches.remove(remains);
        }

        if (!errors.isEmpty()) {
            throw errors.stream().reduce((e1, e2) -> {
                e1.addSuppressed(e2);
                return e1;
            }).get();
        }
        return result;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() {
        synchronized (this) {
            if (closed) {
                return;
            }
            closed = true;
        }
        workers.forEach(Thread::interrupt);
        for (int i = 0; i < workers.size(); ++i) {
            try {
                workers.get(i).join();
            } catch (InterruptedException ignored) {
                --i;
            }
        }
        synchronized (latches) {
            latches.forEach(HashedIntLatch::invalidate);
        }
    }
}
