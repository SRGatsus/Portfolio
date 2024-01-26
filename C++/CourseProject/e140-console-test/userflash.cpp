//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include "Lusbapi.h"
#include <math.h>

#define USER_START  0x0800
#define PARTIAL_XFER

void AbortProgram(const char *ErrorString, ...);

ILE140 *pModule;
HANDLE ModuleHandle;

//------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    DWORD DLL_Ver;
    int i;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 DevDesc;
    char command;
    long L;
    WORD offset = 0;
    BYTE databyte;
    char *pEOL;
    USER_FLASH_E140 Flash;

    printf("** E-140 user flash test **\n\n");

    /* Параметры: r|w offset [data] */
    if (argc < 2)
        AbortProgram("Syntax: userflash e (erase) | z (zero) | d (dump) | r hex_offset | w hex_offset hex_byte\n");
    command = toupper(argv[1][0]);
    if ((command == 'R') || (command == 'W'))
        {
        if (argc < 3) AbortProgram("Missing offset\n");
        L = strtol(argv[2], &pEOL, 16);
        if ((*pEOL != '\0') || (L < 0) || (L > 511))
            AbortProgram("Invalid flash offset %ld (must be 0..511)\n", L);
        offset = L;
        }
    if (command == 'W')
        {
        if (argc < 4) AbortProgram("Missing data byte\n");
        L = strtol(argv[3], &pEOL, 16);
        if ((*pEOL != '\0') || (L < 0) || (L > 0xFF))
            AbortProgram("Invalid data %lX (must be 0..FF)\n", L);
        databyte = L;
        }

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

    switch (command)
        {
        case 'R':
#ifdef PARTIAL_XFER
            if (!pModule->GetArray(&databyte, 1, USER_START + offset))
                AbortProgram("Cannot read flash\n");
#else
            if (!pModule->READ_FLASH_ARRAY(&Flash))
                AbortProgram("Cannot read flash\n");
            databyte = Flash.Buffer[offset];
#endif
            printf("Read flash: offset = %02X, value = %02X\n", offset, databyte);
            break;
        case 'D':
            if (!pModule->READ_FLASH_ARRAY(&Flash))
                AbortProgram("Cannot read flash\n");
            printf("Flash dump\n"
                   "----------");
            for (unsigned int i = 0; i < sizeof(Flash.Buffer); i++)
                {
                if (!(i & 0x0F))
                    printf("\n[%04X]", i);
                printf(" %02X", Flash.Buffer[i]);
                }
            puts("");
            break;
        case 'E':
        case 'Z':
            memset(Flash.Buffer, (command == 'E') ? 0xFF : 0x00, sizeof(Flash.Buffer));
            if (!pModule->ENABLE_FLASH_WRITE(1))
                AbortProgram("Cannot enable flash write\n");
            if (!pModule->WRITE_FLASH_ARRAY(&Flash))
                {
                puts("Cannot write flash");
                }
            else
                {
                printf("User flash filled with %02X\n", Flash.Buffer[0]);
                }
            if (!pModule->ENABLE_FLASH_WRITE(0))
                AbortProgram("Cannot disable flash write\n");
            break;
        case 'W':
            printf("Write flash: offset = %02X, value = %02X\n", offset, databyte);
            if (!pModule->ENABLE_FLASH_WRITE(1))
                AbortProgram("Cannot enable flash write\n");
#ifdef PARTIAL_XFER
            if (!pModule->PutArray(&databyte, 1, USER_START + offset))
                puts("Cannot write flash");
#else
            if (!pModule->READ_FLASH_ARRAY(&Flash))
                {
                puts("Cannot read flash");
                }
            else
                {
                Flash.Buffer[offset] = databyte;
                if (!pModule->WRITE_FLASH_ARRAY(&Flash))
                    puts("Cannot write flash");
                }
#endif
            if (!pModule->ENABLE_FLASH_WRITE(0))
                AbortProgram("Cannot disable flash write\n");
            break;
        default:
            AbortProgram("Invalid command \"%c\"\n", command);
        }

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
