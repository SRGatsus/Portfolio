#include <algorithm>
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <TCHAR.H>
#include <vector>


using namespace std;

extern "C" int _stdcall _FindPath(int, int, int[][16], int, int*, int*, int);

HWND hWindow, hWndinp, hListBoxA, hListBoxB, texter, texter2;
HINSTANCE hInst;
LPCWSTR words[16] = { L"A",L"B",L"C",L"D",L"E",L"F",L"G",L"H",L"I",L"J",L"K",L"L",L"M",L"N",L"O",L"P" };
HWND but[16][16], col[16], row[16], finMat[16][16], colChild[16], rowChild[16];
int lastrow = -1, lastcol = -1;
bool FBstatus = false;
HWND ChildBut;
int dosMat[16][16], tempArr[16][16];
HWND edit, editcol, editrow, finalButtom, fillButton;
int TownCount = 1;
int DlinaPuti = 0;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProcChild(HWND, UINT, WPARAM, LPARAM);


//void FindPath(int end, int begin_index, int a[][16], int weight, int d[], int ver[], int k) {
//
//	// Восстановление пути
//
//	while (end != begin_index) // пока не дошли до начальной вершины
//	{
//		for (int i = 0; i < 16; i++) // просматриваем все вершины
//			if (a[i][end] != 0)   // если связь есть
//			{
//				int temp = weight - a[i][end]; // определяем вес пути из предыдущей вершины
//				if (temp == d[i]) // если вес совпал с рассчитанным
//				{                 // значит из этой вершины и был переход
//					weight = temp; // сохраняем новый вес
//					end = i;       // сохраняем предыдущую вершину
//					ver[k] = i + 1; // и записываем ее в массив
//					k++;
//				}
//			}
//	}
//
//}

vector <int> MinPut(int matr[][16], int n) // алгоритм Дейкстры
{
	int a[16][16]; // матрица связей
	int d[16]; // минимальное расстояние
	int v[16]; // посещенные вершины
	int temp, minindex, min;
	int begin_index = lastrow;

	// Инициализация матрицы связей
	for (int i = 0; i < n; i++) { // заполнение матрицы связей из матрицы, пришедшей в параметре функции
		for (int j = 0; j < n; j++) {
			a[i][j] = matr[i][j];
		}
	}

	//Инициализация вершин и расстояний
	for (int i = 0; i < 16; i++)
	{
		d[i] = 10000;
		v[i] = 1;
	}
	d[begin_index] = 0;
	// Шаг алгоритма
	do {
		minindex = 10000;
		min = 10000;
		for (int i = 0; i < 16; i++)
		{ // Если вершину ещё не обошли и вес меньше min
			if ((v[i] == 1) && (d[i] < min))
			{ // Переприсваиваем значения
				min = d[i];
				minindex = i;
			}
		}
		// Добавляем найденный минимальный вес
		// к текущему весу вершины
		// и сравниваем с текущим минимальным весом вершины
		if (minindex != 10000)
		{
			for (int i = 0; i < 16; i++)
			{
				if (a[minindex][i] > 0)
				{
					temp = min + a[minindex][i];
					if (temp < d[i])
					{
						d[i] = temp;
					}
				}
			}
			v[minindex] = 0;
		}
	} while (minindex < 10000);

	DlinaPuti = d[lastcol];

	// Восстановление пути
	int ver[16]; // массив посещенных вершин
	int end = lastcol; // индекс конечной вершины = 5 - 1
	ver[0] = end + 1; // начальный элемент - конечная вершина
	int k = 1; // индекс предыдущей вершины
	int weight = d[end]; // вес конечной вершины

	_FindPath(end, begin_index, a, weight, d, ver, k);


	// считаем количество вершин в конечном результате

	int count = 0;

	for (int i = 0; i < 16; ++i) {
		if (ver[i] > 0) {
			++count;
		}
	}

	// создаем и заполняем массив-вектор-ответ
	vector <int> otvet(count);
	for (int i = 0; i < count; ++i) {
		otvet[i] = ver[i];
	}

	return otvet;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	/**
	 *@param HNDCLASSEX wParentClass - Переменная родительского класса
	 *@param HNDCLASSEX wChildClass - Переменная дочернего класса
	 */
	WNDCLASSEX wParentClass, wChildClass;

	// Регистрация класса окна
	ZeroMemory(&wParentClass, sizeof(wParentClass)); // Обнуляем память
	ZeroMemory(&wChildClass, sizeof(wChildClass));

	wParentClass.cbSize = sizeof(WNDCLASSEX); //размер родительской структуры
	wParentClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 0); // Определяем фон окна
	wParentClass.hInstance = hInstance; // hInstance window
	wParentClass.lpfnWndProc = (WNDPROC)WndProc; // Процедура обработки окна
	wParentClass.lpszClassName = L"PClass"; // Имя класса

	wChildClass.cbSize = sizeof(WNDCLASSEX);
	wChildClass.hbrBackground = (HBRUSH)COLOR_DESKTOP;
	wChildClass.hInstance = hInstance;
	wChildClass.lpfnWndProc = (WNDPROC)WndProcChild;
	wChildClass.lpszClassName = L"CClass";

	if (!RegisterClassEx(&wChildClass))
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Класс окна не был создан!", L"Ошибка", MB_ICONERROR);
		return -1;
	}

	if (!RegisterClassEx(&wParentClass)) //Регестрируем класс окна
	{
		int nResult = GetLastError(); //Получаем значение кода последней ошибки
		MessageBox(NULL, L"Класс родительского окна не был создан!", L"Ошибка", MB_ICONWARNING);
		return -1;
	}
	hInst = hInstance;

	// Создаем родительское окно при помощи функции WinApi CreateWindowEx 
	hWindow = CreateWindowEx(
		NULL, //Расширенный стиль создаваемого окна
		L"PClass", // Имя класса, который мы определили ранее
		L"Поиск минимального пути в графе. Лаб 3. Бощаев Р. Р.", // Имя окна
		WS_OVERLAPPEDWINDOW, //Cтиль создаваемого окна
		100, 50,
		434, 600,
		NULL, //Дескриптор родительского окна
		NULL, //Дескриптор меню окна
		hInstance, // переменная экземпляра приложения
		NULL //Дополнительные данные кот доступны через указатель
	);

	if (!hWindow)
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Окно не было создано!", L"Ошибка", MB_ICONERROR);
		return 1;
	}

	hWndinp = CreateWindowEx(NULL, L"CClass", L"Ответ", WS_TILED, 100, 100, 436, 550, hWindow, NULL, hInstance, NULL);

	if (!hWndinp)
	{
		int nResult = GetLastError();
		MessageBox(NULL, L"Окно не было создано!", L"Ошибка", MB_ICONERROR);
		return -1;
	}

	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	// Объявляем переменную для сообщений типа MSG
	MSG msg;
	// Обнуляем память по размеру структуры MSG
	ZeroMemory(&msg, sizeof(MSG));

	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char* Text = NULL;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		CreateWindow(L"STATIC", L"Укажите количество вершин графа:", WS_VISIBLE | WS_CHILD, 20, 20, 378, 20, hwnd, NULL, NULL, NULL);
		edit = CreateWindow(L"EDIT", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 20, 50, 378, 20, hwnd, NULL, NULL, NULL);
		CreateWindow(L"BUTTON", L"Ввод", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 75, 90, 20, hwnd, (HMENU)1, NULL, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int param = LOWORD(wParam);
		switch (param) //макрас берущий младшие байты word из dword
		{
		case 1:
		{
			Text = new char[GetWindowTextLength(edit) + 1];
			GetWindowTextA(edit, Text, GetWindowTextLength(edit) + 1);
			TownCount = atoi(Text);
			delete[] Text;

			if (TownCount < 1 || TownCount>16)
			{
				SetWindowText(edit, L"1");
				MessageBox(hwnd, L"Введите числа от 1 до 16", L"Неправильный ввод", MB_ICONWARNING);
				return -1;
			}

			lastrow = -1;
			lastcol = -1;
			FBstatus = false;

			for (int i = 0; i < 16; i++)
			{
				DestroyWindow(col[i]);
				DestroyWindow(row[i]);
				for (int j = 0; j < 16; j++) //очистка
				{
					DestroyWindow(but[i][j]);
				}
			}

			DestroyWindow(finalButtom);
			finalButtom = CreateWindow(L"BUTTON", L"ПОИСК МИНИМАЛЬНОГО ПУТИ", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 22 * TownCount + 140, 378, 50, hwnd, (HMENU)4, NULL, NULL);
			EnableWindow(finalButtom, false);

			DestroyWindow(editcol);
			editcol = CreateWindow(L"STATIC", L"Первый:", WS_VISIBLE | WS_CHILD, 130, 75, 55, 20, hwnd, NULL, NULL, NULL);
			DestroyWindow(editrow);
			editrow = CreateWindow(L"STATIC", L"Второй:", WS_VISIBLE | WS_CHILD, 280, 75, 55, 20, hwnd, NULL, NULL, NULL);

			if (hListBoxA) DestroyWindow(hListBoxA); //создаём список 1
			hListBoxA = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 210, 75, 50, 25, hwnd, (HMENU)2, hInst, NULL);

			// Отменяем режим перерисовки списка 1
			SendMessage(hListBoxA, WM_SETREDRAW, FALSE, 0L);

			if (hListBoxB) DestroyWindow(hListBoxB); //создаём список 2
			hListBoxB = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD, 348, 75, 50, 25, hwnd, (HMENU)3, hInst, NULL);

			// Отменяем режим перерисовки списка 2
			SendMessage(hListBoxB, WM_SETREDRAW, FALSE, 0L);

			for (int i = 0; i < TownCount; i++) {
				SendMessage(hListBoxA, LB_ADDSTRING, 0, (LPARAM)(LPSTR)words[i]); //заполняем списки
				SendMessage(hListBoxB, LB_ADDSTRING, 0, (LPARAM)(LPSTR)words[i]);

				col[i] = CreateWindow(L"STATIC", words[i], WS_VISIBLE | WS_CHILD | ES_CENTER, 48 + i * 22, 105, 20, 20, hwnd, NULL, NULL, NULL);
				row[i] = CreateWindow(L"STATIC", words[i], WS_VISIBLE | WS_CHILD | ES_CENTER, 23, 130 + i * 22, 20, 20, hwnd, NULL, NULL, NULL);

				for (int j = 0; j < TownCount; j++) {
					if (i < j) {
						but[i][j] = CreateWindow(L"STATIC", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 48 + i * 22, 130 + j * 22, 20, 20, hwnd, NULL, NULL, NULL);
					}
					else if (i == j) {
						but[i][j] = CreateWindow(L"EDIT", L"0", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 48 + i * 22, 130 + j * 22, 20, 20, hwnd, NULL, NULL, NULL);
					}
					else {
						but[i][j] = CreateWindow(L"EDIT", L"1", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER, 48 + i * 22, 130 + j * 22, 20, 20, hwnd, NULL, NULL, NULL);
					}

				}
			}

			// Включаем режим перерисовки списка
			SendMessage(hListBoxA, WM_SETREDRAW, TRUE, 0L);
			// Перерисовываем список
			InvalidateRect(hListBoxA, NULL, TRUE);
			// Включаем режим перерисовки списка
			SendMessage(hListBoxB, WM_SETREDRAW, TRUE, 0L);
			// Перерисовываем список
			InvalidateRect(hListBoxB, NULL, TRUE);
			break;
		}
		case 2:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				int uSelectedItem;
				// Определяем номер выделенной строки
				uSelectedItem = (int)SendMessage(hListBoxA, LB_GETCURSEL, 0, 0L);
				// Если в списке есть выделенная строка,
				// выводим ее на экран 
				if (uSelectedItem != LB_ERR)
				{
					if (lastrow > -1)
					{
						DestroyWindow(row[lastrow]);
						row[lastrow] = CreateWindow(L"STATIC", words[lastrow], WS_VISIBLE | WS_CHILD | ES_CENTER, 23, 130 + lastrow * 22, 20, 20, hwnd, NULL, NULL, NULL);
					}
					DestroyWindow(row[uSelectedItem]);
					row[uSelectedItem] = CreateWindow(L"STATIC", words[uSelectedItem], WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER, 23, 130 + uSelectedItem * 22, 20, 20, hwnd, NULL, NULL, NULL);
					lastrow = uSelectedItem;
					if (!FBstatus && lastcol > -1)
					{
						EnableWindow(finalButtom, true);
						FBstatus = true;
					}
				}
			}
			break;
		}
		case 3:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				int uSelectedItem;
				uSelectedItem = (int)SendMessage(hListBoxB, LB_GETCURSEL, 0, 0L);
				if (uSelectedItem != LB_ERR)
				{
					if (lastcol > -1)
					{
						DestroyWindow(col[lastcol]);
						col[lastcol] = CreateWindow(L"STATIC", words[lastcol], WS_VISIBLE | WS_CHILD | ES_CENTER, 48 + lastcol * 22, 105, 20, 20, hwnd, NULL, NULL, NULL);
					}
					DestroyWindow(col[uSelectedItem]);
					col[uSelectedItem] = CreateWindow(L"STATIC", words[uSelectedItem], WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER, 48 + uSelectedItem * 22, 105, 20, 20, hwnd, NULL, NULL, NULL);
					lastcol = uSelectedItem;
					if (!FBstatus && lastrow > -1)
					{
						EnableWindow(finalButtom, true);
						FBstatus = true;
					}
				}

			}
			break;
		}
		case 4: {


			for (int i = 0; i < TownCount; i++) {
				for (int j = 0; j < TownCount; j++) {
					Text = new char[GetWindowTextLength(but[i][j]) + 1];
					GetWindowTextA(but[i][j], Text, GetWindowTextLength(but[i][j]) + 1);
					int size = SendMessage(but[i][j], EM_LINELENGTH, -1, 0);
					if (size > 0) {
						if (i > j) SetWindowTextA(but[j][i], Text);
					}
					else {
						SetWindowTextA(but[i][j], "0");
					}
					delete[] Text;
				}
			}

			// проверка графа на связность

			bool cvyaznoi = true;
			int counter = 0;

			for (int i = 0; i < TownCount; i++) {
				for (int j = 0; j < TownCount; j++) {
					Text = new char[GetWindowTextLength(but[i][j]) + 1];
					GetWindowTextA(but[i][j], Text, GetWindowTextLength(but[i][j]) + 1);
					tempArr[i][j] = stoi(Text);
					delete[] Text;
				}
			}

			for (int i = 0; i < TownCount; i++) {

				for (int j = 0; j < TownCount; j++) {

					if (tempArr[i][j] == 0) {

						++counter;

					}

				}

				if (counter == TownCount) {
					cvyaznoi = false;
				}
				counter = 0;
			}

			if (cvyaznoi) {
				SendMessage(hWndinp, WM_COMMAND, 2, 0L);
				ShowWindow(hWndinp, SW_SHOW);
				EnableWindow(hWindow, FALSE);
			}
			else {

				MessageBox(hwnd, L"Программа работает только со свзяными графами. Добавьте хотя бы по одной связи для всех вершин!", L"Неправильный ввод", MB_ICONWARNING);

			}

			break;
		}
		case 10: {
			HWND po = HWND(lParam);
			Text = new char[GetWindowTextLength(po) + 1];
			GetWindowTextA(po, Text, GetWindowTextLength(po) + 1);

			if (Text[0] == '0') SetWindowText(po, L"1");
			else SetWindowText(po, L"1");
			delete[] Text;
			break;
		}

		}
		break;

	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
LRESULT CALLBACK WndProcChild(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char* Text = NULL;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		texter = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 10, 378, 20, hwnd, NULL, NULL, NULL);
		texter2 = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 35, 378, 20, hwnd, NULL, NULL, NULL);
		CreateWindow(L"BUTTON", L"Закрыть", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 55, 378, 20, hwnd, (HMENU)1, NULL, NULL);
		break;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			// Обработка команд (нажатие кнопок, мыши, полей ввода и т.д.)
		case 1:
		{
			ShowWindow(hWndinp, SW_HIDE);
			EnableWindow(hWindow, TRUE);
			SetFocus(hWindow);
			break;
		}
		case 2:
		{
			for (int i = 0; i < TownCount; i++) {
				for (int j = 0; j < TownCount; j++) {
					Text = new char[GetWindowTextLength(but[i][j]) + 1];
					GetWindowTextA(but[i][j], Text, GetWindowTextLength(but[i][j]) + 1);
					dosMat[i][j] = stoi(Text);
					delete[] Text;
				}
			}

			vector <int> put;
			DlinaPuti = 0;
			put = MinPut(dosMat, TownCount); //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			string buf, buf2;

			for (int i = 0; i < 16; i++)
			{
				DestroyWindow(colChild[i]);
				DestroyWindow(rowChild[i]);
				if (i < TownCount)
				{
					colChild[i] = CreateWindow(L"STATIC", words[i], WS_VISIBLE | WS_CHILD | ES_CENTER, 42 + i * 22, 90, 20, 20, hwnd, NULL, NULL, NULL);
					//if (row[i]) DestroyWindow(row[i]);
					rowChild[i] = CreateWindow(L"STATIC", words[i], WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 112 + i * 22, 20, 20, hwnd, NULL, NULL, NULL);
				}

				for (int j = 0; j < 16; j++)//очиска
				{
					DestroyWindow(finMat[i][j]);
					if (i < TownCount && j < TownCount)
					{
						if (i == lastcol && j == lastrow)
							finMat[i][j] = CreateWindow(L"STATIC", L"0", WS_VISIBLE | WS_CHILD | ES_CENTER | WS_BORDER, 42 + i * 22, 112 + j * 22, 20, 20, hwnd, NULL, NULL, NULL);
						else
							finMat[i][j] = CreateWindow(L"STATIC", L"0", WS_VISIBLE | WS_CHILD | ES_CENTER, 42 + i * 22, 112 + j * 22, 20, 20, hwnd, NULL, NULL, NULL);
						buf = to_string(dosMat[i][j]);
						SetWindowTextA(finMat[i][j], buf.c_str());
					}
				}
			}


			Text = new char[GetWindowTextLength(col[lastrow]) + 1];
			GetWindowTextA(col[lastrow], Text, GetWindowTextLength(col[lastrow]) + 1);
			string first = Text;
			buf = "Минимальный путь из вершины <" + first + ">" + " в <";
			delete[] Text;

			Text = new char[GetWindowTextLength(col[lastcol]) + 1];
			GetWindowTextA(col[lastcol], Text, GetWindowTextLength(col[lastcol]) + 1);

			first = Text;
			delete[] Text;
			buf += first + ">:";
			SetWindowTextA(texter, buf.c_str());// вывод выбранных вершин и сообщения
			buf = "";
			SetWindowTextA(texter2, buf.c_str());// вывод выбранных вершин и сообщения

			if (lastcol == lastrow) {

				Text = new char[GetWindowTextLength(but[lastcol][lastrow]) + 1];
				GetWindowTextA(but[lastcol][lastrow], Text, GetWindowTextLength(but[lastcol][lastrow]) + 1);
				buf = first + " -> " + first + " = " + Text;


				SetWindowTextA(texter2, buf.c_str());// вывод выбранных вершин и сообщения
			}
			else {

				buf = "";

				for (int i = put.size() - 1; i >= 0; i--) {

					Text = new char[GetWindowTextLength(col[put[i] - 1]) + 1];
					GetWindowTextA(col[put[i] - 1], Text, GetWindowTextLength(col[put[i] - 1]) + 1);
					string vershina = Text;

					if (i == 0) {
						buf += vershina;
					}
					else {
						buf += vershina + "->";
					}

					delete[] Text;

				}

				buf += " = " + to_string(DlinaPuti);

				SetWindowTextA(texter2, buf.c_str());// вывод выбранных вершин и сообщения
			}

			break;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////


		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int _stdcall _FindPath(int, int, int**, int, int*, int*, int)
{
	return 0;
}
