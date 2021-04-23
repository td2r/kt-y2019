package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedMultiply extends BinaryOperation {
    public CheckedMultiply(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    public static int checkOverflow(final int a, final int b, final String msg) {
        if (
                (a == -1 && b == Integer.MIN_VALUE) || (a == Integer.MIN_VALUE && b == -1)
                    || b > 0 && (a > Integer.MAX_VALUE / b || a < Integer.MIN_VALUE / b)
                    || b < -1 && (a < Integer.MAX_VALUE / b || a > Integer.MIN_VALUE / b)
        ) {
            throw new OverflowException(msg);
        }
        return a * b;
    }

    @Override
    protected int calculate(int a, int b) {
        return checkOverflow(a, b, "multiply overflow");
    }

    @Override
    public int getPriority() {
        return 2;
    }

    @Override
    protected String getSymbol() {
        return " * ";
    }
}
