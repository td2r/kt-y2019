package expression.generic;

import expression.exceptions.DivisionByZeroException;

public class UncheckedIntegerOperation implements Operation<Integer> {
    @Override
    public Integer add(Integer x, Integer y) {
        return x + y;
    }

    @Override
    public Integer sub(Integer x, Integer y) {
        return x - y;
    }

    @Override
    public Integer mul(Integer x, Integer y) {
        return x * y;
    }

    @Override
    public Integer div(Integer x, Integer y) {
        if (y == 0) {
            throw new DivisionByZeroException("dividend = " + x);
        }
        return x / y;
    }

    @Override
    public Integer neg(Integer x) {
        return -x;
    }

    @Override
    public Integer parseValue(String value) {
        return Integer.parseInt(value);
    }

    @Override
    public Integer count(Integer x) {
        return Integer.bitCount(x);
    }

    @Override
    public Integer min(Integer x, Integer y) {
        return Integer.min(x, y);
    }

    @Override
    public Integer max(Integer x, Integer y) {
        return Integer.max(x, y);
    }
}
