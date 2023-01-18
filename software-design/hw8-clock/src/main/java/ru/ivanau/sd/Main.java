package ru.ivanau.sd;

import ru.ivanau.sd.clock.NormalClock;
import ru.ivanau.sd.statistics.LastHourEventStatistics;

public class Main {
    public static void main(String[] args) {
        LastHourEventStatistics statistics = new LastHourEventStatistics(new NormalClock());
        statistics.incEvent("a");
        statistics.incEvent("a");
        statistics.incEvent("a");
        statistics.incEvent("a");
        statistics.incEvent("b");
        statistics.incEvent("b");
        statistics.incEvent("b");
        statistics.incEvent("c");
        statistics.incEvent("c");
        statistics.incEvent("c");
        statistics.incEvent("c");
        statistics.printStatistics();
    }
}
