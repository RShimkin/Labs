using System.Text;

#region Algorithms
(List<int>, int) Naive(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Naive");

    int counter = 0;
    List<int> result = new();

    text = text.ToLower();
    pattern = pattern.ToLower();

    int lenght = pattern.Length, last = text.Length - lenght + 1;

    for (int i = 0, j = 0; i < last; i++, j = 0)
    {
        bool possible = true;

        while (possible && j < lenght)
        {
            counter++;

            if (print) PrintCompare(text, pattern, i + j, j, counter);
            if (debug)
                Console.WriteLine($"text[{i + j}] > {text[i + j]} ? {pattern[j]} < pattern[{j}] #{counter}");

            if (text[i + j] == pattern[j])
                j++;
            else possible = false;
        }

        if (j == lenght) result.Add(i);
    }

    return (result, counter);
}

/*(List<int>, int) KMP(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Knuth-Morris-Pratt\n");

    int counter = 0;
    List<int> result = new List<int>();
    text = text.ToLower();
    pattern = pattern.ToLower();
    int tLength = text.Length, pLength = pattern.Length;
    int[] offsets = BuildOffsets(pattern, debug);
    if (print)
        Console.WriteLine($"offsets: {ArrayOfIntToString(offsets)}");

    int i = 0, j = 0;
    if (debug) Console.WriteLine($"i({i}) <= {tLength - pLength + j}");
    while (i <= tLength - pLength + j)
    {
        bool possible = true;
        while (possible && j < pLength)
        {
            counter++;
            if (print) PrintCompare(text, pattern, i, j, counter);
            if (text[i] == pattern[j])
            {
                if (debug)
                    Console.WriteLine($"text[{i}] ({text[i]}) == ({pattern[j]}) pattern[{j}]  ->  {i}++, {j}++  #{counter}");
                i++;
                j++;
            }
            else
            {
                if (debug)
                    Console.WriteLine($"text[{i}] ({text[i]}) != ({pattern[j]}) pattern[{j}]  ->  possible = false  #{counter}");
                possible = false;
            }
        }

        if (j == pLength)
        {
            if (debug)
                Console.WriteLine($"j ({j}) == pLength -> i-pLength ({i - pLength}) !");
            result.Add(i - pLength);
        }

        if (j == 0)
        {
            if (debug)
                Console.WriteLine($"j == 0 -> {i}++");
            i = i + 1;
        }
        else
        {
            if (debug)
                Console.WriteLine($"j = offsets[{j}]-1 ({offsets[j] - 1})");
            j = offsets[j] - 1;
        }
    }

    return (result, counter);

    int[] BuildOffsets(string pattern, bool debug = false)
    {
        string tmpPattern = "$" + pattern;
        int patternLenght = tmpPattern.Length;
        int[] indexes = new int[patternLenght];

        indexes[0] = 0;
        indexes[1] = 1;

        for (int i = 2; i < patternLenght; i++)
        {
            if (debug)
                Console.WriteLine($"currentIndex = indexes[i-1] == indexes[{i - 1}] = {indexes[i - 1]}");
            int currentIndex = indexes[i - 1];
            if (debug)
                Console.WriteLine($"{tmpPattern}[{currentIndex}] > {tmpPattern[currentIndex]} " +
                    $"? {tmpPattern[i]} < {tmpPattern}[{i}]");
            while (currentIndex > 1 && tmpPattern[currentIndex] != tmpPattern[i])
            {
                if (debug)
                    Console.WriteLine($"currentIndex ({currentIndex}) = indexes[currentIndex - 1] ({indexes[currentIndex - 1]})");
                currentIndex = indexes[currentIndex - 1];
            }

            if (tmpPattern[currentIndex] == tmpPattern[i])
            {
                if (debug)
                    Console.WriteLine($"{tmpPattern[currentIndex]} == {tmpPattern[i]} -> indexes[{i}] = currentIndex + 1 = {currentIndex + 1}");
                indexes[i] = currentIndex + 1;
            }
            else
            {
                if (debug)
                    Console.WriteLine($"{tmpPattern[currentIndex]} != {tmpPattern[i]} -> indexes[{i}] = {1}");
                indexes[i] = 1;
            }
        }

        return indexes;
    }
}*/

(List<int>, int) KMP(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Knuth-Morris-Pratt\n");

    int counter = 0;
    List<int> result = new List<int>();
    text = text.ToLower();
    pattern = pattern.ToLower();
    int tLength = text.Length, pLength = pattern.Length;
    int[] offsets = BuildOffsets(pattern, debug);
    if (print)
        Console.WriteLine($"offsets: {ArrayOfIntToString(offsets)}");

    int i = 0, j = 0;
    while (i <= tLength - pLength + j)
    {
        bool possible = true;
        while (possible && j < pLength)
        {
            counter++;
            if (print) PrintCompare(text, pattern, i, j, counter);
            if (text[i] == pattern[j])
            {
                i++;
                j++;
            }
            else
            {
                possible = false;
            }
        }

        if (j == pLength)
        {
            result.Add(i - pLength);
        }

        if (j == 0)
        {
            i = i + 1;
        }
        else
        {
            j = offsets[j] - 1;
        }
    }

    return (result, counter);

    int[] BuildOffsets(string pattern, bool debug = false)
    {
        string tmpPattern = "$" + pattern;
        int patternLenght = tmpPattern.Length;
        int[] indexes = new int[patternLenght];

        indexes[0] = 0;
        indexes[1] = 1;

        for (int i = 2; i < patternLenght; i++)
        {
            int currentIndex = indexes[i - 1];
            while (currentIndex > 1 && tmpPattern[currentIndex] != tmpPattern[i])
            {
                currentIndex = indexes[currentIndex - 1];
            }

            if (tmpPattern[currentIndex] == tmpPattern[i])
            {
                indexes[i] = currentIndex + 1;
            }
            else
            {
                indexes[i] = 1;
            }
        }

        return indexes;
    }
}

(List<int>, int) KMPExtra(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Knuth-Morris-Pratt*");

    int counter = 0;
    List<int> result = new List<int>();

    text = text.ToLower();
    pattern = pattern.ToLower();

    int tLenght = text.Length, pLenght = pattern.Length;

    int[] offsets = BuildOffsets(pattern);
    int[] betterOffsets = BuildExtraOffsets(pattern, offsets);
    if (print)
    {
        Console.WriteLine($"      offsets: {ArrayOfIntToString(offsets)}");
        Console.WriteLine($"extra offsets: {ArrayOfIntToString(betterOffsets)}");
    }

    int i = 0, j = 0;
    while (i <= tLenght - pLenght + j)
    {
        bool possible = true;
        while (possible && j < pLenght)
        {
            counter++;
            if (print) PrintCompare(text, pattern, i, j, counter);
            if (text[i] == pattern[j])
            {
                i++;
                j++;
            }
            else possible = false;
        }

        if (j == pLenght) result.Add(i - pLenght);
        if (j == 0) i = i + 1;
        else j = betterOffsets[j] - 1;
    }


    return (result, counter);

    int[] BuildOffsets(string str)
    {
        string tmpPattern = "$" + str;
        int lenght = tmpPattern.Length;
        int[] indexes = new int[lenght];

        indexes[0] = 0;
        indexes[1] = 1;

        for (int i = 2; i < lenght; i++)
        {
            int cur = indexes[i - 1];
            while (cur > 1 && tmpPattern[cur] != tmpPattern[i])
            {
                cur = indexes[cur - 1];
            }

            if (tmpPattern[cur] == tmpPattern[i]) indexes[i] = cur + 1;
            else indexes[i] = 1;
        }


        return indexes;
    }

    int[] BuildExtraOffsets(string ptrn, int[] ind)
    {
        string tmpPattern = ptrn + "$";
        int lenght = tmpPattern.Length;
        int[] indexes = new int[lenght];

        //goodIndexes[0] = 0; //так как 0 тоже приводит к первому символу паттерна
        indexes[0] = 1;

        for (int i = 1; i < lenght; i++)
        {
            if (tmpPattern[i] == tmpPattern[ind[i] - 1])
            {
                indexes[i] = indexes[ind[i] - 1];
            }
            else
            {
                indexes[i] = ind[i];
            }
        }

        return indexes;
    }
}

(List<int>, int) BM(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Boyer-Moore\n");

    int counter = 0;
    List<int> result = new List<int>();

    text = text.ToLower();
    pattern = pattern.ToLower();

    int tLenght = text.Length, pLenght = pattern.Length;

    var (symbolDict, symbolMax) = ProcessSymbols(pattern);
    var suffixes = BuildSuffixes(pattern);
    if (print)
    {
        Console.WriteLine($"symbolDict {DictToArray(symbolDict)}  max {symbolMax}");
        Console.WriteLine($"suffixes   {ArrayOfIntToString(suffixes)}");
    }

    string tmpText = "$" + text, tmpPattern = "&" + pattern;

    int i = pLenght;
    while (i <= tLenght)
    {
        int j = pLenght;
        bool possible = true;
        while (possible)
        {
            counter++;
            if (print)
            {
                PrintCompare(tmpText, tmpPattern, i, j, counter);
            }
            if (tmpText[i] == tmpPattern[j])
            {
                if (debug)
                    Console.WriteLine($"tmptext[{i}] ({tmpText[i]}) == ({tmpPattern[j]}) tmppattern[{j}] -> {i}++, {j}++ #{counter}");
                j--;
                i--;
            }
            else
            {
                if (debug)
                    Console.WriteLine($"tmptext[{i}] ({tmpText[i]}) != ({tmpPattern[j]}) tmppattern[{j}] -> !{possible} #{counter}");
                possible = false;
            }
        }

        if (j == 0)
        {
            if (debug) Console.WriteLine($"{i} !");
            result.Add(i);
        }
        if (debug)
            Console.Write($"i ({i}) = i + Max(GetShiftBySymbol(symbolDict, symbolMax, {tmpText[i]}), {suffixes[j]}) = ");
        i = i + Math.Max(GetShiftBySymbol(symbolDict, symbolMax, tmpText[i]), suffixes[j]);
        if (debug)
            Console.Write($"{i}\n");
    }

    return (result, counter);

    (Dictionary<char, int>, int) ProcessSymbols(string ptrn)
    {
        var dict = new Dictionary<char, int>();

        int cur = 0;
        for (int i = ptrn.Length - 1; i >= 0; i--)
        {
            if (!dict.ContainsKey(ptrn[i]))
            {
                dict.Add(ptrn[i], cur);
            }
            cur++;
        }

        return (dict, cur);
    }

    int GetShiftBySymbol(Dictionary<char, int> dict, int max, char sym) => dict.ContainsKey(sym) ? dict[sym] : max;

    int[] BuildSuffixes(string ptrn)
    {
        int plen = ptrn.Length, ilen = ptrn.Length + 1;
        var indexes = new int[ilen];

        int[] suffixShifts = GetArrayOfSuffixShifts(ptrn);
        int[] prefixShifts = GetArrayOfPrefixShifts(ptrn);

        if (print)
        {
            Console.WriteLine($"suffixShifts:       {ArrayOfIntToString(suffixShifts)}");
            Console.WriteLine($"prefixShifts:       {ArrayOfIntToString(prefixShifts)}");
        }

        for (int i = 0; i < ilen; i++)
        {
            if (suffixShifts[i] != 0)
            {
                if (debug)
                {
                    Console.WriteLine($"sShifts[{i}] ({suffixShifts[i]}) != 0  " +
                        $"-> ind[{i}] ({indexes[i]}) = plen - sShifts[i] = {plen} - {suffixShifts[i]}");
                }
                indexes[i] = plen - suffixShifts[i];
            }
            else
            {
                if (debug)
                {
                    Console.WriteLine($"sShifts[{i}] ({suffixShifts[i]}) == 0  " +
                        $"-> ind[{i}] ({indexes[i]}) = 2*plen - pShifts[i] - i = {2 * plen} - {prefixShifts[i]} - {i}");
                }
                indexes[i] = 2 * plen - prefixShifts[i] - i;
            }
        }

        return indexes;
    }

    int[] GetArrayOfSuffixShifts(string ptrn)
    {
        int plen = ptrn.Length, arrlen = ptrn.Length + 1;
        int[] shifts = new int[arrlen];
        shifts[0] = 0;

        //string reversed = ReversePattern(ptrn);
        string reversed = new(ptrn.Reverse().ToArray());
        if (print) Console.WriteLine($"reversed:        {reversed}");
        int[] borders = GetArrayOfPatternBorders(reversed);
        if (print) Console.WriteLine($"borders:     {ArrayOfIntToString(borders)}");
        Array.Reverse(borders);
        if (print) Console.WriteLine($"borders rev:     {ArrayOfIntToString(borders)}");

        for (int i = 1; i < plen - 1; i++)
        {
            if (borders[i] <= borders[i + 1])
            {
                int q = borders[i + 1];
                shifts[arrlen - q - 1] = i + 1;
                if (debug)
                {
                    Console.WriteLine($"borders[{i}] ({borders[i]}) <=  ({borders[i + 1]}) [{i + 1}]  -> " +
                        $"sshifts[arrlen-q-1] = sshifts[{arrlen}-{q}-1] = i+1 = {i + 1}");
                }
                while (q > borders[i])
                {
                    if (debug)
                    {
                        Console.WriteLine($"q ({q}) > ({borders[i]}) borders[{i}]  -> " +
                            $"q = {borders[plen - q]}, sshifts[{arrlen - q - 1}] = i+1 = {i + 1}");
                    }
                    q = borders[plen - q];
                    shifts[arrlen - q - 1] = i + 1;
                }
            }

        }

        return shifts;
    }

    int[] GetArrayOfPatternBorders(string ptrn)
    {
        int plen = ptrn.Length;
        int[] borders = new int[ptrn.Length];

        borders[0] = 0;

        for (int i = 1; i < plen; i++)
        {
            int border = borders[i - 1];
            while (border > 0 && ptrn[border] != ptrn[i])
            {
                border = borders[border - 1];
            }

            if (ptrn[border] == ptrn[i]) borders[i] = border + 1;
            else borders[i] = 0;
        }

        return borders;
    }

    int[] GetArrayOfPrefixShifts(string ptrn)
    {
        int arrlen = ptrn.Length + 1;
        int[] shifts = new int[arrlen];

        int[] borders = GetArrayOfPatternBorders(ptrn);
        if (print) Console.WriteLine($"borders:       {ArrayOfIntToString(borders)}");

        int i = 0;
        int border = ptrn.Length;
        while (i < arrlen)
        {
            border = borders[border - 1];
            int num = arrlen - border;
            while (i < num)
            {
                shifts[i] = border;
                i++;
            }
        }

        return shifts;
    }

}

(List<int>, int) BMH(string text, string pattern, bool debug, bool print)
{
    Console.WriteLine("Boyer-Moore-Horspool\n");

    int pLength = pattern.Length, tLength = text.Length;
    Dictionary<char, int> dict = new();
    List<int> result = new();
    int counter = 0;

    for (int i = pLength-2; i >= 0; i--)
    {
        if (!dict.ContainsKey(pattern[i]))
        {
            dict[pattern[i]] = pLength - i - 1;
        }
    }

    if (!dict.ContainsKey(pattern[^1]))
    {
        dict[pattern[pLength - 1]] = pLength;
    }

    dict['*'] = pLength;

    if (print) Console.WriteLine($"dict: {DictToArray(dict)}");

    if (tLength >= pLength)
    {
        int i = pLength - 1;
        while (i < tLength)
        {
            int k = 0, j = 0;
            bool unequal = false;
            int offset = 0;
            for (j = pLength - 1; j >= 0; j--)
            {
                counter++;
                if (print)
                {
                    PrintCompare(text, pattern, i - k, j, counter);
                }
                if (text[i-k] != pattern[j])
                {
                    if (j == pLength - 1)
                    {
                        offset = dict.ContainsKey(text[i]) ? dict[text[i]] : dict['*'];
                    }
                    else
                    {
                        offset = dict[pattern[j]];
                    }

                    i += offset;
                    unequal = true;
                    break;
                }

                k += 1;
            }

            if (!unequal)
            {
                result.Add(i - k + 1);
                i = i + dict[pattern[^1]];
            }
        }
    }

    return (result, counter);
}

int WagnerFischer(string one, string two)
{
    one = one.ToLower();
    two = two.ToLower();
    Console.WriteLine(one);
    Console.WriteLine(two);
    int m = one.Length + 1, n = two.Length + 1;
    List<List<int>> d = new();

    for (int i = 0; i < m; i++)
    {
        d.Add(new List<int>());
        for (int j = 0; j < n; j++)
        {
            d[i].Add(0);
        }
        d[i][0] = i;
    }
    for (int k = 0; k < n; k++)
    {
        d[0][k] = k;
    }
    d[0][0] = 0;

    for (int i = 1; i < m; i++)
    {
        for (int j = 1; j < n; j++)
        {
            int cost = (one[i - 1] == two[j - 1]) ? 0 : 1;

            d[i][j] = Math.Min(d[i - 1][j] + 1, Math.Min(d[i][j - 1] + 1, d[i - 1][j - 1] + cost));
        }
    }

    foreach (var row in d)
    {
        Console.WriteLine(ArrayOfIntToString(row.ToArray()));
    }

    return d[m - 1][n - 1];
}

#endregion

#region Helpers
void PrintCompare(string text, string pattern, int t, int p, int counter)
{
    int offset = t - p;
    string ts = $"{(t > 0 ? text[..t] : "")}{text[t..(t + 1)].ToUpper()}{text[(t + 1)..]}   #{counter} text[{t}] pattern[{p}]";
    string ps = string.Concat(Enumerable.Repeat(" ", offset))
        + $"{(p > 0 ? pattern[..p] : "")}{pattern[p..(p + 1)].ToUpper()}{pattern[(p + 1)..]}";
    Console.WriteLine(ts);
    Console.WriteLine(ps);
}

string GetFibString(string firstString, string secondString, int numberOfRequiredString)
{
    string result = String.Empty;
    if (numberOfRequiredString >= 0)
    {
        if (numberOfRequiredString == 0) result = firstString;
        else if (numberOfRequiredString == 1) result = secondString;
        else
        {
            string prevPrevString = firstString;
            string prevString = secondString;
            string currentString = prevString + prevPrevString;

            for (int i = 2; i < numberOfRequiredString; i++)
            {
                prevPrevString = prevString;
                prevString = currentString;
                currentString = prevString + prevPrevString;
            }

            result = currentString;
        }
    }

    return result;
}

string ArrayOfIntToString(int[] array)
{
    StringBuilder result = new("");
    foreach (int i in array)
    {
        result.Append($" {i}");
    }

    return result.ToString().Trim();
}

string DictToArray<T, V>(Dictionary<T, V> dict)
{
    StringBuilder result = new();
    foreach (var pair in dict)
    {
        result.Append($"{pair.Key}:{pair.Value} ");
    }
    return result.ToString();
}

#endregion 

Main();
void Main()
{
    List<(string, string)> data = new();
    //data.Add(("bgffaabaabaaaaaababbaaba", "aaba"));
    //data.Add(("aaaaaaaaaaaaaaaaaa", "aaaaaa"));
    //data.Add(("aaaabbbbaaaabbbbaa", "aaabbb"));
    //data.Add(("aabaabaacaabaabaab", "aabaab"));
    //data.Add(("abcde", "aaa"));
    //data.Add(("abcatevaerweabcabcwefabqwe", "abcab"));
    //data.Add(("dbcatevaerweabcabcwefabqwe", "abcabd"));
    //data.Add(("abcdabcababcabdabbbbbbbabcabc", "abcabd"));
    //data.Add(("aabacaba", "aba"));
    //data.Add(("ababacababab", "abab"));
    //data.Add(("ababacababab", "abab"));
    //data.Add(("aaaabbbbaaaabbbbaa", "aaabbb"));
    //data.Add(("aaaabbbbaaaabbbbaa", "abcabcd"));
    //data.Add(("abcdeabababa", "ababa"));
    data.Add(("ababacabaababab", "abab"));

    //text = GetFibString("b", "a", 7);

    List<(Func<string, string, bool, bool, (List<int>, int)>, bool, bool)> algos = new();
    //algos.Add((Naive, false, false));
    //algos.Add((KMP, false, true));
    algos.Add((KMPExtra, false, true));
    algos.Add((BM, false, true));
    algos.Add((BMH, false, true));

    foreach (var datapair in data)
    {
        var (text, pattern) = datapair;
        Console.WriteLine($"\ntext: \n{text}");
        Console.WriteLine($"pattern: \n{pattern}\n");
        foreach (var tup in algos)
        {
            var (algo, debug, print) = tup;
            var (indexes, counter) = algo(text, pattern, debug, print);
            Console.WriteLine($"\nindexes: {ArrayOfIntToString(indexes.ToArray())}");
            Console.WriteLine($"comparisons: {counter}\n");
        }
    }

    //Console.WriteLine(WagnerFischer("kitten", "sitting"));
    //Console.WriteLine(WagnerFischer("rests", "stress"));
    //Console.WriteLine(WagnerFischer("Kirill", "Roman"));
    //Console.WriteLine(WagnerFischer("аасаб", "баса"));
}