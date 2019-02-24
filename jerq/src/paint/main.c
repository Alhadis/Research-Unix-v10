#include "paint.h"
Image *brush, Canvas;
static int myvis=0, myinhibited=0;
static Point mypos;
/*
 * Draw my cursor at position p.  Inhibit the real cursor if necessary.
 */
mycurs(p)
Point p;
{
	if(!myinhibited){
		cursinhibit();
		myinhibited=1;
	}
	if(myvis){
		if(p.x==mypos.x && p.y==mypos.y)
			return;
		mydrawcurs();
	}
	mypos=p;
	mydrawcurs();
	myvis=1;
}
/*
 * Inhibit my cursor.  If allow is set, turn the real cursor back on.
 */
mycursoff(allow){
	if(myvis){
		mydrawcurs();
		myvis=0;
	}
	if(allow && myinhibited){
		cursallow();
		myinhibited=0;
	}
}
/*
 * draw (or erase) the cursor
 */
mydrawcurs(){
	bitblt(brush->alf, brush->rect, &display, sub(mypos, brushcen), F_XOR);
}
main(){
	register NMitem *mp;
	NMitem mhit();
	int opindex=0;
	canvas->rect.or=Drect.or;
	canvas->rect.co=sub(Drect.co, Pt(0, 20));
	canvas->grn = &display;
	if((canvas->alf=balloc(canvas->rect))==0)
		exit(1);
	msgrect.or.x=canvas->rect.or.x;
	msgrect.or.y=canvas->rect.co.y+1;
	msgrect.co=Drect.co;
	rectf(canvas->grn, canvas->rect, F_CLR);
	rectf(canvas->alf, canvas->rect, F_CLR);
	segment(&display, Pt(canvas->rect.or.x, canvas->rect.co.y), canvas->rect.co,
		F_OR);
	readstate();
	showstate();
	brushread(brushname);
	if(!brush){
		pause("Initialization failure!");
		exit(1);
	}
	request(MOUSE|KBD|SEND|RCV);
	for(;;){
		wait(CPU);
		mycurs(mouse.xy);
		if(button12()){
			mycursoff(0);
			(*styles[curstyle])();
		}
		else if(button3()){
			mycursoff(1);
			if((mp=mhit(&Mmain, 3, 0)) && !mp->next && mp->fn){
				(*mp->fn)(mp);
				showstate();
			}
		}
	}
}
