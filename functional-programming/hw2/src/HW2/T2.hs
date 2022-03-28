module HW2.T2 where

import HW2.T1

distOption      :: (Option a, Option b) -> Option (a, b)
distOption (Some x, Some y) = Some (x, y)
distOption (None, _)        = None
distOption (_, None)        = None

distPair        :: (Pair a, Pair b) -> Pair (a, b)
distPair (P x1 x2, P y1 y2) = P (x1, y1) (x2, y2)

distQuad        :: (Quad a, Quad b) -> Quad (a, b)
distQuad (Q x1 x2 x3 x4, Q y1 y2 y3 y4) = Q (x1, y1) (x2, y2) (x3, y3) (x4, y4)

distAnnotated   :: Semigroup e => (Annotated e a, Annotated e b) -> Annotated e (a, b)
distAnnotated (ax :# ex, ay :# ey) = (ax, ay) :# (ex <> ey)

distExcept      :: (Except e a, Except e b) -> Except e (a, b)
distExcept (Success x, Success y) = Success (x, y)
distExcept (Success _, Error y)   = Error y
distExcept (Error x, _)           = Error x

distPrioritised :: (Prioritised a, Prioritised b) -> Prioritised (a, b)
distPrioritised (Low x, Low y)       = Low (x, y)
distPrioritised (Low x, Medium y)    = Medium (x, y)
distPrioritised (Low x, High y)      = High (x, y)
distPrioritised (Medium x, Low y)    = Medium (x, y)
distPrioritised (Medium x, Medium y) = Medium (x, y)
distPrioritised (Medium x, High y)   = High (x, y)
distPrioritised (High x, Low y)      = High (x, y)
distPrioritised (High x, Medium y)   = High (x, y)
distPrioritised (High x, High y)     = High (x, y)

distStream      :: (Stream a, Stream b) -> Stream (a, b)
distStream (hx :> tx, hy :> ty) = (hx, hy) :> distStream (tx, ty)

distList        :: (List a, List b) -> List (a, b)
distList (hx :. tx, y) =  appendConjList hx y (distList (tx, y))
  where
    appendConjList x (yh :. yt) l = (x, yh) :. appendConjList x yt l
    appendConjList _ Nil l        = l
distList (Nil, _) = Nil

distFun         :: (Fun i a, Fun i b) -> Fun i (a, b)
distFun (F f, F g) = F $ \x -> (f x, g x)

wrapOption      :: a -> Option a
wrapOption = Some

wrapPair        :: a -> Pair a
wrapPair x = P x x

wrapQuad        :: a -> Quad a
wrapQuad x = Q x x x x

wrapAnnotated   :: Monoid e => a -> Annotated e a
wrapAnnotated a = a :# mempty

wrapExcept      :: a -> Except e a
wrapExcept = Success

wrapPrioritised :: a -> Prioritised a
wrapPrioritised = Low

wrapStream      :: a -> Stream a
wrapStream x = s
  where
    s = x :> s

wrapList        :: a -> List a
wrapList x = x :. Nil

wrapFun         :: a -> Fun i a
wrapFun = F . const
