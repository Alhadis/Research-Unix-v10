/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* pic.h -  picture files.  See man page for picfile(5).
   They have an ascii header, terminated by \n\n; see PIC_hdr for its data fields.
   The rest is binary scanner data. Each `PIC_hdr.bpl' bytes holds one scan-line's
   pixels, 1 bit/pixel.  A `1' bit means black.  The order of the bytes in a line
   is left-to-right across the page.  The low-order bit in a byte is the left-most.
   Conventionally, X-coordinates start at 0, at the left of the page, and
   increase to the right.  Y-coordinates start at 0 at the top of the page,
   and increase down.
   NOTE:  must be preceded by #include "Coord.h"
   */

typedef struct PIC_hdr {
	/* Contents of picfile ASCII header (or other structure) */
	char type[40];		/* "rle" "dump" "binary" "ccitt-g4" "imdir" etc */
	Bbx bx;			/* WINDOW (bounding box) */
	short res_x;		/* resolution in pixels/inch (x,y may differ) */
	short res_y;
	char *parms;		/* PARMS= string */
	char *misc;		/* all other unexpected header lines, catenated */
	/* the following are used by I/O routines */
	FILE *fp;
	long seek;		/* current seek address */
	int bpl;		/* bytes per line (in file) */
	unsigned char *line;	/* one line of picture (malloc space) */
	unsigned char *pline;	/* prior line of picture (malloc space) */
	Scoor cy;		/* index no. of *line */
	} PIC_hdr;
#define Init_PIC_hdr {"",Init_Bbx,0,0,NULL,NULL,NULL,0L,0,NULL,NULL,0}
#if MAIN
    PIC_hdr empty_PIC_hdr = Init_PIC_hdr;
#else
    extern PIC_hdr empty_PIC_hdr;
#endif

/* these functions are in /usr/hsb/ocr/piclib.c */
PIC_hdr *alloc_PIC_hdr();
int PIC_get_hdr();
PIC_put_hdr();
err_PIC_line();
PIC_skip();
int PIC_rline();
int PIC_wline();
char *PIC_hdr_toa();
