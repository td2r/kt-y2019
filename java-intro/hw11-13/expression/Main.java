package expression;

import expression.exceptions.CheckedPow2;
import expression.exceptions.EvaluateException;
import expression.parser.ExpressionParser;

public class Main {
    private static void test(final PriorityExpression expression, final int x) {
        try {
            System.out.println(expression.evaluate(x));
        } catch (EvaluateException e) {
            System.out.println(e.getMessage());
        }
    }

    public static void main(String[] args) {
        final PriorityExpression expression = new CheckedPow2(new Add(new Const(1), new Const(2)));
        System.out.println(expression.evaluate(100));
        System.out.println(expression.toMiniString());
    }
}
