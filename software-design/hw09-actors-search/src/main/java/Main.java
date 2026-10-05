import akka.actor.ActorSystem;
import com.xebialabs.restito.server.StubServer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    public static void main(String[] args) {
        StubServer server = new SearchStubServer();
        ActorSystem system = ActorSystem.create("search");

        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));

        while(true) {
            String query;
            try {
                query = reader.readLine();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            if (query == null || query.isEmpty()) {
                server.stop();
                System.exit(0);
            }

            System.out.println(ActorsSearchEngine.search(query, system));
        }
    }
}
