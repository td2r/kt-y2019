package ru.ivanau.sd.visitor;

import ru.ivanau.sd.token.*;

import java.io.BufferedOutputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.List;

public class PrintVisitor implements TokenVisitor {
    private final PrintStream out;

    public PrintVisitor(final OutputStream os) {
        this.out = new PrintStream(os);
    }

    @Override
    public void visit(final NumberToken token) {
        out.print(token.getValue());
    }

    @Override
    public void visit(final BraceLeft token) {
        out.print('(');
    }

    @Override
    public void visit(final BraceRight token) {
        out.print(')');
    }

    @Override
    public void visit(final OperationPlus token) {
        out.print('+');
    }

    @Override
    public void visit(final OperationMinus token) {
        out.print('-');
    }

    @Override
    public void visit(final OperationMult token) {
        out.print('*');
    }

    @Override
    public void visit(final OperationDivide token) {
        out.print('/');
    }

    public void visit(final List<Token> tokens) {
        for (Token token : tokens) {
            token.accept(this);
            out.print(' ');
        }
        out.println();
    }
}
