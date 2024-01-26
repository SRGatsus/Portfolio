// dllmain.cpp : Определяет точку входа для приложения DLL.
#include"pch.h"
#include "DLL1Dekster.h"
#include <stdexcept>
#include "WinApi.h"
#include <tchar.h>
using namespace std;



EXPORT HMENU InitMain()//создаём главное окно
{

	HMENU MainMenu = CreateMenu();
	HMENU hPopupMenu = CreatePopupMenu();
	HMENU hPopupMenu2 = CreatePopupMenu();
	HMENU hPopupMenu3 = CreatePopupMenu();

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu, L"&Файл");
	{
		AppendMenu(hPopupMenu, MF_STRING, 1, L"Выход");
	}

	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu2, L"Матрицы");
	{
		AppendMenu(hPopupMenu2, MF_STRING, 2, L"Матрица смежности");
	}
	AppendMenu(MainMenu, MF_STRING | MF_POPUP, (UINT)hPopupMenu3, L"&Ответы");
	{
		AppendMenu(hPopupMenu3, MF_STRING, 3, L"из Пункты А в B?");
	}
	return  MainMenu;
}
