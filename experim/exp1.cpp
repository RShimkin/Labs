#include <iostream>

int main(){

int a=1;
++a=++(++a) + ++(--a);
std::cout<<++a;
return 0;
}
