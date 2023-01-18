package ru.ivanau.htanalyzer.http;

import org.json.JSONObject;
import org.junit.Assert;
import org.junit.ClassRule;
import org.junit.Test;
import ru.ivanau.htanalyzer.rule.HostReachableRule;

import java.util.List;

@HostReachableRule.HostReachable("api.vk.com")
public class HttpAgentTest {
    @ClassRule
    public static final HostReachableRule rule = new HostReachableRule();

    @Test
    public void getResponse() {
        HttpAgentImpl httpAgent = new HttpAgentImpl();
        JSONObject response =
                httpAgent.getResponse(
                        "https://api.vk.com/method/users.get?user_ids=210700286&fields=bdate&access_token=49e2bef949e2bef949e2bef9244af39507449e249e2bef92ab70796609d4ff44d6638d6&v=5.131",
                List.of());
        System.out.println(response);
        Assert.assertArrayEquals(response.keySet().toArray(), new String[]{"response"});
        Assert.assertEquals(response.getJSONArray("response").getJSONObject(0).getLong("id"),
                210700286);
    }
}
