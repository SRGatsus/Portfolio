#include <stdio.h>
#include "Lusbapi.h"

#define DESC_ADDR   0x2780
#define DESC_SIZE   0x80

void AbortProgram(const char *ErrorString, ...);

ILE140 *pModule;

//------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    DWORD DLL_Ver;
    int i;
    char ModuleName[32];
    BYTE desc[DESC_SIZE];

    printf("** E-140 descriptor dump **\n\n");

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
    if (!pModule->GetModuleName(ModuleName))
        AbortProgram("GetModuleName() failed.\n");

    printf("Connected to %s.\n", ModuleName);

    if (!pModule->GetArray(desc, sizeof(desc), DESC_ADDR))
        AbortProgram("Cannot read flash\n");
    printf("Descriptor dump\n"
           "---------------");
    for (unsigned int i = 0; i < sizeof(desc); i++)
        {
        if (!(i & 0x0F))
            printf("\n[%04X]", i);
        printf(" %02X", desc[i]);
        }
    puts("");

    AbortProgram("Done.\n");
    }

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
    exit(0);
    }
