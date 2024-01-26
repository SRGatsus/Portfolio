#pragma once

#include "DLL/Include/Lusbapi.h"
#include"E14-140/Examples/MicroSoft Visual C++ 6.0/DynLoad/LoadDll.h"
#include "DLL/Source/Lusbapi/Lusbbase.h"
#define CHANNELS_QUANTITY			(0x4)
using namespace System::Collections::Generic;

namespace CourseProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	class Axes
	{
	public:
		Axes() {
			Init();
		}
		void Init() {
			Factor = 1;
			BufferIndex = 5;
			Active = true;
			Delta = 0.1;
			Last = 0;
		}
		int Factor = 1;
		int BufferIndex = 5;
		bool Active = true;
		float Delta = 0.1;
		float Last = 0;
	};
	/// <summary>
	/// Сводка для Mainh
	/// </summary>
	public ref class Mainh : public System::Windows::Forms::Form
	{
	public:
		Mainh(void)
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
		~Mainh()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveAsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ Program_Menu;
	private: System::Windows::Forms::ToolStripMenuItem^ Settings_SubMenu;

	private: System::Windows::Forms::ToolStripMenuItem^ About_SubMenu;
	private: System::Windows::Forms::Button^ Start;
	private: System::Windows::Forms::Button^ Pause;
	private: System::Windows::Forms::Button^ Stop;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ Graph;
	private: System::Windows::Forms::ContextMenuStrip^ contextMenuStrip1;
	private: System::Windows::Forms::Timer^ timer1;
	private: System::Windows::Forms::Label^ time;
	private: System::Windows::Forms::Timer^ timer2;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::Button^ ReloadConnect;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ Clear;


	private: System::ComponentModel::IContainer^ components;



	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(Mainh::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Program_Menu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Settings_SubMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->About_SubMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Start = (gcnew System::Windows::Forms::Button());
			this->Pause = (gcnew System::Windows::Forms::Button());
			this->Stop = (gcnew System::Windows::Forms::Button());
			this->Graph = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->time = (gcnew System::Windows::Forms::Label());
			this->timer2 = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->ReloadConnect = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->Clear = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Graph))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->Program_Menu
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(7, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(1067, 27);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openToolStripMenuItem,
					this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(58, 23);
			this->fileToolStripMenuItem->Text = L"Файл";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(176, 24);
			this->openToolStripMenuItem->Text = L"Открыть";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Mainh::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(176, 24);
			this->saveToolStripMenuItem->Text = L"Сохранить";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Mainh::saveToolStripMenuItem_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(176, 24);
			this->saveAsToolStripMenuItem->Text = L"Сохранить как";
			this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Mainh::saveAsToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(176, 24);
			this->exitToolStripMenuItem->Text = L"Выход";
			// 
			// Program_Menu
			// 
			this->Program_Menu->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->Settings_SubMenu,
					this->About_SubMenu
			});
			this->Program_Menu->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Program_Menu->Name = L"Program_Menu";
			this->Program_Menu->Size = System::Drawing::Size(96, 23);
			this->Program_Menu->Text = L"Программа";
			// 
			// Settings_SubMenu
			// 
			this->Settings_SubMenu->Name = L"Settings_SubMenu";
			this->Settings_SubMenu->Size = System::Drawing::Size(167, 24);
			this->Settings_SubMenu->Text = L"Настройки";
			this->Settings_SubMenu->Click += gcnew System::EventHandler(this, &Mainh::Settings_SubMenu_Click);
			// 
			// About_SubMenu
			// 
			this->About_SubMenu->Name = L"About_SubMenu";
			this->About_SubMenu->Size = System::Drawing::Size(167, 24);
			this->About_SubMenu->Text = L"О программе";
			this->About_SubMenu->Click += gcnew System::EventHandler(this, &Mainh::About_SubMenu_Click);
			// 
			// Start
			// 
			this->Start->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Start->Location = System::Drawing::Point(124, 528);
			this->Start->Name = L"Start";
			this->Start->Size = System::Drawing::Size(157, 47);
			this->Start->TabIndex = 1;
			this->Start->Text = L"Старт";
			this->Start->UseVisualStyleBackColor = true;
			this->Start->Click += gcnew System::EventHandler(this, &Mainh::Start_Click);
			// 
			// Pause
			// 
			this->Pause->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Pause->Location = System::Drawing::Point(321, 528);
			this->Pause->Name = L"Pause";
			this->Pause->Size = System::Drawing::Size(157, 47);
			this->Pause->TabIndex = 2;
			this->Pause->Text = L"Пауза";
			this->Pause->UseVisualStyleBackColor = true;
			this->Pause->Click += gcnew System::EventHandler(this, &Mainh::Pause_Click);
			// 
			// Stop
			// 
			this->Stop->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Stop->Location = System::Drawing::Point(512, 528);
			this->Stop->Name = L"Stop";
			this->Stop->Size = System::Drawing::Size(157, 47);
			this->Stop->TabIndex = 3;
			this->Stop->Text = L"Стоп";
			this->Stop->UseVisualStyleBackColor = true;
			this->Stop->Click += gcnew System::EventHandler(this, &Mainh::Stop_Click);
			// 
			// Graph
			// 
			chartArea1->Name = L"ChartArea1";
			this->Graph->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->Graph->Legends->Add(legend1);
			this->Graph->Location = System::Drawing::Point(12, 94);
			this->Graph->Name = L"Graph";
			series1->BorderWidth = 5;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Color = System::Drawing::Color::Red;
			series1->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			series1->Legend = L"Legend1";
			series1->MarkerColor = System::Drawing::Color::Black;
			series1->MarkerSize = 1;
			series1->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Square;
			series1->Name = L"Usb данные";
			series1->SmartLabelStyle->AllowOutsidePlotArea = System::Windows::Forms::DataVisualization::Charting::LabelOutsidePlotAreaStyle::Yes;
			series1->SmartLabelStyle->CalloutBackColor = System::Drawing::Color::Empty;
			series1->SmartLabelStyle->MinMovingDistance = 10;
			this->Graph->Series->Add(series1);
			this->Graph->Size = System::Drawing::Size(789, 402);
			this->Graph->TabIndex = 4;
			this->Graph->Text = L"chart1";
			this->Graph->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Mainh::Graph_MouseClick);
			this->Graph->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Mainh::Graph_MouseMove);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Mainh::timer1_Tick);
			// 
			// time
			// 
			this->time->AutoSize = true;
			this->time->Font = (gcnew System::Drawing::Font(L"Times New Roman", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->time->Location = System::Drawing::Point(317, 48);
			this->time->Name = L"time";
			this->time->Size = System::Drawing::Size(118, 21);
			this->time->TabIndex = 5;
			this->time->Text = L"Время: 0:0:0:0";
			this->time->Visible = false;
			// 
			// timer2
			// 
			this->timer2->Interval = 10;
			this->timer2->Tick += gcnew System::EventHandler(this, &Mainh::timer2_Tick);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->Filter = L"txt files (*.txt)|*.txt|All files (*.*)|*.*";
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->Filter = L"txt files (*.txt)|*.txt|All files (*.*)|*.*";
			// 
			// ReloadConnect
			// 
			this->ReloadConnect->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->ReloadConnect->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"ReloadConnect.Image")));
			this->ReloadConnect->Location = System::Drawing::Point(12, 41);
			this->ReloadConnect->Name = L"ReloadConnect";
			this->ReloadConnect->Size = System::Drawing::Size(32, 34);
			this->ReloadConnect->TabIndex = 6;
			this->ReloadConnect->UseVisualStyleBackColor = true;
			this->ReloadConnect->Click += gcnew System::EventHandler(this, &Mainh::ReloadConnect_Click);
			// 
			// textBox1
			// 
			this->textBox1->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->textBox1->Location = System::Drawing::Point(847, 94);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(154, 402);
			this->textBox1->TabIndex = 8;
			this->textBox1->Text = L"4\r\n4\r\n4\r\n4\r\n4\r\n\r\n\r\n4\r\n44\r\n\r\n44\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4"
				L"\r\n4\r\n\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n4\r\n44\r\n4\r\n4\r\n\r\n4";
			// 
			// Clear
			// 
			this->Clear->Font = (gcnew System::Drawing::Font(L"Times New Roman", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->Clear->Location = System::Drawing::Point(847, 528);
			this->Clear->Name = L"Clear";
			this->Clear->Size = System::Drawing::Size(154, 47);
			this->Clear->TabIndex = 9;
			this->Clear->Text = L"Очистить";
			this->Clear->UseVisualStyleBackColor = true;
			this->Clear->Click += gcnew System::EventHandler(this, &Mainh::Clear_Click);
			// 
			// Mainh
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1067, 587);
			this->Controls->Add(this->Clear);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->ReloadConnect);
			this->Controls->Add(this->time);
			this->Controls->Add(this->Graph);
			this->Controls->Add(this->Stop);
			this->Controls->Add(this->Pause);
			this->Controls->Add(this->Start);
			this->Controls->Add(this->menuStrip1);
			this->Font = (gcnew System::Drawing::Font(L"Times New Roman", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Mainh";
			this->Text = L"USB";
			this->Load += gcnew System::EventHandler(this, &Mainh::Mainh_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Graph))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Settings_SubMenu_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void About_SubMenu_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Mainh_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Start_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Pause_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Stop_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void ActiveButton();
	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void timer2_Tick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void UpdateTime(int ms, int(&masTime)[4]);
	private: System::Void AddXYinGraph(int x, int y);
	private: System::Void SaveFile();
	private: System::Void DeleteData();
	private: System::Void Graph_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void Graph_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void ReloadConnect_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void AbortProgram(char* ErrorString, bool AbortionFlag);
	private: System::Boolean WaitingForRequestCompleted(OVERLAPPED* ReadOv);
	private: System::Void ShowThreadErrorMessage(void);
	private: System::Void ConnectModule();
	private: System::Void StopTimer();
	private: bool EnableSearch = false;
	private: bool NotEnableSearch = true;

	public:
		// флажок завершения работы потока сбора данных
		bool IsReadThreadComplete;
		int rate = 15;
		int ms = 0;
		int Gain = 1;
		int IndexGain = 0;
		char* ModuleName;
		const double AdcRate = 100.0;
		const WORD MaxVirtualSoltsQuantity = 127;
		const WORD NDataBlock = 80;
		const WORD InputRangeIndex = ADC_INPUT_RANGE_2500mV_E140;
		String^ saveFile;
		String^ mesError;
		// идентификатор потока сбора данных
		HANDLE hReadThread;
		// идентификатор файла
		HANDLE hFile;
		// дескриптор устройства
		HANDLE ModuleHandle;
		DWORD* ReadTid;
		// кол-во получаемых отсчетов (кратное 32) для Ф. ReadData()
		DWORD DataStep = 64 * 1024;
		// экранный счетчик-индикатор
		DWORD Counter = 0x0, OldCounter = 0xFFFFFFFF;
		// версия библиотеки
		DWORD DllVersion;
		// будем собирать NDataBlock блоков по DataStep отсчётов в каждом
		WORD ReadThreadErrorNumber;
		// буфер данных
		SHORT* ReadBuffer;
		// указатель на класс динамической загрузки DLL
		TLoadDll* pLoadDll;
		// указатель на интерфейс модуля
		ILE140* pModule;
		MODULE_DESCRIPTION_E140* ModuleDescription;
		// структура параметров работы АЦП модуля
		ADC_PARS_E140* ap;
		Axes* X;
		Axes* Y;




private: System::Void vScrollBar1_Scroll(System::Object^ sender, System::Windows::Forms::ScrollEventArgs^ e) {
}
private: System::Void Clear_Click(System::Object^ sender, System::EventArgs^ e) {
	textBox1->Text = "";
}
};

}
