/*
 *
 *	Obsolete send bitmap for Brush  --  brush/newsend.c
 *
 *	NOTE:	this code courtesy of Andy Schnable
 *
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */


#include <jerq.h>
#include <jerqio.h>

#ifndef VALSCREENCOLOR
#include <setup.h>
#endif

#define sendshort(w)	(putc(w&255, filep),putc((w>>8)&255, filep))
#define WID (r.corner.x-r.origin.x)
#define BNULL (Bitmap *) NULL

static Bitmap *braster = BNULL;
static short ctype, count; 
static short *p1;
static FILE *filep;
#define allbuttons	((mouse.buttons&7)==7)

sendbitmap(bp,r,fileptr)
Bitmap *bp; 
Rectangle r; 
FILE *fileptr;
{
	short nrasters, rastwid; 
	Rectangle rrast;

	filep = fileptr;
	nrasters = r.corner.y-r.origin.y;
	rastwid =(WID+15)/16;
	sendshort(nrasters); 
	sendshort(rastwid);

	if ( braster == BNULL) {
		if ( ( braster= balloc(Rect(0, 0, XMAX, 1))) == BNULL )
			return 0;
		}

	rectf(braster,Rect(0,0,XMAX,1),F_CLR);

	rrast=r;
	rectf(bp,r,F_XOR);

	for ( ; rrast.origin.y<r.corner.y; rrast.origin.y++ )
	    {   rrast.corner.y = rrast.origin.y+1;
		rectf(bp,rrast,F_XOR);
		bitblt(bp, rrast, braster, Pt(0,0), F_STORE);
		if (!VALSCREENCOLOR)
			rectf(braster, Rect(0,0,WID,1), F_XOR);
		sendrast(braster, Pt(0,0), rastwid);
		if ( allbuttons ) return 0;			}
	return 1;
}

static
sendrast(bmap, pnt, rastwid)
Bitmap	  *bmap;
Point	pnt;
short	rastwid;
{
	short *p2, *endraster;

	p1=p2=(short *)addr(bmap, pnt);
	endraster = p1 + rastwid - 1;

	do {
		if (p1 >= p2) {
			p2=p1+1; 
			count=2;
			ctype=(*p1 == *p2);

		} else if ((*p2 == *(p2+1)) == ctype) {
			if (++count >= 127) {
				sendbits();
				p1=p2+2;
			} else p2++;

		} else if (ctype) {
			sendbits();
			p1=p2+1;
			ctype=0;

		} else {
			count--; 
			sendbits();
			p1=p2;
			ctype=1;
		}
	} while (p2<endraster);

	if (p1 > endraster) return;
	if (p2 > endraster) count--;
	sendbits();
	fflush(filep);
	sync();
}

static sendbits()
{
	short c;
	c=count; 
	if (ctype) { 
		c += 128; 
		count=1; 
	}
	putc(c, filep);
	fwrite((char *)p1, 2*count, 1, filep);
}
