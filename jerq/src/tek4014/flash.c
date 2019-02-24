
#include <jerq.h>
#include <font.h>

#define SAVEC		64
#define SAVEL		32
#define SAVEP		128

#define NORMAL		0
#define DEFOCUSED	1
#define WRITETHRU	2

#define DISALLOWED	0
#define ALLOWED		1
#define FLASHED		2
#define PENDING		3
#define DIMMED		4

extern int flashmode, zaxis, F_DRAW;

extern Font *font[8];

typedef	struct saveC{
	char writethrU;
	Point poinT;
	int fonT;
	char chaR;
} saveC;

saveC savec[SAVEC], *savectop, *savecbot;


typedef	struct saveL{
	char writethrU;
	Point starT,enD,shifT;
	Word texturE;
} saveL;

saveL savel[SAVEL], *saveltop, *savelbot;

typedef	struct saveP{
	char writethrU;
	Point poinT;
} saveP;

saveP savep[SAVEP], *saveptop, *savepbot;

Bitmap *savecbits,*savepbits;

extern Word styles[];

extern Font tinyfont, bigaplfont, tinyaplfont;

int flash() {
/*	*((char *)(384*1024L+062)) = 3;  */
	flashchars();
	flashpoints();
	flashlines();
	return FLASHED;
}

unflash() {
	unflashlines();
	unflashpoints();
	unflashchars();
/*	*((char *)(384*1024L+062)) = 1;  */
}

flashchars() {
	Point dest,p,q;
	saveC *tmp = savecbot;

	dest.x = 0;
	dest.y = 0;
	q.x = 10;
	q.y = 18;
	
	while (tmp != savectop) {
		if (++tmp >= &savec[SAVEC])
			tmp = savec;
		p = sub(tmp->poinT,Pt(0,font[tmp->fonT]->ascent));
		bitblt(&display,Rpt(p,add(p,q)),savecbits,dest,F_STORE);
		dest.x += 10;
	}

	tmp = savecbot;
	while (tmp != savectop) {
		if (++tmp >= &savec[SAVEC])
			tmp = savec;
		if (flashmode)
			drawchar(tmp->fonT,tmp->chaR,add(tmp->poinT,Pt(0,1)),F_DRAW);
		if (tmp->writethrU)
			drawchar(tmp->fonT,tmp->chaR,tmp->poinT,F_DRAW);
		else
			drawchar(tmp->fonT,tmp->chaR,
				 add(tmp->poinT,Pt(1,0)),F_DRAW);
	}
}

flashpoints() {
	Point dest,p,q;
	saveP *tmp = savepbot;

	dest.x = 0;
	dest.y = 0;
	q.x = 2;
	q.y = 2;
	
	while (tmp != saveptop) {
		if (++tmp >= &savep[SAVEP])
			tmp = savep;
		p = tmp->poinT;
		bitblt(&display,Rpt(p,add(p,q)),savepbits,dest,F_STORE);
		dest.x += 2;
	}

	tmp = savepbot;
	while (tmp != saveptop) {
		if (++tmp >= &savep[SAVEP])
			tmp = savep;
		if (flashmode)
			point(&display,add(tmp->poinT,Pt(0,1)),F_DRAW);
		if (tmp->writethrU)
			point(&display,tmp->poinT,F_DRAW);
		else
			point(&display,add(tmp->poinT,Pt(1,0)),F_DRAW);
	}
}

flashlines() {
	saveL *tmp = savelbot;
	Point t;

	while (tmp != saveltop) {
		if (++tmp >= &savel[SAVEL])
			tmp = savel;
		t = sub(tmp->enD,tmp->starT);
		if (abs(t.y) > abs(t.x)) {
			tmp->shifT.x = 1; tmp->shifT.y = 0;
		} else {
			tmp->shifT.y = 0; tmp->shifT.y = 1;
		}
		dsegment(&display,add(tmp->starT,tmp->shifT),
			 add(tmp->enD,tmp->shifT),F_XOR,tmp->texturE);
		if (tmp->writethrU)
			dsegment(&display,tmp->starT,tmp->enD,F_XOR,tmp->texturE);
		else
			dsegment(&display,sub(tmp->starT,tmp->shifT),
			 	 sub(tmp->enD,tmp->shifT),F_XOR,tmp->texturE);
	}
}

unflashchars() {
	Point dest,p,q;
	saveC *tmp = savecbot;

	dest.x = 0;
	dest.y = 0;
	q.x = 10;
	q.y = 18;
	
	while (tmp != savectop) {
		if (++tmp >= &savec[SAVEC])
			tmp = savec;
		p = sub(tmp->poinT,Pt(0,font[tmp->fonT]->ascent));
		bitblt(savecbits,Rpt(dest,add(dest,q)),&display,p,F_STORE);
		dest.x += 10;
	}
	savecbot = savectop;
}

unflashpoints() {
	Point dest,p,q;
	saveP *tmp = savepbot;

	dest.x = 0;
	dest.y = 0;
	q.x = 2;
	q.y = 2;
	
	while (tmp != saveptop) {
		if (++tmp >= &savep[SAVEP])
			tmp = savep;
		p = tmp->poinT;
		bitblt(savepbits,Rpt(dest,add(dest,q)),&display,p,F_STORE);
		dest.x += 2;
	}
	savepbot = saveptop;
}

unflashlines() {
	flashlines();
	savelbot = saveltop;
}

savechar(f,c,p)
int f;
Point p;
char c;
{
	int wt = (zaxis == WRITETHRU);

	if (!flashmode && !wt)
		return;
	if (++savectop >= &savec[SAVEC])
		savectop = savec;
	if (savectop == savecbot)
		if (++savecbot >= &savec[SAVEC])
			savecbot = savec;
	savectop->writethrU = wt;
	savectop->fonT = f;
	savectop->poinT = p;
	savectop->chaR = c; 
}

saveline(p,q,t)
Point p,q;
Word t;
{
	int wt = (zaxis == WRITETHRU);

	if (!flashmode && !wt)
		return;
	if (++saveltop >= &savel[SAVEL])
		saveltop = savel;
	if (saveltop == savelbot)
		if (++savelbot >= &savel[SAVEL])
			savelbot = savel;
	saveltop->writethrU = wt;
	saveltop->starT = p;
	saveltop->enD = q;
	saveltop->texturE = t;
}

savepoint(p)
Point p;
{
	int wt = (zaxis == WRITETHRU);

	if (!flashmode && !wt)
		return;
	if (++saveptop >= &savep[SAVEP])
		saveptop = savep;
	if (saveptop == savepbot)
		if (++savepbot >= &savep[SAVEP])
			savepbot = savep;
	saveptop->writethrU = wt;
	saveptop->poinT = p;
}

savereset() {
	savecbot = savectop = savec;
	savelbot = saveltop = savel;
	savepbot = saveptop = savep;
}

