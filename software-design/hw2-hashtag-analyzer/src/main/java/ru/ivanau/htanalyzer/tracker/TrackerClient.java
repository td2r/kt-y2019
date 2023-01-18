package ru.ivanau.htanalyzer.tracker;

import org.apache.hc.core5.http.NameValuePair;
import org.apache.hc.core5.http.message.BasicNameValuePair;
import org.json.JSONArray;
import org.json.JSONObject;
import ru.ivanau.htanalyzer.HttpAgent;
import ru.ivanau.htanalyzer.http.HttpAgentImpl;

import java.time.Instant;
import java.util.List;

public class TrackerClient {
    private static final long SECONDS_IN_HOUR = 60 * 60;
    private static final String API_METHOD = "newsfeed.search";
    private static final String ACCESS_TOKEN = "49e2bef949e2bef949e2bef9244af39507449e249e2bef92ab70796609d4ff44d6638d6";
    private static final String API_VERSION = "5.131";
    private final String baseUrl;
    private final HttpAgent httpAgent;

    public TrackerClient(final String host, final int port) {
        baseUrl = host + (port >= 0 ? ":" + port : "") + "/method/" + API_METHOD + "?";
        httpAgent = new HttpAgentImpl();
    }

    public Long getInfo(final String hashtag, final int hours) {
        if (!(1 <= hours && hours <= 24)) {
            throw new IllegalArgumentException("hours must be in [1,24]");
        }
        long unixTime = Instant.now().getEpochSecond();
        final List<NameValuePair> params = List.of(
                new BasicNameValuePair("q", "#" + hashtag),
                new BasicNameValuePair("count", "0"),
                new BasicNameValuePair("start_time", Long.toString(unixTime - SECONDS_IN_HOUR * hours)),
                new BasicNameValuePair("end_time", Long.toString(unixTime - SECONDS_IN_HOUR * (hours - 1))),
                new BasicNameValuePair("v", API_VERSION),
                new BasicNameValuePair("access_token", ACCESS_TOKEN)
        );
        JSONObject json = httpAgent.getResponse(baseUrl, params);
        return json.getJSONObject("response").getLong("total_count");
    }
}
