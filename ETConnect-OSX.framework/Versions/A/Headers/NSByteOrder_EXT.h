/* NSByteOrder functions for GNUStep
   Copyright (C) 1998 Free Software Foundation, Inc.

   Written by:  Richard Frith-Macdonald <richard@brainstorm.co.uk>
   Date: 1998

   This file is part of the GNUstep Base Library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02111 USA.
 */

#ifndef __NSByteOrder_h_GNUSTEP_BASE_INCLUDE
#define __NSByteOrder_h_GNUSTEP_BASE_INCLUDE

#import "EIConnectInternal.h"

#if defined(__cplusplus)
extern "C" {
#endif
    
/*
 *	Size definitions for standard types
 */
#ifdef OBSOLETE /* -> platform dependent stuff, should not be used ... */
#define	GS_SIZEOF_SHORT		2
#define	GS_SIZEOF_INT		4
#define	GS_SIZEOF_LONG		4
#define	GS_SIZEOF_LONG_LONG	8
#define	GS_SIZEOF_FLOAT		4
#define	GS_SIZEOF_DOUBLE	8
#define	GS_SIZEOF_VOIDP		4
#endif //#ifdef OBSOLETE

/*
 *	Function prototypes.
 */

#if (__GNUC__ == 2) && (__GNUC_MINOR__ <= 6) && !defined(__attribute__)
#  define __attribute__(x)
#endif

/*
 *	First the GNUstep functions for byte swapping
 */
static inline uint16_t
GSSwapI16(uint16_t in) __attribute__((unused));
static inline uint32_t
GSSwapI32(uint32_t in) __attribute__((unused));
static inline uint64_t
GSSwapI64(uint64_t in) __attribute__((unused));
static inline gsu128
GSSwapI128(gsu128 in) __attribute__((unused));

static inline uint16_t
GSSwapBigI16ToHost(uint16_t in) __attribute__((unused));
static inline uint32_t
GSSwapBigI32ToHost(uint32_t in) __attribute__((unused));
static inline uint64_t
GSSwapBigI64ToHost(uint64_t in) __attribute__((unused));
static inline gsu128
GSSwapBigI128ToHost(gsu128 in) __attribute__((unused));

static inline uint16_t
GSSwapHostI16ToBig(uint16_t in) __attribute__((unused));
static inline uint32_t
GSSwapHostI32ToBig(uint32_t in) __attribute__((unused));
static inline uint64_t
GSSwapHostI64ToBig(uint64_t in) __attribute__((unused));
static inline gsu128
GSSwapHostI128ToBig(gsu128 in) __attribute__((unused));

static inline uint16_t
GSSwapLittleI16ToHost(uint16_t in) __attribute__((unused));
static inline uint32_t
GSSwapLittleI32ToHost(uint32_t in) __attribute__((unused));
static inline uint64_t
GSSwapLittleI64ToHost(uint64_t in) __attribute__((unused));
static inline gsu128
GSSwapLittleI128ToHost(gsu128 in) __attribute__((unused));

static inline uint16_t
GSSwapHostI16ToLittle(uint16_t in) __attribute__((unused));
static inline uint32_t
GSSwapHostI32ToLittle(uint32_t in) __attribute__((unused));
static inline uint64_t
GSSwapHostI64ToLittle(uint64_t in) __attribute__((unused));
static inline gsu128
GSSwapHostI128ToLittle(gsu128 in) __attribute__((unused));

#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 1)
/* gcc 3.1 with option -O2 generates bad (i386?) code when compiling
   the following inline functions inside a .m file.  A call to a
   dumb function seems to work. */
extern void _gcc3_1_hack(void);
#endif

static inline uint16_t
GSSwapI16(uint16_t in)
{
    union swap {
        uint16_t num;
        uint8_t byt[2];
    } dst;
    union swap    *src = (union swap*)&in;
#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 1)
    _gcc3_1_hack();
#endif
    dst.byt[0] = src->byt[1];
    dst.byt[1] = src->byt[0];
    return dst.num;
}

static inline uint32_t
GSSwapI32(uint32_t in)
{
    union swap {
        uint32_t num;
        uint8_t byt[4];
    } dst;
    union swap    *src = (union swap*)&in;
#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 1)
    _gcc3_1_hack();
#endif
    dst.byt[0] = src->byt[3];
    dst.byt[1] = src->byt[2];
    dst.byt[2] = src->byt[1];
    dst.byt[3] = src->byt[0];
    return dst.num;
}

static inline uint64_t
GSSwapI64(uint64_t in)
{
    union swap {
        uint64_t num;
        uint8_t byt[8];
    } dst;
    union swap    *src = (union swap*)&in;
#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 1)
    _gcc3_1_hack();
#endif
    dst.byt[0] = src->byt[7];
    dst.byt[1] = src->byt[6];
    dst.byt[2] = src->byt[5];
    dst.byt[3] = src->byt[4];
    dst.byt[4] = src->byt[3];
    dst.byt[5] = src->byt[2];
    dst.byt[6] = src->byt[1];
    dst.byt[7] = src->byt[0];
    return dst.num;
}

static inline gsu128
GSSwapI128(gsu128 in)
{
    union swap {
        gsu128 num;
        uint8_t byt[16];
    } dst;
    union swap    *src = (union swap*)&in;
#if (__GNUC__ == 3) && (__GNUC_MINOR__ == 1)
    _gcc3_1_hack();
#endif
    dst.byt[0] = src->byt[15];
    dst.byt[1] = src->byt[14];
    dst.byt[2] = src->byt[13];
    dst.byt[3] = src->byt[12];
    dst.byt[4] = src->byt[11];
    dst.byt[5] = src->byt[10];
    dst.byt[6] = src->byt[9];
    dst.byt[7] = src->byt[8];
    dst.byt[8] = src->byt[7];
    dst.byt[9] = src->byt[6];
    dst.byt[10] = src->byt[5];
    dst.byt[11] = src->byt[4];
    dst.byt[12] = src->byt[3];
    dst.byt[13] = src->byt[2];
    dst.byt[14] = src->byt[1];
    dst.byt[15] = src->byt[0];
    return dst.num;
}

/*
 *	Swap Big endian to host
 */
static inline uint16_t
GSSwapBigI16ToHost(uint16_t in)
{
    return GSSwapI16(in);
}
static inline uint32_t
GSSwapBigI32ToHost(uint32_t in)
{
    return GSSwapI32(in);
}
static inline uint64_t
GSSwapBigI64ToHost(uint64_t in)
{
    return GSSwapI64(in);
}
static inline gsu128
GSSwapBigI128ToHost(gsu128 in)
{
    return GSSwapI128(in);
}

/*
 *	Swap Host to Big endian
 */
static inline uint16_t
GSSwapHostI16ToBig(uint16_t in)
{
    return GSSwapI16(in);
}
static inline uint32_t
GSSwapHostI32ToBig(uint32_t in)
{
    return GSSwapI32(in);
}
static inline uint64_t
GSSwapHostI64ToBig(uint64_t in)
{
    return GSSwapI64(in);
}
static inline gsu128
GSSwapHostI128ToBig(gsu128 in)
{
    return GSSwapI128(in);
}

/*
 *	Swap Little endian to Host
 */
static inline uint16_t
GSSwapLittleI16ToHost(uint16_t in)
{
    return in;
}
static inline uint32_t
GSSwapLittleI32ToHost(uint32_t in)
{
    return in;
}
static inline uint64_t
GSSwapLittleI64ToHost(uint64_t in)
{
    return in;
}
static inline gsu128
GSSwapLittleI128ToHost(gsu128 in)
{
    return in;
}

/*
 *	Swap Host to Little endian
 */
static inline uint16_t
GSSwapHostI16ToLittle(uint16_t in)
{
    return in;
}
static inline uint32_t
GSSwapHostI32ToLittle(uint32_t in)
{
    return in;
}
static inline uint64_t
GSSwapHostI64ToLittle(uint64_t in)
{
    return in;
}
static inline gsu128
GSSwapHostI128ToLittle(gsu128 in)
{
    return in;
}

#if defined(__cplusplus)
}
#endif

#endif /* __NSByteOrder_h_GNUSTEP_BASE_INCLUDE */
