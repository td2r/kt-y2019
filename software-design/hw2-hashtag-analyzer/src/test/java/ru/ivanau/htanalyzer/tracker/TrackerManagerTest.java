package ru.ivanau.htanalyzer.tracker;

import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.List;

import static org.mockito.Mockito.when;

public class TrackerManagerTest {
    private TrackerManager trackerManager;

    @Mock
    private TrackerClient client;

    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        trackerManager = new TrackerManager(client);
    }

    @Test
    public void mockTest() {
        String tag = "christmas";
        when(client.getInfo(tag, 1)).thenReturn(Long.valueOf(42));
        when(client.getInfo(tag, 2)).thenReturn(Long.valueOf(44));
        when(client.getInfo(tag, 3)).thenReturn(Long.valueOf(43));

        List<Long> diagram = trackerManager.getHashtagFrequencyDiagram(tag, 3);
        Assert.assertArrayEquals(diagram.toArray(), new Long[]{42L, 44L, 43L});
    }
}
