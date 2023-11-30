// ������ �.�. 62 ������ ��� ��� Dev-cpp 5.11
// �������: ������� ����� n-�� ���������� �������� (n={2..20}) � �������� ��������� � �� ������� [-1;1]; F(n) = 2xf(n-1) - f(n-2)
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;
double e; // �������� 
double *rootArr; // ��������� ��� ������� ������
int **matr; // ��������� ��� ������� �������������
int n; // ������� �������� �������� � ����������� ������� ������

const double mul = 0.001, a = -1.0, b = 1.0; // ������� ������� � ��������� ��� ���������
const int nmax = 20, nmin = 2; // ������� �������� ��������

template <typename T> 
void setNull(T* a){ 
	for (int i=0; i<n; i++){
		a[i]=0;
	}
}

void mult(int* res, int* src){ // �������, ����������� ������������� �������� f(n) = 2x*f(n-1) � �������������
    for (int i=0;i<n;i++){
        res[i+1]=src[i]*2;
    };
}

void min(int* res, int* src){ // ������������� �������� f(n) = ... - f(n-2)
    for(int i=0;i<n;i++){
        res[i]-=src[i];
    }
}

template <typename T>
void printArr(T* a, int k, string name){
	cout<<name<<": ";
	for(int i=0;i<k;i++){
		cout<<a[i]<<" ";
	};
	cout<<"\n";
}

void printPoly(int* cf){ // �������� ������� � ����� ����������� ����
    const char sign[] = { '-','+'};
    bool last=true;
    for(int i=n-1;i>=0;i--){
        if(cf[i]!=0){
            if(!last){
                cout << sign[(bool)(cf[i]>0)];
            }
                else last=false;
            cout << abs(cf[i]) << "*" << "x^" << i ;
        };
    };
    cout << "\n";
};
    
double value( double x, int m){
	double v=0.0, r=1;
	for(int i=0;i<=m;i++){
		v+=r*matr[m][i];
		r*=x;
	};
	return v;
}
    
double bissec(double x1, double x2, int m){ // ����������� �������, ����������� ����� ���������
	double x, res;
	x=(x1+x2)/2;
	if (abs(value(x,m))<e) res=x;
		else 	
			if ((value(x1,m)*value(x,m))<0) res=bissec(x1,x,m);
				else res=bissec(x,x2,m);
	return res;
}
    
void roots(int p){ // �������, ��������� ����� p-�� ���������� �� ������ ����������� � ����������� �� � ���������� ������ ������
	// cout << "roots (" << p << "):\n"; // ��� ������� 
	double* temp = new double [p];
	double* buf = new double [p+1];
	for (int i=0; i<p-1; i++){
		buf[i+1] = rootArr[i];
	}
	buf[0]=a; buf[p]=b;
	for (int i=0; i<p; i++){
		temp[i] = bissec(buf[i],buf[i+1],p);
		// cout << temp[i] << " ";
    };
    // cout << "\n";
	for (int i=0; i<p; i++){
		rootArr[i] = temp[i];
	}
	delete[] temp;
	delete[] buf;
};

void getPoly(){ // �������, ������������ ������� ������������� 
    int* cf0=matr[0]; // � ���������� �� ������ ������, �.�. ��� ������� ��������, ������� ���������� ������
    int* cf1=matr[1];
    int* curr=matr[2];
    for (int i=0;i<n;i++){
    	setNull(matr[i]);
    };
    cf0[0]=1;   cf1[1]=1;
    roots(1);
    for (int i=2;i<n;i++){ // ���-�� � ���� ����� ��� � ����� ������� roots, ������ �����,
    	curr=matr[i]; // ���� ������, ��-�� ������� ��� ������������ ������� ��������� n
        mult(curr,cf1); // (��������, 9)
        min(curr,cf0); // ��� ��� ������������ ����� n � e (��������, 9 � 0.007, 10 � 0.5)
        cf0=cf1; // ��������� �������� ��� ������� ������ ���� ����� 3 ��� 5 ���������,
        cf1=curr; // ������ ��� ������ n ��� �� �������� ��� ������� ��� �������
        roots(i); // � ���������� ������� � dev-cpp �������� ������ 3221226356,
    }; // � ������ ����������� ������� ��������� � ��������� �����, �� � ������� 134, 
    printArr(curr, n, "Coefficients "); // ���� ��������� ����� ��������
    printPoly(curr); // � �� �������, ��� ����� ���� ������ � ��� ��� ����� �������� �� n
}

int main()
{   
    srand(time(0));
    int input;
    cout << "Enter 0 for hand input or 1 for random values: ";
    cin >> input;
    if (input == 0){
        cout << "Enter N (degree) in range [2;20] and E (precision): ";
        cin >> n >> e;
    } 
        else {
            n = rand()%18+2;
            cout << "n: " << n;
            e = rand()%10*mul;
            while (e==0) e = rand()%10*mul;
            cout << "e: " << e << endl;
        };
    if (n>nmax||n<nmin){
        cout << "Wrong input! n: ";
        cin >> n;
	}
    int k=n;
    n++;
    matr = new int* [n];
    for (int i=0; i<n; i++)
    	matr[i] = new int [n];
    rootArr = new double [n];
    setNull(rootArr);
    getPoly(); // ������ �������� � ������� ����� �� n
    printArr(rootArr, k, "Roots ");
    for (int i=0; i<n; i++)
    	delete[] matr[i];
    delete[] matr;
    delete[] rootArr;
    return 0;
}
