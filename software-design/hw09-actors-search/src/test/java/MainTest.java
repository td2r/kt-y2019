import akka.actor.ActorSystem;
import com.xebialabs.restito.server.StubServer;
import org.junit.Test;

import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static org.junit.Assert.assertEquals;

public class MainTest {
    ActorSystem system = ActorSystem.create("testSystem");

    @Test
    public void fastServer() {
        StubServer server = new SearchStubServer();
        for (int i = 0; i < 15; i++) {
            String str = Integer.toString(i);
            List<String> result = ActorsSearchEngine.search(str, system);
            assertEquals(45, result.size());
            Map<String, Long> count = result.stream().collect(Collectors.groupingBy(x -> x.split("_")[0], Collectors.counting()));
            count.forEach((k, v) -> assertEquals(Long.valueOf(15), v));
        }
        server.stop();
    }

    @Test
    public void defaultServer() {
        StubServer server = new SearchStubServer(300);
        for (int i = 0; i < 15; i++) {
            String str = Integer.toString(i);
            List<String> result = ActorsSearchEngine.search(str, system);
            assertEquals(45, result.size());
            Map<String, Long> count = result.stream().collect(Collectors.groupingBy(x -> x.split("_")[0], Collectors.counting()));
            count.forEach((k, v) -> assertEquals(Long.valueOf(15), v));
        }
        server.stop();
    }

    @Test
    public void slowServer() {
        StubServer server = new SearchStubServer(1000);
        for (int i = 0; i < 15; i++) {
            String str = Integer.toString(i);
            List<String> result = ActorsSearchEngine.search(str, system);
            assertEquals(0, result.size());
            Map<String, Long> count = result.stream().collect(Collectors.groupingBy(x -> x.split("_")[0], Collectors.counting()));
            count.forEach((k, v) -> assertEquals(Long.valueOf(0), v));
        }
        server.stop();
    }
}
