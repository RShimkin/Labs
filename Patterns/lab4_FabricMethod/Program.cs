using System;

namespace lab4_FabricMethod
{
    class Program
    {
        static void Main(string[] args)
        {
            Championship cup = new(3);
            cup.Play(true);
        }
    }
}
