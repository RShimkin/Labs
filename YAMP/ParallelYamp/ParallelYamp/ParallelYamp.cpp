// Шимкин Р.Р.  62 группа  
// найти минимальный элемент в целочисленном массиве
#include <windows.h>
#include <iostream>
#include <process.h>
#include <thread>

using namespace std;

// спит 0.2, выводит t двоек)
DWORD WINAPI func2(LPVOID t) { 
    for (int i = 0; i < *(static_cast<int*>(t)); i++) {
        Sleep(200);
        cout << "2";
    }
    return 0;
}

// спит 0.02, выводит 100 троек
unsigned long __stdcall func3(void *t) {
    for (int i = 0; i < 100; i++) {
        Sleep(20);
        cout << "3";
    };
    return 0;
}

const int COUNT = 20000000;
const int NTHREAD = 12;
int arr[COUNT];

// структура для данных каждого потока: массив, лево право, результат, временные метки
using INFORM = struct {
    int* a;
    int left, right, res;
    clock_t begin, end;
};

// последовательная функция
void nonparallel(int* a) {
    int min = a[0];

    clock_t st1 = clock();
    for (int i = 1; i < COUNT; i++) {
        if (a[i] < min) {
            min = a[i];
        }
    };
    clock_t st2 = clock();

    clock_t st = st2 - st1;
    float t = st2 - st1;

    cout << "\nnonparallel: ";
    cout << "\n" << t << "ms";
    cout << "\nres: " << min;
    cout << endl;
}

unsigned __stdcall func(void* arg) {
    INFORM* inf = (INFORM*)arg; 
    int l = inf->left, r = inf->right, min = inf->a[inf->left];

    inf->begin = clock();
    for (int i = l + 1; i < r; i++) {
        if (inf->a[i] < min) min = inf->a[i];
    }
    inf->end = clock();

    inf->res = min;
    if (inf->right != COUNT) _endthreadex(0);
    return 0;
}

// ф-ия для thread без критической секции
void fun(int* a, int l, int r, int& res) {
    res = a[l];
    for (int i = l + 1; i < r; i++) {
        if (a[i] < res) res = a[i];
    };
}

// параллельная на threadах без критической секции
void parallel3(int* a) {
    thread t[NTHREAD];
    int mins[NTHREAD];
    int n = COUNT / NTHREAD;
    int* arr = new int[COUNT];
    int last = NTHREAD - 1;

    clock_t st1 = clock();
    for (int i = 0; i < last; i++) {
        t[i] = thread(fun, a, n * i, n * (i + 1), ref(mins[i]));
    }
    //t[NTHREAD - 1] = thread(fun, a, (n * last), COUNT, ref(mins[last]));
    //for (int i = 0; i < NTHREAD; i++) t[i].join();
    fun(a, n * last, COUNT, mins[last]);
    for (int i = 0; i < NTHREAD - 1; i++) t[i].join();
    clock_t st2 = clock();

    int res;
    fun(mins, 0, NTHREAD, res);

    float sec = st2 - st1;
    cout << "\nparallel3 (thread): ";
    cout << "\ntime: " << sec << "ms";
    cout << "\nres: " << res;
    cout << endl;
}

// параллельная на beginthreadex + главный поток
void parallel(int* a) {
    HANDLE t[NTHREAD - 1];
    INFORM inf[NTHREAD];
    int n = COUNT / NTHREAD;

    inf[NTHREAD - 1].a = arr;
    inf[NTHREAD - 1].left = (NTHREAD - 1) * n;
    inf[NTHREAD - 1].res = 0;
    inf[NTHREAD - 1].right = COUNT;

    for (int i = 0; i < NTHREAD - 1; i++) {
        inf[i].a = arr;
        inf[i].left = n * i;
        inf[i].res = 0;
        inf[i].right = n * (i + 1);
    }

    clock_t st1 = clock();
    for (int i = 0; i < NTHREAD - 1; i++) 
        t[i] = (HANDLE)_beginthreadex(NULL, 0, &func, &inf[i], 0, NULL);
    func(&inf[NTHREAD - 1]);
    WaitForMultipleObjects(NTHREAD - 1, t, true, INFINITE);
    clock_t st2 = clock();

    clock_t begin = inf[0].begin, end = inf[0].end;
    for (int i = 1; i < NTHREAD; i++) {
        if (inf[i].begin < begin) begin = inf[i].begin;
        if (inf[i].end > end) end = inf[i].end;
    };

    int res = inf[0].res;
    for (int i = 1; i < NTHREAD; i++) 
        res = min(res, inf[i].res);

    for (int i = 0; i < NTHREAD - 1; i++) CloseHandle(t[i]);

    float sec = ((float)(st2 - st1)) / CLOCKS_PER_SEC;

    cout << "\nparallel (beginthread): ";
    cout << "\ntime: " << sec << "s";
    cout << "\nprecise: " << ((float)(end - begin)) / CLOCKS_PER_SEC << "s ";
    cout << "\nres: " << res;
    cout << endl;
}

// параллельная на beginthreadex
void parallel2(int* a) {
    HANDLE t[NTHREAD];
    INFORM inf[NTHREAD];
    int n = COUNT / NTHREAD;

    /*
    inf[NTHREAD - 1].a = arr;
    inf[NTHREAD - 1].left = (NTHREAD - 1) * n;
    inf[NTHREAD - 1].res = 0;
    inf[NTHREAD - 1].right = COUNT;
    */
    for (int i = 0; i < NTHREAD; i++) {
        inf[i].a = arr;
        inf[i].left = n * i;
        inf[i].res = 0;
        if (i == NTHREAD - 1) inf[i].right = COUNT;
        else inf[i].right = n * (i + 1);
    }

    clock_t st1 = clock();
    for (int i = 0; i < NTHREAD; i++) 
        t[i] = (HANDLE)_beginthreadex(NULL, 0, &func, &inf[i], 0, NULL);
    WaitForMultipleObjects(NTHREAD, t, true, INFINITE);
    clock_t st2 = clock();

    clock_t begin = inf[0].begin, end = inf[0].end;
    for (int i = 1; i < NTHREAD; i++) {
        if (inf[i].begin < begin) begin = inf[i].begin;
        if (inf[i].end > end) end = inf[i].end;
    };

    int res = inf[0].res;
    for (int i = 1; i < NTHREAD; i++) 
        res = min(res, inf[i].res);

    for (int i = 0; i < NTHREAD; i++) 
        CloseHandle(t[i]);

    float sec = ((float)(st2 - st1)) / CLOCKS_PER_SEC;

    cout << "\nparallel2 (beginthread): ";
    cout << "\ntime: " << sec << "s";
    cout << "\nprecise: " << ((float)(end - begin)) / CLOCKS_PER_SEC << "s ";
    cout << "\nres: " << res;
    cout << endl;
}

// глобальный замок
volatile long lock = false;

// ф-ия для thread с блокировкой (interlock)
void funcInterlock(int* a, int l, int r, volatile int& global) {
    int local = a[l];
    for (int i = l + 1; i < r; i++) {
        if (a[i] < local) local = a[i];
    }
    if (local < global) {
        while (_InterlockedExchange(&lock, true)) 
            Sleep(2);
        if (local < global) global = local;
        _InterlockedExchange(&lock, false);
    }
}

// параллельная на threadах с interlockом
void parallel4(int* a) {
    thread t[NTHREAD];
    int n = COUNT / NTHREAD;
    int last = NTHREAD - 1;
    int res = INT_MAX;
    clock_t st1 = clock();
    for (int i = 0; i < last; i++) {
        t[i] = thread(funcInterlock, a, n * i, n * (i + 1), ref(res));
    }
    //t[NTHREAD - 1] = thread(funcInterlock, a, n * last, COUNT, ref(res));
    //for (int i = 0; i < NTHREAD; i++) t[i].join();
    funcInterlock(a, n * last, COUNT, res);
    for (int i = 0; i < NTHREAD - 1; i++) t[i].join();
    clock_t st2 = clock();
    float sec = st2 - st1;
    cout << "\nparallel4 (interlocked): ";
    cout << "\ntime: " << sec << "ms";
    cout << "\nres: " << res;
    cout << endl;
}

void parallel5(int* a, int num) {
    num %= 12;
    thread* t = new thread[num];
    int n = COUNT / num;
    int last = num - 1;
    int res = INT_MAX;
    clock_t st1 = clock();
    for (int i = 0; i < last; i++) {
        t[i] = thread(funcInterlock, a, n * i, n * (i + 1), ref(res));
    }
    //t[NTHREAD - 1] = thread(funcInterlock, a, n * last, COUNT, ref(res));
    //for (int i = 0; i < NTHREAD; i++) t[i].join();
    funcInterlock(a, n * last, COUNT, res);
    for (int i = 0; i < num - 1; i++) t[i].join();
    clock_t st2 = clock();
    float sec = st2 - st1;
    cout << "\nparallel5 (dynamic interlocked): ";
    cout << "\ntime: " << sec << "ms";
    cout << "\nres: " << res;
    cout << endl;
}

void print(int* a, int s = -1) {
    if (s == -1) {
        int bar = 25;
        cout << a[0];
        for (int i = 1; i < COUNT; i++) {
            cout << a[i] << ",";
            if (i % bar == 0) cout << "\n";
        }
    }
}

int main()
{
    srand(time(0));
    /*                     test
    int* p = new int(100);
    HANDLE tr2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);
    HANDLE tr3 = CreateThread(NULL, 0, func3, NULL, 0, NULL);
    for (int i = 0; i < 100; i++) cout << "1";
    */

    cout << "size: " << COUNT << endl;
    int dif = rand() % 100;
    clock_t st1 = clock();
    for (int i = 0; i < COUNT; i++) arr[i] = (rand() % 30) + dif;
    clock_t st2 = clock();
    cout << ": " << st2 - st1 << endl;
    //print(arr);

    int num;
    
    nonparallel(arr);
    //parallel(arr);
    //parallel2(arr);
    //parallel3(arr);
    //parallel4(arr);
    cin >> num;
    parallel5(arr, num);

}