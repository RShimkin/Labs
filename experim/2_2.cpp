#include <iostream>
#include <string>
using namespace std;
 
int main()
{
	int count(0);
    string s1="Main string:", s2="\nThe substring:";
    cout<<s1;
    getline(cin, s1);
    cout<<s2;
    getline(cin, s2);
 
    for(int i = 0; i < s1.length(); ++i)
        if(s1[i] == s2[0])
            if(s1.substr(i, s2.length()) == s2)
            {
                ++count;
                i += s2.length() - 1;
            }
 
    cout << count;
    return 0;
}
