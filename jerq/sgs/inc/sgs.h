/*
 * static char *ID_sgsh = "@(#) sgs.h: 1.13 1/25/84";
 */

#define	SGS	""

/*	The symbol FBOMAGIC is defined in filehdr.h	*/

#define MAGIC	FBOMAGIC
#define TVMAGIC (MAGIC+1)

#define ISMAGIC(x)	(x ==  MAGIC)


#ifdef ARTYPE
#define	ISARCHIVE(x)	( x ==  ARTYPE)
#define BADMAGIC(x)	((((x) >> 8) < 7) && !ISMAGIC(x) && !ISARCHIVE(x))
#endif


/*
 *	When a UNIX aout header is to be built in the optional header,
 *	the following magic numbers can appear in that header:
 *
 *		AOUT1MAGIC : default
 *		PAGEMAGIC  : configured for paging
 */

#define AOUT1MAGIC 0407
#define AOUT2MAGIC 0410
#define PAGEMAGIC  0413



/*
 * MAXINT defines the maximum integer for printf.c
 */

#define MAXINT	2147483647

#define	SGSNAME	""
#define	RELEASE	"System V Release 3P.0.0.0 1/25/84"
