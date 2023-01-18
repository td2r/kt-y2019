package ru.ivanau.sd.statistics;

import ru.ivanau.sd.clock.Clock;
import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class LastHourEventStatistics implements EventStatistics {
    private final Clock clock;
    private final Map<String, Queue<Instant>> stats;

    public LastHourEventStatistics(final Clock clock) {
        this.clock = clock;
        this.stats = new HashMap<>();
    }

    private void removeBefore(final Queue<Instant> q, final Instant time) {
        while (!q.isEmpty() && q.peek().compareTo(time) < 0) {
            q.remove();
        }
    }

    private void removeBefore(final Instant time) {
        final Set<String> keySet = Set.copyOf(stats.keySet());
        for (final String name : keySet) {
            final Queue<Instant> q = stats.get(name);
            removeBefore(q, time);
            if (q.isEmpty()) {
                stats.remove(name);
            }
        }
    }

    @Override
    public void incEvent(final String name) {
        final Instant now = clock.now();
        removeBefore(now.minus(1, ChronoUnit.HOURS));
        Queue<Instant> q = stats.get(name);
        if (q == null) {
            q = new ArrayDeque<>();
            stats.put(name, q);
        }
        q.add(now);
    }

    private List<Long> convert(final Queue<Instant> q, final Instant now) {
        final List<Long> res = new ArrayList<>(IntStream.rangeClosed(0, 60).mapToLong(i -> 0).boxed().toList());
        q.stream()
                .mapToInt(t -> (int) ChronoUnit.MINUTES.between(t, now))
                .forEach(x -> res.set(x, res.get(x) + 1));
        return res;
    }

    @Override
    public List<Long> getEventStatisticsByName(final String name) {
        final Instant now = clock.now();
        Queue<Instant> q = stats.get(name);
        if (q == null) {
            q = new ArrayDeque<>();
        }
        removeBefore(q, now.minus(1, ChronoUnit.HOURS));
        return convert(q, now);
    }

    @Override
    public Map<String, List<Long>> getAllEventStatistics() {
        final Instant now = clock.now();
        removeBefore(now.minus(1, ChronoUnit.HOURS));
        Map<String, List<Long>> res = new HashMap<>();
        for (final Map.Entry<String, Queue<Instant>> entry : stats.entrySet()) {
            res.put(entry.getKey(), convert(entry.getValue(), now));
        }
        return res;
    }

    @Override
    public void printStatistics() {
        Map<String, List<Long>> allStats = getAllEventStatistics();
        for (final Map.Entry<String, List<Long>> entry : allStats.entrySet()) {
            System.out.print("\"" + entry.getKey() + "\": ");
            System.out.println(IntStream.range(0, entry.getValue().size())
                    .filter(i -> entry.getValue().get(i) != 0)
                    .mapToObj(i -> "{" + i + ", " + entry.getValue().get(i) + "}")
                    .collect(Collectors.joining(", "))
            );
        }
    }
}
