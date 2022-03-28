module HW2.T4 where

import Control.Monad
import Data.Ratio
import HW2.T1

data State s a = S { runS :: s -> Annotated s a }

mapState :: (a -> b) -> State s a -> State s b
mapState f s = S { runS = \c -> case runS s c of a :# e -> f a :# e }

wrapState :: a -> State s a
wrapState a = S { runS = (a :#) }

joinState :: State s (State s a) -> State s a
joinState s = S { runS = \c -> case runS s c of a :# e -> runS a e }

modifyState :: (s -> s) -> State s ()
modifyState f = S { runS = \c -> () :# f c }

instance Functor (State s) where
  fmap = mapState

instance Applicative (State s) where
  pure = wrapState
  p <*> q = Control.Monad.ap p q

instance Monad (State s) where
  m >>= f = joinState (fmap f m)

data Prim a =
    Add a a      -- (+)
  | Sub a a      -- (-)
  | Mul a a      -- (*)
  | Div a a      -- (/)
  | Abs a        -- abs
  | Sgn a        -- signum

data Expr = Val Double | Op (Prim Expr)

instance Num Expr where
  x + y         = Op (Add x y)
  x - y         = Op (Sub x y)
  x * y         = Op (Mul x y)
  abs x         = Op (Abs x)
  signum x      = Op (Sgn x)
  fromInteger x = Val (fromInteger x)

instance Fractional Expr where
  fromRational x = Val $ fromInteger (numerator x) / fromInteger (denominator x)
--  recip (Op (Div num denom)) = Op (Div denom num)
--  recip (Val x)              = Val (1 / x)
--  recip x                    = Op (Div (Val 1) x)
  x / y = Op (Div x y)

opArity :: Prim a -> Int
opArity prim = case prim of
  Abs _   -> 1
  Sgn _   -> 1
  Add _ _ -> 2
  Sub _ _ -> 2
  Mul _ _ -> 2
  Div _ _ -> 2

decomposeUnaryOp :: Prim a -> (a, Double -> Prim Double, Double -> Double)
decomposeUnaryOp prim = case prim of
  Abs x -> (x, Abs, abs)
  Sgn x -> (x, Sgn, signum)
  _     -> undefined

decomposeBinaryOp :: Prim a -> (a, a, Double -> Double -> Prim Double, Double -> Double -> Double)
decomposeBinaryOp prim = case prim of
  Add x y -> (x, y, Add, (+))
  Sub x y -> (x, y, Sub, (-))
  Mul x y -> (x, y, Mul, (*))
  Div x y -> (x, y, Div, (/))
  _       -> undefined

eval :: Expr -> State [Prim Double] Double
eval (Val value) = pure value
eval (Op prim)   = case opArity prim of
  1 -> do
    let (x, constructor, f) = decomposeUnaryOp prim
    resX <- eval x
    modifyState (constructor resX :)
    return $ f resX
  2 -> do
    let (x, y, constructor, f) = decomposeBinaryOp prim
    resX <- eval x
    resY <- eval y
    modifyState (constructor resX resY :)
    return $ f resX resY
  _ -> undefined

-- (>>=) :: Monad m => m a -> (a -> m b) -> m b
-- eval x >>= \resX ->
-- eval y >>= \resY ->
-- modifyState (...) >>
-- return $ resX + resY

--eval (Op op) = S { runS = runState }
--  where
--    runState arr = case op of
--      Add x y -> do
--        (xval :# xarr) <- runS (eval x) arr
--        (yval :# yarr) <- runS (eval y) xarr
--        (xval + yval) :# (Add xval yval : yarr)

-- (>>=) :: Monad m => m a -> (a -> m b) -> m b

--eval :: Expr -> State [Prim Double] Double
-- eval (Op (Add x y)) = do
--   resX <- eval x
--   resY <- eval y
--   modifyState (Add resX resY :)
--   return (resX + resY)

--eval :: Expr -> State [Prim Double] Double
-- eval (Op (Add x y)) =
--   eval x >>= \resX ->
--   eval y >>= \resY ->
--   modifyState (Add resX resY :) >>= \_ ->
--   return $ resX + resY

