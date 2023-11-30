data Operand = Operand Double

instance Show Operand where
    show (Operand a) = show a



data UnaryOperation = Neg | Sin | Cos

instance Show UnaryOperation where
    show Neg = "~"
    show Sin = "sin"
    show Cos = "cos"



data BinaryOperation = Plus | Minus | Prod | Div | Exponen

instance Show BinaryOperation where
    show Plus = "+"
    show Minus = "-"
    show Prod = "*"
    show Div = "/"
    show Exponen = "^"



data ElemOfExpr = Oper Operand | Unary UnaryOperation | Binary BinaryOperation | LeftBracket

getPriority::ElemOfExpr -> Int
getPriority (Unary a) = 4
getPriority (Binary (Exponen)) = 3
getPriority (Binary (Prod)) = 2
getPriority (Binary (Div)) = 2
getPriority (Binary (Plus)) = 1
getPriority (Binary (Minus)) = 1
getPriority (LeftBracket) = 0
getPriority (Oper a) = -1

instance Eq ElemOfExpr where
    (==) a b = (getPriority a) == (getPriority b)
    (/=) a b = not (a==b)

instance Ord ElemOfExpr where
    compare a b = compare (getPriority a) (getPriority b)

instance Show ElemOfExpr where
    show (Oper a) = show a 
    show (Unary a) = show a 
    show (Binary a) = show a
    show LeftBracket = "("


data Result a = Solution a | Trouble [Char]

instance Show a => Show (Result a) where
    show (Solution a) = show a 
    show (Trouble a) = a


getListOfTokens::[Char] -> Result [ElemOfExpr]
getListOfTokens [] = Solution ([])
getListOfTokens input = helper input [] []
    where
        helper [] output stack = Solution (output ++ stack)
        helper (x:xs) output stack
            |isElemOfNumber x = helper (snd number) (output ++ [fst number]) stack
            |isElemOfName x = helperWithTrouble func
            |x == ' ' = helper xs output stack
            |x == '+' = helper xs (snd putPlus) (fst putPlus)
            |x == '-' = helper xs (snd putMinus) (fst putMinus)
            |x == '/' = helper xs (snd putDiv) (fst putDiv)
            |x == '*' = helper xs (snd putProd) (fst putProd)
            |x == '^' = helper xs (snd putExponen) (fst putExponen)
            |x == '~' = helper xs output ((Unary (Neg)) : stack)
            |x == '(' = helper xs output ((LeftBracket) : stack)
            |x == ')' = helper xs (snd rightBracket) (fst rightBracket)
            |otherwise = Trouble ("Неизвестный символ " ++ (show x ) ++ " в выражении")
            where
                number = getNumberToken (x:xs)
                func = getFuncToken (x:xs)
                rightBracket = findRightBracket output stack
                putPlus = putNewOperator output stack (Binary (Plus))
                putMinus = putNewOperator output stack (Binary (Minus))
                putDiv = putNewOperator output stack (Binary (Div))
                putProd = putNewOperator output stack (Binary (Prod))
                putExponen = putNewOperator output stack (Binary (Exponen))

                helperWithTrouble (Solution a) = helper (snd a) output ((fst a) : stack)
                helperWithTrouble (Trouble a) = Trouble (a)

getNumberToken::[Char] -> (ElemOfExpr, [Char])
getNumberToken input = (Oper (Operand (read (take currLenght input))), drop currLenght input)
    where
        currLenght = helper input 0
        helper [] result = result
        helper (x:xs) result
            |isElemOfNumber x = helper xs (result + 1)
            |otherwise = result

getFuncToken::[Char] -> Result (ElemOfExpr, [Char])
getFuncToken input 
    |fun == "sin" = Solution ((Unary Sin, drop 3 input))
    |fun == "cos" = Solution ((Unary Cos, drop 3 input))
    |otherwise = Trouble ("В выражении используется неизвестная функция " ++ (show fun))
        where 
            fun = take 3 input


findRightBracket::[ElemOfExpr] -> [ElemOfExpr] -> ([ElemOfExpr],[ElemOfExpr])
findRightBracket [] [] = ([],[])
findRightBracket output [] = ([], output)
findRightBracket output [LeftBracket] = ([], output)
findRightBracket output (LeftBracket:xs) = (xs, output)
findRightBracket output (x:xs) = findRightBracket (output ++ [x]) xs

putNewOperator::[ElemOfExpr] -> [ElemOfExpr] -> ElemOfExpr -> ([ElemOfExpr],[ElemOfExpr])
putNewOperator output [] newOperation = ([newOperation], output)
putNewOperator output stack newOperation
    | newOperation <= (head stack) = putNewOperator (output ++ [head stack]) (tail stack) newOperation
    | otherwise = (newOperation : stack, output)

isElemOfNumber::Char -> Bool
isElemOfNumber x = ((x >= '0') && (x <= '9')) || (x =='.')

isElemOfName::Char -> Bool
isElemOfName x = ( x >= 'a') && ( x <= 'z')

getSolve:: Result [ElemOfExpr] -> Result Double
getSolve (Trouble a) = (Trouble a)
getSolve (Solution (tokens)) = solver tokens []
    where 
        solver [] [] = Trouble ("Пустое выражение")
        solver [] [x] = Solution (x)
        solver (x:xs) stack = helper x stack
           where
             helper (Oper (Operand a)) stack = solver xs (a:stack)
             helper (Unary Neg) (a:cs) = solver xs ( (-(a)) : cs )
             helper (Unary Sin) (a:cs) = solver xs ( (sin a) : cs )
             helper (Unary Cos) (a:cs) = solver xs ( (cos a) : cs )
             helper (Binary Plus) (a:b:cs) = solver xs ((b + a) : cs)
             helper (Binary Minus) (a:b:cs) = solver xs ((b - a) : cs)
             helper (Binary Div) (a:b:cs) = if (a /= 0) then solver xs ((b / a) : cs) else Trouble ("Ошибка, деление на ноль")
             helper (Binary Prod) (a:b:cs) = solver xs ((b * a) : cs)
             helper (Binary Exponen) (a:b:cs) = solver xs ((b ** a) : cs)
             helper x stack = Trouble ("Выражение содержит ошибку")


test = "(3+4*(2+cos(0/3)))^2"
test1 = "3+6-7%4"
test2 = "3+d7-56"
test3 = ""
test4 = "3/(1-1)"
test5 = "++2" 