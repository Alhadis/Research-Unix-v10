# include "runup.h"
copysource()
{
struct bibfile *bibp;
FILE *indco;
FILE *indcn;
FILE *fso;
FILE *fnew;
char indln[100], nowopen[100], newver[100], outline[100];
int nbyte, off=0;
long nowpos, copyn();
int ind_lg; char ind_nm[100]; long ind_st;
long indpos =0L, copl=0L, cnpl=0L;
bibp=bibs;
if (bibp->bibfno<0) /* no bib files */
	{
	link("Ind.ic", "XInd.ic");
	return;
	}
nowopen[0]=0;
indco = fopen("Ind.ic", "r");
assert(indco!=NULL);
indcn = fopen("XInd.ic", "w");
assert(indcn!=NULL);
while (fgets(indln, 100, indco))
	{
	register char *s1, *s2;
	extern long atol();
	copl += strlen(indln);
	/* sscanf(indln, "%[^:]:%ld,%d", ind_nm, &ind_st, &ind_lg); */
	/* the previous line has been replaced for efficiency's sake */
	s1 = ind_nm; s2 = indln;
	while (*s2!=':') *s1++ = *s2++;
	*s1=0;
	ind_st = atol(++s2);
	while (*s2++!=',');
	ind_lg = atoi(s2);
	if (bibp->bibfno>=0)
	switch (bcmp( bibp-> bibfref, bibp-> bibfstrt, ind_nm, ind_st))
		{
		case '>': break;
		case '=':
# if D1
printf("match %s %ld %d\n",ind_nm,ind_st,ind_lg);
# endif
		if (chatty) printf("bib%03d\n",bibp->bibfno);
		if (strcmp(bibp->bibfref, nowopen)!=SAME)
			{
			if (nowopen[0])
				wrapo(fso,fnew,  nowopen);
			strcpy (nowopen, bibp->bibfref);
			fso=fopen(nowopen, "r");
			assert(fso!=NULL);
			sprintf(newver, "%s.x", nowopen);
			fnew = fopen(newver, "w");
			assert(fnew!=NULL);
			nowpos=0;
			off=0;
			if (verbose) printf("Copying %s got %.1f of %d sec.\n",nowopen,tuse(),tlapse());
			}
# if D1
printf("copying from %ld to %ld\n", nowpos, bibp->bibfstrt);
# endif
		nowpos += copyn(fso, fnew, bibp->bibfstrt-nowpos);
# if D2
printf("nowpos now %ld\n",nowpos);
# endif
		nowpos += skipn(fso, bibp->bibfleng);
# if D2
printf("now pos now %ld\n",nowpos);
# endif
		nbyte= copyb(fnew, bibp);
# if D1
printf("nbyte %d fleng %d off %d\n",nbyte,bibp->bibfleng, off);
# endif
		if (nbyte>0)
		sprintf(outline, "%s:%ld,%d\n", ind_nm, ind_st+off, nbyte);
		fputs(outline, indcn);
		cnpl += strlen(outline);
		off += (int)(nbyte-bibp->bibfleng);
		bibp->bibival = indpos;
		ckpos(copl, cnpl);
# if D1
	printf("item copied; bibst %ld,%d off now %d\n",bibp->bibfstrt,bibp->bibfleng,off);
# endif
		bibp++;
		continue;
		case '<':
			fprintf(stderr, "Missing bib%03d ref %s:%ld,%d\n",
			bibp->bibfno, bibp->bibfref, bibp->bibfstrt,bibp->bibfleng);
			bibp++;
		}
	if (off!=0 &&strcmp(lastsl(nowopen), ind_nm)!=SAME)
		{
# if D1
		printf("Reset offset since %s not %s\n",nowopen, ind_nm);
# endif
		off=0;
		}
	sprintf(outline, "%s:%ld,%d\n", ind_nm, ind_st+off, ind_lg);
	fputs(outline, indcn);
	cnpl += strlen(outline);
	ckpos(indpos=copl, cnpl);
	}
if (nowopen[0])
wrapo(fso,fnew, nowopen);
fclose(indco); fclose(indcn);
}
long
copyn(fo, fn, l)
	FILE *fo, *fn;
	long l;
{
int k; long j;
# if D1
printf("copyn %ld bytes\n",l);
# endif
j=0;
while (l--)
	{
	k = getc(fo);
	if (k<0) break;
	j++;
	putc(k,fn);
	}
# if D2
printf("returns %ld bytes\n", j);
# endif
return(j);
}
skipn(fo, l)
	FILE *fo;
{
int lu;
lu =l;
# if D1
printf("skip %d bytes\n",l);
# endif
while (l && getc(fo)>=0) l--;
# if D2
printf("returns %d\n",lu-l);
# endif
return(lu-l);
}
copyb(fn, bibp)
	FILE *fn;
	struct bibfile *bibp;
{
FILE *fb;
char ln[100];
int k, j;
sprintf(ln, "%s/bib%03d", rundir, bibp->bibfno);
fb = fopen(ln, "r");
fgets (ln, 100, fb);
j=0;
while ((k=getc(fb)) >=0)
	{
	j++;
	putc(k, fn);
	}
fclose(fb);
return(j);
}
wrapo(fso, fnew,  nowopen)
	FILE *fso, *fnew;
	char *nowopen;
{
	copyn(fso, fnew, 9999999L);
	fclose(fso);
	fclose(fnew);
	svmove(nowopen);
}
bcmp( bpnm, bpst, nm, st)
	long bpst, st;
	char *bpnm, *nm;
{
int i; long l;
char bn1[100], bn2[100];
flcopy(bn1, bpnm);
flcopy(bn2, nm);
i = strcmp(bn1, bn2);
if (i<0) return('<');
if (i>0) return('>');
l = bpst - st;
if (l<0) return('<');
if (l>0) return('>');
return('=');
}

flcopy(s, t)
	char *s, *t;
{
if (t[0]=='/' )
	strcpy(s,t);
else
	sprintf(s,"%s/%s", Master,t);
}
