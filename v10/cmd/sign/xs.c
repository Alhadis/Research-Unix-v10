#include <stdio.h>
#include <string.h>

char *xs();
unsigned long xsum();


char *
xs(key,body,n)
char *key, *body;
{
	unsigned long t;
	unsigned int x[2];
	static char s[17];
	char key2[8];
	int i;

	strncpy(key2,key,8);
	sprintf(s, "%d", n);
	xinit();
	xstring(s, strlen(s));
	xstring("$$$",3);
	xstring(key2,8);
	xstring(body,n);
	xstring("$$$",3);
	xstring(body,n);
	xstring(key2,8);
	xstring("omega",5);

	t = xsum();
	x[0] = t & 0xffff;
	x[1] = (t>>16) & 0xffff;

	sprintf(s, "%4x%4x", x[0], x[1]);
	xstring("digamma",7);
	t = xsum();
	x[0] ^= t & 0xffff;
	x[1] ^= (t>>16) & 0xffff;
	sprintf(s+8, "%4x%4x", x[0], x[1]);
	for(i=0; i<16; i++) if(s[i]==' ') s[i] = '0';
	return s;
}

#define SRLEN 5

static unsigned long sr[SRLEN], *sa, *sb;

xinit()
{
	int i;
	for(i=0; i<SRLEN; i++) sr[i] = 0;
	sa = &sr[0];
	sb = &sr[1];
}

unsigned long xsum() {
	return *sa ^ *sb;
}

xstring(s,n)
register char *s;
{
	while(n-->0)
		xcrank(*s++);
}

unsigned long
phi(x)
register unsigned long x;
{
	do {
		x += 14921776 + x*(17761492 + x);
		x ^= 0x5827066;
		x += 0x2c357 + x*(10661688 ^ x);
		x ^= x>>16;
	} while((x&0x80000002) == 2);
	return x;
}

xcrank(x)
register x;
{
	*sb++ += phi((x&0xff) + *sa++);
	if(sa >= &sr[SRLEN]) sa = &sr[0];
	if(sb >= &sr[SRLEN]) sb = &sr[0];
}
