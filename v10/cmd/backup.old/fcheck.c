#include	<ftw.h>
#include <sys/param.h>
#include <sys/stat.h>
#include	<libc.h>

off_t	maxsize;
time_t	mintime;

main(argc, argv)
char **argv;
{
	long n;
	extern fn();

	if(argc < 4){
usage:
		fprint(2, "Usage: fcheck maxsize minctime files\n");
		exit(1);
	}
	maxsize = atol(*++argv)*1024;
	time(&mintime);
	mintime -= 24*3600L*(n = atol(*++argv));
	if((maxsize == 0) || (n == 0))
		goto usage;
	while(*++argv)
		if(ftw(*argv, fn, NOFILE-5))
			exit(1);
	exit(0);
}

fn(name, statb, code, s)
	char *name;
	struct stat *statb;
	struct FTW *s;
{
	char *prefix = "";

	if(strchr(name, '\n'))
		return(0);
	switch(code)
	{
	case FTW_SL:
		if(s->level == 0){
			s->quit = FTW_FOLLOW;
			prefix = "/";
			if(stat(name, statb) < 0)
				break;
		}
	case FTW_F:
	case FTW_D:
		if(statb->st_nlink && (statb->st_size < maxsize) &&
				(statb->st_ctime >= mintime))
			Fprint(1, "%s%s\n", prefix, name);
		break;
	case FTW_NS:
		perror(name);
		return(0);
	case FTW_DNR:
		fprint(2, "cannot read %s\n", name);
		return(0);
	case FTW_DP:
		break;
	case FTW_NSL:
		fprint(2, "cannot follow symbolic link %s\n", name);
		return(0);
	}
	return(0);
}
