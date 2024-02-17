// Шимкин Р.Р. ПММ62 2 курс Лаба №4?
// задание: реализовать игру "шашки"
#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <time.h>

using namespace std;

// перечисление типов вариантов хода
enum VariantType {
	vOUT,
	vEMPTY,
	vBLOCKED,
	vJUMP
};

// перечисление состояний курсора
enum CursorState {
	cHOVER,
	cACTIVE,
	cCHOICE
};

// перечисление типов шашек
enum UnitType {
	NO,
	FIRST,
	SECOND
};

// перечисление состояний шашек
enum UnitState {
	PASSIVE,
	HOVER,
	ACTIVE,
};

// перечисление игроков
enum Player {
	pFIRST,
	pSECOND
};

// перечисление состояний клетки поля
enum SquareState {
	sPASSIVE,
	sHOVER,
	sVARIANT,
	sACTIVEVARIANT
};

// перечисление цветов клетки поля
enum Color {
	WHITE,
	BLACK
};

// абстрактный класс квадрата
class Quadrate {
public:

	// чисто виртуальный метод 
	virtual void show() = 0 {
		cout << "  " << endl;
	}
};

// класс шашки, наследуемый от квадрата
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
	// метод отрисовки шашки в зависимости от состояния
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
};

// класс курсора
class Cursor {
public:
	int size;
	int x, y;
	// переменные для запоминания предыдущих выбранных координат
	int chosenX, chosenY;
	CursorState state;
	Cursor(int k) {
		size = k;
		state = cHOVER;
		putOnCenter();
	}
	// метод для изменения состояния курсора
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
	// метод для перемещения курсора на центр
	void putOnCenter() {
		x = size / 2;
		y = size / 2;
		chosenX = chosenY = 0;
	}
	// метод движения курсора с проверкой выхода за границы поля
	void move() {
		char c;
		c = _getch();
		switch (c) {
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
			state = cACTIVE;
			break;
		}
	}
};

// класс клетки, наследуемый от квадрата
class Square : public Quadrate {
	Color color;
	SquareState state;
public:
	Square() {
		state = sPASSIVE;
	}
	void setColor(Color cl) {
		color = cl;
	}
	// переопределение метода отрисовки
	void show() {
		if (state == sPASSIVE) {
			if (color == WHITE) cout << "w ";
			else cout << "b ";
		}
		else if (state == sHOVER) {
			if (color == WHITE) cout << "w!";
			else cout << "b!";
		}
		else if (state == sVARIANT) {
			if (color == WHITE) cout << "w?";
			else cout << "b?";
		}
		else cout << "??";
	}
	void setState(SquareState st) {
		state = st;
	}
};

// класс игрового поля, состоящего из клеток
class Desk {
	const int size;
	// двумерный массив клеток
	Square** kv;
	// переменные для хранения координат клетки, над которой висит курсор
	int hoverX, hoverY; 
	// переменные для хранения координат неактивного варианта хода
	int varX, varY;
	// переменные для хранения координат активного хода
	int activevarX, activevarY;
public:
	// конструктор поля, клеткам задаются чередующиеся цвета
	Desk(int k) : size(k) {
		kv = new Square * [size];
		for (int i = 0; i < size; i++) {
			kv[i] = new Square[size];
			for (int j = 0; j < size; j++)
				if ((i + j) % 2 == 0) kv[i][j].setColor(WHITE);
				else kv[i][j].setColor(BLACK);
		}
	}
	// метод отрисовки конкретной клетки
	void show(int x, int y) {
		kv[x][y].show();
	}
	// сеттер состояния конкретной клетки
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
	// сеттер состояния поля клеток
	// устанавливает в пассивное состояние активные клетки перед следующим ходом
	void setState() {
		kv[hoverX][hoverY].setState(sPASSIVE);
		kv[varX][varY].setState(sPASSIVE);
		kv[activevarX][activevarY].setState(sPASSIVE);
	}
};

// класс шашек, содержащий матрицу шашек
class Checkers {
	const int size;
	// двумерный массив шашек
	Unit** unit;
	int player1, player2;
	int hoverX, hoverY;
	int activeX, activeY;
public:
	// в конструкторе шашки инициализируются принадлежностью 
	// первому или второму игроку
	Checkers(int k) : size(k) {
		int numOfUnits = k / 2 * 3;
		player1 = player2 = numOfUnits;
		unit = new Unit * [size];
		for (int i = 0; i < size; i++) {
			unit[i] = new Unit[size];

		}
		for (int i = 0, first = numOfUnits; (i < size) && first; i++) {
			for (int m = 0; m < size; m++) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(FIRST);
					first--;
				}
			}
		}
		for (int i = size - 1, second = numOfUnits; (i >= 0) && second; i--) {
			for (int m = size - 1; m >= 0; m--) {
				if ((i + m) % 2 != 0) {
					unit[i][m].setType(SECOND);
					second--;
				}
			}
		}
		hoverX = hoverY = activeX = activeY = 0;
	}
	// геттер типа конкретной шашки
	UnitType getType(int x, int y) {
		return unit[x][y].getType();
	}
	// сеттер состояния конкретной шашки
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
	// сеттер состояния шашек
	void setState() {
		unit[hoverX][hoverY].setState(PASSIVE);
		unit[activeX][activeY].setState(PASSIVE);
	}
	// отрисовка конкретной шашки
	void show(int i, int j) {
		unit[i][j].show();
	}
	// метод получения количества живых шашек 1 игрока
	int getPlayer1State() {
		return player1;
	}
	// метод получения количества живых шашек 2 игрока
	int getPlayer2State() {
		return player2;
	}
	// метод уменьшения количества живых шашек 1 из игроков
	void decPlayer(UnitType t) {
		if (t == FIRST) player1--;
		else player2--;
	}
	// сеттер типа конкретной шашки
	void setType(int a, int b, UnitType t) {
		unit[a][b].setType(t);
	}
	// объявление курсора дружественным классом (для упрощения алгоритма)
	friend class Cursor;
};

// структура вариантов хода
struct Variants {
public:
	// переменная для хранения текущего выбора хода
	int now;
	int num;
	int size;
	// буферные переменные координат вариантов
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
	// метод проверки корректности вариантов хода
	// не должно быть корректного второго и некорректного первого
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
	// простейший сеттер по умолчанию
	void set() {
		num = 0;
		now = 1;
		nowType = firstType = secondType = vOUT;
	}
	// сеттер для ввода непроверенных координат вариантов хода
	// проверяется лишь выход за границы поля
	// не может быть инициализирован второй и не инициализирован первый
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
				num++;
				x1 = c;
				y1 = d;
			}
		}
	}
};

// класс статуса и статистики
class Status {
	// счётчик ходов
	int count;
	string now;
	string one, two;
	clock_t time;
	int first, second;
public:
	Status() {
		one = "player1";
		two = "player2";
		now = one;
		count = 0;
	}
	// обновление статуса
	// принимает статус игры, был ли совершен ход на предыдущей итерации
	// и ходящего игрока
	void update(bool goon, bool moved, Player p) {
		if (moved) count++;
		if (goon) {
			if (p == pFIRST) now = one;
			else now = two;
		}
		time = clock();
	}
	// печать информации в зависимости от статуса игры
	void print(bool goon, clock_t start = -1) {
		if (goon) {
			cout << "Now: " << now << endl;
		}
		else {
			cout << "Winner: " << now << endl;
			cout << "Number of steps: " << count << endl;
			cout << "Total time: " << (time - start) / 1000;
		}
	}
};

// класс игры, выполненный в виде композиции классов
// наследующий от квадрата
class Game : public Quadrate {
	const int size;
	Desk desk;
	Checkers chek;
	Cursor curs;
	Variants variants;
	Status status;
	// сеттер состояния поля, шашек и вариантов 
	// в зависимости от статуса хода (курсора)
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
			if (variants.num == 2) {
				if (variants.now == 1) desk.setState(variants.x2, variants.y2, sVARIANT);
				else desk.setState(variants.x1, variants.y1, sVARIANT);
			}
			desk.setState(curs.x, curs.y, sACTIVEVARIANT);
		}
	}
public:
	// конструктор со списком инициализации
	Game(int k) : size(k), desk(k), chek(k), curs(k), variants(k) {}
	// метод отрисовки поля и шашек
	void show() {
		for (int i = 0; i < size; i++) {
			for (int m = 0; m < size; m++) {
				if (chek.getType(i, m) != NO) chek.show(i, m);
				else desk.show(i, m);
			}
			cout << endl;
		}
	}
	// метод переключения между вариантами хода и выбора варианта
	void switchCursor() {
		char x = _getch();
		if (variants.num == 2 && ((x == 'a') || (x == 'A') || (x == 'd') || (x == 'D'))) {
			if (variants.y1 == curs.y) {
				variants.nowType = variants.secondType;
				curs.x = variants.x2;
				curs.y = variants.y2;
				variants.now = 2;
			}
			else {
				variants.nowType = variants.firstType;
				curs.x = variants.x1;
				curs.y = variants.y1;
				variants.now = 1;
			}
		}
		else if ((x == 'g') || (x == 'G')) {
			curs.state = cCHOICE;
		}
	}
	// метод проверки типа варианта
	// (пустая клетка, чужой, свой, выход за границы)
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
				variants.x1 = tempX;
				variants.y1 = tempY;
				if ((tempX >= 0) && (tempY >= 0) && (tempX < size) && (tempY < size)) {
					UnitType next = chek.getType(tempX, tempY);
					if (next == NO) variants.firstType = vJUMP;
					else variants.firstType = vBLOCKED;
				}
			}
		}
	}
	// главный метод игры
	void play() {
		bool goon = true;
		Player pl = pFIRST;
		bool marked = false;
		bool checked = false;
		clock_t start = clock();
		while (goon) {
			bool moved = false;
			// если не выбрана шашка для хода
			if (curs.state == cHOVER) {
				setState(1);
			}
			// если активная шашка выбрана
			else if (curs.state == cACTIVE) {
				int a = curs.x, b = curs.y;
				UnitType type = chek.getType(a, b);
				// проверка, свою ли шашку выбрал игрок
				// осуществляется один раз после выбора шашки
				if (!checked && ((type == NO) || ((pl == pFIRST) && (type == SECOND)) || ((pl == pSECOND) && (type == FIRST)))) {
					curs.state = cHOVER;
					setState(1);
					status.update(goon, moved, pl);
				}
				else {
					checked = true;
					// просчёт вариантов хода
					// осуществляется один раз после выбора шашки
					if (!marked) {
						int x1, x2, y1, y2;
						if (pl == pFIRST) x2 = x1 = a + 1;
						else x2 = x1 = a - 1;
						y1 = b - 1;
						y2 = b + 1;
						variants.set(x1, y1, x2, y2);
						variantsType(pl, a, b);
						variants.checkTypes();
						curs.chosenX = a;
						curs.chosenY = b;
					}

					// если вариантов нет, то игнорирование выбора
					if (!variants.num) {
						curs.state = cHOVER;
						setState(1);
						marked = checked = false;
					}
					// если варианты есть, то дальнейший выбор хода
					else {
						if (!marked) {
							curs.x = variants.x1;
							curs.y = variants.y1;
							variants.nowType = variants.firstType;
						}

						marked = true;
						setState(2);
					}
					status.update(goon, moved, pl);
				}
			}
			// если был сделан выбор хода
			else {
				moved = true;
				// если выбран направление с убийством шашки оппонента
				// уменьшение количества его шашек
				if (variants.nowType == vJUMP) {
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

				// перемещение выбранной шашки
				if (pl == pFIRST) chek.setType(curs.x, curs.y, FIRST);
				else chek.setType(curs.x, curs.y, SECOND);
				chek.setType(curs.chosenX, curs.chosenY, NO);

				// переключение состояния курсора и передача хода
				curs.state = cHOVER;
				curs.putOnCenter();
				setState(1);
				if (pl == pFIRST) pl = pSECOND;
				else pl = pFIRST;
				checked = marked = false;
				// обнуление вариантов
				variants.set();
				// обновление статуса
				status.update(goon, moved, pl);
			}
			system("cls");
			// отрисовка
			show();
			// вывод информации
			status.print(goon);
			// перемещение курсора (пользователем)
			if (curs.state == cACTIVE) switchCursor();
			else curs.move();
			setState();
			int num1 = chek.getPlayer1State();
			int num2 = chek.getPlayer2State();
			// проверка статуса игры 
			// по количеству шашек у оппонентов
			goon = num1 && num2;
			//status.update(goon, moved, pl);
		}
		// печать статуса после завершения игры
		status.print(goon, start);
	}
};

auto main() -> int {
	// шашки на поле 8х8
	Game shashki(8);
	shashki.play();
}
