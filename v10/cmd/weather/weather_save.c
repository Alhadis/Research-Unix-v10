# include "stdio.h"
# include "assert.h"
# include "ctype.h"
# include "time.h"
# include "sys/types.h"
# include "sys/stat.h"
FILE *fo, *ft;
char code[10];
int pass=0;
char passes[200][8];
int np=0;

main(argc,argv)
	char *argv[];
/* distributes weather.  codes to pass through are on file in argv[1] */
{
char lbuff[200], *line, *trim();
if (argc>1) passlist(argv[1]);
while (gets(lbuff))
	{
	line = trim(lbuff);
	if (strlen(line)==8 && alldigs(line) && strncmp(line+3, "00",2)==0)
		newcode(line+3);
	if (pass)
		puts(line);
	if (fo)
		fprintf(fo, "%s\n", line);
	}
}
char *
trim(s)
	char *s;
{
char *p;
while (*s &&iscntrl(*s)) s++;
for(p=s; *p; p++)
	if (iscntrl(*p)) *p = ' ';
while (*--p == ' ') *p=0;
return(s);
}
newcode(s)
	char *s;
{
long t, time();
char fn[10], otherf[10];
int i, k1, k2;
struct tm *localtime(), *tp;
struct stat thisbuf, otherbuf;
if (strcmp(s, code)==0) return;
if (fo!=NULL) fclose(fo);
strcpy(code, s);
t = time(0);
tp = localtime(&t);
sprintf(fn, "W%s%c", code, tp->tm_hour>=12 ? 'P' : 'A');
sprintf(otherf, "W%s%c", code, tp->tm_hour<12 ? 'P' : 'A');
k1=stat (fn, &thisbuf);
k2=stat (otherf, &otherbuf);
if (k1==0 && k2==0 && otherbuf.st_mtime > thisbuf.st_mtime)
	unlink(fn);
fo = fopen(fn, "a");
pass=0;
for(i=0; i<np; i++)
	if (strcmp(passes[i], code)==0)
		pass=1;
}
passlist(s)
	char *s;
{
ft = fopen(s, "r");
if (ft==NULL) return;
while (fgets(passes[np], 8, ft))
	passes[np++][5]=0;
}
alldigs(s)
	char *s;
{
while (*s)
	if (!isdigit(*s++))
		return(0);
return(1);
}
