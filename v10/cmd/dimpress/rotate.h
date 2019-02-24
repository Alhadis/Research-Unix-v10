
/*
 *
 * A few definitions and macros used by routines that rotate glyphs.
 *
 * Although the Imprint-10 System Manual seems to imply that downloaded
 * glyphs can be rotated if desired, version 1.8 only appears to use the
 * rotation bits to locate glyphs that the user claims can be used in a
 * rotated coordinate system. In otherwords it appears that we're either
 * going to have to use raster tables that have already been rotated or
 * do all the required rotations and coordinate mappings at run time.
 *
 */


#include "impcodes.h"			/* just needed for ROT_COUNT */


/*
 *
 * The obvious functions, for the defined rotation angles, used in the
 * in the coordinate transformation macros given below. Arrays are defined
 * in rotate.c.
 *
 */


#define COSINE(A)	cosine[A]
#define SINE(A)		sine[A]
#define DELTA(A)	delta[A]


/*
 *
 * A few macros used to do the rotation and translation of the bitmaps and
 * the other important glyph parameters.
 *
 */


#define ROTX(X, Y, A)			/* x in rotated coordinate system */	\
										\
	(X) * COSINE((A) % ROT_COUNT) - (Y) * SINE((A) % ROT_COUNT)


#define ROTY(X, Y, A)			/* y in rotated coordinate system */	\
										\
	(X) * SINE((A) % ROT_COUNT) + (Y) * COSINE((A) % ROT_COUNT)


#define TRANSX(H, W, A)			/* new x after bitmap translation */	\
										\
	(H-1) * DELTA(((A)-ROT_90)%ROT_COUNT) + (W-1) * DELTA(((A)-ROT_180)%ROT_COUNT)


#define TRANSY(H, W, A)			/* y after bitmap translation */		\
										\
	(H-1) * DELTA(((A)-ROT_180)%ROT_COUNT) + (W-1) * DELTA(((A)-ROT_270)%ROT_COUNT)


#define NEWX(X, Y, H, W, A)		/* x after rotation and translation */	\
										\
	ROTX(X, Y, A) + TRANSX(H, W, A)


#define NEWY(X, Y, H, W, A)		/* y after rotation and translation */	\
										\
	ROTY(X, Y, A) + TRANSY(H, W, A)



/*
 *
 * sine, cosine, and delta are integer arrays declared in rotate.c. Anyone
 * trying to use to coordinate transformation macros will need to know about
 * them and include rotate.c in their program.
 *
 */


extern int	cosine[];
extern int	sine[];
extern int	delta[];

