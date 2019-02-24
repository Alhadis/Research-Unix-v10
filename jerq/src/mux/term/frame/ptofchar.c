#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include "frame.h"
/*
 * Screen position of cn'th character in frame t.
 * Returns last visible point if cn is offscreen.
 */
#ifdef	SAFE
Point
ptofchar(t, cn)
	register Frame *t;
	register cn;
{
	frameop(t, opnull, t->rect.origin, t->str.s, cn);
	return endpoint;
}
#else
Point
ptofchar(t, cn)
	register Frame *t;
	register cn;
{
	register Nchar *np;
	register l, c;	/* line number, char number */
	/* nlines-1 because we must do the frameop */
	for(l=0,c=0,np=t->cpl; l<t->nlines-1 && c+*np<cn; l++)
		c+= *np++;
	frameop(t, opnull, startline(t, l), t->str.s+c, cn-c);
	return endpoint;
}
#endif
/*
 * Index of character at position pt in frame t.
 * Returns (efficiently!) number of chars in frame if pt is off end.
 */
int
charofpt(t, pt)
	register Frame *t;
	Point pt;
{
	register l, c, starty;	/* line, char posn */
	register i, y=pt.y;
	register Nchar *np;
	register char *s;
	pt.y-=(y-t->rect.origin.y)%newlnsz;	/* on a line bdry */
	y=pt.y-t->rect.origin.y;
	if((y/=newlnsz) > t->nlines)	/* y is line containing pt */
		y=t->nlines;	/* not nlines-1 because of upcoming if */
	for(c=0,l=0,np=t->cpl; l<y; l++)
		c+= *np++;
	if(y < t->nlines){
		endpoint=startline(t, y);
		starty=endpoint.y;
		s=t->str.s+c;
		for(i=0; i<*np; i++,c++){
			frameop(t, opnull, endpoint, s++, 1/* sigh */);
			if(endpoint.x>pt.x || endpoint.y!=starty)
				break;
		}
	}
	return c;
}
/*
 * Point at start of line n in frame t
 */
Point
startline(t, n)
	register Frame *t;
	register n;
{
	Point pt;
	pt=t->rect.origin;
	pt.y+=n*newlnsz;
	return pt;
}
/*
 * Calculate and set the number of characters per line in lines
 * first through last, inclusive, in frame t.
 */
setcpl(t, first, last)
	register Frame *t;
{
	register Nchar *np;
	register l, c;	/* lines, characters */
	
	if(last>=t->nlines)
		last=t->nlines-1;
	for(c=0,l=0,np=t->cpl; l<first; l++)
		c+= *np++;
	/* c now tracks number of chars before line of interest */
	while(l<=last && c<t->str.n)
		c+= *np++=cpl(t, c, startline(t, l++));
	while(l++ <= last)
		*np++=0;
}
/* Frameop operator to calculate number of chars in a line */
static nc, lasty, wrapped;
/*ARGSUSED*/
void
opcpl(t, p, q, cp, n)
	Frame *t;
	Point p, q;
	char *cp;
	int n;
{
	if(p.y==lasty)
		nc+=n;
	else
		wrapped=TRUE;
}
cpl(t, posn, pt)
	register Frame *t;
	int posn;
	Point pt;
{
	register ntotry=(t->rect.corner.x-t->rect.origin.x)/cwidth('1');
	/*
	 * If there are no control chars, you can't get more than
	 * (linewidth/charwidth) chars on a line, so to be safe we
	 * keep trying if the frameop didn't reach the end of the line.
	 */
	lasty=pt.y;
	do{
		if((ntotry+=7) > t->str.n-posn)	/* 7 is arbitrary */
			ntotry=t->str.n-posn;
		nc=0;
		wrapped=FALSE;
		frameop(t, opcpl, pt, t->str.s+posn, ntotry);
	}while(!wrapped && complete && posn+ntotry<t->str.n);
	return nc;
}
/*
 * Scroll first through last entries in the cpl array by n.
 * n<0 ==> up screen, n>0 ==> down screen.  First and last must
 * be in the correct order in the call, i.e. first>last for n>0,
 * first<last for n<0.
 */
scrollcpl(t, first, last, n)
	Frame *t;
	register first, last, n;
{
	register i;
	register Nchar *np= &t->cpl[first];
	register delta=(n>0)? -1 : 1;
	if(first>last){
		i=first;
		first=last;
		last=i;
	}
	for(i=first; i<=last; i++,np+=delta)
		np[n]=np[0];
}
lineno(t, y)
	register Frame *t;
	register y;
{
	return (y-t->rect.origin.y)/newlnsz;
}
