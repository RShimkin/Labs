// лаба 4 Шимкин Р.Р. 62 группа 1 курс
// стек семафор 

#include <iostream>
#include <Windows.h>
#include <thread>
#include <atomic>
#include <stack>
#include <vector>
#include <conio.h>

using namespace std;

//const int n = 5;
const int ARRNUM = 15; // кол-во массивов
const int PROD = 4; // кол-во производителей
const int CONS = 3; // кол-во потребителей
const int LEN = 15; // длина массивов
const int ret = 40;

int mul(int x) { return x * x; }

atomic_flag lockProd = ATOMIC_FLAG_INIT; // флаг для производителей
atomic_flag lockCons = ATOMIC_FLAG_INIT; // флаг для потребителей

class ThreadSafeStack {
    stack<vector<int>> st;
    HANDLE sem;
    int nprod, ncons;
public:
    ThreadSafeStack() {
        sem = CreateSemaphore(NULL, 1, 1, NULL);
    }
    void push(int id, vector<int> k) {
        // бесконечно ждём семафор для pusha и консоли
        WaitForSingleObject(sem, INFINITE); 
        st.push(k);
        print(id, k, false);
        // переключаем семафор
        ReleaseSemaphore(sem, 1, NULL);
    }
    bool pop(int id, vector<int>& a) {
        bool exist = false;
        // бесконечно ждём семафор для извлечения и консоли
        WaitForSingleObject(sem, INFINITE);
        // проверяем пустоту стека
        if (!st.empty()) {
            exist = true;
            a = st.top();
            st.pop();
            print(id, a, true);
        }
        // переключаем семафор
        ReleaseSemaphore(sem, 1, NULL);
        return exist;
    }
    void print(int id, vector<int>& a, bool how) {
        if (how) {
            cout << endl << "\tC" << id + 1 << "<<<  (" << a[LEN] << ") ";
            for (int i : a) cout << i << ' ';
            cout << endl;
        }
        else {
            cout << endl << "P" << id + 1 << ">>>\t       (" << a[LEN] << ") ";
            for (int i : a) cout << i << ' ';
            cout << endl;
        }
    }
    ~ThreadSafeStack() {
        CloseHandle(sem);
    }
};

ThreadSafeStack s; // глобальный потокобезопасный стек

// ф-ия генерации массива
void genArr(int id, int& k) {
    static int pos = 1;
    srand(time(0));
    bool h = true; // флаг оставшихся массивов
    while (h) {
        while (lockProd.test_and_set()) Sleep(0);
        if (k > 0) k--; // понижаем кол-во оставшихся массивов (атомарно)
            else h = false;
        lockProd.clear();
        // если остались массивы, то генерируем
        if (h) {
            vector<int> tmp;
            for (int i = 0; i < LEN; i++) {
                int ch = id + rand() % 100;
                tmp.push_back(ch);
                Sleep(ret * mul(id) * (rand()%5)); // замедление
            }
            tmp.push_back(pos);
            pos++;
            s.push(id, tmp);
        }
    }
}

// ищем максимум в массиве
void findMax(int id, int& k, vector<int>& vectmax) {
    static int pos = 0;
    bool h = true; // флаг оставшихся массивов
    while (h) {
        while (lockCons.test_and_set()) Sleep(0);
        if (k > 0) k--; // понижаем кол-во оставшихся массивов (атомарно)
            else h = false;
        lockCons.clear();
        // если остались массивы, то достаём и ищем макс
        if (h) { 
            vector<int> tmp;
            bool exist = s.pop(id, tmp);
            // ждём, пока в стеке появится массив 
            while (!exist) {
                exist = s.pop(id, tmp);
                Sleep(0);
            }
            // ищем макс
            int max = tmp[0];
            for (int i : tmp) {
                if (max < i) max = i;
                Sleep((rand() % 5) * ret / 2);
            }
            vectmax.push_back(max);
        }
    }
}

// основная ф-ия
void Work() {
    thread t[PROD + CONS];
    int w = ARRNUM, v = ARRNUM;
    vector<int> max;
    int n = PROD + CONS; // кол-во потоков
    for (int i = 0; i < PROD; i++) t[i] = thread(genArr, i, ref(w));
    for (int i = PROD; i < n; i++) t[i] = thread(findMax, i, ref(v), ref(max));
    for (int i = 0; i < n; i++) {
        if (t[i].joinable()) t[i].join();
    }
    // печатаем максы
    cout << "\nmax: ";
    for (int g : max) cout << g << ' ';
}

int main()
{
    _getch();
    Work();
}
