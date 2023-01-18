package ru.ivanau.htanalyzer;

import org.apache.hc.core5.http.NameValuePair;
import org.json.JSONObject;

import java.util.List;

public interface HttpAgent {
    JSONObject getResponse(final String host, List<NameValuePair> requestParameters);
}
