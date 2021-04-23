package expression;

public class Variable<T> implements GenericExpression<T> {
    private final String variable;

    public Variable(final String variable) {
        this.variable = variable;
    }

    public String toString() {
        return variable;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        switch (variable) {
            case "x":
                return x;
            case "y":
                return y;
            case "z":
                return z;
            default:
                throw new AssertionError("Unknown variable name: " + variable);
        }
    }
}
