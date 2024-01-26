//******************************************************************************
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <ctype.h>
#include "Lusbapi.h"
#include <math.h>
#include <getopt.h>

void AbortProgram(const char *ErrorString, ...);

ILE140 *pModule;
HANDLE ModuleHandle;

#define DAC_RATE        200000
#define F_MIN           (DAC_RATE / 5120.0)
#define F_MAX           (DAC_RATE / 64.0)
#define F_DFLT          50.0

#define U_MAX           5.0
#define AMP_COEFF       (32768.0 / U_MAX)

/* Режим калибровки: 0 - не использовать, 1 - заводская, 2 - собственная */
#define CALIB_MODE      1

#if CALIB_MODE > 1
/* Собственные калибровки -- тест -- подбор для конкретного экземпляра */
#define CALIB_OFS_0     2.678784 // 2.703360
#define CALIB_OFS_1     4.284416 // 4.282368
#define CALIB_SCALE_0   0.982044 // 0.982059
#define CALIB_SCALE_1   0.984028 // 0.984035
#endif

/* Calibration:
 * x_cal = (x_raw + offset) * scale
 *
 * x1 -> y1, x2 -> y2
 *
 * offset = (x1 * y2 - x2 * y1) / (x2 - x1)
 * scale  = (x2 - x1) / (y2 - y1)
 */

#define ADDR_DAC_PARS   0x0160
#define ADDR_DIAG_DACUC 0x10C8 // DAC underrun count

typedef struct { SHORT ch[2]; } t_frame;

#define DAC_MODE_SET_CONST_ON_STOP  0x01    /* при останове ЦАП установить на выходе константу
                                               params.StopState */
#define DAC_MODE_NO_CLEAR_BUFFER    0x02    /* не очищать буфер при автоматической остановке
                                               по достижению TotalSamples в циклическом режиме */
#define DAC_MODE_LOOP               0x10    /* циклический режим (0 = потоковый режим) */

#pragma pack(1)
typedef struct
    {
    unsigned short RateDiv;         /* Делитель частоты, f = 200000/(RateDiv+1) Hz */
    unsigned char SyncWithADC;      /* 0 = обычный пуск; 1 = синхронизировать с пуском АЦП */
    unsigned char RunMode;          /* Режим работы (сумма DAC_MODE_...) */
    unsigned int TotalSamples;      /* Количество отсчетов (в DWORD), по достижении которого
                                       делается автоматический стоп (в потоковом или циклическом
                                       режиме). Если 0, то бесконечно. */
    unsigned short PreloadSamples;  /* поток. режим: размер данных в DWORD, достаточный для пуска
                                       цикл. режим: размер повторяемой последовательности в DWORD */
                                    /* Если 0, то по умолчанию DAC_DEFAULT_PRELOAD_SAMPLES. */
    short StopState[2];             /* Константы (по 2 каналам) для установки при останове,
                                       если (RunMode & DAC_MODE_SET_CONST_ON_STOP) != 0 */
    /* Резерв дла расширения */
    unsigned char Reserved[2];
    }
    t_DAC_ParamStruct;
#pragma pack()

enum en_option
    {
    OPT_AMP0    = 0x01,
    OPT_RMS0    = 0x02,
    OPT_AMP1    = 0x04,
    OPT_RMS1    = 0x08,
    OPT_PHI     = 0x10,
    OPT_SINGLE  = 0x20,
    OPT_FREQ    = 0x40
    };
static const struct option options[] =
    {
    { "freq",   required_argument, NULL, OPT_FREQ },
    { "f",      required_argument, NULL, OPT_FREQ },
    { "amp0",   required_argument, NULL, OPT_AMP0 },
    { "a0",     required_argument, NULL, OPT_AMP0 },
    { "rms0",   required_argument, NULL, OPT_RMS0 },
    { "r0",     required_argument, NULL, OPT_RMS0 },
    { "amp1",   required_argument, NULL, OPT_AMP1 },
    { "a1",     required_argument, NULL, OPT_AMP1 },
    { "rms1",   required_argument, NULL, OPT_RMS1 },
    { "r1",     required_argument, NULL, OPT_RMS1 },
    { "phi",    required_argument, NULL, OPT_PHI },
    { "phase",  required_argument, NULL, OPT_PHI },
    { "single", no_argument,       NULL, OPT_SINGLE },
    { NULL, 0, NULL, 0 }
    };

//------------------------------------------------------------------------
static void f_ShowDiag()
    {
    DWORD underruns;
    if (!pModule->GetArray((BYTE*)&underruns, sizeof(underruns), ADDR_DIAG_DACUC))
        {
        printf("Cannot read underrun counter!\n");
        }
    else
        {
        if (underruns)
            printf("Warning: %lu DAC underruns detected!\n", underruns);
        }
    }
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static void f_WaitKey()
    {
    while (kbhit()) getch();
    if (getch() == 0x1B)
        {
        pModule->STOP_DAC();
        AbortProgram("User pressed ESC\n");
        }
    }
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static double f_optval(const char* name, const char* str, double min_val, double max_val)
    {
    char* peol;
    double val;

    val = strtod(str, &peol);
    if (!peol || *peol)
        AbortProgram("Parameter %s = \"%s\" is not a number.\n", name, str);
    if ((val < min_val) || (val > max_val))
        AbortProgram("Parameter %s = \"%s\" is out of range (%1.2f .. %1.2f).\n", name, str, min_val, max_val);
    return val;
    }
//------------------------------------------------------------------------

//------------------------------------------------------------------------
static SHORT f_calib(int ch, const struct DAC_INFO_LUSBAPI* dac, double val)
    {
    double x = (val + dac->OffsetCalibration[ch] * 16.0) * dac->ScaleCalibration[ch];
    if (x < -32768.0) x = -32768.0;
    if (x > 32767.0) x = 32767.0;
    return (SHORT)((x < 0) ? x - 0.5 : x + 0.5);
    }
//------------------------------------------------------------------------

//------------------------------------------------------------------------
int main(int argc, char* argv[])
    {
    DWORD DLL_Ver;
    int i, opt, optflag;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 DevDesc;
    OVERLAPPED ov;
    DWORD xfer_size;
    t_DAC_ParamStruct _dp;
    INT wait_result;
    double amp[2] = { 0, 0 };
    double phase = 0, freq = F_DFLT;

    /* Проверка версии Lusbapi.dll */
    DLL_Ver = GetDllVersion();
    if (DLL_Ver != CURRENT_VERSION_LUSBAPI)
        AbortProgram("Lusbapi.dll version mismatch (found %lu.%lu, need %u.%u)\n",
            DLL_Ver >> 16, DLL_Ver & 0xFFFF,
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF);

    /* Разбор параметров */
    optflag = 0;
    while (0 <= (opt = getopt_long_only(argc, argv, "", options, &i)))
        {
        switch (opt)
            {
            case OPT_AMP0:
            case OPT_AMP1:
                amp[OPT_AMP1 == opt] = f_optval(options[i].name, optarg, -U_MAX, U_MAX);
                break;
            case OPT_RMS0:
            case OPT_RMS1:
                amp[OPT_RMS1 == opt] =
                    f_optval(options[i].name, optarg, -U_MAX/M_SQRT2, U_MAX/M_SQRT2) * M_SQRT2;
                break;
            case OPT_PHI:
                phase = f_optval(options[i].name, optarg, -180.0, 180.0);
                break;
            case OPT_FREQ:
                freq = f_optval(options[i].name, optarg, F_MIN, F_MAX);
                break;
            case OPT_SINGLE:
                break;
            default:
                opt = 0;
                break;
            }
        optflag |= opt;
        }

    if (!(optflag & (OPT_RMS0 | OPT_AMP0))
        || !(optflag & (OPT_RMS1 | OPT_AMP1))
        || !(optflag & (OPT_PHI | OPT_SINGLE)))
        {
        AbortProgram(
            "Usage: dac_sine {-a[mp]0 volts | -r[ms]0 volts} {-a[mp]1 volts | -r[ms]1 volts}\n"
            "                -{phi | phase} degrees [-single] [-f[req] hz]\n"
            "    -amp0, -a0:     Channel 0 amplitude, %1.0f..%1.0f V\n"
            "    -rms0, -r0:     Channel 0 RMS voltage, %1.3f..%1.3f V\n"
            "    -amp1, -a1:     Channel 1 amplitude, %1.0f..%1.0f V\n"
            "    -rms1, -r1:     Channel 1 RMS voltage, %1.3f..%1.3f V\n"
            "    -phase, -phi:   Channel 1 phase, -180...180 degrees\n"
            "    -freq, -f:      Sine frequency, %1.2f..%1.2f Hz, default %1.0f Hz\n"
            "    -single:        DC mode (output amplitude values and exit)\n"
            "\n"
            "    For each channel, specify either amplitude or RMS.\n"
            "    When using -single, frequency and phase are not needed.\n",

            -U_MAX, U_MAX, -U_MAX/M_SQRT2, U_MAX/M_SQRT2,
            -U_MAX, U_MAX, -U_MAX/M_SQRT2, U_MAX/M_SQRT2,
            F_MIN, F_MAX, F_DFLT
            );
        }

    /* Соединяемся с модулем */
    pModule = static_cast<ILE140 *>(CreateLInstance((char*)"e140"));
    if (!pModule)
        AbortProgram("Connection failed: Cannot create module interface.\n");
    for (i = 0; i < 127; i++)
        {
        if (pModule->OpenLDevice((WORD)i))
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

    printf("Connected to %s (S/N %s).\n", ModuleName, DevDesc.Module.SerialNumber);

    /* Принудительная установка калибровочных коэффициентов */
#if CALIB_MODE == 0
    DevDesc.Dac.OffsetCalibration[0] = 0.0; DevDesc.Dac.ScaleCalibration[0] = 1.0;
    DevDesc.Dac.OffsetCalibration[1] = 0.0; DevDesc.Dac.ScaleCalibration[1] = 1.0;
#elif CALIB_MODE > 1
    DevDesc.Dac.OffsetCalibration[0] = CALIB_OFS_0; DevDesc.Dac.ScaleCalibration[0] = CALIB_SCALE_0;
    DevDesc.Dac.OffsetCalibration[1] = CALIB_OFS_1; DevDesc.Dac.ScaleCalibration[1] = CALIB_SCALE_1;
#endif
    
    /* Вывод калибровочных коэффициентов */
    printf("Calibration coeffs:\n"
           "ch0\toffset = %1.6f\tscale = %1.6f\n"
           "ch1\toffset = %1.6f\tscale = %1.6f\n",
           DevDesc.Dac.OffsetCalibration[0], DevDesc.Dac.ScaleCalibration[0],
           DevDesc.Dac.OffsetCalibration[1], DevDesc.Dac.ScaleCalibration[1]);

    if (!pModule->STOP_DAC())
        AbortProgram("Cannot stop DAC.\n");

    if (optflag & OPT_SINGLE)
        {
        t_frame frame;
        frame.ch[0] = f_calib(0, &DevDesc.Dac, AMP_COEFF * amp[0]);
        frame.ch[1] = f_calib(1, &DevDesc.Dac, AMP_COEFF * amp[1]);
        printf("Setting output to:\n"
               "ch0\tu = %1.6f V\tDAC value = %d\n"
               "ch1\tu = %1.6f V\tDAC value = %d\n",
               amp[0], frame.ch[0], amp[1], frame.ch[1]);

        if (!pModule->DAC_SAMPLES(&frame.ch[0], &frame.ch[1]))
            AbortProgram("DAC write error.\n");
        }
    else
        {
        unsigned int wave_samples = (unsigned int)(DAC_RATE / freq);
        // Количество отсчетов в буфере (с добивкой до 256 байт)
        unsigned int buf_samples = (wave_samples + 63) & ~63;

        t_frame* buf = (t_frame*)malloc(buf_samples * sizeof(t_frame));
        if (!buf)
            AbortProgram("Cannot allocate memory.\n");

        freq = DAC_RATE / wave_samples; /* пересчет реально получившейся частоты */

        amp[0] = fabs(amp[0]);
        amp[1] = fabs(amp[1]);

        /* Заполняем массив */
        for (unsigned int k = 0; k < wave_samples; k++)
            {
            /* Канал 0 */
            buf[k].ch[0] = f_calib(0, &DevDesc.Dac,
                AMP_COEFF * amp[0] * sin(M_PI * (2.0 * (double)k / wave_samples))
                );
            /* Канал 1 */
            buf[k].ch[1] = f_calib(1, &DevDesc.Dac,
                AMP_COEFF * amp[1] * sin(M_PI * (2.0 * (double)k / wave_samples + phase / 180.0))
                );
            }

        /* Готовим структуру для overlapped */
        ZeroMemory(&ov, sizeof(ov));
        ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        memset(&_dp, 0, sizeof(_dp));
        _dp.RateDiv = (200000 / DAC_RATE) - 1;
        _dp.SyncWithADC = 0;
        _dp.RunMode = 0
            | DAC_MODE_SET_CONST_ON_STOP
            | DAC_MODE_LOOP
            ;
        _dp.TotalSamples = 0;
        _dp.PreloadSamples = (USHORT)wave_samples;
        _dp.StopState[0] = f_calib(0, &DevDesc.Dac, 0.0);
        _dp.StopState[1] = f_calib(1, &DevDesc.Dac, 0.0);
        if (!pModule->PutArray((BYTE*)&_dp, sizeof(_dp), ADDR_DAC_PARS))
            AbortProgram("Cannot set DAC parameters.\n");
        if (!pModule->GetArray((BYTE*)&_dp, sizeof(_dp), ADDR_DAC_PARS))
            AbortProgram("Cannot get DAC parameters to check.\n");

        if (!WriteFile(ModuleHandle, (BYTE*)buf, buf_samples * sizeof(t_frame), NULL, &ov)
            && (GetLastError() != ERROR_IO_PENDING))
            AbortProgram("WriteFile() failed.\n");

        wait_result = WaitForSingleObject(ov.hEvent, 5000);

        if (wait_result != WAIT_OBJECT_0)
            {
            CancelIo(ModuleHandle);
            GetOverlappedResult(ModuleHandle, &ov, &xfer_size, TRUE);
            if (WAIT_TIMEOUT == wait_result)
                {
                AbortProgram("WaitForSingleObject timed out.\n");
                }
            else
                {
                AbortProgram("WaitForSingleObject failed (error %ld).\n", GetLastError());
                }
            }

        if (!GetOverlappedResult(ModuleHandle, &ov, &xfer_size, FALSE))
            AbortProgram("GetOverlappedResult() failed (error %ld).\n", GetLastError());

        free(buf);
        
        if (!pModule->START_DAC())
           AbortProgram("Cannot start DAC.\n");

        printf("Generating waveform: F = %1.2f Hz, Ua0 = %1.3f V (DAC %d), Ua1 = %1.3f V (DAC %d), phi = %g\n"
               "Press any key to stop.\n",
            freq,
            amp[0], f_calib(0, &DevDesc.Dac, AMP_COEFF * amp[0]),
            amp[1], f_calib(1, &DevDesc.Dac, AMP_COEFF * amp[1]),
            phase);

        f_WaitKey();
        f_ShowDiag();

        if (!pModule->STOP_DAC())
            AbortProgram("Cannot stop DAC.\n");
        }
    
    pModule->ReleaseLInstance();
    puts("Done.");
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
