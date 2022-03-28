module HW2.T5 where

import Control.Monad
import HW2.T1
import HW2.T2
import HW2.T3
import HW2.T4 (Expr (..), Prim (..), decomposeUnaryOp, opArity)

data ExceptState e s a = ES { runES :: s -> Except e (Annotated s a) }

mapExceptState :: (a -> b) -> ExceptState e s a -> ExceptState e s b
mapExceptState f ES {runES = run } = ES { runES = mapExcept (mapAnnotated f) . run }

wrapExceptState :: a -> ExceptState e s a
wrapExceptState x = ES { runES = \c -> wrapExcept (x :# c) }

joinExceptState :: ExceptState e s (ExceptState e s a) -> ExceptState e s a
joinExceptState ES { runES = runOuter } = ES { runES =
    \cOuter -> joinExcept $ mapExcept (\(ES { runES = runInner } :# cInner) -> runInner cInner) (runOuter cOuter)
  }

modifyExceptState :: (s -> s) -> ExceptState e s ()
modifyExceptState f = ES { runES = \c -> wrapExcept (() :# f c) }

throwExceptState :: e -> ExceptState e s a
throwExceptState e = ES { runES = const $ Error e }

instance Functor (ExceptState e s) where
  fmap = mapExceptState

instance Applicative (ExceptState e s) where
  pure = wrapExceptState
  p <*> q = Control.Monad.ap p q

instance Monad (ExceptState e s) where
  m >>= f = joinExceptState (fmap f m)

decomposeUnsafeBinaryOp :: Prim a -> (a,
                                      a,
                                      Double -> Double -> Prim Double,
                                      Double -> Double -> Double,
                                      Double -> Double -> Maybe EvaluationError)
decomposeUnsafeBinaryOp prim = case prim of
    Add x y -> (x, y, Add, (+), const . const Nothing)
    Sub x y -> (x, y, Sub, (-), const . const Nothing)
    Mul x y -> (x, y, Mul, (*), const . const Nothing)
    Div x y -> (x, y, Div, (/), \_ denom -> if denom == 0 then Just DivideByZero else Nothing)
    _       -> undefined

data EvaluationError = DivideByZero
eval :: Expr -> ExceptState EvaluationError [Prim Double] Double
eval (Val value) = pure value
eval (Op prim) = case opArity prim of
  1 -> do
    let (x, constructor, f) = decomposeUnaryOp prim
    resX <- eval x
    modifyExceptState (constructor resX :)
    return $ f resX
  2 -> do
    let (x, y, constructor, f, validate) = decomposeUnsafeBinaryOp prim
    resX <- eval x
    resY <- eval y
    modifyExceptState (constructor resX resY :)
    case validate resX resY of
      Just e  -> throwExceptState e
      Nothing -> return $ f resX resY
  _ -> undefined
