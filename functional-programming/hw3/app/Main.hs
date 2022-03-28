module Main
  ( main
  ) where

import HW3.Action
import HW3.Base ()
import HW3.Evaluator
import HW3.Parser
import HW3.Pretty
import System.Console.Haskeline
import Text.Megaparsec.Error (errorBundlePretty)
import qualified Data.Set as Set
import Control.Monad.IO.Class (liftIO)

permissions :: Set.Set HiPermission
permissions = Set.fromList [AllowRead, AllowWrite, AllowTime]

main :: IO ()
main = runInputT defaultSettings loop
  where
    loop :: InputT IO ()
    loop = do
      minput <- getInputLine "hi> "
      case minput of
        Just input -> do
          case parse input of
--            Right expr -> outputStrLn $ show expr
            Right expr -> do
             evaluated <- liftIO $ runHIO (eval expr) permissions
             case evaluated of
               Left err  -> outputStrLn $ show err
               Right value -> outputStrLn $ show $ prettyValue value
            Left err -> outputStrLn $ errorBundlePretty err
          loop
        Nothing -> return ()
