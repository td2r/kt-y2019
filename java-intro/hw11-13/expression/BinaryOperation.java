package expression;

public abstract class BinaryOperation implements PriorityExpression {
    private final PriorityExpression left;
    private final PriorityExpression right;

    protected BinaryOperation(final PriorityExpression left, final PriorityExpression right) {
        this.left = left;
        this.right = right;
    }

    protected abstract int calculate(int a, int b);
    protected abstract String getSymbol();

    @Override
    public int evaluate(int x) {
        return calculate(left.evaluate(x), right.evaluate(x));
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return calculate(left.evaluate(x, y, z), right.evaluate(x, y, z));
    }

    @Override
    public String toString() {
        return "(" + left + getSymbol() + right + ")";
    }

    private String toMiniString(final boolean brackets, final PriorityExpression expr) {
        return brackets ? "(" + expr.toMiniString() + ")" : expr.toMiniString();
    }

    @Override
    public String toMiniString() {
        return toMiniString(left.getPriority() < getPriority(), left) + getSymbol()
                + toMiniString(
                right.getPriority() < getPriority() ||
                right.getPriority() == getPriority() && (this.isBracketsRequired() || right.isBracketsRequired()),
                right
        );
    }

    @Override
    public int hashCode() {
        return 31 * 31 * left.hashCode() + 31 * right.hashCode() + getClass().hashCode();
    }

    @Override
    public boolean equals(final Object obj) {
        if (obj != null && getClass() == obj.getClass()) {
            BinaryOperation binaryOperation = (BinaryOperation) obj;
            return left.equals(binaryOperation.left) && right.equals(binaryOperation.right);
        }
        return false;
    }
}
