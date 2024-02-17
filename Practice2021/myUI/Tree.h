#pragma once
#include <string>
#include <fstream>

struct Node {
	std::string last_name = " ";
	double avg;
	Node* right, * left;
};
typedef Node* Tree;

bool IsEmpty(Tree t);
void Add_node(Tree& t, std::string f, double a);
void read_from_f(Tree& t, std::string path);
void reading_f(Tree& t, std::ifstream& file);