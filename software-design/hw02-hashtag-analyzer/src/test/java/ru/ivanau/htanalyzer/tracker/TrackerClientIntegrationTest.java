package ru.ivanau.htanalyzer.tracker;

import org.junit.Assert;
import org.junit.ClassRule;
import org.junit.Test;
import ru.ivanau.htanalyzer.rule.HostReachableRule;

@HostReachableRule.HostReachable(TrackerClientIntegrationTest.HOST)
public class TrackerClientIntegrationTest {
    public static final String HOST = "api.vk.com";

    @ClassRule
    public static final HostReachableRule rule = new HostReachableRule();

    @Test
    public void getInfo() {
        TrackerClient client = new TrackerClient("https://" + HOST, -1);
        Long posts = client.getInfo("halloween", 2);
        Assert.assertTrue(posts >= 0);
    }
}
