#define	MPXTERM
#define MUXTERM
#include <jerq.h>
#include <layer.h>
#include <font.h>
#include "struct.h"

#define	XMARGIN	3	/* inset from border of layer */
#define	YMARGIN	3
#define	INSET	3
#define	BUFS	32
#define	HISTSIZ	4096	/* number of history characters */
#define	BLBUT	01	/* blocked by button */
#define	BLPAG	02	/* blocked by end of page */
#undef	NS
#define	NS	defont.height
#undef	CW
#define	CW	defont.info['0'].width

#undef	button123
#undef	button13
#ifndef	MUX
#define	button123()	(((struct udata *)P->fcn)->mouse.buttons & 07)
#define	button13()	(((struct udata *)P->fcn)->mouse.buttons & 05)
#else
#define	button123()	(((struct udata *)P->data)->mouse.buttons & 07)
#define	button13()	(((struct udata *)P->data)->mouse.buttons & 05)
#endif
#define	C_RESHAPE 8	/* likewise should be visible */

struct M {
	char	*menutext[9];
} m = {
	"backup",
	"forward",
	"reset",
	"clear",
	"24x80",
	"scroll",
#ifndef	MUX
	"new",
#endif
	0
};

struct User {
	short	x, y;	/* character positions */
	char	*backp;
	short	backc;
	short	atend;
	short	nbacklines;
	short	xmax, ymax;
	short	blocked;
	short	pagemode;
	short	olines;
	int	peekc;
	char	orig;
	Menu	menu;
	struct	M m;
	char	*histp;
	char	hist[HISTSIZ];
};

Point	pt();

unsigned rchar();

main()
{
	realmain(1);
}

#ifndef	MUX
newhp()
{
	realmain(0);
}
#endif

realmain(orig)
{
	char buf[BUFS+1];
	register struct Proc *p = P;
	register struct User *up;
	register n;
	register c;
	register standout = 0;
	register insmode = 0;

	crunchinit();

#ifndef	MUX
	if (!orig)
		p->fcn = (int(*)())alloc(sizeof(struct udata));
#endif
	p->state |= USER;	/* make mpxterm update mouse */
	if ((up = (struct User *)alloc(sizeof(struct User)))==(struct User *)NULL)
		exit();
	up->backc = 0;
	up->atend = 0;
	up->histp = up->hist;
	up->m = m;
	up->menu.item = up->m.menutext;
	up->orig = orig;
	up->pagemode = 1;
	request(RCV|KBD|SEND|MOUSE);
	p->state |= RESHAPED;
	for (;;) {
		if (up->x > up->xmax) {
			up->x = 0;
			newline(up);
		}
		buf[0] = getchar(up, 0);
		buf[1] = '\0';
		switch(buf[0]) {

		case '\007':		/* bell */
			ringbell();
			break;

		case '\t':		/* tab modulo 8 */
			up->x = (up->x|7)+1;
			break;

		case '\033':
			switch(getchar(up, 0)) {

			case 'j':
				getchar(up, 0);
				break;

			case '&':	/* position cursor &c */
				switch(getchar(up, 0)) {

				case 'a':
					for (;;) {
						n = number(up, buf);
						switch(buf[0]) {

						case 'r':
						case 'y':
							up->y = n;
							continue;

						case 'c':
							up->x = n;
							continue;

						case 'R':
						case 'Y':
							up->y = n;
							break;

						case 'C':
							up->x = n;
							break;
						}
						break;
					}
					break;

				case 'd':	/* underline stuff */
					if ((n=getchar(up, 0))>='A' && n <= 'O')
						standout++;
					else if (n == '@')
						standout = 0;
					break;

				default:
					getchar(up, 0);
					break;

				}
				break;

			case 'i':	/* back tab */
				if (up->x>0)
					up->x = (up->x-1) & ~07;
				break;

			case 'H':	/* home cursor */
			case 'h':
				up->x = 0;
				up->y = 0;
				break;

			case 'L':	/* insert blank line */
				scroll(up, up->y, up->ymax, up->y+1, up->y);
				break;

			case 'M':	/* delete line */
				scroll(up, up->y+1, up->ymax+1, up->y, up->ymax);
				break;

			case 'J':	/* clear to end of display */
				xtipple(up, Rpt(pt(0, up->y+1),
					    pt(up->xmax+1, up->ymax+1)));
				/* flow */
			case 'K':	/* clear to EOL */
				xtipple(up, Rpt(pt(up->x, up->y),
					    pt(up->xmax+1, up->y+1)));
				break;

			case 'P':	/* delete char */
				bitblt(p->layer, Rpt(pt(up->x+1, up->y),
						     pt(up->xmax+1, up->y+1)),
				   p->layer, pt(up->x, up->y), F_STORE);
				xtipple(up, Rpt(pt(up->xmax, up->y),
					    pt(up->xmax+1, up->y+1)));
				break;

			case 'Q':	/* enter insert mode */
				insmode++;
				break;

			case 'R':	/* leave insert mode */
				insmode = 0;
				break;

			case 'S':	/* roll up */
				scroll(up, 1, up->ymax+1, 0, up->ymax);
				break;

			case 'T':
				scroll(up, 0, up->ymax, 1, 0);
				break;

			case 'A':	/* upline */
			case 't':
				if (up->y>0)
					up->y--;
				if (up->olines > 0)
					up->olines--;
				break;

			case 'B':
			case 'w':
				up->y++;	/* downline */
				break;

			case 'C':	/* right */
			case 'v':
				up->x++;
				break;

			case 'D':	/* left */
			case 'u':
				up->x--;

			}
			break;

		case '\b':		/* backspace */
			if(up->x > 0)
				--up->x;
			break;

		case '\n':		/* linefeed */
			newline(up);
			standout = 0;
			break;

		case '\r':		/* carriage return */
			up->x = 0;
			standout = 0;
			break;

		default:		/* ordinary char */
			n = 1;
			c = 0;
			while (up->x+n<=up->xmax && n<BUFS
			    && (c = getchar(up, 1))>=' ' && c<'\177') {
				buf[n++] = c;
				c = 0;
			}
			buf[n] = 0;
			if (insmode) {
				bitblt(p->layer, Rpt(pt(up->x, up->y),
						     pt(up->xmax-n+1, up->y+1)),
				       p->layer, pt(up->x+n, up->y), F_STORE);
			}
			xtipple(up, Rpt(pt(up->x,up->y), pt(up->x+n, up->y+1)));
			string(&defont, buf, p->layer, pt(up->x, up->y), F_XOR);
			if (standout)
				rectf(p->layer, Rpt(pt(up->x,up->y),
						    pt(up->x+n,up->y+1)),F_XOR);
			up->x += n;
			up->peekc = c;
			break;
		}
	}
}

xtipple(up, r)
register struct User *up;
Rectangle r;
{
	rectclip(&r, P->rect);
	rectf(P->layer, r, F_CLR);
}

newline(up)
register struct User *up;
{
	up->nbacklines--;
	if(up->y >= up->ymax) {
		up->y = up->ymax;
		if (up->pagemode && up->olines >= up->ymax) {
			up->blocked |= BLPAG;
			return;
		}
		scroll(up, 1, up->ymax+1, 0, up->ymax);
	} else
		up->y++;
	up->olines++;
}

curse(up, bl)
register struct User *up;
{
	rectf(P->layer, Rpt(sub(pt(up->x, up->y), Pt(1,1)),
			    add(pt(up->x, up->y), Pt(CW,NS))), F_XOR);
	if (bl==0)
		return;
	rectf(P->layer, Rpt(pt(up->x, up->y),
		    add(pt(up->x, up->y), Pt(CW-1,NS-1))), F_XOR);
}

getchar(up, w)
register struct User *up;
{
	register c = up->peekc;

	up->peekc = 0;
	if (c > 0)
		return(c);
	while (c <= 0) {
		if (up->backp) {
			c = *up->backp;
			if (c && up->nbacklines >= 0) {
				up->backp++;
				if (up->backp >= &up->hist[HISTSIZ])
					up->backp = up->hist;
				return(c);
			}
			up->backp = 0;
		}
		if (w && !up->blocked) {   /* faster if don't want to block */
			c = rchar();
			if (c <= 0)
				return(c);
		} else
			c = waitchar(up);
	}
	*up->histp++ = c;
	if (up->histp >= &up->hist[HISTSIZ])
		up->histp = up->hist;
	*up->histp = '\0';
	return(c);
}

waitchar(up)
register struct User *up;
{
	register c, wcond;
	char echobuf[32];
	register char *ep;
	int blcur;

	for (;;) {
		if (P->state & RESHAPED)
			reshape(up);
		if (up->backp)
			return(0);
		if (button123() && (own()&MOUSE)) {
			if (button13()) {
				request(RCV|KBD|SEND);
				sleep(1);
				request(RCV|KBD|SEND|MOUSE);
				continue;
			}
			readmenu(up);
			continue;
		}
		if (!up->blocked && (c = rchar()) > 0)
			return(c);
		ep = echobuf;
		while ((c = kbdchar()) > 0 && ep < &echobuf[32-1]) {
			if (c & 0200) {
				c = transkbd(up, c);
				if (c == 0)
					continue;
				if (c & 0200)
					*ep++ = '\033';
				c &= 0177;
			}
			*ep++ = c;
			if (up->blocked&BLPAG)
				P->state |= RESHAPED;
			up->blocked = 0;
			up->olines = 0;
		}
		if (ep > echobuf)
			sendnchars((int)(ep-echobuf), echobuf);
		blcur = up->blocked;
		curse(up, blcur);
		wcond = KBD | (up->blocked? 0: RCV);
		do {
			wait(wcond | MOUSE);
		} while (!((own() & wcond) || button123()));
		curse(up, blcur);
	}
}

#define ESCA	0200	/* means send escape first */

struct keytab {
	unsigned char	key;
	unsigned char	host;
} keytab[]= {
#ifdef	mc68000
	0xf1, ESCA|'t',	/* up arrow */
	0xf2, ESCA|'w',	/* down arrow */
	0xf3, ESCA|'u',	/* left arrow */
	0xf4, ESCA|'v',	/* right arrow */
	0xe0, ESCA|'\177',	/* break */
	0xe1,      '7',	/* 7 */
	0xe2,      '8',	/* 8 */
	0xe3,      '9',	/* 9 */
	0xe4,      '-',	/* - */
	0xea, ESCA|'\177',	/* break */
	0xeb, ESCA|'\177',	/* break */
	0xec, ESCA|'\177',	/* break */
	0xd0,      '4',	/* 4 */
	0xd1,      '5',	/* 5 */
	0xd2,      '6',	/* 6 */
	0xd3,      ',',	/* , */
	0xc0,      '1',	/* 1 */
	0xc1,      '2',	/* 2 */
	0xc2,      '3',	/* 3 */
	0xc3,      '\r',/* enter */
	0xb1,      '0',	/* 0 */
	0xb2,      '.',	/* . */
	0xfe, ESCA|'w',	/* Setup */
	0xf6, ESCA|'x',	/* PF1 */
	0xf7, ESCA|'y',	/* PF2 */
	0xf8, ESCA|'z',	/* PF3 */
#else
	0xd2,	   '7',		
	0x97,	   '7',		
	0xd4,	   '8',
	0x98,	   '8',
	0xd6,	   '9',
	0x99,	   '9',
	0xd3,	   '4',
	0x94,	   '4',
	0xd5,	   '5',
	0x95,	   '5',
	0xd7,	   '6',
	0x96,	   '6',
	0xc0,	   '1',
	0x91,	   '1',
	0xc1,	   '2',
	0x92,	   '2',
	0xc6,	   '3',
	0x93,	   '3',
	0xc4,	   '-',
	0x9a,	   '-',
	0xc2,	   '0',
	0x90,	   '0',
	0xc3,	   '.',
	0x9b,	   '.',
#endif
	0
};

transkbd(up, c)
register struct User *up;
register c;
{
	register struct keytab *p;

	for (p = keytab; p->key; p++)
		if (c == p->key)
			return(p->host);
	/* flip blocked state on unknown key */
	if (up->blocked) {
		if (up->blocked&BLPAG) {
			P->state |= RESHAPED;
			up->olines = 0;
		}
		up->blocked = 0;
	} else
		up->blocked = BLBUT;
	return(0);
}

reshape(up)
register struct User *up;
{
	register struct Proc *p = P;

	p->state &= ~RESHAPED;
	up->xmax = (p->rect.corner.x-p->rect.origin.x-2*XMARGIN)/CW-1;
	up->ymax = (p->rect.corner.y-p->rect.origin.y-2*YMARGIN)/NS-1;
	up->x = 0;
	up->y = 0;
	xtipple(up, p->rect);
}

readmenu(up)
register struct User *up;
{
	register struct Proc *p = P;
	Rectangle r;
	register struct Layer *l;

	switch(menuhit(&up->menu, 2)) {

	case 0:		/* back up */
		if (up->atend == 0) {
			up->backc++;
			backup(up, up->backc);
		} else {
			rectf(p->layer, p->rect, F_XOR);
			rectf(p->layer, p->rect, F_XOR);
		}
		return;

	case 1:		/* move forward */
		up->backc--;
		if (up->backc >= 0)
			backup(up, up->backc);
		else {
			up->backc = 0;
			rectf(p->layer, p->rect, F_XOR);
			rectf(p->layer, p->rect, F_XOR);
		}
		return;

	case 2:		/* reset */
		up->backc = 0;
		backup(up, 0);
		return;

	case 3:		/* clear screen */
		p->state |= RESHAPED;
		return;

	case 4:		/* 24x80 */
		r.origin = p->layer->rect.origin;
		r.corner = add(p->layer->rect.origin,
			   Pt(80*CW+2*XMARGIN+2*INSET, 24*NS+2*YMARGIN+2*INSET));
		if (r.corner.x > (XMAX-5)) {
			r.origin.x -= r.corner.x - (XMAX-5);
			r.corner.x = (XMAX-5);
		}
		if (r.corner.y > (YMAX-5)) {
			r.origin.y -= r.corner.y - (YMAX-5);
			r.corner.y = (YMAX-5);
		}
		dellayer(p->layer);
		l = newlayer(r);
		if (l == 0) {
			r.corner = add(r.origin, Pt(100,50));
			l = newlayer(r);
			if (l == 0)
				exit();	/* The Oh Shit case */
		}
		p->layer = l;
		p->rect = r;
		rectf(p->layer, r, F_STORE);
		p->rect = inset(r, INSET);
		p->state |= RESHAPED;
#ifndef	MUX
		mpxnewwind(p, C_RESHAPE);
#else
		muxnewwind(p, C_RESHAPE);
#endif
		backup(up, 0);
		return;

	case 5:		/* pause and clear at end of screen */
		up->pagemode = 1-up->pagemode;
		up->menu.item[5] = up->pagemode? "scroll": "page";
		return;

#ifndef	MUX
	case 6:		/* new hp window */
		/*
		 * This dissociates the code space from all processes 
		 * so as to avoid having it freed when the 1st layer is deleted
		 */
		if (up->orig)
			((char **)P->fcn)[-1] = 0;
		newwindow(newhp);
		return;
#endif
	}
}

backup(up, count)
register struct User *up;
{
	register n;
	register char *cp;

	P->state |= RESHAPED;
	n = 3*(count+1)*up->ymax/4;
	cp = up->histp;
	up->atend = 0;
	while (n >= 0) {
		cp--;
		if (cp < up->hist)
			cp = &up->hist[HISTSIZ-1];
		if (*cp == '\0') {
			up->atend = 1;
			break;
		}
		if (*cp == '\n')
			n--;
	}
	cp++;
	if (cp >= &up->hist[HISTSIZ])
		cp = up->hist;
	up->backp = cp;
	up->nbacklines = up->ymax-2;
}

Point
pt(x, y)
register x, y;
{
	return add(P->rect.origin, Pt(x*CW+XMARGIN, y*NS+YMARGIN));
}

scroll(up, sy, ly, dy, cy)	/* source, limit, dest, which line to clear */
register struct User *up;
{
	bitblt(P->layer, Rpt(pt(0, sy), pt(up->xmax+1, ly)),
	    P->layer, pt(0, dy), F_STORE);
	xtipple(up, Rpt(pt(0, cy), pt(up->xmax+1, cy+1)));
}

number(up, p)
register struct User *up;
char *p;
{
	register n = 0;
	register c;

	while ((c = getchar(up, 0)) >= '0' && c <= '9')
		n = n*10 + c - '0';
	*p = c;
	return(n);
}

#ifdef	mc68000
ringbell()
{
	*((char *)(384*1024L+062)) = 2;
}
#endif

/*
 * Rchar behave much like rcvchar, but interposes
 * data decompression between rcvchar and its callers.
 */
unsigned
rchar()
{
	static struct strdesc sd;
	static short state = 0;
	register nodep curr;

	/* If we have chars left over from a previous call, return one. */
	if (sd.len) {
		sd.len--;
		return *sd.ptr++;
	}

	/*
	 * Tokens are 12-bit objects and characters are 8-bit objects.
	 * Here, we read enough characters to put together a token,
	 * which we leave in 'curr'.  If at any time during this process
	 * rcvchar returns -1, we have to save enough state to pick up
	 * again later.  Thus the arcane code.
	 */
	do {
		do {
			static int byte;
			register unsigned c;
			c = rcvchar();
			if (c == -1)
				return c;

			switch (state) {
			case 0:
				byte = c;
				break;
			case 1:
				curr = byte | c<<4 & 0xf00;
				byte = c & 0xf;
				break;
			case 2:
				curr = c | byte<<8;
				break;
			}

			if (++state == 3)
				state = 0;
		} while (state == 1);
	} while (curr == NULLNODE);

	/*
	 * Curr now has the token number.  Convert it to a string
	 * and update the dictionary appropriately.
	 */
	sd = nodestr (curr);
	extend (prevnode, curr);
	prevnode = curr;

	/*
	 * Since curr was not NULLNODE, it represents a non-empty string.
	 * Return the first character of the string.
	 */
	sd.len--;
	return *sd.ptr++;
}
