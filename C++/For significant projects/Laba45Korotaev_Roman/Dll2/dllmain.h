#pragma once
#include <winnt.h>

extern "C" __declspec(dllexport) bool isNumber(WCHAR* s);
extern "C" __declspec(dllexport) bool AlgrtmDekstrBil(int AFG, int B, int FGH, int  MassMatrix[10][10]);