#include<iostream>
#include<fstream>
#include<cstring>
#include<cmath>

using namespace std;
 
int main(){
	ofstream output;
	output.open("output.txt"); 
	output << "Output!";
	output.close();
	ifstream input("input.txt");
	char* str;
	input.getline(str,10,' ');
	cout << str;
	input.close()
;	return 0;
}
 
    
