/* read fstab and drive chuck */
/* this has to make up raw device names, or used the cooked devices */
#include <fstab.h>
#include <sys/param.h>	/* param for BITFS */
#include <sys/stat.h>
#include <libc.h>

enum {E64 = 64};
struct fstab fs[E64];	/* surely enough for all time */
int procs[E64], pstat[E64];
int min = 0x7fffffff, max = -1, exitcode, cnt;
char *options = "cwp:";
char *cmd = "/etc/chuck";
int wflag, cflag;
extern int errno;
extern char *sys_errlist[];
struct stat rst;

main(argc, argv)
char **argv;
{	int i, n;
	int did;
	extern int optind;
	extern char *optarg;
	struct fstab *p;
	while((i = getopt(argc, argv, options)) != -1)
	switch(i) {
	default:
		pmesg("weird opt %c, should be %s\n", i, options);
		exit(1);
	case 'w':
		wflag = 1;
		continue;
	case 'p':
		cmd = optarg;
		continue;
	case 'c':
		cflag = 1;
		continue;
	}
	stat("/", &rst);
	for(n = 0; n < E64; n++)
		if(p = getfsent())
			fs[n] = *p;
		else
			break;
	if(n >= E64)
		pmesg("only doing %d entries from fstab\n", n);
	cnt = n;
	for(i = 0; i < n; i++) {
		if(fs[i].fs_ftype != 0)
			continue;
		if(fs[i].fs_passno < min)
			min = fs[i].fs_passno;
		if(fs[i].fs_passno > max)
			max = fs[i].fs_passno;
	}
	for(i = min; i <= max; i++)
		pass(i);
	for (did = 0, i = 0; i < n; i++) {
		if (pstat[i] == 0)
			continue;
		if (did == 0) {
			did++;
			pmesg("Failed for:\n");
		}
		pmesg("%s\n", fs[i].fs_spec);
	}
	exit(exitcode);
}

pass(n)
{	int i, pid, stat;
	for(i = 0; i < cnt; i++)
		if(fs[i].fs_ftype == 0 && fs[i].fs_passno == n)
			execit(i, fs+i);
	while((pid = wait(&stat)) != -1) {
		for(i = 0; i < cnt; i++)
			if (procs[i] == pid)
				break;
		if (i >= cnt) {
			pmesg("upchuck: unexpected wait pid %d stat %d\n", pid, stat);
			continue;
		}
		pstat[i] = stat;
		if (stat == 0)
			continue;
		exitcode = 1;
		if (stat & 0377)
			pmesg("%s: exit signal %d\n", stat);
		pmesg("%s: do manually\n", fs[i].fs_spec);
	}
}

char *
guessraw(s)
char *s;
{	char *p, *q, *strrchr();
	if(cflag)
		return(s);
	p = strrchr(s, '/');
	if(!p)
		return(s);
	q = (char *)malloc(strlen(s)+2);
	*p = 0;
	sprintf(q, "%s/r%s", s, p+1);
	*p = '/';
	if(access(q, 4) == 0)
		return(q);
	return(s);
}

/* cooked device for root, otherwise raw if appropriate access */
execit(n, p)
struct fstab *p;
{	int i, fl = 0;
	struct stat stb;
	char *s;
	if(stat(p->fs_spec, &stb) < 0) {
		exitcode = 1;
		pmesg("couldn't stat (%s,%s)\n", p->fs_spec, sys_errlist[errno]);
		return;
	}
	/* is it a wretched 4k or a wretched 1k file system? */
	if(!BITFS(stb.st_rdev))
		fl = 2;
	if(stb.st_rdev == rst.st_dev) {
		s = p->fs_spec;
		goto known;
	}
	s = guessraw(p->fs_spec);
known:
	switch(i = fork()) {
	case -1:
		exitcode = 1;
		pmesg("upchuck couldn't fork (%s)\n", sys_errlist[errno]);
		return;
	case 0:
		switch(fl+wflag) {
		case 0:
			execlp(cmd, cmd, s, 0);
			break;
		case 1:
			execlp(cmd, cmd, "-w", s, 0);
			break;
		case 2:
			execlp(cmd, cmd, "-b", "1024", s, 0);
			break;
		case 3:
			execlp(cmd, cmd, "-w", "-b", "1024", s, 0);
			break;
		}
		pmesg("upchuck couldn't exec %s (%s)\n", cmd, sys_errlist[errno]);
		exit(1);
	default:
		procs[n] = i;
		return;
	}
}

char ebuf[128];
pmesg(s, a, b, c, d, e, f)
char *s;
{
	sprintf(ebuf, s, a, b, c, d, e, f);
	write(2, ebuf, strlen(ebuf));
}
