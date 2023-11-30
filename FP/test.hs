helper (x:xs) result count
    |count == 0 = result
    |otherwise = helper (xs ++ [x]) ((x:xs) : result) (pred count)

generate [] = [[]]
generate [x] = [[x]]
generate list = helper list [] (length list)

getAllOptions list n = map (\x -> (fst tmp) ++ x) (generate ( snd tmp) )
    where 
    tmp = splitAt n list
                       
newStep list n len
    |(len - n ) < 1 = list
    |otherwise = newStep ( concat (map (flip getAllOptions $ n) list)) (succ n) len

task7:: [a] -> [[a]]
task7 list = newStep [list] 0 (length list)

seeker::[a]->[a]->[Int]
seeker big small = check big small [] 0 0 0
    where 
        check (bhead:btail) (shead:stail) poses pos curb curs
            | (bhead == shead && length btail > 0 && length stail > 0) = check btail stail poses  curb (curb + 1) (curs + 1)
            | (bhead == shead && length stail == 0) = check btail 