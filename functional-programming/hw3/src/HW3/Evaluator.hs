module HW3.Evaluator
  ( eval
  ) where

import HW3.Base
import Control.Monad.Trans.Except
import qualified Data.Text as Text
import Data.Semigroup (stimes)
import Data.Ratio ((%), denominator, numerator)
import qualified Data.Sequence as Seq (fromList, reverse, (><), length, lookup, drop, take)
import qualified Data.ByteString as Bytes (pack, unpack, length, reverse, index, drop, take)
import Data.Foldable (toList)
import Data.Word (Word8)
import Data.Text.Encoding (encodeUtf8, decodeUtf8')
import qualified Data.ByteString.Lazy as BytesLazy
import Codec.Compression.Zlib (compressWith, decompressWith)
import Codec.Compression.Zlib.Internal (defaultCompressParams, compressLevel, bestCompression, defaultDecompressParams)
import Codec.Serialise (serialise, deserialise)
import Data.Maybe (fromMaybe)
import Control.Monad.Trans.Class (lift)
import Text.Read (readMaybe)
import Data.Time.Clock (addUTCTime, diffUTCTime)
import GHC.Base (liftA2)
import qualified Data.Map as Map
import qualified Data.List as List
import Data.Function (on)

arityOf :: HiFun -> Int
arityOf f = case f of
  HiFunAdd -> 2
  HiFunSub -> 2
  HiFunMul -> 2
  HiFunDiv -> 2
  HiFunNot -> 1
  HiFunAnd -> 2
  HiFunOr -> 2
  HiFunLessThan -> 2
  HiFunGreaterThan -> 2
  HiFunEquals -> 2
  HiFunNotLessThan -> 2
  HiFunNotGreaterThan -> 2
  HiFunNotEquals -> 2
  HiFunIf -> 3
  HiFunLength -> 1
  HiFunToUpper -> 1
  HiFunToLower -> 1
  HiFunReverse -> 1
  HiFunTrim -> 1
  HiFunList -> undefined
  HiFunRange -> 2
  HiFunFold -> 2
  HiFunPackBytes -> 1
  HiFunUnpackBytes -> 1
  HiFunEncodeUtf8 -> 1
  HiFunDecodeUtf8 -> 1
  HiFunZip -> 1
  HiFunUnzip -> 1
  HiFunSerialise -> 1
  HiFunDeserialise -> 1
  HiFunRead -> 1
  HiFunWrite -> 2
  HiFunMkDir -> 1
  HiFunChDir -> 1
  HiFunParseTime -> 1
  HiFunRand -> 2
  HiFunEcho -> 1
  HiFunCount -> 1
  HiFunKeys -> 1
  HiFunValues -> 1
  HiFunInvert -> 1

validateInteger :: HiMonad m => Rational -> ExceptT HiError m Integer
validateInteger x = if denominator x == 1 then return (numerator x) else throwE HiErrorInvalidArgument

validateArg :: HiMonad m => (a -> Bool) -> HiError -> a -> ExceptT HiError m a
validateArg p e x = if p x then return x else throwE e

semigroupMul :: (HiMonad m, Semigroup a) => (a -> HiValue) -> a -> Rational -> ExceptT HiError m HiValue
semigroupMul constructor sg n =
  constructor . flip stimes sg <$> (validateInteger n >>= validateArg (>= 1) HiErrorInvalidArgument)

evalApply :: HiMonad m => HiFun -> [HiValue] -> ExceptT HiError m HiValue
evalApply HiFunAdd [HiValueNumber a, HiValueNumber b] = return $ HiValueNumber $ a + b
evalApply HiFunSub [HiValueNumber a, HiValueNumber b] = return $ HiValueNumber $ a - b
evalApply HiFunMul [HiValueNumber a, HiValueNumber b] = return $ HiValueNumber $ a * b
evalApply HiFunDiv [HiValueNumber a, HiValueNumber b] =
  HiValueNumber . (a /) <$> validateArg (/= 0) HiErrorDivideByZero b

evalApply HiFunNot [HiValueBool b] = return $ HiValueBool $ not b
evalApply HiFunLessThan [a, b] = return $ HiValueBool $ a < b
evalApply HiFunGreaterThan [a, b] = return $ HiValueBool $ a > b
evalApply HiFunEquals [a, b] = return $ HiValueBool $ a == b
evalApply HiFunNotLessThan [a, b] = return $ HiValueBool $ a >= b
evalApply HiFunNotGreaterThan [a, b] = return $ HiValueBool $ a <= b
evalApply HiFunNotEquals [a, b] = return $ HiValueBool $ a /= b

evalApply HiFunLength [HiValueString s] = return $ HiValueNumber $ toRational $ Text.length s
evalApply HiFunToUpper [HiValueString s] = return $ HiValueString $ Text.toUpper s
evalApply HiFunToLower [HiValueString s] = return $ HiValueString $ Text.toLower s
evalApply HiFunReverse [HiValueString s] = return $ HiValueString $ Text.reverse s
evalApply HiFunTrim [HiValueString s] = return $ HiValueString $ Text.strip s
evalApply HiFunAdd [HiValueString a, HiValueString b] = return $ HiValueString $ a <> b
evalApply HiFunMul [HiValueString s, HiValueNumber n] = semigroupMul HiValueString s n
evalApply HiFunDiv [HiValueString a, HiValueString b] = return $ HiValueString $ a <> (Text.pack "/") <> b

evalApply HiFunList elems = return $ HiValueList $ Seq.fromList elems
evalApply HiFunRange [HiValueNumber from, HiValueNumber to] =
  return $ HiValueList $ Seq.fromList $ map HiValueNumber [from .. to]
evalApply HiFunFold [f, HiValueList l]
  | null l    = return HiValueNull
  | otherwise = evalExpr $ foldl1 (\a b -> HiExprApply (HiExprValue f) [a, b]) $ fmap HiExprValue l
evalApply HiFunLength [HiValueList l] = return $ HiValueNumber $ toRational $ length l
evalApply HiFunReverse [HiValueList l] = return $ HiValueList $ Seq.reverse l
evalApply HiFunAdd [HiValueList l1, HiValueList l2] = return $ HiValueList $ l1 Seq.>< l2
evalApply HiFunMul [HiValueList l, HiValueNumber n] = semigroupMul HiValueList l n

evalApply HiFunPackBytes [HiValueList l] =
  do
    HiValueBytes . Bytes.pack . toList <$> mapM numberToWord l
  where
    numberToWord :: HiMonad m => HiValue -> ExceptT HiError m Word8
    numberToWord hiValue = do
      case hiValue of
        HiValueNumber value ->
          fromInteger <$> (validateInteger value >>= validateArg (\x -> 0 <= x && x <= 255) HiErrorInvalidArgument)
        _ -> throwE HiErrorInvalidArgument
evalApply HiFunUnpackBytes [HiValueBytes bytes] =
  return $ HiValueList $ fmap (HiValueNumber . fromInteger . toInteger) $ Seq.fromList $ Bytes.unpack bytes
evalApply HiFunEncodeUtf8 [HiValueString s] = return $ HiValueBytes $ encodeUtf8 s
evalApply HiFunDecodeUtf8 [HiValueBytes bytes] =
    catchE (HiValueString <$> (except $ decodeUtf8' bytes)) (const $ return HiValueNull)
evalApply HiFunZip [HiValueBytes bytes] = return $ HiValueBytes $ BytesLazy.toStrict $
  compressWith defaultCompressParams {compressLevel = bestCompression} $ BytesLazy.fromStrict bytes
evalApply HiFunUnzip [HiValueBytes bytes] = return $ HiValueBytes $ BytesLazy.toStrict $
  decompressWith defaultDecompressParams $ BytesLazy.fromStrict bytes
evalApply HiFunSerialise [value] = return $ HiValueBytes $ BytesLazy.toStrict $ serialise value
evalApply HiFunDeserialise [HiValueBytes bytes] = return $ deserialise $ BytesLazy.fromStrict bytes
evalApply HiFunLength [HiValueBytes bytes] = return $ HiValueNumber $ toRational $ Bytes.length bytes
evalApply HiFunReverse [HiValueBytes bytes] = return $ HiValueBytes $ Bytes.reverse bytes
evalApply HiFunAdd [HiValueBytes bytes1, HiValueBytes bytes2] = return $ HiValueBytes $ bytes1 <> bytes2
evalApply HiFunMul [HiValueBytes bytes, HiValueNumber n] = semigroupMul HiValueBytes bytes n

evalApply HiFunRead [HiValueString path] = return $ HiValueAction $ HiActionRead $ Text.unpack path
evalApply HiFunWrite [HiValueString path, HiValueBytes bytes] =
  return $ HiValueAction $ HiActionWrite (Text.unpack path) bytes
evalApply HiFunWrite [HiValueString path, HiValueString str] =
  evalApply HiFunWrite [HiValueString path, HiValueBytes $ encodeUtf8 str]
evalApply HiFunMkDir [HiValueString path] = return $ HiValueAction $ HiActionMkDir $ Text.unpack path
evalApply HiFunChDir [HiValueString path] = return $ HiValueAction $ HiActionChDir $ Text.unpack path

evalApply HiFunParseTime [HiValueString strTime] =
  return $ fromMaybe HiValueNull $ HiValueTime <$> (readMaybe $ Text.unpack strTime)
evalApply HiFunAdd [HiValueTime time, HiValueNumber add] = return $ HiValueTime $ addUTCTime (fromRational add) time
evalApply HiFunSub [HiValueTime t1, HiValueTime t2] = return $ HiValueNumber . toRational $ diffUTCTime t1 t2

evalApply HiFunRand [HiValueNumber a, HiValueNumber b] =
  HiValueAction <$> liftA2 HiActionRand (fromInteger <$> validateInteger a) (fromInteger <$> validateInteger b)

evalApply HiFunEcho [HiValueString s] = return $ HiValueAction $ HiActionEcho s

evalApply HiFunKeys [HiValueDict dict] = return $ HiValueList $ Seq.fromList $ Map.keys dict
evalApply HiFunValues [HiValueDict dict] = return $ HiValueList $ Seq.fromList $ Map.elems dict
evalApply HiFunCount [hiValue] = case hiValue of
  HiValueString str -> countHelper (Text.unpack str) (HiValueString . Text.pack . pure)
  HiValueList list -> countHelper (toList list) id
  HiValueBytes bytes -> countHelper (Bytes.unpack bytes) (HiValueNumber . toRational)
  _ -> throwE HiErrorInvalidArgument
  where
    countHelper :: (HiMonad m, Ord a) => [a] -> (a -> HiValue) -> ExceptT HiError m HiValue
    countHelper arr f = return $ HiValueDict $ Map.fromList $
      map (\gr -> (f (head gr), HiValueNumber . toRational $ length gr)) $ List.group $ List.sort arr
evalApply HiFunInvert [HiValueDict dict] = return $ HiValueDict $ Map.fromList $
  map (\x -> (snd . head $ x, HiValueList $ Seq.fromList $ map fst x)) $
  List.groupBy ((==) `on` snd) $ List.sortBy (compare `on` snd) $ Map.assocs dict

evalApply f args = if length args /= arityOf f then throwE HiErrorArityMismatch else throwE HiErrorInvalidArgument

applyIf :: HiMonad m => [HiExpr] -> ExceptT HiError m HiValue
applyIf [c, a, b] = do
  cond <- evalExpr c
  case cond of
    HiValueBool condBool -> if condBool then evalExpr a else evalExpr b
    _ -> throwE HiErrorInvalidArgument
applyIf _ = throwE HiErrorArityMismatch

applyAnd :: HiMonad m => [HiExpr]-> ExceptT HiError m HiValue
applyAnd [a, b] = do
  aValue <- evalExpr a
  case aValue of
    HiValueBool False -> return aValue
    HiValueNull -> return aValue
    _ -> evalExpr b
applyAnd _ = throwE HiErrorArityMismatch

applyOr :: HiMonad m => [HiExpr]-> ExceptT HiError m HiValue
applyOr [a, b] = do
  aValue <- evalExpr a
  case aValue of
    HiValueBool False -> evalExpr b
    HiValueNull -> evalExpr b
    _ -> return aValue
applyOr _ = throwE HiErrorArityMismatch

unifyFromTo :: Int -> Rational -> Rational -> (Int, Int)
unifyFromTo n from to = (unify from, unify to)
  where
    unify x = let i = truncate x in max (min (if i < 0 then i + n else i) n) 0

arraySubscript :: HiMonad m => HiValue -> Int -> ExceptT HiError m HiValue
arraySubscript (HiValueString str) i = return $ HiValueString $ Text.singleton $ Text.index str i
arraySubscript (HiValueList list) i = return $ fromMaybe HiValueNull $ Seq.lookup i list
arraySubscript (HiValueBytes bytes) i = return $ HiValueNumber $ toRational $ Bytes.index bytes i
arraySubscript hiValue _ = error $ "arraySubscript called on unexpected type: " ++ show hiValue

arraySlice :: HiMonad m => HiValue -> (Int, Int) -> ExceptT HiError m HiValue
arraySlice (HiValueString str) (from, to) = return $ HiValueString $ Text.take (to - from) $ Text.drop from str
arraySlice (HiValueList list) (from, to) = return $ HiValueList $ Seq.take (to - from) $ Seq.drop from list
arraySlice (HiValueBytes bytes) (from, to) = return $ HiValueBytes $ Bytes.take (to - from) $ Bytes.drop from bytes
arraySlice hiValue _ = error $ "arraySlice called on unexpected type: " ++ show hiValue

applyArrayHelper :: HiMonad m => HiValue -> Int -> [HiValue] -> ExceptT HiError m HiValue
applyArrayHelper arr len [HiValueNumber i] = let idx = truncate i in
  case arr of
    HiValueList _ -> arraySubscript arr idx
    _ -> if idx < 0 || idx >= len then return HiValueNull else arraySubscript arr idx
applyArrayHelper arr len [HiValueNumber a, HiValueNumber b] = arraySlice arr $ unifyFromTo len a b
applyArrayHelper arr len [HiValueNull, to] = applyArrayHelper arr len [HiValueNumber 0, to]
applyArrayHelper arr len [from, HiValueNull] = applyArrayHelper arr len [from, HiValueNumber $ toInteger len % 1]
applyArrayHelper _ _ args | (\x -> x == 1 || x == 2) (length args) = throwE HiErrorInvalidArgument
                          | otherwise                              = throwE HiErrorArityMismatch

evalList :: HiMonad m => [HiExpr] -> ExceptT HiError m [HiValue]
evalList = evalList' (return [])
  where
    evalList' :: HiMonad m => ExceptT HiError m [HiValue] -> [HiExpr] -> ExceptT HiError m [HiValue]
    evalList' res []      = res
    evalList' res (h : t) = do
      hEvaluated <- evalExpr h
      (hEvaluated :) <$> (evalList' res t)

evalExpr :: HiMonad m => HiExpr -> ExceptT HiError m HiValue
evalExpr (HiExprValue value)  = return value
evalExpr (HiExprApply f args) = do
  fValue <- evalExpr f
  case fValue of
    HiValueFunction HiFunIf -> applyIf args
    HiValueFunction HiFunAnd -> applyAnd args
    HiValueFunction HiFunOr -> applyOr args
    _ -> do
      argsValues <- evalList args
      case fValue of
        HiValueString str -> applyArrayHelper fValue (Text.length str) argsValues
        HiValueList list -> applyArrayHelper fValue (Seq.length list) argsValues
        HiValueBytes bytes -> applyArrayHelper fValue (Bytes.length bytes) argsValues
        HiValueDict dict -> case argsValues of
          [item] -> return $ fromMaybe HiValueNull $ Map.lookup item dict
          _ -> throwE HiErrorArityMismatch
        HiValueFunction func -> evalApply func argsValues
        _ -> throwE HiErrorInvalidFunction
evalExpr (HiExprDict assocs) = let (keyExprs, valueExprs) = unzip assocs in
  do
    keys <- evalList keyExprs
    values <- evalList valueExprs
    return $ HiValueDict $ Map.fromList $ zip keys values
evalExpr (HiExprRun expr) = do
  exprValue <- evalExpr expr
  case exprValue of
    HiValueAction action -> lift $ runAction action
    _ -> throwE HiErrorInvalidArgument

eval :: HiMonad m => HiExpr -> m (Either HiError HiValue)
eval = runExceptT . evalExpr
