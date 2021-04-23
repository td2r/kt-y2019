package TicTacToe;

public class TicTacToePosition implements Position {
    private final TicTacToeBoard board;

    TicTacToePosition(final TicTacToeBoard board) {
        this.board = board;
    }

    @Override
    public int getN() {
        return board.getN();
    }

    @Override
    public int getM() {
        return board.getM();
    }

    @Override
    public Cell getCell(int r, int c) {
        return board.getCell(r, c);
    }

    @Override
    public boolean isValid(final Move move) {
        return board.isValid(move);
    }

    public String toString() {
        return board.toString();
    }
}
