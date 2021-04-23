package TicTacToe;

/**
 * @author Georgiy Korneev (kgeorgiy@kgeorgiy.info)
 */
public class Main {
    public static void main(String[] args) {
        final int n = 5;
        final int m = 5;
        final int k = 4;
        final Player[] players = new Player[]{
                new HumanPlayer(),
                new RandomPlayer()
        };

        final Game game = new Game(true, players);
        int result = game.play(new TicTacToeBoard(n, m, k, players.length));
        System.out.println("Game result: " + result);
    }
}
