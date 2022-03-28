module HW3.Pretty
  ( prettyValue
  ) where

import HW3.Base (HiValue(..), HiAction(..))
import HW3.Parser (funcName)
import Prettyprinter (Doc, pretty, encloseSep, group)
import Prettyprinter.Render.Terminal (AnsiStyle)
import Data.Ratio
import Data.Scientific (fromRationalRepetendUnlimited, formatScientific, FPFormat(Fixed))
import qualified Data.Map as Map
import qualified Data.Map.Strict as SMap
import qualified Data.ByteString as Bytes (unpack)
import Numeric (showHex)
import Data.Foldable (toList)
import Data.Sequence (adjust')

prettyValue :: HiValue -> Doc AnsiStyle
prettyValue (HiValueNumber x) =
  let
    num = numerator x
    denom = denominator x
  in
    if denom == 1 then
      pretty num
    else
      case fromRationalRepetendUnlimited x of
        (realValue, Nothing) -> pretty $ formatScientific Fixed Nothing realValue
        _ -> if abs num < denom then
          pretty num <> pretty "/" <> pretty denom
        else
          let
            (q, r) = quotRem (abs num) denom
            (qSign, rSign) = if num > 0 then ("", " + ") else ("-", " - ")
          in
            pretty qSign <> pretty q <> pretty rSign <> pretty r <> pretty "/" <> pretty denom

prettyValue (HiValueFunction f) = pretty $
  case SMap.lookup f funcName of
    Just name -> name
    Nothing -> error $ "No string representation for function " ++ show f

prettyValue (HiValueBool b) = pretty $ if b then "true" else "false"

prettyValue HiValueNull = pretty "null"

prettyValue (HiValueString s) = prettyShow s

prettyValue (HiValueList l) =
  group . encloseSep (pretty "[") (pretty " ]") (pretty ", ") $ toList $ adjust' (pretty " " <>) 0 $ fmap prettyValue l

prettyValue (HiValueBytes b) = pretty "[#"
  <> (pretty $ concatMap ((" " ++) . showByte) $ Bytes.unpack b)
  <> pretty " #]"
  where
    showByte a = if a < 16 then reverse $ showHex a "0" else showHex a ""

prettyValue (HiValueAction action) = case action of
  HiActionRead path -> pretty "read(" <> prettyShow path <> pretty ")"
  HiActionWrite path bytes -> pretty "write("
    <> prettyShow path <> pretty ", " <> prettyValue (HiValueBytes bytes)
    <> pretty ")"
  HiActionMkDir path -> pretty "mkdir(" <> prettyShow path <> pretty ")"
  HiActionChDir path -> pretty "cd(" <> prettyShow path <> pretty ")"
  HiActionCwd -> pretty "cwd"
  HiActionNow -> pretty "now"
  HiActionRand a b -> pretty "rand(" <> pretty a <> pretty ", " <> pretty b <> pretty ")"
  HiActionEcho text -> pretty "echo(" <> prettyShow text <> pretty ")"

prettyValue (HiValueTime time) = pretty "parse-time(\"" <> prettyShow time <> pretty "\")"

prettyValue (HiValueDict dict) =
  group . encloseSep (pretty "{ ") (pretty " }") (pretty ", ") $
    map (\(k, v) -> prettyValue k <> pretty ": " <> prettyValue v) $ Map.assocs dict

prettyShow :: Show a => a -> Doc AnsiStyle
prettyShow s = pretty $ show s
