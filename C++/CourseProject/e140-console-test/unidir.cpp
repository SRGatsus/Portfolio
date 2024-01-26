#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <winioctl.h>
#include "Lusbapi.h"

/*================================================================================================*/
#define ADC_FREQ        100000
#define ADC_BUF_SAMPLES 50000

#define CH_GAIN_1       0x00 /* 10 V */
#define CH_GAIN_4       0x40 /* 2.5 V */
#define CH_GAIN_16      0x80 /* 0.625 V */
#define CH_GAIN_64      0xC0 /* 0.15625 V */
#define CH_MODE_DIFF    0x00
#define CH_MODE_CGROUND 0x20
#define CH_MEASURE_ZERO 0x10 /* only when CH_MODE_CGROUND == 0 */

#define CTL_TOGGLE      1
#define CTL_TERMINATE   2
#define CTL_SLEEP       3
/*================================================================================================*/

/*================================================================================================*/
ILE140 *pModule;
HANDLE ModuleHandle;
CRITICAL_SECTION cs;
ADC_PARS_E140 ap;
short int ADC_Buf[2][ADC_BUF_SAMPLES][2]; /* Два буфера на 2 канала */
BYTE bConst_1 = 1;

/* Переменные для связи с потоками */
const char *pThreadMessage = NULL;
HANDLE hControlEvent;
HANDLE hThreadMsgEvent;
int Control = 0;
/*================================================================================================*/

/*------------------------------------------------------------------------------------------------*/
static void f_CloseHandles(int Count, HANDLE *pList)
    {
    while (Count--)
        {
        if (*pList != INVALID_HANDLE_VALUE)
            {
            CloseHandle(*pList);
            *pList = INVALID_HANDLE_VALUE;
            }
        pList++;
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static void f_ThreadMsg(const char *msg)
    {
    pThreadMessage = msg;
    SetEvent(hThreadMsgEvent);
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static void f_PrintThreadMsg(const char *prefix)
    {
    if (pThreadMessage)
        {
        printf("%s: %s\n", prefix, pThreadMessage);
        pThreadMessage = NULL;
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
DWORD WINAPI ControlThread(LPVOID)
    {
    for (;;)
        {
        switch (getch())
            {
            case 'a':
            case 'A':
                Control = CTL_TOGGLE;
                SetEvent(hControlEvent);
                break;
            case 's':
            case 'S':
                Control = CTL_SLEEP;
                SetEvent(hControlEvent);
                break;
            case 0x1B:
                Control = CTL_TERMINATE;
                SetEvent(hControlEvent);
                return 0; /* Подав команду выхода, завершаемся сами */
            }
        }
    return 0;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
DWORD WINAPI ADC_Thread(LPVOID)
    {
    int running = 0;
    int idx = 0;
    HANDLE hEvent[2] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
    OVERLAPPED ov[2];
    unsigned int StartCount = 1;
    static char FileName[64];
    HANDLE hFile = INVALID_HANDLE_VALUE;
    DWORD dummy, TransferSize;
    int ok;

    for (;;)
        {
        if (!running)
            {
            WaitForSingleObject(hControlEvent, INFINITE);
            switch (Control)
                {
                case CTL_TERMINATE:      /* Завершение программы */
                    return 0; /* Ничего не открыто, так что просто выйти */
                case CTL_SLEEP:
                    EnterCriticalSection(&cs);
                    if (pModule->PutArray(&bConst_1, 1, 0x430))
                        {
                        f_ThreadMsg("Sleep mode engaged");
                        }
                    else
                        {
                        f_ThreadMsg("Cannot enter sleep mode");
                        }
                    LeaveCriticalSection(&cs);
                    break;
                case CTL_TOGGLE:        /* Запуск АЦП */
                    hEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
                    hEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
                    ok = 0;
                    EnterCriticalSection(&cs);
                    do
                        {
                        if (!pModule->SET_ADC_PARS(&ap))
                            {
                            f_ThreadMsg("Cannot set ADC parameters");
                            break;
                            }
                        /* Формируем имя файла */
                        sprintf(FileName, "adc%03u.dat", StartCount);
                        /* Открываем файл */
                        hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN |
                            FILE_FLAG_WRITE_THROUGH, NULL);
                        if (hFile == INVALID_HANDLE_VALUE)
                            {
                            f_ThreadMsg("Cannot create data file");
                            break;
                            }
                        /* Ставим в очередь первый запрос чтения данных */
                        ZeroMemory(&ov[0], sizeof(OVERLAPPED));
                        ov[0].hEvent = hEvent[0];
                        if (!ReadFile(ModuleHandle, ADC_Buf[0], sizeof(ADC_Buf[0]), NULL, &ov[0])
                            && (GetLastError() != ERROR_IO_PENDING))
                            {
                            f_ThreadMsg("ReadFile() failed during start");
                            CloseHandle(hFile);
                            break;
                            }
                        /* Запускаем АЦП */
                        if (!pModule->START_ADC())
                            {
                            f_ThreadMsg("Cannot start ADC");
                            CloseHandle(hFile);
                            break;
                            }
                        idx = 1;
                        running = 1;
                        f_ThreadMsg("ADC started");
                        StartCount++;
                        ok = 1;
                        }
                    while (0);
                    LeaveCriticalSection(&cs);
                    if (!ok)
                        f_CloseHandles(2, hEvent);
                    break;
                }
            }
        else /* running */
            {
            /* Ставим в очередь следующий буфер */
            ZeroMemory(&ov[idx], sizeof(OVERLAPPED));
            ov[idx].hEvent = hEvent[idx];
            ok = 1;
            if (!ReadFile(ModuleHandle, ADC_Buf[idx], sizeof(ADC_Buf[idx]), NULL, &ov[idx])
                && (GetLastError() != ERROR_IO_PENDING))
                {
                f_ThreadMsg("ReadFile() failed, stopping ADC");
                ok = 0;
                }
            else
                {
                HANDLE WaitList[2];
                idx ^= 1;
                /* Ждем окончания предыдущего чтения или прихода команды */
                WaitList[0] = hControlEvent;
                WaitList[1] = hEvent[idx];
                while (WaitForMultipleObjects(2, WaitList, 0, INFINITE) == WAIT_OBJECT_0)
                    {
                    /* Пришла команда */
                    if ((Control == CTL_TERMINATE) || (Control == CTL_TOGGLE)
                        || (Control == CTL_SLEEP))
                        {
                        CancelIo(ModuleHandle);
                        f_CloseHandles(2, hEvent);
                        CloseHandle(hFile);
                        EnterCriticalSection(&cs);
                        if (pModule->STOP_ADC())
                            {
                            f_ThreadMsg("ADC stopped");
                            }
                        else
                            {
                            f_ThreadMsg("Cannot stop ADC");
                            }
                        LeaveCriticalSection(&cs);
                        running = 0;
                        if (Control == CTL_SLEEP)
                            {
                            EnterCriticalSection(&cs);
                            if (pModule->PutArray(&bConst_1, 1, 0x430))
                                {
                                f_ThreadMsg("Sleep mode engaged");
                                }
                            else
                                {
                                f_ThreadMsg("Cannot enter sleep mode");
                                }
                            LeaveCriticalSection(&cs);
                            }
                        if (Control == CTL_TERMINATE)
                            return 0;
                        }
                    }
                ResetEvent(hEvent[idx]);
                if (running)
                    {
                    f_ThreadMsg("data...");
                    if (!GetOverlappedResult(ModuleHandle, &ov[idx], &TransferSize, FALSE))
                        {
                        f_ThreadMsg("Data stream interrupted, stopping ADC");
                        ok = 0;
                        }
                    if (!WriteFile(hFile, ADC_Buf[idx], TransferSize, &dummy, NULL))
                        {
                        f_ThreadMsg("File write error, stopping ADC");
                        ok = 0;
                        }
                    }
                } /* if ReadFile */
            if (!ok)
                {
                CancelIo(ModuleHandle);
                f_CloseHandles(2, hEvent);
                CloseHandle(hFile);
                EnterCriticalSection(&cs);
                pModule->STOP_ADC();
                LeaveCriticalSection(&cs);
                running = 0;
                }
            } /* if running */
        } /* for (;;) */
    return 0;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main()
    {
    DWORD DLL_Ver;
    int i;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 DevDesc;
    HANDLE hThread[2];
    HANDLE WaitList[2];

    printf("** E-140 ADC test **\n\n");

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
    printf("Ready to test with ADC frequency %u Hz.\n\n", ADC_FREQ);

    /* Заполняем конфигурацию каналов АЦП */
    ap.ClkSource = 0;
    ap.EnableClkOutput = 0;
    ap.InputMode = NO_SYNC_E140;
    ap.SynchroAdType = 0; /* 0 = level, 1 = edge */
    ap.SynchroAdMode = 0; /* 0 = up, 1 = down */
    ap.SynchroAdChannel = CH_GAIN_1 | CH_MODE_DIFF | 0x0F;
    ap.SynchroAdPorog = 1000;
    ap.ChannelsQuantity = 2;
    ap.ControlTable[0] = CH_GAIN_1 | CH_MODE_DIFF | 0x00;
    ap.ControlTable[1] = CH_GAIN_1 | CH_MODE_DIFF | 0x01;
    ap.AdcRate = (double)ADC_FREQ / 1000.0;
    ap.InterKadrDelay = 0.0;

    /* Стираем все старые файлы данных */
    {
    HANDLE hFind;
    WIN32_FIND_DATA FindData;
    hFind = FindFirstFile("adc*.dat", &FindData);
    if (hFind != INVALID_HANDLE_VALUE)
        {
        do
            {
            DeleteFile(FindData.cFileName);
            }
        while (FindNextFile(hFind, &FindData));
        FindClose(hFind);
        }
    }

    /* Остановим АЦП */
    if (!pModule->STOP_ADC())
        AbortProgram("Cannot stop ADC.\n");

    /* Создаем события для управления потоками */
    hControlEvent = CreateEvent(NULL, FALSE /* auto reset */, FALSE, NULL);
    /* Создаем события для передачи сообщений */
    hThreadMsgEvent = CreateEvent(NULL, TRUE /* manual reset */, FALSE, NULL);

    puts("[A]   - start/stop ADC");
    puts("[S]   - sleep (uses ADC thread)");
    puts("[Esc] - exit");
    while (kbhit()) getch();

    /* Создаем критическую секцию для обращения к модулю */
    InitializeCriticalSection(&cs);

    /* Создаем потоки записи в ЦАП, чтения из АЦП и управляющий */
    hThread[0] = CreateThread(0, 0x2000, ADC_Thread, NULL, 0, NULL);
    hThread[1] = CreateThread(0, 0x2000, ControlThread, NULL, 0, NULL);

    /* Цикл управления */
    WaitList[0] = hThread[1]; /* По завершению управляющего потока */
    WaitList[1] = hThreadMsgEvent;

    for (;;)
        {
        switch (WaitForMultipleObjects(2, WaitList, 0, INFINITE))
            {
            case WAIT_OBJECT_0: /* Управляющий поток завершился */
                /* Дождаться завершения потока АЦП */
                WaitForSingleObject(hThread[0], INFINITE);
                f_PrintThreadMsg("ADC");
                CloseHandle(hThreadMsgEvent);
                CloseHandle(hControlEvent);
                AbortProgram("Done.\n");
                break;
            case WAIT_OBJECT_0 + 1:
                f_PrintThreadMsg("ADC");
                ResetEvent(hThreadMsgEvent);
                break;
            }
        }
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
