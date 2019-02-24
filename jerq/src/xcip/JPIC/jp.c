/*
 *	jp - interpret pic on dmd
 */

#include <dmd.h>
#define	YFUDGE	5	/* move chars down this much */
extern POINT current;
char	input[10000];
char	*ip = input;
POINT	mpos;
POINT	scanpoint();

main(argc, argv)
char *argv[];
{
	jinit();
	sysinit();
	spl0();
	jBonW();	/* black on white */
	getpic();
	printpic();
	for (;;) {
		if (button1()) {
			jclear();
			printpic();
		} else if (button23()) {
			jclear();
			getpic();
			printpic();
		}
	}
}

getpic()
{
	char temp[10];
	register char *start, *ip;
	static POINT pos = {20,20};

	ip = input;
	do {
		start = ip;
		while ((*ip++ = jgetchar()) != '\n')
			;
	} while (*start != 'S');
	*ip++ = 0;
	sprintf(temp, "%d", ip-input);
	jmoveto(pos);
	jstring(temp);
}

printpic()
{
	register int c;
	int i, n;
	POINT pos, pos1, pos2;
	char s[100];

	mpos = mouse.xy;
	cursinhibit();
	for (ip = input; *ip; ) {
		switch (*ip++) {
		case ' ':
		case '\n':
		case '\t':
			break;
		case 'm':
			pos = scanpoint();
			jmoveto(add(pos, mpos));
			break;
		case 'e':
			pos = scanpoint();
			scanint();
			scanint();
			break;
		case 'c':
			pos = scanpoint();
			jcircle(add(pos, mpos), scanint(), F_STORE);
			break;
		case 'a':
			pos = add(scanpoint(), mpos);
			pos2 = add(scanpoint(), mpos);
			pos1 = add(scanpoint(), mpos);
			jarc(pos, pos1, pos2, F_STORE);
			break;
		case 'l':
			pos = scanpoint();
			jmoveto(add(pos, mpos));
			pos = scanpoint();
			jlineto(add(pos, mpos), F_STORE);
			break;
		case 'b':
			pos1 = add(scanpoint(), mpos);
			pos2 = add(scanpoint(), mpos);
			jmoveto(pos1);
			pos.x = pos2.x;
			pos.y = pos1.y;
			jlineto(pos, F_STORE);
			jlineto(pos2, F_STORE);
			pos.x = pos1.x;
			pos.y = pos2.y;
			jlineto(pos, F_STORE);
			jlineto(pos1, F_STORE);
			break;
		case 't':
			pos = scanpoint();
			while ((c = *ip++) == ' ')
				;	/* find type */
			for (i = 0; (s[i++] = *ip++) != '\n'; )
				;
			s[i] = 0;
			pos.y += YFUDGE;
			n = strwidth(s);
			switch (c) {
			case 'C':
				pos.x -= n/2;
				break;
			case 'L':
				break;
			case 'R':
				pos.x -= n;
				break;
			}
			jmoveto(add(pos, mpos));
			jstring(s);
			break;
		case '~':	/* spline, expressed in delta's */
			dospline();
			break;
		default:
			while (*ip++ != '\n')
				;
			break;
		}
	}
	cursallow();
}

dospline()
{
	long w, t1, t2, t3;
	int x[50], y[50];
	int i, j, xp, yp, n;
	long scale = 1000;
	int steps = 10;
	POINT p;

	n = scanint();
	for (i = 1; i <= n; i++) {
		x[i] = scanint();
		y[i] = scanint();
	}
	x[0] = x[1]; y[0] = y[1];
	x[n+1] = x[n]; y[n+1] = y[n];
	p.x = x[0]; p.y = y[0];
	jmoveto(add(p, mpos));

	for (i = 0; i < n; i++) {
		for (j = 0; j < steps; j++) {
			w = scale * j / steps;
			t1 = w * w / (2 * scale);
			w = w - scale/2;
			t2 = 3*scale/4 - w * w / scale;
			w = w - scale/2;
			t3 = w * w / (2*scale);
			p.x = (t1*x[i+2] + t2*x[i+1] + t3*x[i] + scale/2) / scale;
			p.y = (t1*y[i+2] + t2*y[i+1] + t3*y[i] + scale/2) / scale;
			jlineto(add(p, mpos), F_STORE);
		}
	}
}

jclear()
{
	cursinhibit();
	jrectf(screenmap.rect, F_CLR);
	cursallow();
}

POINT
scanpoint()
{
	POINT p;

	p.x = scanint();
	p.y = scanint();
	return p;
}

scanint()
{
	register int n, c;

	while ((c = *ip++) == ' ' || c == '\t')
		;
	n = 0;
	do {
		n = 10 * n + c - '0';
	} while ((c = *ip++) >= '0' && c <= '9');
	return(n);
}
