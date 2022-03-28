module HW1.T4 where

import HW1.T3

tfoldr :: (a -> b -> b) -> b -> Tree a -> b
tfoldr _ z Leaf             = z
tfoldr f z (Branch _ l n r) = tfoldr f (f n (tfoldr f z r)) l

treeToList :: Tree a -> [a]    -- output list is sorted
treeToList = tfoldr (:) []
