#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

int main() {
    srand(time(0));
    int n, k;
    cout << "n and k: ";
    cin >> n >> k;
    vector<int> a(n);
    cout << "generated array: ";
    for (int i = 0; i < n; ++i) {
        a[i] = rand() % 1000 + 1;
        cout << a[i] << ' ';
    }
    cout << '\n' << k << " biggest elements : ";
    sort(a.begin(), a.end());
    for (int i = a.size() - 1; i >= a.size() - k; --i) {
        cout << a[i] << ' ';
    }
    cout << '\n';
    return 0;
}
