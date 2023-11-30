data Tree = Empty | Node Int Tree Tree
    deriving(Show, Eq)

tree1 = Node 4 (Node 3 Empty Empty) (Node 5 Empty (Node 6 Empty Empty))
tree2 = Node 4 (Node 3 Empty Empty) (Node 7 (Node 6 (Node 5 Empty Empty) Empty) (Node 8 Empty Empty))
tree3 = Node 4 (Node 3 Empty Empty) (Node 7 (Node 6 (Node 5 Empty Empty) (Node 6 Empty Empty)) (Node 8 (Node 7 Empty Empty) Empty))

insert::Int->Tree->Tree
insert a Empty = Node a Empty Empty
insert a inputTree@(Node value left right)
    |a < value = Node value (insert a left) right
    |a >= value = Node value left (insert a right)
    |otherwise = inputTree

toList::Tree->[Int]
toList Empty = []
toList (Node value left right) = (toList left) ++ value : (toList right) --[value] ++ (toList right)

level::Tree->Int
level Empty = 0
level root@(Node value left right) = helper root 0
    where
        helper Empty result = result
        helper (Node value left right) result = max (helper left (result + 1)) (helper right (result + 1))

instance Semigroup Tree where
    (<>) tree Empty = tree
    (<>) Empty tree = tree
    (<>) tree1 (Node value left right) = (Node value ((<>) tree1 left) right) 
        --where
            --helper f Empty = f
            --helper f (Node value left right) = (Node value (helper f left) right)

instance Monoid Tree where
    mempty = Empty

tryCondition = toList (mappend tree1 tree2) == mappend (toList tree1) (toList tree2)

f = (toList . mconcat $ [tree1,tree2,tree3]) == (mconcat . map toList $ [tree1,tree2,tree3])

f1 = ((tree1 <> tree2) <> tree3) == (tree1 <> (tree2 <> tree3))