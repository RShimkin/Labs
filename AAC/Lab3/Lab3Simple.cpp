#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include <iterator>

using namespace std;

class Marriage {
    const int size;
    int unmarried;
    set<char> names;
    struct Person {
        char name;
        Person* partner;
        vector<Person*> pref;
        int varNum;
        Person(char n) {
            name = n;
            varNum = 0;
            partner = NULL;
        }
        Person* nextVar() {
            if (varNum < pref.size())
                return pref[varNum++];
            else {
                varNum = 0;
                return pref[varNum];
            }
        }
        int findPos(Person* pers) {
            for (int i = 0; i < pref.size(); i++) {
                if (pref[i] == pers)
                    return i;
            }
        }
        bool isBetter(Person* candidate) {
            if (!partner) return true;
            else {
                int curPos = findPos(partner);
                int newPos = findPos(candidate);
                if (curPos > newPos) return true;
                else return false;
            }
        }
        void add(Person* pers) {
            pref.push_back(pers);
        }
    };
public:
    vector<Person> man;
    vector<Person> woman;
    Marriage(int num): size(num) {
        unmarried = size;
    }
    Person& find(char x) {
        bool isMan = ((x >= 'a') && (x <= 'z'));
        if (isMan) {
            for (auto cur : man) {
                if (cur.name == x) return cur;
            }
        }
        else {
            for (auto cur : woman) {
                if (cur.name == x) return cur;
            }
        }
    }
    void parseNames(string str) {
        int pos = 0;
        while (pos != str.length()) {
            char x = str[pos];
            bool firstCondition = ((x >= 'a') && (x <= 'z'));
            bool secondCondition = ((x >= 'A') && (x <= 'Z'));
            if (!(firstCondition || secondCondition))
                pos++;
            else {
                if (firstCondition) {
                    man.push_back(Person(x));
                    names.insert(x);
                }
                else woman.push_back(Person(x));
                pos++;
            }
        }
    }
    void parse(string str) {
        char name = str[0];
        string prefstr = str.substr(2, str.length() - 2);
        Person* cur = NULL;
        bool isGood = (((name >= 'a') && (name <= 'z')) || ((name >= 'A') && (name <= 'Z')));
        if (isGood) cur = &find(name);
        else cout << "Wrong input!" << endl;
        if (cur) {
            int pos = 0;
            char newPrefName = prefstr[pos];
            Person* newPref;
            while (pos != size) {
                //cur->pref.push_back(&find(newPref));
                newPref = &find(newPrefName);
                cur->add(newPref);
                newPrefName = prefstr[++pos];
            }
        }
    }
    void setPairs() {
        while (unmarried) {
            Person* cur = NULL;
            for (auto x : man) {
                if (!x.partner) {
                    cur = &x;
                    break;
                }
            }
            Person* var = cur->nextVar();
            if (!var->partner) {
                cur->partner = var;
                var->partner = cur;
                unmarried--;
            }
            else if (var->isBetter(cur)) {
                var->partner->partner = NULL;
                var->partner = cur;
                cur->partner = var;
            }
        }
    }
    void printPairs() {
        for (auto it = names.begin(); it != names.end(); it++) {
            cout << *it << " " << (find(*it).partner)->name << endl;
        }
    }
};

int main()
{
    int numOfTests;
    cin >> numOfTests;
    for (int test = 0; test < numOfTests; test++) {
        int n;
        cin >> n;
        Marriage brak(n);
        string names;
        cout << "names: |" << names << "|" << endl;
        cin.ignore();
        getline(cin, names);
        cout << "names: |" << names << "|" << endl;
        brak.parseNames(names);

        auto x = &brak.find('a');
        cout << "x: " << x << endl;
        auto y = &brak.find('A');
        cout << "y: " << y << endl;
        x->partner = y;
        x->pref.push_back(y);

        for (int k = 0; k < 2 * n; k++) {
            string str;
            getline(cin, str);
            cout << "str: |" << str << "|" << endl;
            brak.parse(str);
        }
        
        brak.setPairs();
        brak.printPairs();
    }
}
