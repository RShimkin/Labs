// Шимкин Роман, 62 группа, лаба 3, вариант 18

using System.Numerics;

Main();

void Main()
{
    int p = 11, q = 5;
    string openText = "BIKER";

    KeyGen keyGen = new(p, q);
    Key publicKey = keyGen.GetOpenKey();
    Key privateKey = keyGen.GetClosedKey();

    Console.WriteLine($"Открытый ключ: {publicKey}");
    Console.WriteLine($"Закрытый ключ: {privateKey}");

    Console.WriteLine($"Параметры: {keyGen.ParamsToStr()}");

    Console.WriteLine($"Сообщение: {openText}");

    Encryptor encryptor = new(publicKey);
    string encrypted = encryptor.Encrypt(openText);
    Console.WriteLine($"Зашифровано: {encrypted}");

    Decryptor decryptor = new(privateKey);
    string decrypted = decryptor.Decrypt(encrypted);
    Console.WriteLine($"Расшифровано: {decrypted}");
}

public struct Key
{
    public int fstVal, sndVal;

    override public string ToString() => $"({fstVal}, {sndVal})";
}

public class KeyGen
{
    int p, q, n, f, e, d;
    IPrimesGen primesGenerator;

    public KeyGen(int inp, int inq)
    {
        (p, q) = (inp, inq);
        n = p * q;
        f = (p - 1) * (q - 1);
        primesGenerator = new Eratosthenes();
        GenerateKeys();
    }

    void GenerateKeys()
    {
        List<int> primes = primesGenerator.GetPrimes();

        e = primes.Skip(1).First(x => f % x != 0 && x != 0);

        d = (primes.First(x => (f * x + 1) % e == 0 && (f * x + 1) != 0) * f + 1) / e;
    }

    public Key GetOpenKey()
    {
        Key openKey = new();
        (openKey.fstVal, openKey.sndVal) = (e,n);
        return openKey;
    }

    public Key GetClosedKey()
    {
        Key closedKey = new();
        (closedKey.fstVal, closedKey.sndVal) = (d,n);
        return closedKey;
    }

    public string ParamsToStr()
    {
        return $"p= {p}, q={q}, n={n}, f={f}, e={e}, d={d}";
    }
}

public class Encryptor
{
    Key publicKey;

    public Encryptor(Key publickey)
    {
        publicKey = publickey;
    }

    public string Encrypt(string message)
    {
        string encrypted = "";
        int len = message.Length;
        foreach (int i in Enumerable.Range(0,len))
        {
            int num = message[i] - 'A' + 1;
            encrypted += BigInteger.Pow(num, publicKey.fstVal) % publicKey.sndVal + ",";
        }
        return encrypted;
    }
}

public class Decryptor
{
    Key privateKey;

    public Decryptor(Key privateKey)
    {
        this.privateKey = privateKey;
    }

    public string Decrypt(string message)
    {
        string decrypted = "";
        List<string> symbols = message.Split(',').ToList();
        foreach (string symbol in symbols.Where(x => x != ""))
        {
            int num = int.Parse(symbol);
            decrypted += (char)((BigInteger.Pow(num, privateKey.fstVal) % privateKey.sndVal) + 'A' - 1);
        }
        return decrypted;
    }
}

public interface IPrimesGen
{
    public List<int> GetPrimes();
}

public class Eratosthenes : IPrimesGen
{
    public List<int> GetPrimes()
    {
        const int max = 10000;
        int[] numbers = Enumerable.Range(0, max).ToArray();

        int primeTop = (int) Math.Sqrt(max);
        foreach (int i in Enumerable.Range(2,primeTop))
            for (int j = 2 * i; j < max; j += i)
                numbers[j] = 0;

        List<int> primes = new();
        foreach (int i in Enumerable.Range(0,max))
            if (numbers[i] != 0) 
                primes.Add(numbers[i]);

        return primes;
    }
}