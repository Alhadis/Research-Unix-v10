/*
 *  static char ID[] = "@(#) params.h: 1.8 10/31/83";
 */
/***********************************************************************
*   This file contains system dependent parameters for the link editor.
*   There must be a different params.h file for each version of the
*   link editor (e.g. b16, n3b, mac80, etc.)
*
*   THIS COPY IS FOR M80
*********************************************************************/


/*
 * Maximum size of a section
 */
#define MAXSCNSIZE	0x20000000L
#define MAXSCNSZ	MAXSCNSIZE


/*
 * Default size of configured memory
 */
#define MEMORG		0x0L
#define MEMSIZE		0xffffffffL
#define NONULLORG	0x20000L

/*
 * Size of a region. If USEREGIONS is zero, the link editor will NOT
 * permit the use of REGIONS, nor partition the address space
 * USEREGIONS is defined in system.h
 */
#define REGSIZE 	0

#define COM_ALIGN	0x3L	/* Align factor for .comm's	*/

/*
 * define boundary for use by paging
 */

#define BOUNDARY	0x80000		/* 512K */

/*
 * define special symbol names
 */

#define _CSTART	"_start"
#define _MAIN	"main"
