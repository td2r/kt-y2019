package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedLog extends BinaryOperation {
    public CheckedLog(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    @Override
    protected int calculate(int a, int b) {
        if (a <= 0 || b <= 1) {
            throw new EvaluateException("Illegal log arguments");
        }
        int result = 0;
        while (a >= b) {
            a /= b;
            result++;
        }
        return result;
    }

    @Override
    public int getPriority() {
        return 3;
    }

    @Override
    protected String getSymbol() {
        return " // ";
    }
}
