//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include "Lusbapi.h"

// ��������� ����� �� ���������
void AbortProgram(const char *ErrorString, ...);

// ��������� �� ��������� ������
ILE140 *pModule;
// ���������� ����������
HANDLE ModuleHandle;

typedef struct
    {
    BYTE VerMinor, VerMajor;
    CHAR Date[14];
    }
    t_VersionInfo;

//------------------------------------------------------------------------
// �������� ���������
//------------------------------------------------------------------------
int main()
    {
    int i;
    BYTE dac_present;
    DWORD devstate;
    SHORT cpld_ver;
    t_VersionInfo vi;
    MODULE_DESCRIPTION_E140 DevDesc;
    char ModuleName[32];

    /* �������� ������ Lusbapi.dll */
    DWORD DLL_Ver = GetDllVersion();
    if (DLL_Ver != CURRENT_VERSION_LUSBAPI)
        AbortProgram("Lusbapi.dll version mismatch (found %lu.%lu, need %u.%u)\n",
            DLL_Ver >> 16, DLL_Ver & 0xFFFF,
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF);

    /* ����������� � ������� */
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

    /* �������� ����� ����������, ������ ��� � ���������� ���������� */
    ModuleHandle = pModule->GetModuleHandle();
    if (ModuleHandle == INVALID_HANDLE_VALUE)
        AbortProgram("GetModuleHandle() failed.\n");
    if (!pModule->GetModuleName(ModuleName))
        AbortProgram("GetModuleName() failed.\n");
    if (!pModule->GET_MODULE_DESCRIPTION(&DevDesc))
        AbortProgram("GET_MODULE_DESCRIPTION() failed.\n");

    printf("Connected to %s (S/N %s, Fosc = %.0f MHz).\n\n",
        ModuleName, DevDesc.Module.SerialNumber, (DevDesc.Mcu.ClockRate / 1000));

    /* ������ ������ �������� */
    if (pModule->GetArray((BYTE*)&vi, sizeof(vi), 0x1080))
        {
        vi.Date[sizeof(vi.Date) - 1] = '\0';
        printf("Firmware version: %d.%02d, %s\n", vi.VerMajor, vi.VerMinor, vi.Date);
        }
    else
        {
        printf("Cannot get firmware version\n");
        }

    /* �������� ������ ���� */
    if (pModule->GetArray((BYTE*)&cpld_ver, sizeof(cpld_ver), 0x1070))
        {
        printf("Hardware revision: %d\n", cpld_ver);
        }
    else
        {
        printf("Cannot get hardware revision\n");
        }
    
    /* �������� ������� ��� */
    if (pModule->GetArray((BYTE*)&dac_present, sizeof(dac_present), 0x0DAC))
        {
        printf("DAC: %s\n", (dac_present) ? "INSTALLED" : "NOT INSTALLED");
        }
    else
        {
        printf("Cannot query DAC\n");
        }

    /* ������ ��������� ���������� */
    if (pModule->GetArray((BYTE*)&devstate, sizeof(devstate), 0x10C0))
        {
        printf("Device state: %08lX\n", devstate);
        }
    else
        {
        printf("Cannot query device state\n");
        }

    pModule->ReleaseLInstance();
    puts("\nDone.");
    }

//------------------------------------------------------------------------
// ��������� ���������� ���������
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
