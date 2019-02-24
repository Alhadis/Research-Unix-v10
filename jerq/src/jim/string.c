#include "jim.h"
#include "file.h"

#define	MINSIZE	16	/* minimum number of chars allocated */

char	*strncpy();

strinit(p)
	String *p;
{
	gcnew(p->s, MINSIZE);
	p->size=MINSIZE;
	p->n=0;
}
strfree(p)
	String *p;
{
	if(p->s)
		gcfree(p->s);
	p->s=0;
	p->n=0;
	p->size=0;
}
String *
bldstring(s)
	register char *s;
{
	static String junk;
	junk.s=s;
	junk.n=strlen(s);
	return &junk;
}
char *
charstar(p)
	String *p;
{
	static char buf[256];
	register n=p->n;
	if(n>=256)
		n=256-1;
	strncpy(buf, p->s, n);
	buf[n]=0;
	return buf;
}
strzero(p)
	register String *p;
{
	if(p->size>MINSIZE){
		(void)gcrealloc(p->s, (ulong)MINSIZE);	/* throw away the garbage */
		p->size=MINSIZE;
	}
	p->n=0;
}
strdup(p, s)
	register String *p;
	register char *s;
{
	strinsure(p, p->n=strlen(s));
	strncpy(p->s, s, (int)p->n);
}
straddc(p, c)
	register String *p;
	char c;
{
	strinsure(p, p->n+1);
	p->s[p->n++] = c;
}
strinsure(p, n)
	register String *p;
	register ulong n;
{
	register int i;
	if(p->size<n){	/* p needs to grow */
		for(i=1; i<n; i<<=1)
			;
		gcrenew(p->s, i);
		p->size=i;
	}
}
strinsert(p, q, p0)
	register String *p, *q;
	register Posn p0;
{
if(p0>p->n) panic("strinsert");
	strinsure(p, (ulong)(p->n+q->n));
	bcopy(p->s+p0, p->s+p->n, p->s+p0+q->n, -1);
	bcopy(q->s, q->s+q->n, p->s+p0, 1);
	p->n+=q->n;
}
strdelete(p, p1, p2)
	register String *p;
	register Posn p1, p2;
{
if(p1>=p->n || p2>p->n || p1>p2) panic("strdelete");
	bcopy(p->s+p2, p->s+p->n, p->s+p1, 1);
	p->n-=(p2-p1);
}
