package stack;

import kotlinx.atomicfu.AtomicRef;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class StackImpl implements Stack {
    private static final int ELIMINATION_TRIES = 200;
    private static final int BUFFER_SIZE = 100;
    private static final int RANGE_RADIUS = BUFFER_SIZE / 20;

    private static class Node {
        final AtomicRef<Node> next;
        final int x;

        Node(int x, Node next) {
            this.next = new AtomicRef<>(next);
            this.x = x;
        }
    }

    private final Random random;
    private final AtomicRef<Node> head;
    private final List<AtomicRef<Integer>> elimination;

    public StackImpl() {
        random = new Random(System.currentTimeMillis());
        head = new AtomicRef<>(null);
        elimination = new ArrayList<>(BUFFER_SIZE);
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            elimination.add(new AtomicRef<Integer>(null));
        }
    }

    @Override
    @SuppressWarnings({"deprecation", "UnnecessaryBoxing", "CachedNumberConstructorCall"})
    public void push(int x) {
        final Integer integer = new Integer(x); // no memoization: new x - new object
        int l = Math.max(random.nextInt(BUFFER_SIZE) - RANGE_RADIUS, 0);
        final int r = Math.min(l + 2 * RANGE_RADIUS + 1, BUFFER_SIZE);
        while (l < r) {
            if (elimination.get(l).compareAndSet(null, integer)) {
                int tries = ELIMINATION_TRIES;
                while (tries-- > 0) {
                    //noinspection NumberEquality
                    if (elimination.get(l).getValue() != integer)
                        return;
                }
                if (elimination.get(l).compareAndSet(integer, null)) {
                    pushImpl(x);
                }
                return;
            }
            l++;
        }
    }

    public void pushImpl(int x) {
        while (true) {
            final Node newHead = new Node(x, head.getValue());
            if (head.compareAndSet(newHead.next.getValue(), newHead))
                return;
        }
    }

    @Override
    public int pop() {
        int l = Math.max(random.nextInt(BUFFER_SIZE) - RANGE_RADIUS, 0);
        final int r = Math.min(l + 2 * RANGE_RADIUS + 1, BUFFER_SIZE);
        while (l < r) {
            int tries = ELIMINATION_TRIES;
            while (tries-- > 0) {
                final Integer integer = elimination.get(l).getValue();
                if (integer != null && elimination.get(l).compareAndSet(integer, null)) {
                    return integer;
                }
            }
            l++;
        }
        return popImpl();
    }

    public int popImpl() {
        while (true) {
            Node curHead = head.getValue();
            if (curHead == null)
                return Integer.MIN_VALUE;
            if (head.compareAndSet(curHead, curHead.next.getValue()))
                return curHead.x;
        }
    }
}
