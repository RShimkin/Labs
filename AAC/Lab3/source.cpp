#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>

using namespace std;

struct Person {
    string name;
    Person* partner;
    vector<Person*> pref;
    string prefstr;
};

int main()
{
    ifstream file("input.txt");
    int numOfTests;
    file >> numOfTests;
    set<string> names;
    for (int test = 0; test < numOfTests; test++) {
        int n;
        file >> n;
        Person* man = new Person[n];
        Person* woman = new Person[n];
        for (int i = 0; i < 2 * n; i++) {
            char x = file.get();
            while (x = ' ') file >> x;
            while (x != ' ') {
                file >> x;
                if (i < n) man[i].name += x;
                else woman[i % n].name += x;
            }
            if (i < n) names.insert(man[i].name);
            else names.insert(woman[i % n].name);
        }
        for (int k = 0; k < 2 * n; k++) {
            string str;
            getline(file, str);
            auto del = str.find(":");
            string name = str.substr(0, del);
            string prefstr = str.substr(del + 1, str.length() - del - 1);
            Person* pers;
            for (int j = 0, bool found = false; !found; j++) {
                if (man[j % n].name == name) {
                    pers = &man[j % n];
                    found++;
                }
                else if (woman[j % n].name == name) {
                    pers = &woman[j % n];
                    found++;
                }
            }
            string buf = "";
            char x;
            for (int m = 0; m < n; m++) {
                file >> x;
                buf += x;

            }
        }
            
    }
}
