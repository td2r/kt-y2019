package info.kgeorgiy.ja.Naumov.concurrent;

import info.kgeorgiy.java.advanced.mapper.ParallelMapper;

import java.util.List;

public class CustomTest {
    public static void main(final String[] args) throws InterruptedException {
        final ParallelMapper pm = new ParallelMapperImpl(10);

        new Thread(() -> {
            try {
                System.out.println(pm.map(o -> {
                    int a = 0;
                    try {
                        Thread.sleep(5000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    return a;
                }, List.of(1, 2, 3, 4, 5)));
            } catch (final InterruptedException e) {
                e.printStackTrace();
            }
        }).start();


        Thread.sleep(1000);
        pm.close();
        System.out.println(Thread.activeCount());
    }
}
