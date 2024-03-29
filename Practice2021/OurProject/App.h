#pragma once
#include "Tree.h"
#include "AVLTree.h"

namespace Prak807 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� App
	/// </summary>
	public ref class App : public System::Windows::Forms::Form
	{
	public:
		App(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//
		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~App()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ Creators;
	private: System::Windows::Forms::TextBox^ TB_fileName;
	private: System::Windows::Forms::Label^ l_file;
	private: System::Windows::Forms::Label^ l_fio;
	private: System::Windows::Forms::TextBox^ TB_fio;
	private: System::Windows::Forms::Label^ l_marks;
	private: System::Windows::Forms::TextBox^ TB_Mark1;
	private: System::Windows::Forms::TextBox^ TB_Mark2;
	private: System::Windows::Forms::TextBox^ TB_Mark3;
	private: System::Windows::Forms::TextBox^ TB_Mark4;
	private: System::Windows::Forms::TextBox^ TB_Mark5;
	private: System::Windows::Forms::Button^ b_fileSend;
	private: System::Windows::Forms::Button^ b_studentSend;


	private: System::Windows::Forms::Button^ b_fileClear;
	private: System::Windows::Forms::Button^ b_fioClear;
	private: System::Windows::Forms::Button^ b_marksClear;
	private: System::Windows::Forms::Button^ b_fileClose;
	private: System::Windows::Forms::Button^ b_list1;
	private: System::Windows::Forms::Button^ b_list2;
	private: System::Windows::Forms::Button^ b_fTree;
	private: System::Windows::Forms::Button^ b_aTree;
	private: System::Windows::Forms::Panel^ p_main;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Panel^ p_bestStudents;


	protected:

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Creators = (gcnew System::Windows::Forms::Label());
			this->TB_fileName = (gcnew System::Windows::Forms::TextBox());
			this->l_file = (gcnew System::Windows::Forms::Label());
			this->l_fio = (gcnew System::Windows::Forms::Label());
			this->TB_fio = (gcnew System::Windows::Forms::TextBox());
			this->l_marks = (gcnew System::Windows::Forms::Label());
			this->TB_Mark1 = (gcnew System::Windows::Forms::TextBox());
			this->TB_Mark2 = (gcnew System::Windows::Forms::TextBox());
			this->TB_Mark3 = (gcnew System::Windows::Forms::TextBox());
			this->TB_Mark4 = (gcnew System::Windows::Forms::TextBox());
			this->TB_Mark5 = (gcnew System::Windows::Forms::TextBox());
			this->b_fileSend = (gcnew System::Windows::Forms::Button());
			this->b_studentSend = (gcnew System::Windows::Forms::Button());
			this->b_fileClear = (gcnew System::Windows::Forms::Button());
			this->b_fioClear = (gcnew System::Windows::Forms::Button());
			this->b_marksClear = (gcnew System::Windows::Forms::Button());
			this->b_fileClose = (gcnew System::Windows::Forms::Button());
			this->b_list1 = (gcnew System::Windows::Forms::Button());
			this->b_list2 = (gcnew System::Windows::Forms::Button());
			this->b_fTree = (gcnew System::Windows::Forms::Button());
			this->b_aTree = (gcnew System::Windows::Forms::Button());
			this->p_main = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->p_bestStudents = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// Creators
			// 
			this->Creators->AutoSize = true;
			this->Creators->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Creators->Location = System::Drawing::Point(12, 27);
			this->Creators->Name = L"Creators";
			this->Creators->Size = System::Drawing::Size(852, 31);
			this->Creators->TabIndex = 0;
			this->Creators->Text = L" ������ ����, �������� �����, ������ �����  2 ���� 62 ������\r\n";
			// 
			// TB_fileName
			// 
			this->TB_fileName->Location = System::Drawing::Point(149, 95);
			this->TB_fileName->Name = L"TB_fileName";
			this->TB_fileName->Size = System::Drawing::Size(298, 20);
			this->TB_fileName->TabIndex = 1;
			// 
			// l_file
			// 
			this->l_file->AutoSize = true;
			this->l_file->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->l_file->Location = System::Drawing::Point(44, 90);
			this->l_file->Name = L"l_file";
			this->l_file->Size = System::Drawing::Size(62, 24);
			this->l_file->TabIndex = 2;
			this->l_file->Text = L"����:";
			// 
			// l_fio
			// 
			this->l_fio->AutoSize = true;
			this->l_fio->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->l_fio->Location = System::Drawing::Point(44, 157);
			this->l_fio->Name = L"l_fio";
			this->l_fio->Size = System::Drawing::Size(69, 24);
			this->l_fio->TabIndex = 3;
			this->l_fio->Text = L"�.�.�:";
			// 
			// TB_fio
			// 
			this->TB_fio->Location = System::Drawing::Point(149, 161);
			this->TB_fio->Name = L"TB_fio";
			this->TB_fio->Size = System::Drawing::Size(298, 20);
			this->TB_fio->TabIndex = 4;
			// 
			// l_marks
			// 
			this->l_marks->AutoSize = true;
			this->l_marks->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->l_marks->Location = System::Drawing::Point(44, 230);
			this->l_marks->Name = L"l_marks";
			this->l_marks->Size = System::Drawing::Size(83, 24);
			this->l_marks->TabIndex = 5;
			this->l_marks->Text = L"������:";
			// 
			// TB_Mark1
			// 
			this->TB_Mark1->Location = System::Drawing::Point(149, 234);
			this->TB_Mark1->Name = L"TB_Mark1";
			this->TB_Mark1->Size = System::Drawing::Size(37, 20);
			this->TB_Mark1->TabIndex = 6;
			// 
			// TB_Mark2
			// 
			this->TB_Mark2->Location = System::Drawing::Point(209, 234);
			this->TB_Mark2->Name = L"TB_Mark2";
			this->TB_Mark2->Size = System::Drawing::Size(37, 20);
			this->TB_Mark2->TabIndex = 7;
			// 
			// TB_Mark3
			// 
			this->TB_Mark3->Location = System::Drawing::Point(272, 234);
			this->TB_Mark3->Name = L"TB_Mark3";
			this->TB_Mark3->Size = System::Drawing::Size(37, 20);
			this->TB_Mark3->TabIndex = 8;
			// 
			// TB_Mark4
			// 
			this->TB_Mark4->Location = System::Drawing::Point(340, 234);
			this->TB_Mark4->Name = L"TB_Mark4";
			this->TB_Mark4->Size = System::Drawing::Size(37, 20);
			this->TB_Mark4->TabIndex = 9;
			// 
			// TB_Mark5
			// 
			this->TB_Mark5->Location = System::Drawing::Point(410, 234);
			this->TB_Mark5->Name = L"TB_Mark5";
			this->TB_Mark5->Size = System::Drawing::Size(37, 20);
			this->TB_Mark5->TabIndex = 10;
			// 
			// b_fileSend
			// 
			this->b_fileSend->Location = System::Drawing::Point(498, 82);
			this->b_fileSend->Name = L"b_fileSend";
			this->b_fileSend->Size = System::Drawing::Size(156, 44);
			this->b_fileSend->TabIndex = 11;
			this->b_fileSend->Text = L"���������";
			this->b_fileSend->UseVisualStyleBackColor = true;
			this->b_fileSend->Click += gcnew System::EventHandler(this, &App::b_fileSend_Click);
			// 
			// b_studentSend
			// 
			this->b_studentSend->Location = System::Drawing::Point(498, 157);
			this->b_studentSend->Name = L"b_studentSend";
			this->b_studentSend->Size = System::Drawing::Size(156, 109);
			this->b_studentSend->TabIndex = 12;
			this->b_studentSend->Text = L"���������";
			this->b_studentSend->UseVisualStyleBackColor = true;
			this->b_studentSend->Click += gcnew System::EventHandler(this, &App::b_studentSend_Click);
			// 
			// b_fileClear
			// 
			this->b_fileClear->Location = System::Drawing::Point(672, 82);
			this->b_fileClear->Name = L"b_fileClear";
			this->b_fileClear->Size = System::Drawing::Size(192, 44);
			this->b_fileClear->TabIndex = 14;
			this->b_fileClear->Text = L"��������";
			this->b_fileClear->UseVisualStyleBackColor = true;
			this->b_fileClear->Click += gcnew System::EventHandler(this, &App::b_fileClear_Click);
			// 
			// b_fioClear
			// 
			this->b_fioClear->Location = System::Drawing::Point(672, 157);
			this->b_fioClear->Name = L"b_fioClear";
			this->b_fioClear->Size = System::Drawing::Size(192, 44);
			this->b_fioClear->TabIndex = 15;
			this->b_fioClear->Text = L"��������";
			this->b_fioClear->UseVisualStyleBackColor = true;
			this->b_fioClear->Click += gcnew System::EventHandler(this, &App::b_fioClear_Click);
			// 
			// b_marksClear
			// 
			this->b_marksClear->Location = System::Drawing::Point(672, 222);
			this->b_marksClear->Name = L"b_marksClear";
			this->b_marksClear->Size = System::Drawing::Size(192, 44);
			this->b_marksClear->TabIndex = 16;
			this->b_marksClear->Text = L"��������";
			this->b_marksClear->UseVisualStyleBackColor = true;
			this->b_marksClear->Click += gcnew System::EventHandler(this, &App::b_marksClear_Click);
			// 
			// b_fileClose
			// 
			this->b_fileClose->Location = System::Drawing::Point(880, 208);
			this->b_fileClose->Name = L"b_fileClose";
			this->b_fileClose->Size = System::Drawing::Size(169, 58);
			this->b_fileClose->TabIndex = 17;
			this->b_fileClose->Text = L"������� ����";
			this->b_fileClose->UseVisualStyleBackColor = true;
			this->b_fileClose->Click += gcnew System::EventHandler(this, &App::b_fileClose_Click);
			// 
			// b_list1
			// 
			this->b_list1->Location = System::Drawing::Point(27, 285);
			this->b_list1->Name = L"b_list1";
			this->b_list1->Size = System::Drawing::Size(122, 45);
			this->b_list1->TabIndex = 18;
			this->b_list1->Text = L"������� 1";
			this->b_list1->UseVisualStyleBackColor = true;
			this->b_list1->Click += gcnew System::EventHandler(this, &App::b_list1_Click);
			// 
			// b_list2
			// 
			this->b_list2->Location = System::Drawing::Point(27, 365);
			this->b_list2->Name = L"b_list2";
			this->b_list2->Size = System::Drawing::Size(122, 45);
			this->b_list2->TabIndex = 19;
			this->b_list2->Text = L"������� 2";
			this->b_list2->UseVisualStyleBackColor = true;
			this->b_list2->Click += gcnew System::EventHandler(this, &App::b_list2_Click);
			// 
			// b_fTree
			// 
			this->b_fTree->Location = System::Drawing::Point(27, 443);
			this->b_fTree->Name = L"b_fTree";
			this->b_fTree->Size = System::Drawing::Size(122, 45);
			this->b_fTree->TabIndex = 20;
			this->b_fTree->Text = L"������ ������";
			this->b_fTree->UseVisualStyleBackColor = true;
			this->b_fTree->Click += gcnew System::EventHandler(this, &App::b_fTree_Click);
			// 
			// b_aTree
			// 
			this->b_aTree->Location = System::Drawing::Point(27, 518);
			this->b_aTree->Name = L"b_aTree";
			this->b_aTree->Size = System::Drawing::Size(122, 45);
			this->b_aTree->TabIndex = 21;
			this->b_aTree->Text = L"��� ������";
			this->b_aTree->UseVisualStyleBackColor = true;
			this->b_aTree->Click += gcnew System::EventHandler(this, &App::b_aTree_Click);
			// 
			// p_main
			// 
			this->p_main->AutoScroll = true;
			this->p_main->BackColor = System::Drawing::SystemColors::Control;
			this->p_main->Location = System::Drawing::Point(173, 286);
			this->p_main->Name = L"p_main";
			this->p_main->Size = System::Drawing::Size(724, 302);
			this->p_main->TabIndex = 22;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(917, 285);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(145, 20);
			this->label1->TabIndex = 23;
			this->label1->Text = L"������ ��������";
			// 
			// p_bestStudents
			// 
			this->p_bestStudents->AutoScroll = true;
			this->p_bestStudents->BackColor = System::Drawing::SystemColors::Control;
			this->p_bestStudents->Location = System::Drawing::Point(921, 309);
			this->p_bestStudents->Name = L"p_bestStudents";
			this->p_bestStudents->Size = System::Drawing::Size(151, 279);
			this->p_bestStudents->TabIndex = 24;
			// 
			// App
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->ClientSize = System::Drawing::Size(1084, 611);
			this->Controls->Add(this->p_bestStudents);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->p_main);
			this->Controls->Add(this->b_aTree);
			this->Controls->Add(this->b_fTree);
			this->Controls->Add(this->b_list2);
			this->Controls->Add(this->b_list1);
			this->Controls->Add(this->b_fileClose);
			this->Controls->Add(this->b_marksClear);
			this->Controls->Add(this->b_fioClear);
			this->Controls->Add(this->b_fileClear);
			this->Controls->Add(this->b_studentSend);
			this->Controls->Add(this->b_fileSend);
			this->Controls->Add(this->TB_Mark5);
			this->Controls->Add(this->TB_Mark4);
			this->Controls->Add(this->TB_Mark3);
			this->Controls->Add(this->TB_Mark2);
			this->Controls->Add(this->TB_Mark1);
			this->Controls->Add(this->l_marks);
			this->Controls->Add(this->TB_fio);
			this->Controls->Add(this->l_fio);
			this->Controls->Add(this->l_file);
			this->Controls->Add(this->TB_fileName);
			this->Controls->Add(this->Creators);
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(1100, 650);
			this->MinimumSize = System::Drawing::Size(1100, 650);
			this->Name = L"App";
			this->Text = L"App";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void b_fileSend_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_studentSend_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_fileClear_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_fioClear_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_marksClear_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_fileClose_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void ClearPanel(System::Windows::Forms::Panel^ p);
private: System::Void b_fTree_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_aTree_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void printTreeToPanel(Tree t, int lvl,/* int ind,*/ int w, int h);
private: System::Void printTreeToPanel(ATree t, int lvl, /*int ind,*/ int w, int h);
private: System::Void b_list1_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void b_list2_Click(System::Object^ sender, System::EventArgs^ e);
};
}
