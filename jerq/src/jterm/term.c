#include <jerq.h>
#include <font.h>
#include "commands.h"

extern Point getPoint();
extern Rectangle getRect();
extern char *getstring();
extern int getint();
extern long getlong();
extern Texture *getTexture();
extern char getChar();

extern Point growarc();
extern Point movearc();
extern Point growbox();
extern Point movebox();
extern Point growcircle();
extern Point movecircle();
extern Point growelarc();
extern Point moveelarc();
extern Point growellipse();
extern Point moveellipse();
extern Point growline();
extern Point moveline();
extern Point growstring();
extern Point movestring();

#define BMTSIZE 50
Bitmap *bmtab[BMTSIZE];

#define MAXFONT 20
Font *fntab[MAXFONT];

#define TXTSIZE 20
Texture txtab[TXTSIZE];

#define MAXMENU 20
Menu *mntab[MAXMENU];

#define ox origin
#define cx corner

main()
{ 	register i, j, k, n;
	Point p, q, a, *pp;
	Rectangle r, s;
	char *str;
	Bitmap *b;
	Texture *t;
	char **tb;

	request(KBD|RCV|SEND|MOUSE);
	
	sendint(Drect.origin.x);
	sendint(Drect.origin.y);
	sendint(Drect.corner.x);
	sendint(Drect.corner.y);
	send();

	fntab[0] = &defont;
	bmtab[0] = &display;

	updateMouseQ();
	while (1) {
		while ( !(own()&RCV)) wait(RCV);
		n = getint();
		switch (n) {
		case CCALARM: 
			getint(n);
			alarm(n);
			break;
		case CCARC: 
			i = getint();
			p = getPoint();
			q = getPoint();
			a = getPoint();
			n = getint();
			arc(bmtab[i],p,q,a,n); 
			break;
		case CCBALLOC:
			i = getint();
			r = getRect();
			bmtab[i] = balloc(r);
			sendint((bmtab[i]==(Bitmap *)0)?-1:i);
			break;
		case CCBFREE:
			i = getint();
			bfree(bmtab[i]);
			bmtab[i] = (Bitmap *) 0;  
			break;
		case CCBITBLT: 
			i = getint();
			j = getint();
			r = getRect();
			p = getPoint();
			n = getint();
			bitblt(i,j,r,p,n);
			break;
		case CCBSPLINE:
			i = getint();
			n = getint();
			pp = (Point *) alloc((n+2)*sizeof(Point));
			for (k=1; k<=n; k++) pp[k] = getPoint();
			j = getint();
			bspline(bmtab[i],pp, n, j);
			free(pp);
			break;
		case CCCIRCLE: 
			i = getint();
			p = getPoint();
			j = getint();
			n = getint();
			circle(bmtab[i],p,j,n);
			break;
		case CCCURSALLOW:
			cursallow(); 
			break;
		case CCCURSINHIBIT:
			cursinhibit(); 
			break;
		case CCCURSSET: 
			p = getPoint();
			cursset(p);
			break;
		case CCCURSSWITCH: 
			t = getTexture();
			t = cursswitch(t);
			sendTexture(t);
			break;
		case CCDISC: 
			i = getint();
			p = getPoint();
			j = getint();
			n = getint();
			disc(bmtab[i],p,j,n);
			break;
		case CCELLIPSE: 
			i = getint();
			p = getPoint();
			j = getint();
			k = getint();
			n = getint();
			ellipse(bmtab[i],p,j,k,n);
			break;
		case CCELDISC: 
			i = getint();
			p = getPoint();
			j = getint();
			k = getint();
			n = getint();
			eldisc(bmtab[i],p,j,k,n);
			break;
		case CCELARC: 
			i = getint();
			p = getPoint();
			j = getint();
			k = getint();
			q = getPoint();
			a = getPoint();
			n = getint();
			elarc(bmtab[i],p,j,k,q,a,n); 
			break;
		case CCEXIT:
			exit(); 
			break;
		case CCFFREE: 
			i = getint();
			ffree(fntab[i]);
			fntab[i] = (Font *) NULL;
			break;
		case CCGETFONT: 
			i = getint();			
			fntab[i] = infont(getChar);
			break;
		case CCGETMOUSE:
			sendQ();
			break;
		case CCGETRECT:
			sendRect(getrect()); 
			break;
		case CCKBDCHAR:
			while ((i=kbdchar())!=-1) sendint(i);
			sendint(-1);
			break;
		case CCKILL:
			n = getint();
			kill(n); 
			break;
		case CCMENU:
			i = getint();
			j = getint();
			tb = (char **) alloc((j+1)*sizeof(char*));
			for (k=0; k<j; k++) tb[k] = getstring();
			tb[j] = NULL;
			mntab[i] = (Menu *) alloc(sizeof(Menu));
			mntab[i]->item = tb;
			break;
		case CCMENUHIT:
			i = getint();
			j = getint();
			sendint(menuhit(mntab[i], j)); 
			clearMouseQ();
			break;
		case CCNAP: 
			n = getint(n);
			nap(n);
			break;
		case CCOWN:
			n = getint();
			sendint(own(n)); 
			break;
		case CCPOINT: 
			i = getint();
			p = getPoint();
			n = getint();
			point(bmtab[i],p,n);
			break;
		case CCREALTIME: 
			realtime();
			break;
		case CCRECT:
		case CCRECTF: 
			i = getint();
			r = getRect();
			j = getint();
			if (n==CCRECT) {
				segment(bmtab[i],r.ox,Pt(r.cx.x,r.ox.y),j);
				segment(bmtab[i],Pt(r.cx.x,r.ox.y),r.cx,j);
				segment(bmtab[i],r.cx,Pt(r.ox.x,r.cx.y),j);
				segment(bmtab[i],Pt(r.ox.x,r.cx.y),r.ox,j);
			}
			else rectf(bmtab[i],r,j);
			break;
		case CCREQUEST:
			i = getint();
			request(i);
			break;
		case CCRINGBELL:
			ringbell();
			break;
		case CCSCREENSWAP: 
			i = getint();
			r = getRect();
			s = getRect();
			screenswap(bmtab[i],r,s);
			break;
		case CCSEGMENT:
			i = getint();
			p = getPoint();
			q = getPoint();
			n = getint();
			segment(bmtab[i],p,q,n); 
			break;
		case CCSLEEP: 
			n = getint();
			sleep(n);
			break;
		case CCSTRING: 
			i = getint();
			str = getstring();
			j = getint();
			p = getPoint();
			k = getint();
			sendPoint(string(fntab[i],str,bmtab[j],p,k));
			break;
		case CCTEXTURE:
			i = getint();
			r = getRect();
			t = getTexture();
			n = getint();
			texture(bmtab[i],r,t,n); 
			break;
		case CCTRACKARC:
			i=getint();
			j = getint();
			p = getPoint();
			q = getPoint();
			a = getPoint();
			sendPoint( i ? growarc(j,p,q,a) : movearc(j,p,q,a) );
			clearMouseQ();
			break;
		case CCTRACKCIRCLE:
			i=getint();
			j = getint();
			p = getPoint();
			k = getint();
			sendPoint( i ? growcircle(j,p,k,0) : movecircle(j,p,k,0));
			clearMouseQ();
			break;
		case CCTRACKDISC:
			i=getint();
			j = getint();
			p = getPoint();
			k = getint();
			sendPoint( i ? growcircle(j,p,k,1) : movecircle(j,p,k,1));
			clearMouseQ();
			break;
		case CCTRACKELARC:
			i=getint();
			j = getint();
			p = getPoint();
			k = getint();
			n = getint();
			q = getPoint();
			a = getPoint();
			sendPoint(i?growelarc(j,p,q,a,k,n):moveelarc(j,p,q,a,k,n));
			clearMouseQ();
			break;
		case CCTRACKELDISC:
			i = getint();
			j = getint();
			p = getPoint();
			k = getint();
			n = getint();
			sendPoint(i?growellipse(j,p,k,n,1):moveellipse(j,p,k,n,1));
			clearMouseQ();
			break;
		case CCTRACKELLIPSE:
			i = getint();
			j = getint();
			p = getPoint();
			k = getint();
			n = getint();
			sendPoint(i?growellipse(j,p,k,n,0):moveellipse(j,p,k,n,0));
			clearMouseQ();
			break;
		case CCTRACKLINE:
			i = getint();
			j = getint();
			p = getPoint();
			q = getPoint();
			sendPoint( i ? growline(j,p,q) : moveline(j,p,q) );
			clearMouseQ();
			break;
		case CCTRACKRECT:
			i=getint();
			j = getint();
			r = getRect();
			sendPoint( i ? growbox(j,r,0) : movebox(j,r,0) );
			clearMouseQ();
			break;
		case CCTRACKRECTF:
			i = getint();
			j = getint();
			r = getRect();
			sendPoint( i ? growbox(j,r,1) : movebox(j,r,1) );
			clearMouseQ();
			break;
		case CCTRACKSTRING:
			i = getint();
			j = getint();
			p = getPoint();
			str = getstring();
			k = getint();
			sendPoint(i ? growstring(j,p,str,fntab[k])
				    : movestring(j,p,str,fntab[k]));
			clearMouseQ();
			break;
		default:
			break;
		}
		send();
		updateMouseQ();
	}
}
