#include "stdio.h"
#include "sys/param.h"
#include "sys/stat.h"
#include "ndir.h"
#include "ftw.h"

enum Params {Maxpathlen=1024, Nlinks=600};
char flags[128];	/* -a, -s */
char buf[Maxpathlen];
unsigned long totlen, dirlen[Maxpathlen];
struct {
	int ino, dev;
} ml[Nlinks];
int nlink, follow, level;
extern int errno;

main(argc, argv)
char **argv;
{	extern char *optarg;
	extern int optind;
	int c;

	while((c = getopt(argc, argv, "as")) != -1)
	switch(c) {
		default:
			fprintf(stderr, "usage: %s [-a] [-s] [files]\n", argv[0]);
			exit(1);
		case 'a': case 's':
			flags[c]++;
			break;
	}
	if(optind >= argc) {
		doit(".");
			if(flags['s'])
				printf("%d\t%s\n", totlen, argv[optind]);
	}
	else
		for(; optind < argc; optind++) {
			follow = 1;
			doit(argv[optind]);
			if(flags['s'])
				printf("%d\t%s\n", totlen, argv[optind]);
			totlen = 0;
		}
	exit(0);
}

fn(name, stb, code, s)
char *name;
struct stat *stb;
struct FTW *s;
{	int n;
	if(follow) {
		follow = 0;
		if(code == FTW_SL) {
			s->quit = FTW_FOLLOW;
			return(0);
		}
	}
	switch(code) {
	case FTW_D:
		dirlen[++level] = 0;
	case FTW_SL:
	case FTW_F:	/* stupid size semantics (BSIZE not networked) */
common:
		if(seenit(stb))
			return(0);
		n = ((stb->st_size + BSIZE(stb->st_dev)-1) >> BSHIFT(stb->st_dev))
			* (BSIZE(stb->st_dev)/1024);
		totlen += n;
		dirlen[level] += n;
		if(flags['a'])
			printf("%d\t%s%s\n", n, name, code==FTW_D? "/": "");
		break;
	case FTW_DP:
		if(!flags['s']) {
			printf("%d\t%s\n", dirlen[level], name);
			fflush(stdout);	/* bogus new stdio */
		}
		dirlen[level-1] += dirlen[level];
		level--;
		break;
	case FTW_DNR:
		perror(name);
		goto common;
	case FTW_NS:
	case FTW_NSL:
		perror(name);
	}
	return(0);
}

doit(s)
char *s;
{
	level = 0;
	ftw(s, fn, 100);
}

seenit(s)
struct stat *s;
{	int i;
	if(s->st_nlink == 1 || (s->st_mode & S_IFMT) != S_IFREG)
		return(0);
	for(i = 0; i < nlink; i++)
		if(ml[i].ino == s->st_ino && ml[i].dev == s->st_dev)
			return(1);
	if(i < Nlinks) {
		ml[i].ino = s->st_ino;
		ml[i].dev = s->st_dev;
		nlink++;
	}
	return(0);
}
