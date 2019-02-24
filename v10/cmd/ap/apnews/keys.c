# include "stdio.h"
# include "assert.h"
# include "ctype.h"

	static long lp, lim;
	static int alph, prevc;
	static char *p, key[20];
	extern char *iglist, *svc();
	void addkey(), searchkey();
	void (*what)();
# define NK 300
	struct keyst {
		char *keytxt;
		int keywt;
		int keyocc;
		} keys[NK];
	struct keyst *keyp;

resetkey()
{
keyp=keys;
}

dofile (name)
	char *name;
{

# define MAXLINE 500
struct keyst *ks;
int c;
FILE *f;
f = fopen(name, "r");
if (f==NULL) return;
alph=lp=prevc=0;
p=key;
while ( (c=getc(f)) != EOF )
	chkey (c);
fclose(f);
}

outkey( ky, lead, trail)
	char *ky;
{
	int n;
n = strlen(ky);
if (n<2) return(0);
if (n<3)
	{
	if (trail == '.') return(0);
	if (strchr(".%,!#$%&'();+:*", lead)) return(0);
	}
if (isdigit(ky[0]))
	if (ky[0] != '1' || ky[1] != '9' || n!= 4) return(0);
if (common(ky))
	return(0);
return(1);
}
chkey (c)
	{
	if ((isalpha(c) || isdigit(c)) && prevc != '\\')
		{
		if (alph++ < 6)
			*p++ = c;
		}
	else
		{
		*p = 0;
		for(p=key; *p; p++)
			*p |= 040;
		if (outkey(p=key,prevc,c))
			(*what)(key);
		prevc=c;
		alph=0;
		}
	}
void
addkey(key)
	char *key;
{
struct keyst *ks;
	for(ks=keys; ks<keyp; ks++)
		if (strcmp(ks->keytxt, key)==0)
			break;
	if (ks<keyp)
		ks->keywt++;
	else
		{
		keyp->keytxt = svc(key);
		keyp->keywt=1;
		assert(++keyp-keys <NK);
		}
	}
void
searchkey(s)
	char *s;
{
struct keyst *ks;
for(ks=keys; ks<keyp; ks++)
	{
	if (strcmp(ks->keytxt, s)==0)
		break;
	}
if (ks<keyp)
	ks->keyocc++;
}
zerokey()
{
struct keyst *ks;
for(ks=keys; ks<keyp; ks++)
	ks->keyocc=0;
}
scorekey()
{
struct keyst *ks;
int sc=0;
for(ks=keys; ks<keyp; ks++)
	sc += min(ks->keyocc, ks->keywt);
return(sc);
}
min(a,b)
{
return(a<b ? a : b);
}
mkkey(s)
	char *s;
{
int i, na; char *sa[20];
na = getargs(s, sa);
for(i=0; i<na; i++)
	{
	if (isdigit(sa[i][0]))
		keyp->keywt = atoi(sa[i]);
	else
		{
		if (strlen(sa[i]) > 6)
			sa[i][6]=0;
		keyp->keytxt = svc(sa[i]);
		keyp->keywt = 1;
		keyp++;
		}
	}
}
