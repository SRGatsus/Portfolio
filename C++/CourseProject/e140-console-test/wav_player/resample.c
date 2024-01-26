/*================================================================================================*
 * Sampling rate converter (using 4-point 3rd order Catmull-Rom spline)
 *================================================================================================*/

#include "resample.h"
#include <limits.h>
#include <assert.h>

/*================================================================================================*/
#if RESAMPLE_USE_FIXED_POINT

/* fixed-point math */
#define FMUL_MACRO  0
#define Q_FACTOR    RESAMPLE_INT_BITS
#define FCONST_1    (1L << Q_FACTOR)
#define FCONST(x)   ((long)((x) * (double)(1L << Q_FACTOR)))
#if FMUL_MACRO
#define FMUL(x ,y)  (((long long)(x) * (y) + (1L << (Q_FACTOR - 1))) >> Q_FACTOR)
#else
#define FMUL(x, y)  f_mul((x), (y))
#endif

#else   

/* floating-point math */
#define FCONST_1    (1.0)
#define FCONST(x)   (x)
#define FMUL(x, y)  ((x) * (y))

#endif

/* Saturation (both fixed and float) */
#define FSAT(x)     (((x) < RESAMPLE_SAMPLE_MIN) ? RESAMPLE_SAMPLE_MIN \
                    : ((x) > RESAMPLE_SAMPLE_MAX) ? RESAMPLE_SAMPLE_MAX : (x))
/*================================================================================================*/

/*================================================================================================*/
#if RESAMPLE_USE_FIXED_POINT
#if !FMUL_MACRO
/*------------------------------------------------------------------------------------------------*/
static long f_mul(long x, long y)
    {
    return (long)(((long long)x * y + (1L << (Q_FACTOR - 1))) >> Q_FACTOR);
    }
/*------------------------------------------------------------------------------------------------*/
#endif
#endif

/*------------------------------------------------------------------------------------------------*/
static t_rsmpl_sample f_interpolate /* Elementary interpolation step */
    (
    const t_rsmpl_sample* buf,      /* delay line buffer */
    t_rsmpl_sample t                /* fractional time, t = [0,1) */
    )
    {
    t_rsmpl_sample c0, c1, c2, c3;
    c1 = buf[2];                            /* c1 = x2 (temp.) */
    c0 = 2 * buf[1];                        /* c0 = 2x1 */
    c3 = buf[3] + buf[1] + c0 - 4 * c1;     /* c3 = 3x1 - 4x2 + x3 (temp.) */
    c1 -= buf[0];                           /* c1 = x2 - x0 */
    c2 = 2 * buf[0] - c0 - c3;              /* c2 = 2x0 - 5x1 + 4x2 - x3 */
    c3 += c1;                               /* c3 = 3x1 - 3x2 + x3 - x0 */
    /* c0 = raw output */
    c0 = (c0 + FMUL(t, c1 + FMUL(t, c2 + FMUL(t, c3)))) / 2;
    /* saturate */
    return FSAT(c0);
    }
/*------------------------------------------------------------------------------------------------*/

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
    )                               /* returns 1 if successfull, 0 on error */
    {
    assert(sizeof(long) >= 4);
    assert(sizeof(long long) >= 8);
    if (!state) return 0;
    if (!num_channels || (num_channels > RESAMPLE_MAX_CHANNELS)
        || !reader_func || !writer_func
        || !in_rate || !out_rate
        || (in_rate / out_rate > 100)
        || (out_rate / in_rate > 100)
        )
        {
        state->num_channels = 0; /* Invalidate structure */
        return 0;
        }
    state->num_channels = num_channels;
    state->get_frame = reader_func;
    state->put_frame = writer_func;
    state->get_frame_param = reader_param;
    state->put_frame_param = writer_param;
#if RESAMPLE_USE_FIXED_POINT
    state->resampling_factor = (long)(((long long)in_rate << Q_FACTOR) / out_rate);
#else
    state->resampling_factor = (double)in_rate / out_rate;
#endif
    state->start_count = 0;
    state->time_to_next = FCONST_1; /* Not counting 2 initial samples (counter by start_count) */
    return 1;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
unsigned long rsmpl_estimate_len    /* Estimate number of output samples */
    (
    unsigned long in_samples,
    unsigned long in_rate,          /* Input sampling rate, Hz */
    unsigned long out_rate          /* Output sampling rate, Hz */
    )                               /* Returns 0 on overflow */
    {
    unsigned long long out_samples;
    if (!in_samples || !in_rate || !out_rate)
        return 0;
    out_samples = 2 + ((unsigned long long)(in_samples - 1) * out_rate + in_rate - 1) / in_rate;
    if (out_samples > ULONG_MAX)
        return 0;
    return (unsigned long)out_samples;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void rsmpl_process                  /* Process data */
    (
    t_rsmpl_state* state            /* Control structure */
    )
    {
    t_rsmpl_sample frame[RESAMPLE_MAX_CHANNELS];

    if (!state || !state->num_channels) return;

    while (state->get_frame(state->get_frame_param, frame))
        {
        unsigned int chan;
        if (state->start_count < 2)
            { /* First two samples handled specially:
               * delay buf = { garbage, 2x[0] - x[1], x[0], x[1] }
               * Then x[2] can be put normally at the end, shifting out the garbage and producing
               * the first portion of real output corresponding to the interval x[0]...x[1].
               */ 
            for (chan = 0; chan < state->num_channels; chan++)
                {
                t_rsmpl_sample* buf = &state->delay_buf[chan][0];
                buf[2 + state->start_count] = frame[chan];
                if (1 == state->start_count)
                    buf[1] = buf[2] + buf[2] - frame[chan];
                }
            state->start_count++;
            }
        else
            {
            t_rsmpl_sample t = state->time_to_next - FCONST_1;

            /* Shift delay line */
            for (chan = 0; chan < state->num_channels; chan++)
                {
                t_rsmpl_sample* buf = &state->delay_buf[chan][0];
                /* Plain copy (may be actually faster on 4 points than indexed circular buffer) */
                buf[0] = buf[1];
                buf[1] = buf[2];
                buf[2] = buf[3];
                buf[3] = frame[chan];
                }

            /* Do interpolation */
            while (t < FCONST_1)
                {
                for (chan = 0; chan < state->num_channels; chan++)
                    {
                    frame[chan] = f_interpolate(&state->delay_buf[chan][0], t);
                    }
                t += state->resampling_factor;
                /* Output frame */
                state->put_frame(state->put_frame_param, frame);
                }

            state->time_to_next = t;
            }
        } /* while get_frame */
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void rsmpl_finish                   /* Finish processing and output the last part */
    (
    t_rsmpl_state* state            /* Control structure */
    )
    {
    if (!state || !state->num_channels) return;

    if (state->start_count >= 2)
        {
        unsigned int chan;
        t_rsmpl_sample frame[RESAMPLE_MAX_CHANNELS];

        t_rsmpl_sample t = state->time_to_next - FCONST_1;

        /* Shift delay line */
        for (chan = 0; chan < state->num_channels; chan++)
            {
            t_rsmpl_sample* buf = &state->delay_buf[chan][0];
            /* Plain copy (may be actually faster on 4 points than indexed circular buffer) */
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            /* Compute last virtual sample */
            buf[3] = buf[2] + buf[2] - buf[1];
            }

        /* Do interpolation */
        while (t <= FCONST_1)    /* "<=" here gives a more accurate ending */
            {
            for (chan = 0; chan < state->num_channels; chan++)
                {
                frame[chan] = f_interpolate(&state->delay_buf[chan][0], t);
                }
            t += state->resampling_factor;
            /* Output frame */
            state->put_frame(state->put_frame_param, frame);
            }
        }
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
