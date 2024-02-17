#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Quadrate {
public:
	char sign = ' ';
	enum Mark {
		NO,
		HOVER, 
		ACTIVE
	} mark;
	virtual void show() {
		cout << sign << " " << endl;
	}
};

class Unit : private Quadrate {
	enum Type {
		NO,
		FIRST,
		SECOND
	} type;
	bool active;
public: 
	Unit() {

	}
	void show() {
		if (active)
	}
};

class Square {
	int x, y;
	enum color {
		WHITE,
		BLACK
	} state;
public:
	Square() {

	}
	Square(int x, int y) {
		this->x = x;
		this->y = y;
		if ((x + y) % 2 == 0) state = WHITE;
		else state = BLACK;
	}
	Square operator=(Square sq) {
		this->x = sq.x;
		this->y = sq.y;
		this->state = sq.state;
		return sq;
	}
	void show() {
		switch (state) {
		case WHITE:
			cout << "w ";
			break;
		case BLACK:
			cout << "b ";
			break;
		}
	}
};

class Desk {
	const int size;
	Square** kv;

public: 
	Desk() : size(8) {
		kv = new Square*[size];
		for (int i = 0; i < size; i++) {
			kv[i] = new Square[size];
			for (int j = 0; j < size; j++)
				kv[i][j] = Square(i, j);
		}	
	}
	void print() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++)
				kv[i][j].show();
			cout << endl;
		}
	}
};

class Checkers {

};

class Game {
	Desk d;

public:
	void play() {

	}
};

auto main() -> int {
	Game shashki;
	shashki.play();
}
