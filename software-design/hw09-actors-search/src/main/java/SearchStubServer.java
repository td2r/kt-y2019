import com.xebialabs.restito.server.StubServer;
import org.glassfish.grizzly.http.Method;

import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static com.xebialabs.restito.builder.stub.StubHttp.whenHttp;
import static com.xebialabs.restito.semantics.Action.*;
import static com.xebialabs.restito.semantics.Condition.method;
import static com.xebialabs.restito.semantics.Condition.startsWithUri;

public class SearchStubServer extends StubServer {
    private static String randomString(String service) {
        return IntStream.range(0, 15)
                .mapToObj(j -> (IntStream.range(0, 32)
                        .map(i -> (char) ('a' + Math.random() * ('z' - 'a')))
                        .collect(() -> new StringBuilder(service + "_"),
                                StringBuilder::appendCodePoint,
                                StringBuilder::append)
                        .toString()))
                .collect(Collectors.joining(","));
    }

    public SearchStubServer() {
        this(0);
    }

    public SearchStubServer(int delay_ms) {
        super(8888);
        Map.of("google", "Google", "bing", "Bing", "duckduckgo", "DuckDuckGo")
                .forEach((k, v) -> whenHttp(this)
                        .match(method(Method.GET), startsWithUri("/" + k)).then(ok())
                        .withSequence(composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms)),
                                composite(stringContent(randomString(v)), delay(delay_ms))
                        )
                );
        run();
    }
}
