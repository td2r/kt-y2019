package info.kgeorgiy.ja.Naumov.implementor;

import java.util.List;

public class Test24 {
    public interface AI<T> {
        T hello();
    }

    public interface BI<T> {
        T hello();
    }

    public static abstract class GenericClass<S, T extends S> implements AI<S>, BI<T> {}
}
