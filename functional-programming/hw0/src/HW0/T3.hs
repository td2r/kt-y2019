module HW0.T3 where

import Prelude()

s :: (a -> b -> c) -> (a -> b) -> (a -> c)
s f g x = f x (g x)

k :: a -> b -> a
k x _ = x

--Using only those combinators and function application
--(i.e. no lambdas, pattern matching, and so on)
--define the following additional combinators:

--i x = x         -- No (parameters on the LHS disallowed)
--i = \x -> x     -- No (lambdas disallowed)
--i = Prelude.id  -- No (only use s and k)
--i = s k k       -- OK
--i = (s k) k     -- OK (parentheses for grouping allowed)

i :: a -> a
i = s k k

compose :: (b -> c) -> (a -> b) -> (a -> c)
compose = s (k s) k

contract :: (a -> a -> b) -> (a -> b)
--contract f = s f i
--contract = flip s i
--so flip is permute
contract = s (s (k s) (s (s (k s) (s (k k) (k s))) k)) (k k) s (s k k)

permute :: (a -> b -> c) -> (b -> a -> c)
permute = s (s (k s) (s (s (k s) (s (k k) (k s))) k)) (k k)
