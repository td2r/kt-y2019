package expression.generic;

public interface Operation<T> {
    T add(T x, T y);
    T sub(T x, T y);
    T mul(T x, T y);
    T div(T x, T y);
    T neg(T x);
    T parseValue(final String value);

    T count(T x);
    T min(T x, T y);
    T max(T x, T y);
}
