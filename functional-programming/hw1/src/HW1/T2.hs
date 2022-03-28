module HW1.T2 where

import GHC.Natural (Natural)

data N = Z | S N

nplus :: N -> N -> N
nplus n Z     = n
nplus n (S m) = nplus (S n) m

nmult :: N -> N -> N
nmult _ Z     = Z
nmult n (S m) = nplus (nmult n m) n

nsub :: N -> N -> Maybe N
nsub Z Z         = Just Z
nsub Z _         = Nothing
nsub n Z         = Just n
nsub (S n) (S m) = nsub n m

ncmp :: N -> N -> Ordering
ncmp Z Z         = EQ
ncmp Z _         = LT
ncmp _ Z         = GT
ncmp (S n) (S m) = ncmp n m

nFromNatural :: Natural -> N
nFromNatural 0 = Z
nFromNatural n = S $ nFromNatural (n - 1)

nToNum :: Num a => N -> a
nToNum Z     = 0
nToNum (S n) = 1 + (nToNum n)

nEven, nOdd :: N -> Bool
nEven Z     = True
nEven (S n) = nOdd n
nOdd Z     = False
nOdd (S n) = nEven n

nDivMod :: N -> N -> (N, N)
nDivMod n m@(S _) =
  case nsub n m of
    Nothing -> (Z, n)
    Just d  -> let (q, r) = nDivMod d m in (S q, r)
nDivMod _ Z = undefined

ndiv :: N -> N -> N
ndiv n m = fst $ nDivMod n m

nmod :: N -> N -> N
nmod n m = snd $ nDivMod n m

--------------- Debug----------------------
instance Show N where
  show n = show $ nToNum n
--------------- Debug----------------------
