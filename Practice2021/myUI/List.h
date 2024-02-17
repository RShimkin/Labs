#pragma once
#include <list>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <algorithm>
#include "Students.h"

Student parse(std::string& str);
bool Fill_List_Based_On_File(std::string file_name, std::list<Student>& exams_info);
std::list<Student> Task2(std::string source_file_name);
std::list<Student> Task3(std::string source_file_name);
std::list<Student> makeBestList(std::string& fname);

