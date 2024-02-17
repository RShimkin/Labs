#include <iostream>
#include <cstdlib>
#include <stack>
#include <string>
#include <conio.h>
#include <windows.h>
#include <fstream>

using namespace std;

class MyQueue {
    float* arr;
    int first, last;
    int arrSize;
    int size;
public:
    MyQueue() {
        arrSize = 10;
        arr = (float*)malloc(arrSize * sizeof(float));
        first = 0;
        last = -1;
        size = 0;
    }
    void printEven() const {
        for (int i = first; i < first + size; i++)
            if (((int)arr[i]) % 2 == 0) cout << arr[i] << " ";
        cout << endl;
    }
    int getSize() const {
        return size;
    }
    void pushBack(float newNumber) {
        arr[++last] = newNumber;
        size++;
        if (((float)size / arrSize) > 0.9) {
            arrSize *= 1.5;
            arr = (float*)realloc(arr, sizeof(float) * arrSize);
        }
    }
    float seeLast() const {
        if (size) return arr[last];
    }
    float seeFirst() const {
        if (size) return arr[first];
    }
    float takeFirst() {
        if (size) {
            size--;
            float x = arr[first++];
            if (first > 5) move();
            return x;
        }
    }
    void move() {
        if (first) {
            int dif = first;
            for (int i = 0; i < size; i++) arr[i] = arr[i + first];
            last -= first;
            first = 0;
        }
    }
    float getElement(int a) const {
        if ((a > 0) && (a <= size)) return arr[first + a - 1];
        else cout << "wrong position";
        return NAN;
    }
    void print() {
        for (int i = first; i < first + size; i++)
            cout << arr[i] << " ";
    }
    void del() {
        first = 0;
        last = -1;
        size = 0;
    }
    void del(int pos) {
        bool correct = (pos > 0) && (pos <= size);
        if (correct) {
            if (pos < size * 0.5) {
                for (int i = --pos + first; i > first; i--)
                    arr[i] = arr[i - 1];
                first++;
            }
            else {
                for (int i = --pos + first; i < last; i++)
                    arr[i] = arr[i + 1];
                last--;
            }
            size--;
        }
        else cout << "wrong position!" << endl;
    }
    ~MyQueue() {
        free(arr);
        arr = NULL;
    }
};

void printEvenStack(stack<float>& st) {
    float x;
    stack<float> temp;
    cout << "stack: ";
    while (!st.empty()) {
        x = st.top();
        temp.push(x);
        if (((int)x) % 2 == 0) cout << x << " ";
        st.pop();
    }
    while (!temp.empty()) {
        st.push(temp.top());
        temp.pop();
    }
    cout << endl;
}

void deleteStack(stack<float>& st) {
    while (!st.empty()) st.pop();
}

void del(stack<float>& st, int a) {
    if ((a > 0) && (a <= st.size())) {
        stack<float> temp;
        for (int i = 1; i < a; i++) {
            temp.push(st.top());
            st.pop();
        };
        st.pop();
        while (!temp.empty()) {
            st.push(temp.top());
            temp.pop();
        }
    }
    else cout << "wrong position!" << endl;
}

float getStackElement(stack<float>& st, int a) {
    if ((a > 0) && (a <= st.size())) {
        stack<float> temp;
        for (int i = 1; i < a; i++) {
            temp.push(st.top());
            st.pop();
        };
        float x = st.top();
        while (!temp.empty()) {
            st.push(temp.top());
            temp.pop();
        }
        return x;
    }
    else cout << "wrong position";
    return NAN;
}

void printStack(stack<float>& st) {
    float x;
    stack<float> temp;
    cout << "stack: ";
    while (!st.empty()) {
        x = st.top();
        temp.push(x);
        cout << x << " ";
        st.pop();
    }
    while (!temp.empty()) {
        st.push(temp.top());
        temp.pop();
    }
    cout << endl;
}

void printQueue(MyQueue& qu) {
    cout << "queue: ";
    qu.print();
    cout << endl;
}

auto task(MyQueue& q1, MyQueue& q2) {
    string filename;
    cout << "enter the file name: ";
    cin >> filename;
    ifstream file(filename.c_str());
    if (file.is_open()) {
        q1.del();
        q2.del();
        cout << "file is opened" << endl;
        cout << "Enter A: ";
        float x, A;
        cin >> A;
        cout << "\nfile:" << endl;
        while (!file.eof()) {
            file >> x;
            cout << x << endl;
            if (x > A) q1.pushBack(x);
            else q2.pushBack(x);
        }
        cout << "\nfirst queue: ";
        q1.print();
        cout << "\nsecond queue: ";
        q2.print();
        cout << endl;
    }
    else cout << "file is not opened!" << endl;
}

auto task(stack<float>& s1, stack<float>& s2) {
    string filename;
    cout << "enter the file name: ";
    cin >> filename;
    ifstream file(filename.c_str());
    if (file.is_open()) {
        deleteStack(s1);
        deleteStack(s2);
        stack<float> temp1, temp2;
        cout << "file is opened" << endl;
        cout << "Enter A: ";
        float x, A;
        cin >> A;
        cout << "\nfile:" << endl;
        while (!file.eof()) {
            file >> x;
            cout << x << endl;
            if (x > A) temp1.push(x);
            else temp2.push(x);
        }
        file.close();
        while (!temp1.empty()) {
            s1.push(temp1.top());
            temp1.pop();
        }
        while (!temp2.empty()) {
            s2.push(temp2.top());
            temp2.pop();
        }
        cout << "\nfirst stack: ";
        printStack(s1);
        cout << "\nsecond stack: ";
        printStack(s2);
        cout << endl;
    }
    else cout << "file is not opened!" << endl;
}

auto menu(MyQueue& q1, MyQueue& q2, stack<float>& s1, stack<float>& s2) {
    string m[] = {
        " #Enter q/Q to quit",
        " #Enter 1 to work with MyQueue",
        " #Enter 2 to work with stack",
        " #Enter m/M to go to the main menu",
        " #Enter 0 to clean the console",
        " #Enter 1 for file input",
        " #Enter 2 for manual input",
        " #Enter 3 to print the first element of the queue/stack",
        " #Enter 4 to print all the elements",
        " #Enter 5 to print particular element",
        " #Enter 6 to delete all the elements",
        " #Enter 7 to fulfill the task",
        " #Enter 8 to print even",
        " #Enter 9 to delete particular element",
    };

    char x;
    bool goon;

    do {
        for (int i = 0; i < 3; i++) cout << m[i] << endl;
        x = _getch();
        char z;
        goon = (x == '1') || (x == '2');

        if (goon) {
            z = '0';
            bool quit = false;
            float temp;
            while (!quit) {
                switch (z) {
                case '0':
                    system("cls");
                    for (int i = 3; i < 14; i++) cout << m[i] << endl;
                    break;
                case '1':
                {
                    string filename;
                    getline(cin, filename);
                    ifstream file(filename.c_str());
                    if (file.is_open()) {
                        while (!file.eof()) {
                            file >> temp;
                            cout << "|" << temp << "|\n"; //////////////////////////////////////////////////
                            if (x == '1') q1.pushBack(temp);
                            else s1.push(temp);
                        }
                        cout << "file is read!" << endl;
                        file.close();
                    }
                    else cout << "file is not opened!";
                }
                break;
                case '2':
                    cout << "Enter the number: ";
                    cin >> temp;
                    if (x == '1') q1.pushBack(temp);
                    else s1.push(temp);
                    break;
                case '3':
                    cout << "first element: ";
                    if (x == '1') cout << q1.seeFirst();
                    else if (!s1.empty()) cout << s1.top();
                    cout << endl;
                    break;
                case '4':
                    if (x == '1') printQueue(q1);
                    else printStack(s1);
                    break;
                case '5':
                    if (x == '1') {
                        int pos;
                        cout << q1.getSize() << " elements in queue: ";
                        cin >> pos;
                        cout << q1.getElement(pos) << endl;
                    }
                    else {
                        int pos;
                        cout << s1.size() << " elements in stack: ";
                        cin >> pos;
                        cout << endl << getStackElement(s1, pos) << endl;
                    }
                    break;
                case '6':
                    if (x == '1') {
                        q1.del();
                        cout << "the queue is empty!" << endl;
                    }
                    else {
                        deleteStack(s1);
                        cout << "the stack is empty!" << endl;
                    }
                    break;
                case '7':
                    if (x == '1') task(q1, q2);
                    else task(s1, s2);
                    break;
                case '8':
                    if (x == '1') q1.printEven();
                    else printEvenStack(s1);
                    break;
                case '9':
                    cout << "enter the position of element to delete: ";
                    int pos;
                    cin >> pos;
                    if (x == '1') q1.del(pos);
                    else del(s1, pos);
                    break;
                }
                z = _getch();
                quit = (z == 'm') || (z == 'M');
            }
            system("cls");
        }
    } while (goon);
}

int main()
{
    MyQueue qu1, qu2;
    stack<float> st1, st2;
    menu(qu1, qu2, st1, st2);
}
