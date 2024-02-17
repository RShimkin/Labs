#include<iostream>
#include "List.h"
#include <fstream>

using namespace std;

int main(){
	Tlist spisok;
	Init(spisok);
	ifstream file("input.txt");
	int value;
	while (!file.eof()){
		file >> value;
		AddToTail(spisok, value);
	}
	file.close();
	View(spisok);
	cout << endl << LongestIncSequence(spisok);
	system("pause");
	return 0;
}