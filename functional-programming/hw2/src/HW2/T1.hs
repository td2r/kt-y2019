module HW2.T1 where

-------------------- Debug --------------------------------------
--printList:: Show a => List a -> String
--printList (h :. t@(_ :. _)) = show h ++ ", " ++ printList t
--printList (h :. Nil)        = show h
--printList Nil               = ""
--
--instance Show a => Show (List a) where
--  show l = "[" ++ printList l ++ "]"
-------------------- Debug --------------------------------------

data Option a = None | Some a

mapOption:: (a -> b) -> (Option a -> Option b)
mapOption f = g
  where
    g (Some x) = Some $ f x
    g None     = None

data Pair a = P a a

mapPair:: (a -> b) -> (Pair a -> Pair b)
mapPair f = \(P x y) -> P (f x) (f y)

data Quad a = Q a a a a

mapQuad:: (a -> b) -> (Quad a -> Quad b)
mapQuad f = \(Q w x y z) -> Q (f w) (f x) (f y) (f z)

data Annotated e a = a :# e
infix 0 :#

mapAnnotated:: (a -> b) -> (Annotated e a -> Annotated e b)
mapAnnotated f = \(a :# e) -> f a :# e

data Except e a = Error e | Success a

mapExcept:: (a -> b) -> (Except e a -> Except e b)
mapExcept f = g
  where
    g (Success x) = Success $ f x
    g (Error e)   = Error e

data Prioritised a = Low a | Medium a | High a

mapPrioritised:: (a -> b) -> (Prioritised a -> Prioritised b)
mapPrioritised f = g
  where
    g (Low x)    = Low    $ f x
    g (Medium x) = Medium $ f x
    g (High x)   = High   $ f x

data Stream a = a :> Stream a
infixr 5 :>

mapStream:: (a -> b) -> (Stream a -> Stream b)
mapStream f = g
  where
    g (h :> t) = f h :> g t

data List a = Nil | a :. List a
infixr 5 :.

mapList:: (a -> b) -> (List a -> List b)
mapList f = g
  where
    g (h :. t) = f h :. g t
    g Nil      = Nil

data Fun i a = F (i -> a)

mapFun:: (a -> b) -> (Fun i a -> Fun i b)
mapFun f = \(F fun) -> F (f . fun)

data Tree a = Leaf | Branch (Tree a) a (Tree a)

mapTree:: (a -> b) -> (Tree a -> Tree b)
mapTree f = g
  where
    g (Branch l x r) = Branch (g l) (f x) (g r)
    g Leaf           = Leaf
