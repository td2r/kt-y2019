package expression;

import expression.generic.Operation;

public class Multiply<T> extends BinaryOperator<T> {
    public Multiply(final GenericExpression<T> left, final GenericExpression<T> right, final Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(final T x, final T y) {
        return operation.mul(x, y);
    }

    @Override
    protected String getSymbol() {
        return " * ";
    }
}
