# include "stdio.h"
# include "assert.h"
# include "ctype.h"

main(argc,argv)
	char *argv[];
{
/* arguments are keys of a paper */
/* output is file & lines to drop */
	char answer[200], filname[60], outname[60];
	int nuse = 0, i, c, length, line1, line2;
	long l, start;
	FILE *lkin, *lkout, *fin, *fout;
	char buff[2000], *bf = buff;
	char *select = 0;
	extern char *indexes;
lkin = fopen("/dev/null", "w");
lkout = fopen("/dev/null", "r");
_assert (lkin !=NULL); _assert (lkout!=NULL);
while (argc>1 &&argv[1][0]=='-')
	{
	switch(argv[1][1])
		{
		case 'p': /* use private index files */
			indexes = argv[2];
			argv += 2; argc -= 2;
			continue;
		case 'u': /* use the Nth item */
			nuse = atoi(argv[1]+2);
			argv++; argc--;
			continue;
		case 's': /* insist on a particular string */
			select = argv[2];
			argv += 2; argc -= 2;
			continue;
		default:
			fprintf(stderr, "unknown option %s\n",argv[1]);
			argv++; argc--;
			continue;
		}
	}
lookup(lkin, lkout);
if (argc<=1) fprintf(stderr, "No search keys\n");
_assert (argc>1);
for(i=1; i<argc; i++)
	fprintf(lkin, "%s ", argv[i]);
fprintf(lkin, "\n");
fclose(lkin);
/* skip items up to nuse */
for(i=1; i<nuse; i++)
	geta(answer, lkout, select);
/* get the right answer */
i = geta(answer, lkout, select);
if (i==0)
	{
	fprintf(stderr, "No match\n");
	exit(1);
	}
sscanf(answer, "%[^:]:%ld,%d", filname, &start, &length);
if (nuse<=0 && geta(answer, lkout, select)>0)
	{
	fprintf(stderr, "Not unique - use lookbib and either -s or -u\n");
	exit(1);
	}
fclose(lkout);
assert((strlen(filname)>0));
fin = fopen(filname, "r");
_assert (fin!=NULL);
c=0;
for(i=getpid()%990; 1; i++)
	{
	_assert (c++<100);
	sprintf(outname, "bib%03d", i);
	if (access(outname, 06) != 0)
		break;
	}
fout = fopen(outname, "w");
line1 = 1;
printf("Extracting as file %s\n", outname);
for(l=0; l<start; l++)
	{
	c = getc(fin);
	_assert (c!=EOF);
	if (c=='\n')
		line1++;
	}
line2=line1;
for(i=0; i<length; i++)
	{
	c = getc(fin);
	_assert (c != EOF);
	putchar(c);
	*bf++ = c;
	if (c== '\n')
		line2++;
	}
fclose(fin);
fprintf(fout, "%%# %s %d %d change %ld %d\n",
	filname, line1, line2-1, start, length);
*bf=0;
fprintf(fout, "%s", buff);
fclose(fout);
}
geta(answer, f, str)
	char *answer, *str;
	FILE *f;
{
answer[0]=0;
while (answer[0]==0 && feof(f)==0)
	{
	answer[0]=0;
	fgets(answer, 200, f);
	trim(answer);
	if (contain(answer, str)==0)
		answer[0]=0;
	}
return( answer[0]!=0);
}
contain(answer, str)
	char *answer, *str;
{
char filname[100]; long start; int length;
int ft;
char buff[1000], *s, *st;
if (str==0) return(1);
if (answer[0]==0) return(1);
sscanf(answer, "%[^:]:%ld,%d", filname, &start, &length);
assert((strlen(filname)>0));
ft = open(filname, 0);
assert(ft>=0);
lseek(ft, start, 0);
if (length>1000) length=1000;
length = read(ft, buff, length);
close(ft);
buff[length]=0;
st = buff+length;
for(s=buff; s<st; s++)
	if (prefix(str, s))
		return(1);
return(0);
}
prefix(sh,lg)
	char *sh, *lg;
{
int c;
while ( (c= *sh++) == *lg++)
	{
	if (c==0)
		return(1);
	}
return(c==0);
}
trim(s)
	char *s;
{
while (*s)s++;
if (*--s=='\n') *s=0;
}
