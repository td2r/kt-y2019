
min([A], A) :- !.
min([H | T], H) :- min(T, M), H =< M, !.
min([H | T], M) :- min(T, M), !.

max([A], A) :- !.
max([H | T], H) :- max(T, M), H >= M, !.
max([H | T], M) :- max(T, M), !.

%tree((Key, Value, Priority), (MinKey, MaxKey, Size), (Left_subtree, Right_subtree))

get_min(nil, 2147483647).
get_min(tree(_, (MIN, _, _), _), MIN).

get_max(nil, -2147483648).
get_max(tree(_, (_, MAX, _), _), MAX).

get_size(nil, 0).
get_size(tree(_, (_, _, SIZE), _), SIZE).

recalc(tree((K, V, P), _, (L, R)), tree((K, V, P), (MIN, MAX, SIZE), (L, R)))
	:- !,
	get_min(L, MIN1), get_min(R, MIN2), min([K, MIN1, MIN2], MIN),
	get_max(L, MAX1), get_max(R, MAX2), max([K, MAX1, MAX2], MAX),
	get_size(L, SIZE1), get_size(R, SIZE2), SIZE is SIZE1 + SIZE2 + 1.

%merge(Tree1, Tree2, Result).
merge(nil, T, T)
	:- !.
merge(T, nil, T)
	:- !.
merge(tree((LK, LV, LP), _, (LL, LR)), tree((RK, RV, RP), RDATA, (RL, RR)), T)
	:- LP > RP, !,
	merge(LR, tree((RK, RV, RP), RDATA, (RL, RR)), NR),
	recalc(tree((LK, LV, LP), _, (LL, NR)), T).

merge(tree((LK, LV, LP), LDATA, (LL, LR)), tree((RK, RV, RP), _, (RL, RR)), T)
	:- LP =< RP, !,
	merge(tree((LK, LV, LP), LDATA, (LL, LR)), RL, NL),
	recalc(tree((RK, RV, RP), _, (NL, RR)), T).

%split(Tree, Key, Result1, Result2).
split(nil, _, nil, nil)
	:- !.
split(tree((K, V, P), _, (L, R)), X, T1, T2)
	:- K =< X, !,
	split(R, X, S1, S2),
	recalc(tree((K, V, P), _, (L, S1)), T1),
	T2 = S2.
split(tree((K, V, P), _, (L, R)), X, T1, T2)
	:- K > X,  !,
	split(L, X, S1, S2),
	T1 = S1,
	recalc(tree((K, V, P), _, (S2, R)), T2).

%insert(Tree, (Key, Value, Priority), Result).
insert(nil, (K, V, P), tree((K, V, P), (K, K, 1), (nil, nil)))
	:- !.
insert(tree((K, V, P), (MIN, MAX, SIZE), (L, R)), (NK, NV, NP), tree((NK, NV, NP), (NMIN, NMAX, NSIZE), (NL, NR)))
	:- NP > P, !,
	min([MIN, NK], NMIN),
	max([MAX, NK], NMAX),
	NSIZE is SIZE + 1,
	split(tree((K, V, P), (MIN, MAX, SIZE), (L, R)), NK, NL, NR).
insert(tree((K, V, P), _, (L, R)), (NK, NV, NP), T)
	:- NK < K, !,
	insert(L, (NK, NV, NP), NL),
	recalc(tree((K, V, P), _, (NL, R)), T).
insert(tree((K, V, P), _, (L, R)), (NK, NV, NP), T)
	:- !,
	insert(R, (NK, NV, NP), NR),
	recalc(tree((K, V, P), _, (L, NR)), T).


replace(tree((K, V, P), DATA, CHILDREN), K, NV, tree((K, NV, P), DATA, CHILDREN))
	:- !.
replace(tree((K, V, P), DATA, (L, R)), NK, NV, tree((K, V, P), DATA, (NL, R)))
	:- NK < K, !,
	replace(L, NK, NV, NL).
replace(tree((K, V, P), DATA, (L, R)), NK, NV, tree((K, V, P), DATA, (L, NR)))
	:- NK >= K, !,
	replace(R, NK, NV, NR).


map_remove(nil, _, nil)
	:- !.
map_remove(tree((K, _, _), _, (L, R)), K, T)
	:- !,
	merge(L, R, T).
map_remove(tree((K, V, P), _, (L, R)), X, T)
	:- X < K, !,
	map_remove(L, X, NL),
	recalc(tree((K, V, P), _, (NL, R)), T).
map_remove(tree((K, V, P), _, (L, R)), X, T)
	:- !,
	map_remove(R, X, NR),
	recalc(tree((K, V, P), _, (L, NR)), T).


map_put(TreeMap, Key, Value, Result)
	:-
	(map_get(TreeMap, Key, _),
		!,
		replace(TreeMap, Key, Value, Result)
	;
		!,
		rand_int(2147483647, Priority),
		insert(TreeMap, (Key, Value, Priority), Result)
	).


map_build([], nil)
	:- !.
map_build([(K, V) | T], Tree)
	:- !,
	map_build(T, TreeTmp),
	map_put(TreeTmp, K, V, Tree).


map_get(tree((K, V, _), _, _), K, V)
	:- !.
map_get(tree((TK, _, _), _, (L, _)), K, V)
	:- K < TK, !,
	map_get(L, K, V).
map_get(tree(_, _, (_, R)), K, V)
	:- !,
	map_get(R, K, V).


map_submapSize(nil, _, _, 0)
	:- !.
map_submapSize(tree(_, (MIN, MAX, SIZE), _), FromKey, ToKey, SIZE)
	:- FromKey =< MIN, MAX < ToKey, !.
map_submapSize(tree(_, (MIN, MAX, _), _), FromKey, ToKey, 0)
	:- (MAX < FromKey ; ToKey =< MIN), !.
map_submapSize(tree((K, _, _), (MIN, MAX, _), (L, R)), FromKey, ToKey, Size)
	:- !,
	map_submapSize(L, FromKey, ToKey, Size1),
	map_submapSize(R, FromKey, ToKey, Size2),
	(FromKey =< K, K < ToKey,
		!,
		Size is Size1 + Size2 + 1
	;
		!,
		Size is Size1 + Size2
	).

