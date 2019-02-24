# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "sys/types.h"
# include "sys/stat.h"
/*
 * given a list of names on the standard input,
 * compare the dates of those files with those of
 * a standard file and print the ones which pass
 * also reject inodes if previously seen
 */
main(argc,argv)
	char *argv[];
{
char *refname, *upname;
long reftime, lseek();
char fname[50];
struct stat stbuff;
# define NI 500
int ilist[NI], nj=0;
refname = argc>1 ? argv[1] : "";
upname = argc>2 ? argv[2] : "";
if (stat(refname, &stbuff) < 0)
	reftime=0;
else
	reftime = stbuff.st_mtime;
/* update file */
if (stat(upname, &stbuff) < 0 || stbuff.st_size==0)
	{
	if (creat (upname, 0666)== -1)
		fprintf(stderr, "Can not creat file %s\n",upname);
	}
else
	{
	int fd; char x[2];
	fd = open(upname, 2);
	assert(fd>0);
	assert(read(fd, x, 1)==1);
	assert(lseek(fd, 0L, 0)!= -1);
	assert(write(fd, x, 1)==1);
	assert(close(fd)==0);
	}
while (gets(fname))
	{
	int k;
	k = stat (fname, &stbuff);
	if (k<0) continue;
	if (stbuff.st_mtime >reftime)
		{
		int i, ino, found=0;
		ino = stbuff.st_ino;
		for(i=0; i<nj; i++)
			if (ilist[i]==ino)
				found=1;
		if (found) continue;
		ilist[nj++]=ino;
		if (nj>=NI) nj = NI/2;
		puts(fname);
		}
	}
}
