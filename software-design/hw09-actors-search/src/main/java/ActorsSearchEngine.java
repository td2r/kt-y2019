import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;

import java.time.Duration;
import java.util.List;
import java.util.concurrent.CompletableFuture;

import static akka.pattern.Patterns.ask;

public class ActorsSearchEngine {
    public static List<String> search(String query, ActorSystem system) {
        String actorName = String.format("searchActor%s%d", query, System.currentTimeMillis());
        ActorRef actor = system.actorOf(Props.create(MasterSearchActor.class), actorName);
        CompletableFuture<Object> response = ask(actor, query, Duration.ofSeconds(5)).toCompletableFuture();
        List<String> result = (List<String>) response.join();
        return result;
    }
}
