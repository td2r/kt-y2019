package queue;

import java.util.function.Predicate;

// inv: Q = [E0, E1, ..., En-1] ⋀ n ≥ 0 ⋀ Ei != null
public interface Queue {
    // pre: E != null
    // post: Q' = [E0, E1, ..., En-1, E]
    void enqueue(final Object element);

    // pre:  n > 0
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = E0
    Object element();

    // pre: n > 0
    // post: Q' = [E1, E2, ..., En-1] ⋀ R = E0
    Object dequeue();

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = n
    int size();

    // pre: inv
    // post: Q' = [E0, E1, ..., En-1] ⋀ R = (n == 0)
    boolean isEmpty();

    // pre: inv
    // post: Q' = []
    void clear();

    // pre: predicate != null
    // post: Q' = [Ei[0], Ei[1], ..., Ei[m-1]] : (∀ 1 <= j < m : i[j] > i[j - 1]  ⋀ predicate(Ei[j]) == false) ⋀
    //    predicate(Ei[0]) == false  ⋀ ∀ 0 <= j < n not in I : predicate(Ej) == true
    void removeIf(final Predicate<Object> predicate);

    // pre: predicate != null
    // post: Q' = [Ei[0], Ei[1], ..., Ei[m-1]] : (∀ 1 <= j < m : i[j] > i[j - 1]  ⋀ predicate(Ei[j]) == true) ⋀
    //     predicate(Ei[0]) == true  ⋀ ∀ 0 <= j < n not in I : predicate(Ej) == false
    void retainIf(final Predicate<Object> predicate);

    // pre: predicate != null
    // post: Q' = [E0, E1, ..., En'-1] : (∀ 0 <= i < n', predicate(Ei) == true ⋀ (n' == n ∨ predicate(En') == false))
    void takeWhile(final Predicate<Object> predicate);

    // pre: predicate != null
    // post: Q' = [Em, Em+1, ..., En-1] : (∀ 0 <= i < m, predicate(Ei) == true ⋀ (m == n ∨ predicate(Em) == false))
    void dropWhile(final Predicate<Object> predicate);
}
