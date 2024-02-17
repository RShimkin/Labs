// Шимкин ПММ62 лаба №1
// нужно реализовать класс для работы со временем, оснастив его необходимыми методами, несколькими констукторами и методом ToString
#include<iostream>
#include<string>
#include<math.h>
#include<sstream>
#include<stdlib.h>

using namespace std;

typedef struct {
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
} Ttime; // структура для хранения часов, минут и секунд

class Time {  
	private:
		Ttime T;
		void SetTime(Time time){ // примитивный аналог конструктора копирования, о котором тогда я ещё не знал
			T.hours=time.T.hours;
			T.seconds=time.T.seconds;
			T.minutes=time.T.minutes;
		}
	public:
		Time(unsigned int x, unsigned int y, unsigned int z);
		Time(string str);
		Time(unsigned int sec);
		Time(Ttime time);
		unsigned int ToSeconds();
		unsigned int ToSeconds(Ttime time);
	    unsigned int ToMinutes();
	    bool Compare(Ttime first, Ttime second);
	    unsigned int Difference(Ttime comp);
	    void DecreaseSeconds(unsigned int sec);
	    void IncreaseSeconds(unsigned int sec);
	    void GetTime();
	    string ToString(); // метод, возвращающий строку с хранящимся в объекте временем
};

string Time::ToString(){ // перевод целых значений в строки, так уж получилось, реализован через промежуточные потоки
	string str,s1,s2,s3;
	stringstream stream1, stream2, stream3;
	stream1<<T.hours;
	stream1>>s1;
	stream2<<T.minutes;
	stream2>>s2;
	stream3<<T.seconds;
	stream3>>s3;
	str=s1+":"+s2+":"+s3;
	return str;
} 

Time::Time(unsigned int x, unsigned int y, unsigned int z){ // конструктор от 3 целых значений
	// инициализация полей структуры с учетом пределов возможных значений часов, минут и секунд
	T.hours=(x+y/60+z/3600)%24; 
	T.minutes=(y+z/60)%60; 
	T.seconds=z%60; 
}

Time::Time(string str){ // конструктор от строки, содержащей часы:минуты:секунды
	unsigned int hours = ((char)str[0]-'0')*10 + ((char)str[1]-'0'); // посимвольный перевод первых 2 цифр в целые часы
	unsigned int minutes = ((char)str[3]-'0')*10 + ((char)str[4]-'0'); // посимвольный перевод вторых 2 цифр в целые минуты
	unsigned int seconds = ((char)str[6]-'0')*10 + ((char)str[7]-'0'); // посимвольный перевод последних 2 цифр в целые секунды
	// далее инициализация полей структуры
	T.hours=hours%24;
	T.minutes=minutes;
	T.seconds=seconds;
}

Time::Time(unsigned int sec){ // конструктор от значения времени в секундах
	unsigned int x = sec;
	T.hours=(x/3600)%24;
	x%=3600;
	T.minutes=x/60;
	x%=60;
	T.seconds=x;
}

Time::Time(Ttime time){ // конструктор от структуры времени
	T.hours=time.hours;
	T.minutes=time.minutes;
	T.seconds=time.seconds;
}

unsigned int Time::ToSeconds(){ // простейшая функция перевода внутреннего времени в секунды
	return T.hours*3600+T.minutes*60+T.seconds; 
}

unsigned int Time::ToSeconds(Ttime time){ // // аналогичная функция перевода времени, хранимого в переданной структуре, в секунды
	return time.hours*3600+time.minutes*60+time.seconds; 
}

unsigned int Time::ToMinutes(){ // функция перевода внутреннего времени в минуты
	unsigned int ost = T.seconds>30?1:0; // в ost хранится 1, если округлить секунды нужно будет в большую сторону минут
	unsigned int min = (T.minutes+ost)%60;
	return T.hours*60+T.minutes+ost;
}

bool Time::Compare(Ttime first, Ttime second){ // проверка на равенство времени двух передаваемых структур через перевод в секунды
	return ToSeconds(first)==ToSeconds(second)?true:false;
}

unsigned int Time::Difference(Ttime comp){ // функция рассчета модуля разницы в секундах между внутренним и передаваемым временем
	return abs((int)ToSeconds(comp)-ToSeconds());
}

void Time::IncreaseSeconds(unsigned int sec){ // функция увеличения внутреннего времени на sec секунд
	unsigned int sum = ToSeconds()+sec; // в sum сохраняется итоговое время в секундах
	Time New(sum); // создается промежуточный объект
	SetTime(New); // вызывается аналог конструктора копирования
}

void Time::DecreaseSeconds(unsigned int sec){ // аналогичная функция уменьшения внутреннего времени на sec секунд 
	unsigned int sum = ToSeconds()-sec;
	Time New(sum);
	SetTime(New);
}

void Time::GetTime(){ // функция печати времени в формате часы:минуты:секунды
	cout<<"Current value is "<<T.hours<<":"<<T.minutes<<":"<<T.seconds<<endl;
} 

int main(){
	string str="22:14:19";
	Time* ptr=new Time(49, 120, 61);
	Ttime temp;
	temp.hours=2; temp.seconds=1; temp.minutes=1;
	cout<<ptr->Difference(temp);
	cout<<ptr->ToString(); 
	ptr->GetTime(); 
	ptr->IncreaseSeconds(97200);
	ptr->GetTime();
	cin.get();
	return 0;
}
