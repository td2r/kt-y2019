package ru.ivanau.sd.visitor;

import ru.ivanau.sd.token.*;

import java.util.List;

public interface TokenVisitor {
    void visit(NumberToken token);
    void visit(BraceLeft token);
    void visit(BraceRight token);
    void visit(OperationPlus token);
    void visit(OperationMinus token);
    void visit(OperationMult token);
    void visit(OperationDivide token);
}
