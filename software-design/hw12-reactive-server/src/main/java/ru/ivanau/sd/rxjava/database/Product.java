package ru.ivanau.sd.rxjava.database;

import org.bson.Document;

public class Product {
    private final String name;
    private final Double price;

    public Product(final Document document) {
        this(document.getString("name"), document.getDouble("price"));
    }

    public Product(final String name, final Double price) {
        this.name = name;
        this.price = price;
    }

    public Document getDocument() {
        return new Document("name", name).append("price", price);
    }

    public String toString(final String currency) {
        final double showPrice = switch (currency) {
            case "rub" -> price * 80;
            case "usd" -> price;
            case "eur" -> price / 1.1;
            default -> throw new IllegalArgumentException("Unknown currency: " + currency);
        };
        return name + ": " + showPrice + " " + currency;
    }
}
