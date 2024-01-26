//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include <limits.h>
#include "Lusbapi.h"

// аварийный выход из программы
void AbortProgram(const char *ErrorString, ...);

// указатель на интерфейс модуля
ILE140 *pModule;
// дескриптор устройства
HANDLE ModuleHandle;

//------------------------------------------------------------------------
// основная программа
//------------------------------------------------------------------------
int main()
    {
    int i;
    MODULE_DESCRIPTION_E140 DevDesc;
    char ModuleName[32];

    /* Проверка версии Lusbapi.dll */
    DWORD DLL_Ver = GetDllVersion();
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

    printf("Connected to %s (S/N %s, Fosc = %.0f MHz).\n\n",
        ModuleName, DevDesc.Module.SerialNumber, (DevDesc.Mcu.ClockRate / 1000));

    puts("Testing TTL I/O speed, press any key to exit...");

    unsigned long count = 0;
    DWORD start_time = GetTickCount();
    DWORD last_print = start_time;

    while (!kbhit() && (count < ULONG_MAX))
        {
        WORD ttl;
        if (!pModule->TTL_IN(&ttl)) { puts("\nTTL IN error"); break; }
        if (!pModule->TTL_OUT(0))   { puts("\nTTL OUT error"); break; }
        count++;
        DWORD ms = GetTickCount() - start_time;
        if ((DWORD)(ms - last_print) >= 1000)
            {
            last_print = ms;
            printf("\rCycles = %10lu, rate ~= %10lu Hz",
                count,
                (unsigned long)(0.5 + count * 1000.0 / ms));
            }
        }

    while (kbhit()) getch();
    puts("\nStopped");

    pModule->ReleaseLInstance();
    }

//------------------------------------------------------------------------
// аварийное завершение программы
//------------------------------------------------------------------------
void AbortProgram(const char *ErrorString, ...)
    {
    va_list va;

    if(pModule)
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
