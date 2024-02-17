#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <assert.h>
#include <math.h>

using namespace std;

struct NODE {
	NODE(int, int, string, int);
	int sup_account_number;
	int cl_account_number;
	string purpose;
	int sum;
	NODE* next;
	NODE* prev;
};

typedef NODE* Tlist;

class Payments {
	int count;
	Tlist head;
	Tlist tail;
public:
	void Menu();
	Payments();
	~Payments();
	void searchPurpose(string);
	void sortSum();
	void printSupplier(int);
	void addToHead(int, int, string, int);
	void addToTail(int, int, string, int);
	void del(Tlist&);
	void delFromHead();
	void delFromTail();
	void clearList();
	void printList();
	void swapNodes(Tlist&, Tlist&);
	void changeNode(Tlist&);
	void readFromFile(ifstream&);
	void writeToFile(ofstream&);
	Tlist& getElemPtr(int);
	//int Count();
private:
	void printNode(Tlist&);
	void insertAfter(Tlist&, Tlist&, bool);
	bool Empty();
};


NODE::NODE(int san, int can, string pur, int s) {
	sup_account_number = san;
	cl_account_number = can;
	purpose = pur;
	sum = s;
}

void Payments::Menu() {
	string message[] = {
	" #Enter 0 to exit",
	" #Enter 1 for input from file",
	" #Enter 2 to manual input",
	" #Enter 3 to delete elements",
	" #Enter 4 to print the list",
	" #Enter 5 to count elements",
	" #Enter 6 to search payments by purpose",
	" #Enter 7 to print payments by supplier",
	" #Enter 8 to sort the list (by sum)",
	" #Enter 9 to change elements",
	" #Enter 10 to write to file",
	" #Enter 11 to clear the list",
	" #Enter -1 to clear the console",
	};

	int choice;
	bool goon = true;
	for (auto& str : message)
		cout << str << endl;

	while (goon) {
		cout << "----------------------------------------------------------";
		cout << "   Your choice: ";
		cin >> choice;

		switch (choice) {
		case -1: {
			system("cls");
			for (auto& str : message)
				cout << str << endl;
			break;
		}

		case 0: {
			goon = false;
			break;
		}

		case 1: {
			cout << "Enter the file name: ";
			string name;
			cin >> name;

			ifstream input(name.c_str());
			if (input.is_open()) {
				readFromFile(input);
				cout << "The file is read" << endl;
				input.close();
			}
			else cout << "The file is not opened" << endl;

			break;
		}

		case 2: {
			cout << "Enter 1 for input to head or 2 for input to tail: ";
			int pos;
			cin >> pos;

			cout << "Enter the number of elements to add";
			int num;
			cin >> num;

			for (int i = 0; i < num; i++) {
				int cl;
				int sup;
				string purpose;
				int sum;
				cout << "Enter supplier, client, purpose and sum: ";
				cin >> sup >> cl >> purpose >> sum;
				if ((!count) || (pos == 1)) addToHead(abs(sup), abs(cl), purpose, abs(sum));
				else if (pos == 2) addToTail(sup, cl, purpose, sum);
			}
			break;
		}

		case 3: {
			cout << "Enter 1 to delete from head, 2 to delete from tail ";
			cout << "or 3 to delete from particular position: ";
			int place;
			cin >> place;
			while ((place > 3) || (place < 1)) {
				cout << "Wrong input!";
				cin >> place;
			};

			cout << "Enter the number of elements to delete: ";
			int num;
			cin >> num;

			if (place == 3) {
				for (int i = 0; i < num; i++) {
					cout << "Enter the position of the element: ";
					int pos;
					cin >> pos;
					if (pos == 1) delFromHead();
					else if (pos == count) delFromTail();
					else if (pos < count) {
						Tlist p = getElemPtr(pos);
						del(p);
					}
					else cout << "Wrong position! Only " << count << " elements in the list" << endl;
				}
			}
			else {
				for (int i = 0; i < num; i++) {
					if (place == 1) delFromHead();
					else if (place == 2) delFromTail();
				}
			}
			break;
		}

		case 4: {
			printList();
			break;
		}

		case 5: {
			cout << count << " elements in the lust" << endl;
			break;
		}

		case 6: {
			cout << "Enter the puspose: ";
			string pur;
			cin >> pur;
			searchPurpose(pur);
			break;
		}

		case 7: {
			cout << "Enter supplier: ";
			int sup;
			cin >> sup;
			printSupplier(sup);
			break;
		}

		case 8: {
			sortSum();
			break;
		}

		case 9: {
			cout << "Enter the position of element to change: ";
			int pos;
			cin >> pos;
			while ((pos < 1) || (pos > count)) {
				cout << "Wrong position!";
				cin >> pos;
			}

			Tlist p = getElemPtr(pos);
			changeNode(p);
			break;
		}

		case 10: {
			cout << "Enter the name of the file: ";
			string name;
			cin >> name;
			ofstream output(name.c_str());
			if (output.is_open()) {
				writeToFile(output);
				cout << "The file is written" << endl;
				output.close();
			}
			else cout << "Error! The file is not written";

			break;
		}

		case 11: {
			clearList();
			break;
		}
		}
		cout << "----------------------------------------------------------" << endl;
	}
}

/*int Payments::Count() {
	return count;
}*/

Tlist& Payments::getElemPtr(int pos) {
	Tlist p = head;
	for (int i = 1; i < pos; i++)
		p = p->next;
	return p;
}

void Payments::del(Tlist& p) {
	p->prev->next = p->next;
	p->next->prev = p->prev;
	p->next = NULL;
	p->prev = NULL;
	printNode(p);
	cout << " deleted" << endl;
	delete p;
	count--;
}

void Payments::sortSum() {
	if (count > 2) {
		Tlist p = head->next;
		while (p != NULL) {
			Tlist it = NULL;
			if (p->prev->sum <= p->sum) {
				p = p->next;
				continue;
			}
			else it = p->prev->prev;

			bool istail = (tail == p) ? true : false;

			while (it != NULL) {
				if (it->sum > p->sum) it = it->prev;
				else {
					if (istail) tail = p->prev;
					Tlist next = p->next;
					insertAfter(it, p, istail);
					p = next;
					break;
				}
			}
			if (it == NULL) {
				if (istail) tail = p->prev;
				Tlist next = p->next;
				p->prev->next = p->next;
				p->next->prev = p->prev;
				p->next = head;
				p->prev = NULL;
				head->prev = p;
				head = p;
				p = next;
			}
		}
		cout << "list has been sorted" << endl;
	}
	else if (count > 0) {
		if (head->sum > tail->sum) {
			head->prev = tail;
			head->next = NULL;
			tail->prev = NULL;
			tail->next = head;
			Tlist temp = head;
			head = tail;
			tail = temp;
			cout << "list has been sorted" << endl;
		}
	}
	else cout << "The list is empty, there is nothing to sort!" << endl;
}

void Payments::insertAfter(Tlist& after, Tlist& node, bool istail) {
	node->prev->next = node->next;
	if (!istail) node->next->prev = node->prev;
	node->next = after->next;
	after->next = node;
	node->prev = after;
	node->next->prev = node;
}

void Payments::writeToFile(ofstream& file) {
	if (!Empty()) {
		Tlist it = head;

		while (it != NULL) {
			string supplier = to_string(it->sup_account_number);
			string client = to_string(it->cl_account_number);
			string purpose = it->purpose;
			string sum = to_string(it->sum);
			string splitter = " ";

			string str = supplier + splitter + client + splitter + purpose + splitter + sum;
			file << str << endl;
			it = it->next;
		}
	}
}

void Payments::readFromFile(ifstream& file) {
	char ch = file.peek();
	while (!file.eof() && (ch != EOF)) {
		string str;
		getline(file, str);

		int n = str.find(' ');
		string sup = str.substr(0, n);
		int supplier = atoi(sup.c_str());
		str = str.substr(n + 1, str.length());

		n = str.find(' ');
		string cl = str.substr(0, n);
		int client = atoi(cl.c_str());
		str = str.substr(n + 1, str.length());

		n = str.find(' ');
		string purpose = str.substr(0, n);
		str = str.substr(n + 1, str.length());

		string s = str;
		int sum = atoi(s.c_str());

		if (Empty()) addToHead(supplier, client, purpose, sum);
		else addToTail(supplier, client, purpose, sum);
	}
}

bool Payments::Empty() {
	return head == NULL;
}

void Payments::changeNode(Tlist& p) {
	cout << "Current state of the payment:" << endl;
	printNode(p);

	int temp;
	string str;
	cout << "Enter new supplier account: ";
	cin >> temp;
	cout << endl;
	p->sup_account_number = temp;
	cout << "Enter new client account: ";
	cin >> temp;
	cout << endl;
	p->cl_account_number = temp;
	cout << "Enter new purpose: ";
	cin >> str;
	cout << endl;
	p->purpose = str;
	cout << "Enter new sum: ";
	cin >> temp;
	cout << endl;
	p->sum = temp;

	cout << "New state of the payment:" << endl;
	printNode(p);
}

void Payments::swapNodes(Tlist& first, Tlist& second) {
	Tlist temp1 = second->next, temp2 = second->prev;
	second->next = first->next;
	second->prev = first->prev;
	first->next = temp1;
	first->prev = temp2;

	if (head == first) head = second;
	else if (head == second) head = first;

	if (tail == first) tail = second;
	else if (tail == second) tail = first;
}

void Payments::printList() {
	Tlist p = head;
	cout << count << " payments:" << endl;
	while (p != NULL) {
		printNode(p);
		cout << endl;
		p = p->next;
	}
}

void Payments::delFromTail() {
	Tlist p = tail;
	tail = tail->prev;
	p->prev = NULL;
	tail->next = NULL;
	delete p;
	count--;
}

void Payments::delFromHead() {
	Tlist p = head;
	head = head->next;
	p->next = NULL;
	head->prev = NULL;
	delete p;
	count--;
}

void Payments::addToHead(int san, int can, string pur, int sum) {
	if (Empty()) {
		head = new NODE(san, can, pur, sum);
		tail = head;
		head->next = NULL;
		head->prev = NULL;
	}
	else {
		Tlist p = new NODE(san, can, pur, sum);
		p->next = head;
		p->prev = NULL;
		head->prev = p;
		head = p;
	}
	count++;
}

void Payments::addToTail(int san, int can, string pur, int sum) {
	//if (!count) addToHead()
	Tlist p = new NODE(san, can, pur, sum);
	p->next = NULL;
	p->prev = tail;
	tail->next = p;
	tail = p;
	count++;
}

void Payments::printNode(Tlist& p) {
	cout << "supplier: " << p->sup_account_number;
	cout << " | client: " << p->cl_account_number;
	cout << " | purpose: " << p->purpose;
	cout << " | sum: " << p->sum << endl;
}

void Payments::printSupplier(int account) {
	Tlist p = head;
	int num = 0;
	int sum = 0;
	while (p != NULL) {
		if (p->sup_account_number == account) {
			num++;
			printNode(p);
			sum += p->sum;
		}
		p = p->next;
	}
	if (!num) cout << "No suppliers found with account number: " << account << endl;
	else cout << num << " suppliers with total: " << sum << endl;
}

void Payments::searchPurpose(string str) {
	Tlist p = head;
	int num = 0;
	while (p != NULL) {
		if (p->purpose == str) {
			num++;
			printNode(p);
		}
		p = p->next;
	}
	if (!num) cout << "Nothing found with purpose: " << str << endl;
}

void Payments::clearList() {
	Tlist p = head;
	while (count) {
		head = head->next;
		p->next = NULL;
		p->prev = NULL;
		delete p;
		p = head;
		count--;
	}
	head = NULL;
	tail = NULL;
	cout << "List has been cleared" << endl;
}

Payments::Payments() {
	count = 0;
	head = NULL;
	tail = NULL;
}

Payments::~Payments() {
	clearList();
}

int main() {
	Payments* paymentList = new Payments;
	paymentList->Menu();
	system("pause");
}
