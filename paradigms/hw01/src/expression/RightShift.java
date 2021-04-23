package expression;

public class RightShift extends BinaryOperation {
    public RightShift(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    @Override
    protected int calculate(int a, int b) {
        return a >> b;
    }

    @Override
    public int getPriority() {
        return 0;
    }

    @Override
    protected String getSymbol() {
        return " >> ";
    }
}
