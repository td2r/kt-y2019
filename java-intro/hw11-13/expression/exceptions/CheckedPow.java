package expression.exceptions;

import expression.BinaryOperation;
import expression.PriorityExpression;

public class CheckedPow extends BinaryOperation {
    public CheckedPow(final PriorityExpression left, final PriorityExpression right) {
        super(left, right);
    }

    @Override
    public boolean isBracketsRequired() {
        return false;
    }

    @Override
    protected int calculate(int a, int b) {
        if (b < 0 || a == 0 && b == 0) {
            throw new EvaluateException("Impossible integer power");
        }
        if (a == -1) {
            return b % 2 == 0 ? 1 : -1;
        } else if (a == 0 || a == 1) {
            return a;
        } else {
            int result  = 1;
            for (int i = 0; i < b; ++i) {
                result = CheckedMultiply.checkOverflow(result, a, "Power overflow");
            }
            return result;
        }
    }

    @Override
    public int getPriority() {
        return 3;
    }

    @Override
    protected String getSymbol() {
        return " ** ";
    }
}
