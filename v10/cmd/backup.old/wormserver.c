#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"backup.h"
#include	<ipc.h>
#include	<sys/filio.h>
#include	<errno.h>

char *todir = 0;
int verbose = 0;
int worm = 0;
int dflag = 0;
int Fflag = 0;
char *inp, *outp;
char map[FNAMELEN];
char olddev[FNAMELEN] = "";
char me[FNAMELEN];
int mgrfd = -1;
char ret[FNAMELEN];

#define		ALARM		150	/* lifetime of this process (secs) */

sigalarm()
{
	logprint("+%s bored; exiting\n", me);
	exit(0);
}

main(argc, argv)
	char **argv;
{
	register fd, n;
	ipcinfo *ip;
	int nfd;
	char buf[256];
	char *dev = argv[1];
	int busy, maxfds;
	fd_set fds, goo;
	int fpipe[2];

	signal(SIGHUP, SIG_IGN);
	for(n = 1; n < 128; n++)
		close(n);
	chdir("/tmp");
	backupdir(map);
	sprint(me, "bkpworm%s", dev);
	logprint("+%s starting\n", me);
	sprint(ret, "/cs/bkpworm%s", dev);
	fd = ipccreat(ret, 0);
	if(fd < 0){
		write(0, "Error", 1);
		logprint("+can't announce %s: %s\n", ret, errstr);
		exit(1);
	}
	write(0, "Okay", 1);
	close(0);
	mgrfd = spawn(dev, ret);
	signal(SIGALRM, sigalarm);
	signal(SIGPIPE, SIG_IGN);
	busy = 0;
	FD_ZERO(fds);
	FD_SET(fd, fds);
	FD_SET(mgrfd, fds);
	maxfds = max(mgrfd, fd)+1;
	alarm(ALARM);
	for(;;){
		goo = fds;
		if(select(maxfds, &goo, (fd_set *)0, 1000*(ALARM+1)) < 0){
			logprint("%s: select fail errno=%d\n", me, errno);
			exit(1);
		}
		if(FD_ISSET(mgrfd, goo)){
			busy = 0;
			read(mgrfd, buf, 1);
			alarm(ALARM);
		}
		if(FD_ISSET(fd, goo)){
			ip = ipclisten(fd);
			if(ip == 0){
				logprint("%s lost the announcement connection somehow\n", me);
				exit(1);
			}
			alarm(0);
			nfd = ipcaccept(ip);
			if(busy){
				logprint("%s: reject %s!%s\n", me,
					ip->machine, ip->user);
				write(nfd, "B", 1);
				close(nfd);
				continue;
			}
			write(nfd, "R", 1);
			logprint("%s: accept %s!%s\n", me, ip->machine, ip->user);
			suckin(nfd);
			if(pipe(fpipe) < 0)
				logprint("%s: fpipe failed errno=%d\n", me, errno);
			if(ioctl(mgrfd, FIOSNDFD, &fpipe[1]) < 0)
				logprint("%s: sendfd failed errno=%d\n", me, errno);
			if(ioctl(mgrfd, FIOSNDFD, &nfd) < 0)
				logprint("%s: sendfd failed errno=%d\n", me, errno);
			else
				busy = 1;
			close(fpipe[1]);
			suckout(fpipe[0]);
			close(nfd);
			close(fpipe[0]);
		}
	}
}

spawn(n1, n2)
	char *n1, *n2;
{
	int p[2], pid;

	if(pipe(p) < 0){
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	if((pid = fork()) < 0){
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	if(pid)
		close(p[0]);
	else {
		close(p[1]);
		dup2(p[0], 0);
		execl("/usr/lib/backup/wormy", "wormy", n1, n2, 0);
		logprint("%s: pipe fail errno=%d\n", me, errno);
		exit(1);
	}
	return(p[1]);
}

static char suckbuf[1024*1024];
static char *sp;

static char *
trunc(iname)
	char *iname;
{
	register char *p;

	if(p = strrchr(iname, ' '))
		iname = p+1;
	if(*iname == '/'){
		for(p = iname; *p; p++)
			if((p[0] == '/') && (p[1] == 'v') && (p[2] == '/'))
				break;
		if(*p)
			iname = p+3;
		else {		/* try for vnnn/nnn */
			for(; --p >= iname;)
				if((*p < '0') || (*p > '9'))
					break;
			if((p < iname) || (*p != '/'))
				goto nomore;
			for(p--; p >= iname; p--)
				if((*p < '0') || (*p > '9'))
					break;
			if((p < iname) || (*p != 'v'))
				goto nomore;
			iname = p;
		nomore:;
		}
	}
	return(iname);
}

suckin(fd)
{
	register nl;
	int nlines = 0;
	char *b;

	Finit(fd, (char *)0);
	for(sp = suckbuf; b = Frdline(fd);){
		if(nlines++)
			b = trunc(b);
		nl = strlen(b);
		memcpy(sp, b, nl);
		sp += nl;
		*sp++ = '\n';
		if(strcmp(b, "bye") == 0) break;
	}
	*sp = 0;
}

suckout(fd)
{
	write(fd, suckbuf, sp-suckbuf);
}
