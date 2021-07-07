package info.kgeorgiy.ja.Naumov.implementor;

import java.util.List;
import java.util.Map;

public class Test20 {
    public interface GenericI1<T1, T2> {
        List<? super T1> f1();
        T2 f2();
    }

    public interface GenericI2<T1> extends GenericI1<T1, Number> {
        T1 f3();
    }

    public interface GenericI3<T1> {
        List<? super T1> f1();
    }

    public abstract static class GenericClass implements GenericI3<Integer>, GenericI2<Number>
    {}
}
