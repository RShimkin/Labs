#include <iostream>
#include <string>
#include <cmath>

using namespace std;

const string alfabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int any_to_dec(string num, int base_from) {
    int result = 0;
    for (int i = num.length() - 1, p = 0; i >= 0; --i, ++p) {
        result += alfabet.find(num[i]) * pow(base_from, p);
    }
    return result;
}

string dec_to_any(int num, int base_to = 10) {
    if (num < base_to) {
        return string(1, alfabet[num]);
    } else {
        return dec_to_any(num / base_to, base_to) + alfabet[num % base_to];
    }
}

string any_to_any(string num, int base_from, int base_to) {
    int n = any_to_dec(num, base_from);
    return dec_to_any(n, base_to);
}

int main() {
    string num;
    int base_f, base_t;
    cout << "Введите число, его основание и новое основание: ";
    cin >> num >> base_f >> base_t;
    cout << num << '(' << base_f << ") = ";
    cout << any_to_any(num, base_f,base_t) << '(' << base_t << ')' << '\n';
    return 0;
}
