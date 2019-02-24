#include <jerq.h>
#include <layer.h>
#include <font.h>

#include "jcom.h"
#include "jjplot.h"
char *menutext[] = {"more", NULL};
Menu menu = {menutext};

graphics(){
	register int c;
	register int counter;
	Point p0, p1, p2, p3, p4;
	struct seg  *s, *sp;
	short *tp;
	int r, d, ymin;
	int cir = 0;

	cursswitch(&cherries);
	counter = 0;
	while ((c = getchar())  != CLOSE){
		if(ckreshape())reinit();
		switch(c) {
	
		case HOME:
			PtCurrent.x = arect.origin.x;
			PtCurrent.y = arect.corner.y - defont.height;
			break;
		case ARC:	/* arc's Pcenter, Pstart, Pfinish */
			p0 = getpt();
			p1 = getpt();
			p2 = getpt();
			arc(&display, p0, p1, p2, F_STORE);
			PtCurrent = p2;
			break;
		case PAUSE:
			ringbell();
			while(1){
			if(!button3()||!(own()&MOUSE)){
				sleep(1);
				continue;
			}
			switch(menuhit(&menu, 3)){
			case MORE:
				sendchar(TOK);
				break;
			case QUIT:
				sendchar(TQUIT);
				goto tterm;
			case -1:
				continue;
			}
			break;
			}
			break;
	
		case ERASE:	/* erase screen */
			stipple(arect);
			PtCurrent = arect.origin;
			break;
	
		case MOVE:	/* move to point */
			PtCurrent = getpt();
			break;
		case POINT:
			PtCurrent = getpt();
			point(&display, PtCurrent, F_STORE);
			break;
		case LINETH:
			lineth = getchar();
			break;
		case FILL:
			d = getint();
			ymin = getint();
			ymin = muldiv(ymin,delta,800);
			if(d > 4){
			s=(struct seg *)alloc((unsigned)(d*
				sizeof(struct seg)));
			if(s==0)break;
			}
			else s=mLine;
			for(sp=s; sp<s+d; sp++){
				sp->j1 = getpt();
				sp->j2 = getpt();
				sp->stat = 1;
			}
			jjfill(d, s, ymin);
			if(d > 4)
				free(s);
			break;
		case TEXTURE:
			for(tp=&t.bits[0];tp<=&t.bits[15];tp++){
				*tp = getint();
			}
			fmode = (getint()>>8)&3;
			break;
		case CIRCLE:
			cir++;
		case DISC:
			cir++;
			p0 = getpt();
			r = getint();
			r = muldiv(r,delta,800);
			if(cir == 1)
				disc(&display, p0, r, F_STORE);
			else circle(&display, p0, r, F_STORE);
			cir = 0;
			break;
		case SBOX:
			p0 = getpt();
			p1 = getpt();
			texture(&display, Rpt(p0, p1), &t, F_STORE);
			PtCurrent = p1;
			break;
	
		case TEXT:
			d = getint();
			PtCurrent.x -= d * CW;
			d = getint();
			while(--d)
				outc(getchar(), &PtCurrent, 1);
			break;
		case VEC:
			p0 = getpt();
			segment(&display, PtCurrent, p0, F_STORE);
			PtCurrent = p0;
			break;
		case PAR:
			p0 = getpt();
			p1 = getpt();
			p2 = getpt();
			parabola(p0, p1, p2);
			break;
		case TOK:
			if(own() & KBD){
				kbdchar();
				sendchar(TQUIT);
				goto tterm;
			} else sendchar(TOK);
			break;
		}
	}
tterm:	PtCurrent.x = arect.origin.x;
	PtCurrent.y = arect.corner.y - defont.height;
	cursswitch((Texture *) 0);
}
