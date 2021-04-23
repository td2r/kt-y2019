package queue;

import java.util.function.Predicate;

public abstract class AbstractQueue implements Queue {
    protected int size;

    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public void removeIf(final Predicate<Object> predicate) {
        assert predicate != null;

        for (int i = size(); i != 0; --i) {
            final Object element = dequeue();
            if (!predicate.test(element)) {
                enqueue(element);
            }
        }
    }

    @Override
    public void retainIf(final Predicate<Object> predicate) {
        removeIf(predicate.negate());
    }

    private void dropWhileImpl(final Predicate<Object> predicate, boolean toDrop) {
        assert predicate != null;

        boolean flag = true;
        for (int i = size(); i != 0; --i) {
            final Object element = dequeue();
            flag &= predicate.test(element);
            if (toDrop ^ flag) {
                enqueue(element);
            }
        }
    }

    @Override
    public void takeWhile(final Predicate<Object> predicate) {
        dropWhileImpl(predicate, false);
    }

    @Override
    public void dropWhile(final Predicate<Object> predicate) {
        dropWhileImpl(predicate, true);
    }
}
