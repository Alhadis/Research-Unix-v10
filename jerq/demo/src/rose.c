
/*
 * ROSE
 *
 * A blit program to generate random patterns derived from the
 * polar equation r=sin(lp*theta).  The two parameters of the
 * program are lp (number of loops) and dt (the amount by which
 * theta is incremented for each new point).  If you want continuous
 * random patterns, NONSTOP should be defined as 1 (the default).
 * Defining NONSTOP as 0 lets you enter the parameters each time.
 * Even if you enter the parameters, if you just hit <CR> you will
 * get random values.  To stop things when NONSTOP is 1 (continuous
 * random patterns), just hit any key.  To stop things when NONSTOP
 * is 0 (i.e. you are entering values), just enter a negative value
 * for 'loops'.
 *
 * The following is a Makefile for 'rose.m' (non-stop random patterns)
 * and'pararose.m' (asks you for the parameters).  To use, just
 * 
 *              68ld rose.m     or      68ld pararose.m
 *
 * JERQ=/usr/jerq
 * JERQINCLUDE=$(JERQ)/include
 * JERQBIN=$(JEQ)/bin
 * MCC=$(JERQBIN)/mcc
 * 
 * rose.m : rose.c
 * 	$(MCC) -DNONSTOP=1 rose.c -o rose.m
 * pararose.m : rose.c
 * 	$(MCC) -DNONSTOP=0 rose.c -o pararose.m
 */

#include <jerq.h>

#ifndef NONSTOP
#define NONSTOP 1
#endif

jmoveto(p)
Point p;
{
}

Point jstring(s)
char *s;
{
	Point p;
	return(p);
}

int xMax,yMax;

main()
{
	char buff[100];
	int cn, r, k, t, dt, lp, x, rx, ry;
	int oldx, oldy, newx, newy;
	int sin(), cos();
	int offx, offy;
	char *getstring();
	Point p;
	int nonstop = NONSTOP;

	request(KBD);

	/* random number seed is derived from position of blit layer */
	srand((unsigned)Drect.corner.x);
	for ( ;; ) {
		lp = dt = 0;
		if ( nonstop == 0 ) {
			/* ask for lp parameter */
			jmoveto(Pt(0,0));
			p = jstring("loops=");
			lp = getnum(p);
			jmoveto(Pt(0,0));
			jstring("loops=");
		}
		if ( lp < 0 )
			exit();
		if ( lp == 0 )
			lp = rand() % 31 + 1;

		if ( nonstop == 0 ) {
			/* ask for dt parameter */
			jmoveto(Pt(0,0));
			p = jstring("delta=");
			dt = getnum(p);
			jmoveto(Pt(0,0));
			jstring("delta=");
		}
		if ( dt <= 0 )
			dt = rand() % 358 + 1;

		/* clear screen */
		rectf(&display,inset(Drect,3),F_CLR);
		t=0;
		xMax = Drect.corner.x-Drect.origin.x;
		yMax = Drect.corner.y-Drect.origin.y;
		oldx = offx = xMax / 2;
		oldy = offy = yMax / 2;
		cn = 1;
		/* draw rose */
		do {
			t=(t+dt)%360;
			x=(lp*t)%360;
			r = sin(x);
			rx=muldiv(r,xMax,1024);
			ry=muldiv(r,yMax,1024);
			newx = offx + muldiv(rx,cos(t),1024) / 2;
			newy = offy +  muldiv(ry,sin(t),1024) / 2;
			segment(&display,add(Drect.origin,Pt(oldx,oldy)),
					add(Drect.origin,Pt(newx,newy)),F_STORE);
			oldx=newx; 
			oldy=newy;
			/* give up the CPU every 10 times around */
			if ( cn++ > 10 ) {
				cn = 0;
				sleep(4);
			}
		} while ( t != 0 && (k=kbdchar()) == -1 ) ;
		if ( nonstop == 1 ) {
			/* in nonstop mode, any key aborts */
			if ( k != -1 )
				exit();
			/* sleep 2 seconds between random patterns */
			sleep(120);
		}
	}
}

getnum(p)
Point p;
{
	static char strbuff[32];
	char s[2];
	int c;
	Point m; register int i;

	strbuff[i=0] = '\0';
	m = p;
	wait(KBD);
	for( ; (c=kbdchar())!='\r'; wait(KBD)) {
		switch (c) {
		case '@':
			jmoveto(p);
			jstring(strbuff);
			i=0;
			m=p;
			break;
		case '\b':
			jmoveto(p);
			jstring(strbuff);
			strbuff[(i>0)? --i : 0] = '\0';
			jmoveto(p);
			m = jstring(strbuff);
			break;
		case '+':
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if ( c == '+' || c == '-' && i != 0 )
				break;
			/* add character to saved string */
			strbuff[i++] = c;
			strbuff[i] = '\0';
			/* echo character */
			jmoveto(m);
			s[0]=c; s[1] = '\0';
			m = jstring(s);
			break;
		}
	}
	jmoveto(p);
	jstring(strbuff);
	return(atoi(strbuff));
}


