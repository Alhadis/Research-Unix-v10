#include <stdio.h>
#include <signal.h>
#include "jcom.h"
#include "jplot.h"
#include <sgtty.h>

static int
	opened = 0		/* 0 if in alphanumeric mode, 1 if graphics */
;

static char
	buf[PACKET],		/* storage for a packet of data */
	*p = &buf[0]		/* free position in buf */
;
int t;
extern int openflg;
int counter;
flush()
{
	char c;
	char *ss;


	if(p != buf){
		if((write(jerq, buf, p - buf)) == -1){
			fprintf(stderr,"write error to blit\n");
		}
/*
		for(ss=buf;ss<p;ss++)
			fprintf(stderr,"%o ",(*ss &0377));
		fprintf(stderr,"\n");
*/
	}
	p = &buf[0];
}

void
xysc(xi, yi)
double xi, yi;
{
	register int xa, ya;
	xa = SCX(xi);
	ya = SCY(yi);

	graphic((xa&077) | 0300);
	graphic(((xa >> 6) & 017) | ((ya >> 3) & 0160) | 0200);
	graphic(ya&0177 | 0200);

	e1->copyx = xi;
	e1->copyy = yi;
}
void
xy(xa, ya)
int xa, ya;
{
	graphic((xa&077) | 0300);
	graphic(((xa >> 6) & 017) | ((ya >> 3) & 0160) | 0200);
	graphic(ya&0177 | 0200);
}
void
intout(n0)
int n0;
{
	graphic((n0&0377));
	graphic(((n0>>8)&0377));
}

void
start(clear)
{
	if(clear){
		graphic(ERASE);
		cmd_sent();
	}
}

void
graphic(c)
char c;
{

	if (opened == 0) {
		++opened;
		graphic(OPEN);
	}

	*p++ = c;
	if (p == &buf[PACKET])
		flush();
}

void
closep()
{
	extern struct sgttyb
		savetty;			/* cooked tty modes */
	if(openflg)
		finish(mpx);
	ioctl(jerq, TIOCSETP, &savetty);
}

void
finish()
{

	graphic(CLOSE);
	flush();
}


normx(x)
double x;
{
	register double xx;
	xx = SCX(x);
	return( (int)(xx>0.? xx+.5: xx-.5) );
}
normy(y)
double y;
{
	register double yy;
	yy = SCY(y);
	return( (int)(yy>0. ? yy+.5: yy-.5) );
}
idle(){}

pcopy(a,b)
struct penvir *a, *b;
{
	b->bottom = a->bottom;
	b->left = a->left;
	b->ymin = a->ymin;
	b->xmin = a->xmin;
	b->scalex = a->scalex;
	b->scaley = a->scaley;
	b->sidex = a->sidex;
	b->sidey = a->sidey;
	b->grade = a->grade;
	b->quantum = a->quantum;
	b->pen = a->pen;
	b->texture = a->texture;
}
ptype(){}
alarmcatch(){}
isjplot(){
	register val;
	char c;
	signal(SIGALRM, alarmcatch);
	c = REQ;
	write(jerq, &c, 1);
	alarm(3);
	read(jerq,&c, 1);
	alarm(0);
	val = ((c&0377) == ACK);
	if(val){
		read(jerq, &c, 1);	/*read newline*/
	}
	return(val);
}
term()
{
	char c;
	read(jerq, &c, 1);
	return(c&0377);
}
