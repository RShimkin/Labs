#include "List.h"

Student parse(std::string& str) {
	int pos1 = str.find_first_of(" ");
	int pos2 = str.find_first_of(" ", pos1 + 1);
	std::string surname = str.substr(pos1, pos2 - pos1);
	std::string tail = str.substr(++pos2);
	int m[5];
	for (int i = 0; i < 5; i++) m[i] = tail[2 * i] - '0';
	Student stud(surname, m);
	return stud;
}

std::list<Student> makeBestList(std::string& fname) {
	setlocale(LC_ALL, "rus");
	std::list<Student> lst, res;
	if (fname.length()) {
		std::ifstream is(fname);
		std::string buf;
		while (!is.eof()) {
			std::getline(is, buf);
			if (buf.length()) lst.push_back(parse(buf));
		}
		is.close();
		lst.sort([](const Student& left, const Student& right) -> bool 
			       { return left.getCount() > right.getCount(); }
		);
		int max = (lst.empty()) ? 0 : lst.front().getCount();
		if (max) {
			std::copy_if(lst.begin(), lst.end(), std::back_inserter(res), [max](const Student& stud)
				{ return stud.getCount() == max; }
			);
		}
	}
	return res;
}

bool Fill_List_Based_On_File(std::string file_name, std::list<Student>& exams_info) {

	bool successfully = true;

	std::ifstream input_file(file_name);
	if (input_file.good()) {

		std::string first_name, last_name, name_info;
		int arr[5] = { 0 };

		while (!input_file.eof()) {

			input_file >> first_name;
			input_file >> last_name;

			name_info = last_name;

			if (name_info.size()) {
				for (unsigned int i = 0; i < 5; i++)
					input_file >> arr[i];

				exams_info.push_back(Student(name_info, arr));
			}
		}

		input_file.close();
		return successfully;
	}
	else return !successfully;
}

std::list<Student> Task2(std::string source_file_name) {

	std::list<Student> exams_info;
	Fill_List_Based_On_File(source_file_name, exams_info);

	std::stack<Student> debtors;

	for (std::list<Student>::iterator it = exams_info.begin(); it != exams_info.end();) {

		if (it->getMinMark()==2) {
			debtors.push(*it);
			it = exams_info.erase(it);
		}
		else ++it;
	}

	while (!debtors.empty()) {
		exams_info.push_front(debtors.top());
		debtors.pop();
	}

	return exams_info;
}

std::list<Student> Task3(std::string source_file_name) {

	std::list<Student> exams_info;
	Fill_List_Based_On_File(source_file_name, exams_info);

	exams_info.sort(
		[](const Student& student1, const Student& student2)
		{ return (student1.getAvg() < student2.getAvg()); }
	);

	return exams_info;
}