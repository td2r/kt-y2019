package info.kgeorgiy.ja.Naumov.hello;

public class NodeStack {
    private Node top;

    public NodeStack() {
        top = null;
    }

    public synchronized boolean isEmpty() {
        return top == null;
    }

    public synchronized void push(final Node node) {
        node.setNext(top);
        top = node;
    }

    public synchronized Node take() {
        final Node node = top;
        if (top != null) {
            top = top.getNext();
            node.setNext(null);
        }
        return node;
    }
}
