#pragma once
#include<string>
#include<iostream>

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


