#pragma once
#include <string>

class Student {
		std::string name;
		int minMark;
		int count5;
		float avg;
		int marks[5];
	public:
		Student(std::string& str, int m[]);
		int getMinMark();
		std::string toString();
		float getAvg() const;
		int getCount() const;
		std::string getName() const;
};