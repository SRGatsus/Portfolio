#pragma once
#include "Mainh.h"
#include <cmath>
namespace CourseProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для Settings
	/// </summary>
	public ref class Settings : public System::Windows::Forms::Form
	{
	public:
		Settings(void)
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
		~Settings()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ Label_K_X;
	protected:
	private: System::Windows::Forms::Label^ label1;




	private: System::Windows::Forms::Label^ updateRate;
	private: System::Windows::Forms::MaskedTextBox^ factorX;
	private: System::Windows::Forms::MaskedTextBox^ factorY;
	private: System::Windows::Forms::MaskedTextBox^ rate;
	private: System::Windows::Forms::ComboBox^ comboBox_X;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::ComboBox^ comboBox_Y;
	private: System::Windows::Forms::CheckBox^ checkBox_X;
	private: System::Windows::Forms::CheckBox^ checkBox_Y;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::ComboBox^ comboBox1;






	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Label_K_X = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->updateRate = (gcnew System::Windows::Forms::Label());
			this->factorX = (gcnew System::Windows::Forms::MaskedTextBox());
			this->factorY = (gcnew System::Windows::Forms::MaskedTextBox());
			this->rate = (gcnew System::Windows::Forms::MaskedTextBox());
			this->comboBox_X = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Y = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox_X = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_Y = (gcnew System::Windows::Forms::CheckBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->SuspendLayout();
			// 
			// Label_K_X
			// 
			this->Label_K_X->AutoSize = true;
			this->Label_K_X->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Label_K_X->Location = System::Drawing::Point(22, 18);
			this->Label_K_X->Name = L"Label_K_X";
			this->Label_K_X->Size = System::Drawing::Size(140, 19);
			this->Label_K_X->TabIndex = 0;
			this->Label_K_X->Text = L"Kоэффициент по X";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(22, 48);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(140, 19);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Kоэффициент по Y";
			// 
			// updateRate
			// 
			this->updateRate->AutoSize = true;
			this->updateRate->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->updateRate->Location = System::Drawing::Point(22, 78);
			this->updateRate->Name = L"updateRate";
			this->updateRate->Size = System::Drawing::Size(147, 19);
			this->updateRate->TabIndex = 4;
			this->updateRate->Text = L"Частота обновления";
			// 
			// factorX
			// 
			this->factorX->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->factorX->Location = System::Drawing::Point(177, 11);
			this->factorX->Name = L"factorX";
			this->factorX->Size = System::Drawing::Size(100, 26);
			this->factorX->TabIndex = 6;
			this->factorX->MaskInputRejected += gcnew System::Windows::Forms::MaskInputRejectedEventHandler(this, &Settings::factorX_MaskInputRejected);
			this->factorX->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::factorX_KeyPress);
			// 
			// factorY
			// 
			this->factorY->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->factorY->Location = System::Drawing::Point(177, 43);
			this->factorY->Name = L"factorY";
			this->factorY->Size = System::Drawing::Size(100, 26);
			this->factorY->TabIndex = 7;
			this->factorY->ValidatingType = System::Int32::typeid;
			this->factorY->MaskInputRejected += gcnew System::Windows::Forms::MaskInputRejectedEventHandler(this, &Settings::factorY_MaskInputRejected);
			this->factorY->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::factorY_KeyPress);
			// 
			// rate
			// 
			this->rate->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->rate->Location = System::Drawing::Point(177, 75);
			this->rate->Name = L"rate";
			this->rate->Size = System::Drawing::Size(100, 26);
			this->rate->TabIndex = 8;
			this->rate->ValidatingType = System::Int32::typeid;
			this->rate->MaskInputRejected += gcnew System::Windows::Forms::MaskInputRejectedEventHandler(this, &Settings::rate_MaskInputRejected);
			this->rate->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Settings::rate_KeyPress);
			// 
			// comboBox_X
			// 
			this->comboBox_X->DisplayMember = L"0";
			this->comboBox_X->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBox_X->FormattingEnabled = true;
			this->comboBox_X->Items->AddRange(gcnew cli::array< System::Object^  >(16) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",
					L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16"
			});
			this->comboBox_X->Location = System::Drawing::Point(177, 107);
			this->comboBox_X->Name = L"comboBox_X";
			this->comboBox_X->Size = System::Drawing::Size(100, 27);
			this->comboBox_X->TabIndex = 9;
			this->comboBox_X->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::comboBox_X_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(22, 110);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(79, 19);
			this->label2->TabIndex = 10;
			this->label2->Text = L"Порт по X";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(22, 143);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(79, 19);
			this->label3->TabIndex = 12;
			this->label3->Text = L"Порт по Y";
			// 
			// comboBox_Y
			// 
			this->comboBox_Y->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBox_Y->FormattingEnabled = true;
			this->comboBox_Y->Items->AddRange(gcnew cli::array< System::Object^  >(16) {
				L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8",
					L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16"
			});
			this->comboBox_Y->Location = System::Drawing::Point(177, 140);
			this->comboBox_Y->Name = L"comboBox_Y";
			this->comboBox_Y->Size = System::Drawing::Size(100, 27);
			this->comboBox_Y->TabIndex = 11;
			this->comboBox_Y->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::comboBox2_SelectedIndexChanged);
			// 
			// checkBox_X
			// 
			this->checkBox_X->Location = System::Drawing::Point(141, 111);
			this->checkBox_X->Name = L"checkBox_X";
			this->checkBox_X->Size = System::Drawing::Size(21, 21);
			this->checkBox_X->TabIndex = 13;
			this->checkBox_X->UseVisualStyleBackColor = true;
			this->checkBox_X->CheckedChanged += gcnew System::EventHandler(this, &Settings::checkBox_X_CheckedChanged);
			// 
			// checkBox_Y
			// 
			this->checkBox_Y->Location = System::Drawing::Point(141, 144);
			this->checkBox_Y->Name = L"checkBox_Y";
			this->checkBox_Y->Size = System::Drawing::Size(21, 21);
			this->checkBox_Y->TabIndex = 14;
			this->checkBox_Y->UseVisualStyleBackColor = true;
			this->checkBox_Y->CheckedChanged += gcnew System::EventHandler(this, &Settings::checkBox_Y_CheckedChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(22, 176);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(135, 19);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Входной диапозон";
			// 
			// comboBox1
			// 
			this->comboBox1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"±10", L"±2.5", L"±0.625", L"±0.15625" });
			this->comboBox1->Location = System::Drawing::Point(177, 173);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(100, 27);
			this->comboBox1->TabIndex = 15;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Settings::comboBox1_SelectedIndexChanged);
			// 
			// Settings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(409, 318);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->checkBox_Y);
			this->Controls->Add(this->checkBox_X);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_Y);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_X);
			this->Controls->Add(this->rate);
			this->Controls->Add(this->factorY);
			this->Controls->Add(this->factorX);
			this->Controls->Add(this->updateRate);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->Label_K_X);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Name = L"Settings";
			this->Text = L"Настройки";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Settings::Settings_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Settings::Settings_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		private: System::Void Settings_Load(System::Object^ sender, System::EventArgs^ e);
		private: System::Void factorX_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e);
		private: System::Void factorY_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e);
		private: System::Void rate_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e);
		public:Mainh^ form;
		public:bool NotEnableSearch = true;
		public:bool Enable_X = true;
		public:bool Enable_Y = true;
		public:int Index_X = 0;
		public:int Index_Y = 0;
		public:int Gain = 0;
		public:int IndexGain = 0;
		private: System::Void Settings_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
		private: System::Void rate_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
		private: System::Void factorY_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
		private: System::Void factorX_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
		private: System::Void ActiveButton();

		private: System::Void comboBox2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		private: System::Void checkBox_Y_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		private: System::Void checkBox_X_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		private: System::Void comboBox_X_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
};
}
