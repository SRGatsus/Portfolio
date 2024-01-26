/*================================================================================================*
 * Детектор перехода через пороги (положительный и отрицательный)
 * Вход = канал 1, режим с общей землей, 20Vpp
 * Выход = TTL1
 *
 * Программа пытается читать АЦП маленькими буферами и отслеживать сигнал
 * (калибровочные коэффициенты не применяются, опущено для простоты)
 * U > U_HI  => TTL1 = 1
 * U < U_LO  => TTL1 = 0
 * В зависимости от производительности компьютера может быть достижима большая или меньшая задержка.
 *
 * Программа пытается установить себе высокий приоритет (если есть права).
 *
 *================================================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <winioctl.h>
#include "Lusbapi.h"

/*================================================================================================*/
#define ARRLEN(array)               (sizeof(array) / sizeof((array)[0]))

#define SAMPLE_FREQ                 100000
#define ADC_BUF_NSAMPLES            (64 * 2) /* Чем больше, тем больше задержка и выше стабильность */
#define ADC_QUEUE_LEN               32
#define TIMEOUT_MS                  2000

#define ADC_CODE_V(gain, volt)      ((SHORT)((volt) * (gain) / 10.0 * 8000))
#define U_HI                        0.1
#define U_LO                        -0.1

#define GAIN                        1

#define CH_GAIN(n)                  ((((n) > 1) + ((n) > 4) + ((n) > 16)) << 6) /* n = 1, 4, 16, 64 */
#define CH_GAIN_1                   0x00 /* 10 V */
#define CH_GAIN_4                   0x40 /* 2.5 V */
#define CH_GAIN_16                  0x80 /* 0.625 V */
#define CH_GAIN_64                  0xC0 /* 0.15625 V */
#define CH_MODE_DIFF                0x00
#define CH_MODE_CGROUND             0x20
#define CH_MEASURE_ZERO             0x10 /* only when CH_MODE_CGROUND == 0 */

#define E140_DIAG_ADDR              0x10C0
typedef struct
    {
    DWORD dev_state;
    DWORD adc_overrun_count;
    DWORD dac_underrun_count;
    DWORD adc_sample_count;
    DWORD dac_sample_count;
    }
    e140_diag_t;
#pragma pack()
/*================================================================================================*/

/*================================================================================================*/
/*------------------------------------------------------------------------------------------------*/
int init_device(ILE140* device)
    {
    char module_name[32];
    MODULE_DESCRIPTION_E140 dev_desc;
    ADC_PARS_E140 ap;
    int ok = 0;

    for (int i = 0; !ok && (i < 127); i++)
        {
        ok = device->OpenLDevice(i);
        }
    if (!ok)
        {
        fprintf(stderr, "Connection failed: E14-140 not found.\n");
        return 0;
        }

    if (!device->GetModuleName(module_name))
        {
        fprintf(stderr, "GetModuleName() failed.\n");
        return 0;
        }

    if (!device->GET_MODULE_DESCRIPTION(&dev_desc))
        {
        fprintf(stderr, "GET_MODULE_DESCRIPTION() failed.\n");
        return 0;
        }

    printf("Connected to %s (S/N %s, Fosc = %.0f MHz).\n",
        module_name, dev_desc.Module.SerialNumber, (dev_desc.Mcu.ClockRate / 1000.0));

    if (!device->STOP_ADC())
        {
        fprintf(stderr, "Cannot stop ADC during initialization.\n");
        return 0;
        }

    /* Заполняем конфигурацию АЦП */
    ZeroMemory(&ap, sizeof(ap));
    ap.ClkSource = INT_ADC_CLOCK_E140;
    ap.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E140;
    ap.InputMode = NO_SYNC_E140;
    ap.ChannelsQuantity = 1;
    ap.ControlTable[0] = CH_GAIN(GAIN) | CH_MODE_CGROUND | 0;
    ap.AdcRate = (double)SAMPLE_FREQ / 1000.0;
    ap.InterKadrDelay = 0.0;
    if (!device->SET_ADC_PARS(&ap))
        {
        fprintf(stderr, "Cannot set ADC parameters.\n");
        return 0;
        }

    return 1;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int show_stats(ILE140* device)
    {
    e140_diag_t diag;
    if (!device->GetArray((BYTE*)&diag, sizeof(diag), E140_DIAG_ADDR))
        {
        fprintf(stderr, "Cannot query device diagnostics.\n");
        return 0;
        }

    printf("-----------\n"
        "Statistics:\n"
        "ADC: %lu samples, %lu overruns\n",
        diag.adc_sample_count, diag.adc_overrun_count);
    return 1;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int data_loop(ILE140* device)
    {
    SHORT adc_buf[ADC_QUEUE_LEN][ADC_BUF_NSAMPLES];
    HANDLE h_event_adc[ADC_QUEUE_LEN];
    OVERLAPPED ov_adc[ADC_QUEUE_LEN];
    HANDLE h_device;
    unsigned int ri, error_flag = 0;
    WORD ttl_out = 0;

    h_device = device->GetModuleHandle();
    if (INVALID_HANDLE_VALUE == h_device)
        {
        fprintf(stderr, "GetModuleHandle() failed.\n");
        return 0;
        }

    /* Инициализация переменных */
    for (unsigned int i = 0; i < ARRLEN(h_event_adc); i++)
        {
        h_event_adc[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!h_event_adc[i])
            {
            fprintf(stderr, "Cannot initialize events\n");
            while (i) CloseHandle(h_event_adc[--i]);
            return 0;
            }
        }

    ZeroMemory(ov_adc, sizeof(ov_adc));

    /* Постановка в очередь запросов чтения */
    for (ri = 0; ri < ADC_QUEUE_LEN; ri++)
        {
        ZeroMemory(&ov_adc[ri], sizeof(ov_adc[ri]));
        ov_adc[ri].hEvent = h_event_adc[ri];
        if (!ReadFile(h_device, adc_buf[ri], sizeof(adc_buf[ri]), NULL, &ov_adc[ri]) && (GetLastError() != ERROR_IO_PENDING))
            {
            fprintf(stderr, "ReadFile failed (%lu)\n", GetLastError());
            error_flag = 1;
            break;
            }
        }

    if (!error_flag)
        {
        /* Запуск АЦП */
        if (!device->START_ADC())
            {
            fprintf(stderr, "Cannot start ADC\n");
            error_flag = 1;
            }

        if (!device->TTL_OUT(ttl_out) || !device->ENABLE_TTL_OUT(1))
            {
            fprintf(stderr, "Cannot enable TTL output\n");
            error_flag = 1;
            }
        }

    ri = 0;
    while (!error_flag && !kbhit())
        { 
        DWORD read_nbytes, in_samples;
        WORD ttl_out_new = ttl_out;

        if (WaitForSingleObject(h_event_adc[ri], TIMEOUT_MS) != WAIT_OBJECT_0)
            {
            fprintf(stderr, "Error waiting for data\n");
            error_flag = 1;
            break;
            }
        if (!GetOverlappedResult(h_device, &ov_adc[ri], &read_nbytes, TRUE))
            {
            fprintf(stderr, "Async read failed (%lu)\n", GetLastError());
            error_flag = 1; /* ошибка чтения */
            break;
            }

        /* Обработка данных */
        in_samples = read_nbytes / sizeof(SHORT);
        for (unsigned int i = 0; i < in_samples; i++)
            {
            SHORT x = adc_buf[ri][i];
            if (x > ADC_CODE_V(GAIN, U_HI))
                {
                ttl_out_new = 1;
                }
            else if (x < ADC_CODE_V(GAIN, U_LO))
                {
                ttl_out_new = 0;
                }
            }

        /* Постановка запроса в конец очереди */
        ZeroMemory(&ov_adc[ri], sizeof(ov_adc[ri]));
        ov_adc[ri].hEvent = h_event_adc[ri];
        if (!ReadFile(h_device, adc_buf[ri], sizeof(adc_buf[ri]), NULL, &ov_adc[ri]) && (GetLastError() != ERROR_IO_PENDING))
            {
            fprintf(stderr, "ReadFile failed (%lu)\n", GetLastError());
            error_flag = 1;
            break;
            }

        /* Вывод TTL */
        if (ttl_out_new != ttl_out)
            {
            ttl_out = ttl_out_new;
            if (!device->TTL_OUT(ttl_out))
                {
                fprintf(stderr, "Cannot write TTL!\n");
                error_flag = 1;
                }
            }

        if (++ri >= ADC_QUEUE_LEN) ri = 0;
        } /* while */

    show_stats(device);

    /* Остановка АЦП */
    if (!device->STOP_ADC())
        {
        fprintf(stderr, "Cannot stop ADC\n");
        error_flag = 1;
        }

    /* Отключение TTL */
    if (!device->TTL_OUT(0) || !device->ENABLE_TTL_OUT(0))
        {
        fprintf(stderr, "Cannot disable TTL output\n");
        error_flag = 1;
        }

    /* Завершение работы */
    CancelIo(h_device);
    for (unsigned int i = 0; i < ARRLEN(h_event_adc); i++)
        {
        DWORD dummy;
        GetOverlappedResult(h_device, &ov_adc[i], &dummy, TRUE);
        CloseHandle(h_event_adc[i]);
        }
    return !error_flag;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main(void)
    {
    ILE140* dev;

    printf("** E14-140-MD: edge detector **\n\n");

    /* Проверка версии Lusbapi.dll */
    DWORD dll_ver = GetDllVersion();
    if (dll_ver != CURRENT_VERSION_LUSBAPI)
        {
        fprintf(stderr, "lusbapi.dll version mismatch (found %lu.%lu, need %u.%u)\n",
            dll_ver >> 16, dll_ver & 0xFFFF,
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF);
        return 1;
        }

    /* Соединяемся с модулем */
    dev = static_cast<ILE140 *>(CreateLInstance((char*)"e140"));
    if (!dev)
        {
        fprintf(stderr, "Cannot create module interface.\n");
        return 2;
        }

    if (!init_device(dev))
        {
        dev->ReleaseLInstance();
        return 3;
        }

    printf("Data rate: %.0f kHz\n", SAMPLE_FREQ / 1000.0);

    while (kbhit()) getch();
    printf("Press any key to exit...\n");

    HANDLE h_process = GetCurrentProcess();
    DWORD saved_prio = GetPriorityClass(h_process);
    SetPriorityClass(h_process, HIGH_PRIORITY_CLASS);

    data_loop(dev);

    SetPriorityClass(h_process, saved_prio);

    while (kbhit()) getch();

    printf("Done.\n");
//    printf("Press any key\n"); getch();

    dev->ReleaseLInstance();
    return 0;
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
