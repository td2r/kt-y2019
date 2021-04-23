package expression.parser;

import expression.*;
import expression.exceptions.*;

public class ExpressionParser implements Parser {
    private final static PriorityExpression VX = new Variable("x");
    private final static PriorityExpression VY = new Variable("y");
    private final static PriorityExpression VZ = new Variable("z");

    private BaseParser bp;

    public PriorityExpression parse(final String expression) {
        bp = new BaseParser(new StringSource(expression));
        bp.updateChar();

        PriorityExpression ret = parseSum();

        bp.expect('\0');
        return ret;
    }

    private PriorityExpression parseConst(final String starts) {
        final StringBuilder sb = new StringBuilder(starts);
        if (bp.test('0')) {
            sb.append('0');
        } else {
            while (bp.between('0', '9')) {
                sb.append(bp.getChar());
            }
        }
        try {
            return new Const(Integer.parseInt(sb.toString()));
        } catch (NumberFormatException e) {
            throw bp.error("Constant " + sb.toString() + " has wrong number format or overflow");
        }
    }

    private PriorityExpression parsePrimitive() {
        skipWhitespace();
        if (bp.between('0', '9')) {
            return parseConst("");
        } else if (bp.test('-')) {
            return bp.between('0', '9') ? parseConst("-") : new CheckedNegate(parsePrimitive());
        } else if (bp.test('(')) {
            final PriorityExpression result = parseSum();
            bp.expect(')');
            return result;
        } else {
            switch (bp.getToken()) {
                case "pow2":
                    return new CheckedPow2(parsePrimitive());
                case "log2":
                    return new CheckedLog2(parsePrimitive());
                case "x":
                    return VX;
                case "y":
                    return VY;
                case "z":
                    return VZ;
                default:
                    throw bp.error("Primitive expected");
            }
        }
    }

    private PriorityExpression parsePowLog() {
        PriorityExpression expression = parsePrimitive();

        while (true) {
            skipWhitespace();
            if (bp.test('*', '*')) {
                expression = new CheckedPow(expression, parsePrimitive());
            } else if (bp.test('/', '/')) {
                    expression = new CheckedLog(expression, parsePrimitive());
            } else {
                return expression;
            }
        }
    }

    private PriorityExpression parseMul() {
        PriorityExpression expression = parsePowLog();

        while (true) {
            //skipWhitespace();
            if (bp.test('*')) {
                expression = new CheckedMultiply(expression, parsePowLog());
            } else if (bp.test('/')) {
                expression = new CheckedDivide(expression, parsePowLog());
            } else {
                return expression;
            }
        }
    }

    private PriorityExpression parseSum() {
        PriorityExpression expression = parseMul();

        while (true) {
            //skipWhitespace();
            if (bp.test('+')) {
                expression = new CheckedAdd(expression, parseMul());
            } else if (bp.test('-')) {
                expression = new CheckedSubtract(expression, parseMul());
            } else {
                return expression;
            }
        }
    }
    private void skipWhitespace() {
        while (bp.test(' ') || bp.test('\r') || bp.test('\n') || bp.test('\t'));
    }
}
