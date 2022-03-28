module HW1.T5 where

import Data.List.NonEmpty (NonEmpty ((:|)), (<|))

splitOn :: Eq a => a -> [a] -> NonEmpty [a]
splitOn s (th : tt)
  | th == s   = [] <| lilSplit
  | otherwise = (th : sh) :| st
  where
    lilSplit@(sh :| st) = splitOn s tt
splitOn _ [] = [] :| []

joinWith :: a -> NonEmpty [a] -> [a]
joinWith _ (h :| [])      = h
joinWith s (h :| (b : t)) = h ++ (s : joinWith s (b :| t))
