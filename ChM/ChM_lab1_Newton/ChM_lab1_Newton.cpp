#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template <typename T> 
void print(vector<T> arr, string name) {
    cout << name << ":\n";
    for (auto x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

template <typename T> 
void printMatr(vector<T> matr, int N) {
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            cout << matr[i + k * N] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

vector<double> buildCoefs(vector<double> x, vector<double> y, int N) {
    vector<double> f;
    int pos;
    double tmp;
    for (int i = 0; i < N; i++) {
        f.push_back(y[i]);
    }
    for (int k = 1; k < N; k++) {
        for (int i = 0; i < N - k; i++) {
            pos = f.size() - N;
            tmp = f[pos + 1] - f[pos];
            tmp = tmp / (x[i + k] - x[i]);
            f.push_back(tmp);
        }
        for (int i = N - k; i < N; i++) {
            f.push_back(0);
        }
    }
    return f;
}

int closest(vector<double> arr, double x) {
    int res = 0;
    double m = x - arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if ((x-arr[i]) > 0 && (x - arr[i]) < m) {
            res = i;
            m = x - arr[i];
        }
    }
    return res;
}

int task(double &YY) {
    int N, pos;
    double EPS, XX, res = 0, prev, delta = INT_MIN, prevdelta = INT_MAX;
    vector<double> x, y, f;
    string fname;
    cin >> fname;
    ifstream file(fname);
    file >> N;
    double tmp;
    for (int i = 0; i < N; i++) {
        file >> tmp;
        x.push_back(tmp);
    }
    for (int i = 0; i < N; i++) {
        file >> tmp;
        y.push_back(tmp);
    }
    file >> XX >> EPS;
    file.close();

    if (XX < x[0] || XX > x[x.size() - 1])
        return 4;

    f = buildCoefs(x, y, N);

    print(x, "x");
    print(y, "y");
    cout << endl;
    printMatr(f, N);

    cout << endl << "XX: " << XX << endl;

    prev = INT_MAX;
    for (int i = 0; i < N; i++) {
        if (i != 0) 
            prev = res;
        res = 0;
        for (int k = 0; k < i; k++) {
            double tmp = 1;
            for (int m = 0; m < k + 1; m++) {
                double val = (XX - x[m]);
                tmp *= (XX - x[m]);
            }
            res += tmp * f[(k + 1)*N];
        }
        res += f[0];
        delta = abs(prev - res);
        cout << "res:" << res << endl;
        cout << "delta:" << delta << endl;
        if (i > N / 2 && abs(prev - res) < EPS) {
            YY = res;
            return 0;
        }
        prevdelta = delta;
    }

    YY = res;
    if (abs(prev - res) > EPS) {
        return 1;
    }
    else {
        return 0;
    }
}

int main()
{
    double YY = 0;
    int EOR = task(YY);
    switch (EOR) {
    case 0:
        cout << "No errors, accuracy achieved, result " << YY;
        break;
    case 1:
        cout << "Accuracy not achieved, N's too small, result: " << YY;
        break;
    case 2:
        cout << "Accuracy not achieved, approximation stopped, result: " << YY;
        break;
    case 3:
        cout << "Error! Wrong order in x";
        break;
    case 4: 
        cout << "Error! XX out of x";
        break;
    }
}