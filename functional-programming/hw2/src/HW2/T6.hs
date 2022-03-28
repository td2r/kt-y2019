{-# LANGUAGE DerivingStrategies         #-}
{-# LANGUAGE GeneralizedNewtypeDeriving #-}

module HW2.T6 where

import Control.Applicative (Alternative (empty, (<|>)), many, some)
import Control.Monad (MonadPlus, mfilter, msum, void)
import Data.Char
import Data.Scientific
import GHC.Natural (Natural)
import HW2.T1 (Annotated (..), Except (..), mapExcept)
import HW2.T4 (Expr (..), Prim (..))
import HW2.T5

-------------------- Debug -------------------------------
--instance Show (Except e a)
--instance Show (Prim a)
--instance Show Expr
--instance Show ParseError
-------------------- Debug -------------------------------

data ParseError = ErrorAtPos Natural

newtype Parser a = P (ExceptState ParseError (Natural, String) a)
  deriving newtype (Functor, Applicative, Monad)

runP :: Parser a -> String -> Except ParseError a
runP (P ES { runES = run }) s = mapExcept (\(x :# _) -> x) (run (0, s))

pChar :: Parser Char
pChar = P $ ES { runES = \(pos, s) ->
  case s of
    []     -> Error (ErrorAtPos pos)
    (c:cs) -> Success (c :# (pos + 1, cs)) }

parseError :: Parser a
parseError = P ES { runES = \(pos, _) ->  Error $ ErrorAtPos pos }

instance Alternative Parser where
  empty = parseError
  (<|>) (P ES { runES = parseP }) (P ES { runES = parseQ }) = P ES {
    runES = \input ->
      case parseP input of
        Success result -> Success result
        Error _        -> parseQ input
    }

instance MonadPlus Parser   -- No methods.

pEof :: Parser ()
pEof = P ES { runES = \input@(pos, s) -> case s of "" -> Success (() :# input) ; _ -> Error (ErrorAtPos pos) }

pAbbr :: Parser String
pAbbr = do
  abbr <- some (mfilter Data.Char.isUpper pChar)
  pEof
  pure abbr

charParser :: a -> Char -> Parser a
charParser r c = do
  c' <- pChar
  if c' == c then return r else parseError

pWhitespaces :: Parser ()
pWhitespaces = void $ many $ charParser () ' '

pToken :: Parser Expr
pToken = do
  comm <- someDigits
  frac <- do void (charParser () '.') ; someDigits <|> pure []
  let b = stringToInteger (stringToInteger 0 comm) frac
  return $ Val $ toRealFloat $ scientific b (negate $ length frac)
  where
    someDigits = some $ mfilter Data.Char.isDigit pChar
    stringToInteger :: Integer -> String -> Integer
    stringToInteger z s = foldl (\x y -> 10 * x + y) z (map (toInteger . digitToInt) s)

pExpr :: Parser Expr
pExpr = do
  pAddSub
  where
    pBinaryLayer binOps pNextLayer = do
      pWhitespaces
      z <- pNextLayer
      xs <- many (do pWhitespaces
                     c <- msum (map (uncurry charParser) binOps)
                     pWhitespaces
                     x <- pNextLayer
                     return (c, x))
      return $ foldl (\x (c, y) -> Op (c x y)) z xs

    pParentheses = do
      charParser () '('
      pWhitespaces
      res <- pExpr
      pWhitespaces
      charParser () ')'
      return res

    pBottom = do
      pWhitespaces
      msum [pParentheses, pToken]

    pMulDiv = pBinaryLayer [(Mul, '*'), (Div, '/')] pBottom
    pAddSub = pBinaryLayer [(Add, '+'), (Sub, '-')] pMulDiv

parseExpr :: String -> Except ParseError Expr
parseExpr = runP (do res <- pExpr; pWhitespaces; pEof; return res)
