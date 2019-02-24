
/*
 *
 * A few general purpose definitons used in most of the files in this
 * package.
 *
 */


#define NON_FATAL	0		/* error classification */
#define FATAL		1

#define OFF		0
#define ON		1

#define FALSE		0
#define TRUE		1

#define BYTE		8
#define BMASK		0377

#ifndef V9
#define PI		3.141592654
#endif


#define MAX(A, B)	((A > B) ? A : B)
#define MIN(A, B)	((A < B) ? A : B)
#define ABS(A)		((A) >= 0 ? (A) : -(A))

