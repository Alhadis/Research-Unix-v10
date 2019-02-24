#include "term.h"

#define WIDTH		9
#define PERLINE		80
#define LINES		5
#define BORDER		5

typedef struct Text {
struct	Text	*down;
	char	text[PERLINE+1];
} Text;

Text Loop[LINES], *Current;
int Ix, Dirty;
Bitmap *Screen;

void PutText(c)
{
	register i;
	register char *q;

	if( !Screen ){
#define		RX (WIDTH*PERLINE+2*BORDER)
#define		RY (LINES*defont.height+2*BORDER)
		Screen = balloc( Rect(0,0,RX,RY) );
		for( i = 0; i < LINES; ++i )
			Loop[i].down = &Loop[(i+1)%LINES];
		Current = Loop;
		Ix = 0;
		}
	Dirty = 1;
	switch( c ){
	case -1:
		bfree(Screen);
		return;
	case '\n':
		Current = Current->down;
	case '\r':
		Current->text[Ix=0] = '\0';
		return;
	case '\b':
		if( Ix > 0 )
			Current->text[Ix--] = '\0';
		return;
	case '\f':
		for( i = 0; i < LINES; ++i )
			PutText('\n');
		return;
	case '\t':
		do PutText(' ');
		while( Ix&3 );
		return;
	}
	if( c<040 && c>=177 )
		return;
	if( Ix >= PERLINE ) PutText('\n');
	q = Current->text+(Ix++);
	*q++ = c;
	*q = '\0';
}

void DIRTYServe()
{
	register Text *t;
	Point p;
	Rectangle clip;

	if( P->state & (RESHAPED|MOVED) ){
		Dirty = 1;
		P->state &= ~(RESHAPED|MOVED);
	}
	if( !Dirty || !Screen ) return;
	Dirty = 0;
	rectf( Screen, Screen->rect, F_STORE );
	rectf( Screen, inset(Screen->rect,2), F_CLR );
	p = add(Screen->rect.origin,Pt(BORDER,BORDER));
	t = Current->down;
	for(;;){
		string( &defont, t->text, Screen, p, F_XOR );
		p.y += defont.height;
		if( t == Current ) break;
		t = t->down;
	};
	clip = Screen->rect;
	p.x = Drect.origin.x;
	p.y = Drect.corner.y-clip.corner.y;
	clip = raddp(clip,p);
	rectclip(&clip,Drect);
	clip = rsubp(clip,p);
	bitblt( Screen, clip, &display, Drect.origin, F_STORE );
	wait(CPU);
}

void PutTextf( fmt, x )
char *fmt;
int x;
{
	register char *s;
	register int c, *ap = &x;

	while( (c = *fmt++) ){
		if( c != '%') PutText(c);
		else {
			switch( c = *fmt++ ){
			case '%':
				PutText('%');
				break;
			case 'd':
			case 'D':
				PutTextf( dec(*ap++) );
				break;
			case 'X':
			case 'x':
				PutTextf( "0x" );
				PutTextf( hex(*ap++) );
				break;
			case  'c':
				PutText( *ap++ );
				break;
			case  's':
				if( s = (char *) *ap++ ){
					while( c = *s++ ) PutText(c);
				break;
			case  '>':
				DIRTYServe();
				}
			}
		}
	}
}
