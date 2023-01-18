package ru.ivanau.sd.visitor;

import ru.ivanau.sd.token.*;

import java.util.ArrayList;
import java.util.List;

public class ParserVisitor implements TokenVisitor {
    private enum TokenType {
        OPEN_BRACKET,
        CLOSE_BRACKET,
        PLUSMINUS,
        MULDIV
    }

    private List<Token> stack;
    private List<TokenType> tokenTypes;

    private List<Token> result;

    @Override
    public void visit(final NumberToken token) {
        result.add(token);
    }

    @Override
    public void visit(final BraceLeft token) {
        stack.add(token);
        tokenTypes.add(TokenType.OPEN_BRACKET);
    }

    @Override
    public void visit(final BraceRight token) {
        while (!stack.isEmpty() && tokenTypes.get(tokenTypes.size() - 1) != TokenType.OPEN_BRACKET) {
            Token t = stack.remove(stack.size() - 1);
            tokenTypes.remove(tokenTypes.size() - 1);
            result.add(t);
        }
        if (stack.isEmpty()) {
            throw new ParserVisitorException("Unmatched bracket");
        }
        stack.remove(stack.size() - 1); // open bracket
        tokenTypes.remove(tokenTypes.size() - 1);
    }

    private void visitPlusMinus(final Token token) {
        while (!stack.isEmpty()) {
            TokenType tt = tokenTypes.get(tokenTypes.size() - 1);
            if (tt != TokenType.PLUSMINUS && tt != TokenType.MULDIV) {
                break;
            }
            result.add(stack.remove(stack.size() - 1));
            tokenTypes.remove(tokenTypes.size() - 1);
        }
        stack.add(token);
        tokenTypes.add(TokenType.PLUSMINUS);
    }

    @Override
    public void visit(final OperationPlus token) {
        visitPlusMinus(token);
    }

    @Override
    public void visit(final OperationMinus token) {
        visitPlusMinus(token);
    }

    private void visitMulDiv(final Token token) {
        while (!stack.isEmpty()) {
            TokenType tt = tokenTypes.get(tokenTypes.size() - 1);
            if (tt != TokenType.MULDIV) {
                break;
            }
            result.add(stack.remove(stack.size() - 1));
            tokenTypes.remove(tokenTypes.size() - 1);
        }
        stack.add(token);
        tokenTypes.add(TokenType.MULDIV);
    }

    @Override
    public void visit(final OperationMult token) {
        visitMulDiv(token);
    }

    @Override
    public void visit(final OperationDivide token) {
        visitMulDiv(token);
    }

    public List<Token> visit(final List<Token> tokens) {
        stack = new ArrayList<>();
        tokenTypes = new ArrayList<>();
        result = new ArrayList<>();
        for (Token token : tokens) {
            token.accept(this);
        }
        while (!stack.isEmpty()) {
            TokenType tt = tokenTypes.get(tokenTypes.size() - 1);
            if (tt == TokenType.OPEN_BRACKET || tt == TokenType.CLOSE_BRACKET) {
                throw new ParserVisitorException("Incorrect brackets sequence");
            }
            result.add(stack.remove(stack.size() - 1));
        }
        return result;
    }
}
