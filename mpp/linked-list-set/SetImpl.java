package linked_list_set;

import kotlinx.atomicfu.AtomicRef;

public class SetImpl implements Set {
    private static class Node {
        AtomicRef<Node> next;
        int x;

        Node(final int x, final Node next) {
            this.next = new AtomicRef<>(next);
            this.x = x;
        }

        Node(final Node other) {
            x = other.x;
            next = other.next;
        }
    }

    private static class DeadNode extends Node {
        public DeadNode(final Node other) {
            super(other);
        }
    }

    private static class Window {
        Node cur, next;
    }

    private final AtomicRef<Node> head = new AtomicRef<>(new Node(Integer.MIN_VALUE, new Node(Integer.MAX_VALUE, null)));

    /**
     * Returns the {@link Window}, where cur.x < x <= next.x
     */
    private Window findWindow(final int x) {
        final Window w = new Window();
        mainLoop: while (true) {
            w.cur = head.getValue();
            w.next = w.cur.next.getValue();
            while (w.next.x < x) {
                final Node nnext = w.next.next.getValue();
                if (nnext instanceof DeadNode) {
                    final Node nnextAlive = new Node(nnext);
                    if (w.cur.next.compareAndSet(w.next, nnextAlive)) {
                        w.next = nnextAlive;
                    } else {
                        continue mainLoop;
                    }
                } else {
                    w.cur = w.next;
                    w.next = nnext;
                }
            }
            final Node nnext = w.next.next.getValue();
            if (nnext instanceof DeadNode) {
                w.cur.next.compareAndSet(w.next, new Node(nnext));
            } else {
                return w;
            }
        }
    }

    @Override
    public boolean add(final int x) {
        while (true) {
            final Window w = findWindow(x);
            if (w.next.x == x) {
                return false;
            }
            final Node node = new Node(x, w.next);
            if (w.cur.next.compareAndSet(w.next, node)) {
                return true;
            }
        }
    }

    @Override
    public boolean remove(final int x) {
        while (true) {
            final Window w = findWindow(x);
            if (w.next.x != x) {
                return false;
            }
            final Node nnext = w.next.next.getValue();
            if (nnext instanceof DeadNode) {
                continue;
            }
            final Node nnextDead = new DeadNode(nnext);
            if (w.next.next.compareAndSet(nnext, nnextDead)) {
                w.cur.next.compareAndSet(w.next, nnext);
                return true;
            }
        }
    }

    @Override
    public boolean contains(final int x) {
        return findWindow(x).next.x == x;
    }
}