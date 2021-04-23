package expression;

import expression.generic.Operation;

public class Divide<T> extends BinaryOperator<T> {
    public Divide(final GenericExpression<T> left, final GenericExpression<T> right, final Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(final T a, final T b) {
        return operation.div(a, b);
    }

    @Override
    protected String getSymbol() {
        return " / ";
    }
}
