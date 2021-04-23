package TicTacToe;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public interface Position {
    int getN();
    int getM();
    Cell getCell(int r, int c);
    boolean isValid(Move move);
}
