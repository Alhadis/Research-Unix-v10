/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/*     T. Thompson - ATT-BL HO - portable versions     */
/*     C. Macey - ATT-DSG LZ - portable versions       */

/* Specify the environment:  CPU, OS, graphics, etc.
   This is an attempt to maintain a single master source tailorable at compile
   time, while encapsulating as many as possible of the required #if's and
   #ifdef's in this one file.
   OS-dependent #includes are triggered by these prior statements:
	#define LIBC_INCL 1		-- strcpy(), etc
	#define VALUES_INCL 1		-- math constants (PI etc)
	#define FILE_TREE_INCL 1	-- UNIX file-tree walking
   */

/* CPUs with some support: */
#define VAX 0
#define CRAY 1
#define SUN 2		/* including SPARC */
#define ATT3B 3
#define I386 4
#define MIPS 5		/* including SGI */

/*** Select CPU here: */
#define CPU VAX


/* Operating systems with some support: */
#define V9_OS 1		/* ATT-BL research UNIX, 9th edition */
#define SV_OS 2		/* ATT UNIX System V */
#define SUN_OS	3	/* Sun OS 4.x */

/*** Select OS here: */
#define OS V9_OS


/* Graphics display environments with some support: */
#define NO_GRAPHICS 0	/* defeat all graphics */
#define Y_GRAPHICS 1	/* the Y graphics interface (used in 1127 for Metheus) */
#define SUN_GRAPHICS 2	/* Tim Thompson's Sun interface */
#define X_GRAPHICS 3    /* Mark Tuomenoksa's X interface */

/*** Select graphics environment here: */
#define GRAPHICS Y_GRAPHICS


/* OCR installation directory */
#ifndef OCRDIR
#define OCRDIR "/usr/ocr"
#endif

/*** Miscellaneous arcane options:  best left alone **/

#define FILE_TREE (1)		/* set to zero to defeat UNIX file tree code */


/*** END OF SELECTIONS ** The rest of this file merely implements their effects */

/* define byte ordering in CPU */

#if CPU == SUN || CPU == MIPS
#define BIG_ENDIAN 1
#endif

/* deal with compilers that don't like (void *) */

#if CPU == MIPS
#define VOID char
#else
#define VOID void
#endif

/* include libc functions defs, if necessary */

#if LIBC_INCL

#if OS == V9_OS || CPU == CRAY
#include <libc.h>
#else
#if OS == SV_OS || OS == SUN_OS || CPU == MIPS
#include <string.h>
#endif
#endif

#endif

/* include values */

#if VALUES_INCL
#include <values.h>
#endif

/* UNIX file-tree-walk includes */

#if FILE_TREE_INCL

#if CPU==VAX || CPU==CRAY || CPU==MIPS
#include <sys/types.h>
#include <sys/stat.h>
#include <ftw.h>
#else
#if CPU==SUN || CPU==I386
#include <sys/types.h>
#include <sys/stat.h>
#include "myftw.h"  /* T. Thompson's version */
#endif
#endif

#endif

#if CPU==SUN||CPU==ATT3B||CPU==MIPS||CPU==I386
#define sgn(V) (((V)>0)? 1: (((V)<0)? -1: 0))
#endif 

#if CPU==SUN||CPU==ATT3B||CPU==MIPS
/* These are all big-endian machines */
#define swapshortin(x) (((x)<<8)&0xff00)|(((x)>>8)&0xff)
#define swapintin(x) \
		((((x)<<24)&0xff000000)| \
		 (((x)<<8) &0x00ff0000)| \
		 (((x)>>8) &0x0000ff00)| \
		 (((x)>>24)&0x000000ff))
/* The order of bytes in an int */
#define INDEX0 3
#define INDEX1 2
#define INDEX2 1
#define INDEX3 0
/* disable fast trick for picking out a short from an int */
#define USESHIFT	/* cf. skewlib.c */
#else
#define swapshortin(x) (x)
#define swapintin(x) (x)
#define INDEX0 0
#define INDEX1 1
#define INDEX2 2
#define INDEX3 3
#endif

/* global max no. classes -- gradually being made dynamic everywhere */
#if CPU==SUN
#define MAX_CL 128
#else
#define MAX_CL 3200	/* adequate for JIS Level 1 */
#endif

/* include char types (is this really necessary?) */

#if CTYPE_INCL

#if CPU == SUN
#include <string.h>
#else
#include <ctype.h>
#endif

#endif

/* includes for atan2 special configurations */

#if ATAN_INCL

#if CPU == CRAY
#define use_fast_atan2 F
#include "CRAY.h"
#else
#define use_fast_atan2 T
#endif

#endif

/* regular expression includes (fix to use standard regexp?) */

#if REGEXP_INCL

#if OS == V9_OS
#include <regexp.h>
#else
#include "regexp.h"
#endif

#endif

/* Use of the graphics display */

#if CPU != CRAY
#define SHOW_GRAPHICS 1
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif
