# include "stdio.h"
# include "assert.h"
# include "ctype.h"
# define SEP 001
# define MAXLIN 100
# define MAXCH 4000
# define control(s) ((s)=='%')
int authsplit =1; /* separate line for each author */
char *lasbl(), *artskip(), *lmin();
int typesplit =1; /* separate line for each type */
int nlines =0;
char *ln[MAXLIN];
char date[20], mdate[20];
int beenhere[5];
char *kdate, stitle[50], *auth;
char *keys= "A%D.T"; /* key to be author, kind of pub, date, title */
main (argc,argv)
	char *argv[];
{
FILE *f;
char bf[MAXCH], *p, *r;
int j, k, m,d, type, skip, comment;
/* type: 1 TM 2 RP 3 Patent 4 Pat. applc */
assert (argc>0);
argc--; argv++;
while (argc-->0)
	{
	assert(*argv!=0);
	if (argv[0][0] == '-')
		switch ( (*argv++)[1])
			{
			case 's': /* no split */
				authsplit=0; continue;
			case 't': /* no type split */
				typesplit= 0; continue;
			case 'k': /* key string */
				keys = *argv++; argc--; continue;
			default: continue;
			}
	f = fopen(*argv++, "r");
	assert (f!=NULL);
	for(;;)
		{
		/* read this input file */
		ln[nlines=0] = bf;
		comment=0;
		for(;;)
			/* obtain item */
			{
			m=(int)fgets(p=ln[nlines], bf+MAXCH-p, f);
			if (m==NULL)break;
			if (*p=='\n') break;
			if (control(p[0]) && p[1]=='X')
				{
				comment=1;
				continue;
				}
			if (comment && !control(p[0]))
				continue;
			comment = 0;
			while (*p++);
			if (p[-2]=='\n') p[-2] = 0;
			assert(nlines<MAXLIN);
			assert(p<bf+MAXCH);
			ln [ ++nlines ] = p;
			}
		if (nlines==0 && m==NULL) break;
		/* item read */
		if (nlines==0) continue;
		/* now get date, etc. */
		strcpy(date,"1980");
		stitle[0]=0;
		for(d=0; d<nlines; d++)
			{
			p = ln[d];
			if (!control(p[0])) continue;
			switch(p[1])
				{
				case 'D':
					r = lasbl(p, 'D');
					if (isdigit(*r))
						sprintf(date, "%s.%02d", r, mon(p+3));
					break;
				case 'M':
					for (p=p+2;*p;p++)
						if (isdigit(*p))
							{
							sprintf(mdate, "19%c%c.%02d", p[0],p[1], atoi(lmin(p)));
							break;
							}
					break;
				case 'T':
					sprintf(stitle,"%.10s", artskip(p+3));
					break;
				}
			}
		for(d=0; d<5; d++)
			beenhere[d]=0;
		if (typesplit)
			for(d=0; d<nlines; d++)
				{
				p=ln[d];
				if (control(p[0]))switch(p[1])
					{
					case 'B':
					case 'J': case 'I':
					case 'R': type = 2; doit(type); break;
					case 'M': type =1; doit(type); break;
					case 'p': type = 3; doit(type); break;
					case 'a': type = 4; doit(type); break;
					}
				}
		else
			doit(0);
		if (m==NULL) break;
		}
	fclose(f);
	}
}
doit(type)
{
int j; char *p;
if (beenhere[type]) return;
beenhere[type]=1;
kdate = (type==1) ? mdate: date;
j= atoi(kdate);
if (type==1 && (j<1960 || j >1990)) kdate=date;
for(j=0; j<nlines; j++)
	{
	p=ln[j];
	if (!control(p[0]) || p[1] != 'A') continue;
	auth=p;
	pline(type);
	if (authsplit==0) break;
	}
}
pline(type)
{
char *pk, *p, *s;
int j, c;
for (pk=keys; c= *pk; pk++)
	{
	switch(c)
		{
		case 'A':
			auprint(auth); break;
		case '%':
			printf("*%d_", type); break;
		case 'T':
			/* printf("%s_", stitle); break; */ fputs(stitle, stdout); putchar('_'); break;
		case 'M':
			for(j=0; j<nlines; j++)
				{
				p=ln[j];
				if (control(p[0]))
					if (p[1]=='M')
						{
						printf("%s_",p+3);
						break;
						}
				}
			break;
		case 'D':
			/* if D is followed by . we want month otherwise not. */
			c = pk[1];
			if (c=='.') pk++;
			else
				{
				for(s=kdate; *s && *s != '.'; s++)
						;
				*s=0;
				}
			/* printf("%s_", kdate); */ fputs(kdate, stdout); putchar('_'); break;
		}
	}
itdump(type);
putchar('\n');
}
static char xxx[50];
char *
lasbl(pin, fl)
	char *pin;
{
char *s, *p, px[100];
char *t;
int inparen=0, normal;
strcpy (s=p=px, pin);
while (*p)p++;
p--;
for (; p>s; p--)
	{
	normal = (p[-1] != '\\');
	if (p[0]==')' && normal) inparen++;
	if (*p=='(' && normal) inparen--;
	if (*p=='(' && normal)
		while (*--p == ' ');
	if (inparen==0 && *p==' ')
		{
		if (p[-1]==',' && fl =='A') /* Jr or II */
			{
			*--p=0;
			continue;
			}
		t=xxx; p++;
		while (*p && *p!=' ') *t++ = *p++;
		*t=0; return(xxx);
		}
	}
fprintf(stderr, "s is %s\n",s);
assert(0);
}
static char *mp[12] ={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
"Aug", "Sep", "Oct", "Nov", "Dec"};
mon (s)
	char *s;
{
	int i;
for(i=0; i<12; i++)
	if (mat3(s, mp[i])) return(i+1);
return(0);
}
mat3(s,t)
	char *s, *t;
{
int k;
for(k=0; k<3; k++)
	if (s[k] != t[k]) return(0);
return(1);
}
char *
lmin(s)
	char *s;
{
char *p;
for(p=s; *p; p++);
while (*--p != '-' && p>s);
return(p+1);
}
char *
artskip(s)
	char *s;
{
if (s[0]=='A' && s[1]== 'n' && s[2] == ' ')
	return(s+3);
if (s[0]=='A' && s[1]==' ')
	return(s+2);
if (s[0]=='T' && s[1]=='h' && s[2]=='e' && s[3] == ' ')
	return(s+4);
return(s);
}
itdump(type) /* print item */
{
int k, skip=0;
char *p;
for(k=0; k<nlines; k++)
	{
	p = ln[k];
	if (skip && !control(p[0])) continue;
	if (type==1 && control(p[0])) 
		switch(p[1])
			{
			case 'J': case 'D':
			case 'R': case 'I':
			case 'K': case 'B':
				skip=1;
				continue;
			}
	if (control(p[0]))
		switch(p[1])
			{
			case 'X': case ']': case '[':
			skip=1;
			continue;
			}
	skip=0;
	/* printf("%c%s", SEP, p); */
	putchar(SEP);
	fputs(p, stdout);
	}
}
auprint(p) /* dump author */
	char *p;
{
int k;
printf("%s_%c_", lasbl(p, 'A'), p[3]);
for(k=7; p[k] == '.'; p += 3) /* lots of initials */
	printf("%c_", p[k-1]);
}
