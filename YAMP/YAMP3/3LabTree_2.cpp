// удвоить четные элементы дерева
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>

using namespace std;

string message[] = {
	" # Enter q/Q to quit",
	" # Enter 0 to clean the console",
	" # Enter 1 for file input",
	" # Enter 2 for manual input",
	" # Enter 3 for preorder print",
	" # Enter 4 for inorder print",
	" # Enter 5 to show the tree",
	" # Enter 6 for postorder print",
	" # Enter 7 to double all even elements",
	" # Enter 8 to delete a node",
	" # Enter 9 to delete all",
	" # Enter s to search the element"
};

struct Node {
	int x;
	Node* left, * right;
};

typedef Node* TreePtr;

bool search(TreePtr& t, int val) {
	bool res = false;
	if (t) {
		if (t->x == val) res = true;
		else if (val < t->x) res = search(t->left, val);
		else res = search(t->right, val);
	}
	return res;
}

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
	static TreePtr temp = NULL;
	if (t) {
		if (value == t->x) {
			if (temp) {
				if (!t->left && !t->right) {
					TreePtr p = t;
					if (temp->right == t) temp->right = NULL;
					else temp->left = NULL;
					delete p;
				}
				else if (t->left && t->right) {
					TreePtr parent = NULL;
					TreePtr min = t->right;
					while (min->left) {
						parent = min;
						min = min->left;
					}
					if (min->right) {
						if (parent) {
							TreePtr p = t;
							parent->left = min->right;
							min->right = t->right;
							min->left = t->left;
							if (temp->right == t) temp->right = min;
							else temp->left = min;
							p->left = p->right = NULL;
							delete p;
						}
						else {
							TreePtr p = t;
							min->left = t->left;
							if (temp->right == t) temp->right = min;
							else temp->left = min;
							p->left = p->right = NULL;
							delete p;
						}
					}
					else {
						TreePtr p = t;
						if (parent) {
							min->right = t->right;
							parent->left = NULL;
						}
						min->left = t->left;
						p->left = p->right = NULL;
						if (temp->right == t) temp->right = min;
						else temp->left = min;
						delete p;
					}
				}
				else if (t->left) {
					TreePtr p = t;
					if (temp->right == t) temp->right = t->left;
					else temp->left = t->left;
					p->left = NULL;
					delete p;
				}
				else {
					TreePtr p = t;
					if (temp->right == t) temp->right = t->right;
					else temp->left = t->right;
					p->right = NULL;
					delete p;
				}
			}
			else {
				if (!t->left && !t->right) {
					TreePtr p = t;
					t = NULL;
					delete p;
				}
				else if (t->left && t->right) {
					TreePtr parent = NULL;
					TreePtr min = t->right;
					while (min->left) {
						parent = min;
						min = min->left;
					}
					if (min->right) {
						if (parent) {
							TreePtr p = t;
							parent->left = min->right;
							min->right = t->right;
							min->left = t->left;
							p->left = p->right = NULL;
							t = min;
							delete p;
						}
						else {
							TreePtr p = t;
							min->left = t->left;
							p->left = p->right = NULL;
							t = min;
							delete p;
						}
					}
					else {
						TreePtr p = t;
						if (parent) {
							min->right = t->right;
							parent->left = NULL;
						}
						min->left = t->left;
						p->left = p->right = NULL;
						t = min;
						delete p;
					}
				}
				else if (t->left) {
					TreePtr p = t;
					t = t->left;
					p->left = NULL;
					delete p;
				}
				else {
					TreePtr p = t;
					t = t->right;
					p->right = NULL;
					delete p;
				}
			}

			
		}
		else if (value < t->x) {
			temp = t;
			deleteNode(t->left, value);
			temp = NULL;
		}
		else {
			temp = t;
			deleteNode(t->right, value);
			temp = NULL;
		}
	}
	else {
		cout << "Element is not found!" << endl;
		temp = NULL;
	}
		
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
			cout << "|" << t->x << endl;
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
		t = NULL;
	}
}

void fromFile(TreePtr& t) {
	cout << "Enter the file name: ";
	string name;
	cin >> name;
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
	cout << "File is read!" << endl;
}

void input(TreePtr& t) {
	cout << "Enter the number of elements to insert: ";
	int num;
	cin >> num;
	cout << "Elements: " << endl;
	for (int i = 0; i < num; i++) {
		int val;
		cin >> val;
		addNode(t, val);
	}
	cout << "Elements are added!" << endl;
}

void doubleEven(TreePtr& t) {
	if (t) {
		doubleEven(t->left);
		if (t->x % 2 == 0) t->x *= 2;
		doubleEven(t->right);
	}
}

TreePtr swap(TreePtr& t) {
	if (t) {
		TreePtr root = new Node;
		root->left = root->right = t;
		TreePtr min = root, max = root;
		while (min->left->left) min = min->left;
		while (max->right->right) max = max->right;
		if (min != max) {
			TreePtr temp = min->left;
			min->left = max->right;
			max->right = temp;
			temp = temp->right;
			max->right->left = min->left->left;
			min->left->right = temp;
		}
	}
	return t;
}

bool menuTree(TreePtr& tree) {
	bool deleted = false;
	for (int i = 0; i < 12; i++) cout << message[i] << endl;
	char x = '+';
	int temp = 0;
	while ((x != 'q') && (x != 'Q')) {
		x = _getch();
		switch (x) {
		case '0':
			system("cls");
			for (int i = 0; i < 11; i++) cout << message[i] << endl;
			break;
		case 'q':
		case 'Q':
			system("cls");
			break;
		case '1':
			fromFile(tree);
			break;
		case '2':
			input(tree);
			inOrder(tree, 0);
			swap(tree);
			cout << endl;
			inOrder(tree, 0);
			break;
		case '3':
			preOrder(tree);
			cout << endl;
			break;
		case '4':
			inOrder(tree);
			cout << endl;
			break;
		case '5':
			inOrder(tree, 0);
			break;
		case '6':
			postOrder(tree);
			cout << endl;
			break;
		case '7':
			doubleEven(tree);
			break;
		case '8':
			cout << "Which element to delete? ";
			cin >> temp;
			deleteNode(tree, temp);
			break;
		case '9':
			deleteAll(tree);
			deleted = true;
			break;
		case 's':
		case 'S':
			cout << "Enter the number to search: ";
			int num;
			cin >> num;
			if (search(tree, num)) cout << "element is in the tree" << endl;
			else cout << "no such element in the tree " << endl;
			break;
		}
	}
	return deleted;
}

int main()
{
	Node* tree = NULL;
	bool check = menuTree(tree);
	if (!check) deleteAll(tree);
}
