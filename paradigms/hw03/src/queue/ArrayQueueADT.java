package queue;

// inv: Q = [E0, E1, ..., En-1] ⋀ n ≥ 0 ⋀ Ei != null
public class ArrayQueueADT {
    private int size;
    private int head;
    private Object[] elements = new Object[8];

    // pre: E != null
    // post: Q' = [E0, E1, ..., En-1, E]
    public static void enqueue(final ArrayQueueADT queue, final Object element) {
        assert element != null;

        ensureCapacity(queue);
        queue.elements[(queue.head + queue.size) % queue.elements.length] = element;
        queue.size++;
    }

    // pre:  n > 0
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = E0
    public static Object element(final ArrayQueueADT queue) {
        assert queue.size > 0;

        return queue.elements[queue.head];
    }

    // pre: n > 0
    // post: Q' = [E1, E2, ..., En-1] ⋀ R = E0
    public static Object dequeue(final ArrayQueueADT queue) {
        assert queue.size > 0;

        final Object element = queue.elements[queue.head];
        queue.elements[queue.head] = null;
        queue.head = (queue.head + 1) % queue.elements.length;
        queue.size--;
        return element;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = n
    public static int size(final ArrayQueueADT queue) {
        return queue.size;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = (n == 0)
    public static boolean isEmpty(final ArrayQueueADT queue) {
        return queue.size == 0;
    }

    // pre: inv
    // post: Q' = []
    public static void clear(final ArrayQueueADT queue) {
        queue.head = queue.size = 0;
        queue.elements = new Object[8];
    }

    // pre: E != null
    // post: Q' = [E, E0, E1, ..., En-1]
    public static void push(final ArrayQueueADT queue, final Object element) {
        assert element != null;

        ensureCapacity(queue);
        queue.head = (queue.head - 1 + queue.elements.length) % queue.elements.length;
        queue.elements[queue.head] = element;
        queue.size++;
    }

    // pre: n > 0
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = En-1
    public static Object peek(final ArrayQueueADT queue) {
        assert queue.size > 0;

        return queue.elements[(queue.head + queue.size - 1) % queue.elements.length];
    }

    // pre: n > 0
    // post: Q' = [E0, E1, ..., En-2] ⋀ R = En-1
    public static Object remove(final ArrayQueueADT queue) {
        assert queue.size > 0;

        final Object e = peek(queue);
        queue.elements[(queue.head + queue.size - 1) % queue.elements.length] = null;
        queue.size--;
        return e;
    }

    // pre: 0 ≤ i < n
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = Ei
    public static Object get(final ArrayQueueADT queue, final int i) {
        assert i < queue.size;

        return queue.elements[(queue.head + i) % queue.elements.length];
    }

    // pre: 0 ≤ i < n ⋀ E != null
    // post: [E0, E1, ..., Ei-1, E, Ei+1, ..., En-1]
    public static void set(final ArrayQueueADT queue, final int i, final Object element) {
        assert element != null;
        assert i < queue.size;

        queue.elements[(queue.head + i) % queue.elements.length] = element;
    }

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1]
    private static void ensureCapacity(final ArrayQueueADT queue) {
        if (queue.size == queue.elements.length) {
            Object[] new_elements = new Object[2 * queue.size];
            System.arraycopy(queue.elements, queue.head, new_elements, 0, queue.size - queue.head);
            System.arraycopy(queue.elements, 0, new_elements, queue.size - queue.head, queue.head);
            queue.head = 0;
            queue.elements = new_elements;
        }
    }
}
