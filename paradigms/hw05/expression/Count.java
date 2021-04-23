package expression;

import expression.generic.Operation;

public class Count<T> extends UnaryOperator<T> {
    public Count(GenericExpression<T> expression, Operation<T> operation) {
        super(expression, operation);
    }

    @Override
    protected T calculate(T x) {
        return operation.count(x);
    }

    @Override
    protected String getSymbol() {
        return "count ";
    }
}
