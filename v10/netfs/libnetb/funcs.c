#include "netb.h"
#include "nberrno.h"
#include "rf.h"
#include "tag.h"

#define	NULL	0
extern char *strchr();
extern int defaultuid, defaultgid;
extern int cuid, cgid;

#define	ARB	4096

/*
 * netb-to-local service translation
 */

static long timeoff;
static timecheck();
static Rfile *root;	/* should this be a global? */
static int nberrno;

/*
 * magic knowledge: fserrno and nberrno codes are the same
 */
#define	fstonberr(e)	(e)

/*
 * compatibility with old clients
 * that don't generate NBROOTTAG
 */
static Tag oldrtag;
static int oldrdev;
#define	OLDRINO	2L
static int oldclient;	/* it used oldrtag */
static int sawroot;

static Rfile *getfile();
static putmode();

/*
 * make a tag from a file
 * the root is an exception; it has NBROOTTAG, always
 * 0x80000000 is a temporary protection against confusion with oldrtag
 */
#define	maketag(f)	(0x80000000|((f)->dev<<16)|((f)->ino&0xffff))

/*
 * file modes
 * not specified in netb.h,
 * but they are the same as V7 Unix
 */

#define	NB_IFMT	0170000	/* file type */
#define	NB_IFDIR	0040000	/* directory */
#define	NB_IFREG	0100000	/* regular file */
#define	NB_PERM	07777	/* permissions */

#define	fstonbmode(m)	(m)	/* other modes (assumed) same as rf */
#define	nbtofsmode(m)	((m)&07777)	/* modes that may be set */

long time();

/*
 * init things;
 * called before the connection is fully open
 * mainly, set up the root
 */

int
_rfinit(argc, argv)
int argc;
char **argv;
{

	if ((root = fsinit(argc, argv)) == NULL) {
		rflog("init failed\n");
		return (0);
	}
	if (_rfnewtag((Tag)NBROOTTAG, root) != root) {
		rflog("can't install root\n");
		return (0);
	}
	return (1);
}

/*
 * namei
 *
 * hand the pathname to fswalk, a piece at a time
 * on reaching the end, do the operation implied by SNB_FLAGS
 * fswalk might return an Rfile that is already in our table;
 * keep the older one.
 * -- be careful when calling fsdone on intermediate directories
 * in the path; they might be in use in some other way, and
 * fsdone will wreak havoc.
 * dirintab is there so _rfdone needn't be called every time,
 * especially for directories we know are already in the table
 */

_rfnamei(fd, sp, len)
int fd;
unsigned char *sp;
int len;
{
	unsigned char rbuf[RNMSIZE];
	register unsigned char *pp;
	register Rfile *f, *df;
	Rfile *lnf;
	int dirintab;
	register char *cp, *ep;
	Tag t;
	int cruid, crgid, crmode;
	long tm;

	nberrno = 0;
	tonetchar(rbuf, RNB_FLAGS, 0);
	pp = sp;
	if (len < SNMSIZE)
		rfpanic("namei: len %d\n", len);
	pp[len] = 0;	/* guarantee null at end of name */
	cp = (char *)pp + SNMSIZE;
	while (*cp == '/')
		cp++;
	dirintab = 1;
	if ((df = getfile(&pp[SNB_TAG])) == NULL) {
		nberrno = NBEINVAL;
		goto out;
	}
	while ((ep = strchr(cp, '/')) != NULL) {
		*ep++ = 0;
		while (*ep == '/')
			ep++;
		if (df->type != RFTDIR) {
			nberrno = NBENOENT;
			goto out;
		}
		if (_rfpdsearch(df, rfuid, rfgid) == NULL) {
			nberrno = NBEACCES;
			goto out;
		}
		if ((f = fswalk(df, cp)) == NULL)
			goto eout;
		if (f != df && dirintab == 0)
			fsdone(df);
		if (f->ino == root->ino && f->dev == root->dev) {
			if (f != root)
				fsdone(f);
			df = root;
			dirintab = 1;
		} else if ((df = _rftagtof(maketag(f))) != NULL) {
			fsdone(f);
			dirintab = 1;
		} else {
			df = f;
			dirintab = 0;
		}
		cp = ep;
	}
	/*
	 * cp is now last component
	 *
	 * the tests that follow are wrong if the root isn't a directory
	 * think about implications
	 */
	if (df->type != RFTDIR) {
		nberrno = NBENOENT;
		goto out;
	}
	if (_rfpdsearch(df, rfuid, rfgid) == 0) {
		nberrno = NBEACCES;
		goto out;
	}
	switch (frnetchar(pp, SNB_FLAGS)) {
	case NI_SEARCH:
		if (cp[0] == 0) {		/* empty filename */
			f = df;
			fsstat(f);		/* get fresh info */
			break;
		}
		if ((f = fswalk(df, cp)) == NULL)
			goto eout;
		break;

	case NI_NXCREAT:
	case NI_CREAT:
		cruid = rfuid;
		if (cruid == RFNOID && defaultuid != RFNOID)
			cruid = defaultuid;
		crgid = rfgid;
		if (crgid == RFNOID && defaultgid != RFNOID)
			crgid = defaultgid;
		crmode = frnetshort(pp, SNM_MODE);
		if ((f = fswalk(df, cp)) == NULL) {	/* doesn't exist */
			if (cruid == RFNOID || crgid == RFNOID	/* don't create ownerless files */
			||  _rfpdwrite(df, cruid, crgid) == 0) {
				nberrno = NBEACCES;
				goto out;
			}
		} else if (frnetchar(pp, SNB_FLAGS) == NI_NXCREAT) {
			_rfdone(f);
			nberrno = NBEEXIST;	/* NXCREAT means it shouldn't exist */
			goto out;
		} else {
			if (_rfpwrite(f, cruid, crgid) == 0) {
				_rfdone(f);
				nberrno = NBEACCES;
				goto out;
			}
			cruid = f->uid;
			crgid = f->gid;
			crmode = f->mode;
			_rfdone(f);		/* we'll re-create anyway */
		}
		if ((f = fscreate(df, cp, crmode, cruid, crgid)) == NULL)
			goto eout;
		break;

	case NI_LINK:
		if ((lnf = getfile(&pp[SNM_INO])) == NULL) {
			nberrno = NBEINVAL;
			goto out;
		}
#if NOTDEF
		if (lnf->type == RFTDIR && _rfpsuper(lnf, rfuid, rfgid) == 0) {
#else
		if (lnf->type == RFTDIR) {
#endif
			nberrno = NBEISDIR;
			goto out;
		}
		if (_rfpdwrite(df, rfuid, rfgid) == 0) {
			nberrno = NBEACCES;
			goto out;
		}
		if (fslink(df, cp, lnf) < 0)
			goto eout;
		goto out;

	case NI_MKDIR:
		cruid = rfuid;
		if (cruid == RFNOID && defaultuid != RFNOID)
			cruid = defaultuid;
		crgid = rfgid;
		if (crgid == RFNOID && defaultgid != RFNOID)
			crgid = defaultgid;
		if (cruid == RFNOID || crgid == RFNOID
		||  _rfpdwrite(df, cruid, crgid) == 0) {
			nberrno = NBEACCES;
			goto out;
		}
		if (fsmkdir(df, cp, frnetshort(pp, SNM_MODE), cruid, crgid) < 0)
			goto eout;
		goto out;

	case NI_RMDIR:
		if (_rfpdwrite(df, rfuid, rfgid) == 0) {
			nberrno = NBEACCES;
			goto out;
		}
		if (fsrmdir(df, cp) < 0)
			goto eout;
		goto out;

	case NI_DEL:
		if (_rfpdwrite(df, rfuid, rfgid) == 0) {
			nberrno = NBEACCES;
			goto out;
		}
		if ((f = fswalk(df, cp)) == NULL)
			goto eout;
#if NOTDEF
		if (f->type == RFTDIR && _rfpsuper(f, rfuid, rfgid) == 0) {
#else
		if (f->type == RFTDIR) {
#endif
			_rfdone(f);
			nberrno = NBEISDIR;
			goto out;
		}
		_rfdone(f);
		if (fsdelete(df, cp) < 0)
			goto eout;
		goto out;

	default:
		nberrno = NBEINVAL;
		goto out;
	}
	/*
	 * returning a newly-opened file
	 */
	if (f->dev == root->dev && f->ino == root->ino)
		t = NBROOTTAG;
	else
		t = maketag(f);
	if ((lnf = _rfnewtag(t, f)) == NULL) {
		fsdone(f);
		nberrno = NBEINVAL;
		goto out;
	} else if (lnf != f) {		/* already had that file */
		fsdone(f);
		f = lnf;
	}
	if (f == df)
		dirintab = 1;	/* we just made sure of that */
	pp = rbuf;
	tonetlong(pp, RNM_TAG, t);
	tonetlong(pp, RNM_INO, f->ino);
	tonetshort(pp, RNM_DEV, f->dev);
	putmode(pp, RNM_MODE, f);
	tonetshort(pp, RNM_NLINK, f->nlink);
	/*
	 * the default ownership here might be wrong...check it.
	 */
	if (defaultuid != RFNOID && rfuid == RFNOID && f->uid == defaultuid)
		len = cuid;	/* tell him he owns it */
	else
		len = _rfcuid(f->uid);
	tonetshort(pp, RNM_UID, len);
	len = _rfcgid(f->gid);	/* stub - this gid processing may be wrong */
	if (len == RFNOID)
		len = cgid;
	tonetshort(pp, RNM_GID, len);
	tonetshort(pp, RNM_RDEV, f->rdev);
#if 0x100000000 == 0	/* 32-bit machine */
	tonetlong(pp, RNM_SIZE, f->size);
#else			/* probably the Cray */
	if ((tm = f->size) > 0x7fffffff)
		tm = 0x7fffffff;
	tonetlong(pp, RNM_SIZE, tm);
#endif
	tm = f->ta + timeoff;
	tonetlong(pp, RNM_ATIME, tm);
	tm = f->tm + timeoff;
	tonetlong(pp, RNM_MTIME, tm);
	tm = f->tc + timeoff;
	tonetlong(pp, RNM_CTIME, tm);
	if (oldclient) {
		len = (f->dev & 0xff) | (oldrdev & 0xff00);
		tonetshort(pp, RNM_DEV, len);
		if (f == root) {
			tonetlong(pp, RNM_TAG, oldrtag);
			tonetshort(pp, RNM_DEV, oldrdev);
			tonetlong(pp, RNM_INO, OLDRINO);
		}
	}
	goto out;

	/*
	 * failed to find a file
	 * if nberrno == 0, popped out of root
	 */
eout:
	if (fserrno)
		nberrno = fstonberr(fserrno);
	else {
		nberrno = 0;
		tonetchar(rbuf, RNB_FLAGS, NBROOT);
		while (*cp && *cp != '/')
			cp++;
		len = cp - (char *)(sp + SNMSIZE);
		if (rfdebug)
			rflog("popped out; used %d\n", len);
		tonetlong(rbuf, RNM_USED, len);
	}
	/*
	 * done
	 */
out:
	if (dirintab == 0)
		_rfdone(df);
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rbuf, RNMSIZE);
}

/*
 * discard f if it is not in the tag table
 * used by rfnamei to discard files fetched halfway,
 * e.g. directories in the middle of a path
 */

_rfdone(f)
register Rfile *f;
{

	if (f == root)
		return;
	if (_rftagtof(maketag(f)) == f)
		return;
	fsdone(f);
}

/*
 * put
 */

_rfput(fd, sp, len)
int fd;
register unsigned char *sp;
int len;
{
	unsigned char rbuf[RNBSIZE];
	Tag t;
	Rfile *f;

	t = frnetlong(sp, SNB_TAG);
	if ((f = getfile(&sp[SNB_TAG])) == NULL)
		nberrno = NBEINVAL;
	else {
		_rfdeltag(t);
		nberrno = 0;
		fsdone(f);
	}
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rbuf, RNBSIZE);
}

/*
 * stat
 */

_rfstat(fd, sp, len)
int fd;
unsigned char *sp;
int len;
{
	int i;
	unsigned char rbuf[RSTSIZE];
	register unsigned char *rp;
	register Rfile *f;
	long tm;

	if (len < SSTSIZE)
		rfpanic("stat: len %d\n", len);
	rp = rbuf;
	if ((f = getfile(&sp[SNB_TAG])) == NULL)
		nberrno = NBEINVAL;
	else if (fsstat(f) < 0)
		nberrno = fstonberr(fserrno);
	else {
		nberrno = 0;
		/* should have hysteresis here */
		timecheck(frnetlong(sp, SST_TIME));
		tonetlong(rp, RST_INO, f->ino);
		tonetshort(rp, RST_DEV, f->dev);
		putmode(rp, RST_MODE, f);
		tonetshort(rp, RST_NLINK, f->nlink);
		if (defaultuid != RFNOID && rfuid == RFNOID && f->uid == defaultuid)
			i = cuid;	/* let the client think he owns it */
		else
			i = _rfcuid(f->uid);
/*rflog("stat: f->uid=%d defaultuid=%d cuid=%d rfcuid=%d returning %d\n",
f->uid, defaultuid, cuid, _rfcuid(f->uid), i);*/
		tonetshort(rp, RST_UID, i);
		i = _rfcgid(f->gid);	/* stub - default gid processing? */
		tonetshort(rp, RST_GID, i);
#if 0x100000000 == 0		/* 32-bit machine */
		tonetlong(rp, RST_SIZE, f->size);
#else				/* probably the Cray */
		if ((tm = f->size) > 0x7fffffff)
			tm = 0x7fffffff;
		tonetlong(rp, RST_SIZE, f->size);
#endif
		tm = f->ta + timeoff;
		tonetlong(rp, RST_ATIME, tm);
		tm = f->tm + timeoff;
		tonetlong(rp, RST_MTIME, tm);
		tm = f->tc + timeoff;
		tonetlong(rp, RST_CTIME, tm);
	}
	tonetshort(rp, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rp, RSTSIZE);
}

/*
 * update:
 * write attributes
 */

_rfupdate(fd, sp, len)
int fd;
register unsigned char *sp;
int len;
{
	unsigned char rbuf[RNBSIZE];
	static Rfile na;
	Rfile *f;

	if (len < SUPSIZE)
		rfpanic("update: len %d\n", len);
	if ((f = getfile(&sp[SNB_TAG])) == NULL)
		nberrno = NBEINVAL;
	else {
		na.mode = nbtofsmode(frnetshort(sp, SUP_MODE));
		na.ta = frnetlong(sp, SUP_ATIME);
		na.tm = frnetlong(sp, SUP_MTIME);
		if (na.ta == 0)		/* current time: local system did it */
			na.ta = f->ta;
		else
			na.ta -= timeoff;
		if (na.tm == 0)
			na.tm = f->tm;
		else
			na.tm -= timeoff;
		na.uid = f->uid;	/* netb doesn't supply */
		na.gid = f->gid;
		na.size = f->size;	/* not set here */
		if ((na.mode & NB_PERM) != (fstonbmode(f->mode) & NB_PERM)
		&&  _rfpowner(f, rfuid, rfgid) == 0)
			nberrno = NBEPERM;
		else if (fsupdate(f, &na) < 0)
			nberrno = fstonberr(fserrno);
		else
			nberrno = 0;
	}
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rbuf, RNBSIZE);
}

/*
 * read
 */
_rfread(fd, sp, mlen)
int fd;
register unsigned char *sp;
int mlen;
{
	unsigned char rbuf[RNBSIZE+ARB];	/* should be negotiated size */
	Rfile *f;
	int len;

	if (mlen < SRDSIZE)
		rfpanic("read: len %d\n", mlen);
	len = frnetlong(sp, SRD_LEN);
	tonetchar(rbuf, RNB_FLAGS, 0);
	if (len <= 0 || (f = getfile(&sp[SNB_TAG])) == NULL) {
		nberrno = NBEINVAL;
		len = 0;
	} else if (_rfpread(f, rfuid, rfgid) == 0) {
		nberrno = NBEACCES;
		len = 0;
	} else {
		if (len > ARB)
			len = ARB;
		len = fsread(f, frnetlong(sp, SRD_OFFSET),
			(char *)rbuf + RNBSIZE, len);
		if (len < 0) {
			nberrno = fstonberr(fserrno);
			len = 0;
		} else {
			nberrno = 0;
			if (len == 0)
				tonetchar(rbuf, RNB_FLAGS, NBEND);
		}
	}
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	len += RNBSIZE;
	_rfresp(fd, sp, rbuf, len);
}

/*
 * read directory
 */
_rfdir(fd, sp, mlen)
int fd;
register unsigned char *sp;
int mlen;
{
	unsigned char rbuf[RDISIZE+ARB];
	Rfile *f;
	int len;
	long noff;

	if (mlen < SRDSIZE)
		rfpanic("readdir: len %d\n", mlen);
	len = frnetlong(sp, SRD_LEN);
	if (len <= 0 || (f = getfile(&sp[SNB_TAG])) == NULL) {
		nberrno = NBEINVAL;
		len = 0;
	} else if (f->type != RFTDIR) {
		nberrno = NBENOTDIR;
		len = 0;
	} else if (_rfpdread(f, rfuid, rfgid) == 0) {
		nberrno = NBEACCES;
		len = 0;
	} else {
		if (len > ARB)
			len = ARB;
		len = fsdirread(f, frnetlong(sp, SRD_OFFSET),
			(char *)rbuf + RDISIZE, len, &noff);
		if (len < 0) {
			nberrno = fstonberr(fserrno);
			len = 0;
		} else {
			nberrno = 0;
			noff -= frnetlong(sp, SRD_OFFSET);
			tonetlong(rbuf, RDI_USED, noff);
		}
	}
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	len += RDISIZE;
	_rfresp(fd, sp, rbuf, len);
}

/*
 * write
 * --permissions botch:
 * if we just check for write permission,
 * f = creat(name, 0) will leave a file descriptor we can't write
 * hence the owner check
 * if the client wants to protect the user better,
 * it can do its own checks;
 * here we are concerned with the server
 */
_rfwrite(fd, sp, mlen)
int fd;
register unsigned char *sp;
int mlen;
{
	unsigned char rbuf[RNBSIZE];
	Rfile *f;
	int n;
	long off;

	n = frnetlong(sp, SWR_LEN);
	if (mlen != SWRSIZE + n)
		rfpanic("write: size skew %d\n", mlen);
	off = frnetlong(sp, SWR_OFFSET);
	nberrno = 0;
	if ((f = getfile(&sp[SNB_TAG])) == NULL)
		nberrno = NBEINVAL;
	else if (_rfpwrite(f, rfuid, rfgid) == 0
	     &&  _rfpowner(f, rfuid, rfgid) == 0)
		nberrno = NBEACCES;
	else if ((n = fswrite(f, off, (char *)sp + SWRSIZE, n)) < 0)
		nberrno = fstonberr(fserrno);
	tonetlong(rbuf, RNB_FSIZE, f->size);
	tonetshort(rbuf, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rbuf, RNBSIZE);
}

/*
 * truncate file
 */
_rftrunc(fd, sp, len)
int fd;
register unsigned char *sp;
int len;
{
	unsigned char rbuf[RNBSIZE];
	register Rfile *f;
	Rfile newf;

	nberrno = 0;
	if ((f = getfile(&sp[SNB_TAG])) == NULL)
		nberrno = NBEINVAL;
	else if (_rfpwrite(f, rfuid, rfgid) == 0
	     &&  _rfpowner(f, rfuid, rfgid) == 0)	/* wrong? */
		nberrno = NBEACCES;
	else {
		newf = *f;
		newf.size = 0;
		if (fsupdate(f, &newf) < 0)
			nberrno = fstonberr(fserrno);
	}
	tonetlong(rbuf, RNB_ERRNO, nberrno);
	_rfresp(fd, sp, rbuf, RNBSIZE);
}

/*
 * turn SNB_TAG (or some other tag)
 * into an Rfile
 */

static Rfile *
getfile(pp)
register unsigned char *pp;
{
	Tag t;
	Rfile *f;

	t = frnetlong(pp, 0);
	if (t == NBROOTTAG || (oldclient && t == oldrtag)) {
		sawroot = 1;
		return (root);		/* easy */
	}
	if ((f = _rftagtof(t)) != NULL)
		return (f);
	if (sawroot == 0 && (t & 0xffff) == OLDRINO) {	/* old-style root tag? */
		oldrtag = t;
		oldrdev = t >> 16;
		oldclient = 1;
		rflog("old client; root tag %lx\n", t);
		sawroot = 1;
		return (root);
	}
	nberrno = NBEINVAL;	/* shouldn't happen */
	rflog("bad tag %lx\n", t);
	return (NULL);
}

static
putmode(pp, off, f)
register unsigned char *pp;
int off;
register Rfile *f;
{
	int mode;

	mode = fstonbmode(f->mode);
	if (f->type == RFTDIR) {
		mode &= 07777;
		mode |= NB_IFDIR;
	} else if ((mode & NB_IFMT) == 0)
		mode |= NB_IFREG;
	/* else pass mode through unchanged for now */
	tonetshort(pp, off, mode);
}

/*
 * correct offset twixt client time and ours
 * only so often, or if difference is large;
 * e.g. hosts synchronize from a common clock,
 * but at different times
 */

static
timecheck(cltime)
long cltime;
{
	static int stall;
	static long lastck;
	long ourtime;
	int diff;

	if (stall-- != 0)	/* only every few stats */
		return;
	stall = 3;
	ourtime = time((long *)0);
	diff = cltime - ourtime;
	if (diff == timeoff) {
		lastck = ourtime;
		return;		/* no change */
	}
	if (diff - timeoff < 2 && diff - timeoff > -2	/* modest change */
	&&  ourtime - lastck < 90*60)		/* and last change was recent */
		return;
	lastck = ourtime;
	rflog("time: client %ld server %ld old diff %d new %d\n",
		cltime, ourtime, timeoff, diff);
	timeoff = diff;
}
