package ru.ivanau.sd.token;

import ru.ivanau.sd.visitor.TokenVisitor;

public interface Token {
    void accept(TokenVisitor visitor);
}
