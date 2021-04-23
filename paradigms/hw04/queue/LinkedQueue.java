package queue;

import java.util.function.Predicate;

public class LinkedQueue extends AbstractQueue {
    private Node head;
    private Node tail;

    @Override
    public void enqueue(final Object element) {
        assert element != null;

        if (size == 0) {
            head = tail = new Node(element);
        } else {
//            tail = tail.next = new Node(element);
            tail.next = new Node(element);
            tail = tail.next;
        }
        size++;
    }

    @Override
    public Object element() {
        assert size > 0;

        return head.value;
    }

    @Override
    public Object dequeue() {
        assert size > 0;

        final Object element = head.value;
        if (size == 1) {
            head = tail = null;
        } else {
            head = head.next;
        }
        size--;
        return element;
    }

    @Override
    public void clear() {
        head = tail = null;
        size = 0;
    }

    private static class Node {
        private Object value;
        private Node next;

        public Node(final Object value, final Node next) {
            this.value = value;
            this.next = next;
        }

        public Node(final Object value) {
            this(value, null);
        }

        public void pullOut(final Node prev) {
            if (prev != null) {
                prev.next = next;
            }
            next = null;
        }
    }
}
