module HW0.T5 where

import GHC.Natural (Natural)
import Control.Monad.Fix (fix)
import HW0.T3

--The following equations must hold:
--nToNum nz       ==  0
--nToNum (ns x)   ==  1 + nToNum x
--nToNum (nplus a b)   ==   nToNum a + nToNum b
--nToNum (nmult a b)   ==   nToNum a * nToNum b

type Nat a = (a -> a) -> a -> a

nz :: Nat a
nz = k i

ns :: Nat a -> Nat a
ns = s compose

nplus, nmult :: Nat a -> Nat a -> Nat a
nplus = s (k s) (s (k (s (k s) k)))
nmult = compose

nFromNatural :: Natural -> Nat a
nFromNatural = fix (\f x -> if x == 0 then k i else s compose (f (x - 1)))

nToNum :: Num a => Nat a -> a
nToNum x = x (+1) 0
