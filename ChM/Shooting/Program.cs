//Решение краевой задачи для ОДУ второго порядка методом пристрелки.
// Задание 1 вариант 7 
// Шимкин Роман 3 курс 62 группа 

//Значения кодов
// IER = -1 - продолжение работы
// IER = 0 - завершение работы без ошибки
// IER = 1 - превышено число итераций

using System;

namespace СhM2_lab3
{
  class Program
  {
    static double
        a = 0.0, b = 4.0, A = 0.0, B = 4096,
        startParameter = 100.0, maxError = 0.0000001, deltaForNewParameter = 1000.0;

    static int N = 10, K = 100;

    static void Main(string[] args)
    {
      Console.WriteLine($"Отрезок а = {a}  b = {b}\n" +
          $"Краевые условия Y'(a) = {A} Y(b) = B = {B}\n" +
          $"Стартовый параметр = {startParameter}\n" +
          $"Максимальная погрешность = {maxError}\n" +
          $"Число отрезков разбиения = {N}\n" +
          $"Предельное число итераций = {K}\n" +
          $"Размер шага поиска нового параметра = {deltaForNewParameter}\n"
          );


      var result = Task();

      Console.WriteLine($"Код завершения {result.IER}\n" +
          $"Конечное значение пристрелочного параметра {result.rightParameter}\n" +
          $"Количество итераций {result.numberOfIterations}\n" +
          $"\nТаблица значений {result.results}");
    }

    static (int numberOfIterations, double rightParameter, int IER, string results) Task()
    {
      double parameter = startParameter, parameter1 = 1.0;
      int numberOfIterations = 0;
      int IER = -1;

      double currentError = Math.Abs(GetError(parameter));
      if (currentError <= maxError) IER = 0;
      else
      {
        double delta = deltaForNewParameter; // шаг параметра
        if (currentError < Math.Abs(GetError(parameter + delta))) delta = -delta;
        parameter1 = parameter + delta;
        while (GetError(parameter) * GetError(parameter1) > 0) // отрезов для МПД
        {
          parameter = parameter1;
          parameter1 = parameter1 + delta;
        }

        while (IER == -1 && numberOfIterations < K)
        {
          if (Math.Abs(parameter1 - parameter) <= 2 * maxError) // малый отрезок 
          {
            parameter = (parameter + parameter1) / 2;
            IER = 0;
          }
          else if (Math.Abs(GetError(parameter)) <= maxError) IER = 0; // условие погрешности
          else
          {
            double tmp = (parameter + parameter1) / 2; // новый отрезок
            if (GetError(parameter) * GetError(tmp) < 0)
            {
              parameter1 = tmp;
            }
            else parameter = tmp;

          }
          numberOfIterations++;
        }

        if (numberOfIterations == K) IER = 1;
      }

      var result = (numberOfIterations: numberOfIterations, rightParameter: parameter, IER: IER, result: GetResult(parameter));
      return result;
    }

    static double GetLastY(double a, double b, double A, double parameter, int N, Func<double, double, double, double> function)
    {
      double X = a;
      double H = (b - a) / N;
      double Y = parameter; // значения искомой функции
      double Y1 = A; //значения первой производной

      for (long i = 0; i < N; i++)
      {
        double K1, K2, K3, K4; // функция
        double L1, L2, L3, L4; // 1 производная (работа с параметром)

        K1 = H * Y1;
        L1 = H * function(X, Y, Y1);

        K2 = H * (Y1 + L1 * 0.5);
        L2 = H * function(X + H * 0.5, Y + K1 * 0.5, Y1 + L1 * 0.5);

        K3 = H * (Y1 + L2 * 0.5);
        L3 = H * function(X + H * 0.5, Y + K2 * 0.5, Y1 + L2 * 0.5);

        K4 = H * (Y1 + L3);
        L4 = H * function(X + H, Y + K3, Y1 + L3);

        Y1 = Y1 + (L1 + 2.0 * L2 + 2.0 * L3 + L4) / 6.0;
        Y = Y + (K1 + 2.0 * K2 + 2.0 * K3 + K4) / 6.0;

        X += H;
      }

      return Y;
    }

    static string GetResult(double parameter)
    {
      string resultY1 = "\n";
      string resultY = "\n";
      double X = a;
      double H = (b - a) / N;
      double Y = parameter; // значения искомой функции
      double Y1 = A; //значения первой производной

      for (long i = 0; i < N; i++)
      {
        double K1, K2, K3, K4; // отвечают за 1 производную (работа с параметром)
        double L1, L2, L3, L4; // отвечают за искомую функцию

        K1 = H * Y1;
        L1 = H * Deriv2(X, Y, Y1);

        K2 = H * (Y1 + L1 * 0.5);
        L2 = H * Deriv2(X + H * 0.5, Y + K1 * 0.5, Y1 + L1 * 0.5);

        K3 = H * (Y1 + L2 * 0.5);
        L3 = H * Deriv2(X + H * 0.5, Y + K2 * 0.5, Y1 + L2 * 0.5);

        K4 = H * (Y1 + L3);
        L4 = H * Deriv2(X + H, Y + K3, Y1 + L3);

        Y1 = Y1 + (L1 + 2.0 * L2 + 2.0 * L3 + L4) / 6.0;
        Y = Y + (K1 + 2.0 * K2 + 2.0 * K3 + K4) / 6.0;

        X += H;

        resultY1 += $"X = {X,22} | Y' = {Y1,22} | delta Y' = {Math.Abs(Y1 - Deriv(X, Y)),22} | \n";
        resultY += $"X = {X,22} | Y = {Y,22} | delta Y = {Math.Abs(Y - Function(X)),22}\n";
      }

      return resultY1 + "\n" + resultY;
    }

    static double GetError(double parameter)
    {
      return GetLastY(a, b, A, parameter, N, Deriv2) - B;
    }

    static double Deriv2(double X, double Y, double Y1)
    {
      double result = 0.0;
      //result = 6 * X;
      result = 30 * X * X * X * X;
      //result = 6 * Y - 2 * Y1 + 6 * X* (X - X * X + 1);
      return result;
    }

    static double Deriv(double X, double Y) // для теста
    {
      double result = 0.0;
      //result = 3 * X * X;
      //result = 5 * X * X * X * X;
      result = 6 * X * X * X * X * X;
      //result = Y - X * X * (X - 3);
      return result;
    }

    static double Function(double X) // для теста
    {
      double result = 0.0;
      //result = X * X * X + 1;
      //result = X * X * X * X * X;
      result = X * X * X * X * X * X;
      return result;
    }
  }
}

