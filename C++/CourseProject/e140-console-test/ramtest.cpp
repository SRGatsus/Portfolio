//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include "Lusbapi.h"
#include <math.h>

#define RAM_START  0x8000
#define RAM_SIZE   0x8000

void AbortProgram(const char *ErrorString, ...);

ILE140 *pModule;
HANDLE ModuleHandle;

//------------------------------------------------------------------------
int main()
    {
    DWORD DLL_Ver;
    int i;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 DevDesc;
    BYTE Buf1[RAM_SIZE];
    BYTE Buf2[RAM_SIZE];

    printf("** E-140 RAM test **\n\n");

    /* Проверка версии Lusbapi.dll */
    DLL_Ver = GetDllVersion();
    if (DLL_Ver != CURRENT_VERSION_LUSBAPI)
        AbortProgram("Lusbapi.dll version mismatch (found %lu.%lu, need %u.%u)\n",
            DLL_Ver >> 16, DLL_Ver & 0xFFFF,
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF);

    /* Соединяемся с модулем */
    pModule = static_cast<ILE140 *>(CreateLInstance((char*)"e140"));
    if (!pModule)
        AbortProgram("Connection failed: Cannot create module interface.\n");
    for (i = 0; i < 127; i++)
        {
        if (pModule->OpenLDevice(i))
            break;
        }
    if (i == 127)
        AbortProgram("Connection failed: E-140 not found.\n");

    /* Получаем хэндл устройства, читаем имя и дескриптор устройства */
    ModuleHandle = pModule->GetModuleHandle();
    if (ModuleHandle == INVALID_HANDLE_VALUE)
        AbortProgram("GetModuleHandle() failed.\n");
    if (!pModule->GetModuleName(ModuleName))
        AbortProgram("GetModuleName() failed.\n");
    if (!pModule->GET_MODULE_DESCRIPTION(&DevDesc))
        AbortProgram("GET_MODULE_DESCRIPTION() failed.\n");

    printf("Connected to %s (S/N %s, Fosc = %.0f MHz).\n",
        ModuleName, DevDesc.Module.SerialNumber, (DevDesc.Mcu.ClockRate / 1000));

    for (i = 0; i < RAM_SIZE; i++)
        {
        Buf1[i] = rand() & 0xFF;
        Buf2[i] = 0;
        }
    if (!pModule->PutArray(Buf1, RAM_SIZE, RAM_START))
        AbortProgram("Error writing RAM\n");
    if (!pModule->GetArray(Buf2, RAM_SIZE, RAM_START))
        AbortProgram("Error reading RAM\n");
    printf("Memory test %s\n", memcmp(Buf1, Buf2, RAM_SIZE) ? "FAILED" : "OK");

    AbortProgram("Done.\n");
    }

//------------------------------------------------------------------------
// аварийное завершение программы
//------------------------------------------------------------------------
void AbortProgram(const char *ErrorString, ...)
    {
    va_list va;

	if (pModule)
        pModule->ReleaseLInstance();

	if (ErrorString)
        {
        va_start(va, ErrorString);
        vprintf(ErrorString, va);
        va_end(va);
        }
    while (kbhit()) getch();
    exit(0);
    }
