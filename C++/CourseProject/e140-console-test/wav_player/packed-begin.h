/*================================================================================================*
 * Include this file before declaring packed structures.
 *================================================================================================*/

/*================================================================================================*/
#ifdef __GNUC__

#define struct          struct __attribute__ ((__packed__))

#elif defined(_MSC_VER)

#pragma warning(disable:4103)
#pragma pack(push, 1)

#else

#warning "Unknown compiler, assuming pargma pack() for packing"
#pragma pack(push, 1)

#endif
/*================================================================================================*/
