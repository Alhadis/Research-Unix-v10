/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     T. Thompson - ATT-BL HO - first version         */
/* Routines for generating PostScript from RLE. */
/* The routines at the bottom of this file, which */
/* do the actual conversion to postscript, were */
/* take from 'sun2ps'.  The original header */
/* giving credit to its authors is there. */

/* The 'binary' version of the output is collected in a */
/* temporary file, which is then fed back to the routines */
/* that generate postscript in a run-length-encoded form. */
/* Ideally it should just convert the original run-length */
/* encoding directly, but this is just a hack.  The REAL */
/* bottleneck is the PostScript printer, of course.  */
/* Printing an 800 by 800 pixel image takes 5 minutes. */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "CPU.h"
#include "boole.h"
#include "limits.h"     /* numeric extreme values */
#include "Units.h"
#include "Coord.h"
#include "pic.h"
#include <sys/types.h>
#include <suntool/sunview.h>
#include <pixrect/pixrect.h>
#include <pixrect/pr_io.h>

int Raswidth;
int Raslength;
extern char Revbyte[256];
extern char Revnib[16];

static FILE *fout;
 
void
RAST_start(h)
PIC_hdr *h;
{
	int n, v;
	struct rasterfile rh;

	/* build a table of reversed bytes */
	for ( n=0; n<256; n++ )
		Revbyte[n] = Revnib[(n>>4)&0xf] | Revnib[n&0xf]<<4;

	fout = h->fp;
	h->bpl = (h->bpl+7)/8;
	Raslength = h->bpl * h->bx.b.y;
	Raswidth = h->bx.b.x;

	rh.ras_magic = RAS_MAGIC;
	rh.ras_width = h->bx.b.x;
	rh.ras_height = h->bx.b.y;
	rh.ras_depth = 1;
	rh.ras_length = Raslength;
	rh.ras_type = RT_STANDARD;
	rh.ras_maptype = RMT_NONE;
	rh.ras_maplength = 0;
	pr_dump_header(fout,&rh,(colormap_t*)0);
}

void
RAST_end()
{
}

/* write a full line of picture data, returning status:  1 OK, 0 EOF,
-1 ERR */
int RAST_wline(h,line)
    PIC_hdr *h;
    unsigned char *line;
{   int stat;
	int n;

	for ( n=h->bpl; n>0; n-- ) {
		if ( putc(Revbyte[*line++],fout) == EOF )
			break;
	}
	if ( n == 0 ) {
                h->seek += h->bpl;
                h->cy++;
                return(1);
                }
        else { /* ERR */
                err("write to fd%d stat%d",fileno(h->fp),stat);
                if((stat>=0)&&(stat<h->bpl)) return(0 /*EOF*/);
                else return(-1);
                };
        }
