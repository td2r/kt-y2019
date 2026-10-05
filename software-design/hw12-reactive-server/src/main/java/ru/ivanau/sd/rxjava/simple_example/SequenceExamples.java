package ru.ivanau.sd.rxjava.simple_example;


import rx.Observable;

public class SequenceExamples {

    public static void main(String[] args) {
        Observable.
                just("Alexnder", "Mary", "Petr", "Timur", "Pavel", "Piter")
                .filter(s -> s.startsWith("P"))
                .map(String::toUpperCase)
                .take(2)
                .subscribe(System.out::println);
    }
}
