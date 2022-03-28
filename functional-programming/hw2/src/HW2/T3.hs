module HW2.T3 where

import HW2.T1

joinOption    :: Option (Option a) -> Option a
joinOption (Some x) = x
joinOption None     = None

joinExcept    :: Except e (Except e a) -> Except e a
joinExcept (Success x) = x
joinExcept (Error x)   = Error x

joinAnnotated :: Semigroup e => Annotated e (Annotated e a) -> Annotated e a
joinAnnotated ((a :# e2) :# e1) = a :# (e1 <> e2)

joinList      :: List (List a) -> List a
joinList (h :. t) = appendList h (joinList t)
  where
    appendList (h1 :. t1) l2 = h1 :. appendList t1 l2
    appendList Nil l2        = l2
joinList Nil = Nil

joinFun       :: Fun i (Fun i a) -> Fun i a
joinFun (F f) = F $ \x -> case f x of F g -> g x
