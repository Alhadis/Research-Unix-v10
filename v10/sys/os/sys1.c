#include "sys/param.h"
#include "sys/systm.h"
#include "sys/map.h"
#include "sys/mtpr.h"
#include "sys/user.h"
#include "sys/proc.h"
#include "sys/buf.h"
#include "sys/reg.h"
#include "sys/inode.h"
#include "sys/acct.h"
#include "/usr/include/wait.h"
#include "sys/pte.h"
#include "sys/vm.h"
#include "sys/text.h"
#include "sys/psl.h"
#include "sys/vlimit.h"
#include "sys/file.h"

/*
 * exec system call
 */
struct execa {
	char	*fname;
	char	**argp;
	char	**envp;
};

struct shdata {
	char		sd_save[DIRSIZ];
	int		sd_gid;
	int		sd_uid;
	int		sd_indir;
	char		sd_flag[SHSIZE];
};

struct swargs {
	char		*sw_cp;
	int		sw_argc;
	int		sw_envc;
	int		sw_chars;
	int		sw_bn;
	struct buf	*sw_bp;
};

void
str_stash(s, sp)
register char		*s;
register struct swargs	*sp;
{
	register char	*cp;
	register int	nc;
	register int	bmask;

	cp = sp->sw_cp;
	nc = sp->sw_chars;
	bmask = BMASK(argdev);

	do {
		if ((nc & bmask) == 0) {
			if (nc >= NCARGS) {
				u.u_error = E2BIG;
				return;
			}
			if (sp->sw_bp != NULL)
				bdwrite(sp->sw_bp);
			sp->sw_bp = getblk(argdev, (daddr_t)(dbtofsb(argdev,
					sp->sw_bn) + (nc>>BSHIFT(argdev))));
			cp = sp->sw_bp->b_un.b_addr;
		}
		nc++;
	} while ((*cp++ = *s++) != '\0');

	sp->sw_chars = nc;
	sp->sw_cp = cp;
	sp->sw_argc++;
}

int
vec_stash(v, sp)
register char		**v;
register struct swargs	*sp;
{
	register char	*s;
	register int	nc;
	register int	nleft;
	register int	i;
	int		bsize;
	int		count;
	int		ap;

	if (v == NULL)
		return 0;

	nc = sp->sw_chars;
	bsize = BSIZE(argdev);
	nleft = ((nc + bsize-1) & ~(bsize-1)) - nc;
	count = 0;

	for (;;) {
		if ((ap = fuword((caddr_t)v++)) < 0) {
			u.u_error = EFAULT;
			return 0;
		}
		if ((s = (char *)ap) == NULL)
			break;
		if ((i = fustrlen(s)) < 0) {
			u.u_error = EFAULT;
			return 0;
		}
		while (i > nleft) {
			if (sp->sw_bp) {
				bcopy(s, sp->sw_cp, nleft);
				s += nleft;
				nc += nleft;
				i -= nleft;
				if (nc >= NCARGS) {
					u.u_error = E2BIG;
					return 0;
				}
				bdwrite(sp->sw_bp);
			}
			sp->sw_bp = getblk(argdev, (daddr_t)(dbtofsb(argdev,
					sp->sw_bn) + (nc>>BSHIFT(argdev))));
			sp->sw_cp = sp->sw_bp->b_un.b_addr;
			nleft = bsize;
		}
		bcopy(s, sp->sw_cp, i);
		sp->sw_cp += i;
		nc += i;
		nleft -= i;
		count++;
	}
	sp->sw_chars = nc;
	sp->sw_argc += count;
	return count;
}

exece()
{
	register int		i;
	register int		ap;
	register int		bsize;
	register char		*cp;
	register char		*ucp;
	register struct execa	*uap;
	struct buf		*bp;
	struct inode		*ip;
	struct shdata		sh;
	struct swargs		sw;
	extern struct map argmap[];
	extern struct inode	*gethead();

	uap = (struct execa *)u.u_ap;
	sh.sd_uid = u.u_uid;
	sh.sd_gid = u.u_gid;
	sh.sd_indir = 0;
	sh.sd_flag[0] = '\0';
	if ((ip = gethead(uap->fname, &sh)) == NULL)
		return;

	sw.sw_argc = 0;
	sw.sw_chars = 0;
	sw.sw_bp = NULL;
	bsize = BSIZE(argdev);
	if ((sw.sw_bn = rmalloc(argmap, ctod(clrnd((int)btoc(NCARGS))))) == 0) {
		swkill(u.u_procp, "exece");
		iput(ip);
		return;
	}
	if (sw.sw_bn % CLSIZE)
		panic("exece rmalloc");
	if (sh.sd_indir) {
		if ((ap = fuword((caddr_t)uap->argp)) < 0){
			u.u_error = EFAULT;
			goto out;
		}
		if (ap) {
			uap->argp++;
			if (fustrlen((char *)ap) < 0) {
				u.u_error = EFAULT;
				goto out;
			}
			/*
			 * improper assumption
			 * ap is a user address;
			 * str_stash assumes kernel-accessible
			 */
			str_stash((char *)ap, &sw);
		}
		if (sh.sd_flag[0] != '\0')
			str_stash(sh.sd_flag, &sw);
		str_stash(uap->fname, &sw);
	}
	vec_stash(uap->argp, &sw);
	sw.sw_envc = vec_stash(uap->envp, &sw);
	bp = sw.sw_bp;
	if (bp) {
		bdwrite(bp);
		bp = NULL;
	}

	if (u.u_error)
		goto out;

	sw.sw_chars = (sw.sw_chars + NBPW-1) & ~(NBPW-1);
	/* 4: argc, null after args, null after env, null after everything,  */
	getxfile(ip, sw.sw_chars + (sw.sw_argc+4)*NBPW, sh.sd_uid, sh.sd_gid);

	if (u.u_error)
		goto out;

	iput(ip);
	ucp = (char *)(USRSTACK - sw.sw_chars - NBPW);
	ap = (int)ucp - (sw.sw_argc + 3) * NBPW;
	u.u_ar0[SP] = ap;
	*(int *)ap = sw.sw_argc - sw.sw_envc;

	for (i = 0, cp = ucp; sw.sw_chars > 0; i++, cp += bsize, sw.sw_chars -= bsize) {
		if (bp)
			brelse(bp);
		bp = bread(argdev, (daddr_t)(dbtofsb(argdev, sw.sw_bn)+i));
		bcopy(bp->b_un.b_addr, cp, min(bsize, sw.sw_chars));	
		bp->b_flags |= B_AGE;
		bp->b_flags &= ~B_DELWRI;
	}

	if (bp)
		brelse(bp);

	for (cp = ucp; ; cp += fustrlen(cp)) {
		ap += NBPW;
		if (sw.sw_argc == sw.sw_envc)
			ap += NBPW;
		if (--sw.sw_argc < 0)
			break;
		*(char **)ap = cp;
	}
	setregs(sh.sd_save);
	rmfree(argmap, ctod(clrnd((int) btoc(NCARGS))), sw.sw_bn);
	return;

out:
	iput(ip);
	for (i = (((sw.sw_chars+bsize-1)&~(bsize-1))>>BSHIFT(argdev))-1;
	     i >= 0; i--) {
		bp = getblk(argdev, (daddr_t)(dbtofsb(argdev, sw.sw_bn)+i));
		bp->b_flags |= B_AGE;
		bp->b_flags &= ~B_DELWRI;
		brelse(bp);
	}
	rmfree(argmap, ctod(clrnd((int) btoc(NCARGS))), sw.sw_bn);
}

/*
 *	Get a shell after #! magic number.
 */
struct inode	*
getshell(ip, sp)
struct inode	*ip;
struct shdata	*sp;
{
	register char		*cp, *fname;
	extern struct inode	*gethead();

	if(u.u_exdata.ux_shell[0] != '#' || u.u_exdata.ux_shell[1] != '!' || sp->sd_indir)
		goto error;
	cp = &u.u_exdata.ux_shell[2];
	for (;;) {
		if (cp == &u.u_exdata.ux_shell[SHSIZE])
			goto error;
		if (*cp == '\n') {
			*cp = '\0';
			break;
		}
		if (*cp == '\t')
			*cp = ' ';
		cp++;
	}
	for (cp = &u.u_exdata.ux_shell[2]; *cp == ' '; cp++);
	if (*cp == '\0')
		goto error;
	fname = cp;
	while (*cp != '\0') {
		if (*cp == ' ') {
			*cp++ = '\0';
			while (*cp == ' ')
				cp++;
			/*
			 *	Shell argument (one only).
			 */
			if (*cp != '\0') {
				register int	i;

				i = 0;
				do
					sp->sd_flag[i++] = *cp++;
				while (*cp != ' ' && *cp != '\0');
				sp->sd_flag[i] = '\0';
			}
			break;
		}
		else
			cp++;
	}
	iput(ip);
	sp->sd_indir = 1;
	return gethead(fname, sp);

error:
	iput(ip);
	u.u_error = ENOEXEC;
	return NULL;
}

/*
 *	Get the header of an executable and do all the right tests.
 */
struct inode	*
gethead(fname, sp)
char *fname;
struct shdata	*sp;
{
	register struct inode	*ip;
	struct argnamei nmarg;

	nmarg = nilargnamei;
	if(sp->sd_indir == 0) {
		nmarg.un.buf = (caddr_t)sp->sd_save;
		nmarg.len = sizeof(sp->sd_save);
		if((ip = namei(fname, SEGUDATA, &nmarg, 1)) == NULL)
			return NULL;
	}
	else if((ip = namei(fname, SEGSYS, &nmarg, 1)) == NULL)
		return NULL;

	/*
	 *	Setuid and setgid denied for network root.
	 */
	if (!(ip->i_mode & ICONC)) {
		if ((ip->i_mode & ISUID) != 0 && ip->i_uid != -1)
			sp->sd_uid = ip->i_uid;

		if ((ip->i_mode & ISGID) != 0 && ip->i_gid != -1)
			sp->sd_gid = ip->i_gid;
	}

	/*
	 *	Check permission.  May not trace something we can't read.
	 */
	if(access(ip, IEXEC) || (PTRACED(u.u_procp) && access(ip, IREAD)))
		goto out;

	/*
	 *	Must be a regular file and must really be executable.
	 */
	if ((ip->i_mode & IFMT) != IFREG ||
	    (ip->i_mode & (IEXEC | (IEXEC >> 3) | (IEXEC >> 6))) == 0) {
		u.u_error = EACCES;
		goto out;
	}

	/*
	 * ux_mag = 407/410/413
	 *	407 is plain executable
	 *	410 is RO text
	 *	413 is demand paged RO text
	 *
	 * Also an ASCII line beginning with #! is
	 * the file name of a ``shell'' and arguments may be prepended
	 * to the argument list if given here.
	 *
	 * Shell names are limited in length.
	 *
	 * Only one argument may be passed to the shell from the ASCII line.
	 */
	u.u_base = (caddr_t)&u.u_exdata;
	u.u_count = sizeof(u.u_exdata);
	u.u_offset = ltoL(0);
	u.u_segflg = SEGSYS;
	readi(ip);
	u.u_segflg = SEGUDATA;

	if (u.u_error)
		goto out;

	if (u.u_count > sizeof (u.u_exdata) - sizeof (u.u_exdata.Ux_A))
		ip = getshell(ip, sp);
	else {
		switch (u.u_exdata.ux_mag) {
		case 0407:
			u.u_exdata.ux_dsize += u.u_exdata.ux_tsize;
			u.u_exdata.ux_tsize = 0;
			break;

		case 0410:
		case 0413:
			if (u.u_exdata.ux_tsize == 0)
				u.u_error = ENOEXEC;
			break;

		default:
			ip = getshell(ip, sp);
		}
	}

out:
	if (u.u_error && ip != NULL) {
		iput(ip);
		ip = NULL;
	}

	return ip;
}

/*
 * Read in and set up memory for executed file.
 */
getxfile(ip, nargc, uid, gid)
register struct inode *ip;
{
	register clicks_t ts, ds, ss;
	int pagi;

	/* for now, no paging off alternate file systems */
	if (u.u_exdata.ux_mag == 0413 && !BITFS(ip->i_dev) && !ip->i_fstyp)
		pagi = SPAGI;
	else
		pagi = 0;
	/* never demand-page if process is traced (panic pagein mfind) */
	if (PTRACED(u.u_procp))
		pagi = 0;

	if(u.u_exdata.ux_tsize!=0 && (ip->i_flag&ITEXT)==0 && ip->i_count!=1) {
		register struct file *fp;

		for (fp = file; fp < fileNFILE; fp++)
			if (fp->f_count && fp->f_inode==ip
			 && (fp->f_flag&FWRITE)) {
				u.u_error = ETXTBSY;
				goto bad;
			}
	}

	/*
	 * find text and data sizes
	 * try them out for possible
	 * exceed of max sizes
	 */

	ts = clrnd(btoc(u.u_exdata.ux_tsize));
	ds = clrnd(btoc((u.u_exdata.ux_dsize+u.u_exdata.ux_bsize)));
	ss = clrnd(SSIZE + btoc(nargc));
	if (chksize(ts, ds, ss))
		goto bad;
	u.u_cdmap = zdmap;
	u.u_csmap = zdmap;
	if (swpexpand(ds, ss, &u.u_cdmap, &u.u_csmap) == NULL)
		goto bad;

	/*
	 * At this point, committed to the new image!
	 * Release virtual memory resources of old process, and
	 * initialize the virtual memory of the new process.
	 */
	u.u_prof.pr_scale = 0;
	vrelvm();
	u.u_procp->p_flag &= ~(SPAGI|SSEQL|SUANOM);
	u.u_procp->p_flag |= pagi;
	u.u_dmap = u.u_cdmap;
	u.u_smap = u.u_csmap;
	vgetvm(ts, ds, ss);

	if (pagi == 0) {
		/*
		 * Read in data segment.
		 */
		u.u_base = (char *)ctob(ts);
		if(u.u_exdata.ux_mag != 0413)
			u.u_offset = ltoL(sizeof(u.u_exdata)+u.u_exdata.ux_tsize);
		else	/* stupid 1k 0413 files */
			u.u_offset = ltoL(BSIZE(0) + u.u_exdata.ux_tsize);
		u.u_count = u.u_exdata.ux_dsize;
		readi(ip);
	}
	xalloc(ip, pagi);
	if (pagi && u.u_procp->p_textp)
		vinifod((struct fpte *)dptopte(u.u_procp, 0),
		    u.u_procp->p_textp->x_iptr,
		    1 + ts/CLSIZE, (int)btoc(u.u_exdata.ux_dsize));

	/* THIS SHOULD BE DONE AT A LOWER LEVEL, IF AT ALL */
	mtpr(TBIA, 0);

	/*
	 * set SUID/SGID protections, if no tracing
	 */
	if (!PTRACED(u.u_procp)) {
		u.u_uid = uid;
		u.u_procp->p_uid = uid;
		u.u_gid = gid;
	}
	if (u.u_procp->p_flag&SSEXEC)
		psignal(u.u_procp, SIGSTOP);
	u.u_tsize = ts;
	u.u_dsize = ds;
	u.u_ssize = ss;
bad:
	return;
}

/*
 * Reset context on exec
 */
setregs(comm)
char *comm;
{
	register i;
	register int (*f)();

	for (i = 0; i < NSIG; i++) {
		if ((f = u.u_signal[i]) == SIG_DFL
		||  f == SIG_IGN || f == SIG_HOLD)
			continue;
		spl6();		/* needed? */
		u.u_signal[i] = SIG_DFL;
		P_SETDFL(u.u_procp, SIGMASK(i));
		spl0();
	}
/*
	for(rp = &u.u_ar0[0]; rp < &u.u_ar0[16];)
		*rp++ = 0;
*/
	u.u_ar0[PC] = u.u_exdata.ux_entloc + 2; /* skip over entry mask */
	for(i=0; i<NOFILE; i++) {
		if (u.u_pofile[i]&EXCLOSE) {
			closef(u.u_ofile[i]);
			u.u_ofile[i] = NULL;
			u.u_pofile[i] &= ~EXCLOSE;
		}
	}
	/*
	 * Remember file name for accounting.
	 */
	u.u_acflag &= ~AFORK;
	bcopy((caddr_t)comm, (caddr_t)u.u_comm, sizeof(u.u_comm));
}

/*
 * exit system call:
 * pass back caller's arg
 */
rexit()
{
	register struct a {
		int	rval;
	} *uap;

	uap = (struct a *)u.u_ap;
	exit((uap->rval & 0377) << 8);
}

/*
 * Release resources.
 * Save u. area for parent to look at.
 * Enter zombie state.
 * Wake up parent and init processes,
 * and dispose of children.
 */
exit(rv)
{
	register int i;
	register struct proc *p, *q;
	register struct file *f;

	vmsizmon();
	p = u.u_procp;
	p->p_flag &= ~(STRC|SULOCK);
	p->p_flag |= SWEXIT;
	p->p_clktim = 0;
	(void) spl6();
	p->p_siga0 = p->p_siga1 = 0;
	p->p_ignsig = ~0;		/* magic: ignore all */
	(void) spl0();
	p->p_cpticks = 0;
	p->p_pctcpu = 0;
	for(i=0; i<NSIG; i++)
		u.u_signal[i] = SIG_IGN;
	vrelvm();
	for(i=0; i<NOFILE; i++) {
		if ((f = u.u_ofile[i]) == NULL)
			continue;
		u.u_ofile[i] = NULL;
		closef(f);
	}
	plock(u.u_cdir);
	iput(u.u_cdir);
	if (u.u_rdir) {
		plock(u.u_rdir);
		iput(u.u_rdir);
	}
	u.u_limit[LIM_FSIZE] = INFINITY;
	acct();
	vrelpt(u.u_procp);
	vrelu(u.u_procp, 0);
	p->p_stat = SZOMB;
	noproc = 1;
	if (p->p_pid < SYSPIDS)
		printf("process %d died\n", p->p_pid);
done:
	((struct xproc *)p)->xp_xstat = rv;		/* overlay */
	((struct xproc *)p)->xp_vm = u.u_vm;		/* overlay */
	vmsadd(&((struct xproc *)p)->xp_vm, &u.u_cvm);
	for(q = proc; q < procNPROC; q++)
		if (q->p_stat != 0 && q->p_pptr == p) {
			q->p_pptr = &proc[INITPID];
			q->p_ppid = 1;
			wakeup((caddr_t)&proc[INITPID]);
			/*
			 * Traced processes are killed
			 * since their existence means someone is screwing up.
			 * this is probably wrong now
			 * Stopped processes are sent a hangup and a continue.
			 * This is designed to be ``safe'' for setuid
			 * processes since they must be willing to tolerate
			 * hangups anyways.
			 */
			if (q->p_flag&STRC) {
				q->p_flag &= ~STRC;
				psignal(q, SIGKILL);
			} else if (q->p_stat == SSTOP) {
				psignal(q, SIGHUP);
				psignal(q, SIGCONT);
			}
			(void) spgrp(q, -1);
		}
	wakeup((caddr_t)p->p_pptr);
	if (p->p_trace)
		wakeup((caddr_t)p->p_trace);
	psignal(p->p_pptr, SIGCHLD);
	swtch();
}

wait()
{
	struct vtimes vm;
	struct vtimes *vp;

	if ((u.u_ar0[PS] & PSL_ALLCC) != PSL_ALLCC) {
		wait1(0, (struct vtimes *)0);
		return;
	}
	vp = (struct vtimes *)u.u_ar0[R1];
	wait1(u.u_ar0[R0], &vm);
	if (u.u_error)
		return;
	(void) copyout((caddr_t)&vm, (caddr_t)vp, sizeof (struct vtimes));
}

/*
 * Wait system call.
 * Search for a terminated (zombie) child,
 * finally lay it to rest, and collect its status.
 * Look also for stopped (traced) children,
 * and pass back status from them.
 */
wait1(options, vp)
	register options;
	struct vtimes *vp;
{
	register f;
	register struct proc *p;
	register struct proc *myp;	/* optimization */

	f = 0;
	myp = u.u_procp;
loop:
	for(p = proc; p < procNPROC; p++) {
		if (p->p_stat == 0 || p->p_pptr != myp)
			continue;
		f++;
		if(p->p_stat == SZOMB) {
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = ((struct xproc *)p)->xp_xstat;
			if (vp)
				*vp = ((struct xproc *)p)->xp_vm;
			vmsadd(&u.u_cvm, &((struct xproc *)p)->xp_vm);
			p->p_stat = 0;
			/* if it mattered, we could bzero(p, sizeof(*p)) here */
			if (p == procNPROC - 1) {
				do
					--p;
				while (p->p_stat == 0);
				procNPROC = p + 1;
			}
			return;
		}
		if (p->p_stat == SSTOP && (p->p_flag&SWTED)==0 &&
		    (p->p_flag&STRC)==0) {
			p->p_flag |= SWTED;
			u.u_r.r_val1 = p->p_pid;
			u.u_r.r_val2 = (p->p_cursig<<8) | WSTOPPED;
			return;
		}
	}
	if (f==0) {
		u.u_error = ECHILD;
		return;
	}
	if (options&WNOHANG) {
		u.u_r.r_val1 = 0;
		return;
	}
	sleep((caddr_t)myp, PWAIT);
	goto loop;
}

/*
 * fork system call.
 */
fork()
{
	register struct proc *p1, *p2;
	register int a, pid;
	static int mpid = 0;

	u.u_cdmap = zdmap;
	u.u_csmap = zdmap;
	if (swpexpand(u.u_dsize, u.u_ssize, &u.u_cdmap, &u.u_csmap) == 0)
		goto out;
retry:
	pid = ++mpid;
	if (pid > MAXPID)
		pid = mpid = 1;
	a = 0;
	p2 = NULL;
	for(p1 = proc; p1 < procNPROC; p1++) {
		if (p1->p_stat == 0) {
			if (p2 == NULL)
				p2 = p1;
			continue;
		}
		if (p1->p_pid == pid || p1->p_pgrp == pid)
			goto retry;
		if (p1->p_uid == u.u_uid)
			a++;
	}
	if (p2 == NULL) {	/* try pushing up high water mark */
		for (; p1 < &proc[proccnt]; p1++)
			if (p1->p_stat == 0) {
				p2 = p1;
				p1++;
				break;
			}
		procNPROC = p1;
	}
	/*
	 * Disallow if
	 *  No processes at all;
	 *  not su and too many procs owned; or
	 *  not su and would take last slot.
	 */
	if (p2==NULL)
		tablefull("proc");
	if (p2==NULL || (u.u_uid!=0 && (p2==&proc[proccnt-1] || a>MAXUPRC))) {
		u.u_error = EAGAIN;
		(void) vsexpand(0, &u.u_cdmap, 1);
		(void) vsexpand(0, &u.u_csmap, 1);
		goto out;
	}
	p1 = u.u_procp;
	if(newproc(p2, pid)) {
		u.u_r.r_val1 = p1->p_pid;
		u.u_r.r_val2 = 1;  /* child */
		u.u_start = time;
		u.u_acflag = AFORK;
		u.u_nbadio = 0;
		return;
	}
	u.u_r.r_val1 = pid;
out:
	u.u_r.r_val2 = 0;
}

/*
 * break system call.
 *  -- bad planning: "break" is a dirty word in C.
 */
sbreak()
{
	struct a {
		char	*nsiz;
	};
	register int n, d;

	/*
	 * set n to new data size
	 * set d to new-old
	 */

	n = btoc(((struct a *)u.u_ap)->nsiz);
#if NOTDEF
	if (!u.u_sep)
#endif
		n -= ctos(u.u_tsize) * stoc(1);
	if (n < 0)
		n = 0;
	d = clrnd(n - u.u_dsize);
	if (ctob(u.u_dsize+d) > u.u_limit[LIM_DATA]) {
		u.u_error = ENOMEM;
		return;
	}
	if (chksize(u.u_tsize, u.u_dsize+d, u.u_ssize))
		return;
	if (swpexpand(u.u_dsize+d, u.u_ssize, &u.u_dmap, &u.u_smap)==0)
		return;
	expand(d, P0BR);
}
