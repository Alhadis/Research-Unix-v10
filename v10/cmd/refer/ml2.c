# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# define SEP 001
# define SAME 1
# define DIFFERENT 0
main(argc,argv)
	char *argv[];
{
char bf[2000],*p, *s;
char oname[100], tname[100];
int ct, type, ntype;
oname[0] = 0;
ct = type = 0;
while (gets(bf))
	{
	p=bf;
	gname(tname,p);
	/* if a date we don't care about month */
	datetrim(tname);
	if (compare(tname, oname) != SAME)
		{
		if (ct > 0)
			printf(".ct %d\n", ct);
		printf(".nn %s\n", tname);
		ct = 0;
		type = 0;
		}
	strcpy (oname, tname);
	ntype = gtype(p);
	if (ntype != type)
		printf(".ty %d\n", ntype);
	type = ntype;
	while (*p++ != SEP);
	for (s=p; *s; s++)
		{
		assert (s<bf+2000);
		if (*s == SEP)
			*s = '\n';
		}
	while (*p=='\n')p++;
	puts(".br\n.[");
	puts(p);
	puts(".]");
	ct++;
	}
if (ct>0) printf(".ct %d\n",ct);
}

gname(buf, s)
	char *buf, *s;
{
	int c;
for ( ; (c= *s) && c != SEP && c != '*'; s++)
	*buf++ = (c != '_' ? c : ' ');
*buf=0;
}
gtype(s)
	char *s;
{
while (*s && *s!= '*')
	s++;
return (*s ? s[1] - '0' : 0);
}
compare(s1, s2)
	char *s1, *s2;
{
/* compare last names */
while (*s1++== *s2++)
	{
	if (*s1==0) return(SAME);
	if (*s1==' ') break;
	}
if (*s1 != ' ' || *s2 != ' ') return(DIFFERENT);
while (*s1==' ') s1++;
while (*s2==' ') s2++;
#/* compare first initials */
if (*s1++ != *s2++) return (DIFFERENT);
while (*s1==' ') s1++;
while (*s2==' ') s2++;
/* if no middle initials OK */
if (*s1==0 || *s2==0) return(SAME);
/* but if middle initials must be same */
if (*s1==*s2) return(SAME);
return (DIFFERENT);
}
datetrim(p)
	char *p;
{
if (isdigit(p[0])&& isdigit(p[1]))
	{
	while(isdigit(*p))p++;
	*p=0;
	}
}
