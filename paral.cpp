// Шимкин Р.Р.  62 группа  
// найти минимальный элемент в целочисленном массиве
#include <windows.h>
#include <iostream>
#include <process.h>
#include <thread>

using namespace std;

// спит 0.2, выводит t двоек
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

--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

#include <iostream>
#include "List1.h"
#include <fstream>
#include <string>
#include <windows.h>

string message[10] = {
	" #Press 0 for exit",
	" #Press 1 for file input",
	" #Press 2 for manual input to head",
	" #Press 3 for manual input to tail",
	" #Press 4 to print the list",
	" #Press 5 to count the enumber of elements",
	" #Press 6 to find the length of the longest increasing sequence",
	" #Press 7 to delete all elements",
	" #Press 8 to delete k elements",
	" #Press 9 to clear the window"
};

using namespace std;

int main() {

	Tlist spisok;
	Init(spisok);

	unsigned int choice;
	bool goon = true;
	string buf = "";

	for (auto str : message)
		cout << str << endl;

	while (goon) {

		cout << "	Your choice: ";
		cin >> choice;
		
		switch (choice) {

		case 0: {
			goon = false;
			break;
		}

		case 1: {
			string name;
			cout << "Enter the file name:\n";
			cin >> name;
			ifstream file(name.c_str());
			if (!file.is_open()) {
				cout << "File is not open!";
				break;
			}

			int value;
			while (!file.eof()) {
				file >> value;
				AddToTail(spisok, value);
			}

			if (file.is_open()) {
				cout << "The file is read\n";
			} 
			file.close();
			break;
		}

		case 2: {
			cout << "Enter the element: ";
			int elem;
			cin >> elem;
			AddToHead(spisok, elem);
			break;
		}

		case 3: {
			cout << "Enter the element: ";
			int elem;
			cin >> elem;
			AddToTail(spisok, elem);
			break;
		}

		case 4: {
			View(spisok);
			break;
		}

		case 5: {
			cout << Count(spisok) << " elements" << endl;
			break;
		}

		case 6: {
			cout << "The length is: " << LongestIncSequence(spisok) << endl;
			break;
		}

		case 7: {
			Delete(spisok);
			cout << "The list is clear";
			break;
		}

		case 8: {
			cout << "Enter the number of elements to delete\n";
			cout << "( k for deletion from head and -k for deletion from tail): ";
			int k;
			cin >> k;
			if (abs(k) > Count(spisok)) {
				cout << "The list is shorter than the number you specified!";
				break;
			}
			Delete(spisok, k);
			break;
		}

		case 9: {
			system("cls");
			for (auto str : message)
				cout << str << endl;
			break;
		}
	 }
	}

	system("pause");
	return 0;
}

--------------

#pragma once

using namespace std;

typedef int Tinfo;

struct NODE {
	Tinfo info;
	NODE* next;
};

typedef NODE* Tlist;

void Init(Tlist&);

bool Empty(Tlist);

void AddToHead(Tlist&, int);

int Count(Tlist list);

void AddToTail(Tlist&, int);

void View(Tlist);

void Delete(Tlist&);

void Delete(Tlist&, int);

int LongestIncSequence(Tlist);

---------

#include "List1.h"
#include <iostream>

using namespace std;

void Init(Tlist& list) {
	list = NULL;
}

bool Empty(const Tlist list) {
	return list == NULL;
}

void AddToHead(Tlist& list, int newElem) {
	Tlist p = new NODE;
	p->info = newElem;
	p->next = list;
	list = p;
}

int Count(Tlist list) {
	int k = 0;
	Tlist p = list;
	while (p != NULL) {
		k++;
		p = p->next;
	}
	return k;
}

void AddToTail(Tlist& list, int newElem) {
	Tlist p = new NODE;
	p->info = newElem;
	p->next = NULL;
	if (Empty(list)) {
		list = p;
	}
	else {
		Tlist it = list;
		while (it->next != NULL) it = it->next;
		it->next = p;
	}
}

void View(Tlist list) {
	Tlist it = list;
	cout << "List: ";
	while (it != NULL) {
		cout << it->info << " ";
		it = it->next;
	}
	cout << endl;
}

void Delete(Tlist& list) {
	if (!(Empty(list))) {
		Tlist p;
		while (list != NULL) {
			p = list;
			list = list->next;
			p->next = NULL;
			delete p;
		}
	}
}

void Delete(Tlist& list, int pos) {
	if (pos > 0) {
		for (int i = 0; i < pos; i++) {
			Tlist it = list;
			list = list->next;
			it->next = NULL;
			delete it;
		}
	}
	else {
		int n = Count(list) + pos;
		Tlist it = list;
		for (int i = 1; i < n; i++) {
			it = it->next;
		}
		Tlist delit = it->next;
		it->next = NULL;
		for (; delit != NULL; ) {
			Tlist p = delit;
			delit = delit->next;
			p->next = NULL;
			delete p;
		}
	}
}

int LongestIncSequence(Tlist list) {
	int k;
	if (Empty(list)) k = 0;
	else {
		k = 0;
		int prev = INT_MIN;
		int temp = 0;
		Tlist it = list;
		while (it != NULL) {
			if (it->info > prev) {
				temp++;
				prev = it->info;
			}
			else {
				if (temp > k) k = temp;
				temp = 1;
				prev = it->info;
			}
			it = it->next;
		}
		if (temp > k) k = temp;
	}
	return k;
}

----------------------------------------------------------------------
----------------------------------------------------------------------

// Шимкин Р.Р. 62 группа ПММ 2 курс  Лаба №3 (жадные алгоритмы, "стабильные бракосочетания")

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <map>

using namespace std;

// класс человека
class Person {
public:
    char name;
    int size;
    int now; 
    Person* partner;  // указатель на партнера
    Person** pref; // массив указателей на предпочитаемых людей
    int varNum; // номер следующего предпочитаемого партнера
    // пустой конструктор по умолчанию, чтобы не ругался компилятор
    Person() {

    }
    // задаём имя, пустого партнера, размер множества,
    // обнуляем номер следующего, иниц-ем пустые указатели предпочтений
    Person(char n, int k) {
        name = n;
        varNum = 0;
        partner = NULL;
        size = k;
        now = 0;
        pref = new Person * [size];
        for (int i = 0; i < size; i++) {
           pref[i] = NULL;
        }
    }
    // функция, возвращающая следующее предпочтение
    Person* nextVar() {
        if (varNum < size)
            return pref[varNum++];
        else {
            // если массив предпочтений кончился, начинаем его с начала
            varNum = 0;
            return pref[varNum];
        }
    }
    // функция ищет позицию человека в списке предпочтений
    int findPos(Person* pers) {
        for (int i = 0; i < size; i++) {
            if (pref[i] == pers)
                return i;
        }
    }
    // сравнивает текущего партнера и кандидата по позиции в списке предпочтений
    bool isBetter(Person* candidate) {
        if (!partner) return true;
        else {         // если есть партнер, то сравниваем
            int curPos = findPos(partner);
            int newPos = findPos(candidate);
            if (curPos > newPos) return true;
            else return false;
        }
    }
    // добавляем человека в список предпочтений
    void add(Person* pers) {
        pref[now++] = pers;
    }
};

// основной класс бракосочетания
class Marriage {
    const int size;
    int unmarried;  // счетчик числа холостых мужчин
    set<char> names; // множетство имен
    int manNow, womanNow;
public:
    Person* man;
    Person* woman;
    Marriage(int num) : size(num) {
        man = new Person[size];
        woman = new Person[size];
        unmarried = size;
        manNow = womanNow = 0;
    }
    // функция возвращает указатель на человека по его имени
    Person* find(char x) const {
        bool isMan = ((x >= 'a') && (x <= 'z')); // проверка имени на мужское
        if (isMan) {
            // поиск в массиве мужчин
            for (int i = 0; i < size; i++) {
                if (man[i].name == x) return &man[i];
            }
        }
        else {
            // поиск в массиве женщин
            for (int i = 0; i < size; i++) {
                if (woman[i].name == x) return &woman[i];
            }
        }
    }
    // парсинг строки имен
    void parseNames(string str) {
        int pos = 0;
        while (pos != str.length()) {
            char x = str[pos];
            // проверка на мужское мужское имя
            bool firstCondition = ((x >= 'a') && (x <= 'z'));
            // проверка на женское имя
            bool secondCondition = ((x >= 'A') && (x <= 'Z'));
            // если не имя, а мусорный символ (или пробел), то пропускаем
            if (!(firstCondition || secondCondition))
                pos++;
            else {
                // иначе добавляем человека
                if (firstCondition) {
                    man[manNow++] = Person(x, size);
                    names.insert(x);
                }
                else woman[womanNow++] = Person(x, size);
                pos++;
            }
        }
    }
    // парсинг строки предпочтений
    void parse(string str) {
        char name = str[0];
        // подстрока предпочтений (со 2-го символа, после ';')
        string prefstr = str.substr(2, str.length() - 2);
        // указатель на текущего человека
        Person* cur = NULL;
        // проверка корректности имени текущего человека
        bool isGood = (((name >= 'a') && (name <= 'z')) || ((name >= 'A') && (name <= 'Z')));
        if (isGood) cur = find(name);
        else cout << "Wrong input!" << endl;
        if (cur) {
            int pos = 0;
            // берем по одному имени из подстроки предпочтений
            char newPrefName = prefstr[pos];
            // проверка на следующее предпочтение
            Person* newPref;
            while (pos != size) {
                // добавляем предпочтение
                newPref = find(newPrefName);
                cur->add(newPref);
                newPrefName = prefstr[++pos];
            }
        }
    }
    // основная функция алгоритма: устанавливает пары
    void setPairs() {
        // пока есть холостые
        while (unmarried) {
            // указатель на текущего человека
            Person* cur = NULL;
            // в цикле ищем холостого мужчину и ставим на него указ-ль cur
            for (int i = 0; i < size; i++) {
                if (!man[i].partner) {
                    cur = &man[i];
                    break;
                }
            }
            // указатель на следующее предпочтение текущего мужчины
            Person* var = cur->nextVar();
            // если у предпочтительной женщины нет партнера, то создаем пару
            if (!var->partner) {
                cur->partner = var;
                var->partner = cur;
                unmarried--;
            }
            // иначе проверяем, лучше ли текущий её пары
            else if (var->isBetter(cur)) {
                var->partner->partner = NULL;
                var->partner = cur;
                cur->partner = var;
            }
        }
    }
    // сохранение пар во внешний map
    void pairsToMap(map<char, char>& Map) {
        for (auto it = names.begin(); it != names.end(); it++)
            Map.insert(make_pair(*it, (find(*it)->partner)->name));
    }
};

int main()
{
    cout << "Input:" << endl;
    int numOfTests; // количество тестов
    cin >> numOfTests;
    vector<map<char, char>> maps;  // массив map-ов для хранения результатов всех тестов

    for (int test = 0; test < numOfTests; test++) {

        int n; // n человек каждого пола
        cin >> n;
        Marriage brak(n);
        string names;
        cin.ignore();
        getline(cin, names);
        brak.parseNames(names); // парсим имена

        // 2*n строк предпочтений
        for (int k = 0; k < 2 * n; k++) {
            string str;
            getline(cin, str);
            brak.parse(str);  // парсим предпочтения
        }

        brak.setPairs();
        map<char, char> tempMap; // временный map для получившихся пар
        brak.pairsToMap(tempMap);
        maps.push_back(tempMap);
        cout << endl;
    }
    cout << "Output:" << endl;
    // вывод map-ов с результатами всех тестов
    for (int i = 0; i < numOfTests; i++) {
        auto it = maps[i].begin();
        while (it != maps[i].end()) {
            cout << it->first << " " << it->second << endl;
            it++;
        }
        cout << endl;
    }
}

--------------------------------------------------------------
--------------------------------------------------------------

// YampLab4.cpp Shimkin R.R. AMM VSU 62
// 

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int m = INT_MAX / 2;

int inputSize() {
    int size;
    cout << "Enter the size: ";
    cin >> size;
    return size;
}

void print(int** a, int size) {
    for (int i = 0; i < size; i++) {
        int sum = 0;
        for (int k = 0; k < size; k++) {
            cout << a[i][k] << " ";
            sum += a[i][k];
        }
        cout << "| " << sum;
        cout << endl;
    }
    cout << endl;
}

int** inputMatr(int size) {
    int** matr = new int* [size];
    cout << "Enter by lines: \n";
    for (int i = 0; i < size; i++) {
        matr[i] = new int[size];
        for (int k = 0; k < size; k++) {
            cin >> matr[i][k];
            //if (matr[i][k] == 0) matr[i][k] = m;
        }
    }
    print(matr, size);
    return matr;
}

int** matrFromFile(int size) {
    cout << "Enter the file name: ";
    string name;
    cin >> name;
    ifstream file(name);
    int** matr = new int* [size];
    int temp;
    for (int i = 0; i < size; i++) {
        matr[i] = new int[size];
        for (int k = 0; k < size; k++) {
            file >> temp;
            if (temp == -1) matr[i][k] = m;
            else matr[i][k] = temp;
        }
    }
    return matr;
}

void Warshall(int** a, int size) {
    int x = 0;
    for (int k = 0; k < size; k++)
        for (int i = 0; i < size; i++)
            for (int w = 0; w < size; w++) {
                a[k][i] = std::min(a[k][i], a[k][w] + a[w][i]);
                x = a[k][i];
                x++;
            }
    print(a, size);
}

class City {
private:
    int pos, now, num;
    vector<int> neighbor;
    bool visited;
public:
    int sum;
    City(const int** a, int p, int size) {
        pos = p;
        num = 0;
        now = 0;
        sum = 0;
        visited = false;
        for (int i = 0; i < size; i++) {
            if (a[p][i] > 0) {
                num++;
                neighbor.push_back(i);
            }
        }
    }
    int next() {
        int val;
        if (num) val = neighbor[now];
        else val = -1;
        if (num) now = ++now % num;
        return val;
    }
    void visit() {
        if (visited == false) visited = true;
        else visited = false;
    }
    int count() {
        return num;
    }
    bool isVisited() {
        return visited;
    }
};

class Graph {
public:
    vector<City> city;
    int** matr;
    int** res;
    int size;
    int min, imin;
    int counter;
public:
    Graph(const int** a, int n) {
        size = n;
        for (int i = 0; i < n; i++) {
            city.push_back(City(a, i, n));
        }
        res = new int* [n];
        matr = new int* [n];
        for (int i = 0; i < n; i++) {
            res[i] = new int[n];
            matr[i] = new int[n];
            for (int k = 0; k < n; k++) {
                res[i][k] = a[i][k];
                matr[i][k] = a[i][k];
            }
        }
        min = INT_MAX;
        imin = -1;
        counter = 0;
        cout << "the city with min distance to others is: " << algorithm() + 1;
        cout << endl << counter << " recursive calls";
    }
    void printRes() {
        for (int i = 0; i < size; i++) {
            for (int k = 0; k < size; k++)
                cout << res[i][k] << " ";
            cout << "\n";
        }
    }
    void printMatr() {
        for (int i = 0; i < size; i++) {
            for (int k = 0; k < size; k++)
                cout << matr[i][k] << " ";
            cout << "\n";
        }
    }
    void recurs(int pos, int from, int s) {
        counter++;
        //if (s < min) {
            if (s < res[from][pos] || res[from][pos] == -1) res[from][pos] = s;
            city[pos].visit();
            int n = city[pos].count();
            for (int i = 0; i < n; i++) {
                int next = city[pos].next();
                if (next != -1 && !city[next].isVisited()) {
                    int ss = s;
                    if (res[pos][next] != -1) ss += res[pos][next];
                    if (ss < min) recurs(next, from, ss);
                }
            }
            city[pos].visit();
        //}
    }
    void sumup() {
        for (int i = 0; i < size; i++) {
            for (int k = 0; k < size; k++) if (res[i][k] > 0) city[i].sum += res[i][k];
        }
    }
    int findMin() {
        int min = 0;
        for (int i = 1; i < size; i++) {
            if (city[i].sum < city[min].sum) min = i;
        }
        return min;
    }
    void checkMin(int pos = -1) {
        int sum;
        if (pos == -1) {
            for (int i = 0; i < size; i++) {
                sum = 0;
                for (int k = 0; k < size; k++) {
                    if (res[i][k] != -1) sum += res[i][k];
                }
                if (sum < min) {
                    min = sum;
                    imin = i;
                }
            }
        }
        else {
            sum = 0;
            for (int k = 0; k < size; k++) {
                if (res[pos][k] != -1) sum += res[pos][k];
            }
            if (sum < min) {
                min = sum;
                imin = pos;
            }
        }
    }
    int algorithm() {
        int s = 0, sum = 0;
        checkMin();
        for (int i = 0; i < size; i++) {
            city[i].visit();
            int n = city[i].count();
            sum = 0;
            for (int k = 0; k < n; k++) {
                int next = city[i].next();
                if (next != -1) {
                    if (res[i][next] > 0) s += res[i][next];
                    recurs(next, i, s);
                    s = 0;
                }
            }
            city[i].visit();
            checkMin(i);
        }
        cout << endl;
        printMatr();
        cout << endl;
        //printRes();
        //sumup();
        return imin;
    }
    
};

int main()
{
    int min, size = inputSize();
    int** matr = matrFromFile(size);

    Graph graph((const int**)matr, size);
    cout << endl;
    Warshall(matr, size);
}

--------------------------------------------------------
--------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int NUM = 26;

struct Node {
    Node* pointers[NUM];
    bool eow;
};

typedef Node* Trie;

void init(Trie t) {
    t = nullptr;
}

bool empty(Trie t) {
    return t == nullptr;
}

void add(Trie& t, const string word, int i) {
    if (t == nullptr) {
        t = new Node;
        t->eow = false;
        for (int i = 0; i < NUM; i++) t->pointers[i] = nullptr;
    }
    if (word.length() - 1 < i) t->eow = true;
    else add(t->pointers[word[i] - 'a'], word, i + 1);
}

bool allEmpty(Trie t) {
    bool res = true;
    int i = 0;
    while (i < NUM && res) {
        if (!t->pointers[i]) i++;
        else res = false;
    }
    return res;
}

void del(Trie& t, const string word, int i) {
    if (t) {
        if (i <= word.length() - 1) del(t->pointers[word[i] - 'a'], word, i + 1);
        else {
            t->eow = false;
            if (allEmpty(t)) {
                delete t;
                t = nullptr;
            }
        }
    }
}

void print(Trie t, string word) {
    if (t->eow) cout << word << endl;
    for (int i = 0; i < NUM; i++)
        if (t->pointers[i]) print(t->pointers[i], word + char(i + 'a'));
}

void clear(Trie& t) {
    for (int i = 0; i < NUM; i++)
        if (t->pointers[i]) clear(t->pointers[i]);
    delete t;
    t = nullptr;
}

void addLetter(Trie& t, char ch) {
    for (int i = 0; i < NUM; i++) {
        if (t->pointers[i]) addLetter(t->pointers[i], ch);
    }
    if (t->eow) {
        if (!t->pointers[ch - 'a']) {
            t->pointers[ch - 'a'] = new Node;
            Trie temp = t->pointers[ch - 'a'];
            for (int k = 0; k < NUM; k++) temp->pointers[k] = nullptr;
        }
        Trie cur = t->pointers[ch - 'a'];
        cur->eow = true;
        t->eow = false;
    }
}

void addLetter2(Trie& t, string str) {
    for (int i = 0; i < NUM; i++) {
        if (t->pointers[i]) addLetter2(t->pointers[i], str);
    }
    if (t->eow) {
        add(t, str, 0);
        t->eow = false;
    }
}

void merge(Trie f, Trie s) {
    for (int i = 0; i < NUM; i++) {
        if (s->pointers[i] != nullptr) {
            if (f->pointers[i] == nullptr) {
                f->pointers[i] = s->pointers[i];
                s->pointers[i] = nullptr;
            }
            else merge(f->pointers[i], s->pointers[i]);
        }
    }
}

void swap(Trie& t, char f, char s) {
    Trie cur = t->pointers[f - 'a'];
    if (cur) {
        if (t->pointers[s - 'a']) merge(t->pointers[s - 'a'], cur);
        else t->pointers[s - 'a'] = cur;
        t->pointers[f - 'a'] = nullptr;
    }
    for (int i = 0; i < NUM; i++) 
        if (t->pointers[i]) swap(t->pointers[i], f, s);
}

void copyWithout1stLetter(Trie& t) {
    Trie n = new Node;
    for (int i = 0; i < NUM; i++) {
        n->pointers[i] = nullptr;
    }
    Trie cur = nullptr;
    for (int i = 0; i < NUM; i++) {
        cur = t->pointers[i];
        if (cur) {
            Trie temp = nullptr;
            for (int k = 0; k < NUM; k++) {
                temp = cur->pointers[k];
                if (temp) {
                    if (n->pointers[k]) merge(n->pointers[k], temp);
                    else n->pointers[k] = temp;
                }
                cur->pointers[k] = nullptr;
            }
        }
        t->pointers[i] = nullptr;
        delete cur;
    }
    t = n;
}

void delete1st(Trie& tree, Trie& t, bool* check, char ch) {
    if (t) {
        for (int i = 0; i < NUM; i++) {
            Trie cur = t->pointers[i];
            if (cur) {
                if (!check[i]) delete1st(tree, tree->pointers[i], check, (char)'a' + i);
                if (tree->pointers[i]) merge(tree->pointers[i], cur);
                else tree->pointers[i] = cur;
                t->pointers[i] = nullptr;
            }
        }
    }
    check[ch - 'a'] = true;
    tree->pointers[ch - 'a'] = nullptr;
    delete t;
}

void delete1stletter(Trie& t) {
    bool check[NUM] = { false };
    for (int i = 0; i < NUM; i++) delete1st(t, t->pointers[i], check, (char)'a' + i);
}

void printSeq(Trie& t, string seq, int pos, string word) {
    if (t) {
        if (pos == seq.length() && t->eow) cout << word << endl;
        else {
            char ch = seq[pos];
            Trie cur = t->pointers[ch - 'a'];
            if (cur) printSeq(cur, seq, pos + 1, word + ch);
        }
        Trie cur = nullptr;
        char ch;
        for (int i = 0; i < NUM; i++) {
            cur = t->pointers[i];
            ch = (char)('a' + i);
            if (cur) printSeq(cur, seq, pos, word + ch);
        }
    }
}

int main(){ 
    Trie tree = nullptr;
    init(tree);
    string str;
    for (int i = 0; i < 5; i++) {
        getline(cin, str);
        add(tree, str, 0);
    }
    cout << endl;
    //print(tree, "");
    cout << endl;

    addLetter(tree, 'a');
    //swap(tree, 'n', 'u');
    //copyWithout1stLetter(tree);
    //delete1stletter(tree);
    //printSeq(tree, "ro", 0, "");

    cout << endl;
    print(tree, "");
    cout << endl;

}

---------------------------------------------------
---------------------------------------------------

// Shimkin R.R. AMM 62 
// to compare 2 sorting algorithms - heap sort and insertion sort - on one input array

using namespace std;

#include <iostream>
#include <fstream>
#include <string>

void fromFile(ifstream& file, int arr[], long n) {
    string str;
    clock_t st1 = clock();
    for (long i = 0; i < n; i++) {
        getline(file, str);
        arr[i] = atoi(str.c_str());
    }
    clock_t st2 = clock();
    cout << "fromfile: " << (float)(st2 - st1) / 1000 << endl;
}

void printSome(int arr[], long n) {
    for (long i = 0; i < n; i += n / 10) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void heapify(int arr[], long n, long i) {
    long largest = i;
    long left = 2 * i + 1;
    long right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

float HeapSort(ifstream& file, long n) {
    int* arr = new int[n];
    fromFile(file, arr, n);
    printSome(arr, n);
    clock_t stamp1 = clock();
    for (long i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (long i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    clock_t stamp2 = clock();
    printSome(arr, n);
    delete arr;
    return ((float)(stamp2 - stamp1)) / 1000;
}

float HeapSort(int arr[], long n) {
    clock_t stamp1 = clock();
    for (long i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (long i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    clock_t stamp2 = clock();
    printSome(arr, n);
    return ((float)(stamp2 - stamp1)) / 1000;
}

float InsertionSort(ifstream& file, long n) {
    int val = 0;
    long i = 0;
    int* arr = new int[n];
    fromFile(file, arr, n);
    printSome(arr, n);
    clock_t stamp1 = clock();
    for (long k = 1; k < n; k++) {
        val = arr[k];
        i = k - 1;
        while (i >= 0 && arr[i] > val) {
            arr[i + 1] = arr[i];
            i -= 1;
            arr[i + 1] = val;
        }
        if (k % 10000 == 0) cout << ((float) k/n) * 100 << "%" << endl;
    }
    clock_t stamp2 = clock();
    printSome(arr, n);
    delete arr;
    return ((float)(stamp2 - stamp1)) / 1000;
}

float InsertionSortFast(ifstream& file, long n) {
    int val = 0;
    long i = 0;
    int* arr = new int[n];
    string str;
    clock_t stamp1 = clock();
    for (long k = 1; k < n; k++) {
        getline(file, str);
        int val = arr[k] = atoi(str.c_str());
        i = k - 1;
        while (i >= 0 && arr[i] > val) {
            arr[i + 1] = arr[i];
            i -= 1;
            arr[i + 1] = val;
        }
        if (k % 10000 == 0) cout << ((float)k / n) * 100 << "%" << endl;
    }
    clock_t stamp2 = clock();
    printSome(arr, n);
    return ((float)(stamp2 - stamp1)) / 1000;
}

float InsertionSort(int arr[], long n) {
    int val = 0;
    long i = 0;
    clock_t stamp1 = clock();
    for (long k = 1; k < n; k++) {
        val = arr[k];
        i = k - 1;
        while (i >= 0 && arr[i] > val) {
            arr[i + 1] = arr[i];
            i -= 1;
            arr[i + 1] = val;
        }
        if (k % 10000 == 0) cout << ((float)k / n) * 100 << "%" << endl;
    }
    clock_t stamp2 = clock();
    printSome(arr, n);
    return ((float)(stamp2 - stamp1)) / 1000;
}

float generate(int arr[], long n) {
    clock_t stamp1 = clock();
    for (long i = 0; i < n; i++) {
        int elem = rand();
        int sign = rand() % 3;
        int mul = (sign == 0) ? -1 : 1;
        arr[i] = mul * elem;
    };
    clock_t stamp2 = clock();
    return (float) (stamp2 - stamp1)/1000;
}

float generateToFile(ofstream& file, long n) {
    clock_t stamp1 = clock();
    for (long i = 0; i < n; i++) {
        int elem = rand();
        int sign = rand() % 3;
        int mul = (sign == 0) ? -1 : 1;
        file << mul * elem << "\n";
    };
    clock_t stamp2 = clock();
    return (float) (stamp2 - stamp1)/1000;
}

float copy(int first[], int second[], long n) {
    clock_t stamp1 = clock();
    for (long i = 0; i < n; i++)
        second[i] = first[i];
    clock_t stamp2 = clock();
    return (float) (stamp2 - stamp1) / 1000;
}

int main()
{
    srand(time(0));
    cout << "Enter the size of array: ";
    long n;
    cin >> n;
    //int *arr = new int[n];
    ofstream out("array.txt");
    cout << "\ngenerating...\n";
    cout << generateToFile(out,n) << " s\n";
    /*cout << generate(arr, n) << " s\n";
    int* arr2 = new int[n];
    int* arr3 = new int[n];
    cout << copy(arr, arr2, n) << " s\n";
    clock_t st1 = clock();
    memcpy(arr3, arr, n * sizeof(long));
    clock_t st2 = clock();
    cout << (float)(st2 - st1) / 1000 << " s\n";
    printSome(arr, n);
    printSome(arr2, n);
    printSome(arr3, n);*/
    ifstream in1("array.txt");
    ifstream in2("array.txt");
    cout << endl;
    
    cout << endl << " Heap sort: " << HeapSort(in1, n) << endl << endl;
    //cout << endl << " Heap sort: " << HeapSort(arr, n) << endl << endl;
    cout << endl << " Insertion sort: " << InsertionSort(in2, n) << endl << endl;
    //cout << endl << " Insertion sort: " << InsertionSort(arr2, n) << endl << endl;
}

-------------------------------------------------------------------
--------------------------------------------------------------------

// Roman Shimkin, 62 group, Lab 1, Variant 9
// to build Trie-Tree with reversed words

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

const int SIZE = 26;

int code(char x) {
    if (x >= 'A' && x <= 'Z') return x - 'A';
    else if (x >= 'a' && x <= 'z') return x - 'a';
    else return -1;
}

stack<char> strToStack(const string& str) {
    string copy(str);
    reverse(copy.begin(), copy.end());
    stack<char> st;
    for (int i = 0; i < copy.length(); i++)
        st.push(copy[i]);
    return st;
}

class Node {
    Node* ptrs[SIZE];
    bool eow;
    int num, now;
public:
    Node() {
        for (int i = 0; i < SIZE; i++)
            ptrs[i] = nullptr;
        eow = false;
        num = 0;
        now = -1;
    }
    int empty() {
        return num == 0;
    }
    int count() {
        return num;
    }
    int next() {
        int n = eow ? num - 1 : num;
        if (n) {
            Node* node = nullptr;
            do {
                now = (now + 1) % 26;
                node = ptrs[now];
            } while (!node);
            return now;
        }
        else return -1;
    }
    void getWords(vector<string>& vect, string prev = "") {
        if (eow) vect.push_back(prev);
        int n = eow ? num - 1 : num;
        for (int i = 0; i < n; i++) {
            int elem = next();
            string str = prev + static_cast<char>(elem + 'a');
            ptrs[elem]->getWords(vect, str);
        }
    }
    int printWords(string prev = "") {
        int res = 0;
        if (eow) {
            cout << "word: '" << prev << "'\n";
            res++;
        }
        int n = eow ? num - 1 : num;
        for (int i = 0; i < n; ) {
            //cout << "i: " << i << endl;
            int elem = next();
            char last = char(elem + 'a');
            //cout << "n: " << n << " prev: " << prev << endl;
            int k = ptrs[elem]->printWords(prev+last);
            i += k;
            res += k;
        }
        return res;
    }
    bool findWord(stack<char>& st) {
        bool res = false;
        if (st.empty()) {
            if (eow) res = true;
        }
        else {
            int elem = code(st.top());
            st.pop();
            if (elem != -1 && ptrs[elem]) res = ptrs[elem]->findWord(st);
        }
        return res;
    }
    void reverseWords(Node* other, stack<char> st) {
        if (eow) other->addWord(st);
        int n = eow ? num - 1 : num;
        for (int i = 0; i < n; i++) {
            int elem = next();
            st.push(char(elem + 'a'));
            ptrs[elem]->reverseWords(other, st);
            st.pop();
        }
    }
    void reverseWords(vector<string>& vect, string prev="") {
        if (eow) vect.push_back(prev);
        int n = eow ? num - 1 : num;
        for (int i = 0; i < n; i++) {
            int elem = next();
            char ch = char(elem) + 'a';
            string str(1, ch);
            str += prev;
            ptrs[elem]->reverseWords(vect, str);
        }
    }
    void deleteWord(stack<char>& st) {
        if (st.empty()) {
            eow = false;
            num--;
        }
        else {
            int elem = code(st.top());
            st.pop();
            ptrs[elem]->deleteWord(st);
            if (ptrs[elem]->empty()) {
                Node* temp = ptrs[elem];
                ptrs[elem] = NULL;
                delete temp;
                num--;
            }
        }
    }
    bool addWord(stack<char> st) {
        if (st.empty()) {
            if (!eow) {
                eow = true;
                num++;
                return true;
            }
            else return false;
        }
        int elem = code(st.top());
        st.pop();
        if (elem == -1) return false;
        else {
            if (!ptrs[elem]) ptrs[elem] = new Node;
            bool res = ptrs[elem]->addWord(st);
            if (!res) {
                if (ptrs[elem]->empty()) {
                    Node* temp = ptrs[elem];
                    ptrs[elem] = NULL;
                    delete temp;
                }
            }
            else num++;
            return res;
        }
    }
};

class TrieTree {
    Node* tree;
    string name;
public:
    TrieTree() { }
    TrieTree(string str) {
        name = str;
    }
    Node* root() {
        return tree;
    }
    bool empty() {
        return tree == nullptr;
    }
    void print(string str="") {
        cout << name << " " << str << " " << count() << " words:" << endl;
        if (tree) tree->printWords();
        cout << "!!!\n";
    }
    vector<string> get() {
        vector<string> vect;
        if (tree) tree->getWords(vect);
        return vect;
    }
    void add(const string word) {
        if (!tree) tree = new Node;
        stack<char> st = strToStack(word);
        if (tree->addWord(st)) cout << "\t" << name << ": '" << word << "' is added\n";
    }
    void find(const string word) {
        if (empty()) cout << "Not found! The tree is empty\n";
        else {
            stack<char> st = strToStack(word);
            if (tree->findWord(st)) cout << "\t" << name << " '" << word << "' is found\n";
            else cout << "\t" << name << " '" << word << "' is absent\n";
        }
    }
    void reverse(TrieTree& other) {
        Node* node = new Node;
        stack<char> st;
        tree->reverseWords(node, st);
        other.set(node);
    }
    vector<string> reverse() {
        vector<string> vect;
        if (!empty()) tree->reverseWords(vect);
        return vect;
    }
    void set(Node* node) {
        tree = node;
    }
    void add(vector<string> vect) {
        for (auto x : vect) add(x);
    }
    int count() {
        return empty() ? 0 : tree->count();
    }
    void del(string word) {
        stack<char> st = strToStack(word);
        if (empty() || !tree->findWord(st)) 
            cout << "\t" << name << " '" << word << "' can't be deleted, it's absent\n";
        else {
            tree->deleteWord(st);
            cout << "\t" << name << " '" << word << "' is deleted\n";
        }
    }
};

int main()
{
    TrieTree firstTree("First");
    int num;
    cin >> num;
    string str;
    cin.ignore();
    for (int i = 0; i < num; i++) {
        getline(cin, str);
        firstTree.add(str);
    }
    firstTree.print("");
    firstTree.find("fun");
    firstTree.find("abram");
    TrieTree secondTree("Second");
    firstTree.reverse(secondTree);
    secondTree.print();
    //secondTree.print("second");
    //cout << "\tWord to delete:\n";
    //getline(cin, str);
    //firstTree.del(str);
    //firstTree.print();
    firstTree.add("giantword");
    firstTree.add("gh7191");
    //secondTree.add("name");
    //secondTree.print();
    cout << endl;
    secondTree.add(firstTree.reverse());
    secondTree.print();
}

--------------------------------------------------
--------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

enum VariantType {
	vOUT,
	vEMPTY,
	vBLOCKED,
	vJUMP
};

enum CursorState {
	cHOVER,
	cACTIVE, 
	cCHOICE
};

enum UnitType {
	NO, 
	FIRST,
	SECOND
};

enum UnitState {
	PASSIVE,
	HOVER,
	ACTIVE, 
};

enum Player {
	pFIRST,
	pSECOND
};

enum SquareState {
	sPASSIVE,
	sHOVER,
	sVARIANT,
	sACTIVEVARIANT
};

class Quadrate {
public:
	
	virtual void show() {
		cout << "  " << endl;
	}
};

class Unit : public Quadrate {
	UnitType type;
	UnitState state;
public:
	Unit() {
		type = NO;
		state = PASSIVE;
	}
	void setType(UnitType t) {
		type = t;
	}
	void set(UnitType t, UnitState st) {
		type = t;
		state = st;
	}
	void setState(UnitState st) {
		state = st;
	}
	UnitType getType() {
		return type;
	}
	UnitState getState() {
		return state;
	}
	void show() {
		switch (state) {
		case PASSIVE:
			cout << (int)type << " ";
			break;
		case HOVER:
			cout << (int)type << "!";
			break;
		case ACTIVE:
			cout << (int)type << (int)type;
			break;
		}
	}
	friend class Cursor;
};

class Cursor : public Quadrate {
public:
	int size;
	int x, y;
	int chosenX, chosenY;
	Unit** u;
	Square** s;
	CursorState state;
	Cursor(int k) {
		size = k;
		state = cHOVER;
		chosenX = chosenY = -1;
	}
	void switchState() {
		switch (state) {
		case cHOVER:
			state = cACTIVE;
			break;
		case cACTIVE:
			state = cCHOICE;
			break;
		case cCHOICE:
			state = cHOVER;
			break;
		}
	}
	void putOnCenter() {
		x = size / 2;
		y = size / 2;
		chosenX = chosenY = 0;
	}
	void move() {
		char x;
		x = _getch();
		switch (x) {
		case 'A':
		case 'a':
			if (y - 1 >= 0) y--;
			break;
		case 'D':
		case 'd':
			if (y + 1 <= size) y++;
			break;
		case 'W':
		case 'w':
			if (x - 1 >= 0) x--;
			break;
		case 'S':
		case 's':
			if (x + 1 <= size) x++;
			break;
		case 'G':
		case 'g':
			if (u[x][y].getType() != 0) {
				chosenX = x;
				chosenY = y;
				switchState();
			}
		}
	}
};

class Square {
	int x, y;
	enum Color {
		WHITE,
		BLACK
	} color;
	SquareState state;
public:
	Square() {

	}
	Square(int x, int y) {
		this->x = x;
		this->y = y;
		state = sPASSIVE;
		if ((x + y) % 2 == 0) color = WHITE;
		else color = BLACK;
	}
	Square operator=(Square sq) {
		this->x = sq.x;
		this->y = sq.y;
		this->state = sq.state;
		return sq;
	}
	void show() {
		if (state == sPASSIVE) {
			if (color == WHITE) cout << "w ";
			else cout << "b ";
		}
		else if (state == sHOVER) {
			if (color == WHITE) cout << "w!";
			else cout << "b!";
		}
		else cout << "??";
	}
	void setState(SquareState st) {
		state = st;
	}
};

class Desk {
	const int size;
	Square** kv;
	int hoverX, hoverY;
	int varX, varY;
	int activevarX, activevarY;
public:
	Desk(int k) : size(k) {
		kv = new Square * [size];
		for (int i = 0; i < size; i++) {
			kv[i] = new Square[size];
			for (int j = 0; j < size; j++)
				kv[i][j] = Square(i, j);
		}
	}
	void show(int x, int y) {
		kv[x][y].show();
	}
	void setState(int x, int y, SquareState st) {
		switch (st) {
		case sHOVER:
			hoverX = x;
			hoverY = y;
			break;
		case sVARIANT:
			varX = x;
			varY = y;
			break;
		case sACTIVEVARIANT:
			activevarX = x;
			activevarY = y;
		}
		kv[x][y].setState(st);
	}
	void setState() {
		kv[hoverX][hoverY].setState(sPASSIVE);
		kv[varX][varY].setState(sPASSIVE);
		kv[activevarX][activevarY].setState(sPASSIVE);
	}
	friend class Game;
};

class Checkers {
	const int size;
	Unit** unit;
	int player1, player2;
	int hoverX, hoverY;
	int activeX, activeY;
public: 
	Checkers(int k) : size(k) {
		int numOfUnits = k / 2 * 3;
		player1 = player2 = numOfUnits;
		unit = new Unit * [size];
		for (int i = 0; i < size; i++) {
			unit[i] = new Unit[size];

		}
		for (int i = 0, int first = numOfUnits; (i < size) && first; i++) {
			for (int m = 0; m < size; m++) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(FIRST);
					first--;
				}
			}
		}
		for (int i = size - 1, int second = numOfUnits; (i >= 0) && second; i--) {
			for (int m = size - 1; m >= 0; m--) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(SECOND);
					second--;
				}
			}
		}
		hoverX = hoverY = -10;
	}
	UnitType getType(int x, int y) {
		return unit[x][y].getType();
	}
	void setState(int x, int y, UnitState st) {
		switch (st) {
		case HOVER:
			hoverX = x;
			hoverY = y;
			break;
		case ACTIVE:
			activeX = x;
			activeY = y;
			break;
		}
		unit[x][y].setState(st);
	}
	void setState() {
		unit[hoverX][hoverY].setState(PASSIVE);
		unit[activeX][activeY].setState(PASSIVE);
	}
	void show(int i, int j) {
		unit[i][j].show();
	}
	int getPlayer1State() {
		return player1;
	}
	int getPlayer2State() {
		return player2;
	}
	int decPlayer(UnitType t) {
		if (t == FIRST) player1--;
		else player2--;
	}
	void setType(int a, int b, UnitType t) {
		unit[a][b].setType(t);
	}
	friend class Cursor;
};

struct Variants {
public:
	int now;
	int num;
	int size;
	int x1, x2, y1, y2;
	VariantType firstType, secondType;
	VariantType nowType;
	Variants(int k) {
		x1 = x2 = y1 = y2 = 0;
		num = 0;
		size = k;
		firstType = secondType = vOUT;
		now = 1;
	}
	void checkTypes() {
		bool check1 = ((firstType == vEMPTY) || (firstType == vJUMP));
		bool check2 = ((secondType == vEMPTY) || (secondType == vJUMP));
		if (!check1 && check2) {
			num--;
			x1 = x2;
			y1 = y2;
			firstType = secondType;
		}
		else if (check1 && !check2 && (num == 2)) num--;
		else if (!check1 && !check2 && num) num = 0;
	}
	void set(int a, int b, int c, int d) {
		if ((a >= 0) && (b >= 0) && (a < size) && (b < size)) {
			num++;
			x1 = a;
			y1 = b;
		}
		if ((c >= 0) && (d >= 0) && (c < size) && (d < size)) {
			if (num) {
				num++;
				x2 = c;
				y2 = d;
			}
			else {
				x1 = c;
				y1 = d;
			}
		}
	}
};

class Game {
	const int size;
	Desk desk;
	Checkers chek;
	Cursor curs;
	Variants variants;
	void setState(int var = 0) {
		int a = curs.x, b = curs.y;
		if (var == 0) {
			chek.setState();
			desk.setState();
		}
		else if (var == 1) {
			if (chek.getType(a, b) != NO) chek.setState(a, b, HOVER);
			else desk.setState(a, b, sHOVER);
		}
		else if (var == 2) {
			chek.setState(curs.chosenX, curs.chosenY, ACTIVE);
			desk.setState(desk.activevarX, desk.activevarY, sVARIANT);
			desk.setState(curs.x, curs.y, sACTIVEVARIANT);
		}
	}
public:
	Game(int k) : size(k), desk(k), chek(k), curs(k), variants(k) {
		
	}
	void show() {
		for (int i = 0; i < size; i++) {
			for (int m = 0; m < size; m++) {
				if (chek.getType(i, m) != NO) chek.show(i, m);
				else desk.show(i,m);
			}
			cout << endl;
		}
	}
	void switchCursor() {
		char x = _getch();
		if (variants.num == 2 && ((x == 'a') || (x == 'A') || (x == 'd') || (x == 'D'))) {
			if (variants.y1 == curs.y) {
				variants.now = variants.secondType;
				curs.x = variants.x2;
				curs.y = variants.y2;
			}
			else {
				variants.now = variants.firstType;
				curs.x = variants.x1;
				curs.y = variants.y1;
			}
		}
		else if ((x == 'g') || (x == 'G')) {
			curs.state = cCHOICE;
		}
	}
	void variantsType(Player p, int a, int b) {
		if (variants.num == 2) {
			UnitType one, two;
			UnitType t = (p == pFIRST) ? FIRST : SECOND;
			one = chek.getType(variants.x1, variants.y1);
			two = chek.getType(variants.x2, variants.y2);

			if (one == NO) variants.firstType = vEMPTY;
			else if (one == t) variants.firstType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x1 + 1;
				else tempX = variants.x1 - 1;
				tempY = variants.y1 - 1;
				variants.x1 = tempX;
				variants.y1 = tempY;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.firstType = vJUMP;
					else variants.firstType = vBLOCKED;
				}
				else variants.firstType = vOUT;
			}

			if (two == NO) variants.secondType = vEMPTY;
			else if (two == t) variants.secondType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x2 + 1;
				else tempX = variants.x2 - 1;
				tempY = variants.y2 + 1;
				variants.x2 = tempX;
				variants.y2 = tempY;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.secondType = vJUMP;
					else variants.secondType = vBLOCKED;
				}
				else variants.secondType = vOUT;
			}
		}
		else if (variants.num == 1) {
			UnitType temp;
			UnitType t = (p == pFIRST) ? FIRST : SECOND;
			temp = chek.getType(variants.x1, variants.y1);

			if (temp == NO) variants.firstType = vEMPTY;
			else if (temp == t) variants.firstType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x1 + 1;
				else tempX = variants.x1 - 1;
				if (variants.y1 > b) tempY = variants.y1 + 1;
				else tempY = variants.y1 - 1;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.firstType = vJUMP;
					else variants.firstType = vBLOCKED;
				}
			}
		}
	}
	void play() {
		bool goon = true;
		Player pl = pFIRST;
		bool marked = false;
		bool checked = false;
		int tempX, tempY;
		while (goon) {
			if (curs.state == cHOVER) {
				setState(1);
			}
			else if (curs.state == cACTIVE) {
				int a = curs.x, b = curs.y;
				UnitType type = chek.getType(a, b);
				if (!checked && (type == NO) || ((pl == pFIRST) && (type == SECOND)) || ((pl == pSECOND) && (type == FIRST))) {
					curs.state = cHOVER;
					setState(1);
				}
				else {
					checked = true;
					if (!marked) {
						int x1, x2, y1, y2;
						if (pl == pFIRST) x2 = x1 = a - 1;
						else x2 = x1 = a + 1;
						y1 = b - 1;
						y2 = b + 1;
						variants.set(x1, y1, x2, y2);
						variantsType(pl, a, b);
						variants.checkTypes();
						curs.chosenX = a;
						curs.chosenY = b;
					}
					
					if (!variants.num) {
						curs.state = cHOVER;
						setState(1);
						marked = checked = false;
					}
					else {
						if (!marked) {
							curs.x = variants.x1;
							curs.y = variants.y1;
							variants.now = variants.firstType;
						}
						
						marked = true;
						setState(2);
					}
				}
			}
			else {	
				if (variants.now == vJUMP) {
					int enemyX, enemyY;

					if (pl == pFIRST) {
						enemyX = curs.x - 1;
						chek.decPlayer(SECOND);
					}
					else {
						enemyX = curs.x + 1;
						chek.decPlayer(FIRST);
					}

					if (curs.y > curs.chosenY) enemyY = curs.y - 1;
					else enemyY = curs.y + 1;

					chek.setType(enemyX, enemyY, NO);
				}

				if (pl == pFIRST) chek.setType(curs.x, curs.y, FIRST);
				else chek.setType(curs.x, curs.y, SECOND);
				chek.setType(curs.chosenX, curs.chosenY, NO);

				curs.state = cHOVER;
				setState(1);
				if (pl == pFIRST) pl = pSECOND;
				else pl = pFIRST;
			}
			show();
			if (curs.state == cACTIVE) switchCursor();
			else curs.move();
			setState();
			goon = chek.getPlayer1State() && chek.getPlayer2State();
		}
	}
};

auto main() -> int {
	Game shashki(8);
	shashki.play();
}

--------------------------------------------------
--------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <conio.h>

using namespace std;

enum State {
	START,
	DOT,
	CUT,
	USED,
	FINISH
};

struct Field {
	int size;
	char* start;
	int startX, startY;
	int finishX, finishY;
	char** kv;
	State** state;
	Field(ifstream& in) {
		in >> size;
		start = NULL;
		kv = new char* [size];
		state = new State * [size];
		string str;
		for (int i = 0; i < size; i++) {
			kv[i] = new char[size];
			state[i] = new State[size];
			in >> str;
			cout << str << endl;
			for (int k = 0; k < size; k++) {
				char x = str[k];
				kv[i][k] = x;
				state[i][k] = DOT;
				if (x == '@') {
					if (!start) {
						start = &kv[i][k];
						state[i][k] = START;
						startX = i;
						startY = k;
					}
					else {
						state[i][k] = FINISH;
						finishX = i;
						finishY = k;
					}
				}
				else if (x == '#') state[i][k] = CUT;
			}
		}
	}
	int getFinishX() {
		return finishX;
	}
	int getFinishY() {
		return finishY;
	}
	int getStartY() {
		return startY;
	}
	int getStartX() {
		return startX;
	}
	bool check(int x, int y) {
		if ((x >= 0) && (y >= 0) && (x < size) && (y < size)) {
			if ((state[x][y] == DOT) || (state[x][y] == FINISH)) return true;
		}
		return false;
	}
	void showFull() {
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++)
				cout << kv[i][k];
			cout << endl;
		}
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++)
				cout << state[i][k];
			cout << endl;
		}
	}
	void show(vector<pair<int, int>>& arr) {
		for (int i = 0; i < arr.size(); i++)
			state[arr[i].first][arr[i].second] = USED;
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++) {
				if (state[i][k] == USED) cout << "@";
				else cout << kv[i][k];
			}
			cout << endl;
		}
	}
};

struct Result {
	vector<pair<int, int>> bufseq;
	vector<pair<int, int>> seq;
	int len;
	int buflen;
	bool success;
	Result() {
		len = 100000;
		buflen = 0;
		success = false;
	}
	Result operator=(Result other) {
		len = other.len;
		success = other.success;
		seq = other.seq;
		return other;
	}
	void update() {
		if (buflen < len) {
			success = true;
			seq.clear();
			for (int i = 0; i < buflen; i++)
				seq.push_back(bufseq[i]);
			len = buflen;
		}
	}
};

void dfs(Field& f, int x, int y, Result& outres, bool& goon) {
	//cout << "dfs " << x << " " << y << endl;
	static Result res;
	static int fx = f.getFinishX();
	static int fy = f.getFinishY();
	if (!goon) return;
	if (res.buflen >= res.len - 1) return;
	if (res.buflen < res.bufseq.size()) res.bufseq[res.buflen] = make_pair(x, y);
	else res.bufseq.push_back(make_pair(x, y));
	res.buflen++;
	if (f.state[x][y] == FINISH) {
		if (res.buflen < res.len) {
			cout << "dfs " << res.buflen << " " << res.len << endl;
			res.update();
			outres = res;
		}
	}
	else {
		bool isdot = (f.state[x][y] != START) && (f.state[x][y] != FINISH);
		if (isdot) f.state[x][y] = USED;

		if (fx >= x) {
			if (fy >= y) {
				if (f.check(x + 2, y + 1)) dfs(f, x + 2, y + 1, outres, goon);
				if (f.check(x + 1, y + 2)) dfs(f, x + 1, y + 2, outres, goon);
				if (f.check(x - 1, y + 2)) dfs(f, x - 1, y + 2, outres, goon);
				if (f.check(x + 2, y - 1)) dfs(f, x + 2, y - 1, outres, goon);
				if (f.check(x + 1, y - 2)) dfs(f, x + 1, y - 2, outres, goon);
				if (f.check(x - 2, y + 1)) dfs(f, x - 2, y + 1, outres, goon);
				if (f.check(x - 2, y - 1)) dfs(f, x - 2, y - 1, outres, goon);
				if (f.check(x - 1, y - 2)) dfs(f, x - 1, y - 2, outres, goon);
			}
			else {
				if (f.check(x + 2, y - 1)) dfs(f, x + 2, y - 1, outres, goon);
				if (f.check(x + 1, y - 2)) dfs(f, x + 1, y - 2, outres, goon);
				if (f.check(x - 1, y - 2)) dfs(f, x - 1, y - 2, outres, goon);
				if (f.check(x + 2, y + 1)) dfs(f, x + 2, y + 1, outres, goon);
				if (f.check(x + 1, y + 2)) dfs(f, x + 1, y + 2, outres, goon);
				if (f.check(x - 2, y - 1)) dfs(f, x - 2, y - 1, outres, goon);
				if (f.check(x - 2, y + 1)) dfs(f, x - 2, y + 1, outres, goon);
				if (f.check(x - 1, y + 2)) dfs(f, x - 1, y + 2, outres, goon);
			}
		}
		else {
			if (fy >= y) {
				if (f.check(x - 2, y + 1)) dfs(f, x - 2, y + 1, outres, goon);
				if (f.check(x - 1, y + 2)) dfs(f, x - 1, y + 2, outres, goon);
				if (f.check(x - 2, y - 1)) dfs(f, x - 2, y - 1, outres, goon);
				if (f.check(x + 1, y + 2)) dfs(f, x + 1, y + 2, outres, goon);
				if (f.check(x - 1, y - 2)) dfs(f, x - 1, y - 2, outres, goon);
				if (f.check(x + 2, y + 1)) dfs(f, x + 2, y + 1, outres, goon);
				if (f.check(x + 2, y - 1)) dfs(f, x + 2, y - 1, outres, goon);
				if (f.check(x + 1, y - 2)) dfs(f, x + 1, y - 2, outres, goon);
			}
			else {
				if (f.check(x - 2, y - 1)) dfs(f, x - 2, y - 1, outres, goon);
				if (f.check(x - 1, y - 2)) dfs(f, x - 1, y - 2, outres, goon);
				if (f.check(x - 2, y + 1)) dfs(f, x - 2, y + 1, outres, goon);
				if (f.check(x + 1, y - 2)) dfs(f, x + 1, y - 2, outres, goon);
				if (f.check(x + 2, y - 1)) dfs(f, x + 2, y - 1, outres, goon);
				if (f.check(x - 1, y + 2)) dfs(f, x - 1, y + 2, outres, goon);
				if (f.check(x + 2, y + 1)) dfs(f, x + 2, y + 1, outres, goon);
				if (f.check(x + 1, y + 2)) dfs(f, x + 1, y + 2, outres, goon);
			}
		}
		
		if (isdot) f.state[x][y] = DOT;
	}
	res.buflen--;
}

void skip(bool& goon) {
	while (goon) {
		if (_getch()) goon = false;
	}
}

int main() {
	cout << "Enter the name of the file: ";
	string name;
	cin >> name;
	ifstream file(name.c_str());
	if (!file.is_open()) cout << "File is not opened!";
	Field field(file);
	file.close();
	int x = field.getStartX();
	int y = field.getStartY();
	Result res;
	bool goon = true;
	thread th(skip, ref(goon));
	dfs(field, x, y, res, goon);
	th.detach();
	cout << "dfs worked" << endl;
	if (res.success == true) {
		cout << "result:\n";
		field.show(res.seq);
		cout << "length: " << res.len - 1;
	}
	else cout << "\nimpossible";
}

-------------------------------------------------------
-------------------------------------------------------

// Шимкин Р.Р. ПММ62 2 курс Лаба №2
/*
задание: реализовать программу, позволяющую пользователю отбивать бластерные выстрелы управляемым мышью и\или клавиатурой световым мечом; 
количество снарядов растёт с течением времени, после поражения требуется вывести на отдельной форме статистику: количество отражённых 
снарядов, количество пропущенных (критическое количество настраивается пользователем в начале), общее время игры
*/

#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <string>

using namespace std;

// макрос для вывода имени и значения переменной
#define watch(x) cout << (#x) << ": " << (x).toString() << endl 

// структура выстрела, содержит поля угла по вертикали, по горизонтали и скорость полета 
// а также конструктор от трех целых
struct Shot { 
    int vert;
    int hor;
    int sp;
    Shot(int x, int y, int s) {
        hor = x;
        vert = y;
        sp = s;
    }
};

// класс бластера с 2 полями: скорость и количество совершенных выстрелов
class Blaster {
    int speed;
    int count;

    // функция ускорения выстрелов до определенного значения
    void changeSpeed() {
        if (speed > 2000) speed -= speed * 0.1;
    }
public:
    // сеттер скорости
    void setSpeed(int speed) {
        this->speed = speed;
    }
    // сеттер счёта
    void setCount(int count) {
        this->count = count;
    }
    // геттер скорости
    int getSpeed(int speed) {
        return this->speed;
    }
    // геттер счёта
    int getCount(int speed) {
        return this->speed;
    }
    // конструктор от значения скорости
    Blaster(int speed) {
        this->speed = speed;
        count = 0;
    }
    // констуктор от значений скорости и счёта
    Blaster(int speed, int count) {
        this->speed = speed;
        this->count = count;
    }
    // конструктор по умолчанию (скорость 5000, счёт 0)
    Blaster() {
        speed = 5000;
        count = 0;
    }

    // функция выстрела
    Shot shoot() {
        srand(time(0));
        // х - рандомная переменная,
        // отвечающая за вертикальную полусферу выстрела
        int x = rand() % 2;
        int v; // v - угол по вертикали
        if (x == 0) v = rand() % 180 + 180;
        else v = rand() % 180;
        int h = rand() % 180 - 90; // угол по горизонтали
        Shot blast(v, h, speed); // создание структуры выстрела
        count++;
        // ускорение каждые два выстрела
        if (count % 2 == 0) changeSpeed();
        return blast;
    }
    // метод приведения к формату строки
    string toString() {
        return " speed: " + to_string(speed) + " count: " + to_string(count);
    }
};

// функция проверки ввода для использования в потоке
void checkInput(char& c) {
    c = _getch();
}

// класс игрока
class Gamer {
    int health;
    int speed;
    int width;
    int height;
    int h;
    int v;
    int score;
    int missed;
    // движение вправо на величину = скорости реакции
    void moveRight() {
        h = (h + speed) % 360;
    }
    // движение влево на величину = скорости реакции
    void moveLeft() {
        int temph = h - speed;
        h = (temph < 0) ? (360 + temph) : temph;
    }
    // движение вверх на величину = скорости реакции
    void moveUp() {
        int tempv = v + speed;
        v = (tempv > 90) ? 90 : tempv;
    }
    // движение вниз на величину = скорости реакции
    void moveDown() {
        int tempv = v - speed;
        v = (tempv < -90) ? -90 : tempv;
    }

    // метод изменения направления
    void Move(char c) {
        switch (c) {
        case 'D':
        case 'd': {
            moveRight();
            break;
        }

        case 'A':
        case 'a': {
            moveLeft();
            break;
        }

        case 'W':
        case 'w': {
            moveUp();
            break;
        }

        case 'S':
        case 's': {
            moveDown();
            break;
        }
        }
    }
public:
    // конструктор по умолчанию
    Gamer() {
        health = 3;
        speed = 20;
        width = 20;
        height = 40;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // конструктор от здоровья, скорости реакции, ширины и высоты меча
    Gamer(int health, int speed, int width, int height) {
        this->health = health;
        this->speed = speed;
        this->width = width;
        this->height = height;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // конструктор от здоровья
    Gamer(int health) {
        this->health = health;
        speed = 20;
        width = 20;
        height = 40;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // сеттер направления
    void setVH(int h, int v) {
        this->h = h;
        this->v = v;
    }
    // сеттер счёта по умолчанию
    void setScore() {
        score = 0;
        missed = 0;
    }
    // геттер счёта
    int getScore() {
        return score;
    }
    // сеттер счёта 
    void setScore(int score) {
        this->score = score;
    }
    // геттер количества пропущенных
    int getMissed() {
        return missed;
    }
    // сеттер здоровья
    void setHealth(int value) {
        health = value;
    }
    // геттер здоровья
    int getHealth() {
        return health;
    }
    // сеттер размеров меча в градусах
    void setSize(int height, int width) {
        this->height = height;
        this->width = width;
    }
    // геттер высоты меча в градусах
    int getHeight() {
        return height;
    }
    // геттер ширины меча в градусах
    int getWidth() {
        return width;
    }
    
    // проверка, жив ли игрок
    bool isAlive() {
        return health > 0;
    }
    
    // метод приведения к строке класса игрока
    string toString() {
        string str;
        string shealth = " health: " + to_string(health);
        string sheight = " height: " + to_string(height);
        string swidth = " height: " + to_string(width);
        string sh = " height: " + to_string(h);
        string sv = " height: " + to_string(v);
        string sscore = " height: " + to_string(score);
        string smissed = " height: " + to_string(missed);
        str = shealth + sheight + swidth + sh + sv + sscore + smissed;
        return str;
    }
    // метод отражения выстрела
    void reject(Shot shot) {
        // начальные направления по горизонтали и вертикали
        h = 0, v = 0;
        clock_t begin = clock();
        clock_t now = clock();
        char ch;

        // пока текущее время меньше времени полета выстрела
        while ( (now - begin) < shot.sp ) {
            system("cls");
            // вывод информации о мече и возможностях игрока
            cout << "Your speed: " << speed << "    Sword size: " << width << " " << height << endl;
            now = clock();

            // вывод направления от текущего положения игрока до направления выстрела
            cout << "Shot: " << shot.hor - h << " " << shot.vert - v << " " << shot.sp - (now - begin) << endl;

            // ожидание пользовательского ввода в отдельном потоке в течение 2мс
            thread input(checkInput, ref(ch));
            Sleep(2);
            
            // прерывание потока
            input.detach();

            // изменение положения игрока
            Move(ch);
            ch = 'M';
        }
        system("cls");

        // проверка, отбил ли меч выстрел с учетом размеров меча
        if ((abs(shot.hor - h) < width) && ((abs(shot.vert - v)) < height)) {
            cout << "You've rejected it!";
            score++;
        }
        else {
            cout << "You're wounded!";
            health--;
            missed++;
        };

        // уменьшающаяся с ростом счёта пауза для отдыха
        Sleep(1000 - 40 * score);
    }

    // метод вывода статистики
    void Stat() {
        cout << " Your score is: " << score << endl;
        cout << " You missed: " << missed << endl;
    }
};


// основная функция игры
void play(Gamer& pl, Blaster& bl) {
        system("cls");
        cout << " #Press 2 to settle the health or any other button to skip";
        char ch = _getch();
        if (ch == '2') {
            cout << "\n #Enter new health value: ";
            int health;
            cin >> health;
            pl.setHealth(health);
        }

        // обнуление счёта
        pl.setScore();

        clock_t begin = clock();

        // пока игрок жив, отбивать новые выстрелы
        while (pl.isAlive()) {
            pl.reject(bl.shoot());
        }
        clock_t end = clock();
        system("cls");
        cout << "Player is dead..." << endl;

        // печать послеигровой статистики
        pl.Stat();
        cout << " time: " << (end - begin)/1000 << endl;
}

// функция для демонстрации работы геттеров, сеттеров и конструкторов
void demonstration() {
    Gamer player1;
    cout << "#1" << endl;
    watch(player1);
    cout << endl;

    Gamer player2(500, 50, 3, 3);
    cout << "#2" << endl;
    watch(player2);
    cout << endl;

    player1.setHealth(20);
    player1.setSize(1,100);
    
    cout << "#3" << endl;
    watch(player1);
    cout << endl;

    cout << "#4" << endl;
    player1.setVH(50, 100);
    watch(player1);
    cout << endl;

    Blaster blas1;
    cout << "#5" << endl;
    watch(blas1);
    cout << endl;

    cout << "#6" << endl;
    Blaster blas2(2000);
    watch(blas2);
    cout << endl;
}

int main()
{
    Gamer player;
    Blaster gun;

    //demonstration();

    play(player, gun);

    system("pause");
}

------------------------------------------------------
------------------------------------------------------

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

--------------------------------------------------------------
--------------------------------------------------------------

// Lab3Hash.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

const int SIZE = 1000;

class Name {
private:
    unsigned int count;
    string name;
public:
    Name() {
        count = 0;
    }
    void plus() {
        count++;
    }
    void setName(const string& str) {
        name = str;
    }
    unsigned int getCount() {
        return count;
    }
    string& getName() {
        return name;
    }
};

class HashTable {
private:
    vector<vector<Name>> names;
    bool check(string& str) {
        for (int i = 0; i < str.length()-1; i++) {
            bool a = str[i] >= 'a' && str[i] <= 'z';
            bool b = str[i] >= 'A' && str[i] <= 'Z';
            if (!a && !b) return false;
        }
        checkLast(str);
        return true;
    }
    void checkLast(string& str) {
        char s = str[str.length() - 1];
        if (s == '.' || s == ',' || s == '!' || s == '?' || s == ':' || s == ';') {
            string newstr = str.substr(0, str.length() - 1);
            str = newstr;
        }
        //cout << "\nstr: " << str;
    }
    unsigned int HF(string& str) {
        const int p = 31;
        long hash = 0, mul = 1;
        if (check(str)) {
            for (int i = 0; i < str.length(); i++) {
                hash += (str[i] - 'A' + 1) * mul;
                mul *= p;
            }
        }
        cout << "\nHF(" << str << "): " << hash % SIZE;
        return (hash % SIZE);
    }
public:
    HashTable() {
        names.clear();
        names.resize(SIZE);
    }
    bool push(Name& name) {
        unsigned int h = HF(name.getName());
        for (unsigned int i = 0; i < names[h].size(); i++) {
            if (names[h][i].getName() == name.getName()) return true;
        }
        names[h].push_back(name);
        return names[h].size() > 1;
    }
    void findName(string& word) {
        unsigned int h = HF(word);
        for (unsigned int i = 0; i < names[h].size(); i++) {
            if (names[h][i].getName() == word) names[h][i].plus();
        }
    }
    void outputFile(string namesFile) {
        string outputName;
        cout << "Enter the name of output file: ";
        cin >> outputName;
        cout << endl;
        ofstream fout(outputName);
        ifstream fnames(namesFile);
        string tmp;
        if (fnames.is_open()) {
            while (!fnames.eof()) {
                fnames >> tmp;
                unsigned int h = HF(tmp);
                for (unsigned int i = 0; i < names[h].size(); i++) {
                    if (names[h][i].getName() == tmp) fout << "Name: " << names[h][i].getName() << " Count: " << names[h][i].getCount() << endl;
                }
            }
        }
        else cout << "File error!\n";
    }
    void print() {
        for (unsigned int i = 0; i < names.size(); i++) {
            for (unsigned int k = 0; k < names[i].size(); k++) {
                cout << "\nName: " << names[i][k].getName() << " count: " << names[i][k].getCount();
            }
        }
    }
};

bool addName(HashTable* table, string fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        string tmp;
        while (!file.eof()) {
            file >> tmp;
            cout << "\ntmp: " << tmp;
            Name newName;
            newName.setName(tmp);
            table->push(newName);
        }
        file.close();
        return true;
    }
    else return false;
}

void search(HashTable* table, string fileName) {
    ifstream text(fileName);
    if (text.is_open()) {
        string word;
        while (!text.eof()) {
            text >> word;
            cout << "\nword: " << word;
            table->findName(word);
        }
    }
    else cout << "File error!\n";
}

int main() {
    HashTable table;
    HashTable* p = &table;
    string fnamesstr, ftextstr;
    cout << "Enter the name of the names file: ";
    cin >> fnamesstr;
    cout << "\nEnter the name of the text file: ";
    cin >> ftextstr;
    if (addName(p, fnamesstr)) {
        cout << "table created\n";
        search(p, ftextstr);
        p->print();
    }
    else cout << "Error\n";
    return 0;
}

------------------------------------------------------
------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>

using namespace std;

enum State {
	START,
	DOT,
	CUT,
	USED,
	FINISH
};

struct Field {
	int size;
	char* start;
	int startX, startY;
	char** kv;
	State** state;
	Field(ifstream& in) {
		in >> size;
		start = NULL;
		kv = new char* [size];
		state = new State * [size];
		string str;
		for (int i = 0; i < size; i++) {
			kv[i] = new char[size];
			state[i] = new State[size];
			in >> str;
			cout << str << endl;
			for (int k = 0; k < size; k++) {
				char x = str[k];
				kv[i][k] = x;
				state[i][k] = DOT;
				if (x == '@') {
					if (!start) {
						start = &kv[i][k];
						state[i][k] = START;
						startX = i;
						startY = k;
					}
					else state[i][k] = FINISH;
				}
				else if (x == '#') state[i][k] = CUT;
			}
		}
	}
	int getStartY() {
		return startY;
	}
	int getStartX() {
		return startX;
	}
	void refcheck(int x, int y, bool& res) {
		res = false;
		if ((x >= 0) && (y >= 0) && (x < size) && (y < size)) {
			if ((state[x][y] == DOT) || (state[x][y] == FINISH)) res = true;
		}
	}
	bool check(int x, int y) {
		if ((x >= 0) && (y >= 0) && (x < size) && (y < size)) {
			if ((state[x][y] == DOT) || (state[x][y] == FINISH)) return true;
		}
		return false;
	}
	void showFull() {
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++)
				cout << kv[i][k];
			cout << endl;
		}
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++)
				cout << state[i][k];
			cout << endl;
		}
	}
	void show(vector<pair<int, int>>& arr) {
		for (int i = 0; i < arr.size(); i++)
			state[arr[i].first][arr[i].second] = USED;
		for (int i = 0; i < size; i++) {
			for (int k = 0; k < size; k++) {
				if (state[i][k] == USED) cout << "@";
				else cout << kv[i][k];
			}
			cout << endl;
		}
	}
};

struct Result {
	vector<pair<int, int>> bufseq;
	vector<pair<int, int>> seq;
	int len;
	int buflen;
	bool success;
	Result() {
		len = 100000;
		buflen = 0;
		success = false;
	}
	Result operator=(Result other) {
		len = other.len;
		success = other.success;
		seq = other.seq;
		return other;
	}
	void update() {
		if (buflen < len) {
			success = true;
			seq.clear();
			for (int i = 0; i < buflen; i++)
				seq.push_back(bufseq[i]);
			len = buflen;
		}
	}
};

void dfs(Field& f, int x, int y, Result& outres) {
	//cout << "dfs " << x << " " << y << endl;
	static Result res;
	if (res.buflen >= res.len) return;
	if (res.buflen < res.bufseq.size()) res.bufseq[res.buflen] = make_pair(x, y);
	else res.bufseq.push_back(make_pair(x, y));
	res.buflen++;
	if (f.state[x][y] == FINISH) {
		if (res.buflen < res.len) {
			res.update();
			outres = res;
			cout << "dfs " << x << " " << y << endl;
		}
	}
	else {
		bool isdot = (f.state[x][y] != START) && (f.state[x][y] != FINISH);
		if (isdot) f.state[x][y] = USED;
		vector<pair<int, int>> variant;
		bool b1, b2, b3, b4, b5, b6, b7, b8;
		thread t1([&f](int a, int b, int c, int d, bool& first, bool& second) {
			f.refcheck(a, b, first);
			f.refcheck(c, d, second);
			}, x - 2, y - 1, x - 2, y + 1, ref(b1), ref(b2) );
		thread t2([&f](int a, int b, int c, int d, bool& first, bool& second) {
			f.refcheck(a, b, first);
			f.refcheck(c, d, second);
			}, x - 1, y - 2, x - 1, y + 2, ref(b3), ref(b4));
		thread t3([&f](int a, int b, int c, int d, bool& first, bool& second) {
			f.refcheck(a, b, first);
			f.refcheck(c, d, second);
			}, x + 1, y - 2, x + 1, y + 2, ref(b5), ref(b6));
		thread t4([&f](int a, int b, int c, int d, bool& first, bool& second) {
			f.refcheck(a, b, first);
			f.refcheck(c, d, second);
			}, x + 2, y - 1, x + 2, y + 1, ref(b7), ref(b8));
		t1.join();
		t2.join();
		t3.join();
		t4.join();
		if (b1) variant.push_back(make_pair(x - 2, y - 1));
		if (b2) variant.push_back(make_pair(x - 2, y + 1));
		if (b3) variant.push_back(make_pair(x - 1, y - 2));
		if (b4) variant.push_back(make_pair(x - 1, y + 2));
		if (b5) variant.push_back(make_pair(x + 1, y - 2));
		if (b6) variant.push_back(make_pair(x + 1, y + 2));
		if (b7) variant.push_back(make_pair(x + 2, y - 1));
		if (b8) variant.push_back(make_pair(x + 2, y + 1));
		for (int i = 0; i < variant.size(); i++)
			dfs(f, variant[i].first, variant[i].second, outres);
		if (isdot) f.state[x][y] = DOT;
	}
	res.buflen--;
}

int main() {
	cout << "Enter the name of the file: ";
	string name;
	cin >> name;
	ifstream file(name.c_str());
	if (!file.is_open()) cout << "File is not opened!";
	Field field(file);
	file.close();
	int x = field.getStartX();
	int y = field.getStartY();
	Result res;
	dfs(field, x, y, res);
	cout << "dfs worked" << endl;
	if (res.success == true) {
		cout << "result:\n";
		field.show(res.seq);
		cout << "length: " << res.len - 1;
	}
	else cout << "\nimpossible";
	//field.showFull();
}

---------------------------------------------------
---------------------------------------------------

#pragma once
#include<string>
#include<iostream>

using namespace std;

struct NODE {
	NODE(int, int, string, int);
	int sup_account_number;
	int cl_account_number;
	string purpose;
	int sum;
	NODE* next;
	NODE* prev;
};

typedef NODE* Tlist;

class Payments {
	int count;
	Tlist head;
	Tlist tail;
public:
	void Menu();
	Payments();
	~Payments();
	void searchPurpose(string);
	void sortSum();
	void printSupplier(int);
	void addToHead(int, int, string, int);
	void addToTail(int, int, string, int);
	void del(Tlist&);
	void delFromHead();
	void delFromTail();
	void clearList();
	void printList();
	void swapNodes(Tlist&, Tlist&);
	void changeNode(Tlist&);
	void readFromFile(ifstream&);
	void writeToFile(ofstream&);
	Tlist& getElemPtr(int);
	//int Count();
private:
	void printNode(Tlist&);
	void insertAfter(Tlist&, Tlist&, bool);
	bool Empty();
};

--------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include "List2.h"
#include <windows.h>
#include <assert.h>
#include <math.h>

using namespace std;

NODE::NODE(int san, int can, string pur, int s) {
	sup_account_number = abs(san);
	cl_account_number = abs(can);
	purpose = pur;
	sum = abs(s);
}

void Payments::Menu() {
	string message[] = {
	" #Enter 0 to exit",
	" #Enter 1 for input from file",
	" #Enter 2 to manual input",
	" #Enter 3 to delete elements",
	" #Enter 4 to print the list",
	" #Enter 5 to count elements",
	" #Enter 6 to search payments by purpose",
	" #Enter 7 to print payments by supplier",
	" #Enter 8 to sort the list (by sum)",
	" #Enter 9 to change elements",
	" #Enter 10 to write to file",
	" #Enter 11 to clear the list",
	" #Enter -1 to clear the console",
	};

	int choice;
	bool goon = true;
	for (auto& str : message)
		cout << str << endl;

	while (goon) {
		cout << "----------------------------------------------------------";
		cout << "   Your choice: ";
		cin >> choice;

		switch (choice) {
		case -1: {
			system("cls");
			for (auto& str : message)
				cout << str << endl;
			break;
		}

		case 0: {
			goon = false;
			break;
		}

		case 1: {
			cout << "Enter the file name: ";
			string name;
			cin >> name;

			ifstream input(name.c_str());
			if (input.is_open()) {
				readFromFile(input);
				cout << "The file is read" << endl;
				input.close();
			}
			else cout << "The file is not opened" << endl;

			break;
		}

		case 2: {
			cout << "Enter 1 for input to head or 2 for input to tail: ";
			int pos;
			cin >> pos;

			cout << "Enter the number of elements to add: ";
			int num;
			cin >> num;

			for (int i = 0; i < num; i++) {
				int cl;
				int sup;
				string purpose;
				int sum;
				cout << "Enter supplier, client, purpose and sum: ";
				cin >> sup >> cl >> purpose >> sum;
				if (pos == 1) addToHead(sup, cl, purpose, sum);
				else if (pos == 2) addToTail(sup, cl, purpose, sum);
			}
			break;
		}

		case 3: {
			cout << "Enter 1 to delete from head, 2 to delete from tail ";
			cout << "or 3 to delete from particular position: ";
			int place;
			cin >> place;
			while ((place > 3) || (place < 1)) {
				cout << "Wrong input!";
				cin >> place;
			};

			cout << "Enter the number of elements to delete: ";
			int num;
			cin >> num;

			if (place == 3) {
				for (int i = 0; i < num; i++) {
					cout << "Enter the position of the element: ";
					int pos;
					cin >> pos;
					if (pos == 1) delFromHead();
					else if (pos == count) delFromTail();
					else if (pos < count) {
						Tlist p = getElemPtr(pos);
						del(p);
					}
					else cout << "Wrong position! Only " << count << " elements in the list" << endl;
				}
			}
			else {
				for (int i = 0; i < num; i++) {
					if (place == 1) delFromHead();
					else if (place == 2) delFromTail();
				}
			}
			break;
		}

		case 4: {
			printList();
			break;
		}

		case 5: {
			cout << count << " elements in the list" << endl;
			break;
		}

		case 6: {
			cout << "Enter the puspose: ";
			string pur;
			cin >> pur;
			searchPurpose(pur);
			break;
		}

		case 7: {
			cout << "Enter supplier: ";
			int sup;
			cin >> sup;
			printSupplier(sup);
			break;
		}

		case 8: {
			sortSum();
			break;
		}

		case 9: {
			cout << "Enter the position of element to change: ";
			int pos;
			cin >> pos;
			while ((pos < 1) || (pos > count)) {
				cout << "Wrong position!";
				cin >> pos;
			}
			
			Tlist p = getElemPtr(pos);
			changeNode(p);
			break;
		}

		case 10: {
			cout << "Enter the name of the file: ";
			string name; 
			cin >> name;
			ofstream output(name.c_str());
			if (output.is_open()) {
				writeToFile(output);
				cout << "The file is written" << endl;
				output.close();
			}
			else cout << "Error! The file is not written";
				
			break;
		}

		case 11: {
			clearList();
			break;
		}
		}
		cout << "----------------------------------------------------------" << endl;
	}

}

/*int Payments::Count() {
	return count;
}*/

Tlist& Payments::getElemPtr(int pos) {
	Tlist p = head;
	for (int i = 1; i < pos; i++)
		p = p->next;
	return p;
}

void Payments::del(Tlist& p) {
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = NULL;
	p->prev = NULL;
	//printNode(p);
	//cout << " deleted" << endl;
	delete p;
	count--;
}

void Payments::sortSum() {
	Tlist p = head;
	if (p) p = p->next; // голова уже отсортирована

	while (p != NULL) {
		Tlist it;
		if (p->prev->sum <= p->sum) {     // проверка, потребуется ли вообще сортировка для элемента p
			p = p->next;
			continue;
		}
		else it = p->prev->prev;

		bool istail = (tail == p) ? true : false;

		while (it != NULL) { 
			if (it->sum > p->sum) it = it->prev;  // в цикле ищем слева узел меньше, чем p
			else {
				if (istail) tail = p->prev; 
				Tlist next = p->next;
				insertAfter(it, p, istail); // вставляем узел p после узла it
				p = next;
				break;
			}
		}
		if (it == NULL) {
			if (istail) tail = p->prev;
			Tlist next = p->next;
			p->prev->next = next;
			if (next) p->next->prev = p->prev;
			p->next = head;
			p->prev = NULL;
			head->prev = p;
			head = p;
			p = next;
		}
	}
	cout << "list has been sorted" << endl;
}

void Payments::insertAfter(Tlist& after, Tlist& node, bool istail) {
	node->prev->next = node->next;
	if (!istail) node->next->prev = node->prev;
	node->next = after->next;
	after->next = node;
	node->prev = after;
	node->next->prev = node;
}

void Payments::writeToFile(ofstream& file) {
	if (!Empty()) {
		Tlist it = head;

		while (it != NULL) {
			string supplier = to_string(it->sup_account_number);
			string client = to_string(it->cl_account_number);
			string purpose = it->purpose;
			string sum = to_string(it->sum);
			string splitter = " ";

			string str = supplier + splitter + client + splitter + purpose + splitter + sum;
			file << str << endl;
			it = it->next;
		}
	}
}

void Payments::readFromFile(ifstream& file) {
	char ch = file.peek();
	if (ch == EOF) cout << "empty file" << endl;
	while (!file.eof()) {
		string str;
		getline(file, str);

		int n = str.find(' ');
		string sup = str.substr(0, n);
		int supplier = atoi(sup.c_str());
		str = str.substr(n+1, str.length());

		n = str.find(' ');
		string cl = str.substr(0, n);
		int client = atoi(cl.c_str());
		str = str.substr(n+1, str.length());

		n = str.find(' ');
		string purpose = str.substr(0, n);
		str = str.substr(n+1, str.length());

		string s = str;
		int sum = atoi(s.c_str());

		if (Empty()) addToHead(supplier, client, purpose, sum);
		else addToTail(supplier, client, purpose, sum);
	}
}

bool Payments::Empty() {
	return head == NULL;
}

void Payments::changeNode(Tlist& p) {       // редактирование узла
	cout << "Current state of the payment:" << endl;
	printNode(p);

	int temp;
	string str;
	cout << "Enter new supplier account: ";
	cin >> temp;
	cout << endl;
	p->sup_account_number = abs(temp);
	cout << "Enter new client account: ";
	cin >> temp;
	cout << endl;
	p->cl_account_number = abs(temp);
	cout << "Enter new purpose: ";
	cin >> str;
	cout << endl;
	p->purpose = str;
	cout << "Enter new sum: ";
	cin >> temp;
	cout << endl;
	p->sum = abs(temp);

    cout << "New state of the payment:" << endl;
	printNode(p);
}

void Payments::swapNodes(Tlist& first, Tlist& second) {
	Tlist temp1 = second->next, temp2 = second->prev;
	second->next = first->next;
	second->prev = first->prev;
	first->next = temp1;
	first->prev = temp2;

	if (head == first) head = second;
	else if (head == second) head = first;

	if (tail == first) tail = second;
	else if (tail == second) tail = first;
}

void Payments::printList() {
	Tlist p = head;
	cout << count << " payments:" << endl;
	while (p != NULL) {
		printNode(p);
		cout << endl;
		p = p->next;
	}
}

void Payments::delFromTail() {
	Tlist p = tail;
	tail = tail->prev;
	p->prev = NULL;
	tail->next = NULL;
	delete p;
	count--;
}

void Payments::delFromHead() {
	Tlist p = head;
	head = head->next;
	p->next = NULL;
	head->prev = NULL;
	delete p;
	count--;
}

void Payments::addToHead(int san, int can, string pur, int sum) {
	if (Empty()) {
		head = new NODE(san, can, pur, sum);
		tail = head;
		head->next = NULL;
		head->prev = NULL;
	}
	else {
		Tlist p = new NODE(san, can, pur, sum);
		p->next = head;
		p->prev = NULL;
		head->prev = p;
		head = p;
	}
	count++;
}

void Payments::addToTail(int san, int can, string pur, int sum) {
	if (Empty()) addToHead(san, can, pur, sum);
	else {
		Tlist p = new NODE(san, can, pur, sum);
		p->next = NULL;
		p->prev = tail;
		tail->next = p;
		tail = p;
		count++;
	}
	
}

void Payments::printNode(Tlist& p) {          // печать узла
	cout << "supplier: " << p->sup_account_number;
	cout << " | client: " << p->cl_account_number;
	cout << " | purpose: " << p->purpose;
	cout << " | sum: " << p->sum << endl;
}

void Payments::printSupplier(int account) {       // печать узлов с поставщиком, соответствующим введенному
	Tlist p = head;
	int num = 0;
	int sum = 0;
	while (p != NULL) {
		if (p->sup_account_number == account) {
			num++;
			printNode(p);
			sum += p->sum;
		}
		p = p->next;
	}
	if (!num) cout << "No suppliers found with account number: " << account << endl;
	else cout << num << " suppliers with total: " << sum << endl;
}

void Payments::searchPurpose(string str) {      // поиск узлов по введенному назначению платежа
	Tlist p = head;
	int num = 0;
	while (p != NULL) {
		if (p->purpose == str) {
			num++;
			printNode(p);
		}
		p = p->next;
	}
	if (!num) cout << "Nothing found with purpose: " << str << endl;
}

void Payments::clearList() {
	Tlist p = head;
	while (count) {
		head = head->next;
		p->next = NULL;
		p->prev = NULL;
		delete p;
		p = head;
		count--;
	}
	head = NULL;
	tail = NULL;
	cout << "List has been cleared" << endl;
}

Payments::Payments() {
	count = 0;
	head = NULL;
	tail = NULL;
}

Payments::~Payments() {
	clearList();
}

--------------------------------------------------

#include "List2.h"

using namespace std;

int main() {
	Payments* paymentList = new Payments;
	paymentList->Menu();
	system("pause");
}

-------------------------------------------------

