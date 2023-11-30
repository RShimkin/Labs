import Data.Char(isUpper, isLower)

-- 1
square'roots::(Ord a, Floating a) => a -> a -> a -> [a]
square'roots a b c 
    |(a==0 && b==0 && c==0) = error "Infinite number of roots"
    |(a==0 && b/=0) = [(-c)/b]
    |(a/=0 && b^2 - 4*a*c >= 0) = [((-b) + sqrt (b^2 - 4*a*c))/(2*a), ((-b) - sqrt (b^2 - 4*a*c))/(2*a)]
    |(a/=0 && b^2 - 4*a*c < 0) = error "Roots are in complex realm"
    |otherwise = error "No roots:("
    

-- 2
sum'n'count::Integer -> (Integer, Integer)
sum'n'count num
    |num == 0 = (0,1)
    |num < 0 = process (-num) 0 0
    |otherwise = process num 0 0
        where
            process num sum count 
                |(num == 0) = (sum, count)
                |otherwise = process (num `div` 10) (sum + (num `mod` 10)) (count + 1)

-- 3
remove'uppers::String -> String
remove'uppers str = unwords . filter (\x -> all isLower x) . words $ str

-- 4
sumSeries::(Fractional a, Enum a) => a -> a
sumSeries n = foldl (+) 0 . map (\x -> (1/x^2)) $ [1..n]

-- 5
removeEveryThird:: [a] -> [a]
removeEveryThird list
    |length list < 3 = list
    |otherwise = helper list
        where helper (x:y:z:xs) = x:y:(removeEveryThird xs)

-- 6
findGrowingSeries:: Ord a => [a] -> [[a]]
findGrowingSeries [] = [[]]
findGrowingSeries [x] = [[x]]
findGrowingSeries list = process list 1 []
    where
        process [] maxN result = reverse result
        process input maxN result
            | currLength == maxN = process (drop currLength input) maxN ((take currLength input) : result)
            | currLength > maxN = process (drop currLength input) currLength ((take currLength input) : [])
            | otherwise = process (drop currLength input) maxN (result)
                where
                    currLength = countOfNonDecreasingS input
                    countOfNonDecreasingS [] = 0
                    countOfNonDecreasingS [x] = 1
                    countOfNonDecreasingS list = helper1 list 1
                        where
                            helper1 [x] n = n 
                            helper1 (x:y:xs) n
                                |x > y = n 
                                |otherwise = helper1 (y:xs) (succ n)

task6:: Ord a => [a] -> [[a]]
task6 [] = [[]]
task6 list = pieces [[]] [] list
    where
        pieces::[[a]] -> [a] -> [a] -> [[a]]
        pieces res cur arr@(h:t)
            |length arr == 0 = res
            |otherwise = pieces (res ++ (cur ++ [h])) (cur ++ [h]) h


--task7
task7:: [a] -> [[a]]
task7 list = newStep [list] 0 (length list)
    where
        newStep list n len
            |(len - n ) < 1 = list
            |otherwise = newStep ( concat (map (flip getAllOptions $ n) list)) (succ n) len
            where
                getAllOptions list n = map (\x -> (fst tmp) ++ x) (generate ( snd tmp) )
                    where 
                        tmp = splitAt n list
                        generate [] = [[]]
                        generate [x] = [[x]]
                        generate list = helper list [] (length list)
                            where 
                                helper (x:xs) result count
                                    |count == 0 = result
                                    |otherwise = helper (xs ++ [x]) ((x:xs) : result) (pred count)