package expression.exceptions;

import expression.PriorityExpression;

public class CheckedNegate implements PriorityExpression {
    private final PriorityExpression expression;

    public CheckedNegate(final PriorityExpression expression) {
        this.expression = expression;
    }

    public static int checkOverflow(final int x, final String msg) {
        if (x == Integer.MIN_VALUE) {
            throw new OverflowException(msg);
        }
        return -x;
    }

    @Override
    public int evaluate(int x) {
        return checkOverflow(expression.evaluate(x), "Negative overflow");
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return checkOverflow(expression.evaluate(x, y, z), "Negative overflow");
    }

    @Override
    public int getPriority() {
        return 4;
    }

    @Override
    public boolean isBracketsRequired() {
        return true;
    }

    @Override
    public String toString() {
        return "-" + (expression.getPriority() == 1 ? "(" + expression.toString()  + ")" : expression.toString());
    }

    @Override
    public String toMiniString() {
        return "-" + (expression.getPriority() == 1 ? "(" + expression.toMiniString()  + ")" : expression.toMiniString());
    }
}
