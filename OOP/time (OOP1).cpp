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
} Ttime;

class Time {
	private:
		Ttime T;
		void SetTime(Time time){
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
	    string ToString();
};

string Time::ToString(){
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

Time::Time(unsigned int x, unsigned int y, unsigned int z){
	T.hours=(x+y/60+z/3600)%24; 
	T.minutes=(y+z/60)%60; 
	T.seconds=z%60; 
}

Time::Time(string str){
	unsigned int hours = ((char)str[0]-'0')*10 + ((char)str[1]-'0');
	unsigned int minutes = ((char)str[3]-'0')*10 + ((char)str[4]-'0');
	unsigned int seconds = ((char)str[6]-'0')*10 + ((char)str[7]-'0');
	T.hours=hours%24;
	T.minutes=minutes;
	T.seconds=seconds;
}

Time::Time(unsigned int sec){
	unsigned int x = sec;
	T.hours=(x/3600)%24;
	x%=3600;
	T.minutes=x/60;
	x%=60;
	T.seconds=x;
}

Time::Time(Ttime time){
	T.hours=time.hours;
	T.minutes=time.minutes;
	T.seconds=time.seconds;
}

unsigned int Time::ToSeconds(){
	return T.hours*3600+T.minutes*60+T.seconds; 
}

unsigned int Time::ToSeconds(Ttime time){
	return time.hours*3600+time.minutes*60+time.seconds; 
}

unsigned int Time::ToMinutes(){
	unsigned int ost = T.seconds>30?1:0;
	unsigned int min = (T.minutes+ost)%60;
	return T.hours*60+T.minutes+ost;
}

bool Time::Compare(Ttime first, Ttime second){
	return ToSeconds(first)==ToSeconds(second)?true:false;
}

unsigned int Time::Difference(Ttime comp){
	return abs((int)ToSeconds(comp)-ToSeconds());
}

void Time::IncreaseSeconds(unsigned int sec){
	unsigned int sum = ToSeconds()+sec;
	Time New(sum);
	SetTime(New);
}

void Time::DecreaseSeconds(unsigned int sec){
	unsigned int sum = ToSeconds()-sec;
	Time New(sum);
	SetTime(New);
}

void Time::GetTime(){
	cout<<"Current value is "<<T.hours<<":"<<T.minutes<<":"<<T.seconds<<endl;
} 

int main(){
	string str="22:14:19";
	Time* ptr=new Time(49, 120, 61);
	/*cout<<"1 - numbers, 2 - string, 3 - seconds, 4 - time: ";
	int choice;
	cin >> choice;
	switch(choice){
		case 1:{
			unsigned int x,y,z;
			cin>>x>>y>>z;
			ptr=new Time(x,y,z);
			break;
		}
		case 2:{
			string s;
			Time New(s);
			ptr=&New;
			break;
		}
		case 3:{
			unsigned int scs;
			Time New(scs);
			ptr=&New;
			break;
		}
		case 4:{
			Ttime t;
			cin>>t.hours>>t.minutes>>t.seconds;
			Time New(t);
			ptr=&New;
			break;
		}
	} */
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
