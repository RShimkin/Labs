#include "List1.h"
#include <iostream>

using namespace std;

void Init(Tlist& list) {
	list = NULL;
}

bool Empty(const Tlist list) {
	return list == NULL;
}

void AddToHead(Tlist& list, int newElem) {
	Tlist p = new NODE;
	p->info = newElem;
	p->next = list;
	list = p;
}

int Count(Tlist list) {
	int k = 0;
	Tlist p = list;
	while (p != NULL) {
		k++;
		p = p->next;
	}
	return k;
}

void AddToTail(Tlist& list, int newElem) {
	Tlist p = new NODE;
	p->info = newElem;
	p->next = NULL;
	if (Empty(list)) {
		list = p;
	}
	else {
		Tlist it = list;
		while (it->next != NULL) it = it->next;
		it->next = p;
	}
}

void View(Tlist list) {
	Tlist it = list;
	cout << "List: ";
	while (it != NULL) {
		cout << it->info << " ";
		it = it->next;
	}
	cout << endl;
}

void Delete(Tlist& list) {
	if (!(Empty(list))) {
		Tlist p;
		while (list != NULL) {
			p = list;
			list = list->next;
			p->next = NULL;
			delete p;
		}
	}
}

void Delete(Tlist& list, int pos) {
	if (pos > 0) {
		for (int i = 0; i < pos; i++) {
			Tlist it = list;
			list = list->next;
			it->next = NULL;
			delete it;
		}
	}
	else {
		int n = Count(list) + pos;
		Tlist it = list;
		for (int i = 1; i < n; i++) {
			it = it->next;
		}
		Tlist delit = it->next;
		it->next = NULL;
		for (; delit != NULL; ) {
			Tlist p = delit;
			delit = delit->next;
			p->next = NULL;
			delete p;
		}
	}
}

int LongestIncSequence(Tlist list) {
	int k;
	if (Empty(list)) k = 0;
	else {
		k = 0;
		int prev = INT_MIN;
		int temp = 0;
		Tlist it = list;
		while (it != NULL) {
			if (it->info > prev) {
				temp++;
				prev = it->info;
			}
			else {
				if (temp > k) k = temp;
				temp = 1;
				prev = it->info;
			}
			it = it->next;
		}
		if (temp > k) k = temp;
	}
	return k;
}