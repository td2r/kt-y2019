package ru.ivanau.htanalyzer.http;

import com.xebialabs.restito.semantics.Action;
import com.xebialabs.restito.semantics.Condition;
import com.xebialabs.restito.server.StubServer;
import org.glassfish.grizzly.http.Method;
import org.glassfish.grizzly.http.util.HttpStatus;
import org.json.JSONObject;
import org.junit.Assert;
import org.junit.Test;
import ru.ivanau.htanalyzer.HttpAgent;

import java.util.List;
import java.util.function.Consumer;

import static com.xebialabs.restito.builder.stub.StubHttp.whenHttp;
import static com.xebialabs.restito.semantics.Condition.method;
import static com.xebialabs.restito.semantics.Condition.startsWithUri;

public class HttpAgentWithStubServerTest {
    private static final int PORT = 32453;
    private final HttpAgent httpAgent = new HttpAgentImpl();

    @Test
    public void requestResponse() {
        withStubServer(PORT, s -> {
            whenHttp(s)
                    .match(method(Method.GET), Condition.startsWithUri("/hello"))
                    .then(Action.stringContent("{response: world}"));

            JSONObject response = httpAgent.getResponse("http://localhost:" + PORT + "/hello", List.of());
            Assert.assertEquals(response.getString("response"), "world");
        });
    }

    @Test(expected = RuntimeException.class)
    public void notFoundError() {
        withStubServer(PORT, s -> {
            whenHttp(s)
                    .match(method(Method.GET), startsWithUri("/sense-of-life"))
                    .then(Action.status(HttpStatus.NOT_FOUND_404));

            httpAgent.getResponse("http://localhost:" + PORT + "/sense-of-life", List.of());
        });
    }

    private void withStubServer(int port, Consumer<StubServer> callback) {
        StubServer stubServer = null;
        try {
            stubServer = new StubServer(port).run();
            callback.accept(stubServer);
        } finally {
            if (stubServer != null) {
                stubServer.stop();
            }
        }
    }
}
