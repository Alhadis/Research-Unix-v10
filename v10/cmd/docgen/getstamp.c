#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
long
getstamp(filename)
char *filename;
{
	long mtime;
	struct stat fbuf, *fp = &fbuf;
	struct tm t, *tp = &t;
	stat(filename, fp);
	t = *localtime(&fp->st_mtime);
	mtime = fp->st_mtime;
	return(mtime);
}
