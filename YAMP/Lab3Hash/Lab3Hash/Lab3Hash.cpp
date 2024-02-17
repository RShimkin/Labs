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
