package info.kgeorgiy.ja.Naumov.implementor;

public class TestForIdea {
    public static interface MyList<T1, T2>
    {}

    public interface GenericI1<T1, T2, T3> {
        MyList<? extends MyList<? extends T1, ? super T2>, ?> f1();
    }

    public interface GenericI2<T1, T2, T3> {
        MyList<? extends MyList<? extends T1, ? super T2>, ? extends MyList< ? super T3, ? extends Number>> f1();
    }

    public abstract static class GenericClass<T1, T2> implements GenericI1<Integer, Number, Number>, GenericI2<Number, Integer, Integer>
    {
        @Override
        public MyList<? extends MyList<? extends Integer, ? super Number>, ? extends MyList<? super Integer, ? extends Number>> f1() {
            return null;
        }
    }
}
