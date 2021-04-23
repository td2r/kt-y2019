package expression.parser;

import expression.*;
import expression.exceptions.ParseException;
import expression.generic.Operation;

public class ExpressionParser<T> {
    private PrimitiveParser bp;
    private final Operation<T> operation;

    public ExpressionParser(final Operation<T> operation) {
        this.operation = operation;
    }

    public GenericExpression<T> parse(final String expression) {
        if (operation == null) {
            throw new ParseException("null operation in parser");
        }
        bp = new PrimitiveParser(new StringSource(expression));
        final GenericExpression<T> result = parseMinMax();
        bp.expect('\0');
        return result;
    }

    private Const<T> parseConst(final String starts) {
        final StringBuilder sb = new StringBuilder(starts);
        while (bp.between('0', '9')) {
            sb.append(bp.getChar());
        }
        return new Const<>(operation.parseValue(sb.toString()));
    }

    private GenericExpression<T> parsePrimitive() {
        skipWhitespace();
        if (bp.between('0', '9')) {
            return parseConst("");
        } else if (bp.test('-')) {
            return bp.between('0', '9') ? parseConst("-") : new Negate<>(parsePrimitive(), operation);
        } else if (bp.test('(')) {
            final GenericExpression<T> result = parseMinMax();
            bp.expect(')');
            return result;
        } else {
            switch (bp.getToken()) {
                case "x":
                    return new Variable<>("x");
                case "y":
                    return new Variable<>("y");
                case "z":
                    return new Variable<>("z");
                case "count":
                    return new Count<>(parsePrimitive(), operation);
                default:
                    throw bp.error("Primitive expected");
            }
        }
    }

    private GenericExpression<T> parseMul() {
        GenericExpression<T> expression = parsePrimitive();

        while (true) {
            skipWhitespace();
            if (bp.test('*')) {
                expression = new Multiply<>(expression, parsePrimitive(), operation);
            } else if (bp.test('/')) {
                expression = new Divide<>(expression, parsePrimitive(), operation);
            } else {
                return expression;
            }
        }
    }

    private GenericExpression<T> parseSum() {
        GenericExpression<T> expression = parseMul();

        while (true) {
            //skipWhitespace();
            if (bp.test('+')) {
                expression = new Add<>(expression, parseMul(), operation);
            } else if (bp.test('-')) {
                expression = new Subtract<>(expression, parseMul(), operation);
            } else {
                return expression;
            }
        }
    }

    private GenericExpression<T> parseMinMax() {
        GenericExpression<T> expression = parseSum();

        while (true) {
            if (bp.test("min")) {
                expression = new Min<>(expression, parseSum(), operation);
            } else if (bp.test("max")) {
                expression = new Max<>(expression, parseSum(), operation);
            } else {
                return expression;
            }
        }
    }

    private void skipWhitespace() {
        while (bp.test(' ') || bp.test('\r') || bp.test('\n') || bp.test('\t'));
    }
}
