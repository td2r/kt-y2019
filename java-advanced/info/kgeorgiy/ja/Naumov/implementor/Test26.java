package info.kgeorgiy.ja.Naumov.implementor;

import java.util.List;

public class Test26 {
    public interface AI {
        List<? super Integer> a();
        List<? super List<? super Integer>> b();
        List<? super List<? super List<? super Integer>>> c();
    }

    public interface BI {
        List<? super Number> a();
        List<? super List<? super Number>> b();
        List<? super List<? super List<? super Number>>> c();
    }

    public static abstract class GenericClass<T, S, R extends S> implements AI, BI {}

    public static class GenericClassImpl<T, S, R extends S> extends GenericClass<T, S, R> {
        @Override
        public List<? super Number> a() {
            return null;
        }

        @Override
        public List<? super List<? super Integer>> b() {
            return null;
        }

        @Override
        public List<? super List<? super List<? super Number>>> c() {
            return null;
        }
    }
}
