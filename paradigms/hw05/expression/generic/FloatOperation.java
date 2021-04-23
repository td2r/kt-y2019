package expression.generic;

public class FloatOperation implements Operation<Float> {
    @Override
    public Float add(Float x, Float y) {
        return x + y;
    }

    @Override
    public Float sub(Float x, Float y) {
        return x - y;
    }

    @Override
    public Float mul(Float x, Float y) {
        return x * y;
    }

    @Override
    public Float div(Float x, Float y) {
        return x / y;
    }

    @Override
    public Float neg(Float x) {
        return -x;
    }

    @Override
    public Float parseValue(String value) {
        return Float.parseFloat(value);
    }

    @Override
    public Float count(Float x) {
        return (float) Integer.bitCount(Float.floatToIntBits(x));
    }

    @Override
    public Float min(Float x, Float y) {
        return Float.min(x, y);
    }

    @Override
    public Float max(Float x, Float y) {
        return Float.max(x, y);
    }
}
