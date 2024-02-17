#include "AVLTree.h"

bool IsEmpty(ATree t) {
	return t == NULL;
}

int getHeight(ATree t) {
	int h = 0;
	if (t) h = t->height;
	return h;
}

void fixNodeHeight(ATree t) {
	int leftHeight = getHeight(t->left);
	int rightHeight = getHeight(t->right);
	t->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

ATree makeRightSmallRotate(ATree t) {
	ATree newTree = t->left;
	t->left = newTree->right;
	newTree->right = t;
	fixNodeHeight(t);
	fixNodeHeight(newTree);
	return newTree;
}

ATree makeLeftSmallRotate(ATree t) {
	ATree newTree = t->right;
	t->right = newTree->left;
	newTree->left = t;
	fixNodeHeight(t);
	fixNodeHeight(newTree);
	return newTree;
}

int getBalanceFactor(ATree t) {
	return getHeight(t->right) - getHeight(t->left);
}

ATree balanceTree(ATree t) {
	fixNodeHeight(t);
	if (getBalanceFactor(t) == 2) {//big left rotate
		if (getBalanceFactor(t->right) < 0) t->right = makeRightSmallRotate(t->right);
		return makeLeftSmallRotate(t);
	}
	if (getBalanceFactor(t) == -2) {//big right rotate
		if (getBalanceFactor(t->left) > 0)t->left = makeLeftSmallRotate(t->left);
		return makeRightSmallRotate(t);
	}
	return t;
}

ATree findMinimalKeyInSubTree(ATree t) {
	return t->left ? findMinimalKeyInSubTree(t->left) : t;
}

ATree Add_node(ATree& t, std::string f, double a) {
	ATree p = new ANode;
	p->last_name = f;
	p->avg = a;
	p->right = p->left = NULL;
	p->height = 1;
	if (IsEmpty(t)) t = p;
	else {
		if (a < t->avg) t->left = Add_node(t->left, f, a);
		else t->right = Add_node(t->right, f, a);
	}
	return balanceTree(t);
}

void read_from_f(ATree& t, std::string path) {
	std::ifstream fin;
	fin.open(path);
	if (!fin.is_open()) {
		System::Windows::Forms::MessageBox::Show("Не удалось открыть файл", "Ошибка");
	}
	else {
		if (fin.peek() != EOF) reading_f(t, fin);
	}
	fin.close();
}

void reading_f(ATree& t, std::ifstream& file) {
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
		t = Add_node(t, f, average);
	}
}