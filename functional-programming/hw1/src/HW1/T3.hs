module HW1.T3 where

data Tree a = Leaf | Branch (Int, Int) (Tree a) a (Tree a)
  deriving Show

tsize :: Tree a -> Int
tsize Leaf                   = 0
tsize (Branch (s, _)  _ _ _) = s

tdepth :: Tree a -> Int
tdepth Leaf                   = 0
tdepth (Branch (_, d)  _ _ _) = d

tmember :: Ord a => a -> Tree a -> Bool
tmember _ Leaf             = False
tmember x (Branch _ l n r) =
  case compare x n of
    LT -> tmember x l
    EQ -> True
    GT -> tmember x r

mkBranch :: Tree a -> a -> Tree a -> Tree a
mkBranch l n r = Branch (s, d) l n r
  where
    s = tsize l + tsize r + 1
    d = max (tdepth l) (tdepth r) + 1

rotateLeft :: Tree a -> Tree a
rotateLeft (Branch _ l x (Branch _ rl rx rr)) = mkBranch (mkBranch l x rl) rx rr

rotateRight :: Tree a -> Tree a
rotateRight (Branch _ (Branch _ ll lx lr) x r) = mkBranch ll lx (mkBranch lr x r)

tinsert :: Ord a => a -> Tree a -> Tree a
tinsert x Leaf = mkBranch Leaf x Leaf
tinsert x tree@(Branch _ l n r) =
  case compare x n of
    LT ->
      let
        l1@(Branch _ l1l _ l1r) = tinsert x l
      in
        if (tdepth l1) - (tdepth r) == 2 then
          if tdepth l1l < tdepth l1r then
            rotateRight $ mkBranch (rotateLeft l1) n r
          else
            rotateRight $ mkBranch l1 n r
        else
          mkBranch l1 n r
    EQ -> tree
    GT ->
      let
        r1@(Branch _ r1l _ r1r) = tinsert x r
      in
        if (tdepth r1) - (tdepth l) == 2 then
          if tdepth r1l <= tdepth r1r then
            rotateLeft $ mkBranch l n r1
          else
            rotateLeft $ mkBranch l n (rotateRight r1)
        else
          mkBranch l n r1

tFromList :: Ord a => [a] -> Tree a
tFromList = foldl (flip tinsert) Leaf
