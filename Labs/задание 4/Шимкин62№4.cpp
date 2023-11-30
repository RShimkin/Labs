// Шимкин Р.Р. 62 группа ПММ ВГУ Dev-cpp 5.11
// задание: вывести количество треугольников, содержащих угол меньше введенного 
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#define SIZE "Enter the number of triangles: " // макросы для большей читаемости кода
#define INPUT "\nHand input (0) or random values (1)? "
#define PI 3.14159265
#define ANGLE "Enter the angle value: "
#define CONTAINS " triangle(s) contain(s) angles < "

using namespace std;

struct Point{
	double x;
	double y;
};

struct Triangle{
	Point vertex[3];
};

double generate(){
	double n = (double)(rand()%100);
	double m = (double)(rand()%100);
	double var = n + m/100;
	return var;
}

void setTriangle(Triangle* tr, int n, bool in){
	for(int i=0; i<n; i++){
		cout << "\tTriangle" << i+1 << ":\n";
		for(int k=0; k<3; k++){
			tr[i].vertex[k].x = generate();
			cout << "X: " << tr[i].vertex[k].x;
			tr[i].vertex[k].y = generate();
			cout << "	Y: " << tr[i].vertex[k].y << endl;
		}
	}
}

void setTriangle(Triangle* tr, int n){
	for(int i=0; i<n; i++){
		cout << "\tTriangle" << i+1 << ":\n";
		for(int k=0; k<3; k++){
			cout << "X: ";
			cin >> tr[i].vertex[k].x;
			cout << "	Y: ";
			cin >> tr[i].vertex[k].y;
			cout << endl;
		}
	}
}

double sqr(double a){
	return a*a;
}

double len(Point a, Point b){
	double val = sqrt(sqr(a.x-b.x)+sqr(a.y-b.y));
	return val;
}

int number(Triangle* tr, int n, double x){
	int num = 0;
	for (int i=0; i<n; i++){
		double a = len(tr[i].vertex[0],tr[i].vertex[1]);
		double b = len(tr[i].vertex[1],tr[i].vertex[2]);
		double c = len(tr[i].vertex[0],tr[i].vertex[2]);
		double cosA = (sqr(b) + sqr(c) - sqr(a))/(2*b*c);
		double cosB = (sqr(a) + sqr(c) - sqr(b))/(2*a*c);
		double cosC = (sqr(b) + sqr(a) - sqr(c))/(2*b*a);
		cout << "Angles " << i << " : " << acos(cosA)*180/PI << "		" << acos(cosB)*180/PI << "		" << acos(cosC)*180/PI << endl;
		if ((x>acos(cosA)*180/PI)||(x>acos(cosB)*180/PI)||(x>acos(cosC)*180/PI)) num++; 
	}
	return num;
}

int main(){
	srand(time(0));
	cout << SIZE;
	int n;
	cin >> n;
	Triangle* triangle = new Triangle[n];
	cout << INPUT;
	bool input;
	cin >> input;
	if (input) setTriangle(triangle,n,input);
		else setTriangle(triangle,n);
	cout << ANGLE;
	double x;
	cin >> x;
	cout << number(triangle,n,x) << CONTAINS << x;
	delete[] triangle;
	return 0;
}




