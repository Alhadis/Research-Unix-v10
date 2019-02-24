/*% 3cc -o # %
 */
#include <jerq.h>
#define DRAW_STOP	0
#define CLEANUP		1
#define EXIT		2
Bitmap screen;
char *menutext[]={
	"stop",
	"clean up",
	"exit",
	0
};
Menu menu={menutext};
Texture grey = {
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444,
	0x1111, 0x4444, 0x1111, 0x4444,
};
CleanUp(){
	texture(&screen, Rect(0, 0, XMAX-9, 8), &grey, F_STORE);
	texture(&screen, Rect(XMAX-9, 0, XMAX, YMAX-9), &grey, F_STORE);
	texture(&screen, Rect(8, YMAX-9, XMAX, YMAX), &grey, F_STORE);
	texture(&screen, Rect(0, 8, 8, YMAX), &grey, F_STORE);
}
PenTrack(p, q)
Point p, q;
{
	segment(&screen, p, q, F_OR);
	segment(&screen, add(p, Pt(1, 0)), add(q, Pt(1, 0)), F_OR);
	segment(&screen, add(p, Pt(0, 1)), add(q, Pt(0, 1)), F_OR);
	segment(&screen, add(p, Pt(1, 1)), add(q, Pt(1, 1)), F_OR);
}
PenPoint(p)
Point p;
{
	rectf(&screen, Rect(p.x, p.y, p.x+2, p.y+2), F_XOR);
}
main(){
	Point midlayer, realcurs, fakecurs, oldfakecurs;
	int drawing;
	screen.base=addr(&display, Pt(0, 0));
	screen.width=XMAX/WORDSIZE;
	screen.rect=Jrect;
	request(MOUSE);
	midlayer=div(add(Drect.origin, Drect.corner), 2);
	wait(MOUSE);
	fakecurs=mouse.xy;
	PenPoint(fakecurs);
	cursset(midlayer);
	nap(2);
	drawing=1;
	cursinhibit();
	for(;;){
		wait(MOUSE);
		if(drawing){
			realcurs=mouse.xy;
			cursset(midlayer);
			nap(2);
			oldfakecurs=fakecurs;
			fakecurs=add(fakecurs, div(sub(realcurs, midlayer), 2));
			if(!eqpt(fakecurs, oldfakecurs)){
				PenPoint(oldfakecurs);
				if(button1())
					PenTrack(oldfakecurs, fakecurs);
				PenPoint(fakecurs);
			}
		}
		if(button3()){
			if(drawing)
				cursallow();
			switch(menuhit(&menu, 3)){
			case DRAW_STOP:
				if(drawing){
					menutext[0]="draw";
					drawing=0;
				}
				else{
					/* reshape ... */
					midlayer=div(add(Drect.origin,
							Drect.corner), 2);
					menutext[0]="stop";
					drawing = 1;
				}
				break;
			case CLEANUP:
				CleanUp();
				break;
			case EXIT:
				exit();
			}
			if(drawing)
				cursinhibit();
		}
	}
}
