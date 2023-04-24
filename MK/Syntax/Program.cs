void Main()
{
    Console.Write("Введите имя файла: ");
    string? fileName = $"..\\..\\..\\{Console.ReadLine()}";

    using StreamReader reader = new(fileName);
    try
    {
        List<Token> tokens = Lexer.GetListOfTokensFromFile(reader);

        foreach (Token token in tokens) { Console.WriteLine(token); }

        List<INode> trees = Parser.GetListOfTrees(tokens);
        TreePrinter printer = new TreePrinter();
        printer.PrintTrees(trees);
    } catch (Exception ex)
    {
        Console.WriteLine($"Ошибка!!! > {ex.Message}");
    }
}

Main();
static class Lexer
{
    static int[,] D =
    { //  ;   :   =   (   )   +   -   *   /  09  aZ   '  __   
        { 11,  9, -9,  7,  8,  2,  3,  4,  5,  1,  1,  6,  0 }, //S0
        { -1, -1, -9, -1, -1, -1, -1, -1, -1,  1,  1, -1, -1 }, //S1
        { -4, -4, -9, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 }, //S2
        { -4, -4, -9, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 }, //S3
        { -4, -4, -9, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 }, //S4
        { -4, -4, -9, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4 }, //S5
        { 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12 }, //S6
        { -7, -7, -9, -7, -7, -7, -7, -7, -7, -7, -7, -7, -7 }, //S7
        { -8, -8, -9, -8, -8, -8, -8, -8, -8, -8, -8, -8, -8 }, //S8
        { -9, -9, 10, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9 }, //S9
        { -6, -6, -9, -6, -6, -6, -6, -6, -6, -6, -6, -6, -6 }, //S10
        { -5, -5, -9, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5 }, //S11
        { -9, -9, -9, -9, -9, -9, -9, -9, -9, -9, -9,-10, -9 }, //S12
    };

    static int[] W = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 };

    public static List<Token> GetListOfTokensFromFile(StreamReader reader)
    {
        List<Token> tokens = new List<Token>();

        string? S = "\0";
        int lineCounter = 0, lenghtOfLine = 0;
        int i = 0, ST = 0, CL = 0, FIX = 0, prev = 0;

        bool working = true;

        while (working)
        {
            if (i == lenghtOfLine)
            {
                if (ST == 0)
                {
                    if ((S = reader.ReadLine()) == null)
                    {
                        working = false;
                        continue;
                    }
                    lineCounter++;
                    S = S + " ";
                    lenghtOfLine = S.Length;
                    i = 0;
                }
                else
                {
                    throw new Exception($"Ошибка в лексеме на позиции {i} в строке {lineCounter}");
                }
            }

            if (ST == 0) FIX = i;

            CL = sclass(S[i]);
            if (CL == -1) throw new Exception($"Неизвестный символ на позиции {i} в строке {lineCounter}");

            prev = ST == 0 ? prev : ST;
            ST = D[ST, CL];
            if (ST == -9) throw new Exception($"Ошибка в лексеме на позиции {FIX + 1} в строке {lineCounter} [{prev} -> {S[i]}]");
            i++;

            if (ST < 0)
            {
                i = i - W[-ST - 1];
                string result = S.Substring(FIX, i - FIX);
                bool put = false;
                if (ST == -4 && result == "-")
                {
                    var prevTokenType = tokens.Last().Type;
                    if (prevTokenType == 'a' || prevTokenType == 'l')
                    {
                        tokens.Add(GetToken(-3, result, lineCounter, FIX + 1));
                        put = true;
                    }
                }
                if (!put)
                    tokens.Add(GetToken(ST, result, lineCounter, FIX + 1));
                ST = 0;
            }
        }
        return tokens;
    }

    private static Token GetToken(int ST, string value, int line, int position)
    {
        char type = ' ';
        switch (ST)
        {
            case -1:
                {
                    type = 'i';
                    break;
                }
            case -2:
                {
                    type = 'n';
                    break;
                }
            case -3:
                {
                    type = 'u';
                    break;
                }
            case -4:
                {
                    type = 't';
                    break;
                }
            case -5:
                {
                    type = 'f';
                    break;
                }
            case -6:
                {
                    type = 'a';
                    break;
                }
            case -7:
                {
                    type = 'l';
                    break;
                }
            case -8:
                {
                    type = 'r';
                    break;
                }
            case -10: { type = 'n'; break; }
        }

        return new Token(type, value, line, position);
    }

    private static int sclass(char c)
    {
        switch (c)
        {
            case ';': return 0;
            case ':': return 1;
            case '=': return 2;
            case '(': return 3;
            case ')': return 4;
            case '+': return 5;
            case '-': return 6;
            case '*': return 7;
            case '/': return 8;
            case '\'': return 11;
            case ' ': case '\0': return 12;
            default:
                {
                    if (Char.IsDigit(c)) return 9;
                    if (Char.IsLetter(c)) return 10;
                    return -1;
                }
        }
    }

}

static class Parser
{
    // SLR - анализатор
    /* Правила
        * (0) S -> B
        * (1) B -> iaEfB
        * (2) B -> f
        * (3) E -> uE
        * (4) E -> EtE
        * (5) E -> i
        * (6) E -> n
        * (7) E -> lEr
        * (8) B -> e
        * 
        * где i - идентификатор, a - присвоение, f - точка с запятой, u - унарная операция, t - бинарная операция, n - число,
        *     l - левая скобка, r - правая скобка, e - пустая строка, B - операция с точкой с запятой, E - логическое выражение 
        */

    public static bool debug = false, debug2 = false;

    static Dictionary<string, Tuple<char, int>> RULES = new Dictionary<string, Tuple<char, int>>
    {
        {"r1", new Tuple<char, int>('B',5)},
        {"r2", new Tuple<char, int>('B',1)},
        {"r3", new Tuple<char, int>('E',2)},
        {"r4", new Tuple<char, int>('E',3)},
        {"r5", new Tuple<char, int>('E',1)},
        {"r6", new Tuple<char, int>('E',1)},
        {"r7", new Tuple<char, int>('E',3)},
        {"r8", new Tuple<char, int>('B',0)},
    };

    //s - shift, r - reduce
    static string[,] ACTION =
    {//   i     u     n     t     l     r     f     a     $
        { "s2", "  ", "  ", "  ", "  ", "  ", "s3", "  ", "r8",}, // I0  
        { "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ",  "a",}, // I1  0 ;
        { "  ", "  ", "  ", "  ", "  ", "  ", "  ", "s4", "  ",}, // I2  i | i:=i;i | i:=n;i
        { "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "r2",}, // I3  0 ; | 10 i:=i;; | 10 i:=n;;
        { "s7", "s6", "s8", "  ", "s9", "  ", "  ", "  ", "  ",}, // I4  i:= | i:=i;i:= | i:=n;i:=
        { "  ", "  ", "  ","s11", "  ", "  ","s10", "  ", "  ",}, // I5  7 i:=i | 8 i:=n
        { "s7", "s6", "s8", "  ", "s9", "  ", "  ", "  ", "  ",}, // I6  i:=u | i:=uu | i:=(u | i:=u(u
                                                                //          | u:=uu(u | i:=itu | i:=ntu
                                                                //          | 7 13 11 i:=(itu | 7 13 11 i:=u(itu
                                                                //          | 8 13 11 i:=(ntu | 8 13 11 i:=u(ntu
                                                                //          | 8 13 11 u:=uu(ntu
        { "  ", "  ", "  ", "r5", "  ", "r5", "r5", "  ", "  ",}, // I7  i:=i | i:=ui | i:=(i | i:=u(i
                                                                //          | i:=iti | i:=nti
                                                                //          | 7 13 11 i:=(iti | 7 13 11 i:=u(iti
                                                                //          | 8 13 11 i:=(nti | 8 13 11 i:=u(nti
                                                                //          | 8 13 11 u:=uu(nti
        { "  ", "  ", "  ", "r6", "  ", "r6", "r6", "  ", "  ",}, // I8  i:=n | i:=un | i:=(n | i:=u(n
                                                                //          | u:=uu(n | i:=itn | i:=ntn
                                                                //          | 7 13 11 i:=(itn | 7 13 11 i:=u(itn
                                                                //          | 8 13 11 i:=(ntn | 8 13 11 i:=u(ntn
                                                                //          | 8 13 11 u:=uu(ntn
        { "s7", "s6", "s8", "  ", "s9", "  ", "  ", "  ", "  ",}, // I9  i:=( | i:=u( | u:=uu( | i:=((
                                                                //          | i:=u(( | u:=uu(( | i:=it( | i:=nt(
                                                                //          | 7 13 11 i:=(it( | 7 13 11 i:=u(it(
                                                                //          | 8 13 11 i:=(nt( | 8 13 11 i:=u(nt(
                                                                //          | 8 13 11 u:=uu(nt(
        { "s2", "  ", "  ", "  ", "  ", "  ", "s3", "  ", "r8",}, // I10 i:=i; | i:=n;
        { "s7", "s6", "s8", "  ", "s9", "  ", "  ", "  ", "  ",}, // I11 i:=it | i:=nt | 7 13 i:=(it | 7 13 i:=u(it
                                                                //           | 8 13 i:=(nt | 8 13 i:=u(nt
                                                                //           | 8 13 u:=uu(nt
        { "  ", "  ", "  ", "r3", "  ", "r3", "r3", "  ", "  ",}, // I12 7 i:=ui | 8 i:=un
        { "  ", "  ", "  ","s11", "  ","s16", "  ", "  ", "  ",}, // I13 7 i:=(i | 7 i:=u(i | 8 i:=(n
                                                                //             | 8 i:=u(n | 8 u:=uu(n
        { "  ", "  ", "  ", "  ", "  ", "  ", "  ", "  ", "r1",}, // I14 10 i:=i;; | 10 i:=n;; | i:=i; | i:=n;
        { "  ", "  ", "  ", "r4", "  ", "r4", "r4", "  ", "  ",}, // I15
        { "  ", "  ", "  ", "r7", "  ", "r7", "r7", "  ", "  ",}  // I16 7 13 i:=(i) | 7 13 i:=u(i)
                                                                //                 | 8 13 i:=(n) | 8 13 i:=u(n)
                                                                //                 | 8 13 u:=uu(n)
    };

    static int[,] GOTO =
    {//  B   E
        {  1,  0}, // I0
        {  0,  0}, // I1
        {  0,  0}, // I2
        {  0,  0}, // I3
        {  0,  5}, // I4
        {  0,  0}, // I5
        {  0, 12}, // I6
        {  0,  0}, // I7
        {  0,  0}, // I8
        {  0, 13}, // I9
        { 14,  0}, // I10
        {  0, 15}, // I11
        {  0,  0}, // I12
        {  0,  0}, // I13
        {  0,  0}, // I14
        {  0,  0}, // I15
        {  0,  0}  // I16
    };

    public static List<INode> GetListOfTrees(List<Token> inputTokens)
    {
        List<Token> tokens = inputTokens;
        tokens.Add(new Token('$', "$", 0, 0));

        List<INode> forestOfExpressionTrees = new List<INode>();
        Stack<int> stateStack = new Stack<int>();
        Stack<INode> nodeStack = new Stack<INode>();

        stateStack.Push(0);

        int numberOfTokens = tokens.Count;
        int index = 0;
        char typeOfCurrentToken = GetTypeOfCurrentToken(tokens, index);
        bool accept = false;

        while (index < numberOfTokens && !accept)
        {
            string action = ACTION[stateStack.Peek(), TClass(typeOfCurrentToken)];

            char act = action[0];

            switch (act)
            {
                case 'a':
                    {
                        accept = true;
                        continue;
                    }
                case 's':
                    {
                        printStack(nodeStack, "&  ", debug2);
                        nodeStack.Push(CreateNodeFromToken(tokens[index]));
                        printStack(nodeStack, "&& ", debug2);

                        index++;
                        typeOfCurrentToken = GetTypeOfCurrentToken(tokens, index);
                        string numberOfNewState = action.Substring(1);
                        stateStack.Push(int.Parse(numberOfNewState));
                        break;
                    }
                case 'r':
                    {
                        printStack(nodeStack, "?  ", debug2);
                        nodeStack = ChangeStackByRule(forestOfExpressionTrees, nodeStack, action);
                        printStack(nodeStack, "?? ", debug2);

                        Tuple<char, int> rule = RULES[action];
                        for (int i = 0; i < rule.Item2; i++)
                        {
                            stateStack.Pop();
                        }
                        stateStack.Push(GOTO[stateStack.Peek(), TClass(rule.Item1)]);
                        break;
                    }
                default:
                    {
                        Token errorToken = tokens[index];
                        throw new Exception($"Ошибка синтаксиса в {errorToken.Line} строке на {errorToken.Position} позиции");
                    }
            }

        }

        return forestOfExpressionTrees;
    }

    private static INode CreateNodeFromToken(Token token)
    {
        char type = token.Type;
        switch (type)
        {
            case 'i': return new VariableNode(token);
            case 'n': return new NumberNode(token);
            case 'a': return new AssignNode(new CommonNode(), new CommonNode());
            case 'u': return new UnaryOperationNode(token, new CommonNode());
            case 't': return new BinaryOperationNode(token, new CommonNode(), new CommonNode());
            default: return new CommonNode();
        }
    }

    private static Stack<INode> ChangeStackByRule(List<INode> forestOfExpressionTrees, Stack<INode> stack, string rule)
    {
        Stack<INode> result = stack;
        switch (rule)
        {
            case "r1":
                {
                    printStack(result, "1  ", debug);
                    result.Pop();
                    result.Pop();
                    INode rightNode = result.Pop();
                    var cur = result.Pop();
                    INode leftNode = result.Pop();

                    while (cur is BinaryOperationNode)
                    {
                        BinaryOperationNode operation = (BinaryOperationNode)cur;
                        operation.LeftNode = leftNode;
                        operation.RightNode = rightNode;
                        result.Push(operation);
                        rightNode = result.Pop();
                        cur = result.Pop();
                        leftNode = result.Pop();
                    }

                    AssignNode assign = (AssignNode)cur;
                    assign.LeftNode = leftNode;
                    assign.RightNode = rightNode;
                    result.Push(assign);
                    forestOfExpressionTrees.Insert(0, assign);
                    printStack(result, "1! ", debug);
                    break;
                }
            case "r3":
                {
                    printStack(result, "3  ", debug);
                    INode targetNode = result.Pop();
                    UnaryOperationNode operation = (UnaryOperationNode)result.Pop();
                    operation.Child = targetNode;
                    result.Push(operation);
                    printStack(result, "3! ", debug);
                    break;
                }
            case "r4":
                {
                    printStack(result, "4  ", debug);
                    INode rightNode = result.Pop();
                    BinaryOperationNode operation = (BinaryOperationNode)result.Pop();

                    if ((new List<string> { "*", "/"/*, "+", "-"*/ }).Contains(operation.Operation.Value))
                    {
                        INode leftNode = result.Pop();
                        operation.LeftNode = leftNode;
                        operation.RightNode = rightNode;
                        result.Push(operation);
                    }
                    else
                    {
                        result.Push(operation);
                        result.Push(rightNode);
                    }
                    printStack(result, "4! ", debug);
                    break;
                }
            case "r7":
                {
                    printStack(result, "7  ", debug);
                    result.Pop();
                    INode node = result.Pop();
                    INode anotherNode = result.Pop();
                    if (anotherNode is CommonNode)
                    {
                        result.Push(node);
                    }
                    else
                    {
                        result.Push(anotherNode);
                        result.Push(node);
                        result = GetSolveForLowPriorityNodes(result, new CommonNode());
                    }
                    printStack(result, "7! ", debug);
                    break;
                }
            case "r8":
                {
                    printStack(result, "8  ", debug);
                    INode node = result.Pop();
                    INode anotherNode = result.Pop();
                    INode interestingNode = result.Pop();
                    if (interestingNode is AssignNode)
                    {
                        result.Push(interestingNode);
                        result.Push(anotherNode);
                        result.Push(node);
                    }
                    else
                    {
                        result.Push(interestingNode);
                        result.Push(anotherNode);
                        result = GetSolveForLowPriorityNodes(result, new AssignNode(new CommonNode(), new CommonNode()));
                        result.Push(node);
                    }
                    result.Push(new CommonNode());
                    printStack(result, "8! ", debug);
                    break;
                }
        }

        return result;
    }

    private static Stack<INode> GetSolveForLowPriorityNodes(Stack<INode> stack, INode endPoint)
    {
        if (debug) Console.WriteLine("LowPriority");

        Stack<INode> result = stack;

        Stack<INode> lowPriority = new Stack<INode>();

        if (endPoint is CommonNode)
        {
            while (!(result.Peek() is CommonNode))
            {
                lowPriority.Push(result.Pop());
            }
            result.Pop();
        }

        if (endPoint is AssignNode)
        {
            while (!(result.Peek() is AssignNode))
            {
                lowPriority.Push(result.Pop());
            }

        }

        while (lowPriority.Count != 1)
        {
            INode leftNode = lowPriority.Pop();
            BinaryOperationNode operation = (BinaryOperationNode)lowPriority.Pop();
            INode rightNode = lowPriority.Pop();
            operation.LeftNode = leftNode;
            operation.RightNode = rightNode;
            lowPriority.Push(operation);
        }

        result.Push(lowPriority.Pop());

        return result;
    }

    private static char GetTypeOfCurrentToken(List<Token> tokens, int index) => tokens[index].Type;

    /*private static int TClass1(char c)
    {
        switch (c)
        {
            case 'n': return 2;
            case 't': return 3;
            case 'l': return 4;
            case 'r': return 5;
            case 'f': return 6;
            case 'a': return 7;
            case '$': return 8;
        }

        return -1;
    }*/

    private static int TClass(char c)
    {
        switch (c)
        {
            case 'i': case 'B': return 0;
            case 'u': case 'E': return 1;
            case 'n': return 2;
            case 't': return 3;
            case 'l': return 4;
            case 'r': return 5;
            case 'f': return 6;
            case 'a': return 7;
            case '$': return 8;
            case 'c': return 9;
        }

        return -1;
    }

    private static void printStack(Stack<INode> result, string str, bool needed)
    {
        if (!needed) return;
        Console.Write(str);
        var tmp = result.ToList();
        tmp.Reverse();
        foreach (var node in tmp)
        {
            Console.Write($"|{node}| ");
        }
        Console.WriteLine();
    }
}

class TreePrinter
{
    int spacesForOneLevel = 5;//10;

    int maxLevelOfCurrentTree;

    int[] offsetForLevels; 
    void PrintExpressionTree(INode tree, int level, int topPosition)
    {
        if (tree is VariableNode variable)
        {
            string value = variable.ToString();
            PrintStringInRightPlace(value, spacesForOneLevel * level, topPosition);
        }

        if (tree is NumberNode number)
        {
            string value = number.ToString();
            PrintStringInRightPlace(value, spacesForOneLevel * level, topPosition);
        }

        if (tree is AssignNode assign)
        {
            string value = " = ";
            PrintExpressionTree(assign.RightNode, level + 2, topPosition);
            PrintExpressionTree(assign.LeftNode, level, topPosition);
            PrintStringInRightPlace(value, spacesForOneLevel * (level + 1), topPosition);
        }

        if (tree is UnaryOperationNode operation)
        {
            string value = operation.Operation.Value;
            PrintExpressionTree(operation.Child, level + 1, topPosition);
            PrintStringInRightPlace(value, spacesForOneLevel * level, topPosition);

        }

        if (tree is BinaryOperationNode binaryOperation)
        {
            string value = binaryOperation.Operation.Value;
            int offset = offsetForLevels[level + 1];
            PrintExpressionTree(binaryOperation.LeftNode, level + 1, topPosition - offset);
            PrintStringInRightPlace(value + "\n", spacesForOneLevel * level, topPosition);
            PrintExpressionTree(binaryOperation.RightNode, level + 1, topPosition + offset);

        }

    }

    void PrintStringInRightPlace(string value, int cursorLeft, int cursorTop)
    {
        Console.SetCursorPosition(cursorLeft, cursorTop);
        Console.Write(value);
    }

    public void PrintTrees(List<INode> trees)
    {
        foreach (INode tree in trees)
        {
            maxLevelOfCurrentTree = GetMaxLevelOfTree(tree);
            offsetForLevels = GenerateArrayOfOffset(maxLevelOfCurrentTree);
            Console.WriteLine();
            PrintExpressionTree(tree, 0, Console.CursorTop + offsetForLevels[0]);
            Console.SetCursorPosition(Console.CursorLeft, Console.CursorTop + maxLevelOfCurrentTree);
            // offsetForLevels[0]);
        }

        Console.SetCursorPosition(Console.CursorLeft, Console.CursorTop + offsetForLevels[0]);
    }

    private int[] GenerateArrayOfOffset(int numberOfLevels)
    {
        int[] offsets = new int[numberOfLevels];
        offsets[numberOfLevels - 1] = 1;
        for (int i = numberOfLevels - 2; i >= 0; i--)
        {
            //offsets[i] = (int) (1.3 * offsets[i+1] + 4);
            offsets[i] = (offsets[i + 1] + 1);
        }

        return offsets;
    }

    int GetMaxLevelOfTree(INode tree)
    {
        if (tree is AssignNode assign)
        {
            return 2 + GetMaxLevelOfTree(assign.RightNode);
        }

        if (tree is UnaryOperationNode operation)
        {
            return 1 + GetMaxLevelOfTree(operation.Child);
        }

        if (tree is BinaryOperationNode binaryOperation)
        {
            return 1 + Math.Max(GetMaxLevelOfTree(binaryOperation.LeftNode), 
                GetMaxLevelOfTree(binaryOperation.RightNode));
        }

        return 1;
    }
}

struct Token
{
    public readonly char Type;
    public readonly string Value;
    public readonly int Line, Position;

    public Token(char type, string value, int line, int position)
    {
        (Type, Value, Line, Position) = (type, value, line, position);
    }

    public override string ToString() => $"Тип: {Type}  Значение: {Value,-8} Строка {Line} Позиция {Position}";
}

abstract class INode
{
    //public abstract string ToString();
}

class CommonNode : INode
{
    public override string ToString() => "";
}

class NumberNode : INode
{
    public Token Number;

    public NumberNode(Token number)
    {
        Number = number;
    }

    public override string ToString() => Number.Value;
}

class VariableNode : INode
{
    public Token Variable;

    public VariableNode(Token variable)
    {
        Variable = variable;
    }

    public override string ToString() => Variable.Value;
}

class AssignNode : INode
{
    public INode LeftNode;
    public INode RightNode;

    public AssignNode(INode leftNode, INode rightNode)
    {
        (LeftNode, RightNode) = (leftNode, rightNode);
    }

    public override string ToString() => $"{LeftNode} = {RightNode}";
}

class UnaryOperationNode : INode
{
    public Token Operation;
    public INode Child;

    public UnaryOperationNode(Token operation, INode child)
    {
        (Operation, Child) = (operation, child);
    }

    public override string ToString() => $"{Operation.Value} {Child}";
}

class BinaryOperationNode : INode
{
    public Token Operation;
    public INode LeftNode, RightNode;

    public BinaryOperationNode(Token operation, INode leftNode, INode rightNode)
    {
        (Operation, LeftNode, RightNode) = (operation, leftNode, rightNode);
    }

    public override string ToString() => $" {LeftNode} {Operation.Value} {RightNode}";
}


