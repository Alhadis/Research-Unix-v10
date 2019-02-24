#include "sam.h"

#define	MINSIZE	16		/* minimum number of chars allocated */
#define	MAXSIZE	BLOCKSIZE	/* maximum number of chars for an empty string */

strinit(p)
	String *p;
{
	gcnew(p->s, MINSIZE);
	p->n=0;
	p->size=MINSIZE;
}
strclose(p)
	String *p;
{
	gcfree(p->s);
}
strzero(p)
	register String *p;
{
	if(p->size>MAXSIZE){
		(void)gcrealloc(p->s, (ulong)MAXSIZE);	/* throw away the garbage */
		p->size=MAXSIZE;
	}
	p->n=0;
}
strdup(p, s)	/* copies the null */
	register String *p;
	register uchar *s;
{
	strinsure(p, (ulong)(p->n=strlen(s)+1));
	bcopy((uchar *)s, s+p->n, p->s, 1);
}
strdupstr(p, q)	/* will copy the null if there's one there */
	register String *p, *q;
{
	strinsure(p, (ulong)q->n);
	p->n=q->n;
	bcopy(q->s, q->s+q->n, p->s, 1);
}
straddc(p, c)
	register String *p;
	register c;
{
	strinsure(p, (ulong)p->n+1);
	p->s[p->n++]=c;
}
strinsure(p, n)
	register String *p;
	register ulong n;
{
	register ulong i;
	if(n>=32767)
		error(Etoolong);
	if(p->size<n){	/* p needs to grow */
		for(i=1; i<n; i<<=1)
			;
		if(i==32768L)
			i=32767L;
		gcrenew(p->s, i);
		p->size=i;
	}
}
strinsert(p, q, p0)
	register String *p, *q;
	register Posn p0;
{
	strinsure(p, (ulong)(p->n+q->n));
	bcopy(p->s+p0, p->s+p->n, p->s+p0+q->n, -1);
	bcopy(q->s, q->s+q->n, p->s+p0, 1);
	p->n+=q->n;
}
strdelete(p, p1, p2)
	register String *p;
	register Posn p1, p2;
{
	bcopy(p->s+p2, p->s+p->n, p->s+p1, 1);
	p->n-=(p2-p1);
}
String *
tempstr(s, n)
	uchar *s;
{
	static String p;
	p.s=s;
	p.n=n;
	p.size=n;
	return &p;
}

/*
 * These all take uchars as arguments.  Only lint and cyntax care.
 */
#ifdef lint
strncmp(s1, s2, n)
	register uchar *s1, *s2;
	register n;
{
	while(--n>=0 && *s1==*s2++)
		if(*s1++=='\0')
			return 0;
	return n<0? 0 : *s1-*--s2;
}
strcmp(s1, s2)
	register uchar *s1, *s2;
{
	while(*s1==*s2++)
		if(*s1++=='\0')
			return 0;
	return *s1-*--s2 ;
}
strcpy(s1, s2)
	register uchar *s1, *s2;
{
	while(*s1++=*s2++)
		;
}
strlen(s)
	register uchar *s;
{
	register n=0;
	while(*s++)
		n++;
	return n;
}
#endif
