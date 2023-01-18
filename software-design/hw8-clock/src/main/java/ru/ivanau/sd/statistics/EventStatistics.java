package ru.ivanau.sd.statistics;

import java.util.List;
import java.util.Map;

public interface EventStatistics {
    void incEvent(final String name);
    List<Long> getEventStatisticsByName(final String name);
    Map<String, List<Long>> getAllEventStatistics();
    void printStatistics();
}
