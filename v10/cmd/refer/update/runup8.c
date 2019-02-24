# include "runup.h"
long *histrt, *hnstrt, *hnew;
fixib()
{
/* straighten up ia, ib */
FILE *fbo, *fbn, *fno, *ft;
long getl(), mappos();
int it, lgind, lgnn, nocc;
int *hfreq, did1, did2;
long l1, l2, nowat;
struct bibfile *bp;
struct newfile *np;
if (chatty) printf("Alterations read\n");
fbo = fopen("Ind.ib", "r");
fbn = fopen("XInd.ib", "w");
fno = fopen("NN.ib", "r");
assert(fbo!=NULL); assert(fbn!=NULL); assert(fno!=NULL);
/* read in old hash pointers */
ft = fopen("Ind.ia", "r");
it = getw(ft);
assert(it==nhash);
lgind = getw(ft);
histrt= calloc(nhash, sizeof(long));
for(it=0; it<nhash; it++)
	histrt[it] = getl(ft);
fclose(ft);
/* read in supplementary hash pointers */
ft = fopen("NN.ia", "r");
it = getw(ft);
assert(it==nhash);
lgnn = getw(ft);
hnstrt= calloc(nhash, sizeof(long));
for(it=0; it<nhash; it++)
	hnstrt[it] = getl(ft);
fclose(ft);

# if D3
/* temporarily, dump the NN hash file */
for(it=0; it<nhash; it++)
	{
	fseek (fno, hnstrt[it], 0);
	while ((l2 = lgnn ? getl(fno) : (long)(getw(fno)))>=0)
		printf("hash %d in %ld\n",it, l2);
	}
# endif
# if D1
printf("in fixib, lgind %d lgnn %d lgout %d nhash %d\n",lgind,lgnn,lgout, nhash);
# endif
/* now read through the hash files */
hnew = calloc(nhash, sizeof(long));
hfreq = calloc(nhash, sizeof(int));
nowat=0;
for(it=0; it <nhash; it++)
	{
	if (verbose && (it%100)==0)
		printf("at hash %d got %.1f of %d sec.\n", it, tuse(), tlapse());
	fseek (fbo, histrt[it], 0);
	fseek (fno, hnstrt[it], 0);
	hnew[it]=nowat;
	bp = bibs;
	nocc = 0;
	did1 = did2 = 0;
	np = news;
	l1 = lgind ? getl(fbo) : (long)(getw(fbo));
	l2 = lgnn  ? getl(fno) : (long)(getw(fno));
	if (l1 == -1 && l2 == -1) 
		{
# if D1
		printf("  none at hash %d\n",it);
# endif
		if (nowat > 0)
			hnew[it] = nowat - (lgout ? 4 : 2);
		else
			lgout ? putl(-1L,fbn) : putw(-1,fbn);
		nowat =ftell(fbn);
		hfreq[it] = 0;
		continue;
		}
# if D1
	printf("Seeding at hash %d l1 %ld l2 %ld\n",it,l1,l2);
# endif
	while (l1 != -1 || l2 != -1)
		{
# if D2
		if(vhash==it) printf("loop: l1 %ld l2 %ld bp %d np %d\n",l1,l2,bp-bibs,np-news);
# endif
		if (l1>=0 && did1==0)
			{
			for (; bp->bibfno >=0 && bp->bibival<l1; bp++)
				;
# if D2
			if(vhash==it) printf("bp now %d\n",bp-bibs);
# endif
			if (bp->bibfno >=0 && l1 == bp->bibival)
				{
# if D2
				if(vhash==it) printf("Matched bibval, ignoring %ld\n",l1);
# endif
				l1 = lgind ? getl(fbo) : (long)(getw(fbo));
				did1=0;
# if D2
				if(vhash==it) printf("New l1 read as %ld\n",l1);
# endif
				continue;
				}
			l1 = mappos(l1);
# if D2
			if(vhash==it) printf("after mappos offset l1 %ld\n",l1);
# endif
			if (l1>=ins1) l1 += add;
# if D2
			if(vhash==it) printf("l1 finally is %ld\n",l1);
# endif
			did1=1;
			}
		if (l2>=0 && did2==0)
			{
			for(; np->nn_ic>=0 && np->nn_ic < l2; np++);
			l2 = np->new_ic;
# if D2
			if(vhash==it) printf("np at %d l2 now %ld\n",np-news,l2);
# endif
			if (l2>=ins2) l2 += add;
# if D2
			if(vhash==it) printf("l2 finally is %ld\n",l2);
# endif
			did2=1;
			}
		if (l1 >=0 && (l2 == -1 || l1<l2))
			{
# if D2
			if(vhash==it) printf("Writing l1 %ld not l2 %ld lgout %d\n",l1,l2,lgout);
# endif
			lgout ? putl(l1,fbn) : putw((int)l1,fbn);
			l1 = lgind ? getl(fbo) : (long)(getw(fbo));
			did1=0;
# if D2
			if(vhash==it) printf("New l1 is %ld\n",l1);
# endif
			nocc++;
			continue;
			}
		if (l2 >= 0 && (l1 == -1 || l2<l1))
			{
# if D2
			if(vhash==it) printf("Writing l2 %ld not l1 %ld lgout %d \n",l2,l1,lgout);
# endif
			lgout ? putl(l2,fbn) : putw((int)l2,fbn);
			l2 = lgnn ? getl(fno) : (long)(getw(fno));
# if D2
			if(vhash==it) printf("New l2 %ld\n",l2);
# endif
			did2=0;
			nocc++;
			continue;
			}
# if D1
	printf("bad case l1 %ld l2 %ld\n",l1,l2);
# endif
		assert(0);
		}
	lgout ? putl(-1L, fbn) : putw(-1, fbn);
	nowat = ftell(fbn);
	hfreq[it] = nocc;
	}
fclose(fbn);
fbn = fopen("XInd.ia", "w");
assert(fbn!=NULL);
putw(nhash, fbn);
putw(lgout, fbn);
for(it=0; it<nhash; it++)
	putl(hnew[it], fbn);
for(it=0; it<nhash; it++)
	putw(hfreq[it], fbn);
fclose(fbn);
}
long
getl(f)
	FILE *f;
{
long in;
int j;
j = fread(&in, sizeof(long), 1, f);
if (j<=0) return(-1L);
return(in);
}
putl(l, f)
	long l;
	FILE *f;
{
int i,j;
i = l>>16;
j = l;
putw(i,f);
putw(j,f);
}
