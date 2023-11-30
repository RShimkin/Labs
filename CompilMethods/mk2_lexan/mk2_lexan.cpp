// Задание 24: сочетание цифр (1,2,3) 
// и букв (a,b,c); первая цифра, дальше только буквы
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string S;
int i, ST, CL, FIX, tmp, len;

int D1[3][4] = {
 //    d   l   _  er  
	{  1, -2,  0, -1 },  //S0
	{ -3,  2, -4, -1 },  //S1
	{ -3,  2, -5, -1 }   //S2
};

int     W1[] = { 1,1,1,1,1 };

string out1[] = {
		"Unknown symbol",					// res-1
		"Starts with letter",				// res-2
		"Unexpected number position",		// res-3
		"String contains no letters",		// res-4
		"Correct string"					// res-5
};

int sclass1(char c) {
	switch (c) {
		case '1': case '2': case '3':
			return 0;
		case 'a': case 'b': case 'c':   
			return 1;
		case ' ': case '\0':
			return 2;
		default:
		{
			return 3;
		}
	}
}

void lexfile1() {
	string name;
	cout << "file name: ";
	cin >> name;
	ifstream file(name);

	i = ST = len = 0;

	while (true) {
		if ((ST == 0)) {
			string tmp;
			if (!file.eof() && file.is_open()) {
				file >> tmp;
				S = tmp + "\0"s;
			}
			else {
				file.close();
				return;
			}
			len = S.size();
			i = 0;
		}

		if (ST == 0) {
			cout << S << " " << len << " " << i << " " << FIX;
			FIX = i;
		}

		CL = sclass1(S[i]);
		tmp = ST;
		ST = D1[ST][CL];
		i++;
		//cout << S[i++] << "[" << tmp << "->" << ST << "]\n";

		for (int j = FIX; j < i; j++)
			cout << S[j];
		cout << endl;

		if (ST < 0) {
			i = i - W1[-ST - 1];
			cout << "(" << i << ") <- " << out1[-ST - 1] << endl;
			ST = 0;
		}
	}

}

int main() {
	lexfile1();
}