#include	<fio.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"sym.h"
#include	"worm.h"
#include	"fail.h"
#include	"backup.h"
#include	<ipc.h>
#include	<sys/filio.h>

/*
	we are started by wormserver. we talk by a pipe and our end
	is fd 0. we look for fd's on 0 and whenever we are done,
	we write on our pipe. then the other end will know it is safe to pass on
	a new fd.

	wormy protocol:
			inputfd, outputfd
		input:
			uname gname\n
			filename\n *
			bye\n
		output:
			message\n | F.*\nfile
*/

Superblock s;
char *todir = 0;
int verbose = 0;
int worm = 0;
int dflag = 0;
int Fflag = 0;
char *inp, *outp;
char map[FNAMELEN];
char olddev[FNAMELEN] = "";
char me[FNAMELEN];
char usname[16], gpname[16];

main(argc, argv)
	char **argv;
{
	register ifd, ofd, n, ok;
	char buf[256];
	char *dev = argv[1];
	char ret[256], goo[256];
	struct passfd pfd;

	for(n = 1; n < 128; n++)
		close(n);
	signal(SIGPIPE, SIG_IGN);
	chdir("/tmp");
	backupdir(map);
	failinit(map);
	strcpy(me, argv[2]);
	s.fd = -1;
	for(;;){
		if(ioctl(0, FIORCVFD, &pfd) < 0){
			/* assume eof */
			exit(0);
		}
		ifd = pfd.fd;
		if(ioctl(0, FIORCVFD, &pfd) < 0){
			logprint("%s: error in rcvfd, errno=%d\n", me, errno);
			exit(0);
		}
		ofd = pfd.fd;
		if(s.fd < 0){
			fprint(ofd, "reading worm%s directory (a couple of minutes)\n", dev);
			ok = dodev(ofd, dev);
		} else{
			ok = 1;
		}
		if(ok)
			session(ifd, ofd);
		close(ifd);
		close(ofd);
		write(0, "X", 1);
	}
}

dodev(fd, dev)
	char *dev;
{
	char buf[256];
	char *err;

	sprint(buf, "/dev/worm%s", dev);
	if((s.fd = open(buf, 0)) < 0){
		fprint(fd, "couldn't open worm device %s, errno=%d\n", buf, errno);
		return(0);
	}
	if(err = openinode(&s, DO_INODE|SPIN_DOWN)){
		fprint(fd, "%s: %s\n", buf, err);
		close(s.fd);
		s.fd = -1;
		return(0);
	}
	return(1);
}

void
purge(name)
	char *name;
{
	symdel(name, S_TOGO);
}

long nf, nb;
int ofd;

session(in, out)
{
	char buf[256];
	long t1, t2;
	char *ss, *b;
	void do1();

	time(&t1);
	nb = nf = 0;
	ofd = out;
	Finit(in, (char *)0);
	if((b = Frdline(in)) == 0){
		logprint("%s: bad read errno=%d\n", me, errno);
		fprint(out, "bad read!\n");
		return;
	}
	if(*b != 'U'){
		logprint("bad user line '%s'\n", b);
		fprint(out, "bad user line '%s'\n", b);
		return;
	}
	for(ss = ++b; *ss++ != ' ';)
		;
	ss[-1] = 0;
	strcpy(usname, b);
	strcpy(gpname, ss);
	for(;;){
		if((b = Frdline(in)) == 0){
			logprint("%s: bad read errno=%d\n", me, errno);
			fprint(out, "bad read!\n");
			return;
		}
		if(strcmp(b, "bye") == 0)
			break;
		ss = strdup(b);
		symlook(ss, S_TOGO, (void *)ss);
	}
	Finit(out, (char *)0);
	symtraverse(S_TOGO, do1);
	announce((char *)0);
	fdump();
	Fflush(out);
	symtraverse(S_TOGO, purge);
	time(&t2);
	logprint("%s: %ld files, %ld bytes, %ds\n", me, nf, nb, t2-t1);
}

void
do1(iname)
	char *iname;
{
	long n;
	char buf[1024];		/* must be a multiple of 1024 */
	long size;
	Inode *i;
	struct backfile h;

	if((i = inodeof(iname)) == 0){
		fail(iname);
		return;
	}
	Seek(&s, i->block);
	if(Read(&s, &h, 1)){
		Fprint(ofd, "%s: %s read error=%d\n", me, iname, errno);
		return;
	}
	if(!permok(&h)){
		Fprint(ofd, "%s: permission denied\n", iname);
		return;
	}
	Fprint(ofd, "FILE\n");		/* nothing wrong */
	if(Fwrite(ofd, (char *)&h, (long)sizeof h) != sizeof h){
		logprint("%s: hdr write error during %s; bye!\n", me, iname);
		return;
	}
	size = h.sbuf.st_size;
	for(n = sizeof buf; size > 0; size -= n){
		if(n > size)
			n = size;
		if(Read(&s, buf, (n+s.blocksize-1)/s.blocksize))
			logprint("%s: %s read error=%d\n", me, iname, errno);
		if(Fwrite(ofd, buf, n) != n)
			return;
	}
	nf++;
	nb += h.sbuf.st_size;
	announce(iname);
}

permok(h)
	register struct backfile *h;
{
/*logprint("permok(%s:%s, %s:%s, 0%o\n", h->uname, usname, h->gname, gpname, h->sbuf.st_mode&0777);/**/
	if(strcmp(h->uname, usname) == 0)
		return(h->sbuf.st_mode&0400);
	if(strcmp(h->gname, gpname) == 0)
		return(h->sbuf.st_mode&0040);
	return(h->sbuf.st_mode&0004);
}

fail(s)
	char *s;
{
	char *os = s;
	int n, i;

	/* get second last path element */
	if(s = strrchr(s, '/')){
		while(s > os)
			if(*--s == '/'){
				s++;
				break;
			}
	} else
		s = os;
	while(*s){
		if((s[0] == 'v') && (s[1] >= '0') && (s[1] <= '9'))
			break;
		s++;
	}
	n = atoi(s+1);
	for(i = 0; i < nvol; i++)
		if(n < vol[i].vol) break;
	vol[--i].fail++;
}

fdump()
{
	register i;

	for(i = 0; i < nvol; i++)
		if(vol[i].fail){
			Fprint(ofd, "%d file%s need%s disk %s\n", vol[i].fail,
				(vol[i].fail==1)?"":"s", (vol[i].fail==1)?"s":"",
				vol[i].name);
			vol[i].fail = 0;
		}
}

char abuf[4096];
int nsofar;
char *np = abuf;
#define	NPERLINE	5

announce(s)
	char *s;
{
	if(s == 0){
		if(nsofar){
			logprint("%s: retrieved%s\n", me, abuf);
			np = abuf;
			nsofar = 0;
		}
		return;
	}
	*np++ = ' ';
	while(*np++ = *s++)
		;
	np--;
	if(++nsofar == NPERLINE)
		announce((char *)0);
}
