#include "sys/param.h"
#include "sys/user.h"
#include "sys/netb.h"
#include "sys/inode.h"
#include "sys/buf.h"
#include "sys/systm.h"
#include "sys/stat.h"
#include "sys/file.h"
#include "sys/conf.h"
#include "sys/nbio.h"

static long nbtrannum;
static int nbfstyp;
int nbdebug;
int nbput(), nbupdat(), nbread(), nbwrite(), nbtrunc(), nbstat();
int nbnami(), nbioctl(), nbmount(), nbdirread();
struct fstypsw nbfs =
fsinit(nbput, nbupdat, nbread, nbwrite, nbtrunc, nbstat,
	nbnami, nbmount, nbioctl, nullopen, nbdirread);
unsigned char *getnbbuf();
#define	NBMAXMSG	(5*1024)	/* probably enough */
int nbbsz = NBMAXMSG;

/*
 * flags in i_un.i_fsflags
 * valid only in the root inode
 */
#define	NBUSY	01		/* already a message outstanding */
#define	NWANT	02		/* someone waiting for BUSY */
#define	NDEAD	04		/* communication line assumed dead */

/*
 * flags valid in any netb inode
 */
#define	NSTOK	010		/* inode data is fresh; skip next stat message */

nbput(ip)
register struct inode *ip;
{
	unsigned char sbuf[SNBSIZE];
	unsigned char rbuf[RNBSIZE];
	register unsigned char *sp;


	/* don't put root, as optimization.  pointless? */
	if (ip->i_mpoint->i_mroot == ip)
		return;
	if(ip->i_flag & ICHG)	/* this is an extra message, but not often? */
		nbupdat(ip, &time, &time, 0);
	sp = sbuf;
	bzero(sp, sizeof(sbuf));
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBPUT);
	tonetlong(sp, SNB_LEN, sizeof(sbuf));
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	nbsend(ip, sp, rbuf, sizeof(rbuf));
}

nbtrunc(ip)
register struct inode *ip;
{
	unsigned char sbuf[SNBSIZE];
	unsigned char rbuf[RNBSIZE];
	register unsigned char *sp;

	ip->i_un.i_fsflags &=~ NSTOK;
	sp = sbuf;
	bzero(sp, sizeof(sbuf));
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBTRNC);
	tonetlong(sp, SNB_LEN, sizeof(sbuf));
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	nbsend(ip, sp, rbuf, sizeof(rbuf));
}

/*
 * the only times we're interested in are those which have waitfor == 0
 * (or those which have ta or tb != &time)
 */
nbupdat(ip, ta, tb, waitfor)
register time_t *ta, *tb;
register struct inode *ip;
{
	unsigned char sbuf[SUPSIZE];
	unsigned char rbuf[RNBSIZE];
	register unsigned char *sp;

	ip->i_un.i_fsflags &=~ NSTOK;
	sp = sbuf;
	bzero(sp, sizeof(sbuf));
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBUPD);
	tonetlong(sp, SNB_LEN, sizeof(sbuf));
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	tonetshort(sp, SUP_MODE, ip->i_mode);
	/*
	 * in theory *ta or *tb could be incremented
	 * while we are copying them char-by-char
	 * in practice, it doesn't matter,
	 * as *t? != &time
	 */
	if((ta != &time) && (ip->i_flag & IACC))
		tonetlong(sp, SUP_ATIME, *ta);
	if((tb != &time) && (ip->i_flag & IUPD))
		tonetlong(sp, SUP_MTIME, *tb);
	nbsend(ip, sp, rbuf, sizeof(rbuf));
	ip->i_flag &= ~(IUPD|IACC|ICHG);
}

/*
 * if NSTOK is set,
 * the inode is full of stat data from a recent namei
 * for stat, NSTOK always set, because we just called namei
 * (except for the root, for which namei isn't called)
 * for fstat, it is probably not set
 */
nbstat(ip, ub)
register struct inode *ip;
struct stat *ub;
{
	unsigned char sbuf[SSTSIZE];
	unsigned char rbuf[RSTSIZE];
	struct stat ds;
	register unsigned char *p;
	long now;

	if ((ip->i_un.i_fsflags & NSTOK) == 0) {
		p = sbuf;
		bzero(p, sizeof(sbuf));
		tonetchar(p, SNB_VERSION, NETB);
		tonetchar(p, SNB_CMD, NBSTAT);
		tonetlong(p, SNB_LEN, sizeof(sbuf));
		tonetlong(p, SNB_TAG, ip->i_un.i_tag);
		now = time;		/* in case clock ticks */
		tonetlong(p, SST_TIME, now);
		nbsend(ip, p, rbuf, sizeof(rbuf));
		if(u.u_error)
			return;
		p = rbuf;
		ip->i_mode = frnetshort(p, RST_MODE);
		ip->i_nlink = frnetshort(p, RST_NLINK);
		ip->i_uid = frnetshort(p, RST_UID);
		ip->i_gid = frnetshort(p, RST_GID);
		ip->i_size = frnetlong(p, RST_SIZE);
		ip->i_un.i_netatime = frnetlong(p, RST_ATIME);
		ip->i_un.i_netmtime = frnetlong(p, RST_MTIME);
		ip->i_un.i_netctime = frnetlong(p, RST_CTIME);
		ip->i_un.i_netrdev = frnetlong(p, RST_RDEV);
	}
	ds.st_dev = ip->i_dev;
	ds.st_ino = ip->i_number;
	ds.st_mode = ip->i_mode;
	ds.st_nlink = ip->i_nlink;
	ds.st_uid = ip->i_uid;
	ds.st_gid = ip->i_gid;
	ds.st_size = ip->i_size;
	ds.st_atime = ip->i_un.i_netatime;
	ds.st_mtime = ip->i_un.i_netmtime;
	ds.st_ctime = ip->i_un.i_netctime;
	ds.st_rdev = ip->i_un.i_netrdev;
	if(copyout((caddr_t)&ds, (caddr_t)ub, sizeof(ds)))
		u.u_error = EFAULT;
	ip->i_un.i_fsflags &=~ NSTOK;	/* so repeated fstats check for changes */
}

/*
 * here and in nbwrite, `tonetlong(..., Ltol(u.u_offset))' is OK.
 * it looks like four function calls, one per byte,
 * but Ltol is a macro without side effects.
 * maybe there should just be a temporary for clarity
 */
nbread(ip)
register struct inode *ip;
{
	unsigned char sbuf[SRDSIZE];
	register unsigned char *rp;
	register unsigned char *sp;
	register int n;

	ip->i_un.i_fsflags &=~ NSTOK;
	rp = getnbbuf();
	sp = sbuf;
	bzero(sp, sizeof(sbuf));
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBREAD);
	tonetlong(sp, SNB_LEN, sizeof(sbuf));
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	do {
		n = u.u_count;
		if(n > nbbsz - RNBSIZE)
			n = nbbsz - RNBSIZE;
		tonetlong(sp, SRD_LEN, n);
		tonetlong(sp, SRD_OFFSET, Ltol(u.u_offset));
		nbsend(ip, sp, rp, nbbsz);
		if (u.u_error)
			break;
		n = frnetlong(rp, RNB_LEN) - RNBSIZE;
		if(n > 0 && n <= nbbsz)	/* give it to user */
			iomove(rp + RNBSIZE, n, B_READ);
	} while (u.u_error == 0 && u.u_count > 0 && n > 0
	  &&  frnetchar(rp, RNB_FLAGS) != NBEND);
	putnbbuf(rp);
}

nbdirread(ip, len)
register struct inode *ip;
{
	unsigned char sbuf[SRDSIZE];
	register unsigned char *sp;
	register unsigned char *rp;	/* RDISIZE */
	int n;
	llong_t oldoff;
	long netoff;	/* just the 32-bit part */

	ip->i_un.i_fsflags &=~ NSTOK;
	rp = getnbbuf();
	sp = sbuf;
	bzero(sp, sizeof(sbuf));
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBDIR);
	tonetlong(sp, SNB_LEN, sizeof(sbuf));
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	if(len > nbbsz - RDISIZE)
		len = nbbsz - RDISIZE;
	tonetlong(sp, SRD_LEN, len);
	oldoff = u.u_offset;
	netoff = Ltol(u.u_offset);
	tonetlong(sp, SRD_OFFSET, netoff);
	nbsend(ip, sp, rp, nbbsz);
	if(u.u_error)
		goto out;
	n = frnetlong(rp, RNB_LEN) - RDISIZE;
	if (n > 0 && n <= nbbsz)
		iomove(rp + RDISIZE, n, B_READ);
	if (u.u_error)
		goto out;
	u.u_offset = Lladd(oldoff, frnetlong(rp, RDI_USED));
	u.u_r.r_val1 = n;
out:
	putnbbuf(rp);
}

nbwrite(ip)
register struct inode *ip;
{
	register unsigned char *sp;
	unsigned char rbuf[RNBSIZE];
	int n;

	ip->i_un.i_fsflags &=~ NSTOK;
	sp = getnbbuf();
	bzero(sp, SWRSIZE);
	tonetchar(sp, SNB_VERSION, NETB);
	tonetchar(sp, SNB_CMD, NBWRT);
	tonetlong(sp, SNB_TAG, ip->i_un.i_tag);
	do {
		n = u.u_count;
		if(n > nbbsz - SWRSIZE)
			n = nbbsz - SWRSIZE;
		tonetlong(sp, SNB_LEN, n + SWRSIZE);
		tonetlong(sp, SWR_LEN, n);
		tonetlong(sp, SWR_OFFSET, Ltol(u.u_offset));
		iomove(sp + SWRSIZE, n, B_WRITE);
		if (u.u_error)
			break;
		nbsend(ip, sp, rbuf, sizeof(rbuf));
		if (u.u_error)
			break;
		ip->i_flag |= IUPD|ICHG;
	} while(u.u_count > 0);
	if(u.u_error == 0)
		ip->i_size = frnetlong(rbuf, RNB_FSIZE);
	putnbbuf(sp);
}

nbioctl(ip, cmd, addr, flag)	/* fp->f_flag */
register struct inode *ip;
caddr_t addr;
{
	register unsigned char *p;
	int size;

	switch(cmd) {
	case NBIOCON:
		nbdebug = 1;
		return;
	case NBIOCOFF:
		nbdebug = 0;
		return;
	case NBIOCBSZ:
		if (copyin(addr, (caddr_t)&size, sizeof(size))) {
			u.u_error = EFAULT;
			return;
		}
		if (size > 1024 && size <= NBMAXMSG)
			nbbsz = size & ~1023;	/* round to even 1K */
		if (copyout(addr, (caddr_t)&nbbsz, sizeof(nbbsz)))
			u.u_error = EFAULT;
		return;
	}
	/*
	 * one buffer, used first for sbuf, then for rbuf
	 */
	p = getnbbuf();
	bzero(p, SIOSIZE);
	tonetchar(p, SNB_VERSION, NETB);
	tonetchar(p, SNB_CMD, NBIOCTL);
	tonetlong(p, SNB_LEN, SIOSIZE + SIODATA);
	tonetlong(p, SNB_TAG, ip->i_un.i_tag);
	tonetlong(p, SIO_CMD, cmd);
	tonetshort(p, SIO_FLAG, flag);
	if(copyin(addr, p + SIOSIZE, SIODATA)) {
		u.u_error = EFAULT;
		goto out;
	}
	nbsend(ip, p, p, RNBSIZE + SIODATA);
	if (u.u_error)
		goto out;
	if(frnetlong(p, RNB_LEN) != RNBSIZE + SIODATA) {
		printf("netb: nbioc got %d, not %d\n",
			frnetlong(p, RNB_LEN), RNBSIZE+SIODATA);
		u.u_error = EIO;
		goto out;
	}
	if(copyout(p+RNBSIZE, addr, SIODATA))
		u.u_error = EFAULT;
out:
	putnbbuf(p);
}

/*
 * namei:
 * send the whole pathname
 * wrong if another filesystem is mounted
 * atop some file in this one
 */
nbnami(p, flagp, follow)
register struct nx *p;
register struct argnamei *flagp;
{
	register char *cp;
	register struct inode *dp;
	register unsigned char *pp;
	register int nlen;
	int used;
	dev_t dev;

	dp = p->dp;
	/*
	 * special case:
	 * if simple lookup of "" in the root,
	 * and the connection is dead,
	 * return OK even though dead so it's possible to unmount
	 */
	if (dp->i_mpoint->i_mroot == dp
	&&  dp->i_un.i_fsflags & NDEAD
	&&  flagp->flag == NI_SEARCH && *p->cp == 0)
		return (0);	/* with the same dp and cp */
	/*
	 * one buffer for rbuf and sbuf
	 */
	pp = getnbbuf();
	bzero(pp, SNMSIZE);
	tonetchar(pp, SNB_VERSION, NETB);
	tonetchar(pp, SNB_CMD, NBNAMI);
	tonetlong(pp, SNB_TAG, dp->i_un.i_tag);
	cp = p->cp;
	while (*cp++)
		;
	nlen = cp - p->cp;	/* length of name, including NUL */
	if (nlen + SNMSIZE >= nbbsz) {
		u.u_error = ENOENT;
		goto outnull;
	}
	used = nlen + SNMSIZE - 1;	/* temporary to improve code */
	tonetlong(pp, SNB_LEN, used);
	bcopy(p->cp, pp + SNMSIZE, nlen);
	tonetchar(pp, SNB_FLAGS, flagp->flag);
	switch (flagp->flag) {

	case NI_CREAT:
	case NI_NXCREAT:
	case NI_MKDIR:
		tonetshort(pp, SNM_MODE, flagp->un.mode);
		break;

	case NI_LINK:
		tonetlong(pp, SNM_INO, flagp->un.il->i_un.i_tag);
		break;
	}
	nbsend(dp, pp, pp, RNMSIZE);
	if(u.u_error)
		goto outnull;
	if (frnetchar(pp, RNB_FLAGS) == NBROOT) {
		/* find my root */
		dp = dp->i_mpoint;
		iput(p->dp);
		if(dp == NULL) {
			u.u_error = EGREG;
			goto out;
		}
		plock(dp);
		dp->i_count++;
		used = frnetlong(pp, RNM_USED);
		if (used < 2 || used >= nlen) {
			u.u_error = EGREG;
			goto outnull;
		}
		cp = p->cp + used;
		*--cp == '.';
		*--cp == '.';
		p->cp = cp;
		goto outmore;
	}
	switch(flagp->flag) {

	default:
		panic("nbnami");

	case NI_CREAT:	/* only case where flagp is used later (creat()) */
	case NI_NXCREAT:	/* return an inode, like search */
		flagp->un.mode = ~flagp->un.mode;	/* obscure side-effects! */
	case NI_SEARCH:
		break;

	case NI_DEL:	/* nothing returned */
	case NI_LINK:	/* nothing returned */
	case NI_MKDIR:	/* nothing returned */
	case NI_RMDIR:	/* nothing returned */
		goto outnull;
	}
	/*
	 * release directory inode, construct a new one
	 * fill in the attributes even if inode was already in core;
	 * the new data is more correct (server gets priority)
	 */
	prele(dp);	/* in case new == old */
	/* cheat: first character of RNM_DEV == minor */
	dev = makedev(major(dp->i_dev), frnetchar(pp, RNM_DEV));
	dp = iget(dp, dev, frnetlong(pp, RNM_INO));
	/* p->dp == dp before iget was called */
	if (dp == NULL) {
		idec(p->dp);
		goto out;
	}
	if (p->dp->i_mpoint != dp->i_mpoint) {	/* something mounted here */
		idec(p->dp);
		p->cp += nlen - 1;	/* point to NUL at the end */
		goto outmore;
	}
	dp->i_un.i_cip = p->dp->i_un.i_cip;
	idec(p->dp);
	nbunpack(dp, pp);
	/*
	 * no filename to save for accounting; oh well
	 */
	if(flagp->flag == NI_SEARCH && flagp->un.buf)
		bcopy("netb exec", flagp->un.buf, flagp->len);

	/*
	 * return the inode in dp
	 */
out:
	p->dp = dp;
	putnbbuf(pp);
	return (0);

	/*
	 * discard dp, return NULL
	 */
outnull:
	iput(dp);
	p->dp = NULL;
	putnbbuf(pp);
	return (0);

	/*
	 * crossed to a different filesystem;
	 * return cp and dp
	 */
outmore:
	p->dp = dp;
	putnbbuf(pp);
	return (1);
}

nbmount(cip, dip, flag, mnt, fstyp)
struct inode *cip, *dip;
{

	nbfstyp = fstyp;
	if (mnt)
		nbon(cip, dip, flag, fstyp);
	else
		nboff(dip);
}

/*
 * data structures for keeping track of device numbers in use
 */
#define	NDEVS	256
static char nbdevs[NDEVS];

#define	D_SET(dev)	nbdevs[dev] = 1
#define	D_CLR(dev)	nbdevs[dev] = 0
#define	D_ISSET(dev)	nbdevs[dev]

/*
 * mount the filesystem
 * fake up a namei message to fetch the root
 * don't call nbnami because it's too hard to get
 * all the first-time error checks right
 */

nbon(cip, dip, flag, fstyp)
struct inode *cip;
struct inode *dip;
{
	register struct inode *rip;
	struct inode pi;
	register unsigned char *pp;
	unsigned char sbuf[SNMSIZE+1];
	unsigned char rbuf[RNMSIZE];
	register int dmaj;

	if(cip->i_sptr == NULL) {	/* shouting into the storm */
		u.u_error = ENXIO;
		return;
	}
	for (dmaj = 0; dmaj < NDEVS; dmaj++)
		if (D_ISSET(dmaj) == 0)
			break;
	if (dmaj >= NDEVS) {
		u.u_error = EMFILE;
		return;
	}
	D_SET(dmaj);
	/*
	 * fake up an inode to represent the connection
	 * to nbsend and iget
	 */
	pi.i_fstyp = fstyp;
	pi.i_mpoint = &pi;
	pi.i_mroot = &pi;	/* pi.i_mpoint->i_mroot == &pi */
	pi.i_un.i_cip = cip;
	pi.i_un.i_fsflags = 0;
	pp = sbuf;
	bzero(pp, sizeof(sbuf));
	tonetchar(pp, SNB_VERSION, NETB);
	tonetchar(pp, SNB_CMD, NBNAMI);
	tonetlong(pp, SNB_TAG, NBROOTTAG);	/* lookup in the root */
	tonetlong(pp, SNB_LEN, SNMSIZE+1);	/* one char of filename */
	/* tonetchar(pp, SNMSIZE, 0);		/* "" */
	tonetchar(pp, SNB_FLAGS, NI_SEARCH);
	nbsend(&pi, pp, rbuf, sizeof(rbuf));
	if (u.u_error) {
		D_CLR(dmaj);
		return;
	}
	pp = rbuf;
	if (frnetchar(pp, RNB_FLAGS) != 0) {
		D_CLR(dmaj);
		u.u_error = EINVAL;
		return;
	}
	if ((rip = iget(&pi, makedev(dmaj, frnetchar(pp, RNM_DEV)), frnetlong(pp, RNM_INO))) == NULL) {
		D_CLR(dmaj);
		return;
	}
	if (rip->i_count != 1) {
		printf("nbon busy, dev %d\n", dmaj);	/* shouldn't happen */
		iput(rip);
		D_CLR(dmaj);
		u.u_error = EBUSY;
		return;
	}
	nbunpack(rip, pp);
	rip->i_un.i_cip = cip;
	dip->i_mroot = rip;
	rip->i_mpoint = dip;
	prele(rip);
	dip->i_count++;
	cip->i_count++;
}

/*
 * unpack inode info from namei response
 * a pity stat can't use this too,
 * but the stat response isn't quite the same format
 */
nbunpack(dp, pp)
register struct inode *dp;
register unsigned char *pp;
{

	dp->i_un.i_tag = frnetlong(pp, RNM_TAG);
	dp->i_mode = frnetshort(pp, RNM_MODE);
	dp->i_nlink = frnetshort(pp, RNM_NLINK);
	dp->i_uid = frnetshort(pp, RNM_UID);
	dp->i_gid = frnetshort(pp, RNM_GID);
	dp->i_size = frnetlong(pp, RNM_SIZE);
	dp->i_un.i_netatime = frnetlong(pp, RNM_ATIME);
	dp->i_un.i_netmtime = frnetlong(pp, RNM_MTIME);
	dp->i_un.i_netctime = frnetlong(pp, RNM_CTIME);
	dp->i_un.i_netrdev = frnetlong(pp, RNM_RDEV);
	if (dp->i_count == 1)
		dp->i_un.i_fsflags = 0;
	dp->i_un.i_fsflags |= NSTOK;
}

/*
 * unmount a netb--
 * mip is the mount point
 * locking needs some thought
 */
nboff(mip)
register struct inode *mip;
{
	register struct inode *rip;

	rip = mip->i_mroot;
	plock(rip);
	if (rip->i_mpoint != mip) {	/* already unmounted */
		prele(rip);
		return;
	}
	xumount(rip);
	iupdat(rip, &time, &time, 1);
	ifsinacc(rip);	/* turn all, including rip, to errfs */
	plock(mip);		
	mip->i_mroot = NULL;
	iput(mip);
	if (rip->i_un.i_cip == NULL)
		panic("nboff");
	iput(rip->i_un.i_cip);
	D_CLR(major(rip->i_dev));
	iput(rip);	/* errput, not nbput */
}

nbsend(ip, sp, rp, size)
struct inode *ip;
register unsigned char *sp;
unsigned char *rp;
{
	int bad;
	long tn;
	struct inode *cip;
	register struct inode *rip;

	cip = ip->i_un.i_cip;
	rip = ip->i_mpoint->i_mroot;
	if (rip == NULL || rip->i_fstyp != ip->i_fstyp) {
		u.u_error = EIO;
		return;
	}
	while ((rip->i_un.i_fsflags & (NBUSY|NDEAD)) == NBUSY
	&&  ip->i_fstyp == nbfstyp) {
		rip->i_un.i_fsflags |= NWANT;
		if (tsleep((caddr_t)cip, PZERO+1, 0) != TS_OK) {
			u.u_error = EIO;
			return;
		}
	}
	if (ip->i_fstyp != nbfstyp	/* unmounted under us */
	||  rip->i_un.i_fsflags & NDEAD) {	/* or connection fell down */
		u.u_error = EIO;
		return;
	}
	rip->i_un.i_fsflags |= NBUSY;
	tonetshort(sp, SNB_UID, u.u_uid);
	tonetshort(sp, SNB_GID, u.u_gid);
	tn = nbtrannum++;
	tonetlong(sp, SNB_TRANNUM, tn);
	if (istwrite(cip, sp, frnetlong(sp, SNB_LEN)) >= 0
	&&  nbrecv(cip, rp, size, tn) >= 0)
		bad = 0;
	else
		bad = 1;
	if (ip->i_fstyp == nbfstyp) {	/* in case unmounted while asleep */
		rip->i_un.i_fsflags &=~ NBUSY;
		if (rip->i_un.i_fsflags & NWANT) {
			wakeup((caddr_t)cip);
			rip->i_un.i_fsflags &=~ NWANT;
		}
		if (bad)
			rip->i_un.i_fsflags |= NDEAD;
	}
	if (bad)
		u.u_error = EIO;
	else if ((bad = frnetshort(rp, RNB_ERRNO)) != 0)
		u.u_error = bad;
}

/*
 * read an answer,
 * and make sure it smells like one of ours
 */

int
nbrecv(cip, rp, size, tn)
struct inode *cip;
register unsigned char *rp;
int size;
long tn;
{
	unsigned char *p;
	register int n, len, netlen;

loop:
	p = rp;
	len = 0;
	/*
	 * header
	 */
	while (len < RNBSIZE) {
		n = istread(cip, p, size - len, 0);
		if (n <= 0)
			return (-1);
		len += n;
		p += n;
	}
	netlen = frnetlong(rp, RNB_LEN);
	if (netlen > size || netlen < RNBSIZE) {
		printf("netb: rcv len %d\n", netlen);
		return (-1);
	}
	/*
	 * following data, if any
	 */
	while (len < netlen) {
		n = istread(cip, p, size - len, 0);
		if (n <= 0)
			return (-1);
		len += n;
		p += n;
	}
	if (frnetlong(rp, RNB_TRANNUM) != tn) {
		if (frnetlong(rp, RNB_TRANNUM) < tn)	/* echo from the past */
			goto loop;
		printf("netb: exp tran %d, got %d\n", tn, frnetlong(rp, RNB_TRANNUM));
		return (-1);
	}
	return (len);
}

/* oh no, it's got its own tiny buffer pool: want to avoid breakage on
	normal (4k reads for now).  we could replace this with a real
	memory manager in the kernel */
#define	NBUF	7	/* who knows? */
char nb_lock;
char nbflgs[NBUF];
unsigned char nbbuf[NBUF][NBMAXMSG];

unsigned char *
getnbbuf()
{
	register int i;

loop:
	for(i = 0; i < NBUF; i++)
		if(!nbflgs[i]) {
			nbflgs[i] = 1;
			return(nbbuf[i]);
		}
	nb_lock = 1;
	sleep((caddr_t)&nb_lock, PZERO);
	goto loop;
}

putnbbuf(s)
unsigned char *s;
{
	register int i;

	for(i = 0; i < NBUF; i++)
		if(nbbuf[i] == s) {
			nbflgs[i] = 0;
			if(nb_lock) {
				nb_lock = 0;
				wakeup((caddr_t) &nb_lock);
			}
			return;
		}
	panic("putnbbuf");
}
