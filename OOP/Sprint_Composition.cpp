// Шимкин Р.Р. ПММ 2 курс 62 группа
// реализовать симулятор спринтерского забега с исп-ем композиции классов
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <windows.h>

using namespace std;

string prefix = "runner";
float period = 0.5; // разбиение времени

class Contestant { // класс бегуна
private:
	string name;
	float injuryChance;
	bool injured, finished;
	float curSpeed, avgSpeed;
	float distance, curDistance;
	float quality;
	int place;
	float runtime;
	float tired, tiredKoef; // усталость и коэф-т роста усталости
	string info;
public:
	Contestant(int i, string str = "") {
		//srand(time(0));
		if (str == "") name = prefix + to_string(i); // рандомное имя
		else name = str;
		injuryChance = (float)(rand() % 100) / 100; // вероятность травмы (0 - 0.99)
		avgSpeed = ((float)(rand() % 3) + (float)(rand() % 100) / 100) + 7.0; // скорость (7.0 - 10.0 м/с)
		tired = (float)(rand() % 20) / 1000; // усталость (0 - 0.019)
		tiredKoef = tired / 10;
		injured = false;
	}
	string print() { // печать информации о бегуне в зав-ти от его сост-я (injured, finished или обычное)
		string one = "", two, three;
		two = name + " ";
		if (finished) {
			one = to_string(place) + " ";
			three = to_string(runtime) + " ";
			//cout << place << " " << name << " " << runtime << endl;
		}
		else if (injured) {
			one = "- ";
			three = "injured " + to_string(curDistance);
			//cout << "- " << name << " " << "injured " << curDistance << endl;
		}
		else {
			one = "? ";
			three = to_string(curDistance) + " " + to_string(runtime) + " " + to_string(curSpeed);
			//cout << "? " << name << " " << curDistance << " " << runtime << " " << curSpeed << endl;
		}
		string res = one + two + three;
		return res;
	}
	float getTime() const {
		return runtime;
	}
	void setPlace (int pos) {
		place = pos;
	}
	void ready(float dist = -1, float qual = -1) {
		// функция подготовки к новому забегу
		if (dist != -1) distance = dist;
		if (qual != -1) quality = qual;
		curSpeed = avgSpeed;
		curDistance = runtime = 0;
		finished = false;
		info = name + " speed: " + to_string(avgSpeed) + " tired: " + to_string(tired);
		if (injured) info += " injured";
	}
	string getInfo() {
		return info;
	}
	bool isInjured() const {
		return injured;
	}
	bool run() {
		// функция бега на текущем участке
		// возвращает true, если бегун получил травму или финишировал
		bool res = false;
		if (!injured && !finished) { // логика отрабатывает, если бегун ещё бежит
			curDistance += curSpeed * period; // изменение текущей дистанции
			if (curDistance >= distance) { // финиш, если текущая дист-я > общей
				finished = res = true;
				// точный расчёт времени последнего отрезка
				runtime += period - (curDistance - distance) / curSpeed; 
			}
			else { // если ещё не финиш, то просчитываем время, усталость, шанс травмы:
				runtime += period;
				// коэф-т вер-ти травмы, завис-ий от парам-ра бегуна, кач-ва трека и текущей дист-ии:
				float koef = curDistance / 1000 * (quality * injuryChance); 
				if (koef > 0.05) injured = res = true;
				// перерасчет текущей скор-ти: (1 - уст*текДист/1000)
				curSpeed *= (1 - tired * curDistance / 10000);
				tired += tiredKoef;
			}
		}
		return res;
	}
};

class Track { // класс трека
	float distance;
	float quality;
	string info;
public:
	float getDistance() {
		return distance;
	}
	float getQuality() {
		return quality;
	}
	// функция настройки парам-ов трека 
	string getInfo() {
		return info;
	}
	void set(float dist = -1, float qual = -1) {
		srand(time(0));
		if (qual == -1) quality = (float)(rand() % 6) / 10;
		else quality = qual;
		if (dist == -1) {
			int x = rand() % 4;
			switch (x) {
			case 0:
				distance = 60;
				break;
			case 1:
				distance = 100;
				break;
			case 2:
				distance = 200;
				break;
			case 3:
				distance = 400;
				break;
			}
		}
		else distance = dist;
		info = "track: " + to_string(distance) + " " + to_string(quality);
	}
};

class Sprint { // главный класс забега
private:
	Track track; // трек
	vector<Contestant> runner; // массив бегунов
	int num, running;
	int pos;
public:
	void printInfo() {
		for (auto x : runner)
			cout << x.getInfo() << endl;
	}
	void Print() {
		vector<string> str;
		for (auto it = runner.begin(); it != runner.end(); it++) {
			str.push_back(it->print());
		}
		cout << track.getInfo() << endl;
		for (auto x : str)
			cout << x << endl;
	}
	int countRunners() { // функция подсчета способных бежать бегунов
		int res = 0;
		for (auto x : runner)
			if (!x.isInjured()) res++;
		return res;
	}
	void Run() {
		running = countRunners(); 
		pos = 1; // место для первого финишера 
		float dist = track.getDistance();
		float qual = track.getQuality();
		// установка парам-ов всех бегунов:
		for (auto it = runner.begin(); it != runner.end(); it++) it->ready(dist, qual); 
		printInfo();
		_getch();
		for (int i = 1; running; i++) { // основной цикл
			// map для сорт-катех, кто финишировал на данном отрезке (их м.б. несколько):
			map<float, int> finishers; 
			for (int k = 0; k < num; k++) {
				if (runner[k].run()) { // если кто-то травмировался или финишировал, то
					running--; // уменьшаем число бегущих
					if (!runner[k].isInjured()) // если не травмирован, то добавляем в map
						finishers.insert(make_pair(runner[k].getTime(), k));
				}
			}
			if (finishers.size()) { // распределение мест новым финишерам 
				for (auto it = finishers.begin(); it != finishers.end(); it++)
					runner[it->second].setPlace(pos++);
			}
			system("cls");
			Print(); // печать инф-ии бегунов
		}
	}
	void TrackMenu() { // меню настройки трека
		cout << "Would you like to set the track? (y/n)" << endl;
		char x = _getch();
		if (x == 'y' || x == 'Y') {
			float dist, qual;
			cout << "distance: ";
			cin >> dist;
			cout << endl << "quality (0 - perfect, 1 - awful): ";
			cin >> qual;
			track.set(dist, qual);
		}
		else if (x == 'n' || x == 'N') track.set();
	}
	void SprintersMenu() { // меню настройки бегунов
		cout << endl << "How many sprinters? ";
		cin >> num;
		cout << endl << "Press 1 to set names or 2 for random names";
		char x = _getch();
		if (x == '1') {
			cout << "names: " << endl;
			string name;
			for (int i = 1; i <= num; i++) {
				cin >> name;
				cout << endl;
				runner.push_back(Contestant(i, name));
			}
		}
		else if (x == '2') {
			for (int i = 1; i <= num; i++)
				runner.push_back(Contestant(i));
		}
	}
	void Menu() { // основное меню забегов
		bool goon = true;
		TrackMenu();
		SprintersMenu();
		while (goon) { // основной цикл
			cout << endl << "Press any key to start the race\n";
			char x = _getch();
			Run();
			cout << "Press 1 to quit, 2 to continue or 3 to change sprinters";
			x = _getch();
			if (x == '1') goon = false;
			else if (x == '2') TrackMenu();
			else if (x == '3') {
				TrackMenu();
				SprintersMenu();
			}
		}
	}
};

int main()
{
	Sprint race;
	race.Menu();
}
