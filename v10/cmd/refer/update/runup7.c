# include "runup.h"
# define NEWN 100
# define NEWCH 2000
struct newfile news[NEWN];
int lgout;
long ins1, ins2, add;
append(argc,argv)
	char *argv[];
{
FILE *foth, *fcp;
int i;
char ln[200], nfile [100];
long mappos();
if (argc<=1) return;
sprintf(nfile, "%s.x", newplace);
if (access(nfile,6)!=0)  /* not there */
	{
	fcp = fopen(newplace, "r");
	foth = fopen(nfile, "w");
	assert(fcp!=NULL);
	assert(foth!=NULL);
	copyn(fcp, foth, 9999999L);
	fclose(fcp);
	fclose(foth);
	}
foth = fopen(nfile, "a");
svmove(newplace);
assert(foth!=NULL);
for(i=1; i<argc; i++)
	{
	if (chatty) printf("%s\n",argv[i]);
	if (argv[i][0] =='/')
		strcpy(ln, argv[i]);
	else
		sprintf(ln, "%s/%s", rundir, argv[i]);
	fcp= fopen(ln,"r");
	assert(fcp!=NULL);
	while (fgets(ln, 200, fcp))
		fputs(ln, foth);
	if (ln[0] != '\n') fputs("\n",foth);
	fclose(fcp);
	}
fclose(foth);
}

/* fix up Ind.ic file: make table of NN->In correspondences */
fixic()
{
FILE *fio, *fin, *fnn;
char indln[100]; long ind_st; int ind_lg; char ind_nm[100];
char nnln[100]; long nn_st; int nn_lg; char nn_nm[100];
int bibno;
long lother, posnnic, atol();
struct newfile *newp;
struct bibfile *bp;
unlink("XXIND");
link("XInd.ic", "XXIND");
unlink("XInd.ic");
fio = fopen("XXIND", "r");
fin = fopen("XInd.ic", "w");
fnn = fopen("NN.ic", "r");
assert(fio!=NULL);
assert(fin!=NULL);
assert(fnn!=NULL);
newp=news;
/* copy Ind file up to last previous "other" entry */
while (fgets(indln, 100, fio))
	{
	register char *s1, *s2;
	/* sscanf(indln, "%[^:]:%ld,%d", ind_nm, &ind_st, &ind_lg); */
	/* the previous line has been replaced for efficiency's sake */
	s1 = ind_nm; s2 = indln;
	while (*s2!=':') *s1++ = *s2++;
	*s1=0;
	ind_st = atol(++s2);
	while (*s2++!=',');
	ind_lg = atoi(s2);
	if (bcmp (newplace, 999999L, ind_nm, ind_st) == '<')
		break;
	fputs(indln, fin);
	lother = ind_st+ind_lg;
	}
# if D1
	printf("last IND entry was %s lother %ld\n", indln, lother);
# endif
ins1=ftell(fin);
for (posnnic=0; fgets(nnln, 100, fnn); posnnic = ftell(fnn))
	{
	register char *s1, *s2;
# if D1
	printf("Line from NN %s", nnln);
# endif
	/* sscanf(nnln, "%[^:]:%ld,%d", nn_nm, &nn_st, &nn_lg); */
	/* the previous line has been replaced for efficiency's sake */
	s1 = nn_nm; s2 = nnln;
	while (*s2!=':') *s1++ = *s2++;
	*s1=0;
	nn_st = atol(++s2);
	while (*s2++!=',');
	nn_lg = atoi(s2);
	if (sscanf(lastsl(nn_nm), "bib%d", &bibno)==1) /* bib file */
		{
		for(bp=bibs; bp->bibfno>=0; bp++)
			{
			if (bp->bibfno == bibno)
				{
				newp->nn_ic = posnnic;
				newp->new_ic = mappos(bp->bibival);
				newp++;
				assert (newp< news+NEWN);
				break;
				}
			}
# if D1
		printf("bib file no. %d is bp %d\n", bibno, bp-bibs);
# endif
		assert(bp->bibfno>=0);
		}
	else /* file added */
		{
		newp->nn_ic = posnnic;
		newp->new_ic = ftell(fin);
		fprintf(fin, "%s:%ld,%d\n", newplace, lother, nn_lg);
# if D1
		printf("new at lot %ld from nn.st %ld nn.lg %d\n",lother,nn_st, nn_lg);
		printf("new mapped from %ld to %ld\n", posnnic, lother);
# endif
		newp++;
		assert(newp<news+NEWN);
		lother += nn_lg;
		}
	}
add = ftell(fin)-ins1;
ins2 = ftell(fin);
# if D1
printf("From %ld (file 1) or %ld (file2) add %ld\n",ins1,ins2,add);
# endif
newp->nn_ic= -1;
/* copy rest of Ind file */
fputs(indln, fin);
while (fgets(indln, 100, fio))
	{
	fputs(indln, fin);
	}
# if D1
for(newp=news; newp->nn_ic>=0; newp++)
	printf("new map %ld to %ld\n",newp->nn_ic, newp->new_ic);
# endif
lgout = ftell(fin) > 65536L;
fclose(fin);
}

lastsl(s)
	char *s;
{
char *p;
for(p=s; *p; p++)
	if (*p=='/')
		s=p+1;
return(s);
}
