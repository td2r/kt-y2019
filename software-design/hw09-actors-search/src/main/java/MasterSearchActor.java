import akka.actor.ActorRef;
import akka.actor.Props;
import akka.actor.UntypedAbstractActor;

import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

import static akka.pattern.Patterns.ask;


public class MasterSearchActor extends UntypedAbstractActor {
    public final static long TIMEOUT_MS = 500;

    private CompletableFuture<Object> makeRequest(String serviceUrl, String query, List<String> agg) {
        ChildSearchActor.SearchRequest request = new ChildSearchActor.SearchRequest(serviceUrl, query);
        ActorRef requestActor = getContext().actorOf(Props.create(ChildSearchActor.class), serviceUrl);
        return ask(requestActor, request, Duration.ofMillis(TIMEOUT_MS)).toCompletableFuture();
    }

    @Override
    public void onReceive(Object message) {
        if (message instanceof String) {
            String query = (String) message;
            List<String> aggregatedResult = new ArrayList<>();
            final List<CompletableFuture<Object>> request = List.of(
                    makeRequest("google", query, aggregatedResult),
                    makeRequest("bing", query, aggregatedResult),
                    makeRequest("duckduckgo", query, aggregatedResult)
            );
            request.forEach(req -> {
                try {
                    final Object results = req.get(TIMEOUT_MS, TimeUnit.MILLISECONDS);
                    if (results instanceof List) {
                        aggregatedResult.addAll((List<String>) results);
                    }
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                } catch (ExecutionException | TimeoutException e) {
                    // nothing
                }
            });

            getSender().tell(aggregatedResult, self());
            getContext().getSystem().stop(getSelf());
        }
    }
}