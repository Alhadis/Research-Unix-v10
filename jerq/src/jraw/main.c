#include "jraw.h"
#include "text.h"
#include "wire.h"
#include "box.h"
#include "inst.h"
#include "menu.h"

extern void _exit(int);
List *dl,*save;
Dict *ml;
Point ocp = Point(-10,-10);
Point cp = Point(-10,-10);		/* cursor point */
Point dp = Point(0,0);			/* delta cp */
Point dotp = Point(-10,-10);
Point origin = Point(0,0);		/* ratcheted rect.origin */
Point offset = Point(0,0);		/* relative to layer */
int moved;
#define GRID	8
Point ZERO = Point(0,0);
extern Word _cross[] = {
	0x80800000, 0x41000000, 0x22000000, 0x14000000, 0x08000000,
	0x14000000, 0x22000000, 0x41000000, 0x80800000
};
Word _ortho[] = {
	0x08000000, 0x08000000, 0x08000000, 0x00000000, 0xe3800000,
	0x00000000, 0x08000000, 0x08000000, 0x08000000
};
Word _box[] = {
	0xff800000, 0x80800000, 0x80800000, 0x80800000, 0x80800000,
	0x80800000, 0x80800000, 0x80800000, 0xff800000
};
Word _dot[] = {
	0xe0000000, 0xa0000000, 0xe0000000
};
Word _dots[] = {
	0x9c800000, 0x00000000, 0x00000000, 0x9c800000, 0x9c800000,
	0x9c800000, 0x00000000, 0x00000000, 0x9c800000
};
extern Bitmap icross;
Bitmap icross = Bitmap(_cross, 1, Rectangle(0, 0, 9, 9));
Bitmap iortho = Bitmap(_ortho, 1, Rectangle(0, 0, 9, 9));
Bitmap ibox = Bitmap(_box, 1, Rectangle(0, 0, 9, 9));
Bitmap dot = Bitmap(_dot, 1, Rectangle(0, 0, 3, 3));
Bitmap idots = Bitmap(_dots, 1, Rectangle(0, 0, 9, 9));
extern Bitmap *icon = &icross;
Texture bullseye = {
	 0x07E0, 0x1FF8, 0x399C, 0x63C6, 0x6FF6, 0xCDB3, 0xD99B, 0xFFFF,
	 0xFFFF, 0xD99B, 0xCDB3, 0x6FF6, 0x63C6, 0x399C, 0x1FF8, 0x07E0,
};
Texture boxcurs = {
	0x43FF, 0xE001, 0x7001, 0x3801, 0x1D01, 0x0F01, 0x8701, 0x8F01,
	0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0x8001, 0xFFFF,
};

drawicon()
{
	bitblt(icon,icon->rect,&display,ocp-Point(4,4),F_XOR);
	return 1;
}

fixit()
{
	cp = mouse.xy;
	cp.x -= cp.x % GRID;
	cp.y -= cp.y % GRID;
	dp = cp - ocp;
	if (dp != ZERO) {
		drawicon();
		ocp = cp;
		moved = 1;
		drawicon();
		return 1;
	}
	return 0;
}

extern void drawdot() {bitblt(&dot,dot.rect,&display,dotp-Point(1,1),F_XOR);}

void newicon(Bitmap *b)
{
	bitblt(icon,icon->rect,&display,ocp-Point(4,4),F_XOR);
	icon = b;
	bitblt(icon,icon->rect,&display,ocp-Point(4,4),F_XOR);
}

void append(List **p, List *x)
{
	register List *l = *p;
	if (l == 0)
		*p = x;
	else {
		while (l->next != 0)
			l = l->next;
		l->next = x;
	}
}
	
int ptype=WIRE;

typing(int c)
{
	Hit h;
	register List *l;
	unselect(ALL);
	h = textcode(dotp);
	if ((h.type & LABHIT) == 0)	/* make a new one */
		append(&dl,new Text(dotp,h.f,h.g));
	for (l = dl; l != 0; l = l->next)
		if (l->type(c))
			break;
	unselect(ALL);
	return 1;
}

dobut1()
{
	int code,hits;
	drawdot();
	unselect(ALL);
	code = selpt(cp);
	hits = code&ALLHIT;
	if (code == 0 || moved == 0) {
		if (code != 0 && moved == 0)
			unselect(ALL);
		switch (ptype) {
		case WIRE:
			append(&dl,new Wire(cp));
			break;
		case ORTHO:
			append(&dl,new Ortho(cp));
			break;
		case BOX:
			append(&dl,new Box(cp));
			break;
		case MACRO:
			append(&dl,new Macro(cp));
			break;
/* rip, cut
		case DOTS:
			append(&dl,new Dots(cp));
			break;
		case MEASURE:
			append(&dl,new Measure(cp));
			break;
 */
		}
	}
	else if ((hits & BOXHIT) && hits != BOXHIT)
		unselect(BOXHIT|ALLBOX);
	else if (code == (WIREHIT|WIREBOX|LABHIT|LABBOX))
		unselect(WIREHIT|WIREBOX|ALLBOX);
	movem(4);
	moved = 0;
	dotp = cp;
	drawdot();
	snapsel();
	ptype = WIRE;
	newicon(&icross);
	return 1;
}

dobut2()
{
	register List *l;
	Point p,q;
	Rectangle r;
	Master *m;
	Inst *in;
	List *odl;
	extern void drawlist(List *, Bitmap *, int);
	extern NMitem toplevel[];
	cursallow();
	switch (meanyhit(2)) {
	case WIRE:
		ptype = WIRE;
		newicon(&icross);
/* for toggling
		toplevel[0].text = "boxes";
		toplevel[0].result = BOX;
 */
		break;
	case ORTHO:
		ptype = ORTHO;
		newicon(&iortho);
		break;
	case BOX:
		ptype = BOX;
		newicon(&ibox);
/* for toggling
		toplevel[0].text = "lines";
		toplevel[0].result = WIRE;
 */
		break;
	case MACRO:
		ptype = MACRO;
		newicon(&ibox);
		break;
/* rip, rip
	case DOTS:
		ptype = DOTS;
		newicon(&idots);
		break;
	case MEASURE:
		ptype = MEASURE;
		newicon(&ibox);
		break;
 */
	case MASTER:
		unselect(ALL);
		selbox(getrect(1));
		fixit();
		if ((l = cut(&dl)) != 0) {
			drawlist(l,0,F_XOR);		/* undraw the list */
			m = new Master(l);
			*ml += m->s;
			(*ml)[m->s] = m;
			p = m->b->rect.o;
			p.x &= ~7;
			p.y &= ~7;
			append(&dl,in = new Inst(m,p));
			in->draw(&display,F_XOR);
		}
		break;
	case ENTER:
		odl = dl;
		drawlist(dl,0,F_XOR);
		for (l = dl; l != 0; l = l->next)
			if (l->enter())
				break;
		dl = odl;
		drawlist(dl,0,F_XOR);
		break;
	case EXIT:
		return 0;
		break;
	case INST:
		unselect(ALL);
		if ((m = (Master *) (*ml)[instname]) != 0) {
			append(&dl,l = (List *) new Inst(m,cp));
			l->draw(&display,F_XOR);	/* yecch */
			movem(0);
			while (button123())
				;
		}
		break;
	case SWEEP:
		unselect(ALL);
		selbox(getrect(1));
		fixit();
		movem(0);
		snapsel();
		break;
	case CUT:
		if ((l = cut(&dl)) != 0) {
			drawlist(l,0,F_XOR);
			removelist(save);
			snapsel();
			save = l;
		}
		break;
	case PASTE:
		unselect(ALL);
		if ((l = copy(save)) != 0) {
			drawlist(l,0,F_XOR);
			dl = l->append(dl);
		}
		movem(0);
		snapsel();
		while (button123())
			;
		break;
	case SNARF:
		if ((l = copy(dl)) != 0) {
			removelist(save);
			save = l;
		}
		break;
	case SCROLL:
		scroll();
		break;
	}
	cursinhibit();
	return 1;
}

char *comm_text[] = {"edit", "write", "read", "exit", 0};
Menu commands = {comm_text};
char filename[80];

char complaint[80];
complain(char *s, char *t)
{
	sprintf(complaint,s,t);
	kbdstr(complaint);
	return 0;
}

infile(char *name)
{
	FILE *f;
	if ((f = fopen(name,"r")) == 0)
		return complain("can't open %s",name);
	drawlist(dl,0,F_XOR);
	offset = origin;
	clearout();
	input(f);
	fclose(f);
	return 1;
}

dobut3()
{
	FILE *f;
	char buf[80];
	List *l;
	cursallow();
	switch (menuhit(&commands,3)) {
	case 0:
		kbdstr(filename);
		if (*filename == 0) return 0;
		infile(filename);
		mbblist(dl);
		ml->init();
		break;
	case 1:
		kbdstr(filename);
		if (*filename == 0) return 0;
		if ((f = fopen(filename,"w")) == 0)
			return complain("can't open %s",filename);
		if (output(f) == 0)
			return complain("can't write %s",filename);
		fclose(f);
		break;
	case 2:
		*buf = 0;
		kbdstr(buf);
		if (*buf == 0) return 0;
		if ((f = fopen(buf,"r")) == 0)
			return complain("can't open %s",buf);
		l = dl;
		if (input(f)) {
			dl->append(l);
			mbblist(dl);
			ml->init();
			movem(0);
		}
		else
			dl = l;
		fclose(f);
		break;
	case 3:
		_exit(0);
		exit();
	}
	cursinhibit();
	return 1;
}
struct pcb {
	int	psw;
	char	(*pc)();
	int	sp;
	int	lower;
	int	upper;
	int	ap;
	int	fp;
	int	reg[9];		/* 9 user registers */
	int	blkmv;
};
struct myProc {
	struct pcb pcb;
	char	*text;
	char	*data;
	char	*bss;
	int	state;
};
#define	uP	(*((struct myProc	**)0x0071e700))
#define	MOVED		256	/* layer got moved */
#define	RESHAPED	2048	/* layer got reshaped */
List *editlist(List *l)
{
	int c;
	Point delta;
	dl = l;
	while (1) {
		wait(MOUSE);
		if (uP->state & (RESHAPED|MOVED)) {
			if (uP->state & MOVED)
				rectf(&display,Drect,F_CLR);
			uP->state &= ~(RESHAPED|MOVED);
			delta = Drect.o & ~7;
			delta -= origin;
			offset += delta;
			origin += delta;
			displacelist(dl,delta);
			drawdot();
			drawlist(dl,&display,F_XOR);
			drawicon();
			continue;
		}
		fixit();
		if ((c=kbdchar()) != -1)
			typing(c);
		if (button1())
			dobut1();
		else if (button2()) {
			if (dobut2() == 0)
				break;
		}
		else if (button3()) {
			dobut3();
		}
	}
	return dl;
}

main(int argc, char *argv[])
{
	void _STImain_c_();
	request(KBD|MOUSE|SEND|RCV);
	_STImain_c_();
	ml = new Dict(40);
	cursinhibit();
	origin = Drect.o & ~7;
	offset = origin;
	if (argc > 1 && infile(argv[1]))
		editlist(dl);
	else
		editlist(new List);
}
