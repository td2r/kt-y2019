package info.kgeorgiy.ja.Naumov.implementor;

import java.lang.reflect.Method;
import java.math.BigDecimal;
import java.util.Arrays;
import java.util.List;

public class Test14 {
    public class A {}
    public class B extends A {}
    public class C extends B {}

    public interface GI1 {
        <T extends Number> A hello();
    }

    public interface GI2 {
        B hello();
    }

    public interface GI3 {
        C hello();
    }

    public static abstract class GenericClass implements GI1, GI2, GI3
    {}
}
