package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedAdd extends BinaryOperation {
    public CheckedAdd(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    public static int checkOverflow(final int a, final int b, final String msg) {
        if (a > 0 && b > Integer.MAX_VALUE - a || a < 0 && b < Integer.MIN_VALUE - a) {
            throw new OverflowException(msg);
        }
        return a + b;
    }

    @Override
    protected int calculate(int a, int b) {
        return checkOverflow(a, b, "Sum overflow");
    }

    @Override
    public int getPriority() {
        return 1;
    }

    @Override
    protected String getSymbol() {
        return " + ";
    }
}
