package expression;

public class Const implements PriorityExpression {
    private final Integer c;

    public Const(final Integer c) {
        this.c = c;
    }

    @Override
    public int getPriority() {
        return 4;
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    @Override
    public int evaluate(int x) {
        return c;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return c;
    }

    @Override
    public String toString() {
        return c.toString();
    }

    @Override
    public String toMiniString() {
        return c.toString();
    }

    @Override
    public int hashCode() {
        return c.hashCode();
    }

    @Override
    public boolean equals(final Object obj) {
        if (obj != null && getClass() == obj.getClass()) {
            Const a = (Const)obj;
            return c.equals(a.c);
        }
        return false;
    }
}
