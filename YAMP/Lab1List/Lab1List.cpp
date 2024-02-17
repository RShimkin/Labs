#include <iostream>
#include "List1.h"
#include <fstream>
#include <string>
#include <windows.h>

string message[10] = {
	" #Press 0 for exit",
	" #Press 1 for file input",
	" #Press 2 for manual input to head",
	" #Press 3 for manual input to tail",
	" #Press 4 to print the list",
	" #Press 5 to count the enumber of elements",
	" #Press 6 to find the length of the longest increasing sequence",
	" #Press 7 to delete all elements",
	" #Press 8 to delete k elements",
	" #Press 9 to clear the window"
};

using namespace std;

int main() {

	Tlist spisok;
	Init(spisok);

	unsigned int choice;
	bool goon = true;
	string buf = "";

	for (auto str : message)
		cout << str << endl;

	while (goon) {

		cout << "	Your choice: ";
		cin >> choice;

		switch (choice) {

		case 0: {
			goon = false;
			break;
		}

		case 1: {
			string name;
			cout << "Enter the file name:\n";
			cin >> name;
			ifstream file(name.c_str());
			if (!file.is_open()) {
				cout << "File is not open!";
				break;
			}

			/*
			int ch;
			ch = file.peek();
			cout << ch;
			*/ 
			int value;
			while (!file.eof()) {
				file >> value;
				if ((long) value == -858993460) break;
				AddToTail(spisok, value);
			}

			if (file.is_open()) {
				cout << "The file is read\n";
			}
			file.close();
			break;
		}

		case 2: {
			cout << "Enter the element: ";
			int elem;
			cin >> elem;
			AddToHead(spisok, elem);
			break;
		}

		case 3: {
			cout << "Enter the element: ";
			int elem;
			cin >> elem;
			AddToTail(spisok, elem);
			break;
		}

		case 4: {
			View(spisok);
			break;
		}

		case 5: {
			cout << Count(spisok) << " elements" << endl;
			break;
		}

		case 6: {
			cout << "The length is: " << LongestIncSequence(spisok) << endl;
			break;
		}

		case 7: {
			Delete(spisok);
			cout << "The list is clear";
			break;
		}

		case 8: {
			cout << "Enter the number of elements to delete\n";
			cout << "( k for deletion from head and -k for deletion from tail): ";
			int k;
			cin >> k;
			if (abs(k) > Count(spisok)) {
				cout << "The list is shorter than the number you specified!";
				break;
			}
			Delete(spisok, k);
			break;
		}

		case 9: {
			system("cls");
			for (auto str : message)
				cout << str << endl;
			break;
		}
		}
	}

	system("pause");
	return 0;
}
