// Шимкин Р.Р. 62 группа ПММ ВГУ Dev-cpp 5.11 
// задание: вывести самого высокооплачиваемого сотрудника по каждой профессии
#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;
const float mul = 0.05; // множитель для премии
const int val = 3000; // множитель для зп
const string names[] = { "Ivanov", "Petrov", "Sidorova", "Bistrov" }; // массив имён для генерации; была идея его вводить, но, мне кажется, это лишнее
const int nnames = 4; // размер массива имён

enum jobs{
	DEV = 80,
	TEST = 50,
	AN = 40, 
	MAN = 120
};

struct employee{
	string surname;
	jobs job;
	int exp;
	float wage; // оплату, т.е. зп и премии, легче хранить в структуре, чем в отдельном массиве
};

void setJob(employee* emp, string prof){
	if (prof=="dev") emp->job = DEV;
		else if (prof=="test") emp->job = TEST;
			else if (prof=="an") emp->job = AN;
				else emp->job = MAN;
	emp->wage = emp->job*val/100;
}

void setJob(employee* emp, int j){
	switch (j){
		case 0: 
			emp->job = DEV;
			break;
		case 1:
			emp->job = TEST;
			break;
		case 2:
			emp->job = AN;
			break;
		case 3:
			emp->job = MAN;
			break;
	};
	emp->wage = emp->job*val/100;
}

void printJob(employee* emp){
	switch (emp->job){
		case 80:
			cout << " dev ";
			break;
		case 50:
			cout << " test ";
			break;
		case 40:
			cout << " an ";
			break;
		case 120:
			cout << " man ";
			break;
	}
}

void handInput(employee* emp, int n, float* av){
	for (int i=0; i<n; i++ ){
		cout << i+1 << " employee surname, profession (dev, test, an, man) and experience (years) in a line:" << endl;
		string str;
		if (i==0) cin.ignore();
		getline(cin,str);
		string buf[3]= {"","",""};
		int field = 0;
		for(int j=0; j<str.length(); j++){
			if (str[j]!=' ') buf[field]+=str[j];
				else {
					if (field == 0) emp[i].surname = buf[field];
						else if (field == 1) setJob(&emp[i], buf[field]);
					field++;
				};
		}
		int mul = 1, num = 0;
		for (int k = buf[field].length()-1; k>=0; k--){
			num+=mul*(buf[field][k]-'0');
			mul*=10;
		};
		emp[i].exp = num;
		cout << endl;
		*av += emp[i].wage;	
	}
}

void randomInput(employee* emp, int n, float* av){
	for (int i=0; i<n; i++){
		cout << i+1 << " employee surname, profession (dev, test, an, man) and experience (years):\n\t";
		string buf;
		int j=rand()%4;
		buf = names[j];
		emp[i].surname = buf;
		cout << buf << " ";
		j=rand()%4;
		setJob(&emp[i],j);
		printJob(&emp[i]);
		emp[i].exp = rand()%30;
		cout << emp[i].exp;
		cout << endl;
		*av += emp[i].wage;
	}
}

void bonus(employee* emp, int n, int av){
	for (int i=0; i<n; i++){
	    cout << "bonus " << i+1 << " = " << emp[i].exp << "*" << av << "*" << mul << " = " << emp[i].exp*av*mul << endl; // вывод расчёта премии; полезен для контроля за результатом
		emp[i].wage += emp[i].exp*av*mul;
	}
}

void theBest(employee* emp, int n){
	int dev=0, test=0, an=0, man=0;
	int devi=-1, testi=-1, ani=-1, mani=-1;
	for (int i=0; i<n; i++){
		if ((emp[i].job == DEV)&&(emp[i].wage > dev)) {
			devi = i;
			dev = emp[i].wage;
		}
		else if ((emp[i].job == TEST)&&(emp[i].wage > test)){
			    testi = i;
			    test = emp[i].wage;
			 }
			 else if ((emp[i].job == AN)&&(emp[i].wage > an)) {
					ani = i;
					an = emp[i].wage;
				  }
				  else if ((emp[i].job == MAN)&&(emp[i].wage > man)) {
				  			mani = i;
				  			man = emp[i].wage;
				       }
		}
	cout << "The most payable are:\n";
	if (devi!=-1) cout << emp[devi].surname << "(developer) -- " << dev << "	 " << emp[devi].job << endl;
		else cout << "No developers\n";
	if (testi!=-1) cout << emp[testi].surname << "(tester) -- " << test << "  " << emp[testi].job << endl;
		else cout << "No testers\n";
	if (ani!=-1) cout << emp[ani].surname << "(analitic) -- " << an << "  " << emp[ani].job << endl;
		else cout << "No analitics\n";
	if (mani!=-1) cout << emp[mani].surname << "(manager) -- " << man << "	" << emp[mani].job << endl;	
		else cout << "No managers\n";	
}

int main(){
	srand(time(0));
	int n;
	cout << "Enter the number of the workers: ";
	cin >> n;
	employee* emp = new employee [n];
	cout << "Hand Input (0) or random generation (1) ? ";
	bool input;
	cin >> input;
	float av=0;
	if (!input) handInput(emp,n,&av);
		else randomInput(emp,n,&av);
	av /= n;
	bonus (emp, n, av);
	theBest(emp,n);
	delete[] emp;
	return 0;
}
