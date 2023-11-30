// Шимкин Р.Р. 62 группа ПММ ВГУ Dev-cpp 5.11
// задание: вывести корня n-го многочлена Чебышева (n={2..20}) с заданной точностью е на участке [-1;1]; F(n) = 2xf(n-1) - f(n-2)
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
using namespace std;
double e; // точность 
double *rootArr; // указатель для массива корней
int **matr; // указатель для матрицы коэффициентов
int n; // степень искомого полинома и размерность массива корней

const double mul = 0.001, a = -1.0, b = 1.0; // границы отрезка и множитель для генерации
const int nmax = 20, nmin = 2; // границы степеней полинома

template <typename T> 
void setNull(T* a){ 
	for (int i=0; i<n; i++){
		a[i]=0;
	}
}

void mult(int* res, int* src){ // функция, реализующая промежуточное действие f(n) = 2x*f(n-1) в коэффициентах
    for (int i=0;i<n;i++){
        res[i+1]=src[i]*2;
    };
}

void min(int* res, int* src){ // промежуточное действие f(n) = ... - f(n-2)
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

void printPoly(int* cf){ // печатает полином в почти стандартном виде
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
    
double bissec(double x1, double x2, int m){ // рекурсивная функция, реализующая метод биссекции
	double x, res;
	x=(x1+x2)/2;
	if (abs(value(x,m))<e) res=x;
		else 	
			if ((value(x1,m)*value(x,m))<0) res=bissec(x1,x,m);
				else res=bissec(x,x2,m);
	return res;
}
    
void roots(int p){ // функция, находящая корни p-го многочлена по корням предыдущего и сохраняющая их в глобальный массив корней
	// cout << "roots (" << p << "):\n"; // для отладки 
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

void getPoly(){ // функция, составляющая матрицу коэффициентов 
    int* cf0=matr[0]; // и вызывающая по каждой строке, т.е. для каждого полинома, функцию нахождения корней
    int* cf1=matr[1];
    int* curr=matr[2];
    for (int i=0;i<n;i++){
    	setNull(matr[i]);
    };
    cf0[0]=1;   cf1[1]=1;
    roots(1);
    for (int i=2;i<n;i++){ // где-то в этом цикле или в самой функции roots, скорее всего,
    	curr=matr[i]; // есть ошибка, из-за которой при определенных входных значениях n
        mult(curr,cf1); // (например, 9)
        min(curr,cf0); // или при определенных парах n и e (например, 9 и 0.007, 10 и 0.5)
        cf0=cf1; // программа вылетает при расчете корней чаще всего 3 или 5 полиномов,
        cf1=curr; // причём при других n эти же полиномы она считает без проблем
        roots(i); // в проблемных случаях в dev-cpp вылетает ошибка 3221226356,
    }; // в онлайн компиляторе полином считается и выводится ответ, но с ошибкой 134, 
    printArr(curr, n, "Coefficients "); // либо программа также вылетает
    printPoly(curr); // я не понимаю, где может быть ошибка и как она может зависеть от n
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
    getPoly(); // строим полиномы и находим корни до n
    printArr(rootArr, k, "Roots ");
    for (int i=0; i<n; i++)
    	delete[] matr[i];
    delete[] matr;
    delete[] rootArr;
    return 0;
}
