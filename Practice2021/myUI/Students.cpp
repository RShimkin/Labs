#include "Students.h"

Student::Student(std::string& str, int m[]) {
	name = str;
	int temp = 5;
	float sum = 0;
	count5 = 0;
	for (int i = 0; i < 5; i++) {
		marks[i] = m[i];
		sum += m[i];
		if (m[i] < temp) temp = m[i];
		if (m[i] == 5) count5++;
	}
	minMark = temp;
	avg = sum / 5;
}

int Student::getMinMark() {
	return minMark;
}

std::string Student::toString() {
	std::string res = name + "(";
	res += char(marks[0] + '0');
	res += char(marks[1] + '0');
	res += char(marks[2] + '0');
	res += char(marks[3] + '0');
	res += char(marks[4] + '0');
	res += ") ";
	return res;
}

float Student:: getAvg() const {
	return avg;
}

int Student::getCount() const {
	return count5;
}

std::string Student::getName() const {
	return name;
}