/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* font.h - typedefs, etc for font-characteristic functions:
   The baseline is defined by the locus of bottoms of all characters that
   have no descenders and are not punctuation.
   The top is the top of characters without risers (negative).
   The `rise' is the (negative) height of risers (above baseline), and `descent'
   is the positive) distance below baseline.
    */

/* font attributes, in absolute scanner coordinates */
typedef struct FontAttr {
	Scoor body;		/* point size */
	Scoor xhgt;		/* x-height */
	Scoor ascd;		/* height of ascenders (above xheight, >0) */
	Scoor caph;		/* capital height (above baseline, >0) */
	Scoor desc;		/* height of descenders (below baseline, >0) */
	Scoor doth;		/* height of punctuation dots */
	} FontAttr;

#ifdef MAIN
FontAttr *ps_to_attr();	/* pointsize, digit'n resol'n --> attributes */
xhgt_to_ps();
caph_to_ps();
#endif
