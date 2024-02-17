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