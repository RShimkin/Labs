--1
class MyAppend a where
  appendr::a->a->a	appendl::a->a->a	appendBoth::a->a->a 	appendBoth a b = appendl (appendr a b) b

instance MyAppend [a] where
    appendr a b = a ++ b
    appendl a b = b ++ a

instance MyAppend Int where
    appendr a b = a * 10 + b
    appendl a b = b * (helper a 10) + a
        where
            helper number result
                |number < 10 = result
                |otherwise = helper (number `div` 10) (result * 10)

class Removable a where
    find::a->a->[Int]

    cut::a->[Int]->Int->a

    remove::a->a->a
    remove big small = cut big (find big small) (length small)

instance Removable [a] where
    find big small = check big small [] 0
        where check (bhead:btail) (shead:stail) poses cur

--2
data Tree = Empty | Node Int Tree Tree
    deriving(Show, Eq)

test = Node 4 (Node 3 Empty Empty) (Node 5 Empty (Node 6 Empty Empty))
test1 = Node 4 (Node 3 Empty Empty) (Node 7 (Node 6 (Node 5 Empty Empty) Empty) (Node 8 Empty Empty))
test2 = Node 4 (Node 3 Empty Empty) (Node 7 (Node 6 (Node 5 Empty Empty) (Node 6 Empty Empty)) (Node 8 (Node 7 Empty Empty) Empty))

insert::Int->Tree->Tree
insert a Empty = Node a Empty Empty
insert a inputTree@(Node value left right)
    |a < value = Node value (insert a left) right
    |a >= value = Node value left (insert a right)
    |otherwise = inputTree

toList::Tree->[Int]
toList Empty = []
toList (Node value left right) = (toList left) ++ [value] ++ (toList right)

level::Tree->Int
level Empty = 0
level root@(Node value left right) = helper root 0
    where
        helper Empty result = result
        helper (Node value left right) result = max (helper left (result + 1)) (helper right (result + 1))

instance Semigroup Tree where
    (<>) tree Empty = tree
    (<>) Empty tree = tree
    (<>) tree1 (Node value left right) = (Node value (helper tree1 left) right) 
        where
            helper f Empty = f
            helper f (Node value left right) = (Node value (helper f left) right)

instance Monoid Tree where
    mempty = Empty

tryCondition = toList (mappend test test1) == mappend (toList test) (toList test1)

f = (toList . mconcat $ [test,test1,test2]) == (mconcat . map toList $ [test,test1,test2])

f1 = ((test <> test1) <> test2) == (test <> (test1 <> test2))