package ru.ivanau.sd.token;

import ru.ivanau.sd.visitor.TokenVisitor;

public class NumberToken implements Token {
    private final Integer value;

    public NumberToken(final Integer value) {
        this.value = value;
    }

    public Integer getValue() {
        return value;
    }

    @Override
    public void accept(final TokenVisitor visitor) {
        visitor.visit(this);
    }
}
