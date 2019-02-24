# include "stdio.h"
# include "ctype.h"
# include "assert.h"
main(argc,argv)
	char *argv[];
{
char line[100], *s, *p, *r;
int ck;
assert(argc>1);
rdict(argv[1]);
while (gets(line))
	{
	s=line;
	while (*s)
		{
		while (*s && !isalpha(*s))
			putc(*s++, stdout);
		p=s;
		while (isalpha(*s))
			s++;
		ck = *s;
		*s = 0;
		r = lkup(p);
		printf("%s", r ? r: p);
		if (ck)
			{
			printf("%c", ck);
			s++;
			}
		}
	printf("\n");
	}
}
static char * wold[500], * wnew[500];
static char wspace[5000], *wp=wspace;
int k=0;
rdict (fname)
	char *fname;
{
FILE *f;
char line[100],*s;
f = fopen(fname, "r");
assert(f!=NULL);
while (fgets(line, 100, f))
	{
	trimnl(line);
	s = index(line, ' ');
	*s++ = 0;
	wold[k] = wp;
	strcpy(wp, line);
	wp += strlen(line)+1;
	wnew[k]= wp;
	strcpy(wp, s);
	wp += strlen(s)+1;
	k++;
	assert(k<=500);
	assert(wp<=wspace+5000);
	}
}
trimnl(s)
	char *s;
{
while (*s) s++;
if (*--s=='\n') *s=0;
}
lkup(s)
	char *s;
{
int i;
for (i=0; i<k; i++)
	if (strcmp(wold[i], s) ==0)
		return(wnew[i]);
return(NULL);
}
