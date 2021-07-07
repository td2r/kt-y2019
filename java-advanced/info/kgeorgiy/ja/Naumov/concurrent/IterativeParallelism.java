package info.kgeorgiy.ja.Naumov.concurrent;

import info.kgeorgiy.java.advanced.concurrent.AdvancedIP;
import info.kgeorgiy.java.advanced.mapper.ParallelMapper;

import java.util.*;
import java.util.function.BinaryOperator;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.stream.Stream;

public class IterativeParallelism implements AdvancedIP {
    private static final Function<Stream<? extends Boolean>, Boolean> FORALL_TRUE
            = stream -> stream.allMatch(Boolean::booleanValue);
    private final ParallelMapper parallelMapper;

    /**
     * Default constructor
     */
    public IterativeParallelism() {
        this(null);
    }

    /**
     * Constructor from a {@link ParallelMapper}.
     *
     * @param parallelMapper Mapper for parallel computations.
     */
    public IterativeParallelism(ParallelMapper parallelMapper) {
        this.parallelMapper = parallelMapper;
    }

    private <T> List<T> concatToList(Stream<? extends List<T>> stream) {
        return stream.flatMap(Collection::stream).collect(Collectors.toList());
    }

    private String concatToString(Stream<?> stream) {
        return stream.map(Objects::toString).collect(Collectors.joining());
    }

    private <T> List<Stream<? extends T>> splitTask(List<? extends T> task, int subtasks) {
        List<Stream<? extends T>> result = new ArrayList<>();
        int subtaskSize = task.size() / subtasks;
        int surplus = task.size() % subtasks;
        for (int l = 0; l < task.size(); ) {
            int r = l + subtaskSize;
            if (surplus > 0) {
                r++;
                surplus--;
            }
            result.add(task.subList(l, r).stream());
            l = r;
        }
        return result;
    }

    private void interruptExecution(List<Thread> threads, InterruptedException exception) {
        threads.forEach(Thread::interrupt);
        for (int i = 0; i < threads.size(); ++i) {
            try {
                threads.get(i).join();
            } catch (InterruptedException e) {
                exception.addSuppressed(e);
                --i;
            }
        }
    }

    private <T, R> List<R> executeParallel(int threads,
                                           List<Stream<? extends T>> tasks,
                                           Function<Stream<? extends T>, ? extends R> solver) throws InterruptedException {
        List<R> result = new ArrayList<>(Collections.nCopies(threads, null));
        List<Thread> workers = IntStream.range(0, threads)
                .mapToObj(i -> new Thread(() -> result.set(i, solver.apply(tasks.get(i)))))
                .peek(Thread::start)
                .collect(Collectors.toList());
        InterruptedException exception = null;
        for (int i = 0; i < workers.size(); ++i) {
            try {
                workers.get(i).join();
            } catch (InterruptedException e) {
                interruptExecution(workers.subList(i, workers.size()), e);
                exception = e;
                break;
            }
        }
        if (exception != null) {
            throw exception;
        }
        return result;
    }

    private <T, R> R abstractParallel(int threads,
                                      List<? extends T> values,
                                      Function<Stream<? extends T>, R> solver,
                                      Function<Stream<? extends R>, R> collector) throws InterruptedException {
        if (threads <= 0) {
            throw new IllegalArgumentException("Can't split task to non-positive amount of threads.");
        }
        if (threads > values.size()) {
            threads = values.size();
        }
        List<Stream<? extends T>> tasks = splitTask(values, threads);
        List<R> results = parallelMapper == null
                ? executeParallel(threads, tasks, solver)
                : parallelMapper.map(solver, tasks);
        return collector.apply(results.stream());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T minimum(int threads, List<? extends T> values, Comparator<? super T> comparator) throws InterruptedException {
        Function<Stream<? extends T>, T> solver =
                stream -> stream.reduce(BinaryOperator.minBy(comparator)).orElseThrow();
        return abstractParallel(threads, values, solver, solver);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T maximum(int threads, List<? extends T> values, Comparator<? super T> comparator) throws InterruptedException {
        return minimum(threads, values, comparator.reversed());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> boolean any(int threads, List<? extends T> values, Predicate<? super T> predicate) throws InterruptedException {
        return !all(threads, values, Predicate.not(predicate));
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> boolean all(int threads, List<? extends T> values, Predicate<? super T> predicate) throws InterruptedException {
        return abstractParallel(threads, values, stream -> stream.allMatch(predicate), FORALL_TRUE);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String join(int threads, List<?> values) throws InterruptedException {
        return abstractParallel(threads, values, this::concatToString, this::concatToString);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> List<T> filter(int threads, List<? extends T> values, Predicate<? super T> predicate) throws InterruptedException {
        return abstractParallel(threads, values,
                stream -> stream.filter(predicate).collect(Collectors.toList()),
                this::concatToList);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T, U> List<U> map(int threads, List<? extends T> values, Function<? super T, ? extends U> f) throws InterruptedException {
        return abstractParallel(threads, values,
                stream -> stream.map(f).collect(Collectors.toList()),
                this::concatToList);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T reduce(int threads, List<T> values, Monoid<T> monoid) throws InterruptedException {
        return mapReduce(threads, values, Function.identity(), monoid);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T, R> R mapReduce(int threads, List<T> values, Function<T, R> lift, Monoid<R> monoid) throws InterruptedException {
        Function<Stream<? extends R>, R> reducer =
                stream -> stream.reduce(monoid.getIdentity(), monoid.getOperator(), monoid.getOperator());
        return abstractParallel(threads, values, stream -> reducer.apply(stream.map(lift)), reducer);
    }
}
