package expression;

public class Const<T> implements GenericExpression<T> {
    private final T value;

    public Const(final T value) {
        this.value = value;
    }

    public String toString() {
        return value.toString();
    }

    @Override
    public T evaluate(final T x, final T y, final T z) {
        return value;
    }
}
