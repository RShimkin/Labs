#include <iostream>

using namespace std;

struct Node {
    int x;
    Node* next;
};

typedef Node* Tlist;

void delAfter(Tlist& list) {
    Tlist p = list->next;
    list->next = p->next;
    p->next = NULL;
    delete p;
}

void task1(Tlist& list, int cifra) {
    Tlist p = list;
    Tlist elem = NULL;
    while (p != NULL) {
        if ((p->next->x) % 10 == cifra) elem = p;
        p = p->next;
    }
    if (elem) delAfter(elem);
}

void task2(Tlist& list) {    // для списка без заглавного звена, увеличить значение нечетных в 2 раза
    Tlist p = list;
    while (p != NULL) {
        if (abs(p->x) % 2 != 0) {
            p->x = 2 * (p->x);
        }
        p = p->next;
    }
}

void anotherTask2(Tlist& list) {    // для списка без заглавного звена, добавить элемент еще раз
    Tlist p = list;
    while (p != NULL) {
        if (abs(p->x) % 2 != 0) {
            Tlist newNode = new Node;
            newNode->next = p->next;
            newNode->x = p->x;
            p->next = newNode;
        }
        p = p->next;
    }
}

void task3(Tlist& list) { // для списка с заглавным звеном
    Tlist pmin = list;
    int min = list->next->x;
    Tlist p = list->next;
    if (list && list->next) {
        while (p->next != NULL) {
            if (p->next->x < min) {
                pmin = p;
                min = p->next->x;
            }
            p = p->next;
        }
        p->next = pmin->next;
        pmin->next = pmin->next->next;
        p->next->next = NULL;
    }
}

void task4(Tlist& list) {
    Tlist p = list;
    if (list && list->next) {
        Tlist t = p->next;
        Tlist temp = t->next;
        while (t) {
            if (p == list) p->next = NULL;
            t->next = p;
            p = t;
            t = temp;
            temp = temp->next;
        }
        list = p;
    }
}

void task5(Tlist& list) {
    Tlist p = list;
    int val;
    if (p) {
        val = p->x;
        while (p->next) {
            if (p->next->x == val) {
                Tlist del = p->next;
                p->next = del->next;
                del->next = NULL;
                delete del;
            }
            else p = p->next;
        }
    }
}







bool find(Tlist list, int val) {
    Tlist p = list;
    bool res = false;
    while (p != NULL) {
        if (p->x == val) {
            res = true;
            return res;
        }
        p = p->next;
    }
    return res;
}

void add(Tlist& list, int val) {
    Tlist newNode = new Node;
    newNode->next = NULL;
    newNode->x = val;

    Tlist p = NULL;
    if (!list) list = newNode;
    else {
        p = list;
        while (p->next) p = p->next;
        p->next = newNode;
    }
}

Tlist& task(Tlist first, Tlist second) {
    Tlist p = first;
    Tlist newList = NULL;
    while (p != NULL) {
        if (!find(second, p->x)) {
            if (!find(newList, p->x)) add(newList, p->x);
        }
        p = p->next;
    }
    p = second;
    while (p != NULL) {
        if (!find(newList, p->x)) add(newList, p->x);
        p = p->next;
    }
    return newList;
}

void print(Tlist list) {
    Tlist p = list;
    while (p != NULL) {
        cout << p->x << " ";
        p = p->next;
    }
    cout << endl;
}

