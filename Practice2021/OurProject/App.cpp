#include "App.h"
#include "List.h"
#include "Students.h"
#include <Windows.h>
//#include <fstream>

using namespace Prak807;
using namespace System;
using namespace System::Windows::Forms;

std::ifstream in;
std::ofstream out;
std::string filename;
std::string curstr;

//std::string toNormalString(String^ oldstr) { //?????, не работает 
//	char cStr[50] = { 0 };
//	if (oldstr->Length < sizeof(cStr))
//		sprintf_s(cStr, "%s", oldstr);// исправил sprintf на sprintf_s
//	std::string newstr(cStr);
//	return newstr;
//}
std::string toNormalString(String^ s)
{
	using namespace Runtime::InteropServices;
	const char* ptr = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	return std::string(ptr);
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew App);
	return 0;
}

System::Void Prak807::App::b_fileSend_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	ClearPanel(p_bestStudents);
	String^ str = TB_fileName->Text;
	filename = toNormalString(str);
	//sw = gcnew System::IO::StreamWriter(textBox1->Text);
	out.open(filename, std::ios::app);
	return System::Void();
}

System::Void Prak807::App::b_studentSend_Click(System::Object^ sender, System::EventArgs^ e)
{
	//sw->WriteLine(textBox2->Text);
	String^ cur = TB_fio->Text;
	curstr = toNormalString(cur);
	auto _1 = Convert::ToInt16(TB_Mark1->Text);
	auto _2 = Convert::ToInt16(TB_Mark2->Text);
	auto _3 = Convert::ToInt16(TB_Mark3->Text);
	auto _4 = Convert::ToInt16(TB_Mark4->Text);
	auto _5 = Convert::ToInt16(TB_Mark5->Text);
	std::vector<decltype(_1)> vect{ _1,_2,_3,_4,_5 };
	bool check = true;
	for (auto x : vect) {
		if (x < 2 || x > 5) check = false;
	}
	if (!check) {
		MessageBox::Show("Неверные оценки...");
	}
	else {
		TB_fio->Text = "";
		if (out) {
			std::string s1 = " " + toNormalString(TB_Mark1->Text);
			std::string s2 = " " + toNormalString(TB_Mark2->Text);
			std::string s3 = " " + toNormalString(TB_Mark3->Text);
			std::string s4 = " " + toNormalString(TB_Mark4->Text);
			std::string s5 = " " + toNormalString(TB_Mark5->Text);
			curstr = curstr + s1 + s2 + s3 + s4 + s5;
			out << curstr << std::endl;
		}
	}

	return System::Void();
}

System::Void Prak807::App::b_fileClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_fileName->Text = "";
	return System::Void();
}

System::Void Prak807::App::b_fioClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_fio->Text = "";
	return System::Void();
}

System::Void Prak807::App::b_marksClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_Mark1->Text = "";
	TB_Mark2->Text = "";
	TB_Mark3->Text = "";
	TB_Mark4->Text = "";
	TB_Mark5->Text = "";
	return System::Void();
}

System::Void Prak807::App::b_fileClose_Click(System::Object^ sender, System::EventArgs^ e)
{
	out.close();
	ClearPanel(p_bestStudents);
	std::vector<St::Student> v = /*ListB::*/makeBestListS(filename);
	if (!v.empty()) {
		int w = 20, h = 10;
		int max = v[0].getCount();
		for (auto it = v.begin(); it != v.end(); it++) {
			if ((*it).getCount() == max) {
				String^ s = gcnew String((*it).toString().c_str());
				/*TextBox^ tb = gcnew TextBox(); через текстбоксы
				tb->Location = System::Drawing::Point(w, h);
				tb->Text = s;
				p_bestStudents->Controls->Add(tb);*/
				Label^ l = gcnew Label();
				l->AutoSize = true;
				l->BackColor = System::Drawing::SystemColors::Control;
				l->Text = s;
				l->Location = Point(w, h);
				p_bestStudents->Controls->Add(l);
				h += 30;
			}
		}
	}
	else {
		//TextBox^ tb = gcnew TextBox(); через текстбоксы
		//tb->Location = System::Drawing::Point(40, 10);
		//tb->Text =  "Пусто";
		///*tb->Size= System::Drawing::Size(127, 31);*/
		//p_bestStudents->Controls->Add(tb);
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = "Нет студентов с 5-ками";
		l->Location = Point(20, 10);
		p_bestStudents->Controls->Add(l);
	}
	return System::Void();
}

System::Void Prak807::App::ClearPanel(System::Windows::Forms::Panel^ p)
{
	int i = p->Controls->Count - 1;
	while (i >= 0) {
		Control^ c = p->Controls[i];
		p->Controls->Remove(c);
		i--;
	}
	return System::Void();
}

System::Void Prak807::App::b_fTree_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	Tree tr = NULL;
	read_from_f(tr,filename);
	if (tr)printTreeToPanel(tr, 1, /*1,*/ 300, 15);
	else MessageBox::Show("Дерево пустое, нечего выводить", "Ошибка");
	return System::Void();
}

System::Void Prak807::App::b_aTree_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	ATree tr = NULL;
	read_from_f(tr, filename);
	if (tr)printTreeToPanel(tr, 1, /*1,*/ 300, 15);
	else MessageBox::Show("Дерево пустое, нечего выводить", "Ошибка");
	return System::Void();
}

System::Void Prak807::App::printTreeToPanel(Tree t, int lvl, /*int ind,*/ int w, int h)
{
	if (t && lvl <= 3) {
		/*TextBox^ tb = gcnew TextBox(); через текстбоксы
		tb->Location = System::Drawing::Point(w, h);
		tb->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		p_main->Controls->Add(tb);*/
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		l->Location = Point(w, h);
		p_main->Controls->Add(l);
		printTreeToPanel(t->left, lvl + 1, /*ind,*/ w - 90 * (3 - lvl), h + 80);
		printTreeToPanel(t->right, lvl + 1, /*ind,*/ w + 90 * (3 - lvl), h + 80);
	}
}

System::Void Prak807::App::printTreeToPanel(ATree t, int lvl, /*int ind,*/ int w, int h)
{
	if (t && lvl <= 3) {
		/*TextBox^ tb = gcnew TextBox(); через текстбоксы
		tb->Location = System::Drawing::Point(w, h);
		tb->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		p_main->Controls->Add(tb);*/
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		l->Location = Point(w, h);
		p_main->Controls->Add(l);
		printTreeToPanel(t->left, lvl + 1, /*ind,*/ w - 90 * (3 - lvl), h + 80);
		printTreeToPanel(t->right, lvl + 1, /*ind,*/ w + 90 * (3 - lvl), h + 80);
	}
}

System::Void Prak807::App::b_list1_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	std::list<St::Student> li2 = Task2(filename);
	if (!li2.empty()) {
		int w = 300, h = 15;
		for (auto it = li2.begin(); it != li2.end(); it++) {
			String^ s = gcnew String((*it).toString().c_str());
			/*TextBox^ tb = gcnew TextBox(); через текстбоксы
			tb->Location = System::Drawing::Point(w, h);
			tb->Text = s;
			p_bestStudents->Controls->Add(tb);*/
			Label^ l = gcnew Label();
			l->AutoSize = true;
			l->BackColor = System::Drawing::SystemColors::Control;
			l->Text = s;
			l->Location = Point(w, h);
			p_main->Controls->Add(l);
			h += 30;
		}
	}
	else MessageBox::Show("Цепочка пуста, нечего выводить", "Ошибка");
	return System::Void();
}

System::Void Prak807::App::b_list2_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	std::list<St::Student> li3 = Task3(filename);
	if (!li3.empty()) {
		int w = 300, h = 15;
		for (auto it = li3.begin(); it != li3.end(); it++) {
			String^ s = gcnew String((*it).toString().c_str());
			/*TextBox^ tb = gcnew TextBox(); через текстбоксы
			tb->Location = System::Drawing::Point(w, h);
			tb->Text = s;
			p_bestStudents->Controls->Add(tb);*/
			Label^ l = gcnew Label();
			l->AutoSize = true;
			l->BackColor = System::Drawing::SystemColors::Control;
			l->Text = s;
			l->Location = Point(w, h);
			p_main->Controls->Add(l);
			h += 30;
		}
	}
	else MessageBox::Show("Цепочка пуста, нечего выводить", "Ошибка");
	return System::Void();
}
