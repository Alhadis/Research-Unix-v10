#include "sys/param.h"
#include "sys/systm.h"
#include "sys/inode.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/psl.h"
#include "sys/stat.h"
#include "sys/buf.h"
#include "sys/pte.h"
#include "sys/cmap.h"
#include "sys/mtpr.h"
#include "sys/vmparam.h"
#include "sys/vmmac.h"
#include "sys/proc.h"
#include "sys/text.h"
#include "sys/file.h"
#include "sys/reg.h"
#include "sys/pioctl.h"
#include "sys/conf.h"

/* I/O via /proc returns EBUSY if any of these bits are set in p_flag: */

#define	SPRBUSY	(SLOCK|SPAGE|SKEEP|SWEXIT|SPROCIO)

#define SYSADR	((caddr_t)0x80000000)	/* virtual address  of system seg. */
#define SYSP	btop(0x80000000)	/* virtual page no. of system seg. */

/* <text page no.> + P1OFF(p) == <stack page no.> of same location */

#define P1OFF(p)	(SYSP - (p)->p_szpt*NPTEPG)

#define TEXT		0
#define DATA		1
#define STACK		2
#define USERAREA	3

#define min(a,b)	((a) <= (b) ? (a) : (b))

/* inumber to pid */
#define PRMAGIC 64

#define PROCDEV (dev_t)0
#define	PRROOT	2

struct proc *prpidlock, *prpidwant, *pritop();

struct pte *Prbufmap; char *priobuf;
struct pte *Prusrmap; struct user *prusrutl;
struct pte *prclmap();

int prput(), prread(), prwrite(), prstat();
int prmount(), prioctl(), prnami();

struct fstypsw prfs =
fsinit(prput, nulldev, prread, prwrite, nulldev, prstat,
	prnami, prmount, prioctl, nullopen, nodev);

prput(ip)
struct inode *ip;
{	struct proc *p;
	if(ip->i_number == PRROOT)
		return;
	if ((p = pritop(ip)) == NULL)
		return;
	p->p_trace = 0;
}

#define SDSIZ	sizeof(struct direct)

prread(ip)
struct inode *ip;
{
	static struct direct dotbuf[] = {
		{ PRROOT, "."},
		{ PRROOT, ".."}
	};
	struct direct dirbuf;
	register int i, n, j;
	int minproc, maxproc, modoff;
	struct proc *p;

	if (ip->i_number == PRROOT) {	/* fake up . .. and the proc inodes */
		if (Lsign(u.u_offset) < 0 || Ltol(u.u_offset) >= ip->i_size ||
		    u.u_count <= 0)
			return;
		if (Ltol(u.u_offset) < 2*SDSIZ) {
			iomove((caddr_t)dotbuf + Ltol(u.u_offset),
			    min(u.u_count, 2*SDSIZ - Ltol(u.u_offset)), B_READ);
			if (u.u_count <= 0 || u.u_error)
				return;
		}
		minproc = (Ltol(u.u_offset) - 2*SDSIZ)/SDSIZ;
		maxproc = (Ltol(u.u_offset) + u.u_count - 1)/SDSIZ;
		modoff = Ltol(u.u_offset) % SDSIZ;
		for (j = 0; j < DIRSIZ; j++)
			dirbuf.d_name[j] = 0;
		for (i=minproc; i<min(maxproc,proccnt); i++) {
			if (proc[i].p_stat == 0)
				dirbuf.d_ino = 0;
			else {
				n = proc[i].p_pid;
				dirbuf.d_ino = n + PRMAGIC;
				for (j = 4; j >= 0; j--)
					dirbuf.d_name[j] = n%10 + '0', n /= 10;
			}
			iomove((caddr_t)&dirbuf + modoff,
			    min(u.u_count, SDSIZ - modoff), B_READ);
			if (u.u_count <= 0 || u.u_error)
				return;
			modoff = 0;
		}
	} else if ((p = pritop(ip)) != NULL) {
		if (prlock(p))
			return;
		prusrio(p, B_READ);
		prunlock(p);
	}
}

prwrite(ip)
struct inode *ip;
{
	register struct proc *p;
	if (ip->i_number == PRROOT) {
		u.u_error = EISDIR;
	} else if ((p = pritop(ip)) != NULL) {
		if (prlock(p))
			return;
		prusrio(p, B_WRITE);
		prunlock(p);
	}
}

prstat(ip, ub)
struct inode *ip;
struct stat *ub;
{	struct stat ds; struct proc *p = (struct proc *)0;
	if(ip->i_number == PRROOT || (p = pritop(ip)) != NULL) {
		ds.st_dev = -1;		/* who knows */
		ds.st_ino = ip->i_number;
		ds.st_mode = ip->i_mode;
		ds.st_nlink = ip->i_nlink;
		ds.st_uid = ip->i_uid;
		ds.st_gid = ip->i_gid;
		ds.st_rdev = -1;
		if (p) ip->i_size =
			(int)ptob(p->p_tsize+p->p_dsize+p->p_ssize+UPAGES);
		ds.st_size = ip->i_size;
		ds.st_atime = ds.st_mtime = ds.st_ctime = time;
		if(copyout((caddr_t)&ds, (caddr_t)ub, sizeof(ds)) < 0)
			u.u_error = EFAULT;
	}
}

prnami(p, flagp, follow)
register struct nx *p;
struct argnamei *flagp;
{
	register struct inode *dp;
	register char *cp;
	register int n;

	cp = p->cp;
	dp = p->dp;
	if ((dp->i_mode & IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		goto error;
	}
	if (access(dp, IEXEC))
		goto error;
	if (flagp->flag != NI_SEARCH) {
		u.u_error = EACCES;
		goto error;
	}
rootloop:
	if (cp[0] == 0)			/* "" == root */
		return (0);
	if (cp[0] == '.') {
		if (cp[1] == 0)		/* "." */
			return (0);
		if (cp[1] == '/') {	/* "./" ... */
			cp += 2;
			p->cp = cp;
			goto rootloop;
		}
		if (cp[1] == '.' && (cp[2] == 0 || cp[2] == '/')) {	/* ".." */
			p->dp = dp->i_mpoint;
			iput(dp);
			plock(p->dp);
			p->dp->i_count++;
			return (1);	/* cp still points to ".." */
		}
		u.u_error = ENOENT;	/* ".somethingelse" */
		goto error;
	}
	n = 0;
	while (*cp >= '0' && *cp <= '9') {
		n *= 10;
		n += *cp++ - '0';
	}
	if (*cp) {		/* non-digit, or /proc/123/... */
		u.u_error = ENOENT;
		goto error;
	}
	p->dp = iget(dp, PROCDEV, n + PRMAGIC);
	if (p->dp == NULL)
		goto error;
	if (p->dp->i_count == 1 && prfillin(p->dp))
		goto error;
	iput(dp);
	return (0);

error:
	p->dp = NULL;
	iput(dp);
	return (0);
}

prfillin(ip)
register struct inode *ip;
{
	register struct proc *p;
	register int n;

	ip->i_un.i_proc = NULL;		/* safety */
	n = ip->i_number - PRMAGIC;
	for (p = proc; p < procNPROC; p++)
		if (p->p_stat && p->p_pid == n)
			break;
	if (p == procNPROC || p->p_stat == SZOMB || p->p_trace) {
		iput(ip);
		u.u_error = ENOENT;
		return (1);
	}
	p->p_trace = ip;
	if (p->p_textp && p->p_textp->x_iptr && access(p->p_textp->x_iptr, IREAD))
		ip->i_mode = IFREG;		/* regular, no permissions */
	else
		ip->i_mode = IFREG | 0600;	/* regular, r/w only by owner */
	ip->i_nlink = 1;
	ip->i_uid = p->p_uid;
	ip->i_gid = 1;			/* who cares */
	ip->i_size = (int)ptob(p->p_tsize+p->p_dsize+p->p_ssize+UPAGES);
	ip->i_un.i_proc = p;
	ip->i_un.i_sigmask = 0;		/* signal trace mask */
	return (0);
}

prmount(sip, ip, flag, mnt, fstyp)
struct inode *sip, *ip;
{
	if (!suser())
		return;
	if (mnt)
		pron(sip, ip, flag, fstyp);
	else
		proff(ip, fstyp);
}

pron(sip, ip, flag, fstyp)
struct inode *sip;
register struct inode *ip;
{
	register struct inode *rip;
	struct inode pi;

	if (ip->i_fstyp == fstyp) {
		u.u_error = EBUSY;
		return;
	}
	pi.i_fstyp = fstyp;
	pi.i_mpoint = NULL;
	if ((rip = iget(&pi, PROCDEV, (ino_t)PRROOT)) == NULL)
		return;
	if (rip->i_count != 1) {	/* already mounted */
		iput(rip);
		u.u_error = EBUSY;
		return;
	}
	rip->i_mpoint = ip;
	rip->i_mode = IFDIR|0555;
	rip->i_nlink = 2;
	rip->i_uid = rip->i_gid = 0;
	rip->i_size = (proccnt+2) * sizeof(struct direct);
	prele(rip);
	ip->i_mroot = rip;
	ip->i_count++;
}

proff(mip, fstyp)
struct inode *mip;
{
	struct inode *rip;

	plock(mip);
	rip = mip->i_mroot;
	mip->i_mroot = NULL;
	iput(mip);
	plock(rip);
	iput(rip);
}

/* special tracing stuff */
prioctl(ip, cmd, cmaddr)
struct inode *ip;
caddr_t cmaddr;
{
	register struct proc *p; struct text *xp; struct inode *ixp;
	int n;		/* mostly for integer arguments */

	if ((p = pritop(ip)) == NULL)
		return;
	switch (cmd) {
	default:
		u.u_error = ENOTTY;
		return;

	case PIOCGETPR:		/* read struct proc */
		if (copyout((char *)p, cmaddr, sizeof(struct proc)))
			u.u_error = EFAULT;
		return;

	case PIOCOPENT:		/* open text file for reading */
		if ((xp = p->p_textp) && (ixp = xp->x_iptr)) {
			plock(ixp);
			ixp->i_count++;
			open1(ixp, FREAD, 0);	/* fairly magic linkage */
		} else
			u.u_error = ENOENT;
		return;

	case PIOCSTOP:		/* send STOP signal and... */
		if (p->p_stat != SSTOP)
			psignal(p, SIGSTOP);
				/* fall through */
	case PIOCWSTOP:		/* wait for process to STOP */
		if (p->p_stat != SSTOP)
			tsleep((caddr_t)p->p_trace, PZERO+1, 0);
		if (p->p_pid != (ip->i_number - PRMAGIC)
		||  p->p_stat == SZOMB)
			u.u_error = ENOENT;
		else if (p->p_stat != SSTOP)
			u.u_error = EINTR;
		return;

	case PIOCRUN:		/* make process runnable */
		if (p->p_stat == SSTOP)
			setrun(p);
		return;

	case PIOCSMASK:		/* set signal trace mask */
		if (copyin(cmaddr, (caddr_t)&ip->i_un.i_sigmask, sizeof(long))) {
			u.u_error = EFAULT;
			return;
		}
		if (ip->i_un.i_sigmask)
			p->p_flag |=  (STRC|SPROCTR);
		else
			p->p_flag &= ~(STRC|SPROCTR);
		return;

	case PIOCCSIG:		/* clear current signal */
		p->p_cursig = 0;
		return;

	case PIOCSSIG:		/* set current signal */
		if (p->p_stat != SSTOP)
			u.u_error = EBUSY;
		else if (copyin(cmaddr, (caddr_t)&n, sizeof(n)))
			u.u_error = EFAULT;
		else if ((unsigned)n > NSIG)
			u.u_error = EINVAL;
		else
			p->p_cursig = n;
		return;

	case PIOCKILL:		/* send signal */
		if (copyin(cmaddr, (caddr_t)&n, sizeof(n)))
			u.u_error = EFAULT;
		else if ((unsigned)n > NSIG)
			u.u_error = EINVAL;
		else
			psignal(p, n);
		return;

	case PIOCSEXEC:		/* stop on exec */
		p->p_flag |=  SSEXEC;
		return;

	case PIOCREXEC:		/* run on exec */
		p->p_flag &= ~SSEXEC;
		return;

	case PIOCNICE:		/* set nice priority */
		if (copyin(cmaddr, (caddr_t)&n, sizeof(n))) {
			u.u_error = EFAULT;
			return;
		}
		n += p->p_nice;
		if (n >= 2*NZERO)
			n = 2*NZERO -1;
		if (n < 0)
			n = 0;
		if (n < p->p_nice && !suser())
			return;
		p->p_nice = n;
		(void) setpri(p);
		return;
	}
}

/* Lock the process p. */

prlock(p)
register struct proc *p;
{
	int s;

	if (p != u.u_procp) {
	waitloop:
		if (prpidlock == p) {		/* wait if p has the interlock */
			prpidwant = u.u_procp;
			if (tsleep((caddr_t)&prpidlock, PZERO+1, 0) == TS_SIG)
				return (u.u_error = EINTR);
			goto waitloop;
		}
		if (p->p_flag & SPAGE) {	/* wait if p is paging */
			p->p_flag |= SPROCWT;
			if (tsleep((caddr_t)&(p->p_stat), PZERO+1, 0) == TS_SIG)
				return (u.u_error = EINTR);
			goto waitloop;
		}

		s = spl6();	/* keep clock out */

		if (p->p_flag&SPRBUSY || (p->p_stat != SSLEEP &&
		    p->p_stat != SRUN && p->p_stat != SSTOP)) {
			splx(s);
			return (u.u_error = EBUSY);
		}
		if (p->p_flag&SLOAD && p->p_stat == SRUN)
			remrq(p);	/* he's now invisible to swtch() */

		/* interlock; also causes swapin, inhibits swapout, setrq, remrq */

		p->p_flag |= SPROCIO;

		splx(s);	/* now do your worst, we don't care */
	}
	u.u_procp->p_flag |= SKEEP;	/* if we get swapped, could deadlock */

	while ((p->p_flag&SLOAD) == 0) {
		/* sched will see SPROCIO, swap him in, and signal us */
		if (tsleep((caddr_t)&p->p_addr, PZERO+1, 0) == TS_SIG) {
			prunlock(p);
			return (u.u_error = EINTR);
		}
	}
	while (prpidlock) {
		prpidwant = u.u_procp;
		if (tsleep((caddr_t)&prpidlock, PZERO+1, 0) == TS_SIG) {
			prunlock(p);
			return (u.u_error = EINTR);
		}
	}
	prpidlock = u.u_procp;
	/* now map his user area into kernel space */
	uaccess(p, Prusrmap, prusrutl);

	return (u.u_error = 0);
}

/* Undo prlock. */

prunlock(p)
register struct proc *p;
{
	int s;

	u.u_procp->p_flag &= ~SKEEP;

	if (p != u.u_procp) {
		s = spl6();	/* keep clock out during process state change */
		p->p_flag  &= ~SPROCIO;
		if (p->p_flag&SLOAD && p->p_stat == SRUN)
			setrq(p);	/* visible again to swtch() */
		splx(s);
	}
	if (prpidlock == u.u_procp) {
		prpidlock = 0;
		if (prpidwant) {
			prpidwant = 0;
			wakeup((caddr_t)&prpidlock);
		}
	}
	return 0;
}

/* Read/write from/to process p. */

#define REGADR(i)	((caddr_t)(regbase + i))
#define SEEKADR		((caddr_t)Ltol(u.u_offset))

prusrio(p, flag)
register struct proc *p; int flag;
{
	register int *regbase;
	caddr_t maxadr; int segment, resid;

	u.u_error = 0;
	if ((u_long)SEEKADR >= (u_long)(maxadr = SYSADR))
		u.u_error = EIO;
	else if (SEEKADR >= (caddr_t)USRSTACK)
		segment = USERAREA;
	else if (SEEKADR >= (maxadr = (caddr_t)USRSTACK) - (int)ptob(p->p_ssize))
		segment = STACK;
	else if (SEEKADR < (maxadr = ptob(p->p_tsize)))
		segment = TEXT;
	else if (SEEKADR < (maxadr = ptob(p->p_tsize+p->p_dsize)))
		segment = DATA;
	else
		u.u_error = EIO;
	if (p->p_flag & SSYS && segment != USERAREA)	/* too conservative? */
		u.u_error = EIO;

	if (u.u_error) return;

	if ((flag & B_READ) == 0) switch(segment) {
	case TEXT:
		prxdup(p);
		break;
	case USERAREA:
		regbase = prusrutl->u_ar0;
		if (SEEKADR < REGADR(AP) ||
		    (u_long)SEEKADR >= (u_long)(maxadr = REGADR(PS+1)))
			u.u_error = EIO;
		else if (SEEKADR >= REGADR(PC)) ;
		else if (SEEKADR < (maxadr = REGADR(FP+1))) ;
		else if (SEEKADR < REGADR(R0))
			u.u_error = EIO;
		else if (SEEKADR < (maxadr = REGADR(R11+1))) ;
		else if (SEEKADR < REGADR(SP) || SEEKADR >= (maxadr = REGADR(SP+1)))
			u.u_error = EIO;
		regbase = (int *)((u_long)prusrutl + (u_long)regbase - (u_long)&u);
		if ((regbase[PS]&PSL_USERSET) != PSL_USERSET ||
		    (regbase[PS]&PSL_USERCLR) != 0)
			u.u_error = EBUSY;
		break;
	}

	if (u.u_error) return;

	resid = u.u_count;
	if ((u_long)(Ltol(u.u_offset) + u.u_count) >= (u_long)maxadr)
		u.u_count = maxadr - (caddr_t)Ltol(u.u_offset);
	resid -= u.u_count;

	if (segment == USERAREA) {
		iomove((caddr_t)prusrutl + ((u_long)Ltol(u.u_offset) - (u_long)&u),
			u.u_count, flag);
		if ((flag & B_READ) == 0) {
			regbase[PS] |=  PSL_USERSET;
			regbase[PS] &= ~PSL_USERCLR;
		}
	} else
		priomove(p, flag);

	u.u_count += resid;
}

/* Move data between the object process and us. */

priomove(p, flag)
	register struct proc *p;
{
	register struct pte *pte;
	register clofset, clcount;
	int waslocked;

	while (u.u_count > 0 && u.u_error == 0) {
		pte = prclmap(p, (caddr_t)Ltol(u.u_offset), &waslocked);
		clofset = Ltol(u.u_offset) & CLOFSET;
		clcount = min(u.u_count, CLSIZE*NBPG - clofset);
		iomove(&priobuf[clofset], clcount, flag);
		prclunmap(pte, flag | waslocked);
	}
}

/* Map and lock a cluster from process into system space. */

struct pte *
prclmap(p, vaddr, flagp)
	register struct proc *p;
	register caddr_t vaddr;
	int *flagp;
{
	register i;
	register struct pte *pte;
	i = clbase(btop(vaddr));
	if (isassv(p, i))
		i -= P1OFF(p);
	pte = p->p_p0br + i;

	*flagp = 0;
	if (pte->pg_v) {
		if (cmap[pgtocm(pte->pg_pfnum)].c_lock)
			*flagp = B_PHYS;
		else
			mlock(pte->pg_pfnum);
	} else {
		p->p_flag |= SDLYU;
		pagein(vaddr, prusrutl);
		p->p_flag &= ~SDLYU;
		if (!pte->pg_v)
			panic("prclmap: pte not valid after pagein");
	}
	for (i=0; i<CLSIZE; i++) {
		*(int *)(Prbufmap + i) = PG_V | PG_KW | (pte->pg_pfnum + i);
		mtpr(TBIS, &priobuf[i*NBPG]);
	}
	return pte;
}

/* Release a cluster, updating its pte's. */

prclunmap(pte, flag)
	register struct pte *pte;
{
	register i;
	if ((flag & B_PHYS) == 0)
		munlock(pte->pg_pfnum);
	if ((flag & B_READ) == 0)	/* Write to device writes memory */
		for (i=0; i<CLSIZE; i++)
			(pte+i)->pg_m = 1;
}

/* Prepare the process' text segment for writing, duplicating it if necessary. */

prxdup(p)
	register struct proc *p;
{
	register struct text *xp, *pxp;

	if ((pxp = p->p_textp) == 0)
		return 0;
	if (!(pxp->x_flag&XPAGI) && pxp->x_count == 1) {
		pxp->x_flag |= XTRC|XWRIT;
		return 0;
	}
	if (pxp->x_flag&XTRC)
		panic("prxdup");

	for (xp = text; xp < textNTEXT && xp->x_iptr; xp++)
		/* void */ ;
	if (xp >= textNTEXT)
		return (u.u_error = ENOSPC);

	xp->x_flag = XLOCK|XTRC|XLOAD;
	xp->x_size = pxp->x_size;
	if (vsxalloc(xp) == NULL)
		return (u.u_error = ENOSPC);

	xp->x_count = 1;
	xp->x_ccount = 0;
	xp->x_rssize = 0;
	(xp->x_iptr = pxp->x_iptr)->i_count++;

	xlock(pxp);
	--pxp->x_count;
	pxp->x_flag &= ~XLOCK;
	xccdec(pxp, p);

	p->p_textp = xp;
	xlink(p);
	p->p_flag &= ~SPAGI;

	prxread(p, prusrutl);

	xp->x_flag |= XWRIT;
	xp->x_flag &= ~XLOAD;
	xunlock(xp);
	return u.u_error;
}

prxread(p, up)
	register struct proc *p;
	struct user *up;
{
	register struct inode *ip = p->p_textp->x_iptr;
	register count; register struct pte *pte;
	register caddr_t vaddr = 0; int waslocked;

	caddr_t ubase = u.u_base; unsigned int ucount = u.u_count;
	off_t uoffset = Ltol(u.u_offset);

	plock(ip);
	ip->i_flag |= ITEXT;

	count = up->u_exdata.ux_tsize;
	if (up->u_exdata.ux_mag == 0413)	/* 0413 on 4k file sys */
		u.u_offset = ltoL(BSIZE(0));
	else
		u.u_offset = ltoL(sizeof(u.u_exdata));

	u.u_segflg = SEGSYS;
	u.u_count = 0;
	while (count > 0 && u.u_count == 0) {
		pte = prclmap(p, vaddr, &waslocked);
		u.u_base = (caddr_t)priobuf;
		count -= u.u_count = min(count, CLSIZE*NBPG);
		vaddr += u.u_count;
		readi(ip);
		prclunmap(pte, B_WRITE | waslocked);
	}
	prele(ip);

	u.u_base = ubase; u.u_count = ucount;
	u.u_offset = ltoL(uoffset); u.u_segflg = SEGUDATA;

	return (count ? (u.u_error = EIO) : 0);
}

struct proc *
pritop(ip)
struct inode *ip;
{
	register struct proc *p;

	if ((p = ip->i_un.i_proc) == NULL
	||  p->p_pid != ip->i_number - PRMAGIC
	||  p->p_stat == 0 || p->p_stat == SZOMB) {
		u.u_error = ENOENT;	/* should really be ENXIO */
		return (NULL);
	}
	return (p);
}
