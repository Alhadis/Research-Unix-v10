# include "runup.h"
int nhash =0;
# define LCOMM 1000
makenew(argc,argv)
	char *argv[];
{
struct bibfile *bibp;
char jcomm[LCOMM], bno[20];
int i, f;
strcpy(jcomm, "chdir ");
strcat (jcomm, rundir);
strcat (jcomm, "; ");
strcat (jcomm, "/usr/lib/refer/mkey ");
for(bibp=bibs; bibp->bibfno>=0; bibp++)
	{
	sprintf(bno, "bib%03d ", bibp->bibfno);
	strcat(jcomm, bno);
# if D1
	printf("bno to add is %s\n",bno);
# endif
	assert(strlen(jcomm)<LCOMM);
	}
for(i=1;i<argc;i++)
	{
	strcat(jcomm, argv[i]);
	strcat(jcomm, " ");
	}
strcat(jcomm, "| /usr/lib/refer/inv -n ");
f = open("Ind.ia", 0);
read(f, &nhash, 2);
close(f);
sprintf(bno, "-h%d ", nhash);
strcat(jcomm, bno);
strcat (jcomm, Master);
strcat (jcomm, "/NN");
assert(strlen(jcomm)<LCOMM);
if (verbose) printf("Invoking system got %.1f of %d sec.  Command:\n\t%s\n",tuse(),tlapse(),jcomm);
system(jcomm);
if (verbose) printf("Returned got %.1f of %d sec.\n", tuse(), tlapse());
}

# define NMAP 100
struct mapper maps[NMAP] {{0L,0L}};
struct mapper *mapnow = maps+1;
ckpos (lold, lnew)
	long lold, lnew;
{
struct mapper *mp;
long lt;
lt = mappos(lold);
if (lt==lnew) return;
mapnow->map_old = lold;
mapnow->map_by = lnew-lold;
mapnow++;
assert(mapnow<maps+NMAP);
# if D1
for(mp=maps; mp<mapnow; mp++)
	printf("Map %ld by %ld\n", mp->map_old, mp->map_by);
# endif
return;
}

long
mappos(lold)
	long lold;
{
struct mapper *mp;
for(mp=maps; mp<mapnow; mp++)
	{
	if (mp->map_old>lold)
		break;
	}
mp--;
assert(mp>=maps);
return(lold+mp->map_by);
}
