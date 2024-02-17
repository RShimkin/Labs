#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <thread>

using namespace std;

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

class Blaster {
    int speed;
    int count;

    void changeSpeed() {
        if (speed > 100) speed -= speed * 0.4;
    }

public:
    Blaster() {
        speed = 2000;
        count = 0;
    }
    Shot shoot() {
        int v = rand() % 360;
        int h = rand() % 180 - 90;
        Shot blast(v, h, speed);
        count++;
        if (count % 1 == 0) changeSpeed();
        return blast;
    }
};

void checkTime(clock_t& timer) {
    timer = clock();
}

void checkInput(char& c) {
    c = _getch();
}

class Gamer {
    int health;
    int speed;
    int width;
    int height;
    int h;
    int v;
    int score;
    void moveRight() {
        h = (h + speed) % 360;
    }
    void moveLeft() {
        int temph = h - speed;
        h = (temph < 0) ? (360 + temph) : temph;
    }
    void moveUp() {
        int tempv = v + speed;
        v = (tempv > 90) ? 90 : tempv;
    }
    void moveDown() {
        int tempv = v - speed;
        v = (tempv < -90) ? -90 : tempv;
    }

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
    Gamer() {
        health = 3;
        speed = 20;
        width = 20;
        height = 40;
        score = 0;
        h = 0;
        v = 0;
    }
    void setScore() {
        score = 0;
    }
    int getScore() {
        return score;
    }
    void setHealth(int value) {
        health = value;
    }
    int getHealth() {
        return health;
    }
    bool isAlive() {
        return health > 0;
    }
    void reject(Shot shot) {
        h = 0, v = 0;
        clock_t begin = clock();
        clock_t now = clock();
        char ch;
        while ( (now - begin) < shot.sp ) {
            system("cls");
            cout << "Your speed: " << speed << "    Sword size: " << width << " " << height << endl;
            now = clock();
            cout << "Shot: " << shot.hor - h << " " << shot.vert - v << " " << shot.sp - (now - begin) << endl;
            thread input(checkInput, ref(ch));
            Sleep(5);
            input.detach();
            Move(ch);
        }
        system("cls");
        if ((abs(shot.hor - h) < width) && ((abs(shot.vert - v)) < height)) {
            cout << "You've rejected it!";
            score++;
        }
        else {
            cout << "You're wounded!";
            health--;
        };
        Sleep(1000 - 40 * score);
    }
};

void Stat(Gamer& pl) {
    cout << " Your score is: " << pl.getScore() << endl;
}

void play(Gamer& pl, Blaster& bl) {
    cout << " #Press 2 to settle the health or any other button to skip";
    char ch = _getch();
    if (ch == 2) {
        cout << " #Enter new health value: ";
        int health;
        cin >> health;
        pl.setHealth(health);
    }

    pl.setScore();

    while (pl.isAlive()) {
        pl.reject(bl.shoot());
    }
    cout << "Player is dead..." << endl;
    Stat(pl);
}

int main()
{
    Gamer player;
    Blaster gun;

    play(player, gun);

    system("pause");
}