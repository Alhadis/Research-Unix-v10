/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */
/* ric.h -  Ricoh IS-30 scanner constants, typedefs, function declarations
   The scanner program `rscan' creates a file (typically >1Mbyte).
   It has an ascii header, terminated by \n\n -- see RIC_hdr for its data fields.
   The rest is binary scanner data. Each `RIC_hdr.bpl' bytes holds one scan-line's
   pixels, 1 bit/pixel.  A `1' bit means black.  The order of the bytes in a line
   is left-to-right across the page.  The low-order bit in a byte is the left-most.
   Conventionally, X-coordinates start at 0, at the left of the page, and
   increase to the right.  Y-coordinates start at 0 at the top of the page,
   and increase down.

   REQUIRES:  prior #include "Coord.h"
   */

typedef struct {	/* Scanner file header */
	short res_x,res_y;  /* resolution in pixels/inch (x,y may differ) */
	short bpl;	/* bytes per scan line */
	Bbx bx;		/* bounding box (pixel indices 0,1,...) */
	} RIC_hdr;

/* these routines are found in /usr/hsb/ricoh/riclib.c */
int RIC_get_hdr();	/* args: fd, (RIC_hdr *); returns: 1==OK, 0==EOF, -1=ERR */
int RIC_line();		/* arg: (char **); returns: 1==OK, 0==EOF, -1==ERR */
RIC_skip();		/* arg: int no. lines to skip */
char *S_hdr_toa();	/* RIC_hdr in printable-string form */

