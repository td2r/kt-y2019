package expression.parser;

import expression.exceptions.ParseException;

public class StringSource implements ExpressionSource {
    private final String data;
    private int pos;

    public StringSource(final String data) {
        this.data = data;
    }

    @Override
    public boolean hasNext() {
        return pos < data.length();
    }

    @Override
    public char next() {
        return data.charAt(pos++);
    }

    @Override
    public ParseException error(final String message) {
        return new ParseException(pos + ": " + message);
    }

    @Override
    public boolean startsWith(String expected) {
        if (data.startsWith(expected, pos)) {
            pos += expected.length();
            return true;
        } else {
            return false;
        }
    }
}
