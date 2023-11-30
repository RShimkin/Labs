#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

#define PI 3.14159265

double turn(int n, int m){
	double var;
	int temp = m;
	int s = 1;
	while(temp){
		s *= 10;
		temp /= 10;
	}
	var = n + (double)m/s;
	return var;
}

double generate(){
	double n = (double)(rand()%100);
	cout << n << " ";
	double m = (double)(rand()%100);
	cout << m << " ";
	cout << m/100 << endl;
	double var = n + m/100;
	return var;
}

double sqr(double a){
	return a*a;
}

int main(){
	/*srand(time(0));
	int n, m;
	n = rand()%100;
	cout << n << " ";
	m = rand()%100;
	cout << m << "\n";
	double x = turn(n,m);
	double y = generate();
	cout << x << " " << y;
	*/
	//cout << sqr(1.5);
	float x;
	cin >> x;
	cout << "cos(x) rad" << cos(x);
	cout << "\ncos(x) gr" << cos(x*PI/180);
	float angle = cos(x*PI/180);
	cout << "\n" << acos(angle) << "	" << acos(angle)*180/PI;
	return 0;
}
