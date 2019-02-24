#include "netb.h"
#include "nberrno.h"
#include "rf.h"

/*
 * netb functions
 */

static int illcmd();
int _rfput(), _rfupdate(), _rfread(), _rfwrite();
int _rfnamei(), _rfstat(), _rfdir(), _rftrunc();

static (*functab[])() = {
	illcmd,		/* (0) illegal */
	_rfput,		/* (1) put */
	illcmd,		/* (2) get */
	_rfupdate,	/* (3) update */
	_rfread,	/* (4) read */
	_rfwrite,	/* (5) write */
	_rfnamei,	/* (6) namei */
	_rfstat,	/* (7) stat */
	illcmd,		/* (8) ioctl */
	_rftrunc,	/* (9) trunc */
	_rfdir,	/* (10) dirread */
};

#define	MAXCMD	10

/*
 * names only for debugging output
 */
static char *funcnames[] = {
	"ill", "put", "get", "upd", "read",
	"write", "namei", "stat", "ioctl", "trunc", "dir"
};

static char *ninames[] = {
	"reg", "del", "creat", "xcreat", "link", "mkdir", "rmdir"
};
#define	MAXNI	6

extern int cuid, cgid;

static slide();

_rfscan(fd, buf, size)
int fd;
char *buf;
int size;
{
	register unsigned char *sp;
	register int blen, mlen;
	register int n, c;

	sp = (unsigned char *)buf;
	blen = 0;
	for (;;) {
		if (blen < SNBSIZE) {
			n = _rfgread(fd, buf+blen, size-blen, SNBSIZE);
			if (n < SNBSIZE)
				break;
			blen += n;
		}
		c = frnetchar(sp, SNB_CMD);
		if (c < 0 || c > MAXCMD)
			c = 0;
		mlen = frnetlong(sp, SNB_LEN);
		if (rfdebug)
			rflog("%ld: %s: cmd %d tag x%lx flag %d len %ld\n",
				frnetlong(sp, SNB_TRANNUM), funcnames[c],
				frnetchar(sp, SNB_CMD), frnetlong(sp, SNB_TAG),
				frnetchar(sp, SNB_FLAGS), mlen);
		if (frnetchar(sp, SNB_VERSION) != NETB)
			rfpanic("ill version: %d\n", frnetchar(sp, SNB_VERSION));
		if (mlen > size)
			rfpanic("msg too long: %d\n", mlen);
		if (mlen > blen) {
			n = _rfgread(fd, buf+blen, size-blen, mlen-blen);
			if (n < mlen - blen)
				break;
			blen += n;
		}
		if (rfdebug
		&&  c == NBNAMI && frnetchar(sp, SNB_FLAGS) <= MAXNI) {
			rflog("\tnamei: %s '%.*s'\n", ninames[frnetchar(sp, SNB_FLAGS)],
				mlen - SNMSIZE, sp + SNMSIZE);
		}
		cuid = frnetshort(sp, SNB_UID);
		cgid = frnetshort(sp, SNB_GID);
		rfuid = _rfsuid(cuid);
		rfgid = _rfsgid(cgid);
		(*functab[c])(fd, sp, mlen);
		blen -= mlen;
		if (blen != 0) {	/* leftover stuff in buffer; shouldn't happen */
			rflog("%d bytes excess read\n", blen);
			slide(buf+mlen, n, mlen);
		}
	}
	if (n)
		rfpanic("bad read: %d\n", n);
}

static
slide(buf, len, dist)
char *buf;
register int len;
int dist;
{
	register char *fr, *to;

	fr = buf;
	to = buf - dist;
	while (--len >= 0)
		*to++ = *fr++;
}

static
illcmd(fd, sp)
int fd;
unsigned char *sp;
{
	unsigned char rbuf[RNBSIZE];

	tonetshort(rbuf, RNB_ERRNO, NBEINVAL);
	_rfresp(fd, sp, rbuf, RNBSIZE);
}

/*
 * send a response
 */

_rfresp(fd, sp, rp, len)
int fd;
register unsigned char *sp, *rp;
int len;
{

	if (rfdebug)
		rflog("\t_rfresp: %ld: errno %d flags %d len %ld\n",
			frnetlong(sp, SNB_TRANNUM), frnetshort(rp, RNB_ERRNO),
			frnetchar(rp, RNB_FLAGS), len);
	tonetlong(rp, RNB_TRANNUM, frnetlong(sp, SNB_TRANNUM));
	tonetlong(rp, RNB_LEN, len);
	if (write(fd, (char *)rp, len) != len)
		rfpanic("bad _rfresp write\n", 0);
}
