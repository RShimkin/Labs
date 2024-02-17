#include "Tree.h"

bool IsEmpty(Tree t) {
	return t == NULL;
}

void Add_node(Tree& t, std::string f, double a) {
	Tree p = new Node;
	p->last_name = f;
	p->avg = a;
	p->right = p->left = NULL;
	if (IsEmpty(t)) t = p;
	else {
		if (a < t->avg) {
			if (t->left != NULL) Add_node(t->left, f, a);
			else t->left = p;
		}
		else {
			if (t->right != NULL) Add_node(t->right, f, a);
			else t->right = p;
		}
	}
}

void read_from_f(Tree& t, std::string path) {
	std::ifstream fin;
	fin.open(path);
	if (!fin.is_open()) 
		System::Windows::Forms::MessageBox::Show("Не удалось открыть файл", "Ошибка");
	else 
		if (fin.peek() != EOF) reading_f(t, fin);
	fin.close();
}

void reading_f(Tree& t, std::ifstream& file) {
	std::string f,d;
	double average;
	while (!file.eof()) {
		file >> d;
		file >> f;
		double n = 0;
		for (int i = 0; i < 5; i++) {
			int k;
			file >> k;
			n = n + k;
		}
		average = n / 5;
		Add_node(t, f, average);
	}
}
