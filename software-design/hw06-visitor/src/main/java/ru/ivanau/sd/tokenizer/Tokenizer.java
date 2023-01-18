package ru.ivanau.sd.tokenizer;

import ru.ivanau.sd.token.*;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Tokenizer {
    private static final Map<Character, Token> OPERATIONS = Map.of(
            '+', new OperationPlus(),
            '-', new OperationMinus(),
            '*', new OperationMult(),
            '/', new OperationDivide()
    );

    private static final Map<Character, Token> BRACKETS = Map.of(
            '(', new BraceLeft(),
            ')', new BraceRight()
    );

    private final BufferedReader reader;
    private State state;

    private interface State {
        void handle(final int ch, final List<Token> tokens);
    }

    private class StartState implements State {
        @Override
        public void handle(final int ch, final List<Token> tokens) {
            if (Character.isWhitespace(ch)) {
                // do nothing
            } else if (OPERATIONS.containsKey((char) ch)) {
                tokens.add(OPERATIONS.get((char) ch));
            } else if (BRACKETS.containsKey((char) ch)) {
                tokens.add(BRACKETS.get((char) ch));
            } else if (Character.isDigit(ch)) {
                state = numberState;
                state.handle(ch, tokens);
            } else if (ch == -1) {
                state = endState;
            } else {
                ((ErrorState) errorState).setMessage("Unknown token");
                state = errorState;
            }
        }
    }

    private class NumberState implements State {
        int value = 0;

        @Override
        public void handle(final int ch, final List<Token> tokens) {
            if (Character.isDigit(ch)) {
                value = 10 * value + (ch - '0');
            } else {
                tokens.add(new NumberToken(value));
                value = 0;
                state = startState;
                state.handle(ch, tokens);
            }
        }
    }
    private static class ErrorState implements State {
        private String message;

        public void setMessage(final String message) {
            this.message = message;
        }

        @Override
        public void handle(final int ch, final List<Token> tokens) {
            // do nothing
        }
    }
    private static class EndState implements State {
        @Override
        public void handle(final int ch, final List<Token> tokens) {
            // do nothing
        }
    }

    private final State startState = new StartState();
    private final State numberState = new NumberState();
    private final State errorState = new ErrorState();
    private final State endState = new EndState();

    public Tokenizer(final InputStream is) {
        this.reader = new BufferedReader(new InputStreamReader(is, StandardCharsets.UTF_8));
    }

    public List<Token> tokenize() throws IOException {
        state = startState;
        int ch;
        final List<Token> tokens = new ArrayList<>();
        while (state != endState && state != errorState) {
            ch = reader.read();
            state.handle(ch, tokens);
        }
        if (state == errorState) {
            throw new TokenizerException(((ErrorState) errorState).message);
        }
        return tokens;
    }
}
