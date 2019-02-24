/*
 * face server, library style
 */
#include <rf.h>
#include <ipc.h>
#include <signal.h>
#include "faceproto.h"

#define	NULL	0
char *malloc();


/*
 * ignore permissions from server for now (who cares?)
 * uid, gid always RFNOID
 * permissions always read for everyone
 */
#define	FPERM	((RFPRD<<RFPOWNER)|(RFPRD<<RFPGROUP)|(RFPRD<<RFPOTHER))
#define	DPERM	(FPERM|(RFPDS<<RFPOWNER)|(RFPDS<<RFPGROUP)|(RFPDS<<RFPOTHER))

char *server;
int commfd = -1;
static int ininit;

#define	TIMEOUT	(2*60)	/* disconnect if idle this long */

/*
 * init:
 * we have to return something,
 * but the server might not be running when we start
 * so fake up a root for now
 */

Rfile *
fsinit(argc, argv)
int argc;
char **argv;
{
	register Rfile *f;
	int timeout();

	if (argc <= 1)
		rfpanic("usage: a.out server\n");
	server = argv[1];
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL)
		rfpanic("no mem for root\n");
	f->fs = "/";
	f->ino = FROOT;
	f->dev = 0;
	f->type = RFTDIR;
	f->mode = DPERM;
	f->size = 512;	/* something nonzero */
	f->nlink = 1;
	f->ta = f->tm = f->tc = 0;
	signal(SIGALRM, timeout);
	ininit = 1;
	if (fsstat(f) < 0)
		fserrno = 0;	/* ignore error, use the fake data */
	ininit = 0;
	return (f);
}

timeout(s)
int s;
{
	signal(s, timeout);
	if (rfdebug)
		rflog("timeout\n");
	callshutdown();
}

/*
 * cheap hack:
 * if we've already checked the root,
 * short-circuit namei calls for it,
 * so status programs (like netb setup) won't make us
 * keep a circuit open
 */
Rfile *
fswalk(df, name)
Rfile *df;
char *name;
{
	register Rfile *f;
	char stbuf[FMAXDATA];
	long rcode;
	char *nname;

	if (df->ino == FROOT
	&&  (name[0] == 0 || (name[0] == '.' && name[1] == 0)))
		return (df);
	if (df->ino == FROOT && strcmp(name, "..") == 0) {
		fserrno = 0;
		return (NULL);
	}
	if ((nname = malloc(FMAXPATH)) == NULL) {
		rflog("no mem for name: %s/%s\n", df->fs, name);
		fserrno = RFEINVAL;
		return (NULL);
	}
	strcpy(nname, df->fs);
	strcat(nname, "/");
	strcat(nname, name);
	if (facecall(nname, DOSTAT, 0L, 0L, stbuf, &rcode) < STLEN) {
		/* call failed or network botch */
		fserrno = RFEINVAL;
		free(nname);
		return (NULL);
	}
	if (rcode < 0) {
		fserrno = RFENOENT;
		free(nname);
		return (NULL);
	}
	if ((f = (Rfile *)malloc(sizeof(Rfile))) == NULL) {
		rflog("no mem for file: %s\n", nname);
		fserrno = RFEINVAL;
		free(nname);
		return (NULL);
	}
	f->fs = nname;
	stunpack(stbuf, f);
	return (f);
}

int
fsput(f)
Rfile *f;
{
	free(f->fs);
	free((char *)f);
	return (0);
}

int
fsstat(f)
Rfile *f;
{
	char stbuf[FMAXDATA];
	long rcode;

	if (f->ino == FROOT && ininit == 0)
		return (0);	/* and don't call */
	if (facecall(f->fs, DOSTAT, 0L, 0L, stbuf, &rcode) < STLEN) {
		fserrno = RFEINVAL;
		return (-1);
	}
	if (rcode < 0) {
		fserrno = RFEINVAL;
		return (-1);
	}
	stunpack(stbuf, f);
	return (0);
}

/*
 * read directories
 */
int
fsdirread(f, off, buf, len, noffp)
Rfile *f;
long off;
char *buf;
int len;
long *noffp;
{
	char dirbuf[FDLEN*32];	/* some even multiple */
	char onebuf[FDLEN-FD_NAME + 10];	/* enough for one entry */
	register unsigned char *dp;
	register int nread, n;
	register char *bp;

	if ((off%FDLEN) != 0) {
		rflog("unaligned fsdirread: off %ld\n", off);
		fserrno = RFEINVAL;
		return (-1);
	}
	bp = buf;
	while ((nread = fsread(f, off, dirbuf, sizeof(dirbuf))) > 0) {
		dp = (unsigned char *)dirbuf;
		while (nread >= FDLEN) {
			/* should check ino==0 here */
			sprintf(onebuf, "%d\t%.*s", frfshort(dp, FD_INO),
				FDLEN - FD_NAME, dp + FD_NAME);
			n = strlen(onebuf)+1;	/* including NUL */
			if ((len -= n) < 0)
				goto out;
			memcpy(bp, onebuf, n);
			bp += n;
			off += FDLEN;
			dp += FDLEN;
			nread -= FDLEN;
		}
	}
out:
	*noffp = off;
	if (bp != buf)		/* if we read anything, return it */
		return (bp - buf);
	if (len < 0) {		/* first entry too large */
		fserrno = RFENOSPC;
		return (-1);
	}
	return (nread);		/* must be 0 or -1 */
}

int
fsread(f, off, buf, len)
Rfile *f;
char *buf;
int len;
long off;
{
	char data[FMAXDATA];
	long rcode;

	if (facecall(f->fs, DOREAD, off, (long)len, data, &rcode) < 0) {
		fserrno = RFEINVAL;
		return (-1);
	}
	if (rcode < 0) {
		fserrno = RFEIO;
		return (-1);
	}
	if (rcode > len)
		rfpanic("fsread wanted %d got %ld\n", len, rcode);
	if (rcode)
		memcpy(buf, data, (int)rcode);
	return (rcode);
}

/* need fsdirread too */

/*
 * one function call to the server
 * interface is overly general and underused
 * there are a filename, a function code, and two long parameters;
 * some data is returned in result,
 * and a long status code is returned in *codep
 * the function return value is the length of the data;
 * -1 means error
 */
int
facecall(file, func, p1, p2, result, codep)
char *file;
int func;
long p1, p2;
char *result;
long *codep;
{
	register unsigned char *p;
	register char msg[F_DATA+FMAXDATA];
	int len;

	alarm(0);
	if (commfd < 0 && callsetup() < 0)
		return (-1);
	len = strlen(file) + 1;
	if (len > FMAXDATA)
		rfpanic("facecall: name too long\n");
	p = (unsigned char *)msg;
	p[F_TYPE] = func;
	p[F_TYPE+1] = 0;	/* safety for old servers */
	tofshort(p, F_LEN, len);
	toflong(p, F_P1, p1);
	toflong(p, F_P2, p2);
	memcpy(&p[F_DATA], file, len);
	if (rfdebug)
		rflog("facecall: %s: func %d p %ld %ld\n", file, func, p1, p2);
	if (write(commfd, msg, len+F_DATA) != len+F_DATA) {
		rflog("net write error\n");
		callshutdown();
		return (-1);
	}
	if (gread(commfd, msg, F_DATA, F_DATA) != F_DATA) {
		rflog("net read error\n");
		callshutdown();
		return (-1);
	}
	len = frfshort(p, F_LEN);
	if (len < 0 || len > FMAXDATA) {
		rflog("net ill len %d\n", len);
		callshutdown();
		return (-1);
	}
	if (len > 0 && gread(commfd, result, len, len) != len) {
		rflog("net read error (body)\n");
		callshutdown();
		return (-1);
	}
	*codep = frflong(p, F_P1);
	if (rfdebug)
		rflog("... len %d p1 %ld\n", len, *codep);
	alarm(TIMEOUT);
	return (len);
}

/*
 * call server
 */
callsetup()
{
	extern char *errstr;

	if (rfdebug)
		rflog("call %s\n", server);
	if ((commfd = ipcopen(ipcpath(server, "", ""), "")) < 0)
		rflog("can't call %s: %s\n", server, errstr);
	return (commfd);
}

callshutdown()
{
	close(commfd);
	commfd = -1;
}

/*
 * unpack stat buffer into a file
 */

stunpack(buf, f)
char *buf;
register Rfile *f;
{
	register unsigned char *p;

	p = (unsigned char *)buf;
	f->ino = frfshort(p, ST_INO);
	f->dev = 0;
	if ((frfshort(p, ST_MODE) & STFMT) == STDIR) {
		f->type = RFTDIR;
		f->mode = DPERM;
	} else {
		f->type = RFTREG;
		f->mode = FPERM;
	}
	f->nlink = frfshort(p, ST_NLINK);
	f->uid = RFNOID;
	f->gid = RFNOID;
	f->size = frflong(p, ST_SIZE);
	f->ta = frflong(p, ST_ATIME);
	f->tm = frflong(p, ST_MTIME);
	f->tc = frflong(p, ST_CTIME);
}

/*
 * gather data that may come in dribs and drabs:
 * read size bytes into buf,
 * but keep looking until at least minsize have arrived
 */

int
gread(fd, buf, size, minsize)
int fd;
char *buf;
int size;
register int minsize;
{
	register int n, tot;

	tot = 0;
	while (minsize > 0) {
		if ((n = read(fd, buf, size)) <= 0)
			break;
		buf += n;
		size -= n;
		minsize -= n;
		tot += n;
	}
	if (tot)
		return (tot);
	return (n);
}
