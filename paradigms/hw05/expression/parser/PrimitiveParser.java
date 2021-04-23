package expression.parser;

import expression.exceptions.ParseException;

public class PrimitiveParser {
    private final ExpressionSource source;
    protected char ch;

    protected PrimitiveParser(final ExpressionSource source) {
        this.source = source;
        nextChar();
    }

    protected void nextChar() {
        ch = source.hasNext() ? source.next() : '\0';
    }

    protected char getChar() {
        char ret = ch;
        nextChar();
        return ret;
    }

    protected String getToken() {
        final StringBuilder sb = new StringBuilder(String.valueOf(getChar()));
        while (Character.isLetterOrDigit(ch)) {
            sb.append(getChar());
        }
        return sb.toString();
    }

    protected boolean test(char expected) {
        if (ch == expected) {
            nextChar();
            return true;
        }
        return false;
    }

    protected boolean test(final String expected) {
        if (ch == expected.charAt(0) && source.startsWith(expected.substring(1))) {
            nextChar();
            return true;
        } else {
            return false;
        }
    }

    protected void expect(final char c) {
        if (ch != c) {
            throw error("Expected '" + c + "', found '" + ch + "'");
        }
        nextChar();
    }

    protected void expect(final String value) {
        for (char c : value.toCharArray()) {
            expect(c);
        }
    }

    protected ParseException error(final String message) {
        return source.error(message);
    }

    protected boolean between(final char from, final char to) {
        return from <= ch && ch <= to;
    }
}