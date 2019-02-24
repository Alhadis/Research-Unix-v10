#ifndef	JSTRUCTS_H
#define	JSTRUCTS_H
/*
 *  32 bit host
 */

#define	WORDSHIFT	5
#define	WORDSIZE	32
#define	WORDMASK	(WORDSIZE-1)
#define	ONES		0xFFFFFFFF
#define	FIRSTBIT	((unsigned)0x80000000)
#define	LASTBIT		((unsigned)0x1)

/*
 *	Graphics definitions
 */

typedef int	Word;		/* 32 bits */

typedef unsigned int	UWord;	/* 32 bits */

typedef struct Point {
	short	x;
	short	y;
} Point;

typedef struct Rectangle {
	Point origin;
	Point corner;
} Rectangle;

typedef struct Bitmap {
	Word	*base;		/* pointer to start of data */
	unsigned width;		/* width in 32 bit words of total data area */
	Rectangle rect;		/* rectangle in data area, local coords */
	char	*_null;		/* unused, always zero */
} Bitmap;

typedef struct Texture32 {
	Word bits[32];
} Texture32;

typedef struct Texture {
	short	bits[16];
} Texture;

#define	muldiv(a,b,c)	((long)((a)*((long)b)/(c)))

/*
 * Function Codes
 */
typedef int	Code;
#define	F_STORE	((Code) 0)	/* target = source */
#define	F_OR	((Code) 1)	/* target |= source */
#define	F_CLR	((Code) 2)	/* target &= ~source */
#define	F_XOR	((Code) 3)	/* target ^= source */
Bitmap *balloc();

#endif
