# include "stdio.h"
# include "assert.h"
# include "ctype.h"
prsum(fn)
	char *fn;
{
char line[100], *s;
FILE *fl;
int k;
	if (fn[0]=='\0')
		return;
	fl = fopen(fn, "r");
	if (fl==NULL) return;
	printf("(%s): ",  fn);
	while (fgets(line, 100, fl))
		if (line[0]=='\t') break;
	for(s=line; *s==' '|| *s=='\t'; s++)
		;
	ctset(12);
	do {
		trimnl(s);
		k = sentend(s);
		ctput(s);
		if (k>0) break;
		} while (fgets(s=line, 100, fl));
	ctout();
	fclose(fl);
}
sentend(s)
	char *s;
{
for( s+=2; *s; s++)
	if (s[0]=='.')
		{
		char *p = s;
		if (s[-2]=='.' || s[-2]==' ')
			continue;
		if (s[-3]==' ' &&strncase(s-2, "st", 2)==0)
			continue;
		if (strncase(s-2, "Mr", 2)==0)
			continue;
		if (strncase(s-3, "Mrs", 3)==0)
			continue;
		if (strncase(s-2, "Ms", 2)==0)
			continue;
		if (strncase(s-3, "Sen", 3)==0)
			continue;
		if (strncase(s-4, "Capt", 4)==0)
			continue;
		if (strncase(s-3, "Col", 3)==0)
			continue;
		if (strncase(s-3, "Rep", 3)==0)
			continue;
		if (strncase(s-2, "Co", 2)==0)
			continue;
		if (strncase (s-3, "Rev", 3)==0)
			continue;
		while (*++p ==' ' || *p == '\t');
		if (p[0]=='\'' && p[1]=='\'')
			{
			p[2]=0;
			return(1);
			}
		if (*p==0 || isupper(*p) )
			{
			s[1]=0;
			return(1);
			}
		}
return(0);
}
static int col= 0;
ctset(n)
{
col=n;
}
ctput(s)
	char *s;
{
int k, m;
m = strlen(s);
if (m>72-col)
	{
	for (k=72-col; k>0 && s[k]!=' '; k--)
		;
	if (k>0)
		{
		s[k++]=0;
		printf("%s\n    ", s);
		}
	else
		printf("\n    ");
	col=4;
	}
else
	k= 0;
s += k;
printf("%s ", s);
col += strlen(s);
}
ctout()
{
printf("\n");
}
strncase(s, t, n)
	char *s, *t;
{
while (n--)
	{
	if ( (*s ^ *t) & 0337)
		return(1);
	if (*s==0) return(0);
	s++; t++;
	}
return(0);
}
