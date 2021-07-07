package info.kgeorgiy.ja.Naumov.implementor;

import java.util.List;

public class Test1 {
    // subst(E) = R
    public interface AI<E> extends Comparable<Integer> {
        E AI_E();
    }

    // subst(T) = R
    public interface BI<T> extends AI<T> {
        T BI_T();
    }

    // subst(T1) = S
    // subst(T) = R
    public abstract static class A<T1, T> implements BI<List<? extends T>> {
        public abstract <S, S1> T A_T(S s);
        public abstract T1 A_T1();
    }

    // subst(T) = S
    // subst(S) = R
    public abstract static class B<T, S> extends A<T, List<? extends S>> {
        public abstract S B_S(T kek);
        public abstract T B_T();
    }

    public abstract static class GenericClass<R, S> extends B<S, R> {
        public abstract R GenericClass_R();
        public abstract S GenericClass_S();
    }
}
