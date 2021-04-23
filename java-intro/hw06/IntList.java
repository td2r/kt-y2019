import java.lang.StringBuilder;
import java.util.Arrays;

public class IntList {
    private int size;
    private int[] a = new int[1];

    public int size() {
        return size;
    }

    public void append(final int x) {
        if (size == a.length) {
            a = Arrays.copyOf(a, 2 * size);
        }
        a[size++] = x;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < size; i++) {
            if (i != 0) {
                sb.append(' ');
            }
            sb.append(a[i]);
        }
        return sb.toString();
    }
}
