package expression;

import expression.generic.Operation;

public class Max<T> extends BinaryOperator<T> {
    public Max(GenericExpression<T> left, GenericExpression right, Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(T x, T y) {
        return operation.max(x, y);
    }

    @Override
    protected String getSymbol() {
        return " max ";
    }
}
