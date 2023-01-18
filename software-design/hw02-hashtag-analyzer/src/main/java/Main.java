import org.apache.hc.client5.http.classic.methods.HttpGet;
import org.apache.hc.client5.http.classic.methods.HttpUriRequestBase;
import org.apache.hc.client5.http.impl.classic.CloseableHttpClient;
import org.apache.hc.client5.http.impl.classic.CloseableHttpResponse;
import org.apache.hc.client5.http.impl.classic.HttpClients;
import org.apache.hc.core5.http.io.entity.EntityUtils;
import org.apache.hc.core5.net.URIBuilder;
import ru.ivanau.htanalyzer.tracker.TrackerClient;
import ru.ivanau.htanalyzer.tracker.TrackerManager;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Arrays;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        TrackerClient client = new TrackerClient("https://api.vk.com", -1);
        TrackerManager manager = new TrackerManager(client);
        List<Long> diagram = manager.getHashtagFrequencyDiagram("coffee", 24);
        System.out.println(diagram);
//        try {
//            HttpGet httpGet = new HttpGet(" /method/users.get?");
//            URI uri = new URIBuilder(httpGet.getUri())
//                    .addParameter("user_ids", "210700286")
//                    .addParameter("fields", "bdate")
//                    .addParameter("access_token", "49e2bef949e2bef949e2bef9244af39507449e249e2bef92ab70796609d4ff44d6638d6")
//                    .addParameter("v", "5.131")
//                    .build();
//            httpGet.setUri(uri);
//            try (CloseableHttpClient httpclient = HttpClients.createDefault()) {
//                try (CloseableHttpResponse response = httpclient.execute(httpGet)) {
//                    System.out.println(response.getCode() + " " + response.getReasonPhrase());
//                    String json = EntityUtils.toString(response.getEntity());
//                    System.out.println(json);
//                }
//            }
//        } catch (Exception e) {
//            // do nothing
//        }
    }
}
