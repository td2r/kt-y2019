import java.util.*;
import java.util.function.Function;
import java.util.stream.IntStream;

/**
 * @author Naumov Ivan t.me/ivanln 2021
 * Walker Scheme implementation
 * https://elementy.ru/problems/2263/Razdelyay_i_uravnivay
 */
public class Walker {
    private static List<Function<Random, Integer>> functions;
    private static Queue<Integer> shorter;
    private static Queue<Integer> longer;
    private static int[] a;
    private static int mean;

    private static void appoint(final int i) {
        if (a[i] < mean) {
            shorter.add(i);
        } else if (a[i] == mean) {
            functions.add(random -> i + 1);
        } else {
            longer.add(i);
        }
    }

    @SuppressWarnings("ConstantConditions")
    private static void build() {
        functions = new ArrayList<>();
        shorter = new ArrayDeque<>();
        longer = new ArrayDeque<>();

        IntStream.range(0, a.length).forEach(Walker::appoint);

        while (!shorter.isEmpty()) {
            final int i = shorter.poll();
            final int j = longer.poll();

            functions.add(random -> random.nextDouble() < (double) a[i] / mean ? i + 1 : j + 1);

            a[j] -= mean - a[i];
            appoint(j);
        }
    }

    public static void main(String[] args) {
        final int n;
        final int q;

        try (final Scanner scanner = new Scanner(System.in)) {
            n = scanner.nextInt();
            q = scanner.nextInt();

            a = new int[n];
            mean = 0;

            for (int i = 0, ai; i < n; ++i) {
                ai = scanner.nextInt();
                mean += ai;
                a[i] = n * ai;
            }
        }

        build();

        final Random random = new Random(System.currentTimeMillis());
        IntStream.range(0, q)
                .map(i -> (int) (n * random.nextDouble()))
                .mapToObj(functions::get)
                .map(f -> f.apply(random) + " ")
                .forEach(System.out::print);
        System.out.println();
    }
}
