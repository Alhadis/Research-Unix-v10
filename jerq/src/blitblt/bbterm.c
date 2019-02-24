#include <jerq.h>
#include <font.h>

#define	UP	0
#define	DOWN	1
#define ZOMBIE	1024
#define NCHFIL	40

#define OUTER	1
#define INNER	2
#define Border(b, r)	border(b, inset(r, OUTER), INNER, F_XOR)

#define drstore(r)	rectf(&display, r, F_STORE)
#define drflip(r)	rectf(&display, r, F_XOR)
#define drclr(r)	rectf(&display, r, F_CLR)
#define drstring(s,p)	string(&defont, s, &display, p, F_XOR)

extern Texture menu3, deadmouse;

char filnam[NCHFIL]="BLITBLT", ppgm[NCHFIL];

char *top_menu[]={
	"choose layer",
	"layer interior",
	"flip border",
	"reverse video",
	NULL,
	"sweep rectangle",
	"layer rectangle",
	"whole screen (!)",
	"write file",
	ppgm,
	"exit",
	NULL
};
Menu topmenu={ top_menu };

Rectangle kbdrect(); Point kbdp; int fail;

Bitmap blit, *bp;
struct Proc *proc, *Getproc(); Rectangle rect;

main()
{
	register m; register char *p;
	struct Proc *pd;
	request(RCV|KBD|MOUSE);
	cursswitch(&deadmouse);
	blit.base=addr(&display,Pt(0,0));
	blit.width=display.width;
	blit.rect=Jrect;

	while (rcvchar() != 255) wait(RCV);
	p = ppgm;
	*p++ = '|'; *p++ = ' ';
	do {
		wait(RCV); *p = rcvchar();
	} while (*p++);
	cursswitch(&menu3);
	checkshape(1);

	for (;;) {
		if (buttons(DOWN) != 3) continue;

		top_menu[4] = (proc == 0)     ? "[ run/halt ]" :
			       proc->state&ZOMBIE ? "run" : "halt";

		cursswitch(NULL);
		switch (m = menuhit(&topmenu,3)) {
		case 0:
		case 1:
			if (pd = Getproc())
				flash(pd, (m ? pd->rect : pd->layer->rect));
			break;
		case 2:
			if (proc)
				Border(proc->layer, proc->layer->rect);
			else if (bp)
				Border(bp, rect);
			break;
		case 3:
			if (proc)
				rectf(proc->layer, proc->rect, F_XOR);
			else if (bp)
				rectf(bp, rect, F_XOR);
			break;
		case 4:
			if (proc) proc->state ^= ZOMBIE;
			break;
		case 5:
			flash(NULL, getrect3());
			break;
		case 6:
			if (pd = Getproc())
				flash(NULL, pd->layer->rect);
			break;
		case 7:
			flash(NULL, Jrect);
			break;
		case 8:
			getfilnam();
			sendrect(filnam);
			break;
		case 9:
			sendrect(ppgm);
			break;
		case 10:
			if (!lexit3()) break;
			sendctl(0);
			alarm(60*60);
			wait(ALARM);
			exit();
		}
		cursswitch(&menu3);
	}
}

sendrect(fp)
char *fp;
{
	fail = -1;
	cursswitch(&deadmouse);
	drstore(kbdrect());
	visible(0);
	if (proc)
		fail = sendbitmap((Bitmap *)(proc->layer), rect, fp);
	else if (bp)
		fail = sendbitmap(bp, rect, fp);
	visible(1);
	drstore(kbdrect());
	switch (fail) {
	case -1:
		drstring("No selection", kbdp); break;
	case 0:
		drstring(fp, drstring("Wrote ", kbdp)); break;
	default:
		drstring("Write failed", kbdp); break;
	}
}

getfilnam()
{
	Point p; char str[NCHFIL];
	drstore(kbdrect());
	p=drstring("File (",kbdp); p=drstring(filnam,p); p=drstring("): ",p);

	if (kbdstring(str,NCHFIL,p) > 0) strcpy(filnam,str);
}
