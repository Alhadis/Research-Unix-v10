# include "stdio.h"
# include "assert.h"
# define SAME 0
struct suft {
	char *told;
	int tlen;
	char *tnew;
	int type;
	}
# define S 1
# define EDING 2
# define LY 3
suftab[] = {
{"is", 2, "is", S},
{"ies", 3, "y", S},
{"us", 2, "us", S},
{"sses", 4, "ss", S},
{"xes", 3, "x", S},
{"jones", 5, "jones", S},
{"ss", 2, "ss", S},
{"ings", 4, "", EDING},
{"namics", 6, "namics", S},
{"hysics", 6, "hysics", S},
{"hes", 3, "h", S},
{"autics", 6, "autics", S},
{"matics", 6, "matics", S},
{"stics", 5, "stics", S},
{"news", 4, "news", S},
{"setts", 5, "setts", S},
{"denes", 5, "denes", S},
{"'s", 2, "", S},
{"s", 1, "", S},
{"speed", 5, "speed", EDING},
{"ied", 3, "y", EDING},
{"ed", 2, "", EDING},
{"ssing", 5, "ss", EDING},
{"ing", 3, "", EDING},
{"ply", 3, "ply", LY},
{"ily", 3, "ily", LY},
{"ly", 2, "", LY},
{0, 0, 0}};
# define OK 0
# define ADD 1
struct et {
	char *esuf;
	int elen;
	int evow;
	int eadd;
	} etab[] = {
{"x", 1, 0, OK},
{"sponsor", 7, 0, OK},
{"monitor", 7, 0, OK},
{"og", 2, 2, OK},
{"Vg", 2, 0, ADD},
{"ow", 2, 0, OK},
{"dit", 3, 0, OK},
{"er", 2, 0, OK},
{"el", 2, 0, OK},
{"ss", 2, 0, OK},
{"et", 2, 0, OK},
{"el", 2, 0, OK},
{"air", 3, 0, OK},
{"en", 2, 2, OK},
{"on", 2, 2, OK},
{"ong", 3, 0, OK},
{"CVC", 3, 0, ADD},
{"VVv", 3, 0, ADD},
{"Vlv", 3, 0, ADD},
{"nc", 2, 0, ADD},
{"pl", 2, 0, ADD},
{"bl", 2, 0, ADD},
{"rg", 2, 0, ADD},
{"lg", 2, 0, ADD},
{"dg", 2, 0, ADD},
{"ng", 2, 0, ADD},
{"iat", 3, 0, ADD},
{"u", 1, 0, ADD},
{"uir", 3, 0, ADD},
{"Cs", 2, 0, ADD},
{"us", 2, 0, ADD},
{0, 0, 0}};
char vow[] = {
/*nul  soh  stx  etx  eot  enq  ack  bel  */
   0,   0,   0,   0,   0,   0,   0,   0,
/*bs   ht   nl   vt   np   cr   so   si   */
   0,   0,   0,   0,   0,   0,   0,   0,
/*dle  dc1  dc2  dc3  dc4  nak  syn  etb  */
   0,   0,   0,   0,   0,   0,   0,   0,
/*can  em   sub  esc  fs   gs   rs   us   */
   0,   0,   0,   0,   0,   0,   0,   0,
/*sp    !    "    #    $    %    &    '   */
   0,   0,   0,   0,   0,   0,   0,   0,
/* (    )    *    +    ,    -    .    /   */
   0,   0,   0,   0,   0,   0,   0,   0,
/* 0    1    2    3    4    5    6    7   */
   0,   0,   0,   0,   0,   0,   0,   0,
/* 8    9    :    ;    <    =    >    ?   */
   0,   0,   0,   0,   0,   0,   0,   0,
/* @    A    B    C    D    E    F    G   */
   0,   1,   0,   0,   0,   1,   0,   0,
/* H    I    J    K    L    M    N    O   */
   0,   1,   0,   0,   0,   0,   0,   1,
/* P    Q    R    S    T    U    V    W   */
   0,   0,   0,   0,   0,   1,   0,   0,
/* X    Y    Z    [    \    ]    ^    _   */
   0,   1,   0,   0,   0,   0,   0,   0,
/* `    a    b    c    d    e    f    g   */
   0,   1,   0,   0,   0,   1,   0,   0,
/* h    i    j    k    l    m    n    o   */
   0,   1,   0,   0,   0,   0,   0,   1,
/* p    q    r    s    t    u    v    w   */
   0,   0,   0,   0,   0,   1,   0,   0,
/* x    y    z    {         }    ~   del  */
   0,   1,   0,   0,   0,   0,   0,   0 };
char *
sufstr(p)
	char *p;
{ /* removes a few common suffixes */
static char wd[50];
struct suft *x;
struct et *xe;
register char *s; register int c;
int nl, nv;
strcpy(wd, p);
nl = strlen(wd);
for(s=wd; *s; s++);
for(x=suftab; x->told; x++)
	{
	if (nl < x->tlen) continue;
	if (strcmp(x->told, s-x->tlen)==SAME)
		{
		s -= x->tlen;
		strcpy(s, x->tnew);
		nv = novow(wd);
		if (strlen(wd)<3 || nv==0)
			return(p);
		switch(x->type)
			{
			case S:
				return(wd);
			case EDING:
			c = s[-1];
			if (s[0]!=0) return(wd);
			/* undouble final consonant? */
			if (!vow[c] && c!= 's' && c==s[-2] && (c!='l'||nv>1))
				{
				s[-1]=0;
				return(wd);
				}
			/* add final e decision */
			for( xe = etab; xe->esuf; xe++)
				{
				if (xe->evow > nv)
					continue;
				if (ecomp(xe->esuf, s-xe->elen))
					{
					if (xe->eadd)
						strcpy(s, "e");
					return(wd);
					}
				}
			return(wd);
			case LY:
				return(wd);
			}
		return(wd);
		}
	}
return(p);
}
ecomp (etent, wd)
	char *etent, *wd;
{
register int c, d;
while ( c= *etent++)
	{
	d = *wd++;
	switch(c)
		{
		case 'C':
			if (!vow[d])
				continue;
			else
				return(0);
		case 'V':
			if (vow[d])
				continue;
			else
				return(0);
		default:
			if (c==d)
				continue;
			else
				return(0);
		}
	}
return(1);
}
novow(s)
	char *s;
{
int c, k=0;
while (c = *s++)
	if (vow[c])
		k++;
return(k);
}
char *comwords[420];
# define NCHASH 401
int hashc[NCHASH];
char comspace[2000];
char *combuff=comspace;
char *comname = "/usr/lib/refer/function";
common (s)
	char *s;
{
char *p;
static int cgate=0;
register int k, c, i;
if (cgate==0)
	{
	cgate=1;
	comstart();
	}
c = s[0];
for(k = hash(s); i = hashc[k]; k=(k+1)%NCHASH)
	{
	p = comwords[i];
	if (p==NULL) continue;
	if (c!= *p) continue;
	if (strcmp(p, s)==SAME)
		return(1);
	}
return(0);
}
hash(s)
	register char *s;
{
register int n=0, c;
while ( c = *s++)
	{
	n += (c<<n%4);
	}
return ( ( n & 077777) %NCHASH);
}
comstart()
{
FILE *comf;
char line[300];
int nw=1, j, i;
/* to permit fast test, we skip one byte at start of combuff */
comf= fopen(comname, "r");
assert(comf!=NULL);
while (fgets(line, 300, comf))
	{
	trimnl(line);
	comwords[i=nw++]=combuff;
	strcpy(combuff, line);
	while (*combuff++);
	assert(nw<300);
	assert(combuff<comspace+2000);
	j = hash (comwords[i]);
	while (hashc[j]) j++;
	hashc[j]=i;
	assert(j<1500);
	}
fclose(comf);
}
