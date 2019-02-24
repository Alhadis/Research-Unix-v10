#include "univ.h"

waitMOUSE() { wait(MOUSE); }

char *itoa(n)
register n;
{
	register char *pic = " [-][0-9]* ";
	char cip[10];
	register char *p = pic, *c;

	*p++ = ' ';
	if( n < 0 ) *p++ = '-', n = -n;
	*(c = &cip[0]) = '\0';
	do {
		*++c = "0123456789"[n%10];
	} while( n = n/10 );
	while( *c ) *p++ = *c--;
	*p++ = ' ';
	*p = '\0';
	return pic;
}
	
dictorder(a,b)
register char *a, *b;
{
	register disc;

	for( ;; ){
		if( !*a	) return 1;
		if( !*b	) return 0;
		if( disc = (*a++|' ')-(*b++|' ') ) return disc<0;
	}
}
#ifdef ASSERT
long assertf(l)
long l;
{
	if( l ) return l;
	rectf( &display, P->rect, F_XOR );
	for( ;; ) sleep(60);
}
#endif

Point dxordy(prev)
Point prev;
{
	register dx, dy;
	Point mxy;

	nap(2);
	mxy = mouse.xy;
	if( abs(dy = mxy.y-prev.y) >= abs(dx = mxy.x-prev.x) )
		dx = 0;
	else
		dy = 0;
	cursset( mxy = add( prev, Pt(dx,dy) ) );
	return mxy;
}

outline( b, r )
register Bitmap *b;
Rectangle r;
{
	segment( b, r.origin, Pt(r.origin.x,--r.corner.y), F_XOR );
	segment( b, Pt(--r.corner.x,r.origin.y), r.origin, F_XOR );
	segment( b, Pt(r.origin.x,r.corner.y), r.corner, F_XOR );
	segment( b, r.corner, Pt(r.corner.x,r.origin.y), F_XOR );
}

Rectangle boundrect( q, r )
Rectangle q, r;
{
	q.origin.x = min( q.origin.x, r.origin.x );
	q.origin.y = min( q.origin.y, r.origin.y );
	q.corner.x = max( q.corner.x, r.corner.x );
	q.corner.y = max( q.corner.y, r.corner.y );
	return q;
}

Rectangle scrollbar( r, from, to, lo, hi )
Rectangle r;
{
	register short rh = r.corner.y - r.origin.y, h = hi-lo;
	
	r.corner.y -= muldiv(hi-to,rh,h);
	r.origin.y += muldiv(from-lo,rh,h);
	return r;
}

rectinrect( r, s )
Rectangle r, s;
{
	Rectangle clipped;

	clipped = r;
	return rectclip(&clipped,s) && eqrect(r, clipped);
}

char *FreeSome = "out of terminal memory; free some to continue";

char *Alloc(n)
{
	char *a;

	if( !(a = alloc(n)) ){
		cursswitch( &NoMemory );
		InvertKBDrect( FreeSome, "" );
		while( !(a = alloc(n)) ) wait( CPU );
		PaintKBD();
		cursswitch( Cursor );
	}
	return a;
}

char *GCAlloc(n,p)
register char **p;
{
	register unsigned long nbytes = n;
	if( !(gcalloc(nbytes, p)) ){
		cursswitch( &NoGCMemory );
		InvertKBDrect( FreeSome, "" );
		while( !(gcalloc(nbytes, p)) ) wait( CPU );
		PaintKBD();
		cursswitch( Cursor );
	}
	while( nbytes>0 ) (*p)[--nbytes] = 0;
	return *p;
}

GCString( p, s )
char **p, *s;
{
	*p = GCAlloc( strlen(s)+1, p );
	strcpy( *p, s );
}

Rectangle moverect(source,bound)
Rectangle source, bound;
{
	Rectangle target;
	Point base, track;
	int b = butts;

	base = mouse.xy;
	bound.origin.x += base.x - source.origin.x;
	bound.origin.y += base.y - source.origin.y;
	bound.corner.x += base.x - source.corner.x;
	bound.corner.y += base.y - source.corner.y;
	do {
		track = mouse.xy;
		if( track.x < bound.origin.x ) track.x = bound.origin.x;
		if( track.y < bound.origin.y ) track.y = bound.origin.y;
		if( track.x > bound.corner.x ) track.x = bound.corner.x;
		if( track.y > bound.corner.y ) track.y = bound.corner.y;
		cursset( track );
		outline( &display, target = raddp( source, sub(track,base) ) );	
		nap(2);
		outline( &display, target );
	} while( butts == b );
	return butts ? source : target;
}

