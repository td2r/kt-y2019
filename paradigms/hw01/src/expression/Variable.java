package expression;

public class Variable implements PriorityExpression {
    private final String variable;

    public Variable(final String variable) {
        this.variable = variable;
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
        return x;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        if (variable.equals("x")) {
            return x;
        } else if (variable.equals("y")) {
            return y;
        } else {
            return z;
        }
    }

    @Override
    public String toString() {
        return variable;
    }

    @Override
    public String toMiniString() {
        return variable;
    }

    @Override
    public int hashCode() {
        return variable.hashCode();
    }

    @Override
    public boolean equals(final Object obj) {
        if (obj != null && getClass() == obj.getClass()) {
            Variable v = (Variable)obj;
            return variable.equals(v.variable);
        }
        return false;
    }
}
