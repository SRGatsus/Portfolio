#pragma once

namespace CourseProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для About
	/// </summary>
	public ref class About : public System::Windows::Forms::Form
	{
	public:
		About(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~About()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ About_Text;
	private: System::Windows::Forms::Button^ About_Button;
	private: System::Windows::Forms::Label^ ModulesViewer;
	private: System::Windows::Forms::Label^ Lusbapi;
	private: System::Windows::Forms::Label^ Ldevusb;
	private: System::Windows::Forms::Label^ Version_MV;
	private: System::Windows::Forms::Label^ Version_Lusbapi;
	private: System::Windows::Forms::Label^ Version_Ldevusb;
	private: System::Windows::Forms::GroupBox^ groupBox1;







	protected:

	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->About_Text = (gcnew System::Windows::Forms::TextBox());
			this->About_Button = (gcnew System::Windows::Forms::Button());
			this->ModulesViewer = (gcnew System::Windows::Forms::Label());
			this->Lusbapi = (gcnew System::Windows::Forms::Label());
			this->Ldevusb = (gcnew System::Windows::Forms::Label());
			this->Version_MV = (gcnew System::Windows::Forms::Label());
			this->Version_Lusbapi = (gcnew System::Windows::Forms::Label());
			this->Version_Ldevusb = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// About_Text
			// 
			this->About_Text->AccessibleRole = System::Windows::Forms::AccessibleRole::Text;
			this->About_Text->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->About_Text->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->About_Text->HideSelection = false;
			this->About_Text->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->About_Text->Location = System::Drawing::Point(12, 12);
			this->About_Text->Multiline = true;
			this->About_Text->Name = L"About_Text";
			this->About_Text->ReadOnly = true;
			this->About_Text->Size = System::Drawing::Size(410, 184);
			this->About_Text->TabIndex = 1;
			this->About_Text->Text = L"Разработчик: Коротаев Роман Семенович\r\nГруппа: ИВТБ - 3301\r\nMail: mailto:kychin.r"
				L"oman@yandex.ru\r\nЦель:....\r\nПоддерживаются следующие внешние USB модули:\r\n    1. "
				L" E14-140";
			this->About_Text->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->About_Text->TextChanged += gcnew System::EventHandler(this, &About::About_Text_TextChanged);
			// 
			// About_Button
			// 
			this->About_Button->Location = System::Drawing::Point(130, 338);
			this->About_Button->Name = L"About_Button";
			this->About_Button->Size = System::Drawing::Size(150, 40);
			this->About_Button->TabIndex = 2;
			this->About_Button->Text = L"ОК";
			this->About_Button->UseVisualStyleBackColor = true;
			// 
			// ModulesViewer
			// 
			this->ModulesViewer->AutoSize = true;
			this->ModulesViewer->Location = System::Drawing::Point(6, 28);
			this->ModulesViewer->Name = L"ModulesViewer";
			this->ModulesViewer->Size = System::Drawing::Size(108, 19);
			this->ModulesViewer->TabIndex = 0;
			this->ModulesViewer->Text = L"ModulesViewer:";
			// 
			// Lusbapi
			// 
			this->Lusbapi->AutoSize = true;
			this->Lusbapi->Location = System::Drawing::Point(6, 56);
			this->Lusbapi->Name = L"Lusbapi";
			this->Lusbapi->Size = System::Drawing::Size(60, 19);
			this->Lusbapi->TabIndex = 1;
			this->Lusbapi->Text = L"Lusbapi:";
			// 
			// Ldevusb
			// 
			this->Ldevusb->AutoSize = true;
			this->Ldevusb->Location = System::Drawing::Point(6, 84);
			this->Ldevusb->Name = L"Ldevusb";
			this->Ldevusb->Size = System::Drawing::Size(68, 19);
			this->Ldevusb->TabIndex = 2;
			this->Ldevusb->Text = L" Ldevusb:";
			// 
			// Version_MV
			// 
			this->Version_MV->Location = System::Drawing::Point(119, 28);
			this->Version_MV->Name = L"Version_MV";
			this->Version_MV->Size = System::Drawing::Size(149, 19);
			this->Version_MV->TabIndex = 3;
			this->Version_MV->Text = L"--------------";
			// 
			// Version_Lusbapi
			// 
			this->Version_Lusbapi->Location = System::Drawing::Point(119, 56);
			this->Version_Lusbapi->Name = L"Version_Lusbapi";
			this->Version_Lusbapi->Size = System::Drawing::Size(149, 19);
			this->Version_Lusbapi->TabIndex = 4;
			this->Version_Lusbapi->Text = L"--------------";
			// 
			// Version_Ldevusb
			// 
			this->Version_Ldevusb->Location = System::Drawing::Point(119, 84);
			this->Version_Ldevusb->Name = L"Version_Ldevusb";
			this->Version_Ldevusb->Size = System::Drawing::Size(149, 19);
			this->Version_Ldevusb->TabIndex = 5;
			this->Version_Ldevusb->Text = L"--------------";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->Version_Ldevusb);
			this->groupBox1->Controls->Add(this->Version_Lusbapi);
			this->groupBox1->Controls->Add(this->Version_MV);
			this->groupBox1->Controls->Add(this->Ldevusb);
			this->groupBox1->Controls->Add(this->Lusbapi);
			this->groupBox1->Controls->Add(this->ModulesViewer);
			this->groupBox1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->groupBox1->Location = System::Drawing::Point(12, 207);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(409, 116);
			this->groupBox1->TabIndex = 3;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Версии";
			this->groupBox1->Visible = false;
			// 
			// About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(434, 395);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->About_Button);
			this->Controls->Add(this->About_Text);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"About";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"О программе";
			this->Load += gcnew System::EventHandler(this, &About::About_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void About_Load(System::Object^ sender, System::EventArgs^ e);
	
private: System::Void About_Text_TextChanged(System::Object^ sender, System::EventArgs^ e) {
}
};
}