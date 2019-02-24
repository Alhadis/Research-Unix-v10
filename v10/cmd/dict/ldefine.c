# include <stdio.h>
# include <ctype.h>
# include <assert.h>
#include <libc.h>
char *dictdir = "/usr/dict/oaldce";
extern char *ggets(), *fgets(), *strchr();
char *jbrk=" ";
main(argc,argv)
	char *argv[];
{
char ln[100];
char *dict = getenv("OALDCEDIR");
if(dict==0)
	dict = dictdir;
assert( chdir(dict) == NULL);
argc--; argv++;
if (argc<=0)
	{
	while (ggets(ln,sizeof ln,stdin))
		lookup(ln);
	}
while (argc-- >0)
	lookup(*argv++);
}
lcase (s)
	char *s;
{
for (; *s; s++)
	if (isupper(*s))
		*s = tolower(*s);
}
char *ltab[] ={
"a", "b", "c", "d", "e", "f", "g", "h", "ijk", "ijk", "ijk", "l", "m",
"no", "no", "p", "q", "r", "s", "t", "uvw", "uvw", "uvw", "xyz", "xyz", "xyz"};
lookup (wd)
	char *wd;
{
int ilet, nwd, yet, ks;
char fn[10], dbuff[300], xbuff[300];
FILE *fd;
while(*wd && isascii(*wd) && isspace(*wd))
	wd++;
if(*wd == 0)
	return;
lcase(wd);
ccut(wd);
ilet = *wd;
assert (ilet>='a' && ilet <= 'z');
sprintf(fn, "D%s", ltab[ilet-'a']);
fd = fopen(fn, "r");
if (fd==NULL)
	{
	fprintf(stderr, "Confused by %s\n", wd);
	return;
	}
binpos (fd, wd);
yet=0;
jbrk="";
while (fgets(dbuff, sizeof dbuff, fd))
	{
	if (dbuff[0]!= '\n' && dbuff[0]!= ' ')
		{
		if (yet>0) lbrk();
		strcpy(xbuff, dbuff);
		lcase(xbuff);
		ccut(xbuff);
		ks = scomp(xbuff, wd);
		if (ks>0)
			break;
		if (ks==0) yet=1;
		}
	if (yet>0) dump(dbuff, wd);
	}
}
scomp( dl, wd)
	char *dl, *wd;
{
int n, k;
n = strlen(wd);
	k= strncmp(dl, wd,n);
	if (k==0 && (dl[n]==' ' || dl[n]==','))
		return(0);
	return(k==0? 1: k);
}
static char cbuff[200];
static char *cp = cbuff;
static int col=0;
dump(s, wd)
	char *s, *wd;
{
int c;
while (c = *s++)
	{
	if (c=='\n')c=' ';
	if (c!='@')
		*cp++ = c;
	else
		{
		if (cp[-1]==toupper(wd[0]))
			strcpy(cp, wd+1);
		else
			strcpy(cp, wd);
		cp += strlen(cp);
		}
	}
*cp=0;
if (strlen(cbuff)<80)
	{
	put(jbrk, cbuff);
	jbrk = " ";
	cp=cbuff;
	*cp=0;
	}
else
	{
	while (strlen(cbuff)>78)
		{
		for(cp=cbuff+78; cp>cbuff && *cp!=' '; cp--)
			;
		*cp++=0;
		put(jbrk, cbuff);
		strcpy(cbuff, cp);
		cp= cbuff+strlen(cbuff);
		*cp=0;
		jbrk= " ";
		}
	}
}
put (s, t)
	char *s, *t;
{
if (*s==' ' && *t==' ')
	printf("%s\n",t);
else
	printf("%s%s\n",s,t);
}
lbrk()
{
printf("%s\n",cbuff);
cp=cbuff;
jbrk = "";
}
binpos(f, word)
	char *word;
	FILE *f;
{
long top, bot, mid;
int k, c, oc;
char line[100];
bot=0;
fseek(f, 0L, 2);
top = ftell(f);
for (;;)
	{
	mid = (top+bot)/2;
	fseek(f, mid, 0);
	c=0;
	do {
		oc=c;
		c = getc(f);
		mid++;
	   } while (c!=EOF && ( c !='\n' || oc != '\n'));
	if (fgets(line, sizeof line, f)==NULL)
		break;
	lcase(line);
	ccut(line);
	k = strcmp(word, line);
	if (k<=0)
		{
		if (top<=mid)
			break;
		top = mid;
		continue;
		}
	else
		{
		bot= mid;
		continue;
		}
	}
fseek(f, bot, 0);
if (bot!=0)
while ( (c=getc(f)) != '\n' && c!= EOF);
}
ccut(s)
	char *s;
{
char x[100], *p, *y;
y=s;
for(p=x; *s &&  *s!=' '; s++)
	if (!strchr("-'_()\"", *s))
		*p++ = *s;
while (*s)
	*p++ = *s++;
*p=0;
strcpy(y, x);
}

char *
ggets(s,n,f)
char *s;
FILE *f;
{
	char *p = fgets(s,n,f);
	char *q;
	if(p && (q=strchr(s,'\n')))
		*q = 0;
	return p;
}
