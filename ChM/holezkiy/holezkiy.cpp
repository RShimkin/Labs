
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>

#define RANGE 10
#define DIVIDER 100

using namespace std;

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void EnteringBadNumbers(double* A, double* f, int N, int L)
{
    for (int i = 0; i < N; i++)
    {
        f[i] = fRand(-RANGE, RANGE);
    }

    int numberOfZeros = L - 1;
    for (int i = 0; i < N; i++)
    {
        int j = 0;
        for (j; j < numberOfZeros; j++)
        {
            A[i * L + j] = 0;
        }

        for (j; j < L; j++)
        {
            A[i * L + j] = 1.0 / (i + j + 1);
        }

        numberOfZeros -= 1;
    }
}

void EnteringColumnOfFreeMembers(double* f, int N)
{
    for (int i = 0; i < N; i++)
    {
        f[i] = fRand(-RANGE, RANGE);
    }
}

void EnteringGoodRandomNumbersUp(double* A, int N, int L)
{
    int numberOfZeros = L - 1;
    for (int i = N - 1; i >= 0; i--)
    {
        int j = L - 1;
        for (j; j >= L - numberOfZeros; j--)
        {
            A[i * L + j] = 0;
        }

        for (j; j > 0; j--)
        {
            A[i * L + j] = fRand(-RANGE, RANGE);
        }

        for (int k = 0; k < N; k++)
        {
            A[k * L] = 1;
        }

        numberOfZeros -= 1;
    }
}

void EnteringGoodRandomNumbersDown(double* A, int N, int L)
{
    int numberOfZeros = L - 1;
    for (int i = 0; i < N; i++)
    {
        int j = 0;
        for (j; j < numberOfZeros; j++)
        {
            A[i * L + j] = 0;
        }

        for (j; j < L; j++)
        {
            A[i * L + j] = fRand(-RANGE, RANGE);
        }

        numberOfZeros -= 1;
    }
}

void PrintArray1(double* A, int N, string nameOfVector)
{
    for (int i = 0; i < N; i++)
    {
        cout << nameOfVector << i + 1 << " = " << A[i] << endl;
    }
    cout << endl;

}

void PrintArray2(double* A, int N, int L)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < L; j++)
        {
            cout << A[i * L + j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void EnteringArrayFromFile1(double* A, fstream& file, int N, int L)
{
    string tmp = "";

    int index = 0;
    for (int i = 0; i < L - 1; i++)
    {
        for (int j = 0; j < L - i - 1; j++)
        {
            A[index] = 0;
            index++;
        }

        for (int j = 0; j <= i; j++)
        {
            file >> A[index];
            index++;
        }
        getline(file, tmp);
    }

    streamoff startOfString = file.tellg();
    streamoff offset = 0;
    for (int j = L - 1; j < N; j++)
    {
        file.seekg(offset - 1, file.cur);
        for (int k = 0; k < L; k++)
        {
            file >> A[index];
            index++;
        }
        offset = file.tellg() - startOfString;
        getline(file, tmp);
        startOfString = file.tellg();
    }

}

void EnteringArrayFromFile2(double* A, fstream& file, int N, int L)
{
    string tmp = "";

    int index = 0; //индекс в массиве хранения ленты

    for (int i = 0; i < L - 1; i++)
    {
        for (int j = 0; j < L - i - 1; j++)
        {
            A[index] = 0;
            index++;
        }

        for (int j = 0; j <= i; j++)
        {
            file >> A[index];
            index++;
        }
        getline(file, tmp);
    }

    for (int j = L - 1; j < N; j++)
    {
        for (int k = 0; k < j - L + 1; k++) //пропуск нулей
        {
            double tmp;
            file >> tmp;
        }

        for (int k = 0; k < L; k++)
        {
            file >> A[index];
            index++;
        }
        getline(file, tmp);

    }
}

void Decomposition(double* A, double* B, double* C, int N, int L)
{
    //Разложение по методу Халецкого

    for (int j = 0; j < N; j++)
    {
        for (int i = j; i < min(j + L, N); i++)
        {
            double sum = A[i * L + j - i + L - 1];
            for (int k = max(0, i - L + 1); k < j; k++)
            {
                sum -= B[i * L + k - i + L - 1] * C[k * L + j - k];
            }
            B[i * L + j - i + L - 1] = sum;
            C[j * L + i - j] = sum / B[j * L + L - 1];
        }
    }
}

void SolutionOfBottomMatrix(double* B, double* f, double* fCh, double* y, double* yCh, int N, int L)
{
    for (int i = 0; i < N; i++)
    {
        y[i] = f[i];
        yCh[i] = fCh[i];
        for (int j = max(0, i - L + 1); j < i; j++)
        {
            y[i] -= B[i * L + j - i + L - 1] * y[j];
            yCh[i] -= B[i * L + j - i + L - 1] * yCh[j];
        }
        y[i] /= B[i * L + L - 1];
        yCh[i] /= B[i * L + L - 1];
    }
}

void SolutionOfTopMatrix(double* C, double* y, double* yCh, double* x, double* xCh, int N, int L)
{
    for (int i = N - 1; i >= 0; i--)
    {
        x[i] = y[i];
        xCh[i] = yCh[i];
        for (int j = min(N - 1, i + L - 1); j > i; j--)
        {
            x[i] -= C[i * L + j - i] * x[j];
            xCh[i] -= C[i * L + j - i] * xCh[j];
        }
    }
}

double GetRelativeError(double* checkVector, int N) {
    for (int i = 0; i < N; i++) {
        checkVector[i] = abs(checkVector[i] - 1);
    }
    double max = checkVector[0];
    for (int i = 1; i < N; i++) {
        if (checkVector[i] > max)max = checkVector[i];
    }
    return max;
}

void GetBadMatrix(double* A, double* B, double* C, double* fCh, int N, int L)
{

    double multiplier = 1.0 / DIVIDER;
    for (int i = 0; i < N; i++)
    {
        B[i * L + L - 1] *= multiplier;
        /*C[i * L] *= multiplier;*/
    }

    PrintArray2(B, N, L);
    PrintArray2(C, N, L);

    double** An = new double* [N];
    for (int i = 0; i < N; i++)
    {
        An[i] = new double[N] {0};
    }

    double** Bn = new double* [N];
    for (int i = 0; i < N; i++)
    {
        Bn[i] = new double[N] {0};
    }

    double** Cn = new double* [N];
    for (int i = 0; i < N; i++)
    {
        Cn[i] = new double[N] {0};
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = max(0, i - L + 1); j <= i; j++)
        {
            Bn[i][j] = B[i * L + j - i + L - 1];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = min(N - 1, i + L - 1); j >= i; j--)
        {
            Cn[i][j] = C[i * L + j - i];
        }
    }

    /*for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int bIndex = i * L + j - i + L - 1;
            int cIndex = i * L + j - i;
            if (bIndex>=0 && bIndex<N*L)Bn[i][j] = B[bIndex];
            if (cIndex >= 0 && cIndex < N * L) Cn[i][j] = C[cIndex];
        }
    }*/

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << Bn[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << Cn[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double sum = 0;
            for (int k = 0; k < N; k++)
            {
                sum += Bn[i][k] * Cn[k][j];
            }
            An[i][j] = sum;
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << An[i][j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < N; i++)
    {
        double sum = 0;
        for (int j = 0; j < N; j++)
        {
            sum += An[i][j];
        }
        fCh[i] = sum;
    }

    for (int i = 0; i < N; i++)
    {
        delete[] An[i];
        delete[] Bn[i];
        delete[] Cn[i];
    }
    delete[] An;
    delete[] Bn;
    delete[] Cn;
}

double PrimaryTaskFromFile()
{
    int N, L;
    fstream file;
    string path = "input4.txt";
    file.open(path, fstream::in);

    string tmp = ""; //узнаём размерности
    getline(file, tmp);
    N = stoi(tmp);
    getline(file, tmp);
    L = stoi(tmp);

    double* A = new double[N * L]{ 0 };
    double* B = new double[N * L]{ 0 };
    double* C = new double[N * L]{ 0 };

    double* f = new double[N] {0};
    double* y = new double[N] {0};
    double* x = new double[N] {0};

    double* fCh = new double[N] {0}; // вектор свободных членов для поиска погрешности
    double* yCh = new double[N] {0};
    double* xCh = new double[N] {0};

    EnteringArrayFromFile2(A, file, N, L); //заполнение из файла массива хранения ленты

    for (int i = 0; i < N; i++) //считываем столбец свободных членов
    {
        file >> f[i];
    }
    file.close();


    for (int i = 0; i < N; i++) //заполнение вектора контрольной сумм
    {
        for (int j = 0; j < L; j++) // сумма элементов строки хранения 
        {
            fCh[i] += A[i * L + j];
        }

        for (int j = 1; j < min(L, N - i); j++) // + хвост по диагонали
        {
            fCh[i] += A[(i + j) * L + L - j - 1];
        }
    }

    PrintArray1(f, N, "f");
    cout << "Матрица А в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(A, N, L);

    PrintArray1(fCh, N, "fCh");

    Decomposition(A, B, C, N, L); // разложение по методу Халецкого

    cout << "Матрица B в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(B, N, L);

    cout << "Матрица С в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(C, N, L);

    //решение нижнетреугольной и верхнетреугольной матриц

    SolutionOfBottomMatrix(B, f, fCh, y, yCh, N, L);

    PrintArray1(y, N, "y");

    SolutionOfTopMatrix(C, y, yCh, x, xCh, N, L);

    PrintArray1(x, N, "x");

    double relativeError = GetRelativeError(xCh, N);
    cout << "Относительная погрешность = " << relativeError << endl;

    file.open("Result.txt", fstream::out);
    file << "Искомые неизвестные:" << endl;
    for (int i = 0; i < N; i++)
    {
        file << "X" << i + 1 << " = " << x[i] << endl;
    }
    file << endl;

    file << "Относительная погрешность = " << relativeError << endl;
    file.close();

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] f;
    delete[] y;
    delete[] x;

    delete[] fCh;
    delete[] yCh;
    delete[] xCh;
    return relativeError;
}

double PrimaryTaskRandomNumbers()
{
    int N = 10, L = 3;

    double* A = new double[N * L]{ 0 };
    double* B = new double[N * L]{ 0 };
    double* C = new double[N * L]{ 0 };

    double* f = new double[N] {0};
    double* y = new double[N] {0};
    double* x = new double[N] {0};

    double* fCh = new double[N] {0}; // вектор свободных членов для поиска погрешности
    double* yCh = new double[N] {0};
    double* xCh = new double[N] {0};

    EnteringGoodRandomNumbersDown(A, N, L);
    EnteringColumnOfFreeMembers(f, N);

    for (int i = 0; i < N; i++) //заполнение вектора контрольной сумм
    {
        for (int j = 0; j < L; j++) // сумма элементов строки хранения 
        {
            fCh[i] += A[i * L + j];
        }

        for (int j = 1; j < min(L, N - i); j++) // + хвост по диагонали
        {
            fCh[i] += A[(i + j) * L + L - j - 1];
        }
    }

    PrintArray1(f, N, "f");
    cout << "Матрица А в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(A, N, L);

    PrintArray1(fCh, N, "fCh");

    Decomposition(A, B, C, N, L); // разложение по методу Халецкого

    cout << "Матрица B в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(B, N, L);

    cout << "Матрица С в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(C, N, L);

    //решение нижнетреугольной и верхнетреугольной матриц

    SolutionOfBottomMatrix(B, f, fCh, y, yCh, N, L);

    PrintArray1(y, N, "y");

    SolutionOfTopMatrix(C, y, yCh, x, xCh, N, L);

    PrintArray1(x, N, "x");

    double relativeError = GetRelativeError(xCh, N);
    cout << "Относительная погрешность = " << relativeError << endl;

    fstream file;
    file.open("Result.txt", fstream::out);
    file << "Искомые неизвестные:" << endl;
    for (int i = 0; i < N; i++)
    {
        file << "X" << i + 1 << " = " << x[i] << endl;
    }
    file << endl;

    file << "Относительная погрешность = " << relativeError << endl;
    file.close();

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] f;
    delete[] y;
    delete[] x;

    delete[] fCh;
    delete[] yCh;
    delete[] xCh;
    return relativeError;
}

double TaskWithBadMatrix()
{
    int N = 20, L = 3;

    double* A = new double[N * L]{ 0 }; double* B = new double[N * L]{ 0 }; double* C = new double[N * L]{ 0 };

    double* f = new double[N] {0};  double* y = new double[N] {0};  double* x = new double[N] {0};

    double* fCh = new double[N] {1}; // вектор свободных членов для поиска погрешности
    double* yCh = new double[N] {0};    double* xCh = new double[N] {0};

    EnteringGoodRandomNumbersDown(B, N, L); EnteringGoodRandomNumbersUp(C, N, L);

    PrintArray2(B, N, L);   PrintArray2(C, N, L);
    GetBadMatrix(A, B, C, fCh, N, L);   
    EnteringColumnOfFreeMembers(f, N);

    PrintArray1(fCh, N, "fCh");

    cout << "Матрица B в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(B, N, L);

    cout << "Матрица С в массиве (массив одномерный, просто представил его так)" << endl;
    PrintArray2(C, N, L);

    //решение нижнетреугольной и верхнетреугольной матриц

    SolutionOfBottomMatrix(B, f, fCh, y, yCh, N, L);

    PrintArray1(y, N, "y");

    SolutionOfTopMatrix(C, y, yCh, x, xCh, N, L);

    PrintArray1(x, N, "x");

    double relativeError = GetRelativeError(xCh, N);
    cout << "Относительная погрешность = " << relativeError << endl;

    fstream file;
    file.open("Result.txt", fstream::out);
    file << "Искомые неизвестные:" << endl;
    for (int i = 0; i < N; i++)
    {
        file << "X" << i + 1 << " = " << x[i] << endl;
    }
    file << endl;

    file << "Относительная погрешность = " << relativeError << endl;
    file.close();

    delete[] A;
    delete[] B;
    delete[] C;
    delete[] f;
    delete[] y;
    delete[] x;

    delete[] fCh;
    delete[] yCh;
    delete[] xCh;
    return relativeError;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));

    //TaskWithBadMatrix();
    PrimaryTaskRandomNumbers();
    /*
    int N = 20, L = 3;
    double* A = new double[N * L]{ 0 }; 
    double* B = new double[N * L]{ 0 }; 
    double* C = new double[N * L]{ 0 };
    EnteringGoodRandomNumbersDown(B, N, L); 
    EnteringGoodRandomNumbersUp(C, N, L);
    PrintArray2(B, N, L);   
    PrintArray2(C, N, L); */

    return 0;
}

