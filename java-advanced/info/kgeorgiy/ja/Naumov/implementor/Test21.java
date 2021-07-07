package info.kgeorgiy.ja.Naumov.implementor;

import java.util.Collection;
import java.util.List;

public class Test21 {
    public static interface MyList<T1, T2>
    {}

    public interface GenericI1<T1, T2> {
        MyList<? super List<? extends Collection<T1>>, ? extends T2> f1();
        T2 f2();
    }

    public interface GenericI2<T1, T2> extends GenericI1<T1, T2> {
        T1 f3();
    }

    public interface GenericI3<T1, T2> {
        MyList<? super List<? extends Collection<T2>>, ? extends T2> f1();
    }

    public abstract static class GenericClass implements GenericI2<Number, Integer>, GenericI3<Integer, Number>
    {}
}