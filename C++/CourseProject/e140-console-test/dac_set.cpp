//******************************************************************************
#include <stdio.h>
#include <conio.h>
#include "Lusbapi.h"

/*------------------------------------------------------------------------------------------------*/
void Set(ILE140* pModule, SHORT* data)
    {
    BOOL found = FALSE;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 ModuleDescription;

    for (int i = 0; !found && (i < 127); i++)
        {
        found = pModule->OpenLDevice(i);
        }
    if (!found)
        {
        puts("ERROR: E-140 not found.");
        return;
        }

    if (!pModule->GetModuleName(ModuleName))
        {
        puts("ERROR: Cannot read module name.");
        return;
        }
    if (strcmp(ModuleName, "E140"))
        {
        puts("ERROR: Module is not E-140.");
        return;
        }

    if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription))
        {
        puts("ERROR: Cannot read module descriptor.");
        return;
        }

    printf("Connected to %s (serial number %s).\n",
        ModuleName, ModuleDescription.Module.SerialNumber);

    if (!pModule->STOP_DAC())
        {
        puts("ERROR: Cannot stop DAC.");
        return;
        }

    if (!pModule->DAC_SAMPLES(&data[0], &data[1]))
        {
        puts("ERROR: Cannot set DAC.");
        return;
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
    {
    ILE140* pModule;
    int volt_mode;
    SHORT sample[2];

    puts("E14-140-MD DAC Test\n"
         "-------------------");
    DWORD dll_ver = GetDllVersion();
    if (dll_ver != CURRENT_VERSION_LUSBAPI)
        {
        printf("ERROR: Wrong version of lusbapi.dll (need %1u.%1u, got %1lu.%1lu).\n",
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF,
            dll_ver >> 16, dll_ver & 0xFFF);
        exit(0);
        }
   
    if (argc <= 3)
        {
        puts("Syntax: \n"
             "    dac_set -n <channel0_val> <channel1_val>\n"
             "        (channelX_val = -32768 .. 32767)\n"
             "    dac_set -v <channel0_voltage> <channel1_voltage>\n"
             "        (channelX_voltage = -5.0 .. 5.0)");
        exit(0);
        }

    if (!stricmp("-n", argv[1]))
        {
        volt_mode = 0;
        }
    else if (!stricmp("-v", argv[1]))
        {
        volt_mode = 1;
        }
    else
        {
        printf("Invalid option %s\n", argv[1]);
        exit(0);
        }

    for (int i = 0; i < 2; i++)
        {
        char *pEOL;
        long val = (volt_mode)
            ? (long)(strtod(argv[2 + i], &pEOL) * (32767.0 / 5.0))
            : strtol(argv[2 + i], &pEOL, 10);
        if ((*pEOL != '\0') || (val < -32768) || (val > 32767))
            {
            printf("Invalid sample value for channel %d\n", i);
            exit(0);
            }
        sample[i] = val;
        }

    pModule = static_cast<ILE140*>(CreateLInstance((char*)"e140"));
    if (!pModule)
        {
        puts("ERROR: Cannot create instance of ILE140.");
        exit(0);
        }

    Set(pModule, sample);

    pModule->ReleaseLInstance();
    }
/*------------------------------------------------------------------------------------------------*/
