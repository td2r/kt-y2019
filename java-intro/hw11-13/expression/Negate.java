package expression;

import expression.exceptions.OverflowException;

public class Negate implements PriorityExpression {
    private final PriorityExpression expression;

    public Negate(final PriorityExpression expression) {
        this.expression = expression;
    }

    @Override
    public int evaluate(int x) {
        return -expression.evaluate(x);
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return -expression.evaluate(x, y, z);
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
