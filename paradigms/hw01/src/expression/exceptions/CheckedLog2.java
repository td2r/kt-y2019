package expression.exceptions;

import expression.PriorityExpression;

public class CheckedLog2 implements PriorityExpression {
    private final PriorityExpression expression;

    public CheckedLog2(final PriorityExpression expression) {
        this.expression = expression;
    }

    private int calculate(int x) {
        if (x <= 0) {
            throw new EvaluateException("Nonpositive argument of log2");
        }
        int result = 0;
        while (x >= 2) {
            x >>= 1;
            result++;
        }
        return result;
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
        return "log2" + (expression.getPriority() == 1 ? "(" + expression.toString()  + ")" : " " + expression.toString());
    }

    @Override
    public String toMiniString() {
        return "log2" + (expression.getPriority() == 1 ? "(" + expression.toMiniString()  + ")" : " " + expression.toMiniString());
    }
}
