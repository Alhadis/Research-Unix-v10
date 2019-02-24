#include "term.h"
#include "../protocol.h"

Select(p)
Proc *p;
{
	register Layer *l;
	if( !p || !(l = p->layer) ) return;
	upfront(l);
	tolayer(l);
}

int StartFocus = 45;
Focus(r)
Rectangle r;
{
	register i;
	outline(r);
	while( own(MOUSE) && !butts ){
		for( i = StartFocus; i>=0 && !butts; --i ){
			outline(inset(r,-i));
			nap(2);
			outline(inset(r,-i));
		}
		for( i = 1; i<=5 && !butts; ++i )
			sleep(4);
	}
	if( butts != BUTT2 )
		outline(r);
}

Graphic( pkt )
{
	register long a, n, i;
	Rectangle r, clipped;
	register Bitmap *b;
	register Texture *t;
	Point shift, p, save;
	register char *error = 0, *offscreen = "offscreen";

	a = RcvLong();
	Select(P);
	waitMOUSE();
	save = mouse.xy;
	cursset(div(add(Drect.origin,Drect.corner),2));
	PutTextf( "at %x: ", a );
	if( !(error = ReadErr(a,4)) ){
	    switch( pkt ){
	    case CG_POINT:
		p = *(Point *) a;
		PutTextf( "Point{%d,%d}%>", p.x, p.y );
		if( !ptinrect(p, Jrect) ){
			error = offscreen;
			break;
		}
		Focus(Rpt(p,p));
		break;
	    case CG_RECT:
		clipped = r = *(Rectangle *)a;
#define ro r.origin
#define rc r.corner
		PutTextf("Rectangle{{%d,%d},{%d,%d}}%>", ro.x, ro.y, rc.x, rc.y);
		if( !rectclip(&clipped,Rect(0,0,XMAX,YMAX)) ){
			error = offscreen;
			break;
		}
		if( !eqrect(clipped,r) )
			error = "partially offscreen";
		Focus(r);
		break;
	    case CG_TEXTURE:
		PutTextf( "Texture%>" );
		t = (Texture*) a;
		for( i = 0; i < 16; ++i )
			if( t->bits[i] ) break;
		if( i >= 16 ){
			error = "zero Texture";
			break;
		}
		cursswitch( t );
		while( !butts ) waitMOUSE();
		r = raddp( Rect(0,0,16,16),
			Pt(mouse.xy.x&0xFFF0, mouse.xy.y&0xFFF0) );
		if( rectclip(&r, Drect) )
			texture( &display, r, a, F_STORE );
		break;
	    case CG_BITMAP:
		b = (Bitmap*) a;
		PutTextf( "Bitmap%>" );
		while( !butts ){
			shift = sub( mouse.xy,
				div(add(b->rect.corner,b->rect.origin),2) );
			r = raddp( b->rect, shift );
			outline(r);
			nap(2);
			outline(r);
		}
		if( butts && rectclip(&r, Drect) ){
			clipped = rsubp( r, shift );
			bitblt(b, clipped, &display, r.origin, F_STORE);
		}
	    }
	}
	PutTextf( " %s\n%>", error );
	SendString(error);
	while( butts ) waitMOUSE();
	cursset(save);
	cursswitch(Cursor);
	Select(UserInterface);
}
