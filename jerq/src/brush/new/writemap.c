/*
 *
 *	write files for Brush  --  brush/writemap.c
 *
 *	NOTE:	dmdps (J5620) code courtesy of Andy Schnable
 *		blitblt (JERQ) code courtesy of Tom Duff
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "myjerq.h"
#include <jerqio.h>

puttextures( tp,n,fp )		/* write a set of textures (brushes or shades) */
	Texture16 *tp[];
	int n;
	FILE *fp;
{
	short buff[16],i,j;

	for ( i=0; i<n; i++ )
	    {
		for ( j=0; j<16; j++ ) buff[j] = tp[i]->bits[j];
		fwrite( (char *)buff,sizeof( Texture16 ),1,fp );
	    }
}

#ifdef BITFILE
#define SHORTSIZE	16
#define sendword(w, f)	(putc(w&255, f), putc((w>>8)&255, f))

extern Word buffer[], raster[];
extern Bitmap bbuffer;
extern int ctype, count;
extern short *p1, *endraster;

putbitmap( bp,r,f,compl )
	Bitmap *bp, *compl;
	Rectangle r;
	FILE *f;
{
	register i, nrasters, rastwid, nrword;
	Rectangle rrast;

	nrasters = r.corner.y - r.origin.y;
	i = r.corner.x - r.origin.x;
	rastwid = ( i+SHORTSIZE-1 )/SHORTSIZE;
	nrword = ( i+WORDSIZE-1 )/WORDSIZE;
	endraster = (short *)raster + rastwid-1;

	sendword( 0,f );		/* 0 1st word for some silly reason	*/
	sendword( r.origin.x,f );
	sendword( r.origin.y,f );
	sendword( r.corner.x,f );
	sendword( r.corner.y,f );

	rectf( &bbuffer,bbuffer.rect,F_CLR );
	for ( i=0; i<nrword; i++ )
		raster[i] = 0;

	rrast = r;

	if ( compl )
		rectf( compl,r,F_XOR );

	for ( ; rrast.origin.y < r.corner.y; rrast.origin.y++ )
	    {
		rrast.corner.y = rrast.origin.y + 1;
		if ( compl )
			rectf( compl,rrast,F_XOR );
		bitblt( bp,rrast,&bbuffer,Pt( 0, 0 ),F_STORE );
		for ( i=0; i<nrword; i++ )
			raster[i] ^= buffer[i];
		putrast( f );
		for ( i=0; i<nrword; i++ )
			raster[i] = buffer[i];
	    }
}

static
putrast( f )
	FILE *f;
{
	register short *p2;

	p1 = p2 = (short *)raster;
	do {
		if ( p1 >= p2 )
		   {
			p2 = p1 + 1;
			count = 2;
			ctype = *p1 == *p2;

		   }
		else	if ( ( *p2 == *( p2+1 ) ) == ctype )
			   {
				if ( ++count >= 127 )
				   {
					putbits( f );
					p1 = p2 + 2;
				   }
				else	p2++;

			   }
			else	if ( ctype )
				   {
					putbits( f );
					p1 = p2 + 1;
					ctype = 0;

				   }
				else {
					count--;
					putbits( f );
					p1 = p2;
					ctype = 1;
				     }
	   } while ( p2 < endraster );

	if ( p1 <= endraster )
	   {
		if ( p2 > endraster )
			count--;
		putbits( f );
	   }
}

static
putbits( f )
	FILE *f;
{
	int c, i;

	c = count;
	if ( ctype )
	   {
		c += 128;
		count = 1;
	   }
	putc( c,f );
	fwrite( (char *)p1,sizeof *p1,count,f );
}
#endif

#ifndef BITFILE

/*	compile DMDPS format code	*/

#ifndef VALSCREENCOLOR
#include <setup.h>
#endif

#define sendshort(w)	(putc(w&255, filep),putc((w>>8)&255, filep))
#define WID (r.corner.x-r.origin.x)
#define BNULL ((Bitmap *) NULL)

static Bitmap *braster = BNULL;
static short ctype, count; 
static short *p1;
static FILE *filep;
#define allbuttons	((mouse.buttons&7)==7)

putbitmap( bp,r,fileptr,dummy )
	Bitmap *bp,*dummy; 
	Rectangle r; 
	FILE *fileptr;
{
	short nrasters,rastwid;
	Rectangle rrast;

	filep = fileptr;
	nrasters = r.corner.y-r.origin.y;
	rastwid = (WID+15)/16;
	sendshort( nrasters ); 
	sendshort( rastwid );

	if ( braster == BNULL)
		if ( ( braster= balloc(Rect(0, 0, XMAX, 1))) == BNULL )
			return 0;

	rrast = r;
	rectf( bp,r,F_XOR );

	for ( ; rrast.origin.y < r.corner.y; rrast.origin.y++ )
	    {
		rrast.corner.y = rrast.origin.y + 1;
		rectf( bp,rrast,F_XOR );
		rectf( braster,Rect( 0,0,XMAX,1 ),F_CLR );

		bitblt( bp,rrast,braster,Pt( 0,0 ),F_STORE );
		if ( !VALSCREENCOLOR )
			rectf( braster,Rect( 0,0,XMAX,1 ),F_XOR );
		sendrast( braster,Pt( 0,0 ),rastwid );
		if ( allbuttons ) return 0;
	    }
	return 1;
}

static
sendrast( bmap,pnt,rastwid )
	Bitmap	*bmap;
	Point	pnt;
	short	rastwid;
{
	short *p2,*endraster;

	p1 = p2 = (short *) addr( bmap, pnt );
	endraster = p1 + rastwid - 1;

	do {
		if ( p1 >= p2 )
		   {
			p2 = p1 + 1; 
			count = 2;
			ctype = ( *p1 == *p2 );

		   }
		else	if ( ( *p2 == *(p2+1) ) == ctype )
			   {
				if ( ++count >= 127 )
				   {
					sendbits();
					p1 = p2 + 2;
				   }
				else	p2++;

			   }
			else	if ( ctype )
				   {
					sendbits();
					p1 = p2 + 1;
					ctype=0;

				   }
				else {
					count--; 
					sendbits();
					p1 = p2;
					ctype=1;
				     }
	   } while ( p2 < endraster );

	if ( p1 > endraster ) return;
	if ( p2 > endraster ) count--;
	sendbits();
	fflush( filep );
	sync();
}

static sendbits()
{
	short c;
	c = count; 
	if ( ctype )
	   { 
		c += 128; 
		count = 1; 
	   }
	putc( c,filep );
	fwrite( (char *)p1,2*count,1,filep );
}

#endif
