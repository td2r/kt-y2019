package TicTacToe;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public final class TicTacToeBoard implements Board {
    private static final Map<Cell, String> SYMBOLS = Map.of(
            Cell.X, "X",
            Cell.O, "O",
            Cell.H, "-",
            Cell.V, "|",
            Cell.E, "."
    );

    private static final List<Cell> turn = List.of(
            Cell.X, Cell.O, Cell.H, Cell.V
    );

    private final int n;
    private final int m;
    private final int k;
    private final int playersNumber;
    private long emptyNumber;
    private final Cell[][] cells;
    private int turnId;

    public TicTacToeBoard(final int n, final int m, final int k, final int playersNumber) {
        if (n <= 0 || m <= 0) {
            throw new IllegalArgumentException("Incorrect board size");
        }
        if (playersNumber < 2 || playersNumber > 4) {
            throw new IllegalArgumentException("Incorrect players number");
        }

        this.n = n;
        this.m = m;
        this.k = k;
        this.playersNumber = playersNumber;
        emptyNumber = n * m;
        cells = new Cell[n][m];
        for (Cell[] row : cells) {
            Arrays.fill(row, Cell.E);
        }
        turnId = 0;
    }

    @Override
    public int getN() {
        return n;
    }

    @Override
    public int getM() {
        return m;
    }

    @Override
    public int getPlayersNumber() {
        return playersNumber;
    }

    @Override
    public Position getPosition() {
        return new TicTacToePosition(this);
    }

    @Override
    public Cell getCell(int r, int c) {
        return cells[r][c];
    }

    @Override
    public Cell getTurn() {
        return turn.get(turnId);
    }

    private boolean correctCoordinate(int r, int c) {
        return 0 <= r && r < n && 0 <= c && c < m;
    }

    private int countInDirection(int x, int y, final int dx, final int dy) {
        final Cell cell = cells[x][y];
        x += dx;
        y += dy;
       int count = 0;
        while (correctCoordinate(x, y) && cells[x][y] == cell) {
            ++count;
            x += dx;
            y += dy;
        }
        return count;
    }

    private boolean checkLine(final int x, final int y, final int dx, final int dy) {
        return countInDirection(x, y, dx, dy) + 1 + countInDirection(x, y, -dx, -dy) >= k;
    }

    @Override
    public Result makeMove(final Move move) {
        if (!isValid(move)) {
            return Result.LOSE;
        }

        int x = move.getRow();
        int y = move.getColumn();

        cells[x][y] = turn.get(turnId);
        emptyNumber--;

        if (checkLine(x, y, -1, 0) || checkLine(x, y, -1, 1)
        || checkLine(x, y, 0, 1) || checkLine(x, y, 1, 1)) {
            return Result.WIN;
        }

        if (emptyNumber == 0) {
            return Result.DRAW;
        }

        turnId = (turnId + 1) % playersNumber;
        return Result.UNKNOWN;
    }

    @Override
    public boolean isValid(final Move move) {
        return move != null && 0 <= move.getRow() && move.getRow() < n
                && 0 <= move.getColumn() && move.getColumn() < m
                && cells[move.getRow()][move.getColumn()] == Cell.E
                && turn.get(turnId) == move.getValue();
    }

//    @Override
//    public String toString() {
//        final StringBuilder sb = new StringBuilder(" ");
//        for (int j = 0; j < m; ++j) {
//            sb.append(j);
//        }
//        for (int i = 0; i < n; ++i) {
//            sb.append("\n");
//            sb.append(i);
//            for (int j = 0; j < m; ++j) {
//                sb.append(SYMBOLS.get(cells[i][j]));
//            }
//        }
//        return sb.toString();
//    }

    private void completeToLength(final String s, final int len, final StringBuilder sb) {
        sb.append(" ".repeat(len - s.length()));
        sb.append(s);
    }

    @Override
    public String toString() {
        final int nl = String.valueOf(n - 1).length();
        final int ml = String.valueOf(m - 1).length() + 1;
        final StringBuilder sb = new StringBuilder(" ".repeat(nl));
        for (int i = 0; i < m; ++i) {
            completeToLength(String.valueOf(i), ml, sb);
        }
        sb.append('\n');
        for (int i = 0; i < n; ++i) {
            completeToLength(String.valueOf(i), nl, sb);
            for (int j = 0; j < m; ++j) {
                completeToLength(SYMBOLS.get(cells[i][j]), ml, sb);
            }
            sb.append('\n');
        }
        return sb.toString();
    }
}
