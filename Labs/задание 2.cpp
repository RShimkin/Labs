#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;
double e;

const float mul = 0.001, a = -1.0, b = 1.0;
const int nmax = 20, nmin = 2;

int n;
int** cfmatr;

#define mas(a,n) int* a=new int[n];
#define matr(a,n) a=new int[n];

void setNull(int* a){
	for (int i=0; i<n;i++){
		a[i]=0;
	}
}

void mult(int* res, int* src){
    for (int i=0;i<n;i++){
        res[i+1]=src[i]*2;
    };
}

void min(int* res, int* src){
    for(int i=0;i<n;i++){
        res[i]-=src[i];
    }
}

int* getPoly(){
    int i;
    int** a=new int*[n]; 
	for (int i=0;i<n;i++)
	matr(a[i],n);
    int* cf0=a[0];
    int* cf1=a[1];
    int* curr=a[2];
    int next=3;
    for (i=0;i<n;i++){
    	setNull(a[i]);
    };
    cf0[0]=1;   cf1[1]=1;
    for (int i=2;i<n;i++){
        mult(curr,cf1);
        min(curr,cf0);
        cf0=cf1;
        cf1=curr;
        curr=a[next];
        next++;
    };
    cfmatr=a;
    return cf1;
}

void printArr(int* a, int k, string name){
	cout<<name<<": ";
	for(int i=0;i<k;i++){
		cout<<a[i]<<" ";
	};
	cout<<"\n";
}

void printArr(double* a, int k, string name){
	cout<<name<<": ";
	for(int i=0;i<k;i++){
		cout<<a[i]<<" ";
	};
	cout<<"\n";
}

void printPoly(int* cf){
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
    
double value(double x, int m){
	double v=0.0, r=1;
	for(int i=0;i<=m;i++){
		v+=r*cfmatr[m][i];
		r*=x;
	};
	cout << " value ( " << x << " " << m << ") = " << v;
	return v;
}
    
double bissec(double x1, double x2, int m){
	double x;
	x=(x1+x2)/2;
	if (abs(value(x,m))<e) {
		cout << "X" << m << " = " << x << "		";
		return x;
	}	
	if ((value(x1,m)*value(x,m))<0){
		return bissec(x1,x, m);
	}
		else { 
		return bissec(x,x2,m);}
}
    
double* roots(int p){
	if (p==1){
		double* rt;
		*rt = bissec(a, b, p);
		cout << "*rt =" << *rt << " at adresse " << rt << "\n";
		return rt;
	}
		else if (p==2){
			double* rt = roots(p-1);
			double* nrt = new double[p];
			nrt[0]=bissec(a, *rt, p);
			nrt[p-1]=bissec(*rt, b, p);
			cout << "nrt0, nrt1, adresse (nrt): " << nrt[0] << " " << nrt[1] << " " << nrt << "\n";
			return nrt;
		}
			else {
				double* rt = roots(p-1);
				double* nrt = new double[p];
				for (int i=0;i<p-2;i++){
				nrt[i+1]=bissec(rt[i],rt[i+1],p);
				};
				nrt[0]=bissec(a, rt[0], p);
				nrt[p-1]=bissec(rt[p-2], b, p);
				cout << "nrt = " << nrt << "\n";
				return nrt;
			}
};
    
int main()
{   
    srand(time(0));
    int input;
    cout << "Enter 0 for hand input or 1 for random values";
    cin >> input;
    if (input == 0){
        cout << "Enter N ( ... of the ... ) in range [2;20] and E (precision):";
        cin >> n >> e;
    } 
        else {
            n = rand()%20 + 2;
            e = rand()%10*mul;
        };
    if (n>nmax||n<nmin)
        cout << "Wrong input!" << exit;
    int k=n;
    n++;
    mas(coef,n);
    coef=getPoly();
    printArr(coef, n, "Coefficients ");
    printPoly(coef);
    double* rootArr = new double[n];
    rootArr = roots(k);
    
    return 0;
}
