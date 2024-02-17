# Входной язык содержит логические выражения, разделённые символом ;
# (точка с запятой). Логические выражения состоят из идентификаторов, констант 0 и 1, знака присваивания (:=),
# операций or, xor, and, not и круглых скобок.

import enum

terminalMartix = [
    # 'and|or|xor''not'        '<:=>'         ';'        '('        ')'       'operand'  '1|0'
    ['Error', '1', 'Error', 'Error', '1', 'Error', '2', '0'],
    ['Error', '1', 'Error', 'Error', '1', 'Error', '2', 'Error'],
    ['0', 'Error', '3', 'Stop', 'Error', '2', 'Error', '3'],
    ['Error', 'Error', 'Error', 'Error', 'Error', 'Error', '2', 'Error']
]

Stack = []
Stack.append('S')

terminalSymbols = ['and|or|xor', 'not', '<:=>', ';', '(', ')', 'operand', '1', '0']

RulesUsed = []

noTernimalMartix = [
    # operand        (           not        ;          and|or|xor'     )  1   0
    ['0', '6', '0', 'Error', 'Error', 'Error', '8', '9'],  # S -> '(' CONDITION  F | CONDITION F
    ['1', '2', '3', 'Error', 'Error', 'Error', 'Error', 'Error'],
    # CONDITION ->  operand (‘>'| ‘<‘| ‘:=‘) operand | (CONDITION) | not operand | true/false operand
    ['Error', 'Error', 'Error', '4', '5', '7', '8', '9']  # F -> ‘;’ | 'and | or | xor'  S| ')'  F
]

regulations = [
    ['CONDITION', 'F'],  # 0 S -> CONDITION F
    ['operand', '<:=>', 'operand'],  # 1 CONDITION ->  operand (‘>'| ‘<‘| ‘:=‘) operand
    ['(', 'CONDITION', ')'],  # 2 CONDITION -> (CONDITION)
    ['not', 'operand'],  # 3 CONDITION -> not operand
    [';'],  # 4 F -> ‘;’
    ['and|or|xor', 'S'],  # 5 F -> 'and | or | xor'  S
    ['(', 'S'],  # 6  S -> '(' CONDITION  F
    [')', 'F'],  # 7 F -> ')'  F
    ['1', 'operand'],   # 8 CONDITION -> true|false operand
    ['0', 'operand']   # 9 CONDITION -> true|false operand
]


def get_nStr(word) -> int:
    if (word == 'S'):
        return 0
    elif (word == 'CONDITION'):
        return 1
    elif (word == 'F'):
        return 2
    else:
        return None


def lexan(str):
    class State(enum.Enum):
        start = 0
        state1 = 1
        state2 = 2
        state3 = 3
        err = 4

    class Char(enum.Enum):
        difrent = 3
        abcdef = 2
        num = 1
        pl_mn = 0
        alpha = 4

    state_machine = [
        [2, 1, 3, 4, 3],
        [4, 1, 1, 4, 4],
        [4, 1, 4, 4, 4],
        [4, 4, 3, 4, 3]
    ]

    # начальное состояние
    cur_state = State.start.value

    # переход
    cur_shift = Char.difrent.value

    for ch in str:
        if cur_state == State.err.value:
            break
        if ch == '+' or ch == '-':
            cur_shift = Char.pl_mn.value
        elif ch >= 'a' and ch <= 'z':
            cur_shift = Char.abcdef.value
        elif ch.isdigit():
            cur_shift = Char.num.value
        else:
            cur_shift = Char.difrent.value

        cur_state = state_machine[cur_state][cur_shift]

    if (cur_state != State.err.value and cur_shift != Char.pl_mn.value):
        # print("Correct")
        return True
    else:
        # print("Not correct" )
        return False

# Проверка, начинается ли правило с нужного условия
def get_nStb(word) -> int:
    if (word == 'and' or word == 'or' or word == 'xor'):
        return 4
    elif (word == '('):
        return 1
    elif (word == 'not'):
        return 2
    elif (word == ';'):
        return 3
    elif (word == ')'):
        return 5
    elif (word == '1'):
        return 6
    elif (word == '0'):
        return 7
    elif (lexan(word) == True):
        return 0
    else:
        return None


def isNotTerminalSymbol(word):
    if get_nStr(word) == None:

        return False
    else:
        return True

def numStb(sc_stack, word) -> int:
    if (word == 'and' or word == 'or' or word == 'xor' ):
        return 0
    elif (word == 'not'):
        return 1
    elif (word == '<' or word == '>' or word == ':=' ):
        return 2
    elif (word == '(' ):
        sc_stack.append('(')
        return 4
    elif (word == '1' or word == '0'):
        return 7
    elif (word == ')' ):
        if len(sc_stack) == 0:
            return None
        elif sc_stack[0] == ')':
            return None
        sc_stack.remove('(')
        # print(sc_stack)
        return 5
    elif (word == ';' ):
        return 3
    elif (lexan(word) == True):
        return 6

    else:
        return None

def isCorrectWord(word) -> bool:
    if word not in ['and', 'or', 'xor', 'not', '(', ')', '<', '>', '=', ';', '0', '1'] or word.isdigit():
        if lexan(word) == True:
            return True
        else:
            return False
    else:

        return True

def stringToArray(text):
    text = text.split(' ')
    tokens = []
    for t in text:
        if t != '':
            tokens.append(t)
            _ = isCorrectWord(t)
    return tokens

def correctArray(array):
    numStr = 0
    sc_stack = []
    for ar in array:
        nStb = numStb(sc_stack, ar)
        if nStb == None:
            return False
        if (terminalMartix[numStr][nStb] == 'Error'):
            return False
        elif (terminalMartix[numStr][nStb] == 'Stop' and len(sc_stack) == 0):
            # print(sc_stack,'8')
            return True
        elif (terminalMartix[numStr][nStb] == 'Stop' and len(sc_stack) != 0):
            # print(sc_stack,'9')
            return False
        else:
            numStr = int(terminalMartix[numStr][nStb])

def pushingStrFromRule(numRule):
    for val in reversed(regulations[int(numRule)]):
        Stack.append(val)
    RulesUsed.append(numRule)

# Проверка на то, является ли числом то, что скрывается под 'operand'
def equalityNumbers(SumbolFromStr, SumbolFromStack):
    # print('symbol',SumbolFromStack,lexan(SumbolFromStr) )
    return SumbolFromStack == 'operand' and lexan(SumbolFromStr)

# Проверка допустимости знака сравнения
def equalitySign(SumbolFromStr, SumbolFromStack):
    return (SumbolFromStack == '<:=>' and
            ((SumbolFromStr == '<') or (SumbolFromStr == ':=') or (SumbolFromStr == '>')))

# Проверка допустимости логики сравнения
def equalityLogic(SumbolFromStr, SumbolFromStack):
    return (SumbolFromStack == 'and|or|xor' and
            ((SumbolFromStr == 'and') or (SumbolFromStr == 'or') or (SumbolFromStr == 'xor')))


def space(numSpace, x=0):
    for ii in range(numSpace + x):
        print(' ', end='')

def print_tree_br(i, numSpace):
    if i < len(RulesUsed) and RulesUsed[i] != '0':
        space(numSpace)
        print('|')
    elif i < len(RulesUsed):
        space(numSpace, 1)
        print('\\')
        space(numSpace, 2)
        print('\\')
        space(numSpace, 3)
        print('\\')

def printRez():
    i = 0
    numSpace = 0

    while i < len(RulesUsed):
        if (int(RulesUsed[i]) == 0):
            space(numSpace, 4)
            print(RulesUsed[i])
            i = i + 1
            space(numSpace, 3)
            print('/ \\')
            space(numSpace, 2)
            print('/   \\')
            space(numSpace, 1)
            print('/     \\')
            space(numSpace)
            print(RulesUsed[i], '     ', RulesUsed[i + 1])
            i = i + 2
            numSpace += 8
            print_tree_br(i, numSpace)
        else:
            space(numSpace)
            print(RulesUsed[i])
            i = i + 1
            print_tree_br(i, numSpace)


def LL_1(array):
    while len(Stack) != 0:
        Sumbol = Stack.pop()
        # print(Sumbol)
        # print(Stack)
        if Sumbol == ';':
            return True
        elif isNotTerminalSymbol(Sumbol):
            nStr = get_nStr(Sumbol)  # 3
            # print(nStr)
            if nStr != None:
                SumbolFromStr = array[0]
                # print(SumbolFromStr)
                nStb = get_nStb(SumbolFromStr)  # 1
                # print(nStb)
                if nStb != None and noTernimalMartix[nStr][nStb] != 'Error':
                    # print(nStb)
                    # запись в стек строку из правила
                    pushingStrFromRule(noTernimalMartix[nStr][nStb])
                else:
                    return False
            else:
                return False
        else:
            SumbolFromStr = array[0]
            SumbolFromStack = Sumbol
            if (SumbolFromStr == SumbolFromStack
                    or equalityNumbers(SumbolFromStr, SumbolFromStack)
                    or equalitySign(SumbolFromStr, SumbolFromStack)
                    or equalityLogic(SumbolFromStr, SumbolFromStack)):
                array = array[1:]
            else:
                return False
    return True


for i in [' ( a := b ) and a 1 a ;',
          ' ( j := s and v := w ) and 1 g ;',
          ' ( ( b := a ) xor 100 > m ) xor w > g  ;',
          ' ( k := t ) xor 1 d > n ;']:
    print()
    print()
    print(i)
    Stack = []
    Stack.append('S')
    RulesUsed = []
    array = stringToArray(i)
    print(array)
    if (correctArray(array) == True):
        if (LL_1(array) == True):

            print(RulesUsed)
            print("Строка верная")
            printRez()

        else:
            print("Строка не верная")
    else:
        print("Строка не верная")

# In[158]:


# In[ ]: