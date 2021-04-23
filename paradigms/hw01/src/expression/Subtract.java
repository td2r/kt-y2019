package expression;

public class Subtract extends BinaryOperation {
    public Subtract(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return true;
    }

    @Override
    protected int calculate(int a, int b) {
        return a - b;
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
