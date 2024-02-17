using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace convert
{
    class Program
    {
        static void Main(string[] args)
        {
            List<string> strs = new();
            string jsonname = Console.ReadLine();
            using (StreamReader sr = new StreamReader(jsonname))
            {
                for (int i = 0; i < 5; i++)
                {
                    Console.WriteLine(sr.ReadLine());
                }
            }
        }
    }
}
