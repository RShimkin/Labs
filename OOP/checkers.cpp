#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <windows.h>

using namespace std;

enum VariantType {
	vOUT,
	vEMPTY,
	vBLOCKED,
	vJUMP
};

enum CursorState {
	cHOVER,
	cACTIVE, 
	cCHOICE
};

enum UnitType {
	NO, 
	FIRST,
	SECOND
};

enum UnitState {
	PASSIVE,
	HOVER,
	ACTIVE, 
};

enum Player {
	pFIRST,
	pSECOND
};

enum SquareState {
	sPASSIVE,
	sHOVER,
	sVARIANT,
	sACTIVEVARIANT
};

class Quadrate {
public:
	
	virtual void show() {
		cout << "  " << endl;
	}
};

class Unit : public Quadrate {
	UnitType type;
	UnitState state;
public:
	Unit() {
		type = NO;
		state = PASSIVE;
	}
	void setType(UnitType t) {
		type = t;
	}
	void set(UnitType t, UnitState st) {
		type = t;
		state = st;
	}
	void setState(UnitState st) {
		state = st;
	}
	UnitType getType() {
		return type;
	}
	UnitState getState() {
		return state;
	}
	void show() {
		switch (state) {
		case PASSIVE:
			cout << (int)type << " ";
			break;
		case HOVER:
			cout << (int)type << "!";
			break;
		case ACTIVE:
			cout << (int)type << (int)type;
			break;
		}
	}
	friend class Cursor;
};

class Cursor : public Quadrate {
public:
	int size;
	int x, y;
	int chosenX, chosenY;
	Unit** u;
	Square** s;
	CursorState state;
	Cursor(int k) {
		size = k;
		state = cHOVER;
		chosenX = chosenY = -1;
	}
	void switchState() {
		switch (state) {
		case cHOVER:
			state = cACTIVE;
			break;
		case cACTIVE:
			state = cCHOICE;
			break;
		case cCHOICE:
			state = cHOVER;
			break;
		}
	}
	void putOnCenter() {
		x = size / 2;
		y = size / 2;
		chosenX = chosenY = 0;
	}
	void move() {
		char x;
		x = _getch();
		switch (x) {
		case 'A':
		case 'a':
			if (y - 1 >= 0) y--;
			break;
		case 'D':
		case 'd':
			if (y + 1 <= size) y++;
			break;
		case 'W':
		case 'w':
			if (x - 1 >= 0) x--;
			break;
		case 'S':
		case 's':
			if (x + 1 <= size) x++;
			break;
		case 'G':
		case 'g':
			if (u[x][y].getType() != 0) {
				chosenX = x;
				chosenY = y;
				switchState();
			}
		}
	}
};

class Square {
	int x, y;
	enum Color {
		WHITE,
		BLACK
	} color;
	SquareState state;
public:
	Square() {

	}
	Square(int x, int y) {
		this->x = x;
		this->y = y;
		state = sPASSIVE;
		if ((x + y) % 2 == 0) color = WHITE;
		else color = BLACK;
	}
	Square operator=(Square sq) {
		this->x = sq.x;
		this->y = sq.y;
		this->state = sq.state;
		return sq;
	}
	void show() {
		if (state == sPASSIVE) {
			if (color == WHITE) cout << "w ";
			else cout << "b ";
		}
		else if (state == sHOVER) {
			if (color == WHITE) cout << "w!";
			else cout << "b!";
		}
		else cout << "??";
	}
	void setState(SquareState st) {
		state = st;
	}
};

class Desk {
	const int size;
	Square** kv;
	int hoverX, hoverY;
	int varX, varY;
	int activevarX, activevarY;
public:
	Desk(int k) : size(k) {
		kv = new Square * [size];
		for (int i = 0; i < size; i++) {
			kv[i] = new Square[size];
			for (int j = 0; j < size; j++)
				kv[i][j] = Square(i, j);
		}
	}
	void show(int x, int y) {
		kv[x][y].show();
	}
	void setState(int x, int y, SquareState st) {
		switch (st) {
		case sHOVER:
			hoverX = x;
			hoverY = y;
			break;
		case sVARIANT:
			varX = x;
			varY = y;
			break;
		case sACTIVEVARIANT:
			activevarX = x;
			activevarY = y;
		}
		kv[x][y].setState(st);
	}
	void setState() {
		kv[hoverX][hoverY].setState(sPASSIVE);
		kv[varX][varY].setState(sPASSIVE);
		kv[activevarX][activevarY].setState(sPASSIVE);
	}
	friend class Game;
};

class Checkers {
	const int size;
	Unit** unit;
	int player1, player2;
	int hoverX, hoverY;
	int activeX, activeY;
public: 
	Checkers(int k) : size(k) {
		int numOfUnits = k / 2 * 3;
		player1 = player2 = numOfUnits;
		unit = new Unit * [size];
		for (int i = 0; i < size; i++) {
			unit[i] = new Unit[size];

		}
		for (int i = 0, int first = numOfUnits; (i < size) && first; i++) {
			for (int m = 0; m < size; m++) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(FIRST);
					first--;
				}
			}
		}
		for (int i = size - 1, int second = numOfUnits; (i >= 0) && second; i--) {
			for (int m = size - 1; m >= 0; m--) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(SECOND);
					second--;
				}
			}
		}
		hoverX = hoverY = -10;
	}
	UnitType getType(int x, int y) {
		return unit[x][y].getType();
	}
	void setState(int x, int y, UnitState st) {
		switch (st) {
		case HOVER:
			hoverX = x;
			hoverY = y;
			break;
		case ACTIVE:
			activeX = x;
			activeY = y;
			break;
		}
		unit[x][y].setState(st);
	}
	void setState() {
		unit[hoverX][hoverY].setState(PASSIVE);
		unit[activeX][activeY].setState(PASSIVE);
	}
	void show(int i, int j) {
		unit[i][j].show();
	}
	int getPlayer1State() {
		return player1;
	}
	int getPlayer2State() {
		return player2;
	}
	int decPlayer(UnitType t) {
		if (t == FIRST) player1--;
		else player2--;
	}
	void setType(int a, int b, UnitType t) {
		unit[a][b].setType(t);
	}
	friend class Cursor;
};

struct Variants {
public:
	int now;
	int num;
	int size;
	int x1, x2, y1, y2;
	VariantType firstType, secondType;
	VariantType nowType;
	Variants(int k) {
		x1 = x2 = y1 = y2 = 0;
		num = 0;
		size = k;
		firstType = secondType = vOUT;
		now = 1;
	}
	void checkTypes() {
		bool check1 = ((firstType == vEMPTY) || (firstType == vJUMP));
		bool check2 = ((secondType == vEMPTY) || (secondType == vJUMP));
		if (!check1 && check2) {
			num--;
			x1 = x2;
			y1 = y2;
			firstType = secondType;
		}
		else if (check1 && !check2 && (num == 2)) num--;
		else if (!check1 && !check2 && num) num = 0;
	}
	void set(int a, int b, int c, int d) {
		if ((a >= 0) && (b >= 0) && (a < size) && (b < size)) {
			num++;
			x1 = a;
			y1 = b;
		}
		if ((c >= 0) && (d >= 0) && (c < size) && (d < size)) {
			if (num) {
				num++;
				x2 = c;
				y2 = d;
			}
			else {
				x1 = c;
				y1 = d;
			}
		}
	}
};

class Game {
	const int size;
	Desk desk;
	Checkers chek;
	Cursor curs;
	Variants variants;
	void setState(int var = 0) {
		int a = curs.x, b = curs.y;
		if (var == 0) {
			chek.setState();
			desk.setState();
		}
		else if (var == 1) {
			if (chek.getType(a, b) != NO) chek.setState(a, b, HOVER);
			else desk.setState(a, b, sHOVER);
		}
		else if (var == 2) {
			chek.setState(curs.chosenX, curs.chosenY, ACTIVE);
			desk.setState(desk.activevarX, desk.activevarY, sVARIANT);
			desk.setState(curs.x, curs.y, sACTIVEVARIANT);
		}
	}
public:
	Game(int k) : size(k), desk(k), chek(k), curs(k), variants(k) {
		
	}
	void show() {
		for (int i = 0; i < size; i++) {
			for (int m = 0; m < size; m++) {
				if (chek.getType(i, m) != NO) chek.show(i, m);
				else desk.show(i,m);
			}
			cout << endl;
		}
	}
	void switchCursor() {
		char x = _getch();
		if (variants.num == 2 && ((x == 'a') || (x == 'A') || (x == 'd') || (x == 'D'))) {
			if (variants.y1 == curs.y) {
				variants.now = variants.secondType;
				curs.x = variants.x2;
				curs.y = variants.y2;
			}
			else {
				variants.now = variants.firstType;
				curs.x = variants.x1;
				curs.y = variants.y1;
			}
		}
		else if ((x == 'g') || (x == 'G')) {
			curs.state = cCHOICE;
		}
	}
	void variantsType(Player p, int a, int b) {
		if (variants.num == 2) {
			UnitType one, two;
			UnitType t = (p == pFIRST) ? FIRST : SECOND;
			one = chek.getType(variants.x1, variants.y1);
			two = chek.getType(variants.x2, variants.y2);

			if (one == NO) variants.firstType = vEMPTY;
			else if (one == t) variants.firstType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x1 + 1;
				else tempX = variants.x1 - 1;
				tempY = variants.y1 - 1;
				variants.x1 = tempX;
				variants.y1 = tempY;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.firstType = vJUMP;
					else variants.firstType = vBLOCKED;
				}
				else variants.firstType = vOUT;
			}

			if (two == NO) variants.secondType = vEMPTY;
			else if (two == t) variants.secondType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x2 + 1;
				else tempX = variants.x2 - 1;
				tempY = variants.y2 + 1;
				variants.x2 = tempX;
				variants.y2 = tempY;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.secondType = vJUMP;
					else variants.secondType = vBLOCKED;
				}
				else variants.secondType = vOUT;
			}
		}
		else if (variants.num == 1) {
			UnitType temp;
			UnitType t = (p == pFIRST) ? FIRST : SECOND;
			temp = chek.getType(variants.x1, variants.y1);

			if (temp == NO) variants.firstType = vEMPTY;
			else if (temp == t) variants.firstType = vBLOCKED;
			else {
				int tempX, tempY;
				if (p == pFIRST) tempX = variants.x1 + 1;
				else tempX = variants.x1 - 1;
				if (variants.y1 > b) tempY = variants.y1 + 1;
				else tempY = variants.y1 - 1;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.firstType = vJUMP;
					else variants.firstType = vBLOCKED;
				}
			}
		}
	}
	void play() {
		bool goon = true;
		Player pl = pFIRST;
		bool marked = false;
		bool checked = false;
		int tempX, tempY;
		while (goon) {
			if (curs.state == cHOVER) {
				setState(1);
			}
			else if (curs.state == cACTIVE) {
				int a = curs.x, b = curs.y;
				UnitType type = chek.getType(a, b);
				if (!checked && (type == NO) || ((pl == pFIRST) && (type == SECOND)) || ((pl == pSECOND) && (type == FIRST))) {
					curs.state = cHOVER;
					setState(1);
				}
				else {
					checked = true;
					if (!marked) {
						int x1, x2, y1, y2;
						if (pl == pFIRST) x2 = x1 = a - 1;
						else x2 = x1 = a + 1;
						y1 = b - 1;
						y2 = b + 1;
						variants.set(x1, y1, x2, y2);
						variantsType(pl, a, b);
						variants.checkTypes();
						curs.chosenX = a;
						curs.chosenY = b;
					}
					
					if (!variants.num) {
						curs.state = cHOVER;
						setState(1);
						marked = checked = false;
					}
					else {
						if (!marked) {
							curs.x = variants.x1;
							curs.y = variants.y1;
							variants.now = variants.firstType;
						}
						
						marked = true;
						setState(2);
					}
				}
			}
			else {	
				if (variants.now == vJUMP) {
					int enemyX, enemyY;

					if (pl == pFIRST) {
						enemyX = curs.x - 1;
						chek.decPlayer(SECOND);
					}
					else {
						enemyX = curs.x + 1;
						chek.decPlayer(FIRST);
					}

					if (curs.y > curs.chosenY) enemyY = curs.y - 1;
					else enemyY = curs.y + 1;

					chek.setType(enemyX, enemyY, NO);
				}

				if (pl == pFIRST) chek.setType(curs.x, curs.y, FIRST);
				else chek.setType(curs.x, curs.y, SECOND);
				chek.setType(curs.chosenX, curs.chosenY, NO);

				curs.state = cHOVER;
				setState(1);
				if (pl == pFIRST) pl = pSECOND;
				else pl = pFIRST;
			}
			show();
			if (curs.state == cACTIVE) switchCursor();
			else curs.move();
			setState();
			goon = chek.getPlayer1State() && chek.getPlayer2State();
		}
	}
};

auto main() -> int {
	Game shashki(8);
	shashki.play();
}
