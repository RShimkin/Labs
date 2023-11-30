// Шимкин Р.Р. 62 группа ПММ ВГУ dev-cpp 5.11
// задание: вводится строка с рандомным количеством пробелов в начале, в конце и между словами (длина неизвестна) и К
// задание: циклически сдвинуть строку на К слов влево, убрав лишние пробелы 
// нельзя использовать string и оператор индексации 
// массив не должен превышать длину итоговой строки более чем на 10
#include <iostream>
#include <cstdlib>

using namespace std;

int len(char* str){ // собственная функция для подсчёта длины строки
	int i=0;
	for (;*(str+i)!='\0';i++);
	return i;
}

void deleteSpace(char* s){ // функция для удаления лишних пробелов
	// cout << len(s) << " ";
	while (*(s+0)==' '){  // цикл удаляет пробелы в начале строки
		for (int i=0; i<len(s); i++)
			*(s+i)=*(s+i+1);
	}
	while (*(s+len(s)-1)==' ') // цикл удаляет пробелы в конце строки
		*(s+len(s)-1)='\0';
	for(int i=0; i<len(s); i++){  // удаляются лишние пробелы между словами
		while((*(s+i)==' ')&&(*(s+i+1)==' ')){
			for (int j=i; j<len(s); j++) 
				*(s+j)=*(s+j+1);
		}
	}
	// cout << len(s) << " ";
}

void move(char* s){ // функция циклического сдвига на 1 слово влево
	int i;
	for(i=0; *(s+i)!=' '; i++);
	char* buf = new char[i];
	for(int m=0; m<i; m++){
		*(buf+m)=*(s+0);
		for (int j=0; j<len(s)+1; j++)
			*(s+j)=*(s+j+1);
	}
	*(buf+i)='\0';
	for (int j=0; j<len(s)+1; j++)
			*(s+j)=*(s+j+1);
	*(s+len(s))=' ';
	i=len(s);
	for (int j=0; j<len(buf); i++,j++)
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
        	str = (char *)realloc(str,(2 + (l++))*sizeof(char)); // добавление памяти 
        	*(str+l)=' ';
    	} 
    	*(str+l+1) = '\0';
    	cout << "Enter K: ";
    	cin >> k;
	}
		else {
			char stroka[] = "1w 2ww          3w 5w.  6w6   77777  "; // демонстрационная строка
			str = (char*) &stroka;
			cout << "\nString: " << str <<endl;	
			k = 2;
			cout << "\nk=" << k << endl;
		}
    deleteSpace(str);
    cout << "initial string with no spaces: " << str << endl;
    for (int i=0; i<k; i++)
    	move(str);
    cout << "Result string: " << str;
    if(input )free(str);
    	else delete[] str;
	return 0;
}
