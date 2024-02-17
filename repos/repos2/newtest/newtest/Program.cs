using System;
using System.Threading;
using System.Threading.Tasks;

namespace newtest
{

    class Program
    {
        async static Task Work(int val)
        {
            async Task Print(int val)
            {
                for (int i = 0; i < 5; i++)
                {
                    Console.WriteLine(val);
                    Thread.Sleep(500);
                }
            }
            await Print(val);
            Console.WriteLine($"{val} finished");
        }

        static void func(long? parameter)
		    {
      Console.WriteLine(parameter * 3);
		    }
        static void Main(string[] args)
        {
      //Work(1);
      //Work(2);
      //Work(3);
      func(3);
        }
    }
}
