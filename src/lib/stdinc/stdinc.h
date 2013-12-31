#ifndef __STD__JOYGAME__H__
#define __STD__JOYGAME__H__


#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdint.h>
#include <assert.h>

#include <limits>
#include <math.h>
/* --------------------------------------------------------------------------
 * Types
 * --------------------------------------------------------------------------
 */

/// This type is an unsigned character.
typedef unsigned char	uchar;
/// This type is an unsigned short.
typedef unsigned short	ushort;
/// This type is an unsigned integer.
typedef unsigned int	uint;
/// This type is an unsigned longer.
typedef unsigned long	ulong;

/// This type is an integer with a size of 8 bits.
typedef int8_t				int8;
/// This type is an unsigned integer with a size of 8 bits.
typedef uint8_t				uint8;

/// This type is an integer with a size of 16 bits.
typedef int16_t				int16;
/// This type is an unsigned integer with a size of 16 bits.
typedef uint16_t			uint16;

/// This type is an integer with a size of 32 bits.
typedef int32_t				int32;
/// This type is an unsigned integer with a size of 32 bits.
typedef uint32_t			uint32;
/// This type is an integer with a size of 64 bits.
typedef int64_t				int64;
/// This type is an unsigned integer with a size of 64 bits.
typedef uint64_t			uint64;

#ifdef _LP64
typedef int64				intptr;
typedef uint64				uintptr;
#define PRI64 "ld"
#define PRIu64 "lu"
#define PRIx64 "lx"
#define PRIX64 "lX"
#else
typedef int32				intptr;
typedef uint32				uintptr;
#define PRI64 "lld"
#define PRIu64 "llu"
#define PRIx64 "llx"
#define PRIX64 "llX"
#endif

#ifndef PRIzd
#define PRIzd "zd"
#endif

#ifndef PRIzu
#define PRIzu "zu"
#endif



//////////////////////////////
#define JG_ASSERT		assert

#define IF_NOT_JG_ASSERT_DEV(exp) \
 		if( !(exp) )
 			

#endif	//__STD__JOYGAME__H__