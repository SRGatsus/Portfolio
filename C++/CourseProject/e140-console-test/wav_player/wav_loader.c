/*================================================================================================*
 * WAV file loader with resampling
 *================================================================================================*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
/* Disable deprecation warnings for io.h */
#pragma warning(disable:4996)
/* Disable "shift count negative or too big" triggering on RESAMPLE_ macros */
#pragma warning(disable:4293)
#endif

#include "wav_loader.h"
#include "resample.h"
#include "common.h"

/*================================================================================================*/
/* WAV header constants */
#define WAV_COMPRESSION_PCM         0x0001  /* Uncompressed PCM */
#define WAV_COMPRESSION_EXTENSIBLE  0xFFFE  /* Wave Format Extensible */

const uint16_t wav_extpcm_guid[8] =
    { 0x0001, 0x0000, 0x0000, 0x0010, 0x0080, 0xAA00, 0x3800, 0x719B };

/* Size of temporary buffer, bytes */
#define WAV_FILEREAD_SIZE           (128 * 1024)

/* WAV header structures */
#include "packed-begin.h"

typedef struct s_wav_riff_header
    {
    char chunk_id[4];               /* 'RIFF' */
    uint32_t chunk_size;            /* = file_size - 8 */
    char type_id[4];                /* 'WAVE' for WAV file */
    }
    t_wav_riff_header;

typedef struct s_wav_fmt_header
    {
    char chunk_id[4];               /* 'fmt ' */
    uint32_t chunk_size;            /* = chunk_size - 8 */
    uint16_t compression;           /* audio compression */
    uint16_t num_channels;          /* number of channels */
    uint32_t sample_rate;           /* sampling rate, Hz */
    uint32_t bytes_per_second;      /* = sample_rate * block_align */
    uint16_t block_align;           /* = bits_per_sample / 8 * num_channels */
    uint16_t bits_per_sample;       /* bits per sample */
    }
    t_wav_fmt_header;

typedef struct s_wav_fmt_header_ex
    {
    uint16_t size;                  /* size of extra data in bytes */
    uint16_t significant_bits;      /* significant bits per sample */
    uint32_t channel_mask;          /* mask of channels present in multi-channel stream (not used) */
    uint16_t subformat_guid[8];     /* subformat GUID */
    }
    t_wav_fmt_header_ex;

typedef struct s_wav_chunk_header
    {
    char chunk_id[4];               /* chunk ID */
    uint32_t chunk_size;            /* = chunk_size - 8 */
    }
    t_wav_chunk_header;

#include "packed-end.h"

/* Control structure for resampling */
typedef struct s_wav_rsmpl
    {
    void* buf;                      /* Data buffer */
    int error;                      /* Overflow indicator (for output) */
    size_t frame_size;              /* Size of 1 sample frame in bytes (== numchan * bits / 8) */
    size_t offset;                  /* Current offset in buffer, bytes */
    size_t size;                    /* Size of buffer, bytes */
    }
    t_wav_rsmpl;
/*================================================================================================*/

/*================================================================================================*/
static const char* f_wav_error_string[] =
    {
/* WAV_ERR_SUCCESS = 0,               */    "No error",
/* WAV_ERR_BADPARAM,                  */    "Invalid parameter",
/* WAV_ERR_FILE_NOT_FOUND,            */    "File not found",
/* WAV_ERR_FILE_READ_ERROR,           */    "File read error",
/* WAV_ERR_WRONG_FILE_FORMAT,         */    "File is not in WAV format",
/* WAV_ERR_CHUNKS_OUT_OF_ORDER,       */    "Wrong order of WAV chunks (fmt not first)",
/* WAV_ERR_UNSUPPORTED_COMPRESSION,   */    "File is not in uncompressed PCM format",
/* WAV_ERR_NOT_ENOUGH_MEMORY,         */    "Not enough memory",
/* WAV_ERR_NO_DATA,                   */    "No sample data in file",
/* WAV_ERR_WRONG_NUM_CHANNELS_IN,     */    "Unsupporetd number of channels in file",
/* WAV_ERR_WRONG_BITS_PER_SAMPLE_IN,  */    "Unsupported number of bits per sample in file",
/* WAV_ERR_WRONG_BITS_PER_SAMPLE_OUT, */    "Unsupported number of bits per sample in output",
/* WAV_ERR_WRONG_SAMPLING_RATE_IN,    */    "Wrong sampling rate in file",
/* WAV_ERR_WRONG_SAMPLING_RATE_OUT,   */    "Wrong sampling rate in output",
/* WAV_ERR_RESAMPLING_OVERFLOW        */    "Resampling buffer exhausted (internal error)",
/* WAV_ERR_RESAMPLING_FAILED          */    "Resampling failed (check sampling rates)"
    };
/*================================================================================================*/

/*================================================================================================*/
/* Callbacks for coverting data between input/output streams and resampler internal format */
#define PTR_ADD(ptr, ofs)   ((char*)(ptr) + (ofs))
#define INT24(ptr)          (((int32_t)((int8_t*)(ptr))[2] << 16)       \
    | ((uint16_t)((uint8_t*)(ptr))[1] << 8)                             \
    | ((uint8_t*)(ptr))[0])

#define DECLARE_ACCESSOR(func_name, sample_type, body)                  \
static int func_name                                                    \
    (void* param, t_rsmpl_sample* frame)                                \
    {                                                                   \
    t_wav_rsmpl* ctl = (t_wav_rsmpl*)param;                             \
    size_t new_ofs = ctl->offset + ctl->frame_size;                     \
    sample_type* data = (sample_type*)PTR_ADD(ctl->buf, ctl->offset);   \
    if (new_ofs > ctl->size)                                            \
        {                                                               \
        ctl->error = 1;                                                 \
        return 0;                                                       \
        }                                                               \
    body                                                                \
    ctl->offset = new_ofs;                                              \
    return 1;                                                           \
    }

/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Unsigned 8-bit mono */
DECLARE_ACCESSOR(cb_read_u8_mono, uint8_t,
    frame[0] = RESAMPLE_FROM_INT(8, (int)data[0] - 128);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Unsigned 8-bit stereo-to-mono sum */
DECLARE_ACCESSOR(cb_read_u8_sum, uint8_t,
    frame[0] = RESAMPLE_FROM_INT(9, (int)data[0] + (int)data[1] - 256);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Unsigned 8-bit stereo */
DECLARE_ACCESSOR(cb_read_u8_stereo, uint8_t,
    frame[0] = RESAMPLE_FROM_INT(8, (int)data[0] - 128);
    frame[1] = RESAMPLE_FROM_INT(8, (int)data[1] - 128);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 16-bit mono */
DECLARE_ACCESSOR(cb_read_s16_mono, int16_t,
    frame[0] = RESAMPLE_FROM_INT(16, data[0]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 16-bit stereo-to-mono sum */
DECLARE_ACCESSOR(cb_read_s16_sum, int16_t,
    frame[0] = RESAMPLE_FROM_INT(17, (int32_t)data[0] + data[1]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 16-bit stereo */
DECLARE_ACCESSOR(cb_read_s16_stereo, int16_t,
    frame[0] = RESAMPLE_FROM_INT(16, data[0]);
    frame[1] = RESAMPLE_FROM_INT(16, data[1]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 24-bit mono */
DECLARE_ACCESSOR(cb_read_s24_mono, int8_t,
    frame[0] = RESAMPLE_FROM_INT(24, INT24(data));
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 24-bit stereo-to-mono sum */
DECLARE_ACCESSOR(cb_read_s24_sum, int8_t,
    frame[0] = RESAMPLE_FROM_INT(25, INT24(data) + INT24(data + 3));
    )
/*------------------------------------------------------------------------------------------------*/
/* Data read accessor: Signed 24-bit stereo */
DECLARE_ACCESSOR(cb_read_s24_stereo, int8_t,
    frame[0] = RESAMPLE_FROM_INT(24, INT24(data));
    frame[1] = RESAMPLE_FROM_INT(24, INT24(data + 3));
    )
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 16-bit mono */
DECLARE_ACCESSOR(cb_write_s16_mono, int16_t,
    data[0] = (int16_t)RESAMPLE_TO_INT(16, frame[0]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 16-bit mono-to-stereo copy */
DECLARE_ACCESSOR(cb_write_s16_copy, int16_t,
    data[1] = data[0] = (int16_t)RESAMPLE_TO_INT(16, frame[0]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 16-bit stereo */
DECLARE_ACCESSOR(cb_write_s16_stereo, int16_t,
    data[0] = (int16_t)RESAMPLE_TO_INT(16, frame[0]);
    data[1] = (int16_t)RESAMPLE_TO_INT(16, frame[1]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 32-bit mono */
DECLARE_ACCESSOR(cb_write_s32_mono, int32_t,
    data[0] = (int32_t)RESAMPLE_TO_INT(32, frame[0]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 32-bit mono-to-stereo copy */
DECLARE_ACCESSOR(cb_write_s32_copy, int32_t,
    data[1] = data[0] = (int32_t)RESAMPLE_TO_INT(32, frame[0]);
    )
/*------------------------------------------------------------------------------------------------*/
/* Data write accessor: Signed 32-bit stereo */
DECLARE_ACCESSOR(cb_write_s32_stereo, int32_t,
    data[0] = (int32_t)RESAMPLE_TO_INT(32, frame[0]);
    data[1] = (int32_t)RESAMPLE_TO_INT(32, frame[1]);
    )
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
/* Table of pointers to read accessors. First index: bit width, second index: MONO/MIXED/STEREO */
static const struct
    {
    unsigned int bits;              /* Bits per sample */
    unsigned int container_size;    /* Size of 1-channel sample in bytes */
    t_rsmpl_accessor callback[3];
    }
    f_cb_read[] =
    {
        { 8,  1, { cb_read_u8_mono,     cb_read_u8_sum,     cb_read_u8_stereo   }},
        { 16, 2, { cb_read_s16_mono,    cb_read_s16_sum,    cb_read_s16_stereo  }},
        { 24, 3, { cb_read_s24_mono,    cb_read_s24_sum,    cb_read_s24_stereo  }}
    },
    f_cb_write[] =
    {
        { 16, 2, { cb_write_s16_mono,   cb_write_s16_copy,  cb_write_s16_stereo }},
        { 20, 4, { cb_write_s32_mono,   cb_write_s32_copy,  cb_write_s32_stereo }},
        { 24, 4, { cb_write_s32_mono,   cb_write_s32_copy,  cb_write_s32_stereo }},
        { 32, 4, { cb_write_s32_mono,   cb_write_s32_copy,  cb_write_s32_stereo }},
    };
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
const char* wav_error_string        /* Get error string by error code */
    (
    t_wav_error code                /* Error code */
    )
    {
    return (code < ARRAYLEN(f_wav_error_string))
        ? f_wav_error_string[code] : "Unknown error";
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
t_wav_error wav_load_file           /* Allocate memory, read and resample a WAV file */
    (
    const char* file_name,          /* Name of WAV file to load */
    t_wav_chan_mode target_mode,    /* Output mode, mono/mono-as-stereo/stero */
    unsigned int target_bits,       /* Output bits per sample (16..32) */
    unsigned int target_rate,       /* Output sampling rate to convert to, Hz */
    void** p_data_buf,              /* (out) Buffer to allocate for output data */
    size_t* p_data_size             /* (out) Number of bytes loaded */
    )
    {
    int fd;
    t_wav_riff_header riff;
    t_wav_fmt_header fmt;
    t_wav_fmt_header_ex fmt_ex;
    t_wav_chunk_header chunk;
    t_rsmpl_state rs;
    t_wav_rsmpl rd, wd;
    unsigned long samples_in, samples_out, do_channels, bytes_left, fpos;
    unsigned int rd_row, rd_col, wr_row, wr_col;

    if (!file_name || !p_data_buf || !p_data_size)
        return WAV_ERR_BADPARAM;
    if (!target_rate)
        return WAV_ERR_WRONG_SAMPLING_RATE_OUT;

    for (wr_row = 0; wr_row < ARRAYLEN(f_cb_write); wr_row++)
        {
        if (target_bits == f_cb_write[wr_row].bits)
            break;
        }
    if (wr_row >= ARRAYLEN(f_cb_write))
        return WAV_ERR_WRONG_BITS_PER_SAMPLE_OUT;

    *p_data_size = 0;
    *p_data_buf = NULL;

    /* Open file */
    fd = open(file_name, O_RDONLY | O_BINARY);
    if (fd < 0)
        return WAV_ERR_FILE_NOT_FOUND;

    /* Read RIFF header */
    if (read(fd, &riff, sizeof(riff)) != sizeof(riff))
        {
        close(fd);
        return WAV_ERR_FILE_READ_ERROR;
        }

    /* Check that it's indeed a WAV file */
    if (strncmp(riff.chunk_id, "RIFF", 4) || strncmp(riff.type_id, "WAVE", 4))
        {
        close(fd);
        return WAV_ERR_WRONG_FILE_FORMAT;
        }
    
    /* We can ignore riff.chunk_size */

    /* Read next chunk header, it should be the format header */
    if (read(fd, &fmt, sizeof(fmt)) != sizeof(fmt))
        {
        close(fd);
        return WAV_ERR_FILE_READ_ERROR;
        }
    
    /* Check the format header */
    if (strncmp(fmt.chunk_id, "fmt ", 4))
        {
        /* This implementation requires that fmt chunk come first */
        close(fd);
        return WAV_ERR_CHUNKS_OUT_OF_ORDER;
        }

    switch (fmt.compression)
        {
        case WAV_COMPRESSION_PCM:
            break;
        case WAV_COMPRESSION_EXTENSIBLE:
            /* Wave extensible: this may be used for 24-bit data */
            if (read(fd, &fmt_ex, sizeof(fmt_ex)) != sizeof(fmt_ex))
                {
                close(fd);
                return WAV_ERR_FILE_READ_ERROR;
                }
            if ((fmt_ex.size < 0x16) 
                || memcmp(fmt_ex.subformat_guid, wav_extpcm_guid, sizeof(wav_extpcm_guid)))
                {
                close(fd);
                return WAV_ERR_UNSUPPORTED_COMPRESSION;
                }
            break;
        default:
            close(fd);
            return WAV_ERR_UNSUPPORTED_COMPRESSION;
        }

    if ((fmt.num_channels < 1) || (fmt.num_channels > 2))
        {
        close(fd);
        return WAV_ERR_WRONG_NUM_CHANNELS_IN;
        }

    for (rd_row = 0; rd_row < ARRAYLEN(f_cb_read); rd_row++)
        {
        if (fmt.bits_per_sample == f_cb_read[rd_row].bits)
            break;
        }
    if (rd_row >= ARRAYLEN(f_cb_read))
        {
        close(fd);
        return WAV_ERR_WRONG_BITS_PER_SAMPLE_IN;
        }

    if (!fmt.sample_rate)
        {
        close(fd);
        return WAV_ERR_WRONG_SAMPLING_RATE_IN;
        }

    /* Seek to the end of fmt chunk */
    fpos = lseek(fd, sizeof(riff) + fmt.chunk_size + 8, SEEK_SET);

    /* Search for data chunk */
    while (fpos < riff.chunk_size + 8)
        {
        /* Read next chunk header */
        if (read(fd, &chunk, sizeof(chunk)) != sizeof(chunk))
            {
            close(fd);
            return WAV_ERR_FILE_READ_ERROR;
            }

        if (!strncmp(chunk.chunk_id, "data", 4))
            break;

        /* Seek to the end of chunk */
        lseek(fd, chunk.chunk_size, SEEK_CUR);
        fpos += sizeof(chunk) + chunk.chunk_size;
        }

    if (fpos >= riff.chunk_size + 8)
        {
        close(fd);
        return WAV_ERR_NO_DATA;
        }

    /* WAV file data is chunk.chunk_size long, starting at the current file position */
    rd.frame_size = fmt.num_channels * f_cb_read[rd_row].container_size;
    samples_in = chunk.chunk_size / rd.frame_size;
    if (!samples_in)
        {
        close(fd);
        return WAV_ERR_NO_DATA;
        }

    /* Output frame: 1 channel if output mode is WAV_MONO, else 2 channels */
    wd.frame_size = f_cb_write[wr_row].container_size * (1 + (target_mode != WAV_MONO));
    samples_out = rsmpl_estimate_len(samples_in, fmt.sample_rate, target_rate);

    if (!samples_out || (samples_out > (size_t)(-1) / wd.frame_size))
        { /* samples_out so large that output data size won't fit into size_t */
        close(fd);
        return WAV_ERR_NOT_ENOUGH_MEMORY;
        }

    /* Determine resampling parameters:
     * 1) resample 2 channels only when input is stereo and output is stereo
     * 2) reader is stereo if resampling 2ch, mono for mono input, else "streo-to-mono sum"
     * 3) writer is stereo if resampling 2ch, mono for WAV_MONO output, else "mono-to-stereo copy"
     */
    do_channels = 1 + ((2 == fmt.num_channels) && (WAV_STEREO == target_mode));
    if (2 == do_channels)
        { /* Resample 2 channels, reader and writer are both stereo */
        rd_col = wr_col = 2; 
        }
    else
        {
        rd_col = (2 == fmt.num_channels);
        wr_col = (WAV_MONO != target_mode);
        }

    /* Initialize resampling */
    if (!rsmpl_prepare(&rs, f_cb_read[rd_row].callback[rd_col], &rd,
                            f_cb_write[wr_row].callback[wr_col], &wd,
                            do_channels, fmt.sample_rate, target_rate))
        {
        close(fd);
        return WAV_ERR_RESAMPLING_FAILED;
        }

    /* Allocate memory for input (temporary) and output buffers */
    wd.offset = 0;
    wd.error = 0;
    wd.size = samples_out * wd.frame_size;
    wd.buf = malloc(wd.size);

    if (!wd.buf)
        {
        close(fd);
        return WAV_ERR_NOT_ENOUGH_MEMORY;
        }

    rd.size = WAV_FILEREAD_SIZE - WAV_FILEREAD_SIZE % rd.frame_size;
    rd.buf = malloc(rd.size);

    if (!rd.buf)
        {
        free(wd.buf);
        close(fd);
        return WAV_ERR_NOT_ENOUGH_MEMORY;
        }

    /* Read and resample data */
    bytes_left = samples_in * rd.frame_size;
    while (bytes_left && !wd.error)
        {
        if (bytes_left < rd.size)
            rd.size = bytes_left;
        if (read(fd, rd.buf, rd.size) != (long)rd.size)
            {
            free(rd.buf);
            free(wd.buf);
            close(fd);
            return WAV_ERR_FILE_READ_ERROR;
            }
        rd.offset = 0;
        rsmpl_process(&rs);
        bytes_left -= rd.size;
        }
    
    close(fd);
    rsmpl_finish(&rs);
    free(rd.buf);

    if (wd.error)
        {
        free(wd.buf);
        return WAV_ERR_RESAMPLING_OVERFLOW;
        }
    else
        {
        *p_data_size = wd.offset;
        *p_data_buf = wd.buf;
        return WAV_ERR_SUCCESS;
        }
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
