#include <jerq.h>
#include <layer.h>
#include <font.h>

#include "jcom.h"
#include "jjplot.h"

char *menu2t[] = {"to exit jplot, type `term mux'", NULL};
Menu menu2 = {menu2t};

Point
	PtCurrent,	/* current position */
	pt(),		/* return character point */
	getpt();		/* return unpaced point from input */

Rectangle
	arect;		/* the aesthetic rectangle in a layer */

Texture t={
0177777, 0177777, 0177777, 0177777,
0177777, 0177777, 0177777, 0177777,
0177777, 0177777, 0177777, 0177777,
0177777, 0177777, 0177777, 0177777};
Texture cherries={
0, 037574, 077776, 017234,
000200, 000500, 003060, 004010,
016030, 037074, 077576, 077576,
077576, 077576, 037074, 016030};
Texture bye = {
0x8000, 0x8000, 0x8000, 0x8000,
0x884C, 0xE451, 0x9451, 0x8AA1,
0x8ADF, 0x8AA0, 0x8121, 0x711E,
0x0100, 0x0200, 0x0200, 0x0400};

int
	cwidth,
	tab,
	lineth = 1,
	xdelta,		/* distance along x axis */
	ydelta,		/* distance along y axis */
	delta,		/* min (xdelta, ydelta) */
	pushed = 0;	/* character stored away 0 = no, 1 = yes */
char
	pushc;		/* character stored away */
struct seg mLine[4];

char cursor = '\01';
main()
{

	request(SEND |KBD| RCV|MOUSE);

	init();
	cwidth = (defont.info + '0')->width;
	tab = cwidth * 8;
	PtCurrent = arect.origin;
	outc(cursor, &PtCurrent, 0);
	for(;;) {
		wait(KBD|RCV|MOUSE);
		if(ckreshape())reinit();
		if(own() && MOUSE){
			if(button2())
				switch(menuhit(&menu2, 2)){
				case 0:
					break;
				}
			else if(button3()){
				request(KBD|RCV);
				wait(CPU);
				request(KBD|SEND|RCV|MOUSE);
			}
		}
		if(own() & KBD){
			while(own() & KBD)
				sendchar(kbdchar() & 0177);
		}
		if(own() & RCV){
			outc(cursor, &PtCurrent, 0);
			while(own() & RCV)
				outc(getchar(), &PtCurrent, 1);
			outc(cursor, &PtCurrent, 0);
		}
	}
}	
newline()
{
	cursinhibit();
	if(PtCurrent.y >= arect.corner.y - 2 * defont.height) {
		bitblt(&display, Rpt(Pt(arect.origin.x, arect.origin.y + defont.height), 
			arect.corner), &display, arect.origin, F_STORE);
		stipple(Rpt(Pt(arect.origin.x, arect.corner.y - defont.height),
			Drect.corner));
	} else
		PtCurrent.y += defont.height;
	cursallow();
}

getchar()
{
	register c;

	if((c = rcvchar()) == -1){
		wait(RCV);
		c = rcvchar();
	}
	return(c);
}

getint(){
	int d,r;
	r = getchar() & 0377;
	d = getchar();
	return( r | ((d & 0377 )<<8) );
}
Point
getpt()
{
	Point	p;
	char	c;

	p.x = getchar()&077;
	c = getchar();
	p.x |= (c&017) << 6;
	p.y = getchar()& 0177;
	p.y |= (c&0160) << 3;

	p.x = muldiv(p.x, delta, 800);
	p.x += arect.origin.x;
	p.y = muldiv(p.y, delta, 800);
	p.y += arect.origin.y;

	return (p);
}

stipple(r)
Rectangle r;
{
	rectf(&display, r, F_CLR);
}
#ifdef mc68000
ringbell(){
	*((char *)(384 * 1024L + 062)) = 2;
}
#endif
reinit(){
	register x, y;
	x = PtCurrent.x - arect.origin.x;
	y = PtCurrent.y - arect.origin.y;
	if(P->state & (MOVED|RESHAPED)){
		init();
	}
	PtCurrent.x = arect.origin.x + x;
	PtCurrent.y = arect.origin.y + y;
	if(!ptinrect(PtCurrent, arect))
		PtCurrent = arect.origin;	
	P->state &= ~(MOVED|RESHAPED);
}
init(){
	arect = Drect;
	arect.origin.x += XMARGIN;
	arect.corner.x -= XMARGIN;
	arect.origin.y += YMARGIN;
	arect.corner.y -= YMARGIN;

	xdelta = arect.corner.x - arect.origin.x;
	ydelta = arect.corner.y - arect.origin.y;
	delta = xdelta > ydelta ? ydelta : xdelta;
}
