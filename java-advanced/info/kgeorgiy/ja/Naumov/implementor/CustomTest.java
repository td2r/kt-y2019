package info.kgeorgiy.ja.Naumov.implementor;

import info.kgeorgiy.java.advanced.implementor.ImplerException;

import java.nio.file.Path;
import java.util.List;

public class CustomTest {
    public interface AI<E> {
        <T extends Comparable<? extends Integer>> List<? extends T> hello();
    }

    public interface BI<E> {
        <T extends Comparable<? extends Integer>> List<? extends T> hello();
    }

    static abstract class GenericClass<T> implements AI<T>, BI<T> {
    }

    static class GenericClassImpl<T extends Comparable<? extends Integer>, R> extends GenericClass<T> {
        @Override
        public List<? extends Object> hello() {
            return null;
        }
    }

    private static void implementTest() {
        try {
            new Implementor().implement(
                    Test1.GenericClass.class,
                    Path.of("/home/td2r/java-advanced-2021/modules/info.kgeorgiy.ja.Naumov.implementor"));
//                    Path.of("/home/td2r/Desktop/govno.jar"));
        } catch (ImplerException e) {
            System.out.println("Impler Exception occurred.");
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        implementTest();
    }
}
