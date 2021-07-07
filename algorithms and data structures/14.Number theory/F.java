import java.math.BigInteger;
import java.util.Scanner;

public class Task {
    public static class LongReference {
        public long value;

        public LongReference() {
            this(0);
        }

        public LongReference(long value) {
            this.value = value;
        }
    }

    private static long gcdex(final long a, final long b, final LongReference x, final LongReference y) {
        if (a == 0) {
            x.value = 0;
            y.value = 1;
            return b;
        }
        LongReference x1 = new LongReference();
        LongReference y1 = new LongReference();
        long d = gcdex(b % a, a, x1, y1);
        x.value = y1.value - (b / a) * x1.value;
        y.value = x1.value;
        return d;
    }

    public static long reverseElement(long a, long m) {
        LongReference x = new LongReference();
        LongReference y = new LongReference();
        long g = gcdex(a, m, x, y);
        return (x.value % m + m) % m;
    }

    public static void main(String[] args) {
        final Scanner scanner = new Scanner(System.in);
        int t = scanner.nextInt();
        while (t-- != 0) {
            long a = scanner.nextInt();
            long b = scanner.nextInt();
            long n = scanner.nextInt();
            long m = scanner.nextInt();
            BigInteger x = new BigInteger("0");
            BigInteger y = BigInteger.valueOf(a);
            BigInteger z = BigInteger.valueOf(b);
            x = x.add(y.multiply(BigInteger.valueOf(m)).multiply(BigInteger.valueOf(reverseElement(m, n))));
            x = x.add(z.multiply(BigInteger.valueOf(n)).multiply(BigInteger.valueOf(reverseElement(n, m))));
            System.out.println(x.mod(BigInteger.valueOf(n * m)));
        }
    }
}
