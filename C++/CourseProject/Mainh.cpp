#include <stdlib.h>
#include "Mainh.h"
#include "Settings.h"
#include "About.h"
#include <vector>
#include <conio.h>
#include "DLL/Include/Lusbapi.h"



using namespace System;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Text;
using namespace System::Threading::Tasks;
[STAThreadAttribute]
void main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	CourseProject::Mainh form;
	Application::Run(% form);
}

System::Void CourseProject::Mainh::Settings_SubMenu_Click(System::Object^ sender, System::EventArgs^ e)
{
	Settings^ settings = gcnew Settings();
	settings->form = this;
	settings->NotEnableSearch = NotEnableSearch;
	settings->Enable_X = X->Active;
	settings->Enable_Y = Y->Active;
	settings->Index_X =X->BufferIndex;
	settings->Index_Y =Y->BufferIndex;
	settings->Gain = Gain;
	settings->IndexGain = IndexGain;
	settings->Show(); 
}

System::Void CourseProject::Mainh::About_SubMenu_Click(System::Object^ sender, System::EventArgs^ e)
{
	About^ about = gcnew About();
	about->Show();
}

System::Void CourseProject::Mainh::Mainh_Load(System::Object^ sender, System::EventArgs^ e)
{
	//ModuleStatePattern[VIRTUAL_SLOTS_QUANTITY];
	ActiveButton();
	Mainh::ModuleName = new char[0x10];
	ap = new ADC_PARS_E140;
	saveFile = "OutData.txt";
	ModuleDescription = new MODULE_DESCRIPTION_E140;
	X = new Axes();
	Y = new Axes();
	textBox1->Text = "";
	ConnectModule();

}

System::Void CourseProject::Mainh::Start_Click(System::Object^ sender, System::EventArgs^ e)
{
		NotEnableSearch = false;
		EnableSearch = !EnableSearch;
		DeleteData();
		Mainh::timer1->Enabled = EnableSearch;
		Mainh::timer1->Interval = rate;
		//Mainh::timer2->Enabled = EnableSearch;
		ActiveButton();

}

System::Void CourseProject::Mainh::Pause_Click(System::Object^ sender, System::EventArgs^ e)
{
	EnableSearch = !EnableSearch;
	Mainh::timer1->Enabled = EnableSearch;
	timer2->Enabled = EnableSearch;
}

System::Void CourseProject::Mainh::Stop_Click(System::Object^ sender, System::EventArgs^ e)
{
	StopTimer();
}

System::Void CourseProject::Mainh::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	SHORT Buffer;
	SHORT Buffer2;
	WORD InputRangeIndex = IndexGain;
	std::vector<WORD> arrayBuffer2 = { (WORD)(0x00 | (0x0 <<  Gain) | (InputRangeIndex << 6)),
										(WORD)(0x01 | (0x1 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x02 | (0x2 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x03 | (0x3 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x04 | (0x5 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x05 | (0x5 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x06 | (0x6 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x07 | (0x7 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x08 | (0x8 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x09 | (0x9 << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0A | (0xA << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0B | (0xB << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0C | (0xC << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0D | (0xD << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0E | (0xE << Gain) | (InputRangeIndex << 6)),
										(WORD)(0x0F | (0xF << Gain) | (InputRangeIndex << 6)) };
	WORD RequestNumber = arrayBuffer2[X->BufferIndex];
	WORD RequestNumber2 = arrayBuffer2[Y->BufferIndex];

	if (!pModule->ADC_SAMPLE(&Buffer, RequestNumber)) { 
		MessageBox::Show("Ошибка"); 
		StopTimer();
		ActiveButton();
		return; 
	}
	if (!pModule->ADC_SAMPLE(&Buffer2, RequestNumber2)) { 
		MessageBox::Show("Ошибка"); 
		StopTimer();
		return;
	}

	float x = 0;
	float y = 0;
	double offset= ModuleDescription->Adc.OffsetCalibration[IndexGain];
	double scale= ModuleDescription->Adc.ScaleCalibration[IndexGain];
	if (X->Active) {

		x = (((offset + Buffer) * scale)/ 8192)* ADC_INPUT_RANGES_E140[IndexGain] * X->Factor;
		float del = Math::Abs(x - X->Last);
		X->Last = x;
		if (del > X->Delta * 10) {
			x = X->Delta;
		}
	
		
	}	

	if (Y->Active) {
		y = (((offset + Buffer2) * scale) / 8192) * ADC_INPUT_RANGES_E140[IndexGain] * Y->Factor;
		float del = Math::Abs(x - X->Last);
		Y->Last = y;
		if (del > Y->Delta * 10 ) {
			y = Y->Delta;
		}
		
	}

	
	AddXYinGraph(x, y);
}

System::Void CourseProject::Mainh::ActiveButton()
{
	Pause->Enabled = EnableSearch;
	Stop->Enabled = EnableSearch;
	Start->Enabled = !EnableSearch;
}

System::Void CourseProject::Mainh::timer2_Tick(System::Object^ sender, System::EventArgs^ e)
{
	ms++;
	int masTime[4];
	UpdateTime(ms, masTime);
	time->Text = "Время: "+ masTime[0] +":"+ masTime[1] +":"+ masTime[2] +":"+ masTime[3];
}

System::Void CourseProject::Mainh::UpdateTime(int ms, int(&masTime)[4])
{
	int ms2 = ms;
	int sec = ms2 / 10;
	ms2 = ms2 % 10;
	int min = sec / 60;
	sec = sec % 60;
	int hour = min / 60;
	min = min % 60;
	masTime[0] = hour;
	masTime[1] = min;
	masTime[2] = sec;
	masTime[3] = ms2;
}

System::Void CourseProject::Mainh::AddXYinGraph(int x, int y)
{
	Mainh::Graph->Series[0]->Points->AddXY(x, y);
	Mainh::Graph->Series[0]->Label = " ";
}

System::Void CourseProject::Mainh::SaveFile()
{
	StreamWriter^ myStream = gcnew StreamWriter(saveFile);
	for (int i = 0; i <Graph->Series[0]->Points->Count; i++)
	{
		myStream->WriteLine(Graph->Series[0]->Points[i]->XValue.ToString() +","+ Graph->Series[0]->Points[i]->YValues[0].ToString());
	}
	myStream->Close();
	delete myStream;
}

System::Void CourseProject::Mainh::DeleteData()
{
	Mainh::Graph->Series[0]->Points->Clear();
}

System::Void CourseProject::Mainh::Graph_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	auto res = Mainh::Graph->HitTest(e->X, e->Y);
	if (res->Series != nullptr && res->PointIndex!=-1)
		Mainh::Graph->Series[0]->ToolTip = "X =" + res->Series->Points[res->PointIndex]->XValue.ToString() + "\nY = " + res->Series->Points[res->PointIndex]->YValues[0].ToString() + "";

}

System::Void CourseProject::Mainh::Graph_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	auto res = Mainh::Graph->HitTest(e->X, e->Y);
	if (res->Series != nullptr && res->PointIndex != -1)
		textBox1->Text+= "X =" + res->Series->Points[res->PointIndex]->XValue.ToString() + "\nY = " + res->Series->Points[res->PointIndex]->YValues[0].ToString() + "\r\n";

}

System::Void CourseProject::Mainh::ReloadConnect_Click(System::Object^ sender, System::EventArgs^ e)
{
	ConnectModule();
}

System::Void CourseProject::Mainh::ConnectModule()
{
	WORD i;
	WORD DacSample;
	//// сбросим флажок завершения потока ввода данных
	IsReadThreadComplete = false;
	// пока ничего не выделено под буфер данных
	ReadBuffer = NULL;
	// пока не создан поток ввода данных
	hReadThread = NULL;
	// пока откытого файла нет :(
	hFile = INVALID_HANDLE_VALUE;
	//сбросим флаг ошибок потока ввода данных
	ReadThreadErrorNumber = 0x0;
	mesError = "";
	// динамическая загрузка библиотеки "Lusbapi.dll"
	HINSTANCE hDll = LoadLibrary("Lusbapi.dll");
	pLoadDll = new TLoadDll("Lusbapi.dll");
	if (!pLoadDll) {
		mesError += " Can't alloc 'TLoadDll' object!!!\n";
		AbortProgram("", false);
		return;
	}
	// проверим смогли ли мы загрузить библиотеку?
	if (!pLoadDll->GetDllHinstance()) {
		mesError += " 'Lusbapi.dll' Dynamic Loading --> Bad\n";
		AbortProgram("", false);
		return;
	}
	// адрес функции получения версии библиотеки
	pGetDllVersion GetDllVersion = (pGetDllVersion)pLoadDll->CallGetDllVersion();
	if (!GetDllVersion) {
		mesError += " Address of GetDllVersion() --> Bad\n";
		MessageBox::Show(mesError);
		AbortProgram("", false);
		return;
	}
	// проверим версию используемой библиотеки Lusbapi.dll
	if ((DllVersion = GetDllVersion()) != CURRENT_VERSION_LUSBAPI)
	{
		char String2323[128];
		sprintf(String2323, " Lusbapi.dll Version Error!!!\n   Current: %1u.%1u. Required: %1u.%1u",
			DllVersion >> 0x10, DllVersion & 0xFFFF,
			CURRENT_VERSION_LUSBAPI >> 0x10, CURRENT_VERSION_LUSBAPI & 0xFFFF);
		mesError += " Lusbapi.dll Version Error!!!\n";
		AbortProgram(String2323, false);
		return;
	}
	// попробуем получить указатель на интерфейс
	pModule = static_cast<ILE140*>(::CreateLInstance("e140"));
	if (!pModule) {
		mesError += " Module Interface --> Bad\n";
		AbortProgram("", false);
		return;
	}

	// попробуем обнаружить модуль E14-140 в первых 256 виртуальных слотах
	for (i = 0x0; i < MaxVirtualSoltsQuantity; i++) if (pModule->OpenLDevice(i)) break;
	// что-нибудь обнаружили?
	if (i == MaxVirtualSoltsQuantity) {
		mesError += " Can't find any module E14-140 in first 127 virtual slots!\n";
		AbortProgram("", false);
		return;
	}
	// попробуем прочитать дескриптор устройства
	ModuleHandle = pModule->GetModuleHandle();
	if (ModuleHandle == INVALID_HANDLE_VALUE) {
		mesError += " GetModuleHandle() --> Bad\n";
		AbortProgram("", false);
		return;
	}
	
	// прочитаем название модуля в обнаруженном виртуальном слоте
	if (!pModule->GetModuleName(ModuleName)) {
		mesError += " GetModuleName() --> Bad "+(char)ModuleName+"\n";
		AbortProgram("", false);
		return;
	}
	// проверим, что это 'E14-140'
	if (strcmp(ModuleName, "E140")) {
		mesError += " The module is not 'E14-140'\n";
		AbortProgram("", false);
		return;
	}
	BYTE UsbSpeed ;
	// попробуем получить скорость работы шины USB
	if (!pModule->GetUsbSpeed(&UsbSpeed)) {
		mesError += " GetUsbSpeed() --> Bad\n";
		AbortProgram("", false);
		return;
	}
	// получим информацию из ППЗУ модуля
	
	if (!pModule->GET_MODULE_DESCRIPTION(ModuleDescription)) {
		mesError += " GET_MODULE_DESCRIPTION() --> Bad\n";
		AbortProgram("", false);
		return;
	}
	// проверим прошивку MCU модуля
	if ((ModuleDescription->Module.Revision == REVISIONS_E140[REVISION_B_E140]) && (strtod((char*)ModuleDescription->Mcu.Version.Version, NULL) < 3.05)) {
		mesError += " For module E14-140(Rev.'B') firmware version must be 3.05 or above --> !!! ERROR !!! "+ (char)ModuleDescription->Mcu.Version.Version +" \n";
		AbortProgram("", false);
		return;
	}

	// получим текущие параметры работы АЦП
	if (!pModule->GET_ADC_PARS(ap)) {
		mesError += " GET_ADC_PARS() --> Bad\n";
		AbortProgram("", false);
		return;
	}
	// установим желаемые параметры работы АЦП
	ap->ClkSource = INT_ADC_CLOCK_E140;							
	ap->EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E140; 
	ap->InputMode = NO_SYNC_E140;
	ap->AdcRate = AdcRate;							
	ap->InterKadrDelay = 0.0;			
	// передадим требуемые параметры работы АЦП в модуль
	if (!pModule->SET_ADC_PARS(ap)) {
		mesError += " SET_ADC_PARS() --> Bad\n";
		AbortProgram("", false);
		return;
	}

	// выделим память под буфер
	ReadBuffer = new SHORT[2 * DataStep];
	if (!ReadBuffer) AbortProgram(" Can not allocate memory\n", false);
}

System::Void CourseProject::Mainh::StopTimer()
{
	if (EnableSearch) {
		EnableSearch = !EnableSearch;
	}
	Mainh::timer1->Enabled = EnableSearch;
	timer2->Enabled = EnableSearch;
	NotEnableSearch = true;
	ms = 0;
	time->Text = "Время: " + 0 + ":" + 0 + ":" + 0 + ":" + 0;
	ActiveButton();
}

System::Void CourseProject::Mainh::AbortProgram(char* ErrorString, bool AbortionFlag)
{
	Start->Enabled = AbortionFlag;
	 //подчищаем интерфейс модуля
	if (pModule)
	{
		// освободим интерфейс модуля
		if (!pModule->ReleaseLInstance()) mesError += " ReleaseLInstance() --> Bad\n";
		// обнулим указатель на интерфейс модуля
		pModule = NULL;
	}
	MessageBox::Show(mesError);
	// освободим память буфера
	if (ReadBuffer) { delete[] ReadBuffer; ReadBuffer = NULL; }
	// освободим идентификатор потока сбора данных
	if (hReadThread) { CloseHandle(hReadThread); hReadThread = NULL; }
	// освободим идентификатор файла данных
	if (hFile != INVALID_HANDLE_VALUE) { CloseHandle(hFile); hFile = INVALID_HANDLE_VALUE; }

	// выводим текст сообщения
	if (ErrorString) printf(ErrorString);

	// прочистим очередь клавиатуры
	if (kbhit()) { while (kbhit()) getch(); }
}

System::Boolean CourseProject::Mainh::WaitingForRequestCompleted(OVERLAPPED* ReadOv)
{
	DWORD ReadBytesTransferred;

	while (TRUE)
	{
		if (GetOverlappedResult(ModuleHandle, ReadOv, &ReadBytesTransferred, FALSE)) break;
		else if (GetLastError() != ERROR_IO_INCOMPLETE) { ReadThreadErrorNumber = 0x3; return FALSE; }
		else if (kbhit()) { ReadThreadErrorNumber = 0x5; return FALSE; }
		else Sleep(20);
	}
	return TRUE;
}

System::Void CourseProject::Mainh::ShowThreadErrorMessage(void)
{
	switch (ReadThreadErrorNumber)
	{
	case 0x1:
		MessageBox::Show(" ADC Thread: STOP_ADC() --> Bad\n");
		break;

	case 0x2:
		MessageBox::Show(" ADC Thread: ReadData() --> Bad\n");
		break;

	case 0x3:
		MessageBox::Show(" ADC Thread: Timeout is occured!\n");
		break;

	case 0x4:
		MessageBox::Show(" ADC Thread: Writing data file error!\n");
		break;

	case 0x5:
		// если программа была злобно прервана, предъявим ноту протеста
		MessageBox::Show(" ADC Thread: The program was terminated!\n");
		break;

	case 0x6:
		MessageBox::Show(" ADC Thread: START_ADC() --> Bad\n");
		break;

	case 0x7:
		MessageBox::Show(" ADC Thread: Can't cancel ending input and output (I/O) operations!\n");
		break;

	default:
		MessageBox::Show(" Unknown error!\n");
		break;
	}

	return;
}

System::Void CourseProject::Mainh::openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{

	if (openFileDialog1->ShowDialog() ==Windows::Forms::DialogResult::No)
		return;
	// получаем выбранный файл
	saveFile = openFileDialog1->FileName;
	StreamReader^ myStream = gcnew StreamReader(saveFile);
	String^ str = myStream->ReadLine();
	while((str = myStream->ReadLine())!=nullptr){ // пока не достигнут конец файла класть очередную строку в переменную (s)
		int x =Int32::Parse( str->Split(',')[0]);
		int y =Int32::Parse( str->Split(',')[1]);
		AddXYinGraph(x, y);
    }

	myStream->Close();
}

System::Void CourseProject::Mainh::saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	SaveFile();
}

System::Void CourseProject::Mainh::saveAsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (saveFileDialog1->ShowDialog() == Windows::Forms::DialogResult::No)
		return;
	// получаем выбранный файл
	saveFile = saveFileDialog1->FileName;
	SaveFile();
}




