#define MAIN
#include "jf.h"

char *top_menu[]={
	"open/close font",
	"bit function",
	"shift / rotate",
	"set sizes",
	"snarf from screen",
	"exit",
	NULL
};
Menu topmenu={ top_menu };

main(argc, argv)
int argc; char *argv[];
{
	int b=0;

	cursswitch(&deadmouse);
	request(MOUSE|KBD); spaceman();

	while (--argc > 0) b |= readfile(*++argv);
	if (b) cursswitch(TNULL); else cursswitch(&menu3);

	for (;wait(MOUSE);sleep(2)) {

		if (reshaped()) spaceman();
		if (!ptinrect(mouse.xy,Drect)) continue;

		if (button12()) {
			b=button1()!=0;
			mousetrack();
			if (mtk.fdp != FDNULL) charopcl(mtk.fdp,mtk.c,b);
			else if (mtk.edp != EDNULL) edispset(mtk.edp,mtk.pxl,b);

		} else if (button3()) {
			switch (menuhit(&topmenu,3)) {
				case 0:
					filefunc();
					break;
				case 1:
					bitfunc();
					break;
				case 2:
					shiftfunc();
					break;
				case 3:
					sizefunc();
					break;
				case 4:
					snarfit();
					break;
				case 5:
					if (lexit3()) exit();
					break;
			}
			cursswitch(TNULL);
		}
	}
}

Texture empty = { 0 };

Bitmap blit;

snarfit()		/* get data from screen */
{
	int dstc; Point pcur, pprev;
	Rectangle rcur, rchar, charect();
	Font *dstfp; Fontdisp *dstfdp; Editdisp *dstedp = 0;

	blit.base=addr(&display,Pt(0,0));
	blit.width=display.width;
	blit.rect=Jrect;

	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); dstedp=mtk.edp; pcur=mouse.xy; }
	if (dstedp == 0) { buttons(UP); return 0; }

	dstc=dstedp->c; dstfdp=dstedp->fdp; dstfp=dstfdp->fp;
	rchar = charect(dstfp, dstc);
	rcur = raddp(rchar, sub(pcur, rchar.origin));
	cursswitch(&empty);
	rectf(&blit, rcur, F_XOR);
	for (pprev=pcur; button3(); nap(2)) {
		pcur=mouse.xy;
		if (!eqpt(pprev,pcur)) {
			rectf(&blit, rcur, F_XOR);
			rcur = raddp(rcur, sub(pcur, pprev));
			rectf(&blit, rcur, F_XOR);
			pprev=pcur;
		}
	}
	rectf(&blit, rcur, F_XOR);
	cursswitch(TNULL);

	bitblt(&blit, rcur, dstfp->bits, rchar.origin, F_STORE);
	fdispstr(dstfdp,dstc); fdispflp(dstfdp,dstc);
	editdisp(dstedp);
	return 1;
}
