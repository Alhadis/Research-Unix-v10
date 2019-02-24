#include <jerq.h>
#include "twid.h"
static Point curspt;
static bigbrush;
static visible=1;
Cursinhibit(){
	if(bigbrush)
		flipcursor(curspt);
	else
		cursinhibit();
}
Cursallow(){
	if(!bigbrush)
		cursallow();
	/* else will get redrawn by updatecurs() */
}
flipcursor(p)
	Point p;
{
	bitblt(brush, brush->rect, &display, add(p, brush->rect.origin), F_XOR);
	visible^=1;
}
/*
 * Strange routine: b is cursor/texture, isbrush is the
 * brush number.  isbrush==>0 means it's not a brush, it's a cursor
 * (e.g. the box cursor); isbrush>0 means it's a brush, and b
 * is either the cursor (isbrush<BBIG) or to be ignored
 */
Cursswitch(b, isbrush)
	Texture *b;
{
	if(visible)
		flipcursor(curspt);
	if(!isbrush){	/* just a regular cursswitch() */
		if(bigbrush){
			bigbrush=0;
			cursallow();
		}
		cursswitch(b);
	}else{		/* a brush, big or small */
		bigbrush=0;
		if(isbrush <= BBIG)
			cursswitch(b);
		else{
			bigbrush=1;
			cursinhibit();
			/* NOTE: Cursor/brush is not visible now! */
		}
	}
}
updatecurs(){
	if(!bigbrush)	/* mpx is doing it for us */
		return;
	/* Big brush! */
	if(visible)
		flipcursor(curspt);
	curspt=mouse.xy;
	flipcursor(curspt);
}
