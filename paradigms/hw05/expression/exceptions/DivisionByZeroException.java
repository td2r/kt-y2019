package expression.exceptions;

public class DivisionByZeroException extends EvaluateException {
    public DivisionByZeroException(final String message) {
        super(message);
    }

    public DivisionByZeroException(final String message, final Throwable cause) {
        super(message, cause);
    }
}
