package expression;

public interface GenericExpression<T> extends ToMiniString  {
    T evaluate(final T x, final T y, final T z);
}
