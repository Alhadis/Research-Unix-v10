# include <stdio.h>
# include <ctype.h>
# include <assert.h>
char *dictdir = "/usr/dict/w7full";
extern char *ggets(), *fgets(), *strchr(), *getenv();
int debug=0;

main(argc,argv)
char *argv[];
{
	char ln[100];
	char *dict = getenv("W7DIR");
	if(dict==0)
		dict = dictdir;
	assert( chdir(dict) == NULL);
	argc--; argv++;
	while (argc>0 && argv[0][0]=='-') {
		switch (argv[0][1])
			{
			case 'd': debug++; break;
			}
		argc--; argv++;
	}
	if (argc<=0) {
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

FILE *desc[26];

lookup (wd)
char *wd;
{
	int ilet, yet, ks;
	char fn[10], dbuff[300], wdb[50], xbuff[300];
	FILE *fd;
	while(*wd && isascii(*wd) && isspace(*wd))
		wd++;
	if(*wd == 0)
		return;
	scnobl(wdb, wd); wd=wdb;
	lcase(wd);
	ccut(wd);
	ilet = *wd;
	assert (ilet>='a' && ilet <= 'z');
	sprintf(fn, "D%c", ilet);
	if (desc[ilet-'a']==0)
		desc[ilet-'a'] = fd = fopen(fn, "r");
	else
		fd = desc[ilet-'a'];
	if (fd==NULL) {
		fprintf(stderr, "Too many files open: %s\n", wd);
		return;
	}
	binpos (fd, wd);
	yet=0;
	while (fgets(dbuff, 300, fd)) {
		if (dbuff[0]=='F' && dbuff[1]==';') {
			scnobl(xbuff, dbuff+2);
			lcase(xbuff);
			ccut(xbuff);
			ks = scomp(xbuff, wd);
			if(debug)fprintf(stderr, "xbuff %s wd %s ks %d\n",xbuff,wd,ks);
			if (ks>0)
				break;
			if (ks==0) yet=1;
		}
		if (yet>0) dump(dbuff);
	}
}

dump(s)
char *s;
{
	char *p;
	if (isupper(s[0]) && s[1]==';')
		s++;
	for(p=s; *p; p++)
		if (*p==';')
			*p=' ';
	fputs(s,stdout);
}

scomp( dl, wd)
char *dl, *wd;
{
	int n, k;
	char t1[100], t2[100];
	scnobl(t1, dl);
	dl=t1;
	scnobl(t2, wd);
	wd=t2;
	n = strlen(wd);
	k = strncmp(dl, wd,n);
	if (k==0 && (dl[n]==0 || dl[n]==';'))
		return(0);
	return(k==0? 1: k);
}

scnobl(s, t)
char *s, *t;
{
	while (*t) {
		if (*t!=' ')
			*s++ = *t;
		t++;
	}
	*s=0;
}

binpos(f, word)
char *word;
FILE *f;
{
	long top, bot, mid;
	int k, c, oc, ooc;
	char line[100];
	bot=0;
	fseek(f, 0L, 2);
	top = ftell(f);
	for (;;) {
		mid = (top+bot)/2;
		fseek(f, mid, 0);
		oc=getc(f);
		c=getc(f);
		mid+=2;
		do {
			ooc=oc;
			oc=c;
			c = getc(f);
			mid++;
		} while (c!=EOF && ( c!='F' || oc!='\n' || ooc=='#'));
		if (fgets(line, 50, f)==NULL)
			break;
		lcase(line);
		if (debug) fprintf(stderr, " bot %ld mid %ld top %ld line %s", bot,mid,top,line);
		ccut(line+1);
		k = scomp(word, line+1);
		if(debug) fprintf(stderr, "  k %d\n",k);
		if (k<=0) {
			if (top<=mid)
				break;
			top = mid;
			continue;
		} else {
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
	for(p=x; *s &&  *s!=';'; s++)
		if (!strchr("-'^`{}:,.~", *s))
			*p++ = *s;
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
