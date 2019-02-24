#include "term.h"

Rectangle ZRectangle;
Bitmap physical = { (Word *) 0x700000, 25, 0, 0, XMAX, YMAX };

waitMOUSE() { wait(MOUSE); }

long assertf(l)
long l;
{
	extern char *J_hist;
	register c;

	if( l ) return l;
#ifdef JOURNAL
	PutTextf( "pi.m: assertion failed near %s\n", J_hist );
#else
	PutTextf( "pi.m: assertion failed\n" );
#endif
	tolayer(P);
	for(;;){
		while( (c = rcvchar()) != -1 )
			if( c>=040 && c<0177 ) PutText(c);
		DIRTYServe();
		rectf( &display, P->rect, F_XOR );
		sleep(60*5);
	}
}

char *dec(n)
register n;
{
	register char *pic = "-zzzzzzzzz9";
	char cip[10];
	register char *p = pic, *c;

	if( n < 0 ) *p++ = '-', n = -n;
	*(c=&cip[0]) = '\0';
	do {
		*++c = '0'+(n%10);
	} while( n = n/10 );
	while( *c ) *p++ = *c--;
	*p = '\0';
	return pic;
}

char *hex(n)
register n;
{
	register char *p = "zzzzzzzx"+7;

	do {
		*p-- = "0123456789ABCDEF"[n&0xF];
		n >>= 4;
	} while( n );
	return p+1;
}
	
outline( r )
Rectangle r;
{
	segment( &physical, r.origin, Pt(r.origin.x,--r.corner.y), F_XOR );
	segment( &physical, Pt(--r.corner.x,r.origin.y), r.origin, F_XOR );
	segment( &physical, Pt(r.origin.x,r.corner.y), r.corner, F_XOR );
	segment( &physical, r.corner, Pt(r.corner.x,r.origin.y), F_XOR );
}
