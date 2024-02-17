using System;

namespace lab5_adapter
{
    class Program
    {
        static void Main(string[] args)
        {
            DigitalAdapter dig = new();
            //dig.SetTime("13:45:01");
            Console.WriteLine(dig);
            char x = Console.ReadKey().KeyChar;
            Console.Write('\b');
            while (x != 'q')
            {
                switch (x)
                {
                    case 'w': case 'W':
                        dig.AddHour();
                        Console.WriteLine(dig);
                        break;
                    case 's': case 'S':
                        dig.SubHour();
                        Console.WriteLine(dig);
                        break;
                    case 'e': case 'E':
                        dig.AddMinute();
                        Console.WriteLine(dig);
                        break;
                    case 'd': case 'D':
                        dig.SubMinute();
                        Console.WriteLine(dig);
                        break;
                    case 'r': case 'R':
                        dig.AddSecond();
                        Console.WriteLine(dig);
                        break;
                    case 'f': case 'F':
                        dig.SubSecond();
                        Console.WriteLine(dig);
                        break;
                }
                x = Console.ReadKey().KeyChar;
                Console.Write('\b');
            }
            //for (int i = 0; i < 10; i++) {Console.WriteLine(dig.GetTime());Thread.Sleep(5000);}
        }
    }
}
