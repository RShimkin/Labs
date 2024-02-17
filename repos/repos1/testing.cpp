#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

float max(float x, float y) {
    if (x > y) return x;
    else return y;
}

float min(float x, float y) {
    if (x > y) return y;
    else return x;
}

void Or(vector<float>& ar1, vector<float>& ar2, float y) {
    cout << "\tOR\n";
    for (int i = 0; i < ar1.size(); i++) {
        float res = y * max(ar1[i], ar2[i]) + (1 - y) * (ar1[i] + ar2[i]) / 2;
        cout << res << endl;
    }
}

void And(vector<float>& ar1, vector<float>& ar2, float y) {
    cout << "\tAND\n";
    for (int i = 0; i < ar1.size(); i++) {
        float res = y * min(ar1[i], ar2[i]) + (1 - y) * (ar1[i] + ar2[i]) / 2;
        cout << res << endl;
    }
}

void Comb(vector<float>& ar1, vector<float>& ar2, float y) {
    cout << "\tCOMB\n";
    for (int i = 0; i < ar1.size(); i++) {
        float res = y * min(ar1[i], ar2[i]) + (1 - y) * max(ar1[i], ar2[i]);
        cout << res << endl;
    }
}

vector<vector<float>> trans(vector<vector<float>> f, vector<vector<float>> s, int n) {
    vector<vector<float>> matr;
    for (int i = 0; i < n; i++) {
        vector<float> vect;
        for (int k = 0; k < n; k++) {
            float max = std::min(f[i][0], s[0][k]);
            for (int m = 1; m < n; m++) {
                float tmp = std::min(f[i][m], s[m][k]);
                if (tmp > max) max = tmp;
            }
            vect.push_back(max);
        }
        matr.push_back(vect);
    }
    return matr;
}

void print(vector<vector<float>> matr, int n) {
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) cout << setw(3) << matr[i][k] << " ";
        cout << endl;
    }
    cout << endl;
}

const int n = 5;

int main()
{
    /*
    vector<float> A{ 0, 0.3, 0.7, 1, 0, 0.2, 0.6 };
    vector<float> Be{ 0.3, 1, 0.5, 0.8, 1, 0.5, 0.6 };
    vector<float> Ce{ 1, 0.5, 0.5, 0.2, 0, 0.2, 0.9 };
    float y1 = 0.2, y2 = 0.5;
    Or(A, Be, y1);
    And(A, Ce, y1);
    Comb(Be, Ce, y1);
    Or(A, Be, y2);
    And(A, Ce, y2);
    Comb(Be, Ce, y2);
    */

    /*
    vector<vector<float>> matr;
    vector<float> vect1 = { 0,0,0.3,0.2,0 };
    vector<float> vect2 = { 0.6,1,0.8,1,0.2 };
    vector<float> vect3 = { 0.2,0,1,0.8,0.3 };
    vector<float> vect4 = { 0,0,0,1,0 };
    vector<float> vect5 = { 1,0,0.2,0.6,0 };
    matr.push_back(vect1);
    matr.push_back(vect2);
    matr.push_back(vect3);
    matr.push_back(vect4);
    matr.push_back(vect5);
    print(matr, n);

    vector<vector<float>> matr2 = trans(matr, matr, n);
    print(matr2,n);
    vector<vector<float>> matr3 = trans(matr2, matr, n);
    print(matr3, n);
    vector<vector<float>> matr4 = trans(matr3, matr, n);
    print(matr4, n);
    vector<vector<float>> matr5 = trans(matr4, matr, n);
    print(matr5, n);
    vector<vector<float>> matr6 = trans(matr5, matr, n);
    print(matr6, n);
    vector<vector<float>> matr7 = trans(matr6, matr, n);
    print(matr7, n);
    vector<vector<float>> matr8 = trans(matr7, matr, n);
    print(matr8, n);
    */

    int a, b, c = 0;
    c = 1;
    //cout << a << " " << b << " " << c << endl;
    
}