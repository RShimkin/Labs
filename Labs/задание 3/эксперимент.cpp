#include <iostream>
#include <string>

using namespace std;

enum jobs {
	DEV = 80,
	TEST = 50, 
	AN = 40,
	MAN = 120
};

struct employee {
	string name;
	jobs job;
};

void setJob(string str, employee* a, int k){
	if (str=="dev") a[k].job=DEV;
		else if (str=="test") a[k].job=TEST;
			else if (str=="an") a[k].job=AN;
				else a[k].job=MAN;
}

int main(){
	int n;
	cin >> n;
	employee* emp = new employee [n];
	for (int i=0; i<n; i++){
		cin >> emp[i].name;
		string buf;
		cin >> buf;
		setJob(buf, emp, i);
	};
	cout << emp[n-1].job;
	jobs best = DEV;
	cout << best;
	int wage = 3000*best/100;
	cout << " " << wage;
	return 0;
}
