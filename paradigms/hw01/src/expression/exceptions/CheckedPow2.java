package expression.exceptions;

import expression.PriorityExpression;

public class CheckedPow2 implements PriorityExpression {
    private final PriorityExpression expression;

    public CheckedPow2(final PriorityExpression expression) {
        this.expression = expression;
    }

    public static int checkOverflow(final int x, final String msg) {
        if (x > 30) {
            throw new OverflowException(msg);
        }
        return 1 << x;
    }

    private int calculate(int x) {
        if (x < 0) {
            throw new EvaluateException("Negative argument of pow2");
        }
        return checkOverflow(x, "Pow2 overflow");
    }

    @Override
    public int evaluate(int x) {
        return calculate(expression.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return calculate(expression.evaluate(x, y, z));
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
        return "pow2" + expression;
    }

    @Override
    public String toMiniString() {
        return "pow2" + (expression.getPriority() == 1 ? "(" + expression.toMiniString()  + ")" : " " + expression.toMiniString());
    }
}
