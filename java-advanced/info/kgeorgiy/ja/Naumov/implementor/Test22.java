package info.kgeorgiy.ja.Naumov.implementor;

import java.util.Collection;

public class Test22 {
    public static interface MyList<T1, T2>
    {}

    public interface GenericI1<T1, T2, T3> {
        MyList<? super MyList<? extends Collection<? super T1>, ? extends T2>, ? extends Double> f1();
    }

    public interface GenericI2<T1, T2, T3> {
        MyList<? super MyList<? extends Collection<? super T1>,  ? extends T2>, ? extends Number> f1();
    }

    public abstract static class GenericClass<T1, T2> implements GenericI1<Number, Number, Number>, GenericI2<Number, Number, Integer>
    {}

}
