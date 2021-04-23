import java.util.Arrays;
import java.lang.Long;
import java.lang.Integer;
import java.io.*;

public class ReverseSort {
    private static class Line implements Comparable<Line> {
        int[] a;
        int size;
        long sum;
        int index;

        Line(int index) {
            a = new int[1];
            this.index = index;
        }

        public void add(int x) {
            if (size == a.length) {
                a = Arrays.copyOf(a, 2 * size);
            }
            a[size++] = x;
            sum += x;
        }

        public void shrinkToFit() {
            if (size != a.length) {
                a = Arrays.copyOf(a, size);
            }
            Arrays.sort(a);
        }

        public int compareTo(Line l) {
            if (sum != l.sum) {
                return Long.valueOf(l.sum).compareTo(sum);
            }
            return Integer.valueOf(l.index).compareTo(index);
        }
    }

    public static void main(String[] args) {
        int n = 0;
        Line[] line = new Line[1];
        try {
            FastScanner in = new FastScanner(System.in);
            try {
                while (in.hasNextLine()) {
                    if (n == line.length) {
                        line = Arrays.copyOf(line, 2 * n);
                    }
                    line[n] = new Line(n);
                    while (in.hasNext(true)) {
                        line[n].add(in.nextInt(true));
                    }
                    in.skipLine();
                    line[n].shrinkToFit();
                    n++;
                }
            } finally {
                in.close();
            }
        } catch (IOException e) {
            System.out.println("Input file reading error: " + e.getMessage());
        }

        line = Arrays.copyOf(line, n);
        Arrays.sort(line);

        for (int i = 0; i < n; i++) {
            for (int j = line[i].size - 1; j >= 0; j--) {
                System.out.print(line[i].a[j] + " ");
            }
            System.out.println();
        }
    }
}
