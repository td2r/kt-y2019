package expression;

import expression.generic.Operation;

public abstract class BinaryOperator<T> implements GenericExpression<T> {
    private final GenericExpression<T> left;
    private final GenericExpression<T> right;
    protected final Operation<T> operation;

    protected BinaryOperator(final GenericExpression<T> left, final GenericExpression right, final Operation<T> operation) {
        this.left = left;
        this.right = right;
        this.operation = operation;
    }

    protected abstract T calculate(final T x, final T y);
    protected abstract String getSymbol();

    @Override
    public T evaluate(final T x, final T y, final T z) {
        return calculate(left.evaluate(x, y, z), right.evaluate(x, y, z));
    }

    public String toString() {
        return "(" + left + getSymbol() + right + ")";
    }
}
