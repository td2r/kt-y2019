package expression;

import expression.generic.Operation;

public class Negate<T> extends UnaryOperator<T> {
    public Negate(GenericExpression<T> expression, Operation<T> operation) {
        super(expression, operation);
    }

    @Override
    protected T calculate(T x) {
        return operation.neg(x);
    }

    @Override
    protected String getSymbol() {
        return "-";
    }
}
