#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main(){
	ifstream file;
	file.open("input.txt");
	if (file.is_open()) cout << "opened" << endl;
		else cout << "not opened" << endl;
	/*string str;
	getline(file,str,20);
	cout << str;
	*/
	file.close();
	return 0;
}
