#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "wav_loader.h"
#include "Lusbapi.h"

/*================================================================================================*/
/* Parameters for loading/resampling WAV files */
#define LOAD_MODE                   WAV_STEREO
#define LOAD_BITS                   16
#define LOAD_RATE                   50000
#define DAC_WRITE_BLOCK             256
#define WRITE_REQUEST_SIZE          0x20000

/* E14-140-M constants */
#define ADDR_VERSION                0x1080
#define ADDR_DAC_PARS               0x0160
#define ADDR_DAC_PRESENT            0x0DAC
#define ADDR_DIAG                   0x10C0

#define DAC_MODE_SET_CONST_ON_STOP  0x01    /* при останове ЦАП установить на выходе константу */
#define DAC_MODE_NO_CLEAR_BUFFER    0x02    /* не очищать буфер при автоматической остановке */
#define DAC_MODE_LOOP               0x10    /* циклический режим (0 = потоковый режим) */

#define PRELOAD_SAMPLES             4096
/*================================================================================================*/

/*================================================================================================*/
#include "packed-begin.h"
typedef struct __attribute__ ((__packed__))
    {
    uint16_t RateDiv;         /* Делитель частоты, f = 200000/(RateDiv+1) Hz */
    uint8_t SyncWithADC;      /* 0 = обычный пуск; 1 = синхронизировать с пуском АЦП */
    uint8_t RunMode;          /* Режим работы (сумма DAC_MODE_...) */
    uint32_t TotalSamples;    /* Количество отсчетов (в DWORD), по достижении которого
                                 делается автоматический стоп (в потоковом или циклическом
                                 режиме). Если 0, то бесконечно. */
    uint16_t PreloadSamples;  /* поток. режим: размер данных в DWORD, достаточный для пуска
                                 цикл. режим: размер повторяемой последовательности в DWORD */
                                    /* Если 0, то по умолчанию DAC_DEFAULT_PRELOAD_SAMPLES. */
    int16_t StopState[2];     /* Константы (по 2 каналам) для установки при останове,
                                 если (RunMode & DAC_MODE_SET_CONST_ON_STOP) != 0 */
    uint8_t Reserved[2];
    }
    t_DAC_ParamStruct;

typedef struct __attribute__ ((__packed__))
    {
    uint32_t DevState;
    uint32_t ADC_OverrunCount;
    uint32_t DAC_UnderrunCount;
    uint32_t ADC_SampleCount;
    uint32_t DAC_SampleCount;
    }
    t_DIAG_data;
#include "packed-end.h"
/*================================================================================================*/

/*================================================================================================*/
/*------------------------------------------------------------------------------------------------*/
static bool f_file_exists(const char* filename)
    {
    if (!filename) return 0;
    DWORD attr = GetFileAttributes(filename);
    return ((INVALID_FILE_ATTRIBUTES != attr) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static ILE140* f_open_device(void)
    {
    ILE140* dev = static_cast<ILE140 *>(CreateLInstance((char*)"e140"));
    if (!dev)
        return NULL;

    for (WORD i = 0; i < 127; i++)
        {
        if (!dev->OpenLDevice(i)) continue;
        do
            {
            MODULE_DESCRIPTION_E140 desc;
            uint8_t ver_info[16];
            uint8_t dac_present;
            if (!dev->GET_MODULE_DESCRIPTION(&desc))
                {
                fprintf(stderr, "Error opening device in slot %d\n", i);
                break;
                }
            printf("Found E14-140 S/N %s in slot %d\n", desc.Module.SerialNumber, i);
            if (!dev->GetArray((BYTE*)&ver_info, sizeof(ver_info), ADDR_VERSION))
                {
                fprintf(stderr, "Cannot read version info\n");
                break;
                }
            printf("Firmware version %d.%02d\n", ver_info[1], ver_info[0]);
            if (!dev->GetArray(&dac_present, sizeof(dac_present), ADDR_DAC_PRESENT))
                {
                fprintf(stderr, "Cannot query DAC presence\n");
                break;
                }
            if (((ver_info[1] << 8) | ver_info[0]) < 0x30A)
                {
                printf("Unsupported firmware version (need 3.10+), device skipped.\n");
                break;
                }
            if (!dac_present)
                {
                fprintf(stderr, "DAC not installed, cannot use this device.\n");
                break;
                }
            return dev;
            }
        while (0);
        dev->CloseLDevice();
        }

    /* not found */
    dev->ReleaseLInstance();
    return NULL;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static void f_close_device(ILE140* dev)
    {
    if (dev)
        {
        dev->CloseLDevice();
        dev->ReleaseLInstance();
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static bool f_play_wav
    (
    ILE140* dev,
    const char* file_name
    )
    {
    void* data_ptr;
    size_t data_size, data_ofs, wr_len;
    t_wav_error load_err;
    t_DAC_ParamStruct dp;
    t_DIAG_data diag;
    HANDLE hdev;
    OVERLAPPED ov;
    DWORD xfer_size;
    bool ok = true;

    if (!dev || !file_name) return false;

    hdev = dev->GetModuleHandle();
    if (INVALID_HANDLE_VALUE == hdev)
        {
        fprintf(stderr, "Cannot get module handle\n");
        return false;
        }
    
    /* Stop DAC */
    if (!dev->STOP_DAC())
        {
        fprintf(stderr, "Cannot stop DAC before init\n");
        return false;
        }

    /* Load file into memory */
    printf("Loading %s...", file_name);
    load_err = wav_load_file(file_name, LOAD_MODE, LOAD_BITS, LOAD_RATE, &data_ptr, &data_size);
    if (WAV_ERR_SUCCESS != load_err)
        {
        printf(" Failed\n");
        fprintf(stderr, "File loading error: %s\n", wav_error_string(load_err));
        return false;
        }
    printf(" Ok, %u bytes\n", data_size);

    /* Setup DAC parameters */
    ZeroMemory(&dp, sizeof(dp));
    dp.RateDiv = (200000 / LOAD_RATE) - 1;
    dp.SyncWithADC = 0;
    dp.RunMode = DAC_MODE_SET_CONST_ON_STOP;
    dp.TotalSamples = data_size / 4;
    dp.PreloadSamples = PRELOAD_SAMPLES;
    dp.StopState[0] = 0;
    dp.StopState[1] = 0;
    if (!dev->PutArray((BYTE*)&dp, sizeof(dp), ADDR_DAC_PARS))
        {
        fprintf(stderr, "Cannot set DAC parameters\n");
        free(data_ptr);
        return false;
        }

    /* Adjust data size to a multiple of DAC_WRITE_BLOCK */
    if (data_size % DAC_WRITE_BLOCK)
        {
        data_size += DAC_WRITE_BLOCK - 1;
        data_size -= (data_size % DAC_WRITE_BLOCK);
        void* new_ptr = realloc(data_ptr, data_size);
        if (!new_ptr)
            {
            fprintf(stderr, "Memory allocation error\n");
            free(data_ptr);
            return false;
            }
        data_ptr = new_ptr;
        }

    /* Start DAC (real output will begin after writing PRELOAD_SAMPLES) */
    if (!dev->START_DAC())
        {
        fprintf(stderr, "Cannot start DAC\n");
        free(data_ptr);
        return false;
        }

    printf("Press [Esc] to abort playing.\n");
    
    ZeroMemory(&ov, sizeof(ov));
    ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    data_ofs = 0;
    wr_len = data_size;
    if (wr_len > WRITE_REQUEST_SIZE) wr_len = WRITE_REQUEST_SIZE;

    if (!WriteFile(hdev, data_ptr, wr_len, &xfer_size, &ov)
        && (ERROR_IO_PENDING != GetLastError()))
        {
        fprintf(stderr, "WriteFile() failed on device (error %lu)\n", GetLastError());
        dev->STOP_DAC();
        CloseHandle(ov.hEvent);
        free(data_ptr);
        return false;
        }

    /* Main write loop */
    while (1)
        {
        if (kbhit() && ('\x1B' == getch()))
            {
            printf("\nAborted.\n");
            break;
            }

        /* Check if done writing */
        if (data_ofs < data_size)
            {
            if (WAIT_OBJECT_0 == WaitForSingleObject(ov.hEvent, 250))
                {
                if (!GetOverlappedResult(hdev, &ov, &xfer_size, TRUE))
                    {
                    puts("");
                    fprintf(stderr, "Async I/O failed on device (error %lu)\n", GetLastError());
                    ok = false;
                    break;
                    }
                data_ofs += wr_len;
                wr_len = data_size - data_ofs;
                if (wr_len > WRITE_REQUEST_SIZE) wr_len = WRITE_REQUEST_SIZE;
                if (wr_len && !WriteFile(hdev, (uint8_t*)data_ptr + data_ofs, wr_len, &xfer_size, &ov)
                    && (ERROR_IO_PENDING != GetLastError()))
                    {
                    fprintf(stderr, "WriteFile() failed on device (error %lu)\n", GetLastError());
                    ok = false;
                    break;
                    }
                }
            }
        else
            {
            Sleep(250);
            }

        /* Check if DAC stopped and how many samples played */
        if (!dev->GetArray((BYTE*)&diag, sizeof(diag), ADDR_DIAG))
            {
            puts("");
            fprintf(stderr, "Error reading device state\n");
            break;
            }

        printf("\rPlaying... (%1.0f%%)", 100.0 * diag.DAC_SampleCount / dp.TotalSamples);
        if (diag.DAC_UnderrunCount > 0)
            printf(", %u underruns", diag.DAC_UnderrunCount);

        if (!(diag.DevState & 0x02))
            {
            printf("\nDone.");
            break;
            }

        } /* while (1) */

    CancelIo(hdev);
    GetOverlappedResult(hdev, &ov, &xfer_size, TRUE);

    dev->STOP_DAC();
    CloseHandle(ov.hEvent);
    free(data_ptr);
    return ok;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
    {
    ILE140* device;

    /* Check lusbapi version */
    DWORD dll_ver = GetDllVersion();
    if (dll_ver != CURRENT_VERSION_LUSBAPI)
        {
        fprintf(stderr, "Lusbapi.dll version mismatch (found %lu.%lu, need %u.%u)\n",
            dll_ver >> 16, dll_ver & 0xFFFF,
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF);
        return 1;
        }

    if (argc < 2)
        {
        fprintf(stderr, "Usage: e140-wav-player filename.wav\n");
        return 2;
        }

    if (!f_file_exists(argv[1]))
        {
        fprintf(stderr, "File %s does not exist\n", argv[1]);
        return 2;
        }

    device = f_open_device();
    if (!device)
        {
        fprintf(stderr, "Cannot find E14-140\n");
        return 3;
        }

    /* Run wav file player */
    bool play_ok = f_play_wav(device, argv[1]);

    f_close_device(device);
    return (play_ok) ? 0 : 4;
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
