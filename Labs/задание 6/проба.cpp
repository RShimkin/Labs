#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>                                //??? ?????????? ??????????? (??? ????? ???? ?????????? ????????????? ?? ?? ?????? ?????? ????? ?? ?????????? ?????).
using namespace std;
int main(){
     FILE *file = fopen("input.txt","r"); 
     if(file==NULL)                                   //???? in(????) ????? NULL,?????? ?? ?? ????? ?????????.???? ???? ?? ???.
     {
         cout<<"�� ������� ������� input.txt"<<endl;
     }
     	else
     	{
         	char str[64];       //??????? ??????(string) ???????? 64.
         	fgets(str,64,file);   //?????????? ? str ?????? ??????.
         	while(!feof(file)){
         		cout<<str;        //??????? ?? ????? ?????? ??????.
         		fgets(str,64,file); //?????????? ??????.
         	}
     	}
    file->close();
    return 0;
}
