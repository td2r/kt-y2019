package TicTacToe;

import java.io.PrintStream;
import java.util.Scanner;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class HumanPlayer implements Player {
    private final PrintStream out;
    private final Scanner in;

    public HumanPlayer(final PrintStream out, final Scanner in) {
        this.out = out;
        this.in = in;
    }

    public HumanPlayer() {
        this(System.out, new Scanner(System.in));
    }

    private int getNextInt(final Scanner scanner) {
        return scanner.hasNextInt() ? scanner.nextInt() : -1;
    }

    @Override
    public Move move(final Position position, final Cell cell) {
//        Board board = (Board)position;
//        board.makeMove(new Move(0, 0, Cell.X));
//        board.makeMove(new Move(0, 1, Cell.O));
        out.println(position);
        out.println("Input row and column:");
        while (in.hasNextLine()) {
            final Scanner line = new Scanner(in.nextLine());
            int r = getNextInt(line);
            int c = getNextInt(line);
            if (r != -1 && c != -1 && !line.hasNext()) {
                final Move move = new Move(r, c, cell);
                if (position.isValid(move)) {
                    return move;
                }
            }
            out.println("Wrong input, try again:");
        }
        return null;
    }
}
