/*
 *
 *	read files for Brush  --  brush/readmap.c
 *
 *	NOTE:	dmdps code courtesy of Andy Schnable
 *		blitblt "      "     " Tom Duff
 *
 *	Brush 
 *	Garry Hodgson	1985
 *
 *	Copyright 1985 AT&T Bell Laboratories
 *	All Rights Reserved
 */

#include "myjerq.h"
#include <jerqio.h>

gettextures( tp,n,fp )		/* read in a set of textures (brushes or shades) */
	Texture16 *tp[];
	int n;
	FILE *fp;
{
	short buff[16],i,j,stat = 1;

	for ( i=0; stat>0 && i<n; i++ )
		if ( ( stat=fread( buff,sizeof( Texture16 ),1,fp ) ) >0 )
			for ( j=0; j<16; j++ )
				tp[i]->bits[j] = buff[j];
}

#ifdef BITFILE

#define SHORTSIZE	16

#define sendword(w, f)	(putc(w&255, f), putc((w>>8)&255, f))

/*	note buffers have extra word so putbitmap() can align	*/

Word buffer[1+XMAX/WORDSIZE], raster[1+XMAX/WORDSIZE];

Bitmap bbuffer={ buffer, (sizeof buffer)/(sizeof(Word)), 0, 0, XMAX, 1};

int ctype, count;
short *p1, *endraster;

getword(stream)		/* snarfed from J5620 version */
FILE *stream;
{
	register l, h;

	if ( ( l=getc( stream ) ) == EOF ) return (EOF); 
	if ( ( h=getc( stream ) ) == EOF ) return(EOF);
	return ( h&255 ) << 8 | ( l&255 );
}

Bitmap *
readbitmap( f )
	FILE *f;
{
	register Bitmap *bp=null( Bitmap );
	register h, w, i, j;
	Rectangle r;

	i = getword( f );		/* eat first 2 null bytes */

	r.origin.x = getword( f );
	r.origin.y = getword( f );
	r.corner.x = getword( f );
	r.corner.y = getword( f );

	h = r.corner.y - r.origin.y;
	w = r.corner.x - r.origin.x;
	w = ( w+SHORTSIZE-1 )/SHORTSIZE;

	if ( ( bp = balloc( r ) ) == null(Bitmap) )
		return null(Bitmap);

	rectf( &bbuffer,bbuffer.rect,F_CLR );

	for ( i=0; i!=h; i++ )
	    {
		if ( getrast( f,(short *)raster,w ) )
		   {
			bfree( bp );
			return null(Bitmap);
		   }
		for ( j=0; j!=bp->width; j++ )
			buffer[j] ^= raster[j];
		bitblt( &bbuffer,bbuffer.rect,
			bp,add(r.origin,Pt( 0,i ) ),F_STORE );
	    }
	return bp;
}

static
getrast( f,p,nwords )	/* receive single compressed raster */
	FILE *f;
	register short *p;
	int nwords;
{
	register i,count;

	for ( ; nwords>0; nwords-=count )
	    {
		if ( ( count=getc( f ) ) <= 0 )
			return 1;
		if ( count&128 )
		   {
			count &= 127;	/* count must not be zero, not checked */
			if ( fread( p,sizeof(short),1,f ) != 1 )
				return 1;
			for ( i=1; i!=count; i++ )
			    {
				*(p+1) = *p;
				p++;
			    }
			p++;
		   }
		else {
			if ( fread( p,sizeof(short),count,f ) !=count )
				return 1;
			p += count;
		     }
	    }
	return ( nwords != 0 );
}
#endif
#ifndef BITFILE

/*	compile DMDPS format code	*/

#ifndef VALSCREENCOLOR
#include <setup.h>
#endif

#define allbuttons	((mouse.buttons&7)==7)

Bitmap *
readbitmap( stream )
	FILE *stream;
{
	register i;
	short rc, nrasters, rastwid;
	Bitmap *bp;

	nrasters = getw(stream);
	rastwid = getw(stream);

	if ( (bp=balloc(Rect(0,0,rastwid*16,nrasters))) == (Bitmap *) NULL )
		return bp;

	for ( i=0 ;(i<nrasters)&&((rc=readrast(bp,i,rastwid,stream))!=EOF); i++)
		if ( allbuttons ) break;

	if ( !VALSCREENCOLOR )
		rectf( bp,bp->rect,F_XOR );
	return bp;
}

readrast( bp,i,nw,in )
	Bitmap *bp;
	int i;
	int	nw;
	FILE	*in;
{
	int count,ctype;
	char *bits;

	bits = (char *)addr(bp,Pt(0,i));
	while ( nw > 0 )
	      {
		count = getc( in );
		count &= 255;
		ctype = count&128;
		count &= 127;
		nw -= count;
		count *= 2;

		if ( ctype )
		   {
			if ( fread( bits,2,1,in ) != 1 )
				return (EOF);
			for ( count-=2; count>0; count-- )
			    {
				*(bits+2) = *bits;
				bits += 1;
			    }
			bits += 2;
		   }
		else {
			if ( fread( bits,count,1,in ) != 1 )
				return (EOF);
			bits += count;
		     }
	}
	return (1);
}

getw(stream)
FILE *stream;
{
	register l, h;

	if ( ( l=getc( stream ) ) == EOF ) return (EOF); 
	if ( ( h=getc( stream ) ) == EOF ) return(EOF);
	return ( h&255 ) << 8 | ( l&255 );
}

#endif

