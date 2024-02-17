
#include "MyForm.h"
#include "List.h"
#include "Students.h"
#include <Windows.h>

using namespace myUI;
using namespace System;
using namespace System::Windows::Forms;

std::ifstream in;
std::ofstream out;
std::string filename;
std::string curstr;

std::string toNormalString(String^ s) {
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

System::Void myUI::App::buttonState(int i) {
	switch (i) { //1->2->1->...
	case 1: {
		TB_fio->Enabled = true;
        TB_Mark1->Enabled = true;
		TB_Mark2->Enabled = true;
		TB_Mark3->Enabled = true;
		TB_Mark4->Enabled = true;
		TB_Mark5->Enabled = true;
		b_studentSend->Enabled = true;
		b_fioClear->Enabled = true;
		b_marksClear->Enabled = true;
		b_fileClose->Enabled = true;
		b_list1->Enabled = false;
		b_list2->Enabled = false;
		b_fTree->Enabled = false;
		b_aTree->Enabled = false;
		break;
	}
	case 2: {
		TB_fio->Enabled = false;
		TB_Mark1->Enabled = false;
		TB_Mark2->Enabled = false;
		TB_Mark3->Enabled = false;
		TB_Mark4->Enabled = false;
		TB_Mark5->Enabled = false;
		b_studentSend->Enabled = false;
		b_fioClear->Enabled = false;
		b_marksClear->Enabled = false;
		b_fileClose->Enabled = false;
		b_list1->Enabled = true;
		b_list2->Enabled = true;
		b_fTree->Enabled = true;
		b_aTree->Enabled = true;
		break;
	}
	}
	return System::Void();
}

System::Void myUI::App::b_fileSend_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (out) out.close();
	ClearPanel(p_main);
	ClearPanel(p_bestStudents);
	String^ str = TB_fileName->Text;
	filename = toNormalString(str);
	out.open(filename, std::ios::app);
	if (!out) MessageBox::Show("Ошибка файла", "Ошибка!");
	buttonState(1);
	return System::Void();
}

System::Void myUI::App::b_studentSend_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (!out) MessageBox::Show("Некуда писать","Ошибка!");
	else {
		String^ cur = TB_fio->Text;
		auto flag = cur->IsNullOrWhiteSpace(cur);
		auto arr = cur->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);
		if (!flag && (arr->Length == 2) && arr[0]->Length <= 20 && arr[1]->Length <= 20) {
			cur = arr[0] + " " + arr[1];
			curstr = toNormalString(cur);

			auto arr1 = TB_Mark1->Text->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);
			auto arr2 = TB_Mark2->Text->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);
			auto arr3 = TB_Mark3->Text->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);
			auto arr4 = TB_Mark4->Text->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);
			auto arr5 = TB_Mark5->Text->Split(gcnew array<Char> { ' ', '\n' }, StringSplitOptions::RemoveEmptyEntries);

			auto _1 = (arr1->Length > 0 && arr1[0]->Length > 0) ? Convert::ToInt16(arr1[0]) : -1 ;
			auto _2 = (arr2->Length > 0 && arr2[0]->Length > 0) ? Convert::ToInt16(arr2[0]) : -1;
			auto _3 = (arr3->Length > 0 && arr3[0]->Length > 0) ? Convert::ToInt16(arr3[0]) : -1;
			auto _4 = (arr4->Length > 0 && arr4[0]->Length > 0) ? Convert::ToInt16(arr4[0]) : -1;
			auto _5 = (arr5->Length > 0 && arr5[0]->Length > 0) ? Convert::ToInt16(arr5[0]) : -1;
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
					out << std::endl;
					std::string s1 = " " + toNormalString(arr1[0]);
					std::string s2 = " " + toNormalString(arr2[0]);
					std::string s3 = " " + toNormalString(arr3[0]);
					std::string s4 = " " + toNormalString(arr4[0]);
					std::string s5 = " " + toNormalString(arr5[0]);
					curstr = curstr + s1 + s2 + s3 + s4 + s5;
					out << curstr;
				}
			}
		}
		else {
			MessageBox::Show("Неверные ФИО", "Ошибка");
		}
		
	}

	return System::Void();
}

System::Void myUI::App::b_fileClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_fileName->Text = "";
	return System::Void();
}

System::Void myUI::App::b_fioClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_fio->Text = "";
	return System::Void();
}

System::Void myUI::App::b_marksClear_Click(System::Object^ sender, System::EventArgs^ e)
{
	TB_Mark1->Text = "";
	TB_Mark2->Text = "";
	TB_Mark3->Text = "";
	TB_Mark4->Text = "";
	TB_Mark5->Text = "";
	return System::Void();
}

System::Void myUI::App::b_fileClose_Click(System::Object^ sender, System::EventArgs^ e)
{
	out.close();
	ClearPanel(p_bestStudents);
	std::list<Student> lst = makeBestList(filename);
	if (!lst.empty()) {
		int w = 0, h = 10;
		for (auto it = lst.begin(); it != lst.end(); it++) {
				String^ s = gcnew String((*it).toString().c_str());
				Label^ l = gcnew Label();
				l->AutoSize = true;
				l->BackColor = System::Drawing::SystemColors::Control;
				l->Text = s;
				l->Location = Point(w, h);
				p_bestStudents->Controls->Add(l);
				h += 30;
		}
	}
	else {
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = "Нет студентов с 5-ками"; 
		l->Location = Point(20, 10);
		p_bestStudents->Controls->Add(l);
	}
	buttonState(2);
	return System::Void();
}

System::Void myUI::App::ClearPanel(System::Windows::Forms::Panel^ p)
{
	int i = p->Controls->Count - 1;
	while (i >= 0) {
		Control^ c = p->Controls[i];
		p->Controls->Remove(c);
		i--;
	}
	return System::Void();
}

System::Void myUI::App::b_fTree_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	Tree tr = NULL;
	read_from_f(tr, filename);
	if (tr) printTreeToPanel(tr, 1, 300, 15);
	else MessageBox::Show("Дерево пустое, нечего выводить", "Ошибка");
	return System::Void();
}

System::Void myUI::App::b_aTree_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	ATree tr = NULL;
	read_from_f(tr, filename);
	if (tr)printTreeToPanel(tr, 1, 300, 15);
	else MessageBox::Show("Дерево пустое, нечего выводить", "Ошибка");
	return System::Void();
}

System::Void myUI::App::printTreeToPanel(Tree t, int lvl, int w, int h)
{
	if (t && lvl <= 3) {
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		l->Location = Point(w, h);
		p_main->Controls->Add(l);
		printTreeToPanel(t->left, lvl + 1, w - 90 * (3 - lvl), h + 80);
		printTreeToPanel(t->right, lvl + 1, w + 90 * (3 - lvl), h + 80);
	}
}

System::Void myUI::App::printTreeToPanel(ATree t, int lvl, int w, int h)
{
	if (t && lvl <= 3) {
		Label^ l = gcnew Label();
		l->AutoSize = true;
		l->BackColor = System::Drawing::SystemColors::Control;
		l->Text = gcnew System::String(t->last_name.c_str()) + ", " + Convert::ToString(t->avg);
		l->Location = Point(w, h);
		p_main->Controls->Add(l);
		printTreeToPanel(t->left, lvl + 1, w - 90 * (3 - lvl), h + 80);
		printTreeToPanel(t->right, lvl + 1, w + 90 * (3 - lvl), h + 80);
	}
}

System::Void myUI::App::b_list1_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	std::list<Student> li2 = Task2(filename);
	if (!li2.empty()) {
		int w = 300, h = 15;
		for (auto it = li2.begin(); it != li2.end(); it++) {
			String^ s = gcnew String((*it).getName().c_str());
			s += ", " + Convert::ToString(it->getAvg());
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

System::Void myUI::App::b_list2_Click(System::Object^ sender, System::EventArgs^ e)
{
	ClearPanel(p_main);
	std::list<Student> li3 = Task3(filename);
	if (!li3.empty()) {
		int w = 300, h = 15;
		for (auto it = li3.begin(); it != li3.end(); it++) {
			String^ s = gcnew String((*it).getName().c_str());
			s += ", " + Convert::ToString(it->getAvg());
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
