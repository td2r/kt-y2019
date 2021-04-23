package expression;

import expression.generic.Operation;

public class Subtract<T> extends BinaryOperator<T> {
    public Subtract(final GenericExpression<T> left, final GenericExpression<T> right, final Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(final T x, final T y) {
        return operation.sub(x, y);
    }

    @Override
    protected String getSymbol() {
        return " - ";
    }
}
