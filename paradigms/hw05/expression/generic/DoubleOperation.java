package expression.generic;

public class DoubleOperation implements Operation<Double> {
    @Override
    public Double add(Double x, Double y) {
        return x + y;
    }

    @Override
    public Double sub(Double x, Double y) {
        return x - y;
    }

    @Override
    public Double mul(Double x, Double y) {
        return x * y;
    }

    @Override
    public Double div(Double x, Double y) {
        return x / y;
    }

    @Override
    public Double neg(Double x) {
        return -x;
    }

    @Override
    public Double parseValue(final String value) {
        return Double.parseDouble(value);
    }

    @Override
    public Double count(Double x) {
        return (double) Long.bitCount(Double.doubleToLongBits(x));
    }

    @Override
    public Double min(Double x, Double y) {
        return  Double.min(x, y);
    }

    @Override
    public Double max(Double x, Double y) {
        return Double.max(x, y);
    }
}
