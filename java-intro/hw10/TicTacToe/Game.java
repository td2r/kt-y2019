package TicTacToe;

import java.util.List;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class Game {
    private final boolean log;
    private final List<Player> players;

    public Game(final boolean log, final Player... players) {
        if (players.length < 2 || players.length > 4) {
            throw new IllegalArgumentException("Incorrect number of players");
        }
        this.log = log;
        this.players = List.of(players);
    }

    public int play(Board board) {
        if (board.getPlayersNumber() != players.size()) {
            throw new IllegalArgumentException("Incompatible numbers of players in board and game");
        }
        while (true) {
            int no = 1;
            for (Player player: players) {
                final int result = move(board, player, no++);
                if (result != -1) {
                    return result;
                }
            }
        }
    }

    private int move(final Board board, final Player player, final int no) {
        final Move move = player.move(board.getPosition(), board.getTurn());
        final Result result = board.makeMove(move);
        log("Player " + no + " move: " + move);
        log("Position:\n" + board);
        if (result == Result.WIN) {
            log("Player " + no + " won");
            return no;
        } else if (result == Result.LOSE) {
            log("Player " + no + " lose");
            return players.size() + 1;
        } else if (result == Result.DRAW) {
            log("Draw");
            return 0;
        } else {
            return -1;
        }
    }

    private void log(final String message) {
        if (log) {
            System.out.println(message);
        }
    }
}
