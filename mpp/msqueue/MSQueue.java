package msqueue;

import kotlinx.atomicfu.AtomicRef;

public class MSQueue implements Queue {
    private static class Node {
        final int x;
        final AtomicRef<Node> next;

        Node(int x) {
            this.x = x;
            next = new AtomicRef<>(null);
        }
    }

    private final AtomicRef<Node> head;
    private final AtomicRef<Node> tail;

    public MSQueue() {
        Node dummy = new Node(0);
        head = new AtomicRef<>(dummy);
        tail = new AtomicRef<>(dummy);
    }

    @Override
    public void enqueue(int x) {
        final Node newTailNode = new Node(x);
        while (true) {
            final Node tailNode = tail.getValue();
            if (tailNode.next.compareAndSet(null, newTailNode)) {
                tail.compareAndSet(tailNode, newTailNode);
                return;
            } else {
                tail.compareAndSet(tailNode, tailNode.next.getValue());
            }
        }
    }

    @Override
    public int dequeue() {
        while (true) {
            final Node headNode = head.getValue();
            final Node nextNode = headNode.next.getValue();
            if (nextNode == null) {
                return Integer.MIN_VALUE;
            } else {
                tail.compareAndSet(headNode, nextNode);
                if (head.compareAndSet(headNode, nextNode)) {
                    return nextNode.x;
                }
            }
        }
    }

    @Override
    public int peek() {
        Node nextNode = head.getValue().next.getValue();
        return nextNode == null ? Integer.MIN_VALUE : nextNode.x;
    }
}