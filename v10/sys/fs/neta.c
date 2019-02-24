#include "sys/param.h"
#include "sys/systm.h"
#include "sys/inode.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/stat.h"
#include "sys/neta.h"
#include "sys/buf.h"
#include "sys/stream.h"
#include "sys/conf.h"

/*
 * obsolete network filesystem protocol
 *
 * i_fsflags is used in various places instead of the
 * obsolete i_key
 * this is a cheat
 * it is allowed only because neta will soon vanish
 */

struct senda nilx;
static long natrannum;
#define ND 100
struct {
	char len, s, n;
	struct senda x[ND];
	struct rcva y[ND];
} netabuf = {ND};
#define addx(z) netabuf.x[netabuf.n] = *z; netabuf.s = 1;
#define addy(z) netabuf.y[netabuf.n++] = *z; if(netabuf.n >= ND) netabuf.n = 0; netabuf.s = 0;

int naput(), naupdat(), naread(), nawrite(), natrunc(), nastat();
int nanami(), namount();
struct fstypsw nafs =
fsinit(naput, naupdat, naread, nawrite, natrunc, nastat,
	nanami, namount, nodev, nullopen, nodev);

namount(sip, ip, flag, mnt, fstyp)
struct inode *ip, *sip;
{
	if (!suser())
		return;
	if (mnt)
		nadomount(sip, ip, flag, fstyp);
	else
		naunmount(ip, fstyp);
}

nadomount(cip, dip, flag, fstyp)
register struct inode *cip, *dip;
{
	struct inode pi;
	register struct inode *rip;

	if(cip->i_sptr == NULL) {
		u.u_error = ENXIO;
		return;
	}
	if((dip->i_mode & IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		return;
	}
	/*
	 * take care, because of the stupid user-generated device
	 */
	if (dip->i_fstyp == fstyp && dip->i_dev == flag) {
		u.u_error = EBUSY;
		return;
	}
	pi.i_dev = flag;
	pi.i_fstyp = fstyp;
	if ((rip = iget(&pi, flag, ROOTINO)) == NULL)
		return;
	if (rip->i_count > 1) {
		iput(rip);
		u.u_error = EBUSY;
		return;
	}
	/*
	 * fake the root, rather than sending NAGET now,
	 * to avoid a deadlock when a server mounts itself
	 * the next stat will correct it
	 */
	rip->i_mode = IFDIR|0555;
	rip->i_un.i_tag = (flag<<16)|ROOTINO;
	rip->i_un.i_cip = cip;
	rip->i_nlink = 2;
	rip->i_size = 32;
	rip->i_uid = rip->i_gid = -1;
	dip->i_mroot = rip;
	rip->i_mpoint = dip;
	prele(rip);
	dip->i_count++;
	cip->i_count++;	
	cip->i_un.i_fsflags = 0;
}

naunmount(mip, fstyp)
register struct inode *mip;
{
	register struct inode *rip;

	rip = mip->i_mroot;
	plock(rip);
	xumount(rip);	/* shared text from remote root */
	if (rip->i_count > 1 || ifsbusy(rip)) {
		u.u_error = EBUSY;
		prele(rip);
		return;
	}
	plock(mip);
	mip->i_mroot = NULL;
	iput(mip);
	if ((mip = rip->i_un.i_cip) == NULL)
		panic("naunmount");
	iput(rip);
	iput(mip);
}

naput(ip)
struct inode *ip;
{	struct senda x;
	struct rcva y;

	if (ip->i_un.i_cip == NULL)
		return;		/* obscure gmount safety */
	if(ip->i_flag & ICHG)
		naupdat(ip, &time, &time, 0);
	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NPUT;
	x.uid = u.u_uid;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	send(ip->i_un.i_cip, &x, &y);
	if(y.errno)
		u.u_error = y.errno;
}

/*
 * special case:
 * the root is faked up locally
 * really just to avoid a silly deadlock in the face server
 */
nafillin(fip, ip)
struct inode *fip;
register struct inode *ip;
{	struct senda x;
	struct rcva y;

	ip->i_un.i_cip = NULL;		/* in case we have to put it */
	if (fip->i_un.i_cip == NULL) {
		u.u_error = ENXIO;
		iput(ip);
		return (-1);
	}
	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NGET;
	x.uid = u.u_uid;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	send(fip->i_un.i_cip, &x, &y);
	if(y.errno == 0) {
		ip->i_mode = y.mode;
		ip->i_un.i_tag = y.tag;
		ip->i_un.i_cip = fip->i_un.i_cip;
		ip->i_nlink = y.nlink;
		ip->i_size = y.size;
		ip->i_uid = y.uid;
		ip->i_gid = y.gid;
		return (0);
	}
	iput(ip);
	u.u_error = y.errno;
	return(-1);
}

/* this is used by CREAT */
naupdat(ip, ta, tm, waitfor)
struct inode *ip;
time_t *ta, *tm;
{	struct senda x;
	struct rcva y;

	if (ip->i_un.i_cip == NULL)
		return;		/* obscure gmount safety */
	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NUPDAT;
	x.uid = u.u_uid;
	x.gid = u.u_gid;
	x.newuid = ip->i_uid;
	x.newgid = ip->i_gid;
	x.mode = ip->i_mode;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	if(ip->i_flag & IACC)
		x.ta = *ta;
	else
		x.ta = 0;
	if(ip->i_flag & ICHG)
		x.tm = *tm;
	else
		x.tm = 0;
	send(ip->i_un.i_cip, &x, &y);
	if(y.errno) {
		u.u_error = y.errno;
		return;
	}
	ip->i_mode = y.mode;
	ip->i_nlink = y.nlink;
	ip->i_size = y.size;
	ip->i_uid = y.uid;
	ip->i_gid = y.gid;
	ip->i_flag &= ~(IUPD|IACC|ICHG);
}

naread(ip)
struct inode *ip;
{	struct senda x;
	struct rcva y;
	struct buf *bp;
	int n;

	bp = geteblk();
	clrbuf(bp);	/* could use user's buffer */
	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NREAD;
	x.uid = u.u_uid;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	x.buf = bp->b_un.b_addr;
	do {
		n = u.u_count;
		if(n > BUFSIZE)
			n = BUFSIZE;
		x.count = n;
		x.offset = Ltol(u.u_offset);
		send(ip->i_un.i_cip, &x, &y);
		if((n = y.count) > 0) {
			if(u.u_segflg != SEGSYS) {
				if(copyout(bp->b_un.b_addr, u.u_base, n)) {
					u.u_error = EFAULT;
					break;
				}
			}
			else
				bcopy(bp->b_un.b_addr, u.u_base, n);
			u.u_base += n;
			u.u_offset = Lladd(u.u_offset, n);
			u.u_count -= n;
		}
		if(y.errno)
			u.u_error = y.errno;
	} while(u.u_error == 0 && u.u_count != 0 && n > 0);
	brelse(bp);
}
nawrite(ip)
struct inode *ip;
{	struct senda x;
	struct rcva y;
	struct buf *bp;
	int n;

	bp = geteblk();
	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NWRT;
	x.uid = u.u_uid;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	x.buf = bp->b_un.b_addr;
	do {
		n = u.u_count;
		if(n > BUFSIZE)		/* should be bufsiz, but ... */
			n = BUFSIZE;
		if(u.u_segflg != SEGSYS) {
			if(copyin(u.u_base, bp->b_un.b_addr, n)) {
				u.u_error = EFAULT;
				break;
			}
		}
		else
			bcopy(u.u_base, bp->b_un.b_addr, n);
		x.count = n;
		x.offset = Ltol(u.u_offset);
		send(ip->i_un.i_cip, &x, &y);
		if(y.errno) {
			u.u_error = y.errno;
			break;
		}
		ip->i_flag |= IUPD|ICHG;
		u.u_count -= n;
		u.u_offset = Lladd(u.u_offset, n);
		u.u_base += n;
	} while(u.u_error == 0 && u.u_count != 0);
	brelse(bp);
}

natrunc(ip)
struct inode *ip;
{	struct senda x;
	struct rcva y;

	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NTRUNC;
	x.uid = u.u_uid;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	send(ip->i_un.i_cip, &x, &y);
	if(y.errno)
		u.u_error = y.errno;
}

nastat(ip, ub)
struct inode *ip;
struct stat *ub;
{	struct senda x;
	struct rcva y;
	struct stat ds;

	x = nilx;
	x.trannum = natrannum++;
	x.cmd = NSTAT;
	x.uid = u.u_uid;
	x.tag = ip->i_un.i_tag;
	x.dev = ip->i_dev;
	x.ino = ip->i_number;
	x.ta = time;
	send(ip->i_un.i_cip, &x, &y);
	if(y.errno) {
		u.u_error = y.errno;
		return;
	}
	ds.st_dev = ip->i_dev;
	ds.st_ino = ip->i_number;
	ip->i_mode = ds.st_mode = y.mode;
	ip->i_nlink = ds.st_nlink = y.nlink;
	ip->i_uid = ds.st_uid = y.uid;
	ip->i_gid = ds.st_gid = y.gid;
	ds.st_size = ip->i_size = y.size;
	ds.st_atime = y.tm[0];
	ds.st_mtime = y.tm[1];
	ds.st_ctime = y.tm[2];
	if(copyout((caddr_t)&ds, (caddr_t)ub, sizeof(ds)) < 0)
		u.u_error = EFAULT;
}

/* a lot like fsnami */
nanami(p, flagp, follow)
struct nx *p;
struct argnamei *flagp;
{	register struct inode *dp, *dip;
	register char *cp;
	int i, fstyp;
	dev_t d;
	struct direct dent;
	struct senda x;
	struct rcva y;

	cp = p->cp;
	dp = p->dp;
	x = nilx;
	x.cmd = NNAMI;
	x.uid = u.u_uid;
	x.gid = u.u_gid;

	/*
	 * dp must be a directory and
	 * must have X permission.
	 * cp is a path name relative to that directory.
	 */

dirloop:
	if((dp->i_mode&IFMT) != IFDIR)
		u.u_error = ENOTDIR;
	(void) access(dp, IEXEC);
	for (i=0; *cp!='\0' && *cp!='/'; i++) {
		if (i >= DIRSIZ) {
			u.u_error = ENOENT;
			break;
		}
		dent.d_name[i] = *cp++;
	}
	if(u.u_error)
		goto outnull;
	while (i < DIRSIZ)
		dent.d_name[i++] = '\0';
	if (dent.d_name[0] == '\0') {		/* null name, e.g. "/" or "" */
		if (flagp->flag != NI_SEARCH) {
			u.u_error = ENOENT;
			goto outnull;
		}
		goto out;
	}
	u.u_segflg = SEGSYS;

	/* send off the request, get back ino, dev, flagp stuff */
	x.trannum = natrannum++;
	x.tag = dp->i_un.i_tag;
	x.dev = dp->i_dev;
	x.ino = dp->i_number;
	x.count = DIRSIZ;
	x.buf = dent.d_name;
	while(*cp == '/')
		cp++;
	if(*cp=='\0') {
		switch(flagp->flag) {
		case NI_SEARCH:
			x.flags = NSEARCH;
			break;
		case NI_DEL:
			x.flags = NDEL;
			break;
		case NI_LINK:
			x.flags = NLINK;
			x.dev = flagp->un.il->i_dev;
			x.ino = flagp->un.il->i_number;
			break;
		case NI_CREAT:
		case NI_NXCREAT:
			x.flags = NCREAT;
			x.mode = flagp->un.mode;
			break;
		case NI_MKDIR:
			x.flags = NMKDIR;
			x.mode = flagp->un.mode;
			break;
		case NI_RMDIR:
			x.flags = NRMDIR;
			break;
		default:
			u.u_error = ENXIO;
			goto outnull;
		}
	}
	send(dp->i_un.i_cip, &x, &y);
	if(y.errno) {
		u.u_error = y.errno;
		goto outnull;
	}
	if(y.flags == NOMATCH)
		goto nomatch;
	dent.d_ino = y.ino;
	if (y.ino == 0) {
		printf("y.ino 0 in nanami\n");
		u.u_error = ENOENT;
		goto outnull;
	}
	d = y.dev;

	if(*cp == 0 && (flagp->flag==NI_DEL || flagp->flag==NI_RMDIR)) {
		/* delete the entry, server did all but xrele */
		dip = iget(dp, d, dent.d_ino);
		if (dip == NULL)
			goto outnull;
		if (dip->i_count == 1) {
			dip->i_un.i_cip = NULL;
			iput(dip);
			goto outnull;
		}
		if(dip->i_flag&ITEXT)
			xrele(dip);		/* free busy text */
		dip->i_nlink--;
		dip->i_flag |= ICHG;
		iput(dip);
		goto outnull;
	}
	/*
	 * Special handling for ".."
	 */
	fstyp = dp->i_fstyp;

	if(y.flags == NROOT) {	/* popped out of net fs */
		if (dp == u.u_rdir)
			dent.d_ino = dp->i_number;
		else if (dp != rootdir && dent.d_ino==ROOTINO
		     &&  dp->i_number == ROOTINO) {
			if ((dip = dp->i_mpoint) == NULL)
				panic("namei: mpoint");
			iput(dp);
			dp = dip;
			plock(dp);
			dp->i_count++;
			/* was there always .. there? */
			if(*cp && cp < p->nbuf + 3)
				panic("nanami");
			if(!*cp && cp < p->nbuf + 2)
				panic("nanami2");
			if(*cp)
				*--cp = '/';
			*--cp = '.';
			*--cp = '.';
			if(fstyp != dp->i_fstyp)
				goto more;
			goto dirloop;
		}
	}
	dip = dp;
	prele(dip);
	dp = iget(dip, d, dent.d_ino);
	if(dp == NULL) {
		iput(dip);
		goto out;
	}
	if (dp->i_count == 1 && nafillin(dip, dp) < 0) {
		iput(dip);
		dp = NULL;
		goto out;
	}
	if(fstyp != dp->i_fstyp) {
		iput(dip);
		goto more;
	}
	/*
	 * Check for symbolic link
	 */
	if ((dp->i_mode&IFMT)==IFLNK && (follow || *cp)) {
		char *ocp;

		ocp = cp;
		while (*cp++)
			;
		if (dp->i_size + (cp-ocp) >= p->nlen-1
		    || ++p->nlink>8) {
			u.u_error = ELOOP;
			iput(dip);
			goto outnull;
		}
		bcopy(ocp, p->nbuf+dp->i_size + 1, cp-ocp);
		*(p->nbuf + dp->i_size) = '/';
		u.u_base = p->nbuf;
		u.u_count = dp->i_size;
		u.u_offset = ltoL(0);
		readi(dp);
		if(u.u_error) {
			iput(dip);
			goto outnull;
		}
		cp = p->nbuf;
		iput(dp);
		if (*cp == '/') {
			iput(dip);
			while (*cp == '/')
				cp++;
			if ((dp = u.u_rdir) == NULL)
				dp = rootdir;
			plock(dp);
			dp->i_count++;
		} else {
			dp = dip;
			plock(dp);
		}
		if(fstyp != dp->i_fstyp)
			goto more;
		goto dirloop;
	}
	iput(dip);
	if(*cp)
		goto dirloop;
	if(flagp->flag == NI_SEARCH && flagp->un.buf)
		bcopy((caddr_t)dent.d_name, flagp->un.buf, flagp->len);
	goto out;

nomatch:
	/*
	 * Search failed.
	 */
	switch(flagp->flag) {	/* probably creating a new file */
	case NI_LINK:	/* make a link */
	case NI_MKDIR:	/* make directory */
		goto outnull;
	case NI_CREAT:	/* create a new file */
	case NI_NXCREAT:
		if(y.errno == EACCES || y.ino == 0) {
			u.u_error = EACCES;
			goto outnull;
		}
		dip = iget(dp, y.dev, y.ino);
		if(dip == NULL)
			goto outnull;
		if (dip->i_count == 1 && nafillin(dp, dip) < 0)
			goto outnull;
		iput(dp);
		dp = dip;
		flagp->un.mode = ~flagp->un.mode;
		goto out;

	default:
		u.u_error = ENOENT;
		goto outnull;
	}

out:
	p->dp = dp;
	return (0);

outnull:
	iput(dp);
	p->dp = NULL;
	return (0);

more:
	p->cp = cp;
	p->dp = dp;
	return (1);
}

struct {
	int tran;
	short proc, dev;
} ntx[32];	/* more debuggery */
send(cip, x, y)
struct inode *cip;
struct senda *x;
struct rcva *y;
{	int n, tn, ix;
	x->version = NETVERSION;
	/* until demux works, use key as a lock */
	while(cip->i_un.i_fsflags)
		sleep((caddr_t)cip, PZERO);
	cip->i_un.i_fsflags = 1;
	tn = x->trannum;
	netabuf.s = 2;
	for(ix = 0; ntx[ix].tran && ix < 32; ix++) {
		ntx[ix].tran = tn;
		ntx[ix].proc = u.u_procp->p_pid;
		ntx[ix].dev = x->dev;
	}
	n = istwrite(cip, (char *)x, sizeof(*x));
	netabuf.s = 3;
	if(n == -1) {
		y->errno = EIO;
		goto bad;
	}
	netabuf.s = 4;
	if(x->count > 0 && x->buf && x->cmd != NREAD) {
		n = istwrite(cip, x->buf, x->count);
		if(n == -1)
			goto bad;
	}
readagain:
	netabuf.s = 5;
	n = istread(cip, (char *)y, sizeof(*y), 0);
	netabuf.s = 6;
	if(n != sizeof(*y))
		goto bad;
	if(y->errno == 0 && x->cmd == NREAD) {
		n = istread(cip, x->buf, y->count, 0);
		if(n != y->count) {
			printf("neta: read %d expected %d\n", n, y->count);
			/* shut it down */
			istwrite(cip, (char *)x, 0);
			goto bad;
		}
	}
	if(y->errno == 0) {
		if(y->trannum != tn) {
			if(y->trannum < tn)	/* distant past */
				goto readagain;
			printf("neta: sent %d got %d\n", tn, y->trannum);
			goto bad;
		}
		addx(x);
		addy(y);
		if(ix < 32)
			ntx[ix].tran = 0;
		cip->i_un.i_fsflags = 0;
		wakeup((caddr_t)cip);
		return;
	}
bad:
	cip->i_un.i_fsflags = 0;
	if(ix < 32)
		ntx[ix].tran = 0;
	for(ix = 0; ix < 32; ix++)
		if(ntx[ix].tran)
			printf("tran %d proc %d 0x%x\n", ntx[ix].tran, ntx[ix].proc, ntx[ix].dev);
	wakeup((caddr_t)cip);
	addx(x);
	addy(y);
	if(y->errno)
		return;
	y->errno = EIO;
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n85/6/9:neta.c\n"};
