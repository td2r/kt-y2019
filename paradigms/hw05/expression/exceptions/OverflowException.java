package expression.exceptions;

import expression.exceptions.EvaluateException;

public class OverflowException extends EvaluateException {
    public OverflowException(final String message) {
        super(message);
    }

    public OverflowException(final String mesage, final Throwable cause) {
        super(mesage, cause);
    }
}
