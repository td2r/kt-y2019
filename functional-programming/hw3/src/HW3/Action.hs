{-# LANGUAGE DerivingVia #-}

module HW3.Action
  ( HIO (..)
  , HiPermission (..)
  , PermissionException (..)
  ) where

import HW3.Base
import Control.Exception (Exception, throwIO)
import Data.Set (Set, member)
import Control.Monad.Trans.Reader (ReaderT(..))
import qualified System.Directory as Dir
import qualified Data.Sequence as Seq (fromList)
import qualified Data.ByteString as Bytes (ByteString, readFile, writeFile)
import Data.Text.Encoding (decodeUtf8')
import qualified Data.Text as Text (pack, unpack)
import Data.Time.Clock (getCurrentTime)
import System.Random (getStdRandom, uniformR)

data HiPermission =
    AllowRead
  | AllowWrite
  | AllowTime
  deriving (Show, Eq, Ord, Enum, Bounded)

data PermissionException = PermissionRequired HiPermission
  deriving Show

instance Exception PermissionException

newtype HIO a = HIO { runHIO :: Set HiPermission -> IO a }
  deriving (Functor, Applicative, Monad) via (ReaderT (Set HiPermission) IO)

instance HiMonad HIO where
  runAction (HiActionRead path) = requirePermission AllowRead $ runRead path
  runAction (HiActionWrite path bytes) = requirePermission AllowWrite $ HiValueNull <$ Bytes.writeFile path bytes
  runAction (HiActionMkDir path) = requirePermission AllowWrite $ HiValueNull <$ Dir.createDirectory path
  runAction (HiActionChDir path) = requirePermission AllowRead $ HiValueNull <$ Dir.setCurrentDirectory path
  runAction HiActionCwd = requirePermission AllowRead $ HiValueString . Text.pack <$> Dir.getCurrentDirectory
  runAction HiActionNow = requirePermission AllowTime $ HiValueTime <$> getCurrentTime
  runAction (HiActionRand a b) = HIO $ const $ HiValueNumber . toRational <$> getStdRandom (uniformR (a, b))
  runAction (HiActionEcho text) = requirePermission AllowWrite $ HiValueNull <$ putStrLn (Text.unpack text)

runRead :: FilePath -> IO HiValue
runRead path = do
  dirExists <- Dir.doesDirectoryExist path
  fileExists <- Dir.doesFileExist path
  case (dirExists, fileExists) of
    (True, _) -> HiValueList . Seq.fromList . fmap (HiValueString . Text.pack) <$> Dir.listDirectory path
    (_, True) -> proceedContent <$> Bytes.readFile path
      where
        proceedContent :: Bytes.ByteString -> HiValue
        proceedContent bytes = case decodeUtf8' bytes of
          Right str -> HiValueString str
          Left _ -> HiValueBytes bytes
    (False, False) -> return HiValueNull

requirePermission :: HiPermission -> IO HiValue -> HIO HiValue
requirePermission required result =
  HIO $ \permissions ->
    if member required permissions
    then result
    else throwIO $ PermissionRequired required
