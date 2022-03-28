module HW1.T7 where

import Data.Semigroup (Semigroup ((<>)))

data ListPlus a = a :+ ListPlus a | Last a
infixr 5 :+

instance Semigroup (ListPlus a) where
  (<>) (Last x) rhl = x :+ rhl
  (<>) (h :+ t) rhl = h :+ (t <> rhl)


data Inclusive a b = This a | That b | Both a b

instance (Semigroup a, Semigroup b) => Semigroup (Inclusive a b) where
  (<>) (This a)   (This b)   = This $ a <> b
  (<>) (This a)   (That b)   = Both a b
  (<>) (This a)   (Both b c) = Both (a <> b) c
  (<>) (That a)   (This b)   = Both b a
  (<>) (That a)   (That b)   = That $ a <> b
  (<>) (That a)   (Both b c) = Both b (a <> c)
  (<>) (Both a b) (This c)   = Both (a <> c) b
  (<>) (Both a b) (That c)   = Both a (b <> c)
  (<>) (Both a b) (Both c d) = Both (a <> c) (b <> d)


newtype DotString = DS String deriving Show

instance Semigroup DotString where
  (<>) (DS "") ds      = ds
  (<>) ds (DS "")      = ds
  (<>) (DS s1) (DS s2) = DS $ s1 ++ ('.' : s2)

instance Monoid DotString where
  mappend = (<>)
  mempty = DS ""


newtype Fun a = F (a -> a)

instance Semigroup (Fun a) where
  (<>) (F f) (F g) = F $ f . g

instance Monoid (Fun a) where
  mappend = (<>)
  mempty = F id
