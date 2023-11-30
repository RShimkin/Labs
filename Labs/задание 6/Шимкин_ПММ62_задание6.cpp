#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(){
	ifstream file("input.txt");
	if (file.is_open()) cout << "file is opened" << endl;
		else cout << "file is not opened" << endl;
	while(file){
		string str;
		getline(file,str);
		if (str.length()==0) continue; // я не знаю, можно ли по-другому избежать зацикливания (не очень умею работать с текстовыми документами)
		int a=0,b=0,c=0,i,mul=1;
		for(i=str.length()-1; str[i]!='='; i--){
			c+=mul*(str[i]-'0');
			mul*=10;
		}
		mul=1;
		for (i--; !((str[i]=='+')||(str[i]=='-')||(str[i]=='/')||(str[i]=='*')); i-- ){
			b+=mul*(str[i]-'0');
			mul*=10;
		}
		mul=1;
		char sign = str[i];
		for (i--; i>=0; i--){
			a+=mul*(str[i]-'0');
			mul*=10;
		}
		int res;
		switch(sign){
			case '+':
				res = a+b;
				break;
			case '-':
				res = a-b;
				break;
			case '*':
				res = a*b;
				break;
			case '/':
				res = a/b;
				break;
		}
		if (res==c)
			cout << str << endl;
	}
	file.close();
	cin.get();
	return 0;
}
