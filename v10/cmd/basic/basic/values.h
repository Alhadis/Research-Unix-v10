/*	@(#)values.h	1.6	*/
/* These values work with any binary representation of integers
 * where the high-order bit contains the sign */

/* A number used normally for size of a shift */
#if gcos
#define BITSPERBYTE	9
#else
#define BITSPERBYTE	8
#endif

/* Short, regular and long ints with only the high-order bit turned on */
#define HIBITS	((short)(1 << (BITSPERBYTE * sizeof(short) - 1)))
#define HIBITI	(1 << (BITSPERBYTE * sizeof(int) - 1))
#define HIBITL	(1L << (BITSPERBYTE * sizeof(long) - 1))

/* Largest short, regular and long int */
#define MAXSHORT	((short)(~HIBITS))
#define MAXINT	(~HIBITI)
#define MAXLONG	(~HIBITL)

/* Various values which describe the binary floating-point representation
 * _EXPBASE	- The exponent base
 * DMAXEXP 	- The maximum exponent of a double
 * FMAXEXP 	- The maximum exponent of a float
 * DMINEXP 	- The minimum exponent of a double
 * FMINEXP 	- The minimum exponent of a float
 * MAXDOUBLE	- The largest double ((_EXPBASE ** (DMAXEXP + 1)) - 1)
 * MAXFLOAT	- The largest float ((_EXPBASE ** (FMAXEXP + 1)) - 1)
 * DSIGNIF	- The number of significant bits in a double
 * FSIGNIF	- The number of significant bits in a float
 * DMAXPOWTWO	- The largest power of two exactly representable as a double
 * FMAXPOWTWO	- The largest power of two exactly representable as a float
 * _DEXPLEN	- The number of bits for the exponent of a double
 * _FEXPLEN	- The number of bits for the exponent of a float
 * _HIDDENBIT	- 1 if high-significance bit of mantissa is implicit
 */
#if u3b
/* this papers around a bug in frexp.c which should be fixed */
/* #define MAXDOUBLE	1.79769313486231570e308 */
#define MAXDOUBLE	1.79769313486231570e307
#define MAXFLOAT	((float)3.402823466385288598e38)
#define _DEXPLEN	11
#define _HIDDENBIT	1
/* this won't work until NaN's work properly */
/* #define DMINEXP	(-(DMAXEXP + DSIGNIF)) */
/* #define FMINEXP	(-(FMAXEXP + FSIGNIF)) */
#define DMINEXP	(-DMAXEXP)
#define FMINEXP	(-FMAXEXP)
#define CUBRTHUGE	2.6196213420787355e102
#define INV_CUBRTHUGE	.38173571415718220434e-102
#endif
#if pdp11 || vax
#define MAXDOUBLE	1.701411834604692293e38
#define MAXFLOAT	((float)1.701411733192644299e38)
#define _DEXPLEN	8
#define _HIDDENBIT	1
#define DMINEXP	(-DMAXEXP)
#define FMINEXP	(-FMAXEXP)
#define CUBRTHUGE 	5.5411913777556862e12
#define INV_CUBRTHUGE	1.80466605794262193729e-13
#endif
#if gcos
#define MAXDOUBLE	1.7014118346046923171e38
#define MAXFLOAT	((float)1.7014118219281863150e38)
#define _DEXPLEN	8
#define _HIDDENBIT	0
#define DMINEXP	(-(DMAXEXP + 1))
#define FMINEXP	(-(FMAXEXP + 1))
#define CUBRTHUGE	5.5411913777556862e12
#define INV_CUBRTHUGE	1.80466605794262193729e-13
#endif
#if u370
#define _LENBASE	4
#else
#define _LENBASE	1
#endif
#define _EXPBASE	(1 << _LENBASE)
#define _FEXPLEN	8
#define DSIGNIF	(BITSPERBYTE * sizeof(double) - _DEXPLEN + _HIDDENBIT - 1)
#define FSIGNIF	(BITSPERBYTE * sizeof(float)  - _FEXPLEN + _HIDDENBIT - 1)
#define _LMAX	(BITSPERBYTE * sizeof(long) - 2)
#define DMAXPOWTWO	((double)(1L << _LMAX) * (1L << (DSIGNIF - _LMAX - 1)))
#define FMAXPOWTWO	((float)(1L << (FSIGNIF - 1)))
#define DMAXEXP	((1 << (_DEXPLEN - 1)) - 1)
#define FMAXEXP	((1 << (_FEXPLEN - 1)) - 1)
#define MAXBEXP	DMAXEXP /* for backward compatibility */
#define MINBEXP	DMINEXP /* for backward compatibility */
#define MAXPOWTWO	DMAXPOWTWO /* for backward compatibility */
