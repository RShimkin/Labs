#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(){
	FILE *file = fopen("input.txt","r");
	if (file!=NULL) cout << "opened" << endl;
		else cout << "not opened" << endl;
	while(!feof(file)){
		char str[20];
		fgets(str,20,file);
		if (strlen(str)==0) continue;
		int a=0,b=0,c=0,i,mul=1;
		str[strlen(str)-1]='\0';
		cout << "string: |" << str << "| " << strlen(str) << endl;
		for(i=strlen(str)-1; str[i]!='='; i--){
			cout << str[i] << " ";
			c+=mul*(str[i]-'0');
			cout << c << " ";
			mul*=10;
		}
		mul=1;
		cout << "b: " << b << " ";
		for (i--; !((str[i]=='+')||(str[i]=='-')||(str[i]=='/')||(str[i]=='*')); i-- ){
			cout << str[i] << " ";
			b+=mul*(str[i]-'0');
			mul*=10;
		}
		cout << "b: " << b << endl;
		mul=1;
		char sign = str[i];
		for (i--; i>=0; i--){
			a+=mul*(str[i]-'0');
			mul*=10;
		}
		cout << "a: " << a << endl;
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
	fclose(file);
	return 0;
}
