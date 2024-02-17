#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>
#include <cstdio>

#define M_PI 3.141592653589793238463
#define RANGE 50
#define ACCURACY 1/100000000

using namespace std;

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

long double safeAcosl(long double value) {
    if (value <= -1.0) {
        return M_PI;
    }
    else if (value >= 1.0) {
        return 0.0;
    }
    else {
        return acosl(value);
    }
}

void PrintMatrix(double** A, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << A[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void PrintVector(double* X, int N)
{
    for (int i = 0; i < N; i++)
    {
        cout << X[i] << " ";
    }
    cout << endl;
}

void CopyVector(double* A, double* B, int N)
{
    for (int i = 0; i < N; i++)
    {
        B[i] = A[i];
    }
}

double Scalar(double* A, double* B, int N)
{
    double result = 0;
    for (int i = 0; i < N; i++)
    {
        result += A[i] * B[i];
    }

    return result;
}

double** ProductOfMatrices(double** A, double** B, int N)
{
    double** result = new double* [N];
    for (int i = 0; i < N; i++)
    {
        result[i] = new double[N] {0};
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return result;
}

double** VectorsToMatrix(double* A, double* B, int N)
{
    double** result = new double* [N];
    for (int i = 0; i < N; i++)
    {
        result[i] = new double[N] {0};
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            result[i][j] = A[i] * B[j];
        }
    }

    return result;
}

void Multiply(double** A, double koef, int N) {
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = A[i][j] * koef;
        }
    }
}

void SubtractMatrix(double** A, double** B, int N) {
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = A[i][j] - B[i][j];
        }
    }
}

double GetNormaOfVector(double* w, int N, bool thirdNorma = false)
{
    double result = 0;
    if (thirdNorma)
    {
        for (int i = 0; i < N; i++)
        {
            result += w[i] * w[i];
        }
        result = sqrt(result);
    }
    else
    {
        double max = abs(w[0]);
        for (int i = 1; i < N; i++)
        {
            if (abs(w[i]) > max) max = abs(w[i]);
        }
        result = max;
    }
    return result;
}

void NormalizationOfVector(double* w, int N)
{
    double norma = GetNormaOfVector(w, N, true);
    if (norma != 0)
    {
        double koef = 1 / norma;
        for (int i = 0; i < N; i++)
        {
            w[i] *= koef;
        }
    }
}

double** GetHouseholderMatrix(int N)
{
    double** H = new double* [N];
    for (int i = 0; i < N; i++)
    {
        H[i] = new double[N] {0.0};
    }

    double* w = new double[N];
    for (int i = 0; i < N; i++)
    {
        /* w[i] = rand()% 30;*/
        w[i] = fRand(-100.0, 100.0); // можно какой-нибудь другой диапазон 
    }

    NormalizationOfVector(w, N);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j <= i; j++) // матрица симметричная
        {
            if (i == j) H[i][j] = 1.0;
            H[i][j] = H[j][i] += -2.0 * w[i] * w[j];
        }
    }

    delete[] w;
    return H;
}

double** GetDiagMatrix(int N)
{
    double** A = new double* [N];
    for (int i = 0; i < N; i++)
    {
        A[i] = new double[N] {0.0};
    }

    for (int i = 0; i < N; i++)
    {
        /*A[i][i] = rand()%20;*/
        A[i][i] = fRand(-RANGE, RANGE);
    }

    return A;
}

double GetMeasure(double** A, double* V, double value, int N);

double** GetRightMatrix(int N)
{
    double** A = new double* [N];
    for (int i = 0; i < N; i++)
    {
        A[i] = new double[N];
    }

    double** diag = GetDiagMatrix(N);
    cout << "Диагональная матрица" << endl;
    PrintMatrix(diag, N);

    double** H = GetHouseholderMatrix(N);
    cout << "Матрица Хаусхолдера" << endl;
    PrintMatrix(H, N);

    A = ProductOfMatrices(H, diag, N);
    A = ProductOfMatrices(A, H, N);

    double value = diag[0][0];
    double* test = new double[N];

    for (int i = 0; i < N; i++)
    {
        test[i] = H[i][0];
    }

    //PrintVector(test, N);
    //cout << "Test: " << GetMeasure(A, test, value, N) << endl;
    //NormalizationOfVector(test, N);
    //PrintVector(test, N);

    //cout << "Test: " << GetMeasure(A, test, value, N) << endl;

    delete[] test;

    for (int i = 0; i < N; i++)
    {
        delete[] diag[i];
    }
    delete[] diag;

    for (int i = 0; i < N; i++)
    {
        delete[] H[i];
    }
    delete[] H;

    return A;
}

void Decomposition(double** A, double** B, double** C, int N) // прямой ход метода Халецкого
{
    for (int j = 0; j < N; j++)
    {
        for (int i = j; i < N; i++)
        {
            double sum = A[i][j];
            for (int k = 0; k < j; k++)
            {
                sum -= B[i][k] * C[k][j];
            }
            B[i][j] = sum;
            C[j][i] = sum / B[j][j];
        }
    }
}

double* GetSolutionOfBottomMatrix(double** B, double* V, int N)
{
    double* Y = new double[N];
    for (int i = 0; i < N; i++)
    {
        Y[i] = V[i];
        for (int j = 0; j < i; j++)
        {
            Y[i] -= B[i][j] * Y[j];
        }
        Y[i] /= B[i][i];
    }

    return Y;
}

double* GetSolutionOfTopMatrix(double** C, double* Y, int N)
{
    double* X = new double[N];
    for (int i = N - 1; i >= 0; i--)
    {
        X[i] = Y[i];
        for (int j = N - 1; j > i; j--)
        {
            X[i] -= C[i][j] * X[j];
        }
    }
    return X;
}

long double GetAngleBetweenTwoVectors(double* A, double* B, int N)
{
    double scalar = 0, lengthA = 0, lengthB = 0;

    for (int i = 0; i < N; i++)
    {
        scalar += A[i] * B[i];
        lengthA += A[i] * A[i];
        lengthB += B[i] * B[i];
    }
    long double result = scalar / (sqrt(lengthA) * sqrt(lengthB));
    result = safeAcosl(abs(result)); // в этом смысле угол ?
    return result;
}

void GetNewX(double** B, double** C, double* V, double* oldX, int N)
{
    double* Y = GetSolutionOfBottomMatrix(B, V, N);
    double* newX = GetSolutionOfTopMatrix(C, Y, N);

    delete[] Y;

    CopyVector(newX, oldX, N);
    delete[] newX;
}

void GetNewX(double** A, double* V, double* oldX, int N)
{
    
    double* newX = new double[N];

    for (int i = 0; i < N; i++) {
        double sum = 0;
        for (int j = 0; j < N; j++) {
            sum += A[i][j] * V[j];
        }
        newX[i] = sum;
    }

    CopyVector(newX, oldX, N);
    delete[] newX;
}

double GetMeasure(double** A, double* V, double value, int N)
{
    double* result = new double[N] {0};

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            result[i] += A[i][j] * V[j];
        }

        result[i] -= value * V[i];
    }

    double measure = GetNormaOfVector(result, N);

    delete[] result;

    return measure;
}

void Task(double& accuracyOfValue, long double& accuracyOfVector, double& measure, int& numberOfIterations)
{
    int N = 10;

    double** A = GetRightMatrix(N);
    cout << "Начальная матрица (HDH)" << endl;
    PrintMatrix(A, N);

    
    double** B = new double* [N];
    for (int i = 0; i < N; i++)
    {
        B[i] = new double[N] {0};
    }

    double* X = new double[N] {1};
    double* V = new double[N] {1};
    double* prevX = new double[N] {0};
    double* prevV = new double[N] {1};

    NormalizationOfVector(V, N);

    double alpha = 0.0, prevAlpha = 0.0;
    accuracyOfVector = 1.0;

    while (accuracyOfVector > ACCURACY && numberOfIterations < 100000)
    {
        CopyVector(X, prevX, N);
        GetNewX(A, V, X, N);
        prevAlpha = alpha;
        alpha = Scalar(V, X, N);

        accuracyOfValue = abs(prevAlpha - alpha);

        CopyVector(X, V, N);
        NormalizationOfVector(V, N);

        accuracyOfVector = GetAngleBetweenTwoVectors(X, prevX, N);
        numberOfIterations += 1;
    }

    cout << "Максимальное собственное значение: " << alpha << endl;

    NormalizationOfVector(X, N);
    cout << "Собственный вектор, соответствующий ему:\n\t";
    PrintVector(X, N);

    //cout << "Собственный вектор (предыдущий):" << endl;
    //PrintVector(prevX, N);
    //cout << endl;

    auto tmp = VectorsToMatrix(X, X, N);
    Multiply(tmp, alpha, N);
    SubtractMatrix(A, tmp, N);

    //------------------------------------------------------------------------
    
    X = new double[N] {1};
    V = new double[N] {1};
    prevX = new double[N] {0};
    prevV = new double[N] {1};

    NormalizationOfVector(V, N);

    alpha = 0.0;
    prevAlpha = 0.0;
    accuracyOfVector = 1.0;
    numberOfIterations = 0;

    while (accuracyOfVector > ACCURACY && numberOfIterations < 100000)
    {
        CopyVector(X, prevX, N);
        GetNewX(A, V, X, N);
        prevAlpha = alpha;
        alpha = Scalar(V, X, N);

        accuracyOfValue = abs(prevAlpha - alpha);

        CopyVector(X, V, N);
        NormalizationOfVector(V, N);

        accuracyOfVector = GetAngleBetweenTwoVectors(X, prevX, N);
        numberOfIterations += 1;
    }
    
    cout << "2 Максимальное собственное значение: " << alpha << endl;

    NormalizationOfVector(X, N);
    cout << "2 Собственный вектор, соответствующий ему:\n\t";
    PrintVector(X, N);

    //cout << " 2 Собственный вектор (предыдущий):" << endl;
    //PrintVector(prevX, N);

    
    // ---------------------------------------------------------------
    

    measure = GetMeasure(A, X, alpha, N);
    

    for (int i = 0; i < N; i++)
    {
        delete[] A[i];
    }
    delete[] A;

    delete[] X;
    delete[] V;
    delete[] prevX;
    delete[] prevV;
    delete[] tmp;
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(0));
    double accuracyOfValue, measure;
    long double accuracyOfVector;
    int numberOfIterations = 0;

    Task(accuracyOfValue, accuracyOfVector, measure, numberOfIterations);

    cout << "Точность СЗ: " << accuracyOfValue << endl;
    cout << "Точность СВ: " << accuracyOfVector << endl;
    cout << "Мера точности: " << measure << endl;
    cout << "Итераций: " << numberOfIterations << endl;

    return 0;
}