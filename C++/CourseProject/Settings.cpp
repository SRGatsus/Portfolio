#include "Settings.h"
#include <iostream>
#include <windows.h>
System::Void CourseProject::Settings::Settings_Load(System::Object^ sender, System::EventArgs^ e)
{
	Settings::factorX->Text = form->X->Factor.ToString();
	Settings::factorY->Text = form->Y->Factor.ToString();
	Settings::rate->Text = form->rate.ToString();
	checkBox_Y->Checked = Enable_Y;
	checkBox_X->Checked = Enable_X;
	comboBox_Y->SelectedIndex = Index_Y;
	comboBox_X->SelectedIndex = Index_X;
	comboBox1->SelectedIndex = IndexGain;
	ActiveButton();
	
}

System::Void CourseProject::Settings::factorX_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e)
{
	form->X->Factor = Int32::Parse(Settings::factorX->Text);
}

System::Void CourseProject::Settings::factorY_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e)
{
	form->Y->Factor = Int32::Parse(Settings::factorY->Text);
}

System::Void CourseProject::Settings::rate_MaskInputRejected(System::Object^ sender, System::Windows::Forms::MaskInputRejectedEventArgs^ e)
{
	form->rate = Int32::Parse(Settings::rate->Text);
}

System::Void CourseProject::Settings::Settings_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
{
	form->X->Factor = Int32::Parse(Settings::factorX->Text);
	form->Y->Factor = Int32::Parse(Settings::factorY->Text);
	form->rate = Int32::Parse(Settings::rate->Text);
	form->X->Active= Enable_X;
	form->Y->Active= Enable_Y;
	form->X->BufferIndex= Index_X;
	form->Y->BufferIndex= Index_Y;
	form->Gain= Gain;
	form->IndexGain= IndexGain;
}

System::Void CourseProject::Settings::rate_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	char number = e->KeyChar;
	Console::WriteLine(number);
	if (((number>='1' && '9'<=number) && '0' != number && number != (char)8))
	{
		e->Handled = true;
		Beep(300, 500);
	}

}

System::Void CourseProject::Settings::factorY_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	char number = e->KeyChar;

	if (((number >= '1' && '9' <= number) && '0' != number && number != (char)8))
	{
		e->Handled = true;
		Beep(300, 500);
	}

}

System::Void CourseProject::Settings::factorX_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
		char number = e->KeyChar;

	if (((number >= '1' && '9' <= number) && '0' != number && number != (char)8))
	{
		e->Handled = true;
		Beep(300, 500);
	}

}

System::Void CourseProject::Settings::ActiveButton()
{
	factorX->Enabled = NotEnableSearch;
	factorY->Enabled = NotEnableSearch;
	rate->Enabled = NotEnableSearch;
	checkBox_X->Enabled = NotEnableSearch;
	checkBox_Y->Enabled = NotEnableSearch;
	comboBox_X->Enabled = NotEnableSearch && Enable_X;
	comboBox_Y->Enabled = NotEnableSearch && Enable_Y;
	
}

System::Void CourseProject::Settings::comboBox2_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	Index_Y = comboBox_Y->SelectedIndex;
}

System::Void CourseProject::Settings::checkBox_Y_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	Enable_Y = checkBox_Y->Checked;
	comboBox_Y->Enabled = checkBox_Y->Checked;
}

System::Void CourseProject::Settings::checkBox_X_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	Enable_X= checkBox_X->Checked;
	comboBox_X->Enabled = checkBox_X->Checked;
}

System::Void CourseProject::Settings::comboBox_X_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	Index_X = comboBox_X->SelectedIndex;
}

System::Void CourseProject::Settings::comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	Gain = pow( 4,comboBox1->SelectedIndex);
	IndexGain = comboBox1->SelectedIndex;
}
