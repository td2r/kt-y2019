package queue;

import java.util.function.Predicate;

public class ArrayQueue extends AbstractQueue {
    private int head;
    private Object[] elements = new Object[8];

    @Override
    public void enqueue(final Object element) {
        assert element != null;

        ensureCapacity();
        elements[(head + size) % elements.length] = element;
        size++;
    }

    @Override
    public Object element() {
        assert size > 0;

        return elements[head];
    }

    @Override
    public Object dequeue() {
        assert size > 0;

        final Object element = elements[head];
        elements[head] = null;
        head = (head + 1) % elements.length;
        size--;
        return element;
    }

    @Override
    public void clear() {
        head = size = 0;
        elements = new Object[8];
    }

    private void ensureCapacity() {
        if (size == elements.length) {
            Object[] new_elements = new Object[2 * size];
            System.arraycopy(elements, head, new_elements, 0, size - head);
            System.arraycopy(elements, 0, new_elements, size - head, head);
            head = 0;
            elements = new_elements;
        }
    }
}
