{-# LANGUAGE DeriveGeneric #-}
{-# LANGUAGE DeriveAnyClass #-}

module HW3.Base
  ( HiError (..)
  , HiExpr (..)
  , HiFun (..)
  , HiMonad (..)
  , HiValue (..)
  , HiAction (..)
  ) where

import Data.ByteString (ByteString)
import Data.Text (Text)
import Data.Sequence (Seq)
import GHC.Generics (Generic)
import Codec.Serialise (Serialise)
import Data.Time (UTCTime)
import Data.Map (Map)

data HiFun

  = HiFunDiv --p7 l
  | HiFunMul --p7 l
  | HiFunAdd --p6 l
  | HiFunSub --p6 l

  | HiFunNot
  | HiFunAnd --p3 r
  | HiFunOr --p2 r
  | HiFunLessThan --p4
  | HiFunGreaterThan --p4
  | HiFunEquals --p4
  | HiFunNotLessThan --p4
  | HiFunNotGreaterThan --p4
  | HiFunNotEquals --p4
  | HiFunIf

  | HiFunLength
  | HiFunToUpper
  | HiFunToLower
  | HiFunReverse
  | HiFunTrim

  | HiFunList
  | HiFunRange
  | HiFunFold

  | HiFunPackBytes
  | HiFunUnpackBytes
  | HiFunEncodeUtf8
  | HiFunDecodeUtf8
  | HiFunZip
  | HiFunUnzip
  | HiFunSerialise
  | HiFunDeserialise

  | HiFunRead
  | HiFunWrite
  | HiFunMkDir
  | HiFunChDir

  | HiFunParseTime
  
  | HiFunRand
  
  | HiFunEcho
  
  | HiFunCount
  | HiFunKeys
  | HiFunValues
  | HiFunInvert

  deriving (Show, Eq, Ord, Generic, Serialise)

data HiValue
  = HiValueBool Bool
  | HiValueNumber Rational
  | HiValueFunction HiFun

  | HiValueNull
  | HiValueString Text

  | HiValueList (Seq HiValue)
  
  | HiValueBytes ByteString
  
  | HiValueAction HiAction
  
  | HiValueTime UTCTime
  
  | HiValueDict (Map HiValue HiValue)

  deriving (Show, Eq, Ord, Generic, Serialise)

data HiExpr
  = HiExprValue HiValue
  | HiExprApply HiExpr [HiExpr]
  | HiExprRun HiExpr
  | HiExprDict [(HiExpr, HiExpr)]
  deriving (Show, Eq, Ord)

data HiError
  = HiErrorInvalidArgument
  | HiErrorInvalidFunction
  | HiErrorArityMismatch
  | HiErrorDivideByZero
  deriving (Show, Eq, Ord)

data HiAction =
    HiActionRead  FilePath
  | HiActionWrite FilePath ByteString
  | HiActionMkDir FilePath
  | HiActionChDir FilePath
  | HiActionCwd
  
  | HiActionNow
  
  | HiActionRand Int Int
  
  | HiActionEcho Text

  deriving (Show, Eq, Ord, Generic, Serialise)

class Monad m => HiMonad m where
  runAction :: HiAction -> m HiValue
