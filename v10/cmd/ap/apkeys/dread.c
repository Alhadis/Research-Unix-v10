# include "stdio.h"
# include "assert.h"
# include "key..c"
# include "sys/types.h"
# include "sys/stat.h"
int nseq =0;
int nsent=0;
int nstory=0;
int ndest= -1;
struct wordstr *wds;
int nwds = 0;
struct matchstr *seqs;
int *ok, *len, *wseen;
struct matchstr *sents;
struct matchstr *stories;
struct deststr *dests;
FILE * zfopen();
extern FILE *yyin;
extern char today[];
stread()
{
char line[400], *calloc(), buff[400];
int i, a, b, c, d, e;
gets(line);
nwds= atoi(line);
wds = (struct wordstr *)calloc(nwds+1, sizeof (*wds));
assert(wds!=0);
for(i=0; i<nwds; i++)
	{
	gets(line);
	sscanf(line, "%s %d %d %d %d", buff, &a, &b, &c, &d);
	wds[i].wch = svc(buff);
	wds[i].seq = a;
	wds[i].ifneg = b;
	wds[i].which = c;
	wds[i].mask = d;
	}
gets(line);
nseq=atoi(line);
seqs = (struct matchstr *)calloc(nseq+1, sizeof(*seqs));
ok = (int *)calloc(nseq+1, sizeof(*ok));
len = (int *)calloc(nseq+1, sizeof(*len));
wseen = (int *)calloc(nseq+1, sizeof(*wseen));
assert(seqs!=0);
for(i=0; i<nseq; i++)
	{
	gets(line);
	sscanf(line, "%d %d %d %d %d", &a, &b, &c, &d, &e);
	seqs[i].next = a;
	seqs[i].ntype = b;
	seqs[i].lim = c;
	seqs[i].smask = d;
	len[i]=e;
	}
gets(line);
nsent=atoi(line);
sents = (struct matchstr *)calloc(nsent+1, sizeof(*sents));
assert(sents!=0);
for(i=0; i<nsent; i++)
	{
	gets(line);
	sscanf(line, "%d %d %d %d", &a, &b, &c, &d);
	sents[i].next = a;
	sents[i].ntype = b;
	sents[i].lim =c;
	sents[i].smask=d;
	}
gets(line);
nstory=atoi(line);
stories = (struct matchstr *)calloc(nstory+1, sizeof(*stories));
assert(stories!=0);
for(i=0; i<nstory; i++)
	{
	gets(line);
	sscanf(line, "%d %d %d", &a, &b, &c);
	stories[i].next = a;
	stories[i].ntype = b;
	stories[i].lim = c;
	}
gets(line);
ndest=atoi(line);
dests = (struct deststr *)calloc(ndest+1, sizeof(*dests));
assert(dests!=0);
for(i=0; i<ndest; i++)
	{
	gets(line);
	dests[i].sndflg = atoi(line);
	assert(strlen(line+2)<400);
	dests[i].dstr = svc(line+2);
	}
fprintf(stderr, "nwds %d nseq %d nsent %d sents %o\n",nwds,nseq,nsent, sents);
fprintf(stderr, "nstory %d ndests %d dests %o\n",nstory, ndest, dests);
fprintf(stderr, "about to exit stread\n");
}
char *yylval;
# define WORD 1
# define DOT 2
extern char *fname;
process()
{
int k, i, j;
yylval= (char *) calloc(10,1); /* get it set up */
for(i=0; i<nseq; i++)
	seqs[i].ct=ok[i]=wseen[i]=0;
for(i=0; i<nsent; i++)
	sents[i].ct=0;
for(i=0; i<nstory; i++)
	stories[i].ct=0;
yyunput('#');
while (k = yylex())
	{
	switch(k)
		{
		case WORD:
			i = search(yylval);
			while (i<nwds && strcmp(wds[i].wch, yylval)==0)
				hit(i++);
			break;
		case DOT:
			senthits();
			for(i=0; i<nsent; i++)
				sents[i].ct=0;
			break;
		}
	for(i=0; i<nseq; i++)
		{
		if (wseen[i] ==len[i] && seqs[i].ct==seqs[i].lim)
			seqhit(i);
		if (wseen[i]>0 && missing(i, wseen[i])) wseen[i]++;
		else
		if (ok[i]==0 || (seqs[i].ct==0) || (wseen[i]>len[i]))
			seqs[i].ct=wseen[i]=0;
		ok[i]=0;
		}
	}
storyhits();
}
hit(i)
{
int j, m, n, ms;
j = wds[i].seq;
ms = wds[i].mask;
switch(wds[i].which)
{
case SEQ:
	ok[j]=1;
	while (missing(j, wseen[j])) wseen[j]++; /* anything we didn't see */
	wseen[j]++;
	seqs[j].ct |= ms;
	if (wseen[j]==len[j] && seqs[j].ct== seqs[j].lim)
		{
		seqhit(j);
		wseen[j]=0;
		}
	return;
case SENT:
	sents[j].ct |= ms;
	return;
case STORY:
	stories[j].ct |= ms;
	return;
case DEST:
	if (wds[i].ifneg==0)
		found(j);
	return;
}
}
seqhit(j)
{
int m, ms;
m = seqs[j].next;
ms = seqs[j].smask;
switch(seqs[j].ntype)
	{
	case SENT:
		sents[m].ct |= ms;
		break;
	case STORY:
		stories[m].ct |= ms;
		break;
	case DEST:
		found(m);
		break;
	}
}
senthits()
{
int m,n;
for(m=0; m<nsent; m++)
	if (sents[m].ct==sents[m].lim)
		{
		n = sents[m].next;
		switch(sents[m].ntype)
			{
			case STORY:
				stories[n].ct |= sents[m].smask;
				continue;
			case DEST:
				found(n);
				continue;
			}
		}
}
storyhits()
{
int n;
for(n=0; n<nstory; n++)
	if (stories[n].ct==stories[n].lim)
		found(stories[n].next);
}
int tosend[400];
int nts=0;
found(nx)
{
int i;
for(i=0; i<nts; i++)
	if (tosend[i]==nx)
		return;
tosend[nts++]=nx;
assert(nts<400);
}
extern int debug;
deliver()
{
char *d, sline[300];
int nx, para, i, sendf;
int gate;
FILE *fi, *fo;
for(i=0; i<nts; i++)
	{
	nx = tosend[i];
	d = dests[nx].dstr;
	if (debug) printf("Send %s to %s\n", fname, d);
	if (d[0]=='*')
		{
		printf("deliver %d to :%s:\n",nx,d+1);
		continue;
		}
	if (d[0]=='|')
		{
		pipeto(d+1, fname, dests[nx].sndflg);
		continue;
		}
	if (d[0]=='>' && d[1]=='>')
		fo = zfopen(d+2, "a");
	else if (d[0]=='>')
		fo = zfopen(d+1, "w");
	if (fo==NULL) /* may be a directory */
		{
		char temp[400];
		while (*d=='>' || *d==' ') d++;
		sprintf(temp, "%s/%s.%s", d, today, fname);
		fo = zfopen(temp, "a");
		}
	if (fo==NULL) continue;
	sendf = dests[nx].sndflg;
	fi = fopen(fname, "r");
	para=0;
	gate = debug ? 1 : 0; /* pass on first line */
	while (fgets(sline, 300, fi))
		{
		if (strncmp(sline, "\t   ", 4)==0) para++;
		if (para==2 && sendf==2) break;
		fputs(sline, fo);
		if (gate>0)
			{
			printf("%s", sline);
			gate=0;
			}
		if (sendf==1) break;
		}
	fclose(fo); fclose(fi);
	}
nts=0;
}
pipeto( command, file, sendf)
	char *command, *file;
{
int fp[2], fr, fw, para, gate;
char sline[300];
FILE *fi, *fo;
assert(pipe(fp)!= -1);
fr = fp[0]; fw = fp[1];
if (fork()==0)
	{
	close(fw);
	close(0);
	assert(dup(fr)==0);
	system(command);
	exit(0);
	}
close(fr);
fi = fopen(file, "r");
fo = fopen("/dev/null", "w");
assert(fi!=NULL);
assert(fo!=NULL);
close(fo->_file);
fo->_file = fw;
para=0;
gate = debug ? 1 : 0;
fprintf(fo, "(%s) ", file);
while (fgets(sline, 300, fi))
	{
	if (strncmp(sline, "\t   ", 4)==0) para++;
	if (para==2 && sendf == 2) break;
	fputs(sline, fo);
	if (gate>0)
		{
		printf("%s", sline);
		gate=0;
		}
	if (sendf==1) break;
	}
fclose(fo);
fclose(fi);
wait(0);
return;
}
search(s)
	char *s;
{
/* binarysearch from 1 to nwds thru wds[].wch */
/* find x such that [x-1]is <s and x big as possible */
int i, j, low, hi, lv, hv;
low=0; hi=nwds-1; i= -10;
while (low+1 < hi)
	{
	i = (low+hi)/2;
	j = strcmp (wds[i].wch, s);
	if (j>=0)
		{
		hi=i;
		continue;
		}
	low=i;
	}
if (i==hi)
	{
	hv = j;
	lv = strcmp(wds[low].wch, s);
	}
else if (i==low)
	{
	lv = j;
	hv = strcmp(wds[hi].wch, s);
	}
else
	{
	lv = strcmp(wds[low].wch, s);
	hv = strcmp(wds[hi].wch, s);
	}
if (lv==0) return(low);
if (hv==0) return(hi);
return(low);
}
missing( i, lni )
{
/* checks whether the next item in a sequence is deliberately missing */
if (lni<0) return(0);
if ((seqs[i].lim & (1<<lni))==0) return(1);
return(0);
}
FILE *
zfopen( fnam, w)
	char *fnam, *w;
{
/* this opens file fnam and if successful, tries to donate
   to the user who owns the including directory */
char td[100];
FILE *f;
struct stat buf;
char *x;
while (*fnam==' ') fnam++;
f = fopen(fnam, w);
if (f==NULL) return(NULL);
strcpy(td, fnam);
for (x=td; *x; x++);
while (x>td && *x!='/') x--;
*x=0;
if (x<=td) return(f);
stat (td, &buf);
chown (fnam, buf.st_uid, buf.st_gid);
return(f);
}
