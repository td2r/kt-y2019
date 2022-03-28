module HW0.T6 where

import Data.Char (isSpace)
import HW0.T1

a = distrib (Left ("AB" ++ "CD" ++ "EF"))     -- distrib from HW0.T1
b = map isSpace "Hello, World"
c = if 1 > 0 || error "X" then "Y" else "Z"

--Determine the WHNF (weak head normal form) of these values:

--a_whnf = distrib (Left ("AB" ++ "CD" ++ "EF"))
a_whnf = (Left a, Left a) where a = "AB" ++ "CD" ++ "EF"
--As I know, haskell holds a pointer to computation, which used in function more than once
--Constructor (Left) stops reduction of variable a

--b_whnf = map isSpace "Hello, World"
b_whnf = isSpace 'H' : map isSpace "ello, World"
--Constructor (:) stops reduction of "isSpace 'H'"

--c_whnf = if 1 > 0 || error ('X' : []) then 'Y' : [] else 'Z' : []
--c_whnf = if True || error ('X' : []) then 'Y' : [] else 'Z' : []
--c_whnf = if True then 'Y' : [] else 'Z' : []
c_whnf = 'Y' : []
