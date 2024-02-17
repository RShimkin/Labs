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
