package ru.ivanau.sd.statistics;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import ru.ivanau.sd.clock.SettableClock;

import java.time.Instant;
import java.time.temporal.ChronoUnit;
import java.util.List;

public class LastHourStatisticsTests {
    private final static Instant TIME_START = Instant.parse("2019-02-14T18:42:00Z");

    SettableClock clock;
    EventStatistics statistics;

    @Before
    public void before() {
        clock = new SettableClock(TIME_START);
        statistics = new LastHourEventStatistics(clock);
    }

    private void setTime(int h, int m, int s) {
        Instant t = TIME_START;
        if (h != 0)
            t = t.plus(h, ChronoUnit.HOURS);
        if (m != 0)
            t = t.plus(m, ChronoUnit.MINUTES);
        if (s != 0)
            t = t.plus(s, ChronoUnit.SECONDS);
        clock.setNow(t);
    }

    @Test
    public void emptyStatisticsTest() {
        final List<Long> l = statistics.getEventStatisticsByName("aboba");
        Assert.assertEquals(l.size(), 61);
        for (final Long amount : l) {
            Assert.assertEquals((long) amount, 0);
        }
    }

    @Test
    public void singleNameTest() {
        statistics.incEvent("event1");
        setTime(0, 1, 0);
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        setTime(0, 2, 0);
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        setTime(0, 3, 0);
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        statistics.incEvent("event1");
        List<Long> l = statistics.getEventStatisticsByName("event1");
        for (int i = 0; i < l.size(); ++i) {
            Assert.assertEquals((long) l.get(i), i < 4 ? 4 - i : 0);
        }
    }

    @Test
    public void multipleNamesTest() {
        for (int i = 0; i <= 60; ++i) {
            setTime(0, i, 0);
            for (int j = 1; j <= 3; ++j) {
                statistics.incEvent("event" + j);
            }
        }
        setTime(1, 0, 30);
        List<List<Long>> l = List.of(statistics.getEventStatisticsByName("event1"),
                statistics.getEventStatisticsByName("event2"),
                statistics.getEventStatisticsByName("event3")
        );
        for (int i = 0; i <= 60; ++i) {
            for (int j = 0; j < 3; ++j) {
                Assert.assertEquals(i == 60 ? 0 : 1, (long) l.get(j).get(i));
            }
        }
    }

    @Test
    public void lastHourTest() {
        for (int i = 0; i <= 60; ++i) {
            setTime(0, i, 0);
            statistics.incEvent("event1");
            statistics.incEvent("event2");
        }
        for (int i = 0; i <= 60; ++i) {
            setTime(1, i, 30);
            if (i % 2 == 0) {
                statistics.incEvent("event2");
            }
        }
        List<Long> l1 = statistics.getEventStatisticsByName("event1");
        List<Long> l2 = statistics.getEventStatisticsByName("event2");
        for (int i = 0; i <= 60; ++i) {
            Assert.assertEquals(0, (long) l1.get(i));
            Assert.assertEquals(1 - (i % 2), (long) l2.get(i));
        }
    }
}
