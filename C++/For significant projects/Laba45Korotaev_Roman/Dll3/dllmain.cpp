// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include "DLL3Dekster.h"
using namespace std;
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

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
