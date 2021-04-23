package expression;

import expression.generic.Operation;

public class Min<T> extends BinaryOperator<T> {
    public Min(final GenericExpression<T> left, final GenericExpression right, final Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(T x, T y) {
        return operation.min(x, y);
    }

    @Override
    protected String getSymbol() {
        return " min ";
    }
}
