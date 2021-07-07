package info.kgeorgiy.ja.Naumov.implementor;

public class Test18 {
    public static interface A<T> {
        <T1 extends Number> void hello();
    }

    public static interface B<T1> extends A<T1> {

    }

    public static abstract class GenericClass<T1> implements B<T1> {}
}
