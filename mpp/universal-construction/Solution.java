/**
 * @author Naumov Ivan
 */
public class Solution implements AtomicCounter {
    final Node root = new Node(0);
    final ThreadLocal<Node> last = ThreadLocal.withInitial(() -> root);

    public int getAndAdd(final int x) {
        Node old, upd;
        do {
            old = last.get();
            upd = new Node(old.value + x);
            last.set(old.consensus.decide(upd));
        } while (upd != last.get());
        return old.value;
    }

    private static class Node {
        final int value;
        final Consensus<Node> consensus;

        Node(final int value) {
            this.value = value;
            consensus = new Consensus<>();
        }
    }
}
