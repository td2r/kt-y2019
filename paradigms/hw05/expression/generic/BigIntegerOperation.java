package expression.generic;

import expression.exceptions.DivisionByZeroException;

import java.math.BigInteger;

public class BigIntegerOperation implements Operation<BigInteger> {
    @Override
    public BigInteger add(BigInteger x, BigInteger y) {
        return x.add(y);
    }

    @Override
    public BigInteger sub(BigInteger x, BigInteger y) {
        return x.subtract(y);
    }

    @Override
    public BigInteger mul(BigInteger x, BigInteger y) {
        return x.multiply(y);
    }

    @Override
    public BigInteger div(BigInteger x, BigInteger y) {
        if (y.equals(BigInteger.ZERO)) {
            throw new DivisionByZeroException("dividend = " + x);
        }
        return x.divide(y);
    }

    @Override
    public BigInteger neg(BigInteger x) {
        return x.negate();
    }

    @Override
    public BigInteger parseValue(String value) {
        return new BigInteger(value);
    }

    @Override
    public BigInteger count(BigInteger x) {
        return BigInteger.valueOf(x.bitCount());
    }

    @Override
    public BigInteger min(BigInteger x, BigInteger y) {
        return x.min(y);
    }

    @Override
    public BigInteger max(BigInteger x, BigInteger y) {
        return x.max(y);
    }
}
