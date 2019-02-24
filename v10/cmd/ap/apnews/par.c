# include "stdio.h"
# include "assert.h"
# include "key..c"
int nseq =0;
int nsent=0;
int nstory=0;
int ndest= -1;
extern int destyet;
# define NW 2000
struct wordstr wds[NW];
int nwds = 0;
int wseen=0;
int final=0;
extern char *udir, *uname;
mkwrd(w, negfl)
	char *w;
{
struct wordstr *wp;
wp = &wds[nwds++];
wp->wch  = w;
wp ->seq= nseq;
wp->ifneg = negfl;
wp->single=0;
wp->mask = (1<<wseen);
if (!negfl)
	final |= (1<<wseen);
wp->which = SEQ;
wseen++;
assert(nwds<NW);
}
int sqseen=0;
#define NSEQ 2000
struct matchstr seqs[NSEQ];
int len[NSEQ];
mkseq()
{
struct matchstr *sp;
len[nseq]=wseen;
sp = &seqs[nseq++];
sp->next = nsent;
sp->lim = final;
sp->smask = (1<<sqseen);
sp->ntype = SENT;
assert (nseq<NSEQ);
if (wseen == 1)
	wds[nwds-1].single=1;
wseen=final=0;
sqseen++;
}
int snseen=0;
#define NSENT 2000
struct matchstr sents[NSENT];
mksent()
{
struct matchstr *sp;
sp = &sents[nsent++];
sp->next = nstory;
sp->lim = (1<<sqseen)-1;
sp->smask = (1<<snseen);
sp->ntype = STORY;
assert (nsent<NSENT);
sqseen=0;
snseen++;
}
#define NSTORY 900
struct matchstr stories[NSTORY];
mkstory()
{
struct matchstr *sp;
if (destyet==0) defdest();
sp = &stories[nstory++];
sp->next = (ndest>0 ? ndest : 0);
sp->lim = (1<<snseen)-1;
sp->ntype = DEST;
assert (nstory<NSTORY);
snseen=0;
}
# define ND 200
struct deststr dests[ND];
int stuff = 3;
svwh(x)
	char *x;
{
ndest++;
destyet=1;
dests[ndest].dstr = x;
dests[ndest].sndflg = stuff;
dests[ndest].homedir = udir;
}
optimize()
{
int i;
struct wordstr *wp;
struct matchstr *sp, *xp;
for(i=0; i<nsent; i++)
	{
	xp = &sents[i];
	sp = &stories[xp->next];
	if (xp->smask == sp->lim)
		{
		xp->next = sp->next;
		xp->ntype = sp->ntype;
		xp->smask = sp->smask;
		sp->ntype=0;
		}
	}
for(i=0; i<nseq; i++)
	{
	xp = &seqs[i];
	sp = &sents[xp->next];
	if (xp->smask == sp->lim)
		{
		xp->next = sp->next;
		xp->ntype = sp->ntype;
		xp->smask = sp->smask;
		sp->ntype=0;
		}
	}
for(i=0; i<nwds; i++)
	{
	wp = &wds[i];
	sp = &seqs[wp->seq];
	if (wp->single)
		{
		wp->seq = sp->next;
		wp->which = sp->ntype;
		wp->mask = sp->smask;
		if (wp->ifneg)
			{
			switch(wp->which)
				{
				case SENT:
					sents[wp->seq].lim &= (~wp->mask);
					break;
				case STORY:
					stories[wp->seq].lim &= (~wp->mask);
					break;
				}
			}
		sp->ntype=0;
		}
	}
}
compress()
{
int i, j, k;
struct matchstr *sp, *tp;
for(i=0, j=nsent-1; i<j; i++)
	{
	if (sents[i].ntype!=0)
		continue;
	while (sents[j].ntype==0)
		j--;
	if (i>=j) break;
	sents[i]=sents[j];
	sents[j].ntype=0;
	for(k=0; k<nwds; k++)
		{
		assert(!(wds[k].which==SENT && wds[k].seq==i));
		if (wds[k].seq == j && wds[k].which == SENT)
			wds[k].seq=i;
		}
	for(k=0; k<nseq; k++)
		{
		assert(!(seqs[k].ntype==SENT && seqs[k].next==i))
		if (seqs[k].next==j && seqs[k].ntype==SENT)
			seqs[k].next=i;
		}
	}
if (sents[i].ntype) i++;
nsent = i;
for(i=0, j=nseq-1; i<j; i++)
	{
	if (seqs[i].ntype!=0)
		continue;
	while (seqs[j].ntype==0)
		j--;
	if (i>=j) break;
	seqs[i]=seqs[j];
	seqs[j].ntype=0;
	len[i]=len[j];
	for(k=0; k<nwds; k++)
		{
		assert(!(wds[k].which==SEQ && wds[k].seq==i));
		if (wds[k].seq == j && wds[k].which == SEQ)
			wds[k].seq=i;
		}
	}
if (seqs[i].ntype) i++;
nseq = i;
for(i=0, j=nstory-1; i<j; i++)
	{
	if (stories[i].ntype!=0)
		continue;
	while (stories[j].ntype==0)
		j--;
	if (i>=j) break;
	stories[i]=stories[j];
	stories[j].ntype=0;
	for(k=0; k<nwds; k++)
		{
		assert(!(wds[k].which==STORY && wds[k].seq==i));
		if (wds[k].seq == j && wds[k].which == STORY)
			wds[k].seq=i;
		}
	for(k=0; k<nseq; k++)
		{
		assert(!(seqs[k].ntype==STORY && seqs[k].next==i))
		if (seqs[k].next==j && seqs[k].ntype==STORY)
			seqs[k].next=i;
		}
	for(k=0; k<nsent; k++)
		{
		assert(!(sents[k].ntype==STORY && sents[k].next==i))
		if (sents[k].next==j && sents[k].ntype==STORY)
			sents[k].next=i;
		}
	}
if (stories[i].ntype) i++;
nstory = i;
}
dump()
{
optimize();
compress();
prvecs();
}
prvecs()
{
int i;
int comp(), exch();
shell (nwds, comp, exch);
printf("%d entries in word list; word, seq, negfl, which, mask\n", nwds);
for(i=0; i<nwds; i++)
	printf("%s %d %d %d %d\n", wds[i].wch, wds[i].seq, wds[i].ifneg, wds[i].which, wds[i].mask);
printf("%d entries in sequence list; sent, ntype, lim, mask, length\n", nseq);
for(i=0; i<nseq; i++)
	printf("%d %d %d %d %d\n", seqs[i].next, seqs[i].ntype, seqs[i].lim, seqs[i].smask, len[i]);
printf("%d entries in sentence list; story, ntype, lim, mask\n", nsent);
for(i=0; i<nsent; i++)
	printf("%d %d %d %d\n", sents[i].next, sents[i].ntype, sents[i].lim, sents[i].smask);
printf("%d entries in story list; dest, ntype, lim\n", nstory);
for(i=0; i<nstory; i++)
	printf("%d %d %d\n", stories[i].next, stories[i].ntype, stories[i].lim);
printf("%d entries in destination list; level, dest\n", ndest+1);
for(i=0; i<=ndest; i++)
	{
	char *p, *s;
	p = dests[i].dstr;
	while (*p=='>' || *p==' ' || *p=='\t')
		p++;
	if (*p=='|' || *p=='/')
		printf("%d %s\n", dests[i].sndflg, dests[i].dstr);
	else
		{
		printf("%d ", dests[i].sndflg);
		for(s=dests[i].dstr; s<p; s++)
			putchar(*s);
		printf("%s/%s\n", dests[i].homedir, p);
		}
	}
}
svsz(n)
{
stuff=n;
}
defdest()
{
static char dbuff[20];
	ndest++;
	sprintf(dbuff, "| mail %s", uname);
	dests[ndest].dstr = svc(dbuff);
	dests[ndest].sndflg=3;
	destyet=1;
	}
comp(i,j) /* compares word list for i and j; true if ok */
{
return (strcmp (wds[i].wch, wds[j].wch) <= 0);
}
exch(i,j)
{
struct wordstr t;
t = wds[i];
wds[i]=wds[j];
wds[j]=t;
}
