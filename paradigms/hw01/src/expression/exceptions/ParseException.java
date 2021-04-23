package expression.exceptions;

public class ParseException extends ExpressionException {
    public ParseException(final String message) {
        super(message);
    }

    public ParseException(final String message, final Throwable cause) {
        super(message, cause);
    }
}
