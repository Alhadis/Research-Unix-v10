
/*
 *
 * Things needed to rotate glyphs. Includes setting new reference point,
 * bitmap dimensions, and filling in the new bitmap.
 *
 */


#include <stdio.h>

#include "gen.h"			/* a few general definitions */
#include "rast.h"			/* for definition of Glyph */
#include "rotate.h"			/* rotation macros */


/*
 *
 * These guys are used in the translation and rotation macros defined
 * in rotate.h.
 *
 */


int	cosine[ROT_COUNT] = {1, 0, -1, 0};
int	sine[ROT_COUNT] = {0, 1, 0, -1};
int	delta[ROT_COUNT] = {1, 0, 0, 0};


char	*bptr = NULL;			/* last rotated bitmap */


/*****************************************************************************/


rotate(glyph, angle)


    Glyph	*glyph;			/* rotate this glyph */
    int		angle;			/* to this orientation */


{


    Glyph	oglyph;			/* old glyph data */
    int		orwid;			/* rows in the old bitmap */
    int		nrwid;			/* same but for the new bitmap */
    int		mapsize;		/* byte size of the new map */
    int		i, j;			/* coordinates in the old bitmap */
    int		x, y;			/* same but in the new bitmap */


/*
 *
 * Called when we want to rotate glyph to an orientation of angle, where
 * angle should be 0, 1, 2, or 3. I decided against using a complete set
 * of raster files that had already been rotated in favor of doing the work
 * at run time. Really don't expect this stuff to be used that much and
 * we've already got too many raster files around.
 *
 */


    if ( angle == 0 ) return;		/* everything's already done */

    oglyph = *glyph;			/* we'll need most of the old values */

    glyph->xref = NEWX(oglyph.xref, oglyph.yref, oglyph.height, oglyph.width, angle);
    glyph->yref = NEWY(oglyph.xref, oglyph.yref, oglyph.height, oglyph.width, angle);
    glyph->width = (angle % 2) ? oglyph.height : oglyph.width;
    glyph->height = (angle % 2) ? oglyph.width : oglyph.height;

    if ( bptr != NULL )
	free(bptr);

    mapsize = ((glyph->width + BYTE - 1) / BYTE) * glyph->height;
    if ( (bptr = (char *) calloc(mapsize, sizeof(char))) == NULL )
	error(FATAL, "can't get memory for bitmap");

    orwid = (oglyph.width + BYTE - 1) / BYTE;
    nrwid = (glyph->width + BYTE - 1) / BYTE;

    for ( j = 0; j < oglyph.height; j++ )
	for ( i = 0; i < oglyph.width; i++ )
	    if ( checkbit(oglyph.bptr, orwid * j * BYTE + i) )  {
		x = NEWX(i, j, oglyph.height, oglyph.width, angle);
		y = NEWY(i, j, oglyph.height, oglyph.width, angle);
		setbit(bptr, nrwid * y * BYTE + x);
	    }	/* End if */

    glyph->bptr = bptr;

}   /* End of rotate */


/*****************************************************************************/


