// Шимкин Р.Р. 62 группа ПММ ВГУ Dev-cpp 5.11
// задание: вывести логический вектор, показывающий, есть ли в соответствующей строке матрице элементы < 1 и > 0.5
#include <iostream>
#include <cstdlib>
#include <windows.h>

using namespace std;

float **a;
int n, *vector;

bool f(int i, float** a);

int main()
{
	cout << "Enter n: "; 
	cin >> n;
	a = new float* [n];
	for (int i = 0; i < n; i++)
		* (a + i) = new float[n];
	vector = new int[n];
	bool input;
	cout << "Enter 1 for random generation or 0 for hand input: ";
	cin >> input;
	cout << endl;
	if (input) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				*(*(a + i) + j) = 0.1 * (rand()%10) - 0.2 ;
				cout << *(*(a + i) + j) << " ";
			};
		cout << "\n";
		} 
	}
		else {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++)
					cin >> *(*(a+i)+j);
			} 
			system("cls");
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++)
					cout << *(*(a+i)+j) << " ";
			cout << "\n";
			} 
		}
	cout << "\nVector: ";
	for (int i = 0; i < n; i++) {
		*(vector + i) = f(i, a);
		cout << vector[i];
	}
	for (int i = 0; i < n; i++) {
		delete[] a[i];
	 }
	delete[] a; 
	delete[] vector;
	return 0;
}

bool f(int i, float **a) {
	bool res = false;
	for (int j = 0; (j<n)&&(!res); j++) {
		if ((*(*(a + i) + j) <= 1)&&(*(*(a + i) + j) >= 0.5)) res = true;
	};
	return res;
};

