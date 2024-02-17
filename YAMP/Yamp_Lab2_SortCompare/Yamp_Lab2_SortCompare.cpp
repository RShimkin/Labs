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
