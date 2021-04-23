package expression;

public class Divide extends BinaryOperation {
    public Divide(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return true;
    }

    @Override
    protected int calculate(int a, int b) {
        return a / b;
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
