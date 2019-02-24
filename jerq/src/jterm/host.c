#include <jerq.h>

#define BUFsize 50
char buf[BUFsize];
int pbuf = 0;

extern Texture txtab[];

void send()
{
	if (pbuf>0) sendnchars(pbuf, buf);
	pbuf = 0;
}

void put(c) int c;
{
	if (pbuf>BUFsize) send();
	buf[pbuf++] = c;
}

sendint(n) register n;
{
	if (n<0) {
		put('-');
		n = -n;
	}
	for ( ; n != 0; n >>= 4) put((n&15)+'A');
	put('\n');
}

char getChar()
{	register int c;
	if ((c = rcvchar()) == -1) {
		wait(RCV);
		c = rcvchar();
	}
	return(c);
}

int getint()
{	char c; 
	register i, n;
	int sign = 1;

	if ((c = getChar()) == '-') {
		sign = -1;
		c = getChar();
	}
	for (n=0, i=0; (c >= 'A' && c <= 'P'); c = getChar(), i+=4) 
		n += (c-'A')<<i;
	return (n * sign);
}

void sendPoint(p) Point p;
{
	sendint(p.x);
	sendint(p.y);
}

Point getPoint()
{	Point p;
	p.x = getint();
	p.y = getint();
	return p;
}

void sendRect(r) Rectangle r;
{
	sendint(r.origin.x);
	sendint(r.origin.y);
	sendint(r.corner.x);
	sendint(r.corner.y);
}

Rectangle getRect()
{	Rectangle r;
	r.origin.x = getint();
	r.origin.y = getint();
	r.corner.x = getint();
	r.corner.y = getint();
	return r; 
}


long getlong() 
{
	return ( (long) getint() );
}

#define MAXSTR 1024
char tempstr[MAXSTR];

char *getstring()
{	char c; 
	register char *s;
	register int i, j;
	
	for (i=0; i<MAXSTR && ((tempstr[i]=getChar())!=0); i++) ;
	if (i==MAXSTR) {
		tempstr[MAXSTR-1] = 0;
		for (; getChar() != 0; ) ;	/*flush rest of string*/
	}
	s = (char *) alloc(i);
	for (j=0; j<i; j++) s[j] = tempstr[j];
	return(s);
}

Texture *getTexture()
{	int i, c; 
	c = getint();
	if (c==-1) {
		/* an index into txtab follows */
		c = getint();
		return( (c<0) ? (Texture *) 0 : &txtab[c]);
	}
	else {
		/* a texture follows */
		for (i=0; i<16; i++) txtab[c].bits[i] = getint();
		return(&txtab[c]);
	}
}

void sendTexture(t) Texture *t;
{
	sendint(t-txtab);
}
