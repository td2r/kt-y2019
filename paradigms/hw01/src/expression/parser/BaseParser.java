package expression.parser;

import expression.exceptions.ParseException;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class BaseParser {
    private final ExpressionSource source;
    protected char ch, next;

    protected BaseParser(final ExpressionSource source) {
        this.source = source;
    }

    protected void nextChar() {
        ch = next;
        next = source.hasNext() ? source.next() : '\0';
    }

    protected void updateChar() {
        nextChar();
        nextChar();
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

    protected boolean test(char c1, char c2) {
        if (ch == c1 && next == c2) {
            updateChar();
            return true;
        }
        return false;
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