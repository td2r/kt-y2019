package info.kgeorgiy.ja.Naumov.arrayset;

import java.util.*;

public class ArraySet<T> extends AbstractSet<T> implements NavigableSet<T> {
    private static class FastReverseList<T> extends AbstractList<T> {
        private final boolean reversed;
        private final List<T> list;

        FastReverseList(List<T> list) {
            if (list instanceof FastReverseList) {
                FastReverseList<T> fastReverseList = (FastReverseList<T>) list;
                reversed = !fastReverseList.reversed;
                this.list = fastReverseList.list;
            } else {
                reversed = true;
                this.list = list;
            }
        }

        @Override
        public T get(int index) {
            return reversed ? list.get(list.size() - index - 1) : list.get(index);
        }

        @Override
        public int size() {
            return list.size();
        }
    }

    private final List<T> data;
    private final Comparator<? super T> comparator;

    public ArraySet() {
        data = List.of();
        comparator = null;
    }

    private ArraySet(List<T> data, Comparator<? super T> comparator) {
        this.data = data;
        this.comparator = comparator;
    }

    public ArraySet(Collection<? extends T> collection) {
        this(collection, null);
    }

    public ArraySet(Collection<? extends T> collection, Comparator<? super T> comparator) {
        final TreeSet<T> treeSet = new TreeSet<>(comparator);
        treeSet.addAll(collection);
        data = new ArrayList<>(treeSet);
        this.comparator = comparator;
    }

    @Override
    public T lower(T t) {
        return getOrNull(lowerInd(t));
    }

    @Override
    public T floor(T t) {
        return getOrNull(floorInd(t));
    }

    @Override
    public T ceiling(T t) {
        return getOrNull(ceilingInd(t));
    }

    @Override
    public T higher(T t) {
        return getOrNull(higherInd(t));
    }

    @Override
    public T pollFirst() {
        throw new UnsupportedOperationException();
    }

    @Override
    public T pollLast() {
        throw new UnsupportedOperationException();
    }

    @Override
    public int size() {
        return data.size();
    }

    @Override
    public boolean isEmpty() {
        return data.isEmpty();
    }

    @SuppressWarnings("unchecked")
    @Override
    public boolean contains(Object o) {
        return Collections.binarySearch(data, (T) o, comparator) >= 0;
    }

    @Override
    public Iterator<T> iterator() {
        return Collections.unmodifiableList(data).iterator();
    }

    @Override
    public NavigableSet<T> descendingSet() {
        return new ArraySet<>(new FastReverseList<>(data), Collections.reverseOrder(comparator));
    }

    @Override
    public Iterator<T> descendingIterator() {
        return descendingSet().iterator();
    }

    @Override
    public NavigableSet<T> subSet(T fromElement, boolean fromInclusive, T toElement, boolean toInclusive) {
        if (fromElement == null || toElement == null || compare(fromElement, toElement)) {
            throw new IllegalArgumentException();
        }
        int fromInd = fromInclusive ? ceilingInd(fromElement) : higherInd(fromElement);
        int toInd = toInclusive ? floorInd(toElement) : lowerInd(toElement);
        if (fromInd > toInd + 1) { // incorrect range with correct given elements means empty range
            fromInd = 0;
            toInd = -1;
        }
        return new ArraySet<>(data.subList(fromInd, toInd + 1), comparator);
    }

    @SuppressWarnings("unchecked")
    private boolean compare(T fromElement, T toElement) {
        if (comparator != null) {
            return comparator.compare(fromElement, toElement) > 0;
        }
        return ((Comparable<T>) fromElement).compareTo(toElement) > 0;
    }

    @Override
    public NavigableSet<T> headSet(T toElement, boolean inclusive) {
        int toInd = inclusive ? floorInd(toElement) : lowerInd(toElement);
        return new ArraySet<>(data.subList(0, toInd + 1), comparator);
    }

    @Override
    public NavigableSet<T> tailSet(T fromElement, boolean inclusive) {
        int fromInd = inclusive ? ceilingInd(fromElement) : higherInd(fromElement);
        return new ArraySet<>(data.subList(fromInd, data.size()), comparator);
    }

    @Override
    public Comparator<? super T> comparator() {
        return comparator;
    }

    @Override
    public SortedSet<T> subSet(T fromElement, T toElement) {
        return subSet(fromElement, true, toElement, false);
    }

    @Override
    public SortedSet<T> headSet(T toElement) {
        return headSet(toElement, false);
    }

    @Override
    public SortedSet<T> tailSet(T fromElement) {
        return tailSet(fromElement, true);
    }

    @Override
    public T first() {
        return getOrNoSuchElem(0);
    }

    @Override
    public T last() {
        return getOrNoSuchElem(data.size() - 1);
    }

    private T getOrNoSuchElem(int i) {
        if (i < 0 || i >= data.size()) {
            throw new NoSuchElementException();
        } else {
            return data.get(i);
        }
    }

    private T getOrNull(int i) {
        if (i < 0 || i >= data.size()) {
            return null;
        } else {
            return data.get(i);
        }
    }

    private int abstractInd(T t, int d1, int d2) {
        int i = Collections.binarySearch(data, t, comparator);
        return (i < 0) ? -i + d1 : i + d2;
    }

    private int lowerInd(T t) {
        return abstractInd(t, -2, -1);
    }

    private int floorInd(T t) {
        return abstractInd(t, -2, 0);
    }

    private int ceilingInd(T t) {
        return abstractInd(t, -1, 0);
    }

    private int higherInd(T t) {
        return abstractInd(t, -1, 1);
    }
}
