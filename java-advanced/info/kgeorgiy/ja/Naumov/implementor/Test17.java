package info.kgeorgiy.ja.Naumov.implementor;

import java.io.IOException;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.sql.SQLException;
import java.util.Arrays;
import java.util.List;

public class Test17 {
    public static class MyException extends Exception
    {}

    public static class MyException2 extends MyException
    {}

    public static interface I1 {
        void a() throws Exception;
    }

    public static interface I2 extends I1 {
        void a() throws IOException, Error;
    }

    public static interface I3 {
        void a() throws SQLException;
    }

    public static interface I4 <E extends Exception> extends I3 {
        void a() throws SQLException;
        void b() throws E;
    }

    public static abstract class GenericClass<E extends RuntimeException> implements I2, I4<E>
    {}
}
