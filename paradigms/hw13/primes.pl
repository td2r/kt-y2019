
next_prime(N, Step, NextPrime) :- not composite(N), NextPrime is N, ! ; NextN is N + Step, next_prime(NextN, Step, NextPrime).
sift(N, D, MAXN) :- N =< MAXN, assertz(composite(N)), NextN is N + D, sift(NextN, D, MAXN).
sieve(N, MAXN) :- N * N =< MAXN, Nsquare is N * N, not sift(Nsquare, N, MAXN), Npp is N + 1, next_prime(Npp, 1, P), sieve(P, MAXN).
init(MAXN) :- not sieve(2, MAXN).

prime(N) :- not composite(N).
min_divisor(N, D, A) :- D * D =< N, (0 is N mod D, A is D, ! ; NextD is D + 1, min_divisor(N, NextD, A)).
prime_divisors(1, []) :- !.
prime_divisors(N, [N]) :- prime(N), !.
prime_divisors(N, [H | T]) :- number(N), !, min_divisor(N, 2, H), M is N / H, prime_divisors(M, T).
prime_divisors(N, [H, B | T]) :- not number(N), !, H =< B, prime(H), prime_divisors(X, [B | T]), N is X * H.

lcm_divisors([], F, F) :- !.
lcm_divisors([H | T1], [H | T2], [H | FL]) :- !, lcm_divisors(T1, T2, FL).
lcm_divisors([H1 | T1], [H2 | T2], [H1 | FL]) :- H1 < H2, !, lcm_divisors(T1, [H2 | T2], FL).
lcm_divisors(FA, FB, FL) :- !, lcm_divisors(FB, FA, FL).

lcm(A, B, LCM) :- prime_divisors(A, FA), prime_divisors(B, FB), lcm_divisors(FA, FB, FL), prime_divisors(LCM, FL).
