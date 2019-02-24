#include "/usr/lib/a68defs"


int	mkfault;
int	stdin;
char readbuf[255]; char *readptr;
char eof, lastchar;

readchar()
{
	REG CHAR c;

	IF readptr==0
	THEN	newline();
	FI
	IF eof ORF mkfault
	THEN	c=EOF;
	ELIF c = *readptr THEN readptr++; FI
	return(lastchar=c);
}

newline()
{
	readptr=readbuf;
	WHILE eof==0 ANDF mkfault==0
	     ANDF ( eof = read(stdin,readptr,1)==0,
		  *readptr++!=NL
		)
	DONE
	*readptr++=0; readptr=readbuf;
}

error(s1,s2)
char *s1, *s2;
{
	prints(s1); prints(s2);
	flushin();
}

flushin()
{
	readptr=0;
}

peekchar()
{
	REG CHAR c;
	c=nextchar();
	backspace();
	return(c);
}

nextchar()
{
	REG CHAR c;

	WHILE (c=readchar())==SP ORF c==TB DONE
	return(c);
}

readint(i)
int *i;
{
	REG INT r;
	REG CHAR c;
	REG INT minus;
	INT rc;

	IF (c=nextchar())=='-'
	THEN	minus=TRUE; c=readchar();
	ELSE	minus=FALSE;
	FI
	IF digit(c)
	THEN	r = 0;
		REP r =* 10; r =+ c-'0'; PER digit(c=readchar()) DONE
		IF minus THEN r=(-r); FI
		*i = r; backspace();
		rc = 1;
	ELSE	rc = 0;
	FI
	IF eof ORF mkfault
	THEN	return(-1);
	ELSE	return(rc);
	FI
}

readreal(a)
float *a;
{
	REAL r;
	INT rc;
	REG INT fr;
	REG CHAR c;
	REG INT minus;

	IF (c=nextchar())=='-'
	THEN	minus=TRUE; c=readchar();
	ELSE	minus=FALSE;
	FI
	r = 0; rc = 0;
	IF digit(c)
	THEN	rc = 1;
		REP r =* 10; r =+ c-'0'; PER digit(c=readchar()) DONE
	FI
	fr = 0;
	IF c=='.'
	THEN	WHILE digit(c=readchar())
		DO r =* 10; r =+ c-'0'; fr++; OD
		IF fr THEN rc = 1; FI
	FI
	WHILE fr-- DO r =/ 10; OD
	IF minus THEN r = (-r); FI
	IF rc THEN backspace(); FI
	*a = r;
	IF eof ORF mkfault
	THEN	return(-1);
	ELSE	return(rc);
	FI
}

reads(term, buffer)
char *term; char buffer[];
{
	register char	c;
	REG CHAR	*ptr;

	ptr=buffer;
	WHILE !any(c=readchar(),term) ANDF c
	DO *ptr++ = c; OD
	IF mkfault ORF eof
	THEN	return(-1);
	ELIF ptr>buffer
	THEN	*ptr++=0; backspace();
		return(1);
	ELSE	return(0);
	FI
}

backspace()
{
	IF readptr>readbuf
	THEN	readptr--;
	FI
}

any(c,s)
char c;
char *s;
{
	register char a, b;
	register char *p;

	p=s; a=c;
	WHILE b = *p++
	DO IF b==c THEN return(1); FI OD
	return(0);
}

digit(c)
char c;
{
	return(c>='0' ANDF c<='9');
}

lineended()
{
	return(readptr==0 ORF *readptr==0 ORF peekchar()==NL);
}

sign(a)
{
	return(a<0?-1:(a!=0));
}

cf(as1, as2)
char *as1, *as2;
{
	REG char *s1, *s2;

	s1 = as1;
	s2 = as2;
	WHILE *s1++ == *s2
	DO	IF *s2++==0
		THEN	return(0);
		FI
	OD
	return(*--s1 - *s2);
}
