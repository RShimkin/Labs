#include <iostream> 
#include <string> 
 
using namespace std; 
 
int main() { 
   string s, ss; 
   int n = 0, p = 0; 
   cout << "string? "; 
   cin >> s; 
   cout << "substring? "; 
   cin >> ss; 
   while ( (p = s.find(ss, p)) != s.npos ) { 
     ++n; 
     ++p; 
   } 
   cout << "substring '" << ss << "' in string '" << s << " " << n <<  " times" << endl; 
   return 0; 
}
