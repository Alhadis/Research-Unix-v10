#include <sys/types.h>
#include <ndir.h>
#include <sys/stat.h>
#include "dirsel.h"

/* almost like 4.2 scandir(), but the returned structs are different */
#define MAXENTS 2000	/* hope we don't find a dir with more than MAXENTS entries */

int
dirsel(dirname, pents, select, compare)
	char *dirname;
	dirent **pents;
	int (*select)();
	int (*compare)();
{
	dirent *ep;
	DIR *ddp;
	struct direct *dp;
	struct stat statbuf;
	unsigned long starttime;
	int nents=0;
	char fullname[MAXNAMLEN+1];
	char *p;

	ddp=opendir(dirname);
	strcpy(fullname, dirname);
	p= &fullname[strlen(dirname)];
	*p++ = '/';
	*p='\0';
	if (ddp==0)
		return 0;
	starttime=(unsigned long)time((long *)0);
	*pents=ep=(dirent *)malloc(MAXENTS * sizeof(dirent));
	while ((dp=readdir(ddp))!=0){
		strcpy(p, dp->d_name);
		if (stat(fullname, &statbuf)!=0)
			continue;	/* shouldn't happen */
		ep->name=(char *)malloc(dp->d_namlen+1);
		strcpy(ep->name, dp->d_name);
		ep->namelen=dp->d_namlen;
		ep->isdir=((statbuf.st_mode&S_IFDIR) != 0);
		ep->age=(long)(starttime-(unsigned long)statbuf.st_mtime);
		if (select && !(*select)(ep))
			continue;	/* don't count this entry */
		ep++, nents++;
		if (nents==MAXENTS)
			break;	/* could realloc */
	}
	closedir(ddp);
	if (nents>1 && compare)
		qsort(*pents, nents, sizeof(dirent), compare);
	return nents;
}
