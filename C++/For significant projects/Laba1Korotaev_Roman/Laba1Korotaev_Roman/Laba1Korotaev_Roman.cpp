
#include "stdafx.h"
#include <iostream>
using namespace std;

bool isNumberUnpit(string s) {
	for (int a = 0; a < s.length(); a++) {
		// Если в строке есть не цифровые символы, то это не число
			if ((s[a] < 48) || (s[a] > 57))  return false;
	

	}
	// Если в строке только цифровые символы значит это число
	return true;
}
int inputArr(int len, vector<byte> &inputData)
{
	int inputElement;
	char Len[256];
	int elem = 0;
	byte elem1 = 0;
	int counter = 0;
	for (int i = 0; i < len; i++)
	{
		do {

			do
			{
				if (counter > 0) cout << "Ой вы ввели не ввели число меньше 0 , либо ввели больше 255" << endl;
				cout << " Коэффициент " << i + 1 << " элемента: ";
				cin >> Len;
				counter++;
			} while (!isNumberUnpit(Len));
			inputElement = atoi(Len);
		} while ((inputElement < 0) || (inputElement > 255));
		counter = 0;
		elem1 = inputElement;
		elem += elem1;
		inputData.push_back(inputElement);
		cout << "\n";
	}
	return elem;
}

int inputArr(int len, vector<int> &inputData)
{
	int inputElement;
	char Len[256];
	int elem = 0;
	for (int i = 0; i < len; i++)
	{
		do
		{
		cout << " Коэффициент " << i + 1 << " элемента: ";
		cin>>Len;
		} while (!isNumberUnpit(Len));
		inputElement=atoi(Len);
		elem += inputElement;
		inputData.push_back(inputElement);
		cout << "\n";
	}
	/*cout << "Уравнение равно: " << elem << "\n\n";*/
	return elem;
}

//определение число или строка
bool isNumber(string s) {
	for (int a = 0; a < (s.length()); a++) {
		// Если в строке есть не цифровые символы, то это не число
		if (s[a] != 43) { // Проверка на знак минус
			if ((s[a] < 48) || (s[a] > 57))  return false;
		}

	}
	// Если в строке только цифровые символы значит это число
	return true;
}




//установка размера массива
int setLength()
{
	/*string Len = "";*/
	char LenEquat[256];
	int counter = 0;
	int FGH = 0;
	float Flag = false;
	do
	{
		if (counter > 0) cout << "данные неверны" << endl;
		cout << "Количество переменных в уравнении:" << endl;
		cin>>LenEquat;
		cout << "\n";
		/*Flag = isNumber(Len);*/
		counter++;
	} while (!(isNumber(LenEquat)));
	FGH = atoi(LenEquat);
	return  FGH;
}

template<class Type>
int testing(vector<Type> data)
{
	int length = setLength();
	int  elem = inputArr(length, data);
	cout << "Уравнение равно: " << elem << "\n\n";
	return 1;

}


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	vector<int> dataInt;
	vector<byte> dataByte;
	char LenEquat[256];
	int variant;

	while (true)
	{
		system("cls");
		cout << "1. Int\n2. Byte\n3. Выход" << endl;
		cin >> LenEquat;
		if(isNumberUnpit(LenEquat)){ variant = atoi(LenEquat); }
		else { variant = 120; }
		switch (variant)
		{
		case 1:
		{
			testing(dataInt);
			system("pause");
			break;
		}
		case 2:
		{
			testing(dataByte);
			system("pause");
			break;
		}
		case 3:
		{
			system("pause");
			exit(0);
		}
		default:
		{
			cout << "Данная команда не определена" << endl;
			system("pause");
			break;
		}
		}
	}

}
