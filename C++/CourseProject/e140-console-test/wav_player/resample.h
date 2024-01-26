/*================================================================================================*
 * Sampling rate converter (using 4-point 3rd order Catmull-Rom spline)
 *================================================================================================*/

#ifndef RESAMPLE_H_
#define RESAMPLE_H_

/*================================================================================================*/
/* 1 = use fixed point, 0 = use floating point */
#define RESAMPLE_USE_FIXED_POINT        1

/* Maximum number of channels in a stream */
#define RESAMPLE_MAX_CHANNELS           2

#if RESAMPLE_USE_FIXED_POINT

/* FIXED POINT MODE:
 * Internal sample representation is a signed long taking values from
 * RESAMPLE_SAMPLE_MIN to RESAMPLE_SAMPLE_MAX.
 * Effectively its lower RESAMPLE_INT_BITS bits contain the sample,
 * and higher bits are duplicates of the sign bit.
 */
typedef long t_rsmpl_sample;
#define RESAMPLE_INT_BITS               24
#define RESAMPLE_SAMPLE_MIN             (-1L << (RESAMPLE_INT_BITS - 1))
#define RESAMPLE_SAMPLE_MAX             ((1L << (RESAMPLE_INT_BITS - 1)) - 1)
#define RESAMPLE_FROM_INT(bits, x)      (((bits) <= RESAMPLE_INT_BITS) \
                                        ? ((long)(x) << (RESAMPLE_INT_BITS - (bits))) \
                                        : ((long)(x) >> ((bits) - RESAMPLE_INT_BITS)))
#define RESAMPLE_TO_INT(bits, x)        (((bits) <= RESAMPLE_INT_BITS) \
                                        ? ((x) >> (RESAMPLE_INT_BITS - (bits))) \
                                        : ((x) << ((bits) - RESAMPLE_INT_BITS)))

#else

/* FLOATING POINT MODE:
 * Internal sample representation is a double in the range [-1.0, 1.0].
 */
typedef double t_rsmpl_sample;
#define RESAMPLE_SAMPLE_MIN             -1.0
#define RESAMPLE_SAMPLE_MAX             1.0
#define RESAMPLE_FROM_INT(bits, x)      ((double)(x) * (1.0 / (1L << ((bits) - 1))))
#define RESAMPLE_TO_INT(bits, x)        (((x) * (1L << ((bits) - 1)) < (1L << ((bits) - 1)) - 1) \
                                        ? (x) * (1L << ((bits) - 1)) \
                                        : (1L << ((bits) - 1)) - 1 )
#endif
/*================================================================================================*/

/*================================================================================================*/
/* Function pointers to convert from/to any sample format to internal format.
 * Each conversion uses two functions: an input reader and an output writer.
 * The number of channels in the frame is assumed to be known to the callback.
 */
typedef int (*t_rsmpl_accessor)
    (
    void* user_param,                   /* User-defined pointer, usually a control structure */
    t_rsmpl_sample* frame               /* Sample frame, up to RESAMPLE_MAX_CHANNELS samples */
    );                                  /* Returns 1 if data has been read/written, else 0 */

typedef struct
    {
    t_rsmpl_sample delay_buf[RESAMPLE_MAX_CHANNELS][4]; /* Last 4 input samples per channel */
    t_rsmpl_accessor get_frame;         /* Callback function to read input samples */
    t_rsmpl_accessor put_frame;         /* Callback function to write output samples */
    void* get_frame_param;              /* User-defined parameter for reader callback */
    void* put_frame_param;              /* User-defined parameter for writer callback */
    t_rsmpl_sample resampling_factor;   /* Resampling factor = in_rate / out_rate */
    t_rsmpl_sample time_to_next;        /* Input sample periods to next output sample */
    unsigned int num_channels;          /* Number of channels */
    unsigned int start_count;           /* Start-up sample counter (0 to 2) */
    }
    t_rsmpl_state;
/*================================================================================================*/

/*================================================================================================*/
/*------------------------------------------------------------------------------------------------*/
int rsmpl_prepare                   /* Initialize resampling process */
    (
    t_rsmpl_state* state,           /* Control structure */
    t_rsmpl_accessor reader_func,   /* Reader of input frames */
    void* reader_param,             /* User-define parameter for reader_func */
    t_rsmpl_accessor writer_func,   /* Writer of output frames */
    void* writer_param,             /* User-define parameter for writer_func */
    unsigned int num_channels,      /* Number of channels in the stream */
    unsigned long in_rate,          /* Input sampling rate, Hz */
    unsigned long out_rate          /* Output sampling rate, Hz */
    );                              /* returns 1 if successfull, 0 on error */
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
unsigned long rsmpl_estimate_len    /* Estimate number of output samples */
    (
    unsigned long in_samples,
    unsigned long in_rate,          /* Input sampling rate, Hz */
    unsigned long out_rate          /* Output sampling rate, Hz */
    );                              /* Returns 0 on overflow */
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void rsmpl_process                  /* Process data */
    (
    t_rsmpl_state* state            /* Control structure */
    );
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void rsmpl_finish                   /* Finish processing and output the last part */
    (
    t_rsmpl_state* state            /* Control structure */
    );
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/

#endif /* RESAMPLE_H_ */
