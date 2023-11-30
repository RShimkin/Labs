class Insertable a where
    get_before::a->Int->a
    get_after::a->Int->a
    insertInto::a->a->Int->a
    combine::a->a->a->a
    number_length::a->Int
    insertInto one two index = combine (get_before one index) two (get_after one index)
    
instance Insertable Int where
    number_length num = check num 1
        where
            check n i
                |n==0 = 1
                |((n>=10^(i-1)) && (n<10^i)) = i
                |otherwise = check n (i+1)

    get_before num index = chop num ((number_length num) - index)
        where
            chop n i
                |i==0 = n
                |otherwise = chop (n `div` 10) (i-1)

    get_after num index = behead num ((number_length num) - index) 1
        where
            behead n i accum
                |i == 0 = n `mod` accum
                |otherwise = behead n (i-1) (accum*10)

    combine one two three = merge one (merge two three)
        where
            merge l r = r + l * (10 ^ (number_length r))

instance Insertable [k] where
    number_length arr = length arr

    get_before arr index = rec arr index
        where
            rec (h:t) index
                |index == 1 = [h]
                |otherwise = (h:(rec t (index-1)))

    get_after arr index = rec arr index
        where
            rec (h:t) index
                |index == 1 = t
                |otherwise = rec t (index-1)

    combine one two three = merge (merge one two) three
        where
            merge [] r = r
            merge (l1:l2) r = l1:merge l2 r