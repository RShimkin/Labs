#include <iostream>
#include <queue>
#include <stack>
#include <string>

using namespace std;

int main() {

    string sequence;
    cin >> sequence;


    stack<char> st;
    int dec = atoi(sequence.c_str());
    while (dec) {
        st.push('0' + dec % 2);
        dec /= 2;
    }
    string binresult;
    while (!st.empty()) {
        cout << st.top();
        binresult += st.top();
        st.pop();
    }
    cout << endl << binresult;

    cout << endl;
    dec = atoi(sequence.c_str());
    cout << dec << endl;
    char proverka[10];
    _itoa_s<10>(dec, proverka, 2);
    cout << proverka << endl;

 
}