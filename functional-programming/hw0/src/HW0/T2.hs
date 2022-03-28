module HW0.T2 where

import Data.Void (Void)
import Data.Function ((&))

type Not a = a -> Void

-- a -> Not (Not a)
-- a -> ((Not a) -> Void)
-- a -> ((a -> Void) -> Void)
-- a -> (a -> Void) -> Void
doubleNeg :: a -> Not (Not a)
doubleNeg = (&)

-- Not (Not (Not a)) -> Not a
-- Not (Not (Not a)) -> (a -> Void)
-- ((Not (Not a)) -> Void) -> (a -> Void)
-- (((Not a) -> Void) -> Void) -> (a -> Void)
-- (((a -> Void) -> Void) -> Void) -> (a -> Void)
-- (((a -> Void) -> Void) -> Void) -> a -> Void
reduceTripleNeg :: Not (Not (Not a)) -> Not a
reduceTripleNeg f x = f $ doubleNeg x
