class Insertable a where
    get_before::a->Int->a
    get_after::a->Int->a
    insertInto::a->a->Int->a
    combine::a->a->a->Bool->a
    number_length::a->Int
    insertInto one two index 
        |number_length one < index = insertInto one two (number_length one)
        |number_length one == index = combine one two (get_after one index) False
        |otherwise = combine (get_before one index) two (get_after one index) True
    
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

    combine one two three cond
        | cond == False = merge one two
        | otherwise = merge one (merge two three)
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

    combine one two three cond
        | cond == True = merge (merge one two) three
        | otherwise = merge one two
        where
            merge [] r = r
            merge (l1:l2) r = l1:merge l2 r

main :: IO ()
main = do
    putStrLn "С каким Insertable типом хотите работать ?"
    putStrLn "(1 - Int, 2 - массив)"
    choice <- getLine
    if choice == "1" then do
        putStrLn "Введите число, в которое нужно сделать вставку"
        num1 <- getLine

        putStrLn "Введите число, которое нужно вставить"
        num2 <- getLine

        putStrLn "Введите позицию вставки"
        pos <- getLine
    
        putStrLn . show $ processInt num1 num2 pos
    else do
        if choice == "2" then do
            putStrLn "Введите массив, в который нужно сделать вставку, через пробел"
            arr1 <- getLine

            putStrLn "Введите массив, который нужно вставить, через пробел"
            arr2 <- getLine

            putStrLn "Введите позицию вставки"
            pos <- getLine

            putStrLn . show $ processArray arr1 arr2 pos
        else 
            error "no such option:("
    putStrLn "Нажмите 1, чтобы повторить"
    ch <- getLine
    if ch == "1" then main else return ()

processInt num1 num2 pos = insertInto (read num1 :: Int) (read num2 :: Int) (read pos :: Int)
processArray arr1 arr2 pos = insertInto (words arr1) (words arr2) (read pos :: Int)
