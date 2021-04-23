package expression.generic;

import expression.exceptions.*;

public class IntegerOperation implements Operation<Integer> {
    @Override
    public Integer add(final Integer x, final Integer y) {
        if (x > 0 && y > Integer.MAX_VALUE - x || x < 0 && y < Integer.MIN_VALUE - x) {
            throw new OverflowException(String.format("int overflow: %d + %d", x, y));
        }
        return x + y;
    }

    @Override
    public Integer sub(final Integer x, final Integer y) {
        return subWithCheck(x, y);
    }

    @Override
    public Integer mul(final Integer x, final Integer y) {
        return mulWithCheck(x, y);
    }

    @Override
    public Integer div(final Integer x, final Integer y) {
        if (y == 0) {
            throw new DivisionByZeroException("dividend = " + x);
        }
        return divWithCheck(x, y);
    }

    @Override
    public Integer neg(final Integer x) {
        return negWithCheck(x);
    }

    @Override
    public Integer parseValue(final String value) {
        return Integer.parseInt(value);
    }

    @Override
    public Integer count(final Integer x) {
        return Integer.bitCount(x);
    }

    @Override
    public Integer min(final Integer x, final Integer y) {
        return Integer.min(x, y);
    }

    @Override
    public Integer max(final Integer x, final Integer y) {
        return Integer.max(x, y);
    }

    private static Integer subWithCheck(final Integer x, final Integer y) {
        if (y < 0 && x > Integer.MAX_VALUE + y || y > 0 && x < Integer.MIN_VALUE + y) {
            throw new OverflowException(String.format("int overflow: %d - %d", x, y));
        }
        return x - y;
    }

    private static Integer mulWithCheck(final Integer x, final Integer y) {
        if (
                (x == -1 && y == Integer.MIN_VALUE) || (x == Integer.MIN_VALUE && y == -1)
                        || y > 0 && (x > Integer.MAX_VALUE / y || x < Integer.MIN_VALUE / y)
                        || y < -1 && (x < Integer.MAX_VALUE / y || x > Integer.MIN_VALUE / y)
        ) {
            throw new OverflowException(String.format("int overflow: %d * %d", x, y));
        }
        return x * y;
    }

    private static Integer divWithCheck(final Integer x, final Integer y) {
        if (x == Integer.MIN_VALUE && y == -1) {
            throw new OverflowException(String.format("int overflow: %d / %d", x, y));
        }
        return x / y;
    }

    private static Integer negWithCheck(final Integer x) {
        if (x == Integer.MIN_VALUE) {
            throw new OverflowException(String.format("int overflow: -(%d)", x));
        }
        return -x;
    }
}
