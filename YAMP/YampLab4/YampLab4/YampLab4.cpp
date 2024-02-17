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