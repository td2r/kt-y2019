import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class SuffixTree {

    static final int ALPHABET = 27;

    static class Node {
        int depth;
        int start;
        Node parent;
        Node[] link;
        Node sufLink;

        public Node(int depth, int start, Node parent) {
            this.depth = depth;
            this.start = start;
            this.parent = parent;
            link = new Node[ALPHABET];
            sufLink = null;
        }
    }

    Node root;

    Node currentState;
    int L, R;
    int[] s;

    SuffixTree(String str) {
        this.s = new int[str.length() + 1];
        for (int i = 0; i < str.length(); i++) {
            this.s[i] = str.charAt(i) - 'a' + 1;
        }
        L = 0;
        R = 0;
        root = currentState = new Node(0, -1, null);
        root.sufLink = root;
        Node noSufLink = null;
        while (R < s.length) {
            while (L <= R) {
                boolean created = goDown(R);
                if (noSufLink != null) {
                    noSufLink.sufLink = currentState.parent;
                    noSufLink = null;
                }
                if (!created) {
                    break;
                }
                currentState = currentState.parent;
                if (currentState.sufLink == null) {
                    noSufLink = currentState;
                    currentState = currentState.parent;
                }
                currentState = currentState.sufLink;
                L++;
                while (currentState.depth < R - L) {
                    goDown(L + currentState.depth);
                }
            }
            R++;
        }
    }

    boolean goDown(int R) {
        int letter = s[R];
        if (currentState.depth != R - L) { // depth > R - L, currentState != root
            int edgeShift = R - L - currentState.parent.depth;
            int letterOnEdge = s[currentState.start + edgeShift];
            if (letterOnEdge != letter) {
                Node newNode = new Node(R - L, currentState.start, currentState.parent);
                currentState.parent.link[s[currentState.start]] = newNode;
                currentState.parent = newNode;
                currentState.start += edgeShift;
                newNode.link[letterOnEdge] = currentState;
                currentState = newNode;
            }
        } // 1. не зашли в if, 2. зашли, но не создали 3. зашли и создали newNode, currentState = newNode
        if (currentState.depth == R - L) {
            if (currentState.link[letter] == null) {
                Node newLeaf = new Node(s.length, R, currentState);
                currentState.link[letter] = newLeaf;
                currentState = newLeaf;
                return true;
            } else {
                currentState = currentState.link[letter];
            }
        }
        return false;
    }

    void dfsBSA(Node v, List<Integer> list, int parentDepth) {
        if (v.depth == s.length) {
            int edgeLength = s.length - v.start;
            int leafDepth = parentDepth + edgeLength;
            list.add(s.length - leafDepth);
            return;
        }
        for (Node to : v.link) {
            if (to != null) {
                dfsBSA(to, list, v.depth);
            }
        }
    }

    int[] buildSuffixArray() {
        List<Integer> listOfSuffixes = new ArrayList<>();
        dfsBSA(root, listOfSuffixes, -1);
        int[] suffixArray = new int[listOfSuffixes.size()];
        for (int i = 0; i < suffixArray.length; i++) {
            suffixArray[i] = listOfSuffixes.get(i);
        }
        return suffixArray;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        String s = in.next();
        SuffixTree tree = new SuffixTree(s);
        int[] suffixArray = tree.buildSuffixArray();
        PrintWriter out = new PrintWriter(System.out);
        for (int i = 0; i < suffixArray.length; i++) {
            if (i > 0) {
                out.print(' ');
            }
            out.print(suffixArray[i]);
        }
        out.println();
        out.close();
    }
}
