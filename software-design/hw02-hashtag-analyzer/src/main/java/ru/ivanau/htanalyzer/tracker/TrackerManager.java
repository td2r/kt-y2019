package ru.ivanau.htanalyzer.tracker;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

public class TrackerManager {
    private final TrackerClient client;

    public TrackerManager(final TrackerClient client) {
        this.client = client;
    }

    public List<Long> getHashtagFrequencyDiagram(final String hashtag, final int hours) {
        if (!(1 <= hours && hours <= 24)) {
            throw new IllegalArgumentException("hours must be in [1,24]");
        }
        List<Long> diagram = new ArrayList<>();
        for (int i = 1; i <= hours; ++i) {
            try {
                TimeUnit.MILLISECONDS.sleep(350);
                diagram.add(client.getInfo(hashtag, i));
            } catch (InterruptedException e) {
                i--;
            }
        }
        return diagram;
    }
}
