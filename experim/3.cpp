#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    int s = 0;
    cout<<s;
    string str;
    ifstream input("D://input.txt");
    if (input) cout << "connected";
    while (getline(input, str)) {
        cout<<str;
        char ch;
        int a=1;
        for ( int i=0; i<=str.length(); i++) {  
        s+=a*(str[i]-'0');
		a*=10;}
		cout << s;
    }
    input.close();
    return 0;
}
