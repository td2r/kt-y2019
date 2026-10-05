package ru.ivanau.sd.rxjava.server;

import io.reactivex.netty.protocol.http.server.HttpServer;
import ru.ivanau.sd.rxjava.database.Product;
import ru.ivanau.sd.rxjava.database.ReactiveMongoDriver;
import ru.ivanau.sd.rxjava.database.User;
import rx.Observable;

import java.security.InvalidParameterException;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;

public class RxNettyHttpServer {
    private final Map<String, Function<Map<String, List<String>>, Observable<String>>> REQUEST_FUN = Map.of(
            "createUser", this::createUser,
            "createProduct", this::createProduct,
            "getUsers", this::getUsers,
            "getProducts", this::getProducts
    );

    public void start(final int port) {
        HttpServer.newServer(port)
                .start((req, resp) -> {
                    final String path = req.getDecodedPath().substring(1);
                    final Map<String, List<String>> parameters = req.getQueryParameters();
                    final Function<Map<String, List<String>>, Observable<String>> resf = REQUEST_FUN.get(path);
                    if (resf == null) {
                        return resp.writeString(Observable.just("Not found"));
                    }
                    return resp.writeString(resf.apply(parameters));
                })
                .awaitShutdown();
    }

    private Observable<String> createUser(final Map<String, List<String>> parameters) {
        final int id;
        final String name;
        final String currency;
        try {
            id = Integer.parseInt(getParamOrException(parameters, "id"));
            name = getParamOrException(parameters, "name");
            currency = getParamOrException(parameters, "currency");
        } catch (final NumberFormatException | InvalidParameterException e) {
            return Observable.just(e.getMessage());
        }
        if (!Set.of("rub", "usd", "eur").contains(currency)) {
            return Observable.just("Unsupported currency");
        }
        ReactiveMongoDriver.createUser(new User(id, name, currency));
        return Observable.just("Success, created User{id=" + id + ", name=\"" + name + "\", currency=" + currency + "}");
    }

    private Observable<String> createProduct(final Map<String, List<String>> parameters) {
        final String name;
        final double price;
        try {
            name = getParamOrException(parameters, "name");
            price = Double.parseDouble(getParamOrException(parameters, "price"));
        } catch (final NumberFormatException | InvalidParameterException e) {
            return Observable.just(e.getMessage());
        }
        ReactiveMongoDriver.createProduct(new Product(name, price));
        return Observable.just("Created product \"" + name + "\" with price " + price + "$");
    }

    private Observable<String> getUsers(final Map<String, List<String>> parameters) {
        return ReactiveMongoDriver.getUsers().map(User::toString);
    }

    private Observable<String> getProducts(final Map<String, List<String>> parameters) {
        final int id;
        try {
            id = Integer.parseInt(getParamOrException(parameters, "id"));
        } catch (final NumberFormatException | InvalidParameterException e) {
            return Observable.just(e.getMessage());
        }
        final Observable<Product> products = ReactiveMongoDriver.getProducts();
        return ReactiveMongoDriver.getUserById(id)
                .map(user -> user.currency)
                .flatMap(currency -> products.map(product -> product.toString(currency)));
    }

    private String getParamOrException(final Map<String, List<String>> parameters, final String paramName) {
        final List<String> valueList = parameters.get(paramName);
        if (valueList == null || valueList.size() != 1) {
            throw new InvalidParameterException("Invalid " + paramName);
        }
        return valueList.get(0);
    }
}
