#include <iostream>
#include <cstdlib>

using namespace std;

const char stroka[] = " Hi iH  "; //No idea    where,   but this  is          a mistake in  a program         .  ";
const int n=9;   //  78;

int len(char* str){ // собственна€ функци€ дл€ подсчЄта длины строки
	int i=0;
	for (;*(str+i)!='\0';i++);
	return i;
}

void deleteSpace(char* s){ // функци€ дл€ удалени€ лишних пробелов
	// cout << len(s) << " ";
	while (*(s+0)==' '){  // цикл удал€ет пробелы в начале строки
		for (int i=0; i<len(s); i++)
			*(s+i)=*(s+i+1);
	}
	while (*(s+len(s)-1)==' ') // цикл удал€ет пробелы в конце строки
		*(s+len(s)-1)='\0';
	for(int i=0; i<len(s); i++){  // удал€ютс€ лишние пробелы между словами
		while((*(s+i)==' ')&&(*(s+i+1)==' ')){
			for (int j=i; j<len(s); j++) 
				*(s+j)=*(s+j+1);
		}
	}
	// cout << len(s) << " ";
}

void move(char* s){ // функци€ циклического сдвига на 1 слово влево
	int i;
	for(i=0; *(s+i)!=' '; i++);
	char* buf = new char[i];
	int m=i;
	for(;i>=0;i--)
		*(buf+i)= *(s+i);
	*(buf+m)='\0';
	for(i=0; i<len(buf); i++)
		for (int j=0; j<len(s)+1; j++)
			*(s+j)=*(s+j+1);
	*(s+len(s)) = ' ';
	for (int i=len(s), j=0; j<len(buf); i++,j++)
		*(s+i) = *(buf+j);
}

int main(){
	cout << " Hand input (1) or auto (0) ? ";
	bool input;
	cin >> input;
	char* str;
	int k;
	if (input){
		long l = 0;
    	char ch;
		str = (char *)malloc(sizeof(char)); // создание массива минимальной начальной длины в куче
    	cout << "\nEnter string :\n";
    	cin.ignore();
    	while((ch = cin.get()) != '\n'){
        	*(str+l) = ch;
        	str = (char *)realloc(str,(2 + (l++))*sizeof(char)); // добавление пам€ти 
        	*(str+l)=' ';
    	} 
    	*(str+l+1) = '\0';
    	cout << "Enter K: ";
    	cin >> k;
	}
		else {
			str = new char[20];
			str = (char*)"Hi iH    Hu  uhhh\0";
			cout << "\nString: " << str <<endl;	
			k = 2;
		}
    deleteSpace(str);
    cout << "initial string with no spaces: " << str << endl;
    cout << "k=" << k << endl;
    for (int i=0; i<k; i++)
    	move(str);
    cout << "Result string: " << str;
    free(str);
	return 0;
}
