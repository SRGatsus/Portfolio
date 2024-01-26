/*================================================================================================*
 * WAV file loader with resampling
 *================================================================================================*/

#ifndef WAV_LOADER_H_
#define WAV_LOADER_H_

#include <stddef.h>

/*================================================================================================*/
typedef enum en_wav_error
    {
    WAV_ERR_SUCCESS = 0,
    WAV_ERR_BADPARAM,
    WAV_ERR_FILE_NOT_FOUND,
    WAV_ERR_FILE_READ_ERROR,
    WAV_ERR_WRONG_FILE_FORMAT,
    WAV_ERR_CHUNKS_OUT_OF_ORDER,
    WAV_ERR_UNSUPPORTED_COMPRESSION,
    WAV_ERR_NOT_ENOUGH_MEMORY,
    WAV_ERR_NO_DATA,
    WAV_ERR_WRONG_NUM_CHANNELS_IN,
    WAV_ERR_WRONG_BITS_PER_SAMPLE_IN,
    WAV_ERR_WRONG_BITS_PER_SAMPLE_OUT,
    WAV_ERR_WRONG_SAMPLING_RATE_IN,
    WAV_ERR_WRONG_SAMPLING_RATE_OUT,
    WAV_ERR_RESAMPLING_OVERFLOW,
    WAV_ERR_RESAMPLING_FAILED
    }
    t_wav_error;

typedef enum en_wav_chan_mode       /* Controls how many wave channels to load */
    {
    WAV_MONO,                       /* Mono */
    WAV_MONO_AS_STEREO,             /* Mono stored as stereo (2 channels with same data) */
    WAV_STEREO                      /* Stereo */
    }
    t_wav_chan_mode;
/*================================================================================================*/

/*================================================================================================*/
/*------------------------------------------------------------------------------------------------*/
const char* wav_error_string        /* Get error string by error code */
    (
    t_wav_error code                /* Error code */
    );
/*------------------------------------------------------------------------------------------------*/
t_wav_error wav_load_file           /* Allocate memory, read and resample a WAV file */
    (
    const char* file_name,          /* Name of WAV file to load */
    t_wav_chan_mode target_mode,    /* Output mode, mono/mono-as-stereo/stero */
    unsigned int target_bits,       /* Output bits per sample (16..32) */
    unsigned int target_rate,       /* Output sampling rate to convert to, Hz */
    void** p_data_buf,              /* (out) Buffer to allocate for output data */
    size_t* p_data_size             /* (out) Number of bytes loaded */
    );
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
#endif /* WAV_LOADER_H_ */
