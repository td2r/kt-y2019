module HW1.T6 where

import Data.Foldable

mcat :: Monoid a => [Maybe a] -> a
mcat = foldl (\r e -> mappend r (case e of { Just x -> x; Nothing -> mempty })) mempty

epart :: (Monoid a, Monoid b) => [Either a b] -> (a, b)
epart = foldl (\(r1, r2) e -> case e of { Left x -> (mappend r1 x, r2); Right x -> (r1, mappend r2 x)}) (mempty, mempty)
