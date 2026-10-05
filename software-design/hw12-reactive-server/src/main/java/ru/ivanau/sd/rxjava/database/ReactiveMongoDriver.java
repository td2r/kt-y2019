package ru.ivanau.sd.rxjava.database;

import com.mongodb.client.model.Filters;
import com.mongodb.rx.client.MongoClient;
import com.mongodb.rx.client.MongoClients;
import rx.Observable;

public class ReactiveMongoDriver {
    private static final MongoClient CLIENT = createMongoClient();
    private static final String DATABASE_NAME = "software-design";
    private static final String USERS_COLLECTION = "users";
    private static final String PRODUCTS_COLLECTION = "products";

    public static void createUser(final User user) {
        CLIENT.getDatabase(DATABASE_NAME)
                .getCollection(USERS_COLLECTION)
                .insertOne(user.getDocument())
                .subscribe();
    }

    public static Observable<User> getUserById(final int id) {
        return CLIENT.getDatabase(DATABASE_NAME)
                .getCollection(USERS_COLLECTION)
                .find(Filters.eq("id", id))
                .toObservable()
                .map(User::new);
    }

    public static Observable<User> getUsers() {
        return CLIENT.getDatabase(DATABASE_NAME)
                .getCollection(USERS_COLLECTION)
                .find()
                .toObservable()
                .map(User::new);
    }

    public static void createProduct(final Product product) {
        CLIENT.getDatabase(DATABASE_NAME)
                .getCollection(PRODUCTS_COLLECTION)
                .insertOne(product.getDocument())
                .subscribe();
    }

    public static Observable<Product> getProducts() {
        return CLIENT.getDatabase(DATABASE_NAME)
                .getCollection(PRODUCTS_COLLECTION)
                .find()
                .toObservable()
                .map(Product::new);
    }

    private static MongoClient createMongoClient() {
        return MongoClients.create("mongodb://localhost:27017");
    }
}

