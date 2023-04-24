// Шимкин Роман 62 группа, лаба 2 вариант 18

void Main()
{
    string encrypted = "", decrypted = "";
    string message = "", key = "BIKER";
    int k = 0;

    Console.WriteLine("Введите сообщение");
    var _ = Console.ReadLine();
    message = string.IsNullOrEmpty(_) ? "SHIFROVANIE" : _.ToUpper();

    foreach (char s in message)
    {
        encrypted += (char)((s + key[k % key.Length]) % 26 + 65);
        k++;
    }

    k = 0;
    foreach (char s in encrypted)
    {
        decrypted += (char)((s - key[k % key.Length] + 26) % 26 + 65);
        k++;
    }

    Console.WriteLine($"[{key}]: {message} -> {encrypted} -> {decrypted}");
}

Main();