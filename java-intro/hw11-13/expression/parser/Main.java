package expression.parser;

import expression.PriorityExpression;
import expression.TripleExpression;

public class Main {
    public static void main(String[] args) {
        Parser parser = new ExpressionParser();
        final TripleExpression expression = parser.parse("x");
        System.out.println(expression);
    }
}
