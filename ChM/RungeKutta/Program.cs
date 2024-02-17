// Численные методы. Лабораторная 2. Задание 1 вариант 5 - Формула Рунге-Кутта третьего порядка.
// Шимкин Роман 3 курс 62 группа

//Icod = -1 - начальное значение, продолжение работы
//Icod = 0 - найдено решение, соответствующее условию максимальной погрешности
//Icod = 1 - процесс решения прекращён, так как изменение размера шага не уменьшает погрешность
//Icod = 2 - процесс решения прекращён, так как размер шага стал предельно малым

using System;

namespace ChM2_lab2
{
    record Data
	  {
        public double X { get; set; }
        public double Y { get; set; }
        public double H { get; set; }
        public double Error { get; set; }
        public int Icod { get; set; }
	  }
    class Program
    {
        const double minPositiveNumber = 2.0E-15;
        const int maxrep = 5;
        static void Main(string[] args)
        {
            double A = 0.0, B = 4.0, H = 1.0;
            double maxError = 0.0000001, Yc = 0.0;
            var result = Task(Function, H, A, B, Yc, maxError);

            Console.WriteLine("Интегрирование\n");
            Console.WriteLine($"При А={A} B={B} H={H} \nМаксимальной погрешности = {maxError} \nНачальном условии Y(c)={Yc} \n");
            Console.WriteLine($"Результат:  {GetStatus(result.Icod)}");
            Console.WriteLine($"Конечная точка: {result.X}");
            Console.WriteLine($"Значение в конечной точке: {result.Y}");
            Console.WriteLine($"При шаге: {result.H}");
            Console.WriteLine($"С апостериорной погрешностью: {result.Error}");
        }

        static Data Task(Func<double, double, double> function, double H, double A, double B, double Yc, double maxError)
        {
            double macheps = GetMacheps();
            double prevError = 1.0, currError = Double.MaxValue, currH = H;
            int Icod = -1;

            double rezX = 0, rezY = 0, rezH = 0, rezError = 0;
            int counter = 0;

            //while (Icod == -1 && counter < maxrep)
            while (counter < maxrep)
            {
                double X = A, Y = Yc;
                double X2 = A, Y2 = Yc;

                GetLastXAndY(ref X, ref Y, currH, B, function); // вариант цикла с количеством шагов
                GetLastXAndY(ref X2, ref Y2, currH * 0.5, B, function);
                counter++;

                prevError = currError;
                currError = Math.Abs((Y2 - Y) / 7.0);

                if (currError <= maxError)
                {
                    Icod = 0;
                    rezX = X;
                    rezY = Y;
                    rezH = currH;
                    rezError = currError;
                    break;
                }
                else
                {
                    if (currError >= prevError) Icod = 1;
                    else
                    {
                        rezX = X;
                        rezY = Y;
                        rezH = currH;
                        rezError = currError;
                        currH = (currH * 0.5) * Math.Pow(maxError / currError, 0.25);
                        currH = (currH * 0.5);
                        currH = GetRightStep(A, B, currH);
                        if (currH < Math.Max(minPositiveNumber, macheps * Math.Max(Math.Abs(A), Math.Abs(B)))) Icod = 2;
                    }
                }
            }

            var data = new Data() { X = rezX, Y = rezY, H = rezH, Error = rezError, Icod = Icod };
            Console.WriteLine($"Итераций: {counter}/{maxrep}");
            return data;
        }

        static void GetLastXAndY(ref double X, ref double Y, double H, double B, Func<double, double, double> function)
        {
            long numberOfSteps = GetNumberOfSteps(X, B, H);
            for (long i = 0; i <= numberOfSteps; i++)
            {
                double K1, K2, K3;
                K1 = H * function(X, Y);
                K2 = H * function(X + H/3, Y + K1/3);
                K3 = H * function(X + H * 2/3, Y + K2 * 2/3);

                Y = Y + (K1 + 3.0 * K3) * 0.25;
                X += H;
            }
            X = X - H;
        }

        static double Function(double X, double Y) // примеры с разными данными функциями-производными
        {
            double result = 0.0;
            //if (X != 0) result = Y * Y - (Y / X); 
            //result = Y - X * X * (X - 3);
            //result = 1;
            //result = 2 * X;
            //result = 3 * X * X;
            //result = 4 * X * X * X;
            //result = 5 * X * X * X * X;
            result = 6 * X * X * X * X * X;
            //result = 7 * X * X * X * X * X * X;
            return result;
        }

        static double GetMacheps()
        {
            double R = 1.0;
            while (R + 1 > 1)
            {
                R = R / 2;
            }
            return R * 2;
        }

        static string GetStatus(int Icod)
        {
            string result = "";
            switch (Icod)
            {
                case 0:
                    {
                        result = "найдено решение, соответствующее условию максимальной погрешности";
                        break;
                    }
                case 1:
                    {
                        result = " процесс решения прекращён, так как изменение размера шага не уменьшает погрешность";
                        break;
                    }
                case 2:
                    {
                        result = "процесс решения прекращён, так как размер шага стал предельно малым";
                        break;
                    }
            }
            return result;
        }

        static double GetRightStep(double A, double B, double H)
        {
            double result = 0.0;
            long numberOfSteps = GetNumberOfSteps(A,B,H);
            if (H * numberOfSteps == B - A) result = H;
            else result = Convert.ToDouble((B - A) / (numberOfSteps + 1));
            return result;
        }

        static long GetNumberOfSteps(double A, double B, double H)
        {
            long numberOfSteps = (long)((B - A) / H);
            return numberOfSteps;
        }
    }
}