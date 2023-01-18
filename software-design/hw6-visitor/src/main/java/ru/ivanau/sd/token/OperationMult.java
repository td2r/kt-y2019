package ru.ivanau.sd.token;

import ru.ivanau.sd.visitor.TokenVisitor;

public class OperationMult implements Token {
    @Override
    public void accept(final TokenVisitor visitor) {
        visitor.visit(this);
    }
}
