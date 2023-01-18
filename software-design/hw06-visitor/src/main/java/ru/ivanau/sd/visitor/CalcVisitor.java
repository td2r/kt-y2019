package ru.ivanau.sd.visitor;

import ru.ivanau.sd.token.*;

import java.util.ArrayList;
import java.util.List;

public class CalcVisitor implements TokenVisitor {
    private List<Integer> stack;

    @Override
    public void visit(final NumberToken token) {
        stack.add(token.getValue());
    }

    @Override
    public void visit(final BraceLeft token) {
        throw new CalcVisitorException("Brackets in Reverse Polish notation");
    }

    @Override
    public void visit(final BraceRight token) {
        throw new CalcVisitorException("Brackets in Reverse Polish notation");
    }

    private void check2ElementOnStack() {
        if (stack.size() < 2) {
            throw new CalcVisitorException("Not enough arguments for operation (req 2)");
        }
    }

    @Override
    public void visit(final OperationPlus token) {
        check2ElementOnStack();
        int b = stack.remove(stack.size() - 1);
        int a = stack.remove(stack.size() - 1);
        stack.add(a + b);
    }

    @Override
    public void visit(final OperationMinus token) {
        check2ElementOnStack();
        int b = stack.remove(stack.size() - 1);
        int a = stack.remove(stack.size() - 1);
        stack.add(a - b);
    }

    @Override
    public void visit(final OperationMult token) {
        check2ElementOnStack();
        int b = stack.remove(stack.size() - 1);
        int a = stack.remove(stack.size() - 1);
        stack.add(a * b);
    }

    @Override
    public void visit(final OperationDivide token) {
        check2ElementOnStack();
        int b = stack.remove(stack.size() - 1);
        if (b == 0) {
            throw new CalcVisitorException("Zero division");
        }
        int a = stack.remove(stack.size() - 1);
        stack.add(a / b);
    }

    public Integer visit(final List<Token> tokens) {
        stack = new ArrayList<>();
        for (Token token : tokens) {
            token.accept(this);
        }
        if (stack.size() != 1) {
            throw new CalcVisitorException("Excess numbers");
        }
        return stack.get(0);
    }
}
