package search;

public class BinarySearchShift {
    // Pre: ∃! s in [0 : |a|) : (Ɐi != s - 1 in [0 : |a| - 1) : a[i] < a[i + 1] ⋀ (s == 0 ∨ a[s - 1] > a[s]))
    private static int iterative(final String[] a) {
        // pre: s in [0 : |a|)
        int l = -1;
        int r = a.length;
        int m = r / 2;
        // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2

        // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2
        final int e = a.length == 0 ? 0 : Integer.parseInt(a[a.length - 1]);
        // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2 ⋀ e < a[Ɐi < s]

        // pre(inv): -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2 ⋀ e < a[Ɐi < s]
        while (l + 1 != r) {
            // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2 ⋀ e < a[Ɐi < s] ⋀ l + 1 != r
            // => m in (l : r)
            if (e < Integer.parseInt(a[m])) {
                // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m in (l : r) ⋀ e < a[Ɐi < s] ⋀ e < a[m]
                // => m < s => s in (m : r] => l' = m
                l = m;
                // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s]
            } else {
                // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m in (l : r) ⋀ e < a[Ɐi < s] ⋀ e ≥ a[m]
                // => m ≥ s => s in (l : m] => r' = m
                r = m;
                // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s]
            }
            // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s]

            // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s]
            m = (l + r + 1) / 2;
            // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2 ⋀ e < a[Ɐi < s]
        }
        //         (1)            (2)                                          (3)
        // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ m == (l + r + 1) // 2 ⋀ e < a[Ɐi < s] ⋀ l + 1 == r;
        //                         (4)                  (5)
        //         (1) ⋀ (3) => s == r; (2) ⋀ (3) => m == r; (4) ⋀ (5) => m == s
        return m;
        // post: returned s
    }
    // Post: returned s

    // Pre: -1 ≤ l < r ≤ |a| ⋀
    //     (∃! s in (l : r] : Ɐi != s - 1 in (l : r - 1): a[i] < a[i + 1] ⋀ (s == 0 ∨ a[s - 1] > a[s])) ⋀ e < a[Ɐi < s]
    private static int recursive(final String[] a, final int l, final int r, final int e) {
        // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s]
        if (l + 1 == r) {
            // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s] ⋀ l + 1 == r
            // => s == r
            return r;
            // post: returned s
        } else {
            // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s] ⋀ l + 1 != r
            int m = (l + r + 1) / 2;
            // post: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s] ⋀ l + 1 != r ⋀ m == (l + r + 1) // 2 => m in (l : r)

            // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ e < a[Ɐi < s] ⋀ l + 1 != r ⋀ m == (l + r + 1) // 2 => m in (l : r)
            if (e < Integer.parseInt(a[m])) {
                // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m in (l : r) ⋀ e < a[Ɐi < s] ⋀ e < a[m] => m < s => s in (m : r]
                return recursive(a, m, r, e);
                // post: returned s
            } else {
                // pre: -1 ≤ l < s ≤ r ≤ |a| ⋀ m in (l : r) ⋀ e < a[Ɐi < s] ⋀ e ≥ a[m] => m ≥ s => s in (l : m]
                return recursive(a, l, m, e);
                // post: returned s
            }
            // post: returned s
        }
        // post: returned s
    }
    // Post: returned s

    // Pre: ∃! s in [0 : |a|) : (Ɐi [0 : |a| - 1), i != s - 1 : a[i] < a[i + 1] ⋀ (s == 0 ∨ a[s - 1] > a[s]))
    private static int recursive(final String[] a) {
        return recursive(a, -1, a.length, a.length == 0 ? 0 : Integer.parseInt(a[a.length - 1]));
    }
    // Post: returned s

    // Pre: ∃! s in [0 : |args|) : (Ɐi [0 : |args|), i != s - 1 : args[i] < args[i + 1] ⋀ (s == 0 ∨ args[s - 1] > args[s]))
    public static void main(String[] args) {
        // pre: ∃! s in [0 : |args|) : Ɐi [0 : |args|), i != s - 1 : args[i] < args[i + 1] ⋀ (s == 0 ∨ args[s - 1] > args[s])
        final int p = iterative(args);
        // post: p == s

        // pre: p == s
        System.out.println(p);
        // post: printed s
    }
    // Post: printed s

}
