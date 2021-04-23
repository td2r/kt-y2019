package expression;

public interface PriorityExpression extends CommonExpression {
    int getPriority();
    boolean isBracketsRequired();
}
