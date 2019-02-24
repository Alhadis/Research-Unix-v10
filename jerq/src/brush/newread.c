/*
 *
 *	Obsolete read bitmap for Brush  --  brush/newread.c
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

#define allbuttons	((mouse.buttons&7)==7)
extern Bitmap *stash;		/* bitmap pointer from recall_pic()	*/

/*
	readbitmap
	returns 0 if no space, 
		1 if ok, 
		-1 if read error. 
*/

readbitmap( stream)
	FILE *stream;
{
	register i;
	short rc, nrasters, rastwid;
	Bitmap *bp;

	nrasters=getw(stream);
	rastwid=getw(stream);
	if ( (bp=balloc(Rect(0,0,rastwid*16,nrasters))) == (Bitmap *) NULL )
		return 0;

	if ( stash ) bfree( stash );
	stash = bp;

	for ( i=0 ;(i<nrasters)&&((rc=readrast(stash,i,rastwid,stream))!=EOF); i++)
	    { if ( allbuttons ) return 2;	} 
	return rc;
}

readrast(bp,i,nw,in)
Bitmap *bp;
int i;
int	nw;
FILE	*in;
{
	int	count, ctype;
	char	*bits;

	bits=(char *)addr(bp,Pt(0,i));
	while (nw>0) {
		count=getc(in);
		count &= 255;
		ctype=count&128;
		count &= 127;
		nw -= count;
		count *= 2;

		if (ctype) {
			if (fread(bits,2,1,in) != 1)
				return (EOF);
			for (count-=2; count>0; count--) {
				*(bits+2) = *bits;
				bits += 1;
			}
			bits += 2;
		} else {
			if (fread(bits,count,1,in) != 1)
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

	if ((l=getc(stream)) == EOF)
		return (EOF); 
	if ((h=getc(stream)) == EOF)
		return(EOF);
	return (h&255)<<8 | (l&255);
}

