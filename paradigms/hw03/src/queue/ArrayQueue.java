package queue;

// inv: Q = [E0, E1, ..., En-1] ⋀ n ≥ 0 ⋀ Ei != null
public class ArrayQueue {
    private int head;
    private int size;
    private Object[] elements = new Object[8];

    // pre: E != null
    // post: Q' = [E0, E1, ..., En-1, E]
    public void enqueue(final Object element) {
        assert element != null;

        ensureCapacity();
        elements[(head + size) % elements.length] = element;
        size++;
    }

    // pre:  n > 0
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = E0
    public Object element() {
        assert size > 0;

        return elements[head];
    }

    // pre: n > 0
    // post: Q' = [E1, E2, ..., En-1] ⋀ R = E0
    public Object dequeue() {
        assert size > 0;

        final Object element = elements[head];
        elements[head] = null;
        head = (head + 1) % elements.length;
        size--;
        return element;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = n
    public int size() {
        return size;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = (n == 0)
    public boolean isEmpty() {
        return size == 0;
    }

    // pre: inv
    // post: Q' = []
    public void clear() {
        head = size = 0;
        elements = new Object[8];
    }

    // pre: E != null
    // post: Q' = [E, E0, E1, ..., En-1]
    public void push(final Object element) {
        assert element != null;

        ensureCapacity();
        head = (head - 1 + elements.length) % elements.length;
        elements[head] = element;
        size++;
    }

    // pre: n > 0
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = En-1
    public Object peek() {
        assert size > 0;

        return elements[(head + size - 1) % elements.length];
    }

    // pre: n > 0
    // post: Q' = [E0, E1, ..., En-2] ⋀ R = En-1
    public Object remove() {
        assert size > 0;

        final Object e = peek();
        elements[(head + size - 1) % elements.length] = null;
        size--;
        return e;
    }

    // pre: 0 ≤ i < n
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = Ei
    public Object get(final int i) {
        assert i < size;

        return elements[(head + i) % elements.length];
    }

    // pre: 0 ≤ i < n ⋀ E != null
    // post: [E0, E1, ..., Ei-1, E, Ei+1, ..., En-1]
    public void set(final int i, final Object element) {
        assert i < size;
        assert element != null;

        elements[(head + i) % elements.length] = element;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1]
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
