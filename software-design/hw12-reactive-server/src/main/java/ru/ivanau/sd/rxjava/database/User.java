package ru.ivanau.sd.rxjava.database;

import org.bson.Document;

public class User {
    public final int id;
    public final String name;
    public final String currency;

    public User(final Document document) {
        this(document.getInteger("id"), document.getString("name"), document.getString("currency"));
    }

    public User(final int id, final String name, final String currency) {
        this.id = id;
        this.name = name;
        this.currency = currency;
    }

    public Document getDocument() {
        return new Document("id", id)
                .append("name", name)
                .append("currency", currency);
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", currency='" + currency + '\'' +
                '}';
    }
}
