#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std; 

double* massive(int k){
	if (k==2) {
		cout << " k = " << k << " ";
		double* newPtr = new double[k];
		cout << "	N =" << k << "	";
		for(int i=0;i<k;i++){
			newPtr[i] = (rand()%10*0.1)*100*k;
			cout << newPtr[i] << " ";
		};
		cout << "newPtr ( " << k << ") = " << newPtr; 
		cout << "\n";
		return newPtr;
	}; 
	cout << " k = " << k << " ";
	double* newPtr = new double[k];
	double* oldPtr = massive(k-1);
	cout << "	N =" << k << "	";
	for(int i=0;i<=k-2;i++){
		newPtr[i]=100*k*oldPtr[i]*(rand()%10);
		cout << newPtr[i] << " ";
	};
	newPtr[k-1]=k;
	cout << (float)newPtr[k-1] << cout << "	newPtr ( " << k << ") = " << newPtr <<"\n";
	return newPtr;
}

int main(){
	srand(time(0));
	double* arr;
	int n;
	cin >> n;
	arr = massive(n);
	cout << "Arr = " << arr << "	";
	for( int i=0;i<n;i++){
		cout << arr[i] << " ";
	};
	return 0;
}
