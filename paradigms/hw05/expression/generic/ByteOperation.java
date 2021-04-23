package expression.generic;

import expression.exceptions.DivisionByZeroException;

public class ByteOperation implements Operation<Byte> {
    @Override
    public Byte add(Byte x, Byte y) {
        return (byte) (x + y);
    }

    @Override
    public Byte sub(Byte x, Byte y) {
        return (byte) (x - y);
    }

    @Override
    public Byte mul(Byte x, Byte y) {
        return (byte) (x * y);
    }

    @Override
    public Byte div(Byte x, Byte y) {
        if (y == 0) {
            throw new DivisionByZeroException("dividend = " + x);
        }
        return (byte) (x / y);
    }

    @Override
    public Byte neg(Byte x) {
        return (byte) -x;
    }

    @Override
    public Byte parseValue(String value) {
        return (byte) Integer.parseInt(value);
    }

    @Override
    public Byte count(Byte x) {
        return (byte) Integer.bitCount(x & 0xff);
    }

    @Override
    public Byte min(Byte x, Byte y) {
        return (byte) Integer.min(x, y);
    }

    @Override
    public Byte max(Byte x, Byte y) {
        return (byte) Integer.max(x, y);
    }
}
