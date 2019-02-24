#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include	<grp.h>

int lflag = 0;
int bflag = 0;

main(argc, argv)
	char **argv;
{
	Superblock s;
	register Inode *i;
	char *e;
	char *dev = "/dev/worm0";
	int c;
	extern char *optarg;
	extern int optind;
	void pr();

	while((c = getopt(argc, argv, "lbf:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 'l':	lflag = 1; break;
		case 'b':	bflag = 1; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if((s.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	c = 0;
	if(optind < argc)
		while(optind < argc){
			if(i = inodeof(argv[optind]))
				pr(i);
			else {
				Fprint(2, "%s not found\n", argv[optind]);
				c = 1;
			}
			optind++;
		}
	else
		inodetraverse(pr);
	exit(c);
}

char *
suid(n)
{
	static char buf[24];
	struct passwd *p;
	char *s;

	sprint(buf, "#%d", n);
	if(s = (char *)symlook(buf, S_UID, (void *)0))
		strcpy(buf, s);
	else {
		s = strdup(buf);
		if(p = getpwuid(n))
			strcpy(buf, p->pw_name);
		(void)symlook(s, S_UID, (void *)strdup(buf));
	}
	return(buf);
}

char *
sgid(n)
{
	static char buf[24];
	struct group *g;
	char *s;

	sprint(buf, "#%d", n);
	if(s = (char *)symlook(buf, S_GID, (void *)0))
		strcpy(buf, s);
	else {
		s = strdup(buf);
		if(g = getgrgid(n))
			strcpy(buf, g->gr_name);
		(void)symlook(s, S_GID, (void *)strdup(buf));
	}
	return(buf);
}

mode(n, sx)
{
	Fputc(1, (n&4)? 'r':'-');
	Fputc(1, (n&2)? 'w':'-');
	Fputc(1, (n&1)? sx:'-');
}

void
pr(i)
	register Inode *i;
{
	char *s;

	if(lflag){
		Fputc(1, ((i->mode&S_IFMT) == S_IFDIR)? 'd':'-');
		mode(i->mode>>6, ((i->mode&S_IFMT) == S_ISUID)? 's':'x');
		mode(i->mode>>3, ((i->mode&S_IFMT) == S_ISGID)? 's':'x');
		mode(i->mode, 'x');
		Fputc(1, ((i->mode&S_IFMT) == S_IFLNK)? 'L':' ');
		s = ctime(&i->ctime);
		s += 4;
		s[12] = 0;
		Fprint(1, "%2d%8s%7s %6ld %s %s\n", 1, suid(i->uid), sgid(i->gid),
			i->nbytes, s, i->name.n);
		return;
	}
	if(bflag)
		Fprint(1, "%s\t%ld\n", i->name.n, i->block);
	else
		Fprint(1, "%s\n", i->name.n);
}

usage()
{
	fprint(2, "Usage: worm ls [-fdevice] [-l] [-b] [files ...]\n");
	exit(2);
}
