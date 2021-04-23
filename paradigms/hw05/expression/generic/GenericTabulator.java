package expression.generic;

import expression.GenericExpression;
import expression.exceptions.EvaluateException;
import expression.parser.ExpressionParser;

import java.util.Map;

public class GenericTabulator implements Tabulator {
    private static final Map<String, Operation<?>> operationByMode = Map.of(
        "i", new IntegerOperation(),
        "d", new DoubleOperation(),
        "bi", new BigIntegerOperation(),
        "u", new UncheckedIntegerOperation(),
        "f", new FloatOperation(),
        "b", new ByteOperation()
    );

    @Override
    public Object[][][] tabulate(final String mode, final String expression, final int x1, final int x2, final int y1, final int y2, final int z1, final int z2) throws Exception {
        return tabulateImpl(expression, x1, x2, y1, y2, z1, z2, operationByMode.get(mode));
    }

    // :NOTE: NPE
    private <T> Object[][][] tabulateImpl(final String str, final int x1, final int x2, final int y1, final int y2, final int z1, final int z2, final Operation<T> operation) {
        final ExpressionParser<T> parser = new ExpressionParser<>(operation);
        final GenericExpression<T> expr = parser.parse(str);
        final Object[][][] table = new Object[x2 - x1 + 1][y2 - y1 + 1][z2 - z1 + 1];
        for (int i = 0; i <= x2 - x1; ++i) {
            final T x = operation.parseValue(String.valueOf(x1 + i));
            for (int j = 0; j <= y2 - y1; ++j) {
                final T y = operation.parseValue(String.valueOf(y1 + j));
                for (int k = 0; k <= z2 - z1; ++k) {
                    try {
                        table[i][j][k] = expr.evaluate(x, y, operation.parseValue(String.valueOf(z1 + k)));
                    } catch (final EvaluateException ignored) {}
                }
            }
        }
        return table;
    }
}
