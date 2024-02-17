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