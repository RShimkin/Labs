 #include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Node {
	int x;
	Node *left, *right;
};

typedef Node* TreePtr;

TreePtr temp = NULL;

void addNode(TreePtr& t, int value) {
	if (t == NULL) {
		t = new Node;
		t->x = value;
		t->left = NULL;
		t->right = NULL;
	}
	else if (value < t->x) addNode(t->left, value);
	else if (value > t->x) addNode(t->right, value);
}

void deleteNode(TreePtr& t, int value) {
	if (t) {
		if (value == t->x) {
			TreePtr parent = NULL;
			TreePtr min = t->right;
			while (min->left) {
				parent = min;
				min = min->left;
			}
			if (min->right) {
				if (parent) {
					parent->left = min->right;
					min->right = t->right;
					min->left = t->left;
					if (temp->right == t) temp->right = min;
					else temp->left = min;
					t->left = t->right = NULL;
					delete t;
				}
				else {
					min->left = t->left;
					if (temp->right == t) temp->right = min;
					else temp->left = min;
					t->left = t->right = NULL;
					delete t;
				}
			}
			else {
				if (parent) {
					min->right = t->right;
					parent->left = NULL;
				}
				min->left = t->left;
				t->left = t->right = NULL;
				TreePtr p = t;
				if (temp && (temp->right == p)) temp->right = min;
				else if (temp) temp->left = min;
				delete p;
			}
		}
		else if (value < t->x) {
			temp = t;
			deleteNode(t->left, value);
		}
		else {
			temp = t;
			deleteNode(t->right, value);
		}
	}
	else cout << "Element is not found!" << endl;
}

void preOrder(TreePtr& t) {
	if (t) {
		cout << t->x << " ";
		preOrder(t->left);
		preOrder(t->right);
	}
}

void postOrder(TreePtr& t) {
	if (t) {
		postOrder(t->left);
		postOrder(t->right);
		cout << t->x << " ";
	}
}

void inOrder(TreePtr t, int lvl = -1) {
	if (t) {
		if (lvl == -1) {
			inOrder(t->left);
			cout << t->x << " ";
			inOrder(t->right);
		}
		else {
			inOrder(t->right, lvl + 1);
			for (int i = 0; i < lvl; i++) cout << " ";
			cout << t->x << endl;
			inOrder(t->left, lvl + 1);
		}
	}
}

void deleteAll(TreePtr& t) {
	if (t) {
		if (t->left) deleteAll(t->left);
		if (t->right) deleteAll(t->right);
		t->left = NULL;
		t->right = NULL;
		delete t;
	}
}

void fromFile(string name, TreePtr& t) {
	ifstream file(name.c_str());
	if (!file.is_open()) {
		cout << "file is not opened!" << endl;
		return;
	}
	while (!file.eof()) {
		int value;
		file >> value;
		addNode(t, value);
	}
	file.close();
}

void input(TreePtr& t) {
	cout << "Enter the number of elements to insert: ";
	int num;
	cin >> num;
	for (int i = 0; i < num; i++) {
		int val;
		cin >> val;
		addNode(t, val);
	}
}

int main()
{
	Node* tree = NULL;
	inOrder(tree);
	input(tree);
	inOrder(tree, 0);
	int del;
	cin >> del;
	cout << endl;
	deleteNode(tree, del);
	inOrder(tree, 0);
	deleteAll(tree);
}
