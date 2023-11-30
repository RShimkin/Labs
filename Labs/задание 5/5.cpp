// ������ �.�. 62 ������ ��� ��� dev-cpp 5.11
// �������: �������� ������ � ��������� ����������� �������� � ������, � ����� � ����� ������� (����� ����������) � �
// �������: ���������� �������� ������ �� � ���� �����, ����� ������ ������� 
// ������ ������������ string � �������� ���������� 
// ������ �� ������ ��������� ����� �������� ������ ����� ��� �� 10
#include <iostream>
#include <cstdlib>

using namespace std;

int len(char* str){ // ����������� ������� ��� �������� ����� ������
	int i=0;
	for (;*(str+i)!='\0';i++);
	return i;
}

void deleteSpace(char* s){ // ������� ��� �������� ������ ��������
	// cout << len(s) << " ";
	while (*(s+0)==' '){  // ���� ������� ������� � ������ ������
		for (int i=0; i<len(s); i++)
			*(s+i)=*(s+i+1);
	}
	while (*(s+len(s)-1)==' ') // ���� ������� ������� � ����� ������
		*(s+len(s)-1)='\0';
	for(int i=0; i<len(s); i++){  // ��������� ������ ������� ����� �������
		while((*(s+i)==' ')&&(*(s+i+1)==' ')){
			for (int j=i; j<len(s); j++) 
				*(s+j)=*(s+j+1);
		}
	}
	// cout << len(s) << " ";
}

void move(char* s){ // ������� ������������ ������ �� 1 ����� �����
	int i;
	for(i=0; *(s+i)!=' '; i++);
	char* buf = new char[i];
	cout << "n: " << n << endl;
	for(;i>=0;i--)
		*(buf+i)= *(s+i);
	*(buf+len(buf))	
	for(i=0; i<len(buf); i++)
		for (int j=0; j<len(s)+1; j++)
			*(s+j)=*(s+j+1);
	*(s+len(s)) = ' ';
	for (int i=len(s), j=0; j<len(buf); i++,j++)
		*(s+i) = *(buf+j);
}

int main(){
	long l = 0;
    char ch;
	char *str = (char *)malloc(sizeof(char)); // �������� ������� ����������� ��������� ����� � ����
    printf("Enter string :\n");
    while((ch = cin.get()) != '\n')
    {
        *(str+l) = ch;
        str = (char *)realloc(str,(2 + (l++))*sizeof(char)); // ���������� ������ 
        *(str+l)=' ';
    } 
    *(str+l+1) = '\0';
    int k;
    cout << "Enter K: ";
    cin >> k;
    deleteSpace(str);
    cout << "initial string with no spaces: " << str << endl;
    for (int i=0; i<k; i++)
    	move(str);
    cout << "Result string: " << str;
    free(str);
	return 0;
}
