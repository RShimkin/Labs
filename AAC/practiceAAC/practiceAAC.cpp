#include <iostream>

using namespace std;

int main()
{
    int n;
    cin >> n;
    int** matr = new int* [n - 1];
    for (int i = 0; i < n - 2; i++) {
        matr[i] = new int[4];
        for (int m = 0; m < 4; m++) matr[i][m] = 0;
    }
    int res = 0;
    if (n == 0) res = 0;
    else if (n == 1) res = 1;
    else if (n == 2) res = 2;
    else {
        matr[0][0] = matr[0][1] = matr[0][2] = matr[0][3] = 1;
        for (int i = 0; i < n - 2; i++) {
            matr[i+1][2] += 
        }
    }
}