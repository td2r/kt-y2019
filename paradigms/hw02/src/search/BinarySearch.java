package search;

// symbols: exists ∃, forall Ɐ, in ∊, and ⋀, leq ≤, greq ≥

public class BinarySearch {

    // pre: a[i - 1] ≥ a[i], Ɐi
    private static int findIterative(final int x, final int[] a) {
        // pre: a[i - 1] ≥ a[i], Ɐi
        int l = -1;
        // post: a[i - 1] ≥ a[i], Ɐi ⋀ l == -1

        // pre: a[i - 1] ≥ a[i], Ɐi ⋀ l == -1
        int r = a.length;
        // post: a[i - 1] ≥ a[i], Ɐi ⋀ l == -1 ⋀ r == |a|
        // => l < r ⋀ min(-1 < i < |a|, |a|) == min(l < i < r : a[i] ≥ x, r)

        // pre: a[i - 1] ≥ a[i], Ɐi ⋀ l < r ⋀ min(-1 < i < |a|, |a|) == min(l < i < r : a[i] ≥ x, r)
        int m = (l + r + 1) / 2;
        // post: a[i - 1] ≥ a[i], Ɐi ⋀ l < r ⋀ min(-1 < i < |a|, |a|) == min(l < i < r : a[i] ≥ x, r) ⋀
        //                                                             m == (l + r + 1) // 2

        // pre: a[i - 1] ≥ a[i], Ɐi ⋀ l < r ⋀ min(-1 < i < |a|, |a|) == min(l < i < r : a[i] ≥ x, r) ⋀
        //                                                             m == (l + r + 1) // 2
        while (l + 1 != r) {
            if (a[m] <= x) {
                r = m;
            } else {
                l = m;
            }
            m = (l + r + 1) / 2;
        }
        // pre: a[i - 1] ≥ a[i], Ɐi ⋀ l < r ⋀ min(-1 < i < |a|, |a|) == min(l < i < r : a[i] ≥ x, r) ⋀
        //                                                             m == (l + r + 1) // 2 ⋀ l + 1 == r
        // => min(l < i < r : a[i] ≥ x, r) == r       }
        //                                            } => m == r == min(l < i < r : a[i] ≥ x, r) == min(-1 < i < |a| : a[i] ≥ x, |a|)
        // => m == (l + r + 1) // 2 == (2r) // 2 == r }
        return m;
        // post: return t : t == min(-1 < i < |a| : a[i] ≥ x, r)
    }
    // post: return t : t == min(-1 < i < |a| : a[i] ≥ x, r)

    // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r
    private static int findRecursive(final int x, final int[] a, final int l, final int r) {
        // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r
        if (r - l == 1) {
            // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 == r
            // => (l + 1 : r] == ∅
            return r;
            // post: return t : t == min(l < i < r : a[i] ≥ x, r)
        } else {
            // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 != r
            // => Δ = r - l ≥ 2 => (2l + Δ + 1) // 2 > l ⋀ (2r - Δ + 1) // 2 < r => l < (l + r + 1) // 2 < r
            final int m = (l + r + 1) / 2;
            // post: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 != r ⋀ l < m < r

            // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 != r ⋀ l < m < r
            if (a[m] <= x) {
                // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 != r ⋀ l < m < r ⋀ a[m] ≤ x
                // => min(l < i ≤ r : a[i] ≥ x, r) == min(l < i ≤ m : a[i ≥ x)
                return findRecursive(x, a, l, m);
                // post: return t : t == min(l < i < r : a[i] ≥ x, r)
            } else {
                // pre: l < r ⋀ a[i - 1] ≥ a[i], l + 1 < Ɐi ≤ r ⋀ l + 1 != r ⋀ l < m < r ⋀ a[m] > x
                // => min(l < i ≤ r : a[i] ≥ x, r) == min(m < i ≤ r : a[i ≥ x, r)
                return findRecursive(x, a, m, r);
                // post: return t : t == min(l < i < r : a[i] ≥ x, r)
            }
            // post: return t : t == min(l < i < r : a[i] ≥ x, r)
        }
        // post: return t : t == min(l < i < r : a[i] ≥ x, r)
    }
    // post: return t : t == min(l < i < r : a[i] ≥ x, r)

    // pre: a[i - 1] ≥ a[i], Ɐi
    private static int findRecursive(final int x, final int[] a) {
        // pre: -1 < |a| ⋀ a[i - 1] ≥ a[i], -1 < Ɐi ≤ |a|
        return findRecursive(x, a, -1, a.length);
        // post: return t : t == min(-1 < i < |a| : a[i] ≥ x, r)
    }
    // post: return t : t == min(-1 < i < |a| : a[i] ≥ x, r)

    // pre: 0 < |args| ⋀ args[i] ∊ int, Ɐi ⋀ args[i - 1] ≥ args[i], 1 < Ɐi < |args|
    public static void main(String[] args) {
        // pre: 0 < |args| ⋀ args[i] ∊ int, Ɐi ⋀ args[i - 1] ≥ args[i], 1 < Ɐi < |args|
        final int[] a = new int[args.length - 1];
        // post: 0 ≤ |a| ⋀ |a| + 1 == |args| ⋀ args[i] ∊ int, Ɐi ⋀ args[i - 1] ≥ args[i], 1 < Ɐi < |args|

        // pre: 0 ≤ |a| ⋀ |a| + 1 == |args| ⋀ args[i] ∊ int, Ɐi ⋀ args[i - 1] ≥ args[i], 1 < Ɐi < |args|
        for (int i = 1; i < args.length; ++i) {
            a[i - 1] = Integer.parseInt(args[i]);
        }
        // post: a[i] == int(args[i + 1]), Ɐi ⋀ args[i] ∊ int, Ɐi ⋀ a[i - 1] ≥ a[i], Ɐi

        // pre: a[i] == int(args[i + 1]), Ɐi ⋀ args[i] ∊ int, Ɐi ⋀ a[i - 1] ≥ a[i], Ɐi
        final int p = findRecursive(Integer.parseInt(args[0]), a);
        // post: p == min(i : a[i] ≥ int(args[0]), |a|)

        // pre: p == min(i : a[i] ≥ x, |a|)
        System.out.println(p);
        // post: p == min(i : a[i] ≥ x, |a|)
    }

}
