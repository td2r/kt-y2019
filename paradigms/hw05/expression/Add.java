package expression;

import expression.generic.Operation;

public class Add<T> extends BinaryOperator<T> {
    public Add(final GenericExpression<T> left, final GenericExpression<T> right, final Operation<T> operation) {
        super(left, right, operation);
    }

    @Override
    protected T calculate(final T x, final T y) {
        return operation.add(x, y);
    }

    @Override
    protected String getSymbol() {
        return " + ";
    }
}
