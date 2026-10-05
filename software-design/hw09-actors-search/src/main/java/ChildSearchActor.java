import akka.actor.UntypedAbstractActor;

import java.net.URI;
import java.util.Arrays;
import java.util.List;

public class ChildSearchActor extends UntypedAbstractActor {
    public static class SearchRequest {
        public final String serviceUrl;
        public final String query;

        public SearchRequest(String serviceUrl, String query) {
            this.serviceUrl = serviceUrl;
            this.query = query;
        }
    }

    @Override
    public void onReceive(Object message) throws Throwable {
        if (message instanceof SearchRequest) {
            SearchRequest req = (SearchRequest) message;
            URI uri = new URI("http://localhost:8888/" + req.serviceUrl + "?query=" + req.query);
            String response = UrlReader.readAsText(String.valueOf(uri));
            List <String> results = parseResponse(response);
            getSender().tell(results, getSelf());
            getContext().getSystem().stop(getSelf());
        }
    }

    public List<String> parseResponse(String response) {
        return Arrays.asList(response.split(","));
    }
}
