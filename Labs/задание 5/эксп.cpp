// Шимкин Р.Р. 62 группа ПММ ВГУ dev-cpp 5.11
// задание: вводится строка с рандомным количеством пробелов в начале, в конце и между словами (длина неизвестна) и К
// задание: циклически сдвинуть строку на К слов влево, убрав лишние пробелы 
// нельзя использовать string и оператор индексации 
// массив не должен превышать длину итоговой строки более чем на 10
#include <iostream>
#include <cstdlib>

using namespace std;

const stroka = "No idea    where,   but this  is          a mistake in  a program         .  ";

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
	for(;i>=0;i--)
		*(buf+i)= *(s+i);
	for(i=0; i<len(buf)+1; i++)
		for (int j=0; j<len(s)+1; j++)
			*(s+j)=*(s+j+1);
	*(s+len(s)) = ' ';
	for (int i=len(s), j=0; j<len(buf); i++,j++)
		*(s+i) = *(buf+j);
}

int main(){
	long l = 0;
    char ch;
	char *str = (char *)malloc(sizeof(char)); // создание массива минимальной начальной длины в куче
    printf("Enter string :\n");
    while((ch = cin.get()) != '\n')
    {
        *(str+l) = ch;
        str = (char *)realloc(str,(1 + (l++))*sizeof(char)); // добавление памяти 
    } 
    *(str+l) = '\0';
    cout << len(str);
    int k;
    cout << "Enter K: ";
    cin >> k;
    deleteSpace(str);
    cout << len(str);
    cout << "initial string with no spaces: " << str << endl;
    for (int i=0; i<k; i++)
    	move(str);
    cout << "Result string: " << str;
    delete[str];
	return 0;
}
