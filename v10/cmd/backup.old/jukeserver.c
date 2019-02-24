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
int readytosend = 0;
int worm = 0;
int dflag = 0;
int Fflag = 0;
char *inp, *outp;
char map[FNAMELEN];
char olddev[FNAMELEN] = "";
char me[FNAMELEN];
int mgrfd;
char ret[FNAMELEN];
char *nextdev();
char userinfo[256];

#define		ALARM		120	/* lifetime of this process (secs) */

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
	fd_set fds;
	int fpipe[2];

	signal(SIGHUP, SIG_IGN);
	for(n = 1; n < 128; n++)
		close(n);
	chdir("/tmp");
	backupdir(map);
	volmapinit();
	sprint(me, "bkpworm%s", dev);
	logprint("+%s starting\n", me);
	sprint(ret, "/cs/bkpworm%s", dev);
	fd = ipccreat(ret, 0);
	if(fd < 0){
		write(0, "Error", 1);
		logprint("can't announce %s: %s\n", ret, errstr);
		exit(1);
	}
	write(0, "Okay", 1);
	close(0);
	signal(SIGALRM, sigalarm);
	signal(SIGPIPE, SIG_IGN);
	busy = 0;
	mgrfd = -1;
	alarm(ALARM);
	for(;;){
		FD_ZERO(fds);
		FD_SET(fd, fds);
		if(mgrfd >= 0)
			FD_SET(mgrfd, fds);
		maxfds = max(mgrfd, fd)+1;
		if(select(maxfds, &fds, (fd_set *)0, 1000*(ALARM+1)) < 0){
			logprint("%s: select fail errno=%d\n", me, errno);
			exit(1);
		}
		if((mgrfd >= 0) && FD_ISSET(mgrfd, fds)){
			read(mgrfd, buf, 1);
			close(mgrfd);
			goto nextbatch;
		}
		if(FD_ISSET(fd, fds)){
			ip = ipclisten(fd);
			if(ip == 0){
				logprint("+%s lost the announcement connection somehow\n", me);
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
			busy = 1;
			readents(nfd);
		nextbatch:
			if(dev = nextdev(nfd)){
				mgrfd = spawn(dev, ret);
				if(pipe(fpipe) < 0)
					logprint("%s: fpipe failed errno=%d\n", me, errno);
				if(ioctl(mgrfd, FIOSNDFD, &fpipe[1]) < 0)
					logprint("%s: sendfd1 failed errno=%d\n", me, errno);
				if(ioctl(mgrfd, FIOSNDFD, &nfd) < 0)
					logprint("%s: sendfd2 failed errno=%d\n", me, errno);
				close(fpipe[1]);
				sendbatch(fpipe[0]);
				close(fpipe[0]);
				readytosend = 0;
			} else {
				close(nfd);
				mgrfd = -1;
				busy = 0;
				alarm(ALARM);
			}
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

typedef struct Ent
{
	int vol, file;
} Ent;
static Ent ents[100000];
static Ent *ne, *de;
typedef struct Volmap
{
	char *vname;
	int vol;
} Volmap;
static Volmap vms[500];
static Volmap *nv;

static
gobble(iname, e)
	char *iname;
	Ent *e;
{
	register char *p;

	for(p = strchr(iname, 0); --p >= iname;)
		if((*p < '0') || (*p > '9'))
			break;
	if((p < iname) || (*p != '/'))
		return(0);
	e->file = atoi(p+1);
	for(p--; p >= iname; p--)
		if((*p < '0') || (*p > '9'))
			break;
	if((p < iname) || (*p != 'v'))
		return(0);
	e->vol = atoi(p+1);
	return(1);
}

cmp(a, b)
	register Ent *a, *b;
{
	register k;

	if(k = a->vol - b->vol)
		return(k);
	return(a->file - b->file);
}

readents(fd)
{
	char *b;

	Finit(fd, (char *)0);
	ne = ents;
	if(b = Frdline(fd))
		strcpy(userinfo, b);
	while(b = Frdline(fd)){
		if(strcmp(b, "bye") == 0) break;
		if(gobble(b, ne))
			ne++;
	}
	qsort((char *)ents, ne-ents, sizeof(Ent), cmp);
	de = ents;
}

char *
nextdev(nfd)
{
	static char buf[256];
	int i, j, fd;
	int pip[2], pid;

	if(de == ne)
		return((char *)0);
	i = vmap(de->vol);
	fprint(nfd, "jukebox: fetching disk %s\n", vms[i].vname);
/*	Fexit(0);	/* flush everything */
	if(pipe(pip) < 0){
		perror("jukeserver pipe");
		return((char *)0);
	}
	pid = fork();
	if(pid < 0){
		perror("jukeserver fork");
		return((char *)0);
	}
	if(pid == 0){
		dup2(pip[1], 1);
		close(pip[0]);
		close(pip[1]);
		execl("/usr/lib/worm/wmount", "wmount", vms[i].vname, (char *)0);
		perror("nextdev execl");
		exit(0);
	}
	wait(&pid);
	close(pip[1]);
	buf[0] = 0;
	j = read(pip[0], buf, sizeof buf);
	if(j < 2)
		strcpy(buf, ".error");
	else
		buf[j-1] = 0;
	close(pip[0]);
	readytosend = 1;
	return(buf);
}

sendbatch(fd)
{
	int lim;

	Finit(fd, (char *)0);
	Fprint(fd, "%s\n", userinfo);
	for(lim = vms[vmap(de->vol)+1].vol; (de < ne) && (de->vol < lim); de++)
		Fprint(fd, "v%d/%d\n", de->vol, de->file);
	Fprint(fd, "bye\n");
	Fflush(fd);
}

volmapinit()
{
	int fd;
	char buf[256], *b, *p;

	sprint(buf, "%s/%s", map, VOLMAP);
	fd = open(buf, 0);
	if(fd < 0){
		write(0, "Error", 1);
		logprint("can't read volmap %s\n", buf);
		exit(1);
	}
	Finit(fd, (char *)0);
	nv = vms;
	while(b = Frdline(fd)){
		if((p = strchr(b, '\t')) == 0){
			write(0, "Error", 1);
			logprint("bad volmap line %s\n", b);
			exit(1);
		}
		*p++ = 0;
		nv->vol = atoi(p);
		nv->vname = strdup(b);
		nv++;
	}
	Fclose(fd);
	nv->vol = 999999;
}

vmap(vol)
{
	register Volmap *v;

	for(v = vms; v->vol <= vol; v++)
		;
	return(v-1-vms);
}
