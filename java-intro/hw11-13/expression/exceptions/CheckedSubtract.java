package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedSubtract extends BinaryOperation {
    public CheckedSubtract(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return true;
    }

    public static int checkOverflow(final int a, final int b, final String msg) {
        if (b < 0 && a > Integer.MAX_VALUE + b || b > 0 && a < Integer.MIN_VALUE + b) {
            throw new OverflowException(msg);
        }
        return a - b;
    }

    @Override
    protected int calculate(int a, int b) {
        return checkOverflow(a, b, "Subtract overflow");
    }

    @Override
    public int getPriority() {
        return 1;
    }

    @Override
    protected String getSymbol() {
        return " - ";
    }
}
