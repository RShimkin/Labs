using System;

namespace MC_1
{
    class Program
    {
        //начальное состояние
        static int cur_state = 1;

        //переход
        static int cur_shift = 0;      


        static int[,] matrix = new int[7, 7]
        {         
            { 123, 1, 2, 3, 1, 2, 3},          // s1
            { 123, 123, 4, 4, 1, 123, 123},    // s2
            { 123, 5, 123, 5, 123, 2, 123 },   // s3
            { 123, 6, 6, 123, 123, 123, 3 },   // s4
            { 123, 0, 0, 0, 123, 4, 4 },       // s5
            { 123, 0, 0, 0, 5, 123, 5 },       // s6
            { 123, 0, 0, 0, 6, 6, 123 }        // s7
        };  
         
        static void Main(string[] args)
        {            
            string[] inputString = { "a", "-a", "-a", "b", "c", "-c", "-c", "b", "-b", "a", "c", "a", "c", "a", "-a" };

            //string[] inputString = { "a", "-a" };

            //string[] inputString = { "a", "a" };     

            //string[] inputString = {"a"};

            for (int i = 0; i < inputString.Length; i++)
            {
                if (cur_state == 123) // error
                {
                    Console.WriteLine("Строка неверная");
                    return;
                }

                if (inputString[i] == "a") cur_shift = 1;

                else if (inputString[i] == "b") cur_shift = 2;

                else if (inputString[i] == "c") cur_shift = 3;

                else if (inputString[i] == "-a") cur_shift = 4;

                else if (inputString[i] == "-b") cur_shift = 5;

                else if (inputString[i] == "-c") cur_shift = 6;

                cur_state = matrix[cur_state, cur_shift];
            }

            Console.WriteLine("Строка верная");

        }
    }
}
