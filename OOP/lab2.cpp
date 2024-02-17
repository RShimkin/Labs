// Шимкин Р.Р. ПММ62 2 курс Лаба №2
/*
задание: реализовать программу, позволяющую пользователю отбивать бластерные выстрелы управляемым мышью и\или клавиатурой световым мечом; 
количество снарядов растёт с течением времени, после поражения требуется вывести на отдельной форме статистику: количество отражённых 
снарядов, количество пропущенных (критическое количество настраивается пользователем в начале), общее время игры
*/

#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <string>

using namespace std;

// макрос для вывода имени и значения переменной
#define watch(x) cout << (#x) << ": " << (x).toString() << endl 

// структура выстрела, содержит поля угла по вертикали, по горизонтали и скорость полета 
// а также конструктор от трех целых
struct Shot { 
    int vert;
    int hor;
    int sp;
    Shot(int x, int y, int s) {
        hor = x;
        vert = y;
        sp = s;
    }
};

// класс бластера с 2 полями: скорость и количество совершенных выстрелов
class Blaster {
    int speed;
    int count;

    // функция ускорения выстрелов до определенного значения
    void changeSpeed() {
        if (speed > 2000) speed -= speed * 0.1;
    }
public:
    // сеттер скорости
    void setSpeed(int speed) {
        this->speed = speed;
    }
    // сеттер счёта
    void setCount(int count) {
        this->count = count;
    }
    // геттер скорости
    int getSpeed(int speed) {
        return this->speed;
    }
    // геттер счёта
    int getCount(int speed) {
        return this->speed;
    }
    // конструктор от значения скорости
    Blaster(int speed) {
        this->speed = speed;
        count = 0;
    }
    // констуктор от значений скорости и счёта
    Blaster(int speed, int count) {
        this->speed = speed;
        this->count = count;
    }
    // конструктор по умолчанию (скорость 5000, счёт 0)
    Blaster() {
        speed = 5000;
        count = 0;
    }

    // функция выстрела
    Shot shoot() {
        srand(time(0));
        // х - рандомная переменная,
        // отвечающая за вертикальную полусферу выстрела
        int x = rand() % 2;
        int v; // v - угол по вертикали
        if (x == 0) v = rand() % 180 + 180;
        else v = rand() % 180;
        int h = rand() % 180 - 90; // угол по горизонтали
        Shot blast(v, h, speed); // создание структуры выстрела
        count++;
        // ускорение каждые два выстрела
        if (count % 2 == 0) changeSpeed();
        return blast;
    }
    // метод приведения к формату строки
    string toString() {
        return " speed: " + to_string(speed) + " count: " + to_string(count);
    }
};

// функция проверки ввода для использования в потоке
void checkInput(char& c) {
    c = _getch();
}

// класс игрока
class Gamer {
    int health;
    int speed;
    int width;
    int height;
    int h;
    int v;
    int score;
    int missed;
    // движение вправо на величину = скорости реакции
    void moveRight() {
        h = (h + speed) % 360;
    }
    // движение влево на величину = скорости реакции
    void moveLeft() {
        int temph = h - speed;
        h = (temph < 0) ? (360 + temph) : temph;
    }
    // движение вверх на величину = скорости реакции
    void moveUp() {
        int tempv = v + speed;
        v = (tempv > 90) ? 90 : tempv;
    }
    // движение вниз на величину = скорости реакции
    void moveDown() {
        int tempv = v - speed;
        v = (tempv < -90) ? -90 : tempv;
    }

    // метод изменения направления
    void Move(char c) {
        switch (c) {
        case 'D':
        case 'd': {
            moveRight();
            break;
        }

        case 'A':
        case 'a': {
            moveLeft();
            break;
        }

        case 'W':
        case 'w': {
            moveUp();
            break;
        }

        case 'S':
        case 's': {
            moveDown();
            break;
        }
        }
    }
public:
    // конструктор по умолчанию
    Gamer() {
        health = 3;
        speed = 20;
        width = 20;
        height = 40;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // конструктор от здоровья, скорости реакции, ширины и высоты меча
    Gamer(int health, int speed, int width, int height) {
        this->health = health;
        this->speed = speed;
        this->width = width;
        this->height = height;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // конструктор от здоровья
    Gamer(int health) {
        this->health = health;
        speed = 20;
        width = 20;
        height = 40;
        score = 0;
        missed = 0;
        h = 0;
        v = 0;
    }
    // сеттер направления
    void setVH(int h, int v) {
        this->h = h;
        this->v = v;
    }
    // сеттер счёта по умолчанию
    void setScore() {
        score = 0;
        missed = 0;
    }
    // геттер счёта
    int getScore() {
        return score;
    }
    // сеттер счёта 
    void setScore(int score) {
        this->score = score;
    }
    // геттер количества пропущенных
    int getMissed() {
        return missed;
    }
    // сеттер здоровья
    void setHealth(int value) {
        health = value;
    }
    // геттер здоровья
    int getHealth() {
        return health;
    }
    // сеттер размеров меча в градусах
    void setSize(int height, int width) {
        this->height = height;
        this->width = width;
    }
    // геттер высоты меча в градусах
    int getHeight() {
        return height;
    }
    // геттер ширины меча в градусах
    int getWidth() {
        return width;
    }
    
    // проверка, жив ли игрок
    bool isAlive() {
        return health > 0;
    }
    
    // метод приведения к строке класса игрока
    string toString() {
        string str;
        string shealth = " health: " + to_string(health);
        string sheight = " height: " + to_string(height);
        string swidth = " height: " + to_string(width);
        string sh = " height: " + to_string(h);
        string sv = " height: " + to_string(v);
        string sscore = " height: " + to_string(score);
        string smissed = " height: " + to_string(missed);
        str = shealth + sheight + swidth + sh + sv + sscore + smissed;
        return str;
    }
    // метод отражения выстрела
    void reject(Shot shot) {
        // начальные направления по горизонтали и вертикали
        h = 0, v = 0;
        clock_t begin = clock();
        clock_t now = clock();
        char ch;

        // пока текущее время меньше времени полета выстрела
        while ( (now - begin) < shot.sp ) {
            system("cls");
            // вывод информации о мече и возможностях игрока
            cout << "Your speed: " << speed << "    Sword size: " << width << " " << height << endl;
            now = clock();

            // вывод направления от текущего положения игрока до направления выстрела
            cout << "Shot: " << shot.hor - h << " " << shot.vert - v << " " << shot.sp - (now - begin) << endl;

            // ожидание пользовательского ввода в отдельном потоке в течение 2мс
            thread input(checkInput, ref(ch));
            Sleep(2);
            
            // прерывание потока
            input.detach();

            // изменение положения игрока
            Move(ch);
            ch = 'M';
        }
        system("cls");

        // проверка, отбил ли меч выстрел с учетом размеров меча
        if ((abs(shot.hor - h) < width) && ((abs(shot.vert - v)) < height)) {
            cout << "You've rejected it!";
            score++;
        }
        else {
            cout << "You're wounded!";
            health--;
            missed++;
        };

        // уменьшающаяся с ростом счёта пауза для отдыха
        Sleep(1000 - 40 * score);
    }

    // метод вывода статистики
    void Stat() {
        cout << " Your score is: " << score << endl;
        cout << " You missed: " << missed << endl;
    }
};


// основная функция игры
void play(Gamer& pl, Blaster& bl) {
        system("cls");
        cout << " #Press 2 to settle the health or any other button to skip";
        char ch = _getch();
        if (ch == '2') {
            cout << "\n #Enter new health value: ";
            int health;
            cin >> health;
            pl.setHealth(health);
        }

        // обнуление счёта
        pl.setScore();

        clock_t begin = clock();

        // пока игрок жив, отбивать новые выстрелы
        while (pl.isAlive()) {
            pl.reject(bl.shoot());
        }
        clock_t end = clock();
        system("cls");
        cout << "Player is dead..." << endl;

        // печать послеигровой статистики
        pl.Stat();
        cout << " time: " << (end - begin)/1000 << endl;
}

// функция для демонстрации работы геттеров, сеттеров и конструкторов
void demonstration() {
    Gamer player1;
    cout << "#1" << endl;
    watch(player1);
    cout << endl;

    Gamer player2(500, 50, 3, 3);
    cout << "#2" << endl;
    watch(player2);
    cout << endl;

    player1.setHealth(20);
    player1.setSize(1,100);
    
    cout << "#3" << endl;
    watch(player1);
    cout << endl;

    cout << "#4" << endl;
    player1.setVH(50, 100);
    watch(player1);
    cout << endl;

    Blaster blas1;
    cout << "#5" << endl;
    watch(blas1);
    cout << endl;

    cout << "#6" << endl;
    Blaster blas2(2000);
    watch(blas2);
    cout << endl;
}

int main()
{
    Gamer player;
    Blaster gun;

    //demonstration();

    play(player, gun);

    system("pause");
}
