# include "runup.h"
# include "sys/types.h"
# include "sys/dir.h"
# include "sys/stat.h"
# define NB 100
struct bibfile bibs[NB];
# define BIBCH 1000
char bibbuff[BIBCH];
char *bibcp = bibbuff;

readbib()
{
glist();
# if D1
		printf("List of bib files\n");
		{ struct bibfile *bp;;
		for(bp=bibs; bp->bibfno>=0; bp++)
			printf("  bib%d\n", bp->bibfno);
		}
# endif
gdata();
# if D1
		printf("Contents\n");
		{struct bibfile *bp;
		for(bp=bibs; bp->bibfno>=0; bp++)
			printf("bib%d is %s %ld,%d\n",
				bp->bibfno, bp->bibfref, bp->bibfstrt,bp->bibfleng);
		}
# endif
}
glist()
{
struct bibfile *bibp = bibs;
int fd;
struct stat sbuf;
char ftr[100];
long tref;
struct direct dir;
sprintf(ftr, "%s/Ind.ib", Master);
assert(stat (ftr, &sbuf)==0);
tref=sbuf.st_mtime;
fd = open(".", 0);
assert(fd>0);
while(read(fd, &dir, sizeof(dir))==sizeof(dir))
	{
	if (dir.d_ino==0)
		continue;
	if (strncmp(dir.d_name, "bib", 3) != SAME)
		continue;
	strncpy(ftr, dir.d_name, 14);
	assert(stat(ftr, &sbuf) ==0);
	if (sbuf.st_mtime<=tref)
		{
		fprintf(stderr, "File %s too old - reenter change\n", ftr);
		continue;
		}
	bibp++->bibfno = atoi(dir.d_name+3);
	assert(bibp<bibs+NB);
	}
bibp -> bibfno = -1;
close(fd);
}
gdata()
{
struct bibfile *bibp, *bp;
FILE *fb;
int junk1, junk2, leng;
long start;
char junk[20], junk3[20], name[50], *np;
char fnam[20], bibln[100];
for(bibp=bibs; bibp->bibfno>=0; bibp++)
	{
	sprintf(fnam, "bib%03d", bibp->bibfno);
	fb = fopen(fnam, "r");
	assert(fb!=NULL);
	assert(fgets(bibln, 100, fb)!=0);
	sscanf(bibln, "%s %s %d %d %s %ld %d",
		junk, name, &junk1, &junk2, junk3, &start, &leng);
	assert(name[0]=='/');
	fclose(fb);
	for(bp=bibs; bp<bibp; bp++)
		{
		if (strcmp(name, bp->bibfref)==SAME)
			{
			bibp->bibfref = bp->bibfref;
			break;
			}
		}
	if (bp==bibp) /* not previously used file name */
		{
		bibp->bibfref = bibcp;
		np=name;
		while (*bibcp++ = *np++);
		}
	bibp->bibfstrt = start;
	bibp->bibfleng = leng;
	}
}
