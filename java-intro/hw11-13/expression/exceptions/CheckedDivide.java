package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedDivide extends BinaryOperation {
    public CheckedDivide(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return true;
    }

    public static int checkOverflow(final int a, final int b, final String msg) {
        if (a == Integer.MIN_VALUE && b == -1) {
            throw new OverflowException(msg);
        }
        return a / b;
    }

    @Override
    protected int calculate(int a, int b) {
        if (b == 0) {
            throw new DivisionByZeroException("division by zero");
        }
        return checkOverflow(a, b, "Division overflow");
    }

    @Override
    public int getPriority() {
        return 2;
    }

    @Override
    protected String getSymbol() {
        return " / ";
    }
}
