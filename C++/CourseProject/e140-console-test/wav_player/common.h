/*================================================================================================*
 * Common includes and definitions for HDA, WAV-loader etc.
 *================================================================================================*/

#ifndef COMMON_H_
#define COMMON_H_

#ifdef _MSC_VER
    #include "pstdint.h"
    #ifndef __cplusplus
        typedef char bool;
        #define true    1
        #define false   0
    #endif
    #include <fcntl.h>  /* file i/o */
    #include <io.h>     /* file i/o */
    #include <stdio.h>  /* file i/o */
#else
    #include <stdbool.h>
    #include <stdint.h>
    #include <fcntl.h>  /* file i/o */
    #include <unistd.h> /* file i/o */
#endif

#include <stdlib.h>
#include <string.h>

/*================================================================================================*/
#define CONCAT_(a, b)       a ## b
#define CONCAT(a,b)         CONCAT_(a, b)
#define ARRAYLEN(array)     (sizeof(array) / sizeof(array[0]))
/*================================================================================================*/

#endif /* COMMON_H_ */
