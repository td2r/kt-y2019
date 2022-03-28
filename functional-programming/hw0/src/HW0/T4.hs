module HW0.T4 where

import GHC.Natural (Natural)
import Data.Function (fix)

--repeat' = Data.List.repeat     -- No (obviously)
--repeat' x = x : repeat' x      -- No (explicit recursion disallowed)
--repeat' x = fix (x:)           -- OK

repeat' :: a -> [a]             -- behaves like Data.List.repeat
repeat' x = fix (x:)

map' :: (a -> b) -> [a] -> [b]  -- behaves like Data.List.map
map' f = fix (\m xs -> case xs of
                      [] -> []
                      xh : xt -> f xh : m xt)

fib :: Natural -> Natural       -- computes the n-th Fibonacci number
fib = snd . fix (\f x -> if x == 0 then (1, 0) else let (a0, b0) = f (x - 1) in (a0 + b0, a0))

fac :: Natural -> Natural       -- computes the factorial
fac = fix (\f x -> if x == 0 then 1 else x * f (x - 1))
