#include <iostream>
#include <vector>
#include <algorithm>
#include <list>

using namespace std;

const int n = 4;
int best, limw;
int w[n], p[n];
bool cur[n], opt[n];

void copy(bool f[], bool s[]) {
    for (int i = 0; i < n; i++)
        f[i] = s[i];
}

void print(bool v[]) {
    cout << endl;
    for (int i = 0; i < n; i++) {
        if (v[i]) cout << i;
    }
}

void func(int now, int curw, int curp) {
    int _w, _p;
    _w = curw + w[now];
    if (_w <= limw) {
        cur[now] = true;
        _p = curp + p[now];
        if (now < n - 1) func(now + 1, _w, _p);
        else {
            print(cur);
            cout << " " << _p << " " << best;
            if (_p > best) {
                copy(opt, cur);
                best = _p;
            }
        }
        cur[now] = false;
    }
    if (now < n - 1) func(now + 1, curw, curp);
    else {
        print(cur);
        cout << " " << curp << " " << best;
        if (curp > best) {
            copy(opt, cur);
            best = curp;
        }
    }
}

int count_even(const vector<int> v) {
    auto fun = [](const int& elem) -> bool {
        if (elem % 2 == 0) return true;
        else return false;
    };
    return count_if(v.begin(), v.end(), fun);
}

int count_even2(const vector<int> v) {
    int res = 0;
    auto count = [&res](const int& elem) {
        if (elem % 2 == 0) res++;
    };
    for_each(v.begin(), v.end(), count);
    return res;
}

int main()
{
    /*
    limw = 10;
    for (int i = 0; i < n; i++) {
        cin >> w[i] >> p[i];
    }
    func(0, 0, 0);
    print(opt);
    */


    vector<int> vect;
    for (int i = 0; i < 100; i++) vect.push_back(i*i + 10);
    //cout << count_even2(vect);

    //cout << vect.size();

    auto it = max_element(vect.begin(), vect.end());
    cout << *it;
    //cout << *it;
    cout << endl;
    cout << vect.front();
    vect[0] = 20;
    cout << vect.front();
    vector<int> vec;
    vec.reserve(20);
    //for(int i = 0; i < 5; i++) cin >> vect[i];
    return 0;
    cout << endl << vec.capacity();
    list<int> lst;
}