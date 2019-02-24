#include <jerq.h>
#include <font.h>

unsigned short delta[] = {
	0x0001, 0x0100, 0x0101, 0x00ff, 0xff00, 0xffff, 0xff01, 0xfeff
};

char	magic[] = {
	0, 0, 0, 1, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0
};

char	*setment[] = {
	"run",
	"clear",
	"read file",
	"write file",
	"exit",
	0
};

Menu	setmen = { setment };

char	*runment[] = {
	"setup",
	"stop",
	"slow",
	"exit",
	0
};

Menu	runmen = { runment };

unsigned long	*hash;
unsigned short	list[512];
unsigned short	*lend;
unsigned short	olist[512];
unsigned short	*olend;
unsigned long	*hlist[768];
unsigned long	**hlistp;
Bitmap	*discmap;
Bitmap	*circmap;
Point	setpt();
unsigned short getpt();
int	stopmode;
int	slowmode;
int	stepcount;

main()
{
	register i;
	register unsigned short *lp;

	olend = olist;
	if ((discmap = balloc(Rect(0, 0, 16, 16))) == (Bitmap *)NULL)
		exit();
	if ((circmap = balloc(Rect(0, 0, 16, 16))) == (Bitmap *)NULL)
		exit();
	rectf(discmap, discmap->rect, F_CLR);
	disc(discmap, Pt(7, 7), 7, F_XOR);
	rectf(circmap, circmap->rect, F_CLR);
	circle(circmap, Pt(7, 7), 2, F_XOR);
	request(KBD|MOUSE|RCV);
	if ((hash = (unsigned long *)gcalloc(sizeof(long)*8192L,
	   (char **)&hash)) == (unsigned long *)NULL)
		exit();
	setup();
	while (kbdchar() != 'q') {
		if (own()&MOUSE && button1())
			stepcount = 1;
		if (own()&MOUSE && button2()) {
			switch(menuhit(&runmen, 2)) {
			case 0:			/* setup */
				setup();
				continue;
			case 1:			/* stop/go */
				runment[1] = stopmode? "stop":"go";
				stopmode ^= 01;
				break;

			case 2:			/* fast/slow */
				runment[2] = slowmode? "slow":"fast";
				slowmode ^= 01;
				break;

			case 3:			/* exit */
				exit();
			}
		}
		if (stopmode && stepcount == 0) {
			wait(CPU);
			continue;
		}
		if (stepcount > 0)
			stepcount--;
		if (slowmode)
			sleep(30);
		gendisplay();
		genhash();
		genlist();
		wait(CPU);
	}
}

genhash()
{
	register unsigned short *lp;
	register unsigned long *hp, vl;
	register short unsigned i, v, vn;

	hlistp = hlist;
	for (lp = list; lp < lend; ) {
		v = *lp++;
		hp = &hash[v>>3];
		if (*hp == 0)
			*hlistp++ = hp;
		*hp |= 0x8L << ((v&07) << 2);
		for (i=0; i<8; i++) {
			vn = v + delta[i];
			hp = &hash[vn>>3];
			if (*hp == 0)
				*hlistp++ = hp;
			vl = 0x4L << ((vn&07) << 2);
			if ((*hp & vl) == 0) {
				vl >>= 2;
				*hp += vl;
			}
		}
	}
	*hlistp = 0;
}

genlist()
{
	register unsigned long h;
	register i, j;
	register unsigned short *lp;
	register unsigned long *hp;

	lp = lend = list;
	hlistp = hlist;
	while (hp = *hlistp++) {
		h = *hp;
		i = hp - hash;
		for (j = 0; h != 0; j++) {
			if (magic[h&0xf] /* && not at side edge */ )
				*lp++ = (i<<3) + j;
			h >>= 4;
		}
		*hp = 0;
	}
	lend = lp;
}

gendisplay()
{
	register x, y;
	register unsigned short *lp;
	Point orig;
	static Texture black={
		0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,
		0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,	0xFFFF,
	};

	rectf(&display, display.rect, F_STORE);
	orig = div(add(Drect.origin, Drect.corner), 2);
	for (lp = list; lp < lend; ) {
		x = ((*lp >> 8) - 0x80) << 2;
		y = ((*lp++ & 0xff) - 0x80) << 2;
		texture(&display, Rpt(add(orig, Pt(x,y)),
		            add(orig, Pt(x+2,y+2))), &black, F_XOR);
	}
}

setup()
{
	register i, j;
	register unsigned short *lp;

	for (i = 0; i < 8192; i++)
		hash[i] = 0;
	cleanset();
    more:
	while(wait(CPU|MOUSE) && button1())
		;
	for (;; wait(MOUSE)) {
		if (button1()) {
			unsigned short p;
			p = getpt();
			for (lp = olist; lp < olend; lp++) {
				if (*lp == p) {
					while (lp < olend-1) {
						*lp = *(lp+1);
						lp++;
					}
					dispt(circmap, p);
					olend--;
					goto more;
				}
			}
			*olend++ = p;
			dispt(discmap, p);
			goto more;
		}
		if (button2()) {
			switch(menuhit(&setmen, 2)) {

			case 0:	/* run */
				break;

			case 1:	/* clear */
				olend = olist;
				cleanset();
				goto more;

			case 2:	/* read file */
				if (getfile('r') == 0)
					goto more;
				olend = olist;
				cleanset();
				for (;;) {
					int x, y;
					if (getnum(&x)==0)
						break;
					if (getnum(&y)==0)
						break;
					x += 0x80;
					y += 0x80;
					*olend = (x<<8) | (y & 0xff);
					dispt(discmap, *olend++);
				}
				goto more;

			case 3:	/* write file */
				if (getfile('w') == 0)
					goto more;
				for (lp = olist; lp < olend; lp++) {
					putnum((*lp>>8)-0x80);
					sendchar(' ');
					putnum((*lp&0xFF)-0x80);
					sendchar('\n');
				}
				sendchar('X');
				goto more;

			case 4:	/* exit */
				exit();

			default:
				continue;
			}
			break;
		}
	}
	for (lp = olist, lend = list; lp < olend; )
		*lend++ = *lp++;
}

Point
setpt(p)
unsigned short p;
{
	Point pt;
	register x, y;

	pt = add(Drect.origin,mul(div(sub(Drect.corner,Drect.origin),32),16));
	x = ((p>>8) - 0x80) << 4;
	y = ((p & 0xff) - 0x80) << 4;
	return(add(pt, Pt(x,y)));
}

dispt(map, p)
register struct Bitmap *map;
unsigned short p;
{
	trimblt(map, map->rect, setpt(p), F_STORE);
}

unsigned short
getpt()
{
	register Point sp, o;

	sp = div(sub(mouse.xy, Drect.origin), 16);
	o = div(sub(Drect.corner,Drect.origin),32);
	sp = sub(sp, o);
	return(((0x80+sp.x)<<8) + ((0x80+sp.y) & 0xff));
}

cleanset()
{
	register unsigned short *lp;
	register i, j;

	for (i=Drect.origin.x; i < Drect.corner.x; i += 16)
		trimblt(circmap, circmap->rect,
		     Pt(i, Drect.origin.y), F_STORE);
	for (j=Drect.origin.y+16; j < Drect.corner.y; j += 16)
		trimblt(&display,
		    Rpt(Drect.origin, Pt(Drect.corner.x, Drect.origin.y+16)),
		    Pt(Drect.origin.x, j), F_STORE);
	for (lp = olist; lp <olend; lp++)
		dispt(discmap, *lp);
}

getnum(np)
register int *np;
{
	register n=0, c, got=0, neg=0;

	for (;;) {
		wait(RCV);
		c = rcvchar();
		if (c==' ' || c=='\t' || c=='\n') {
			if (got) {
				if (neg)
					n = -n;
				*np = n;
				return(1);
			}
		} else if (c>='0' && c<='9') {
			n = n*10 + c - '0';
			got = 1;
		} else if (c=='-')
			neg = 1;
		else {
			if (c=='N') {
				rectf(&display, Drect, F_CLR);
				string(&defont, "Not a life file", &display,
				    add(display.rect.origin, Pt(20,20)), F_XOR);
				sleep(120);
				cleanset();
			}
			return(0);
		}
	}
}

getfile(rw)
{
	char file[128];
	register char *fp = file;
	register c;
	Point fs;
	int strw = strwidth(&defont, "x");

	fp = file;
    redraw:
	while (kbdchar() >= 0)
		;
	while (rcvchar() >= 0)
		;
	rectf(&display, Drect, F_CLR);
	fs = string(&defont, "File: ", &display,
		 add(display.rect.origin, Pt(20,20)), F_XOR);
	for (;;) {
		wait(KBD);
		c = kbdchar();
		switch(c) {

		case -1:
			return(0);

		case '\n':
		case '\r':
			break;

		case '\b':
			if (fp > file) {
				*fp = '\0';
				fp--;
				string(&defont, fp, &display,
				  sub(fs, Pt(strw, 0)), F_XOR);
				fs = sub(fs, Pt(strw, 0));
			}
			continue;

		case '@':
			fp = file;
			goto redraw;

		default:
			*fp = c;
			fp[1] = '\0';
			fs = string(&defont, fp++, &display, fs, F_XOR);
			continue;
		}
		break;
	}
	*fp++ = '\0';
	sendchar(rw);
	fp = file;
	while (*fp)
		sendchar(*fp++);
	sendchar('\n');
	while ((wait(RCV)&RCV) == 0)
		;
	c = rcvchar();
	if (c == 'y') {
		cleanset();
		return(1);
	}
	rectf(&display, Drect, F_CLR);
	fs = string(&defont, "Can't open it.", &display,
		add(display.rect.origin, Pt(20,20)), F_XOR);
	sleep(120);
	cleanset();
	return(0);
}

putnum(x)
register x;
{
	register t;

	if (x < 0) {
		sendchar('-');
		x = -x;
	}
	if ((t = x/10) > 0)
		putnum(t);
	sendchar(x%10 + '0');
}

trimblt(sb, r, p, f)
Bitmap *sb;
Rectangle r;
Point p;
Code f;
{
	Rectangle nr;
	int v;
	
	nr = raddp(r, sub(p, r.origin));
	v = rectclip(&nr, Drect);
	if (v) {
		nr = rsubp(nr, sub(p, r.origin));
		bitblt(sb, nr, &display, p, f);
	}
}
