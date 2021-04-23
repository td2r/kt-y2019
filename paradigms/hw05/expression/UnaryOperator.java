package expression;

import expression.generic.Operation;

public abstract class UnaryOperator<T> implements GenericExpression<T> {
    private final GenericExpression<T> expression;
    protected final Operation<T> operation;

    protected UnaryOperator(final GenericExpression<T> expression, final Operation<T> operation) {
        this.expression = expression;
        this.operation = operation;
    }

    protected abstract T calculate(final T x);
    protected abstract String getSymbol();

    @Override
    public T evaluate(final T x, final T y, final T z) {
        return calculate(expression.evaluate(x, y, z));
    }

    public String toString() {
        return getSymbol() + "(" + expression + ")";
    }
}
