#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>

using namespace std;

static double e;
static double* oldroot=NULL, *newroot;

const float mul = 0.001, A = -1.0, B = 1.0;
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
    int* curr;
    int next=2;
    for (i=0;i<n;i++){
    	setNull(a[i]);
    };
    cf0[0]=1;   cf1[1]=1;
    for (int i=2;i<n;i++){
    	curr=a[next++];
        mult(curr,cf1);
        min(curr,cf0);
        cf0=cf1;
        cf1=curr;
    };
    cfmatr=a;
    return cf1;
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
	cout << "value (" << x << " " << m << ") = " << v << "\n";
	return v;
}
    
double bissec(double x1, double x2, int m){
	double x;
	x=(x1+x2)/2;
	cout<<"x (bissec "<< x1 << " " << x2 << " " << m << ") = " << x << "\n";
	if (abs(value(x,m))<e) return x;
	if ((value(x1,m)*value(x,m))<0){
		cout << "\n";
		return bissec(x1,x, m);
	}
		else { 
		
		cout << "\n";
		return bissec(x,x2,m);}
}
    
void roots(int k){
	 	oldroot = newroot;
	 	int m = 2;
	 	cout << " Oldroot =" << oldroot << "-->" << *oldroot;
	 	static double* buf = new double[m];
	 	buf[0] = bissec(A, *oldroot, m);
	 	buf[1] = bissec(*oldroot, B, m);
	 	newroot = buf;
	    for (int i=3; i<=k; i++){
	 		oldroot = newroot;
	 		static double* buf = new double[i];
	 		for(int j=0; j<i-3; j++){
	 			buf[j+1] = bissec(oldroot[j], oldroot[j+1], i);
		 	};
	 		buf[0] = bissec(A, oldroot[0], i);
	 		buf[i-1] = bissec(oldroot[i-2], B, i);
	 		newroot = buf;
	 }
}
    
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
        cout << "Wrong input!" ;
    int k=n;
    n++;
    mas(coef,n);
    coef=getPoly();
    printPoly(coef);
	cout << oldroot << "	" << newroot;
	newroot = new double[k];
	cout << oldroot << "	" << newroot;
	return 0;
}
