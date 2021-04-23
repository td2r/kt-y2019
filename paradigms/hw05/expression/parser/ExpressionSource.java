package expression.parser;

import expression.exceptions.ParseException;

public interface ExpressionSource {
    boolean hasNext();
    char next();
    ParseException error(final String message);
    boolean startsWith(final String expected);
}

