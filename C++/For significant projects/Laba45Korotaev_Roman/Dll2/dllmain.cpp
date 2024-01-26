// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <stdexcept>
using namespace std;
extern "C" bool mul_v(int MassMatrix2[10][10], int  FGH, int AFG, int B, int node[10], int pos[10]);
__declspec(dllexport) HICON hIcon;
BOOL APIENTRY dllMain(HMODULE hModule,
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
		DestroyIcon(hIcon);
		break;

		break;
	}
	return TRUE;
}

extern "C" __declspec(dllexport)
bool isNumber(WCHAR * s) {

	for (int a = 0; a < wcslen(s) + 1; a++) {
		// Если в строке есть не цифровые символы, то это не число
		if (s[a] != 45 || s[a] != 43) { // Проверка на знак минус
			if ((s[a] < 48) || (s[a] > 57))  return false;
		}

	}
	// Если в строке только цифровые символы значит это число
	return true;
}
extern "C" __declspec(dllexport)
bool AlgrtmDekstrBil(int AFG, int B, int FGH, int MassMatrix[10][10]) {


	int* node = new int[10];
	int* pos = new int[10];
	if (mul_v(MassMatrix, FGH, AFG, B, node, pos)) {
		return false;
	}
	else {
		return true;
	}
}
