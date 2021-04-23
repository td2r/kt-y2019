%tree((Key, Value, Priority), Size, (Left_subtree, Right_subtree))
get_size(nil, 0).
get_size(tree(_, SIZE, _), SIZE).

recalc(tree((K, V, P), _, (L, R)), tree((K, V, P), SIZE, (L, R)))
	:- get_size(L, SIZE1), get_size(R, SIZE2), SIZE is SIZE1 + SIZE2 + 1.

%merge(Tree1, Tree2, Result).
merge(nil, T, T) :- !.
merge(T, nil, T) :- !.
merge(tree((LK, LV, LP), LSIZE, (LL, LR)), tree((RK, RV, RP), RSIZE, (RL, RR)), tree((LK, LV, LP), LSIZE + RSIZE, (LL, NR)))
	:- LP > RP, !,
	merge(LR, tree((RK, RV, RP), RSIZE, (RL, RR)), NR).
merge(tree((LK, LV, LP), LSIZE, (LL, LR)), tree((RK, RV, RP), RSIZE, (RL, RR)), tree((RK, RV, RP), LSIZE + RSIZE, (NL, RR)))
	:- LP =< RP, !,
	merge(tree((LK, LV, LP), LSIZE, (LL, LR)), RL, NL).

%split(Tree, Key, Result1, Result2).
split(nil, _, nil, nil) :- !.
split(tree((K, V, P), _, (L, R)), X, T1, T2)
	:- K < X, !,
	split(R, X, S1, S2),
	recalc(tree((K, V, P), _, (L, S1)), T1),
	T2 = S2.
split(tree((K, V, P), _, (L, R)), X, T1, T2)
	:- K >= X,  !,
	split(L, X, S1, S2),
	T1 = S1,
	recalc(tree((K, V, P), _, (S2, R)), T2).

%insert(Tree, (Key, Value, Priority), Result).
insert(nil, (K, V, P), tree((K, V, P), 1, (nil, nil))) :- !.
insert(tree((K, V, P), SIZE, (L, R)), (NK, NV, NP), tree((NK, NV, NP), SIZE + 1, (NL, NR)))
	:- NP > P, !,
	split(tree((K, V, P), SIZE, (L, R)), NK, NL, NR).
insert(tree((K, V, P), SIZE, (L, R)), (NK, NV, NP), tree((K, V, P), SIZE + 1, (NL, R)))
	:- NK < K, !,
	insert(L, (NK, NV, NP), NL).
insert(tree((K, V, P), SIZE, (L, R)), (NK, NV, NP), tree((K, V, P), SIZE + 1, (L, NR)))
	:- !,
	insert(R, (NK, NV, NP), NR).

map_remove(nil, _, nil) :- !.
map_remove(tree((K, _, _), _, (L, R)), K, T) :- !, merge(L, R, T).
map_remove(tree((K, V, P), _, (L, R)), X, T) :- X < K, !, map_remove(L, X, NL), recalc(tree((K, V, P), _, (NL, R)), T).
map_remove(tree((K, V, P), _, (L, R)), X, T) :- !, map_remove(R, X, NR), recalc(tree((K, V, P), _, (L, NR)), T).

map_put(TreeMap, Key, Value, Result)
	:-
	map_remove(TreeMap, Key, TreeTmp),
	rand_int(2147483647, Priority),
	insert(TreeTmp, (Key, Value, Priority), Result).

map_build([], nil) :- !.
map_build([(K, V) | T], Tree) :- !, map_build(T, TreeTmp), map_put(TreeTmp, K, V, Tree).

map_get(tree((K, V, _), _, _), K, V) :- !.
map_get(tree((TK, _, _), _, (L, _)), K, V) :- K < TK, !, map_get(L, K, V).
map_get(tree(_, _, (_, R)), K, V) :- !, map_get(R, K, V).

map_submapSize(Tree, FromKey, ToKey, Size)
	:- !,
	split(Tree, FromKey, _, T1),
	split(T1, ToKey, T2, _),
	get_size(T2, Size).

