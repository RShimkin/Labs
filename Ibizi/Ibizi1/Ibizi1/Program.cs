// Шимкин Роман, 62 группа, Лаба 1, вариант 18 

Main();

void Main()
{
    string key1 = "54213", key2 = "12453";
    string message = "ШимкинРоманШифрование!!!!";
    Console.WriteLine($"Ключи: {key1} {key2}");
    Console.WriteLine($"Сообщение: {message}");

    Cryptor cryptor = new(key1, key2);
    (string encrypted, char[,] matrix) = cryptor.Encrypt(message);
    PrintMatrix(matrix, "Матрица шифрования:");
    Console.WriteLine($"Зашифрованный текст: {encrypted}");

    (string decrypted, _) = cryptor.Decrypt(encrypted);
    Console.WriteLine($"Расшифрованный текст: {decrypted}");

    void PrintMatrix(char[,] matrix, string title = "")
    {
        Console.WriteLine(title);
        for (int i = 0; i < matrix.GetLength(0); i++, Console.WriteLine())
            for (int j = 0; j < matrix.GetLength(1); j++)
                Console.Write($"{matrix[i, j]} ");
        Console.WriteLine();
    }
}

public class Cryptor
{
    string firstKey, secondKey;
    int fstLen, sndLen;

    public Cryptor(string firstKey, string secondKey)
    {
        (this.firstKey, this.secondKey) = (firstKey, secondKey);
        (fstLen, sndLen) = (firstKey.Length, secondKey.Length);
    }

    public (string, char[,]) Encrypt(string message)
    {
        char[,] encryptionMatrix = GetEncryptionMatrix(message);
        string encryptedMessage = GetEncrypted(encryptionMatrix);
        return (encryptedMessage, encryptionMatrix);
    }

    private char[,] GetEncryptionMatrix(string message)
    {
        char[,] encryptionMatrix = new char[fstLen, sndLen];

        for (int i = 0; i < fstLen; i++)
        {
            int currentRow = (firstKey[i] - '0') - 1;
            for (int j = 0; j < sndLen; j++)
            {
                encryptionMatrix[currentRow, j] = message[i * fstLen + j];
            }
        }


        return encryptionMatrix;
    }

    private string GetEncrypted(char[,] encryptionMatrix)
    {
        string encryptedMessage = "";

        for (int i = 0; i < sndLen; i++)
        {
            int currentColumn = (secondKey[i] - '0') - 1;
            for (int j = 0; j < fstLen; j++)
            {
                encryptedMessage += encryptionMatrix[j, currentColumn];
            }
        }

        return encryptedMessage;
    }

    public (string, char[,]) Decrypt(string message)
    {
        char[,] matrix = GetDecryptionMatrix(message);
        string decrypted = GetDecrypted(matrix);
        return (decrypted, matrix);
    }

    private char[,] GetDecryptionMatrix(string message)
    {
        char[,] decryptionMatrix = new char[fstLen, sndLen];

        for (int i = 0; i < sndLen; i++)
        {
            int currentColumn = (secondKey[i] - '0') - 1;
            for (int j = 0; j < fstLen; j++)
            {
                decryptionMatrix[j, currentColumn] = message[i * sndLen + j];
            }
        }

        return decryptionMatrix;
    }

    private string GetDecrypted(char[,] decryptionMatrix)
    {
        string decryptedMessage = "";

        for (int i = 0; i < fstLen; i++)
        {
            int currentRow = (firstKey[i] - '0') - 1;
            for (int j = 0; j < sndLen; j++)
            {
                decryptedMessage += decryptionMatrix[currentRow, j];
            }
        }

        return decryptedMessage;
    }
}