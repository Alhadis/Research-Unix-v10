# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "signal.h"
# include "sys/types.h"
# include "sys/stat.h"
# include "cbt.h"
# include "time.h"
char names[20][14];
char heads[20][50];
# define NST 200
char storylist[NST][12];
int stc=0;
int intr();
char *getlogin(), *ctime();
struct tm *localtime();
int day, yesterday;
int stp=0;
int inolim=0;
int inodes[NST+5];
char *getenv(), *getpwd();
char nowfile[20]="";
char nowhead[50];
char pwdir[50];
long filtim();
char *storydir = "/usr/spool/ap";

/* logging */
char *apnlog = "/usr/spool/ap/apnlog";
long tmstart, tmend;
int nslug, nstory, nsearch;

main(argc,argv)
	char *argv[];
{
char line[100];
int i, lc=1;
long l;
char *ctp, *userp;
char forcebuf[BUFSIZ];
setbuf(stdout, forcebuf);
for (argc--,argv++; argc>0 && argv[0][0]=='-'; argc--,argv++)
	switch(argv[0][1])
		{
		case 'f':
			storydir = argv[1];
			argv++; argc--;
			break;
		}
tmstart = l = time(0);
day = localtime(&l)->tm_mday;
l -= 86400; /* go back one day */
yesterday = localtime(&l)->tm_mday;
signal (SIGINT, intr);
strcpy(pwdir, getpwd());
i = chdir(storydir);
if (i!=0)
	{
	fprintf(stderr, "no directory %s\n",storydir);
	exit(1);
	}
top();
extend(20);
display();
menu(lc);
fflush(stdout);
extend(30);
while (gets(line))
	{
	tpos(0,0);
	if (line[0]!=0)
		rate();
	switch(line[0])
		{
		case '0': case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': case '9': 
			i = atoi(line)-1;
			if (i<0 || i>19) continue;
			pr(names[i], heads[i]);
			nstory++;
			lc=0;
			break;
		case 0: /* continue this story */
			if (lc==0)
				pr(0,0);
			else if (lc==1)
				display();
			else if (lc==2)
				sums();
			if (lc>0) menu(lc);
			break;
		case '?':
			menu(0);
			break;
		case '!':
			system(line+1);
			break;
		case '.':
			disprint();
			break;
		case 'a':
			pr(line, "");
			nstory++;
			lc=0;
			break;
		case '+':
		case 'm':
			shorten(line+1);
			display();
			lc=1;
			break;
		case 'M':
			sums();
			menu(lc=2);
			break;
		case 't':
			top();
			extend(20);
			shorten(line+1);
			display();
			menu(lc=1);
			break;
		case 'T':
			top();
			extend(20);
			sums();
			menu(lc=2);
			break;
		case 'c':
		case 'C': /* copy file to my directory */
			i = atoi(line+1);
			if (i>0)
				strcpy(nowfile, names[i-1]);
			copyto(line+1, nowfile);
			break;
		case 'D': /* delete file*/
			i = atoi(line+1);
			if (i>0)
				strcpy(nowfile, names[i-1]);
			printf("Deleting /%s/\n",nowfile);
			unlink(nowfile);
			break;
		case 'Y':
		case 'y': /* must get keyword list */
			getkeys(line[0]=='Y');
			break;
		case 'S':
		case 's': /* search for words */
			wdsearch (line+1, line[0]=='S');
			nsearch++;
			break;
		case 'q':
			tpos(23,0);
			wrap();
		}
	fflush(stdout);
	}
tpos(23,0);
wrap();
}
intr()
{
tpos(23,0);
wrap();
exit(0);
}
int llens[10] ={70,70,70,70,70,70,70,70,70,70};
pr (s, h)
	char *s, *h;
{
static FILE *f=NULL;
char line[120];
int i, k, k1;
if (s && s[0]==0)
	return;
if (s!=0)
	{
	if (f!= NULL) fclose(f);
	f = fopen(s, "r");
	if (f==NULL)
		{
		printf("No file %s\n", s);
		return;
		}
	strcpy(nowfile, s);
	strcpy(nowhead, h);
	}
else
	if (f==NULL) return;
tpos(12,0);
if (ifclear())
	clr2end();
for(i=0; i<10; i++)
	{
	if(fgets(line, 100, f)==0)
		break;
	trimnl(line);
	k = strlen(line);
	if (line[0]=='\t') k+= 7;
	if (!ifclear() &&ifclrline())
		clrline();
	fputs(line, stdout);
	if (!ifclear() && !ifclrline())
		for(k1=llens[i]; k1>k; k1--)
			putchar(' ');
	printf("\n");
	llens[i]=k+1;
	}
}
trimnl(s)
	char *s;
{
while (*s) s++;
if (*--s == '\n') *s=0;
}
static hfgate=0;
static char hfbuff[5000];
static char *hflab[400];
static int hftop=0;
hfast (out, snam)
	char *out, *snam;
{
char *s;
int i;
if (hfgate==0)
	{
	int f, n;
	char fn[40], *index();
	sprintf(fn, "%s/.S%d", storydir, day);
	f = open(fn, 0);
	if (f<0) return(0);
	n= read (f, hfbuff, 5000);
	hfbuff[n]=0;
	close(f);
	hfgate=1;
	s = hfbuff;
	for(hftop=0; hftop<400; hftop++)
		{
		if (*s==0) break;
		hflab[hftop] = s;
		s = index(s, '\n');
		if (s==NULL) break;
		*s++ = 0;
		}
	}
for(i=0; i<hftop; i++)
	{
	if (strncmp(snam, hflab[i], 5)==0 && hflab[i][6] !=0)
		{
		strcpy(out, hflab[i]+6);
		return(1);
		}
	}
return(0);
}
hget(s, f)
	char s[];
	FILE *f;
{
int i, c, nw;
char line[100], *wv[40], *p;
if (f==NULL)
	{
	*s=0;
	return;
	}
fgets(line, 100, f);
trimnl(line);
if (line[1]==' ' && line[3]==' ' && line[6]=='-') /* their heading */
	{
	for(p=line+7; (c = *p) && c!=' '; p++)
		*s++ = c;
	*s=0;
	return;
	}
nw = getargs(line, wv);
strcpy(s, wv[i=pickbig(wv, nw)]);
wv[i]="";
strcat(s, wv[pickbig(wv,nw)]);
return;
}
notnew(i, inodes, fname)
	char *fname;
	int *inodes;
{
int ino, j;
struct stat stbuff;
stat (fname, &stbuff);
ino = stbuff.st_ino;
for(j=0; j<i; j++)
	if (ino==inodes[j])
		return(1);
inodes[i]=ino;
return(0);
}
display()
{
getnam(20);
disprint();
nslug += 20;
}
disprint()
{
int i, kl, a;
char bform[30];
FILE *fl;
startsc();
for(i=0; i<20; i++)
	{
	if (names[i][0]== '\0')
		break;
	if (hfast(heads[i], names[i]))
		continue;
	fl = fopen(names[i], "r");
	if (fl!=NULL)
		{
		hget(heads[i],fl);
		fclose(fl);
		}
	else
		heads[i][0]=names[i][0]=0;
	}
for(kl=i=0; i<10; i++)
	{
	a = strlen(heads[i]);
	if (a>kl) kl=a;
	}
kl += 6;
if (kl>30) kl=30;
sprintf(bform, "%%3d: (%%5s) %%-%d.%ds ", kl, kl);
for(i=0; i<10; i++)
	{
	char xheads[50], yheads[50];
	clrline();

	sprintf(xheads, "%s [%c]", heads[i], scr(names[i]));
	sprintf(yheads, "%s [%c]", heads[i+10], scr(names[i+10]));
	if (strlen(names[i])<=5)
		printf(bform, i+1, names[i], xheads);
	else
		printf("%3d: (%11s) %-19.19s ",i+1,names[i],xheads);
	printf("%3d: (%5s) %.30s\n",i+11,names[i+10],yheads);
	}
nowfile[0]=0;
}
top()
{
	char tstories[NST][6];
	int f, n, i, j;
	stc = inolim = stp = 0;
	f = open("..tlist", 0);
	if (f<0) return;
	n = read(f, tstories, NST*6);
	stc = n/6;
	/* reverse tstories */
	for(i=0, j=stc-1; i<j; i++,j--)
		{
		char x[6];
		strncpy(x,tstories[i],6);
		strncpy(tstories[i],tstories[j],6);
		strncpy(tstories[j],x,6);
		}
	close(f);
	for(i=0; i<stc; i++)
		{
		strncpy(storylist[i],tstories[i],6);
		storylist[i][6]= 0;
		}
}
getnam(n)
{
int i;
for (i=0; i<n; i++)
	heads[i][0]= names[i][0]= '\0';
for (i=0; i<n; i++)
	{
		try:
		if (stp>=stc) break;
		strcpy(names[i], storylist[stp++]);

		if (!interest(names[i])) goto try;
		if (names[i][0]=='a' && isdigit(names[i][1]))
			continue;
		if (names[i][5]=='.' && names[i][6]=='a' &&
		isdigit(names[i][7]))
			continue;
		goto try;
	}
}
sums()
{
int i;
startsc();
getnam(10);
for (i=0; i<10; i++)
	{
	printf("%d ",i+1);
	prsum(names[i]);
	}
}
wrap()
{
FILE *f;
tmend = time(0);
f = fopen(apnlog, "a");
if (f!=NULL)
	{
	fprintf(f,
	"%s slugs %d stories %d searches %d dir %s %.16s %d secs\n",
		getlogin(), nslug, nstory, nsearch, storydir,
		ctime(&tmstart), tmend-tmstart);
	fclose(f);
	}
exit(0);
}
rate()
{
char line[100]; int r;
if (nowfile[0]==0) return;
tpos(12,0);
if (ifclear())
	clr2end();
else
if (ifclrline())
	{clrline(); tpos(13,0); clrline();}
tpos(12,0);
printf("Please rate story %s: 1 bad, 5 good: ", nowhead);
fflush(stdout);
gets(line);
r = atoi(line);
if (r<0) r=0;
if (r>5) r=5;
upd(nowfile, r);
}

short nread[1000];
short neval[1000];
short tscor[1000];
short sqscr[1000];
static char * scname = "Scores";
short nvl[1000];
int ngate=0;
upd(nm, r)
{
int f, n;
n = atoi(nm+1);
if (n<=0 || n>999) return;
f = open(scname, 0);
if (f>0)
	{
	ngate=1;
	read(f, nread, 2000);
	read(f, neval, 2000);
	read(f, tscor, 2000);
	read(f, sqscr, 2000);
	close(f);
	}
if (r>10) return;
nread[n]++;
if (r>0)
	{
	neval[n]++;
	tscor[n]+=r;
	sqscr[n]+= r*r;
	}
f = creat(scname, 0666);
if (f>0)
	{
	write(f, nread, 2000);
	write (f, neval, 2000);
	write (f, tscor, 2000);
	write (f, sqscr, 2000);
	close(f);
	}
}
scr(s)
	char *s;
{
int n;
double x;
if (ngate==0) upd (" 100", 100);
n = atoi(s+1);
if (neval[n]==0)
	return(' ');
x = (double) tscor[n];
x /= neval[n];
x += 0.5;
n = (int) x;
return(n+'0');
}
ctr(s)
	char *s;
{
int n;
n = atoi(s+1);
return(nread[n]);
}
int intlevel=0;
shorten(s)
	char *s;
{
if (*s!='>') intlevel=0;
else
intlevel = atoi(s+1);
if (intlevel>5) intlevel=5;
}
interest(s)
	char *s;
{
int n;
if (intlevel<=0) return(1);
n = atoi(s+1);
upd("", 100);
if (neval[n]==0) return(0);
if ( (double)tscor[n]/neval[n] > intlevel) return(1);
return(0);
}
