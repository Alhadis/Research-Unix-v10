#include <jerq.h>
#include <font.h>
#include <signal.h>
#include "thinkblt.h"

#define	UP	0
#define	DOWN	1
#define ZOMBIE	1024

#define RNULL	Rect(0, 0, 0, 0)

#define OUTER	1
#define INNER	2
#define Border(b, r)	(b ? border(b, inset(r, OUTER), INNER, F_XOR) : 0)
#define Rectf(b, r, f)	(b ? (rectf(b, r,f),0) : 0)

#define drstore(r)	rectf(&display, r, F_STORE)
#define drflip(r)	rectf(&display, r, F_XOR)
#define drclr(r)	rectf(&display, r, F_CLR)
#define drstring(s,p)	string(&defont, s, &display, p, F_XOR)

#define SIZEOF(a)	(sizeof(a)/sizeof *(a))

#define Rcvchar(c)	((c = rcvchar()) >= 0 ? c : (wait(RCV), c = rcvchar()))

typedef struct String{
	char *s;	/* pointer to string */
	short n;	/* number used */
	short size;	/* size of allocated area */
} String;

extern Texture menu3, menu23, deadmouse;

char *top_menu[] = {
	"choose layer",
	"layer interior",
	"flip border",
	"reverse video",
	NULL,
	"sweep rectangle",
	"layer rectangle",
	"whole screen (!)",
	"print bitmap",
	"print mux buffer",
	"reset printer",
	"exit",
	NULL
};
enum {
	Ch_layer, Ch_layint, Flip, Revid, Run_halt, Sweep, Lrect, Screen,
	Pr_bitmap, Pr_mux, Reset, Exit,
	Pr_host,
};
Menu topmenu={ top_menu };

struct HPesc { char *title, *seq; };
struct HPstate { int state, nstates; struct HPesc *esc; };
struct HPesc b2_pitch[] = {
	{ "80 columns", "\33&k0S" }, { "142 columns", "\33&k2S" },
	{ "40 columns", "\33&k1S" }, { "71 columns", "\33&k3S" },
};
struct HPesc b2_bold[] = {
	{ "bold off", "\17" }, { "bold on", "\16" },
};
struct HPesc b2_space[] = {
	{ "6 lines/in", "\33&l6D" }, { "8 lines/in", "\33&l8D" },
};
struct HPesc b2_wrap[] = {
	{ "wrap off", "\33&s1C" }, { "wrap on", "\33&s0C" },
};
struct HPesc b2_disp[] = {
	{ "display off", "\33Z" }, { "display on", "\33Y" },
};
struct HPesc b2_eject[] = {
	{ "page eject", "\14" },
};
struct HPesc b2_eject3[] = {
	{ "1/3 page eject", "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" },
};
struct HPesc b2_test[] = {
	{ "self test", "\33z" },
};
struct HPstate b2_states[] = {
	{ 0, 4, b2_pitch },
	{ 0, 2, b2_bold },
	{ 0, 2, b2_space },
	{ 0, 2, b2_wrap },
	{ 0, 2, b2_disp },
	{ 0, 1, b2_eject },
	{ 0, 1, b2_eject3 },
	{ 0, 1, b2_test },
};
char *b2_gen(), *b2_select();
Menu b2menu={ 0, b2_gen };

char *busy_menu[] = {
	"pause",
	"abort print",
	NULL
};
Menu busymenu={ busy_menu };

char *pmsg[] = {
	"No selection",
	"Print done",
	"Print aborted",
};

char *rmsg[] = {
	"Reset done",
	"Reset failed",
};

Rectangle kbdrect(); Point kbdp;

int hoststate, fail, quitfun(), printch(), iprtchar, recvch(), nchrecv;

Bitmap blit, *bp; String printstr;

struct Proc *proc, *Getproc(); Rectangle rect;

main(argc, argv)
char **argv;
{
	Initialize(argv);
	while (wait(MOUSE|RCV|KBD)) {
		while (own()&KBD)
			kbdchar();
		while (own()&RCV) switch(rcvchar()) {
		default:
			hoststate=0; break;
		case 255:
			if (++hoststate == 4)
				hoststate=0, do_print(Pr_host);
			break;
		}
		if ((own()&MOUSE) && ptinrect(mouse.xy, Drect)) {
			if (button2())
				do_menu2();
			if (button3() && do_menu3() < 0)
				break;
		}
	}
	End();
	exit();
}

do_print(what)
{
	fail = -1;
	drstore(kbdrect());
	cursswitch(&menu3);
	switch (what) {
	case Pr_host:
		nchrecv = 0;
		if (fail = thinkprint(recvch))
			kill(SIGTERM);
		break;
	case Pr_bitmap:
		visible(0);
		fail = bp ? thinkmap(bp, rect) : -1;
		visible(1);
		break;
	case Pr_mux:
		getmuxbuf(&printstr);
		iprtchar = 0;
		fail = printstr.n > 0 ? thinkprint(printch) : -1;
		break;
	}
	drstore(kbdrect());
	drstring(pmsg[fail+1], kbdp);
}

do_menu3()
{
	register m; char *p;
	register struct Proc *pd;

	top_menu[Run_halt] = (proc == 0) ? "[ run/halt ]" :
				proc->state&ZOMBIE ? "run" : "halt";

	cursswitch(NULL);
	switch (m = menuhit(&topmenu,3)) {
	case Ch_layer:
	case Ch_layint:
		if (pd = Getproc())
			flash(pd, (m==Ch_layer?pd->layer->rect:pd->rect));
		break;
	case Flip:
		Border(bp, rect); break;
	case Revid:
		Rectf(bp, rect, F_XOR); break;
	case Run_halt:
		if (proc) proc->state ^= ZOMBIE; break;
	case Sweep:
		flash(NULL, getrect3()); break;
	case Lrect:
		if (pd = Getproc())
			flash(NULL, pd->layer->rect);
		break;
	case Screen:
		flash(NULL, Jrect); break;
	case Pr_bitmap:
	case Pr_mux:
		do_print(m);
		break;
	case Reset:
		drstore(kbdrect());
		fail = 0;
		m = 160;
		while (--m >= 0)
			thinkchar(0);
		thinknchars(2, "\033E");
		thinkflush();
		drstore(kbdrect());
		drstring(rmsg[fail], kbdp);
		break;
	case Exit:
		if (!lexit3()) break;
		thinkstop();
		return -1;
	}
	cursswitch(&menu23);
	return 0;
}

do_menu2()
{
	register char *p;
	cursswitch(NULL);
	p = b2_select(menuhit(&b2menu,2));
	if (p) {
		while (*p)
			thinkchar(*p++);
		thinkflush();
	}
	cursswitch(&menu23);
}

char *
b2_gen(i)
register i;
{
	register struct HPstate *p;
	if (i < 0 || i >= SIZEOF(b2_states))
		return NULL;
	p = b2_states + i;
	i = p->state;
	if (p->nstates > 1)
		i = (i + 1)%p->nstates;
	return p->esc[i].title;
}

char *
b2_select(i)
{
	register struct HPstate *p;
	if (i < 0 || i >= SIZEOF(b2_states))
		return NULL;
	p = b2_states + i;
	if (p->nstates > 1)
		p->state = (p->state + 1)%p->nstates;
	return p->esc[p->state].seq;
}

printch()
{
	return (iprtchar < printstr.n) ? printstr.s[iprtchar++] : -1;
}

recvch()
{
	register c;
	if (fail > 0)
		return -1;
	if (--nchrecv < 0) {
		if (nchrecv < -1)
			return -1;
		nchrecv = Rcvchar(c) << 8;
		nchrecv |= Rcvchar(c);
		if (--nchrecv < 0)
			return -1;
	}
	return Rcvchar(c);
}

quitfun()
{
	int paused, m; Texture *prev;
	if (fail > 0)
		return 1;
	if (!button3() || !ptinrect(mouse.xy, Drect))
		return 0;
	paused = 0;
	for (;;) {
		prev = cursswitch(NULL);
		m = menuhit(&busymenu, 3);
		cursswitch(prev);
		switch (m) {
		case 1:
			return (fail = 1);
		case 0:
			paused = 1 - paused;
			busy_menu[0] = paused ? "continue" : "pause";
		default:
			if (!paused)
				return 0;
			while (!button3() || !ptinrect(mouse.xy, Drect))
				wait(CPU);
		}
	}
}

Initialize(argv)
char **argv;
{
	char linkname[64];
	register char *p = linkname; register c;
	cursswitch(&deadmouse);
	request(RCV | KBD | MOUSE);
	while (rcvchar() != 0)
		sleep(6);
	while (rcvchar() != 255)
		sleep(6);
	do {
		*p++ = Rcvchar(c);
	} while (c);
	cursswitch(&menu23);

	if (strcmp(argv[1], "-v0") == 0) {
		extern int thinkvec;
		thinkvec = 0;
	}

	drstore(kbdrect());
	drstring(linkname, kbdp);

	if (thinkstart(quitfun))
		End();
	thinkshare();
	blit.base=addr(&display,Pt(0,0));
	blit.width=display.width;
	blit.rect=Jrect;
}


End()
{
	static char msg[4] = { EXIT, };
	sendnchars(sizeof msg, msg);
	alarm(60*60);
	wait(ALARM);
}
