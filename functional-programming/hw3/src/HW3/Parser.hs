module HW3.Parser
  ( HW3.Parser.parse
  , funcName
  ) where

import HW3.Base
import Data.Void
import qualified Data.Text as Text (pack)
import qualified Data.List as List
import Data.Function
import qualified Data.Map.Strict as Map
import Text.Megaparsec
import Text.Megaparsec.Char as C
import Text.Megaparsec.Char.Lexer as L
import Control.Monad.Combinators.Expr (Operator, Operator(..), makeExprParser)
import Data.ByteString as Bytes (pack)
import GHC.Base (liftA2)
import Data.Char (digitToInt, isHexDigit, isAlpha, isAlphaNum)
import Data.Word (Word8)

type Parser = Parsec Void String

funcName :: Map.Map HiFun String
funcName = Map.fromList [(HiFunAdd, "add")
                       , (HiFunSub, "sub")
                       , (HiFunMul, "mul")
                       , (HiFunDiv, "div")
                       , (HiFunNot, "not")
                       , (HiFunAnd, "and")
                       , (HiFunOr, "or")
                       , (HiFunLessThan, "less-than")
                       , (HiFunGreaterThan, "greater-than")
                       , (HiFunEquals, "equals")
                       , (HiFunNotLessThan, "not-less-than")
                       , (HiFunNotGreaterThan, "not-greater-than")
                       , (HiFunNotEquals, "not-equals")
                       , (HiFunIf, "if")
                       , (HiFunLength, "length")
                       , (HiFunToUpper, "to-upper")
                       , (HiFunToLower, "to-lower")
                       , (HiFunReverse, "reverse")
                       , (HiFunTrim, "trim")
                       , (HiFunList, "list")
                       , (HiFunRange, "range")
                       , (HiFunFold, "fold")
                       , (HiFunPackBytes, "pack-bytes")
                       , (HiFunUnpackBytes, "unpack-bytes")
                       , (HiFunZip, "zip")
                       , (HiFunUnzip, "unzip")
                       , (HiFunEncodeUtf8, "encode-utf8")
                       , (HiFunDecodeUtf8, "decode-utf8")
                       , (HiFunSerialise, "serialise")
                       , (HiFunDeserialise, "deserialise")
                       , (HiFunRead, "read")
                       , (HiFunWrite, "write")
                       , (HiFunMkDir, "mkdir")
                       , (HiFunChDir, "cd")
                       , (HiFunParseTime, "parse-time")
                       , (HiFunRand, "rand")
                       , (HiFunEcho, "echo")
                       , (HiFunCount, "count")
                       , (HiFunKeys, "keys")
                       , (HiFunValues, "values")
                       , (HiFunInvert, "invert")
                       ]

pBool :: Parser HiValue
pBool = HiValueBool . (== "true") <$> (string "true" <|> string "false")

pNull :: Parser HiValue
pNull = HiValueNull <$ string "null"

pCwd :: Parser HiValue
pCwd = HiValueAction HiActionCwd <$ string "cwd"

pNow :: Parser HiValue
pNow = HiValueAction HiActionNow <$ string "now"

pNumeric :: Parser HiValue
pNumeric = HiValueNumber . toRational <$> L.signed C.space L.scientific

pString :: Parser HiValue
pString = HiValueString . Text.pack <$> (char '"' *> manyTill L.charLiteral (char '"'))

pBytes :: Parser HiValue
pBytes = HiValueBytes . Bytes.pack <$> between (string "[#" <* C.space) (string "#]") (many (p2xHex <* C.space))
  where
    p2xHex :: Parser Word8
    p2xHex = do
      [a, b] <- many $ fromIntegral . digitToInt <$> satisfy isHexDigit
      return $ a * 16 + b

pFunction :: Parser HiValue
pFunction = HiValueFunction <$>
                 choice (map (\(f, name) -> f <$ string name)
                       $ List.sortBy (flip compare `on` (length . snd)) -- make "not-equals" go before "not"
                       $ Map.assocs funcName)

pList :: Parser HiExpr
pList = HiExprApply (HiExprValue (HiValueFunction HiFunList)) <$>
  between (char '[' <* C.space) (char ']') (sepBy pExpr (char ',') <* C.space)

pDic :: Parser HiExpr
pDic = HiExprDict <$> between (char '{' <* C.space) (char '}') (sepBy pPair (char ',') <* C.space)
  where
    pPair :: Parser (HiExpr, HiExpr)
    pPair = liftA2 (,) (pExpr <* char ':') pExpr

pValue :: Parser HiExpr
pValue = choice [HiExprValue <$> choice [pBool, pNull, pCwd, pNow, pNumeric, pString, pBytes, pFunction]
               , pList
               , pDic
               , between (char '(' <* C.space) (char ')') pExpr]
         <* C.space

pApplication :: Parser HiExpr
pApplication = liftA2 (foldl HiExprApply) (C.space *> pValue) (many ((pArgs <|> pDot) <* C.space))
  where
    pArgs :: Parser [HiExpr]
    pArgs = between (char '(' <* C.space) (char ')') (sepBy pExpr (char ',') <* C.space)

    pDot :: Parser [HiExpr]
    pDot = char '.' *>
      (pure . HiExprValue . HiValueString . Text.pack . concat <$>
        (((:) <$> satisfy isAlpha <*> many (satisfy isAlphaNum)) `sepBy1` char '-'))

pAction :: Parser HiExpr
--pAction = pApplication <**> (fromMaybe id <$> optional (HiExprRun <$ char '!' <* C.space))
pAction = liftA2 (foldl $ const . HiExprRun) pApplication (many (char '!' <* C.space))

table :: [[Operator Parser HiExpr]]
table =
  [ [ makeOperator (string "*") HiFunMul InfixL,
      makeOperator (try (string "/" <* notFollowedBy (string "="))) HiFunDiv InfixL
    ],
    [ makeOperator (string "+") HiFunAdd InfixL,
      makeOperator (string "-") HiFunSub InfixL
    ],
    [ makeOperator (try (string "<" <* notFollowedBy (string "="))) HiFunLessThan InfixN,
      makeOperator (try (string ">" <* notFollowedBy (string "="))) HiFunGreaterThan InfixN,
      makeOperator (string "<=") HiFunNotGreaterThan InfixN,
      makeOperator (string ">=") HiFunNotLessThan InfixN,
      makeOperator (string "==") HiFunEquals InfixN,
      makeOperator (string "/=") HiFunNotEquals InfixN
    ],
    [makeOperator (string "&&") HiFunAnd InfixR],
    [makeOperator (string "||") HiFunOr InfixR]
  ]

makeOperator :: Parser String -> HiFun -> (Parser (HiExpr -> HiExpr -> HiExpr) -> Operator Parser HiExpr) -> Operator Parser HiExpr
makeOperator parser fun operator = operator $ (\a b -> HiExprApply (HiExprValue $ HiValueFunction fun) [a, b]) <$ parser

pExpr :: Parser HiExpr
pExpr = makeExprParser pAction table

parse :: String -> Either (ParseErrorBundle String Void) HiExpr
parse = runParser (pExpr <* eof) ""
