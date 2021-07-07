import java.math.BigInteger;
import java.util.Arrays;
import java.util.Scanner;

public class Main {
    static final int MAXN = 100 + 3;
    static final int MAXS = 100 + 3;

    private static class Node {
        boolean term;
        int parent;
        int parent_code;
        int suffix;
        int[] trans;

        public Node() {
            term = false;
            parent = -1;
            suffix = -1;
            trans = new int[MAXN];
            Arrays.fill(trans, -1);
        }

        @Override
        public String toString() {
            StringBuilder stringBuilder = new StringBuilder(
                    String.format("node { parent %d, parent_code %d, suffix %d,", parent, parent_code, suffix)
            );
            for (int x = 0; x < MAXS; ++x) {
                if (trans[x] != -1) {
                    stringBuilder.append(String.format(" '%d': %d", x, trans[x]));
                }
            }
            stringBuilder.append(" }");
            return stringBuilder.toString();
        }
    }

    static int n;
    static int m;
    static int p;
    static int[] alphabet;
    static int nn = 0;
    static Node[] node = new Node[MAXS];

    static void readInput() {
        Scanner scanner = new Scanner(System.in);
        Scanner nmp = new Scanner(scanner.nextLine());
        n = nmp.nextInt();
        m = nmp.nextInt();
        p = nmp.nextInt();
        nmp.close();
        alphabet = new int[n];
        String alphabet_string = scanner.nextLine();
        for (int i = 0; i < alphabet_string.length(); ++i) {
            alphabet[i] = alphabet_string.charAt(i) - 33;
        }
        for (int i = 0; i < p; ++i) {
            addWord(scanner.nextLine());
        }
        scanner.close();
    }

    static void addWord(final String word) {
        int v = 1; // root
        for (int i = 0; i < word.length(); ++i) {
            int x = word.charAt(i) - 33;
            if (node[v].trans[x] == -1) {
                node[nn] = new Node();
                node[nn].parent = v;
                node[nn].parent_code = x;
                node[v].trans[x] = nn++;
            }
            v = node[v].trans[x];
        }
        node[v].term = true;
    }

    static void buildSuffixLink(int v) {
        int x = node[v].parent_code;
        int u = node[node[v].parent].suffix;
        while (node[u].trans[x] == -1) {
            u = node[u].suffix;
        }
        u = node[u].trans[x];
        node[v].suffix = u;
        node[v].term |= node[u].term;
    }

    static void buildSuffixLinks() {
        int[] q = new int[nn];
        q[0] = 1; // root
        int i = 0;
        int j = 1;
        while (i < j) {
            int v = q[i];
            for (int x : alphabet) {
                int u = node[v].trans[x];
                if (u != -1) {
                    buildSuffixLink(u);
                    q[j++] = u;
                }
            }
            ++i;
        }
    }

    static int nextBy(int v, int x) {
        while (node[v].trans[x] == -1) {
            v = node[v].suffix;
        }
        return node[v].trans[x];
    }

    static BigInteger solve() {
        BigInteger[] dp1 = new BigInteger[nn];
        Arrays.fill(dp1, BigInteger.ZERO);
        dp1[1] = BigInteger.ONE; // root
        BigInteger[] dp2 = new BigInteger[nn];
        Arrays.fill(dp2, BigInteger.ZERO);

        for (int i = 1; i <= m; ++i) {
            for (int v = 1; v < nn; ++v) {
                if (dp1[v].compareTo(BigInteger.ZERO) > 0) {
                    for (int x : alphabet) {
                        int u = nextBy(v, x);
                        if (!node[u].term) {
                            dp2[u] = dp2[u].add(dp1[v]);
                        }
                    }
                }
            }
            dp1 = dp2;
            dp2 = new BigInteger[nn];
            Arrays.fill(dp2, BigInteger.ZERO);
//            System.out.println("iteration " + i);
//            for (int v = 0; v < nn; ++v) {
//                if (dp1[v].compareTo(BigInteger.ZERO) > 0) {
//                    System.out.println("[" + v + "] = " + dp1[v]);
//                }
//            }
        }
        BigInteger result = BigInteger.ZERO;
        for (int v = 1; v < nn; ++v) {
            result = result.add(dp1[v]);
        }
        return result;
    }

    public static void main(String[] args) {
        node[nn++] = new Node();
        node[nn++] = new Node();
        Arrays.fill(node[0].trans, 1); // root
        node[1].suffix = 0; // root

        readInput();
        buildSuffixLinks();

        System.out.println(solve());
    }
}
