# include "runup.h"
sortbib()
{
struct bibfile *bibp;
extern int comp(), exch();
int nb;
for(bibp=bibs; bibp->bibfno>=0; bibp++)
	;
nb=bibp-bibs;
shell(nb, comp, exch);
# if D1
	printf("sorted %d bibs\n", nb);
	{
	struct bibfile *bp;
	for(bp=bibs; bp->bibfno>=0; bp++)
		printf("  bib%03d\n",bp->bibfno);
	}
# endif
}
comp(i,j)
{
int k;
k = strcmp(bibs[i].bibfref, bibs[j].bibfref);
if (k!=0) return(k<0);
if (bibs[i].bibfstrt == bibs[j].bibfstrt)
	{
	fprintf(stderr, "Error: files bib%03d and bib%03d are redundant\n",bibs[i].bibfno, bibs[j].bibfno);
	unlock();
	exit(1);
	}
return(bibs[i].bibfstrt-bibs[j].bibfstrt<0);
}
exch(i,j)
{
struct bibfile tb;
tb=bibs[i];
bibs[i]=bibs[j];
bibs[j]=tb;
}
# define NMV 100
# define NB 1000
char *tomove[NMV];
int nm = 0;
char nmbuff[NB];
char *nbmp = nmbuff;
svmove(s)
	char *s;
{
int i;
for(i=0;i<nm;i++)
	if (strcmp(tomove[i], s)==SAME)
		return;
tomove[nm++] = nbmp;
while (*nbmp++ = *s++);
assert(nbmp<nmbuff+NB);
assert(nm<NMV);
return;
}
remove()
{
int i;
struct bibfile *bp;
char newver[100];
unlink("XXIND");
for(i=0; i<nm; i++)
	{
	sprintf(newver, "%s.x", tomove[i]);
	if (access(newver, 4)!=0)
		continue;
	unlink(tomove[i]);
	link(newver, tomove[i]);
	unlink(newver);
	}
unlink("Ind.ia"); link("XInd.ia", "Ind.ia"); unlink("XInd.ia");
unlink("Ind.ib"); link("XInd.ib", "Ind.ib"); unlink("XInd.ib");
unlink("Ind.ic"); link("XInd.ic", "Ind.ic"); unlink("XInd.ic");
unlink("NN.ia"); unlink ("NN.ib"); unlink("NN.ic");
chdir (rundir);
for (bp=bibs; bp->bibfno>=0; bp++)
	{
	sprintf(newver, "bib%03d", bp->bibfno);
	unlink(newver);
	}
}

garbage()
{
/* called on interrupt only */
int i;
char newver[100];
fprintf(stderr, "Interrupt.\n");
for (i=0; i<nm; i++)
	unlink(sprintf(newver, "%s.x", tomove[i]));
unlink("XInd.ia"); unlink("XInd.ib"); unlink("XInd.ic"); 
unlink("XXIND");
unlink("NN.ia"); unlink("NN.ib"); unlink("NN.ic"); 
unlink(sprintf(newver, "%s.x", newplace)); /* other */
unlock();
exit(1);
}
