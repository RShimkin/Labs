#include <iostream>

using namespace std;

class A {
	int a, b;
public:
	A(int _a, int _b): a(_a), b(_b) {}
};

class B : public A {
	int a, b;
public:
	B(int _a, int _b) :A(_a, _b) {
		a = _a;
		b = _b;
	}
};

int main() {
	B obj(1, 2);
	cout << " ";
}