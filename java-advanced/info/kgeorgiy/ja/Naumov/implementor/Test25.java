package info.kgeorgiy.ja.Naumov.implementor;

import java.util.Map;

public class Test25 {
    public interface AI<T> {
        T hello();
    }

    public interface BI {
        Map<?, ?> hello();
    }

    public static abstract class GenericClass<T extends Map<?, ?>> implements AI<T>, BI {}

    public interface T1 {}

    public interface A<E> {
        <T> T hello(T t);
        // T is already used, so T -> T1 and hello implementation looks like
        // <T1> T1 hello(T1 t);
        // which conflicts with interface T1
    }

    public static abstract class B<T> implements A<T> {}
}
