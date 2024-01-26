/*================================================================================================*
 * Повторитель формы сигнала АЦП (канал 1, режим с общей землей, 20Vpp) на ЦАП (канал 1, 10Vpp)
 *
 * Программа пытается читать АЦП маленькими буферами и писать копию сигнала на ЦАП.
 * В зависимости от производительности компьютера может быть достижима большая или меньшая задержка.
 * В конце работы выводится статистика опустошений буфера, должно быть 0 underruns.
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
#define ADC_BUF_NSAMPLES            (64 * 5) /* Чем больше, тем больше задержка и выше стабильность */
#define ADC_QUEUE_LEN               32
#define DAC_QUEUE_LEN               2
#define TIMEOUT_MS                  2000

#define CH_GAIN_1                   0x00 /* 10 V */
#define CH_GAIN_4                   0x40 /* 2.5 V */
#define CH_GAIN_16                  0x80 /* 0.625 V */
#define CH_GAIN_64                  0xC0 /* 0.15625 V */
#define CH_MODE_DIFF                0x00
#define CH_MODE_CGROUND             0x20
#define CH_MEASURE_ZERO             0x10 /* only when CH_MODE_CGROUND == 0 */

#pragma pack(1)
#define E140_DAC_PARS_ADDR          0x0160
#define DAC_MODE_SET_CONST_ON_STOP  0x01    /* при STOP_DAC установить stop_state */
#define DAC_MODE_NO_CLEAR_BUFFER    0x02    /* не очищать буфер при автостопе в циклическом режиме */
#define DAC_MODE_LOOP               0x10    /* 1 = циклический режим, 0 = потоковый режим */
typedef struct
    {
    USHORT rate_div;            /* делитель частоты, f = 200000/(RateDiv+1) Hz */
    BYTE sync_with_adc;         /* 1 = синхронизировать с пуском АЦП */
    BYTE dac_mode;              /* режим работы (сумма DAC_MODE_...) */
    DWORD total_samples;        /* количество отсчетов до автостопа, 0 = бесконечно */
    USHORT preload_samples;     /* поток. режим: количество отсчетов для пуска
                                   цикл. режим: размер повторяемой последовательности в отсчетах */
    SHORT stop_state[2];        /* константы (по 2 каналам) для установки при DAC_STOP */
    BYTE reserved[2];
    }
    e140_dac_param_t;

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
    e140_dac_param_t dp;
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

    if (!device->STOP_DAC())
        {
        fprintf(stderr, "Cannot stop DAC during initialization.\n");
        return 0;
        }

    /* Заполняем конфигурацию АЦП */
    ZeroMemory(&ap, sizeof(ap));
    ap.ClkSource = INT_ADC_CLOCK_E140;
    ap.EnableClkOutput = ADC_CLOCK_TRANS_DISABLED_E140;
    ap.InputMode = NO_SYNC_E140;
    ap.ChannelsQuantity = 1;
    ap.ControlTable[0] = CH_GAIN_1 | CH_MODE_CGROUND | 0;
    ap.AdcRate = (double)SAMPLE_FREQ / 1000.0;
    ap.InterKadrDelay = 0.0;
    if (!device->SET_ADC_PARS(&ap))
        {
        fprintf(stderr, "Cannot set ADC parameters.\n");
        return 0;
        }

    /* Заполняем конфигурацию ЦАП */
    ZeroMemory(&dp, sizeof(dp));
    dp.rate_div = (200000 / SAMPLE_FREQ) - 1;
    dp.sync_with_adc = 1;
    dp.dac_mode = DAC_MODE_SET_CONST_ON_STOP;
    dp.total_samples = 0;
    dp.preload_samples = ADC_BUF_NSAMPLES;
    dp.stop_state[0] = dp.stop_state[1] = 0;
    if (!device->PutArray((BYTE*)&dp, sizeof(dp), E140_DAC_PARS_ADDR))
        {
        fprintf(stderr, "Cannot set DAC parameters.\n");
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
        "ADC: %lu samples, %lu overruns\n"
        "DAC: %lu samples, %lu underruns\n",
        diag.adc_sample_count, diag.adc_overrun_count,
        diag.dac_sample_count, diag.dac_underrun_count);
    return 1;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int data_loop(ILE140* device)
    {
    SHORT adc_buf[ADC_QUEUE_LEN][ADC_BUF_NSAMPLES];
    SHORT dac_buf[DAC_QUEUE_LEN][ADC_BUF_NSAMPLES][2];
    HANDLE h_event[ADC_QUEUE_LEN + DAC_QUEUE_LEN];
    OVERLAPPED ov[ADC_QUEUE_LEN + DAC_QUEUE_LEN];
    HANDLE h_device;
    unsigned int ri, wi, error_flag = 0;
    OVERLAPPED* const ov_adc = ov;
    OVERLAPPED* const ov_dac = ov + ADC_QUEUE_LEN;
    HANDLE* const h_event_adc = h_event;
    HANDLE* const h_event_dac = h_event + ADC_QUEUE_LEN;

    h_device = device->GetModuleHandle();
    if (INVALID_HANDLE_VALUE == h_device)
        {
        fprintf(stderr, "GetModuleHandle() failed.\n");
        return 0;
        }

    /* Инициализация переменных */
    for (unsigned int i = 0; i < ARRLEN(h_event); i++)
        {
        h_event[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (!h_event[i])
            {
            fprintf(stderr, "Cannot initialize events\n");
            while (i) CloseHandle(h_event[--i]);
            return 0;
            }
        }

    ZeroMemory(ov, sizeof(ov));
    ZeroMemory(dac_buf, sizeof(dac_buf));

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

    /* Постановка в очередь запросов записи */
    if (!error_flag)
    for (wi = 0; wi < DAC_QUEUE_LEN; wi++)
        {
        ZeroMemory(&ov_dac[wi], sizeof(ov_dac[wi]));
        ov_dac[wi].hEvent = h_event_dac[wi];
        if (!WriteFile(h_device, dac_buf[wi], sizeof(dac_buf[wi]), NULL, &ov_dac[wi]) && (GetLastError() != ERROR_IO_PENDING))
            {
            fprintf(stderr, "WriteFile failed (%lu)\n", GetLastError());
            error_flag = 1;
            break;
            }
        }

    if (!error_flag)
        {
        /* Запуск ЦАП */
        if (!device->START_DAC())
            {
            fprintf(stderr, "Cannot start DAC\n");
            error_flag = 1;
            }
        /* Запуск АЦП */
        if (!device->START_ADC())
            {
            fprintf(stderr, "Cannot start ADC\n");
            error_flag = 1;
            }
        }

    ri = wi = 0;
    while (!error_flag && !kbhit())
        { 
        DWORD read_nbytes, in_samples;

//        if (!ri) show_stats(device);

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
            dac_buf[wi][i][0] = adc_buf[ri][i] << 2; /* 14 -> 16 bit */
            dac_buf[wi][i][1] = 0;
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

        /* Запись в ЦАП */
        if (WaitForSingleObject(h_event_dac[wi], TIMEOUT_MS) != WAIT_OBJECT_0)
            {
            fprintf(stderr, "DAC write timeout.\n");
            error_flag = 1;
            }
        else
            {
            DWORD write_nbytes;
            GetOverlappedResult(h_device, &ov_dac[wi], &write_nbytes, TRUE);
            ZeroMemory(&ov_dac[wi], sizeof(ov_dac[wi]));
            ov_dac[wi].hEvent = h_event_dac[wi];
            if (!WriteFile(h_device, dac_buf[wi], in_samples * 2 * sizeof(SHORT), NULL, &ov_dac[wi]) && (GetLastError() != ERROR_IO_PENDING))
                {
                fprintf(stderr, "WriteFile failed (%lu)\n", GetLastError());
                error_flag = 1;
                }
            }

        if (++ri >= ADC_QUEUE_LEN) ri = 0;
        if (++wi >= DAC_QUEUE_LEN) wi = 0;
        } /* while */

    show_stats(device);

    /* Остановка ЦАП */
    if (!device->STOP_DAC())
        {
        fprintf(stderr, "Cannot stop DAC\n");
        error_flag = 1;
        }

    /* Остановка АЦП */
    if (!device->STOP_ADC())
        {
        fprintf(stderr, "Cannot stop ADC\n");
        error_flag = 1;
        }

    /* Завершение работы */
    CancelIo(h_device);
    for (unsigned int i = 0; i < ARRLEN(h_event); i++)
        {
        DWORD dummy;
        GetOverlappedResult(h_device, &ov[i], &dummy, TRUE);
        CloseHandle(h_event[i]);
        }
    return !error_flag;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main(void)
    {
    ILE140* dev;

    printf("** E14-140-MD: low latency repeater **\n\n");

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

    printf("Data rate: %.0f kHz, DAC-to-ADC lag: %.3f ms\n",
        SAMPLE_FREQ / 1000.0, 
        ADC_BUF_NSAMPLES * DAC_QUEUE_LEN * 1000.0 / SAMPLE_FREQ);

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
