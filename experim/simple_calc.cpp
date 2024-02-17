
#include <iostream>
#include <locale>

using namespace std;

int main (){
	setlocale(LC_ALL, "Russian");
	char s;
	do {
		float x,y,r;  char o;
		cin>>x;  cin>>o;  cin>>y;
		switch (o){
			case '+': r=x+y; break;
			case '-': r=x-y; break;
			case '*': r=x*y; break;
			case '/': r=x/y; break;
		};
		cout<<'\n';
		cout<<x<<o<<y<<'=';
		cout<< r;
		cin>>s;
	} while (s=='y');
	return 0;
}
