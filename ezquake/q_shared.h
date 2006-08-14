/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

    $Id: q_shared.h,v 1.5 2006-08-14 15:31:50 vvd0 Exp $

*/
// q_shared.h -- functions shared by all subsystems

#ifndef __Q_SHARED_H__
#define __Q_SHARED_H__

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "mathlib.h"
#include "sys.h"

#ifdef _MSC_VER
#pragma warning( disable : 4244 4127 4201 4214 4514 4305 4115 4018)
#endif

#define QUAKE_GAME // as opposed to utilities

typedef unsigned char byte;

#undef true
#undef false
typedef enum {false, true} qbool;

#ifndef NULL
#define NULL ((void *) 0)
#endif


#ifdef _WIN32
#define IS_SLASH(c) ((c) == '/' || (c) == '\\')
#else
#define IS_SLASH(c) ((c) == '/')
#endif

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

//#define bound(a,b,c) (max((a), min((b), (c))))
#define bound(a,b,c) ((a) >= (c) ? (a) : \
					(b) < (a) ? (a) : (b) > (c) ? (c) : (b))

//============================================================================

#ifdef id386
#define UNALIGNED_OK		1		// set to 0 if unaligned accesses are not supported
#else
#define UNALIGNED_OK		0
#endif

//============================================================================

#define	MINIMUM_MEMORY		0x550000

#define	MAX_QPATH			64		// max length of a quake game pathname
#define	MAX_OSPATH			128		// max length of a filesystem pathname

#define	ON_EPSILON			0.1		// point on plane side epsilon

//============================================================================

typedef struct sizebuf_s {
	qbool	allowoverflow;	// if false, do a Sys_Error
	qbool	overflowed;		// set to true if the buffer size failed
	byte	*data;
	int		maxsize;
	int		cursize;
} sizebuf_t;

char *com_args_original;

void SZ_Init (sizebuf_t *buf, byte *data, int length);
void SZ_Clear (sizebuf_t *buf);
void *SZ_GetSpace (sizebuf_t *buf, int length);
void SZ_Write (sizebuf_t *buf, void *data, int length);
void SZ_Print (sizebuf_t *buf, char *data);	// strcats onto the sizebuf

//============================================================================

short	ShortSwap (short l);
int		LongSwap (int l);
float	FloatSwap (float f);
int		LongSwapPDP2Big (int l);
int		LongSwapPDP2Lit (int l);
float	FloatSwapPDP2Big (float f);
float	FloatSwapPDP2Lit (float f);

#ifdef __BIG_ENDIAN__Q__
#define BigShort(x)		(x)
#define BigLong(x)		(x)
#define BigFloat(x)		(x)
#define LittleShort(x)	ShortSwap(x)
#define LittleLong(x)	LongSwap(x)
#define LittleFloat(x)	FloatSwap(x)
#elif defined(__LITTLE_ENDIAN__Q__)
#define BigShort(x)		ShortSwap(x)
#define BigLong(x)		LongSwap(x)
#define BigFloat(x)		FloatSwap(x)
#define LittleShort(x)	(x)
#define LittleLong(x)	(x)
#define LittleFloat(x)	(x)
#elif defined(__PDP_ENDIAN__Q__)
#define BigShort(x)		ShortSwap(x)
#define BigLong(x)		LongSwapPDP2Big(x)
#define BigFloat(x)		FloatSwapPDP2Big(x)
#define LittleShort(x)	(x)
#define LittleLong(x)	LongSwapPDP2Lit(x)
#define LittleFloat(x)	FloatSwapPDP2Lit(x)
#else
#error Unknown byte order type!
#endif

/*
unsigned int BuffBigLong (const unsigned char *buffer);
unsigned short BuffBigShort (const unsigned char *buffer);
unsigned int BuffLittleLong (const unsigned char *buffer);
unsigned short BuffLittleShort (const unsigned char *buffer);
*/
#define	BuffLittleLong(buffer)	LittleLong(*(int*)buffer)
#define	BuffLittleShort(buffer)	LittleShort(*(int*)buffer)

//============================================================================

int Q_atoi (const char *str);
float Q_atof (const char *str);
char *Q_ftos (float value); // removes trailing zero chars

// Added by VVD {
#ifdef _WIN32
#define strcasecmp(s1, s2)	_stricmp  ((s1),   (s2))
#define strncasecmp(s1, s2, n)	_strnicmp ((s1),   (s2),   (n))
int snprintf(char *str, size_t n, char const *fmt, ...);
int vsnprintf(char *buffer, size_t count, const char *format, va_list argptr);
#endif

#if defined(__linux__) || defined(_WIN32)
size_t strlcpy (char *dst, char *src, size_t siz);
size_t strlcat (char *dst, char *src, size_t siz);
char  *strnstr (char *s, char *find, size_t slen);
#endif
// Added by VVD }

qbool Q_glob_match (const char *pattern, const char *text);

int Com_HashKey (const char *name);

//============================================================================

// memory management
void *Q_malloc (size_t size);
void *Q_calloc (size_t n, size_t size);
char *Q_strdup (const char *src);
// might be turned into a function that makes sure all Q_*alloc calls are matched with Q_free
#define Q_free(ptr) free(ptr)

//============================================================================

#define	MAX_MSGLEN			1450		// max length of a reliable message
#define	MAX_DATAGRAM		1450		// max length of unreliable message
#define	MSG_BUF_SIZE		8192		// max length of msg buf; MVD demo need it
#define	FILE_TRANSFER_BUF_SIZE	MAX_MSGLEN - 100

#endif /* __Q_SHARED_H__ */
