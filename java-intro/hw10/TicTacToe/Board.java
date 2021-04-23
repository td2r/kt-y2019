package TicTacToe;
/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public interface Board extends Position {
    Cell getTurn();
    int getPlayersNumber();
    Position getPosition();
    Result makeMove(Move move);
    Cell getCell(int r, int c);
}
