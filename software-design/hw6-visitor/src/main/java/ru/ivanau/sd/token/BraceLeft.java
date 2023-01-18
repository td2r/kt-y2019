package ru.ivanau.sd.token;

import ru.ivanau.sd.visitor.TokenVisitor;

public class BraceLeft implements Token {
    @Override
    public void accept(final TokenVisitor visitor) {
        visitor.visit(this);
    }
}
