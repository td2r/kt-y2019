package ru.ivanau.sd.rxjava;

import ru.ivanau.sd.rxjava.server.RxNettyHttpServer;

public class Main {
    public static void main(String[] args) {
        new RxNettyHttpServer().start(8080);
    }
}
