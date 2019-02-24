/*	sys4.c	4.10	81/07/04	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/proc.h"
#include "sys/clock.h"
#include "sys/timeb.h"
#include "sys/times.h"
#include "sys/file.h"

/*
 * Everything in this file is a routine implementing a system call.
 */

/*
 * return the current time (old-style entry)
 */
gtime()
{
	u.u_r.r_time = time;
	clkcheck();
}

/*
 * New time entry-- return TOD with milliseconds, timezone,
 * DST flag
 */
ftime()
{
	register struct a {
		struct	timeb	*tp;
	} *uap;
	struct timeb t;
	register unsigned ms;
	extern int timezone, dstflag;

	uap = (struct a *)u.u_ap;
	(void) spl7();
	t.time = time;
	ms = lbolt;
	(void) spl0();
	if (ms > HZ) {
		ms -= HZ;
		t.time++;
	}
	t.millitm = (1000*ms)/HZ;
	t.timezone = timezone;
	t.dstflag = dstflag;
	if (copyout((caddr_t)&t, (caddr_t)uap->tp, sizeof(t)))
		u.u_error = EFAULT;
	clkcheck();
}

/*
 * Set the time
 */
stime()
{
	register struct a {
		time_t	time;
	} *uap;
	extern time_t bootime;

	uap = (struct a *)u.u_ap;
	if(suser()) {
		bootime += uap->time - time;	/* silly */
		time = uap->time;
		clkset();
	}
}

setuid()
{
	register uid;
	register struct a {
		int	uid;
	} *uap;

	uap = (struct a *)u.u_ap;
	uid = uap->uid;
	if(u.u_ruid == uid || u.u_uid == uid || suser()) {
		u.u_uid = uid;
		u.u_procp->p_uid = uid;
		u.u_ruid = uid;
	}
}

getuid()
{

	u.u_r.r_val1 = u.u_ruid;
	u.u_r.r_val2 = u.u_uid;
}

setruid()
{
	register uid;
	register struct a {
		int	uid;
	} *uap;

	uap = (struct a *)u.u_ap;
	uid = uap->uid;
	if(suser())
		u.u_ruid = uid;
}

setgid()
{
	register gid;
	register struct a {
		int	gid;
	} *uap;

	uap = (struct a *)u.u_ap;
	gid = uap->gid;
	if(u.u_rgid == gid || u.u_gid == gid || suser()) {
		u.u_gid = gid;
		u.u_rgid = gid;
	}
}

getgid()
{

	u.u_r.r_val1 = u.u_rgid;
	u.u_r.r_val2 = u.u_gid;
}

setgroups()
{
	register struct	a {
		u_int	gidsetsize;
		short	*gidset;
	} *uap = (struct a *)u.u_ap;
	register short *gp;

	if (!suser())
		return;
	if (uap->gidsetsize > sizeof u.u_groups / sizeof u.u_groups[0]) {
		u.u_error = EINVAL;
		return;
	}
	if (copyin((caddr_t)uap->gidset, (caddr_t)u.u_groups,
			uap->gidsetsize * sizeof u.u_groups[0]) < 0) {
		u.u_error = EFAULT;
		return;
	}
	for (gp = &u.u_groups[uap->gidsetsize] ; gp < &u.u_groups[NGROUPS]; gp++)
		*gp = NOGROUP;
}

/*
 * Check if gid is a member of the group set.
 */
groupmember(gid)
	short gid;
{
	register short *gp;

	if (u.u_gid == gid)
		return (1);
	for (gp = u.u_groups; gp < &u.u_groups[NGROUPS] && *gp != NOGROUP; gp++)
		if (*gp == gid)
			return (1);
	return (0);
}

getgroups()
{
	register struct	a {
		u_int	gidsetsize;
		short	*gidset;
	} *uap = (struct a *)u.u_ap;
	register short *gp;

	for (gp = &u.u_groups[NGROUPS]; gp > u.u_groups; gp--)
		if (gp[-1] != NOGROUP)
			break;
	if (uap->gidsetsize < gp - u.u_groups) {
		u.u_error = EINVAL;
		return;
	}
	uap->gidsetsize = gp - u.u_groups;
	if (copyout((caddr_t)u.u_groups, (caddr_t)uap->gidset,
			uap->gidsetsize * sizeof u.u_groups[0]) < 0) {
		u.u_error = EFAULT;
		return;
	}
	u.u_r.r_val1 = uap->gidsetsize;
}

getpid()
{
	u.u_r.r_val1 = u.u_procp->p_pid;
	u.u_r.r_val2 = u.u_procp->p_ppid;
}

sync()
{

	update();
}

nice()
{
	register n;
	register struct a {
		int	niceness;
	} *uap;

	uap = (struct a *)u.u_ap;
	n = uap->niceness + u.u_procp->p_nice;
	if(n >= 2*NZERO)
		n = 2*NZERO -1;
	if(n < 0)
		n = 0;
	if (n < u.u_procp->p_nice && !suser())
		return;
	u.u_procp->p_nice = n;
}

/*
 * Unlink system call.
 * Hard to avoid races here, especially
 * in unlinking directories.
 */
unlink()
{
	struct a {
		char	*fname;
	};
	struct argnamei nmarg;

	nmarg = nilargnamei;
	nmarg.flag = NI_DEL;
	(void) namei(((struct a *)u.u_ap)->fname, SEGUDATA, &nmarg, 0);
}
chdir()
{
	chdirec(&u.u_cdir);
}

chroot()
{
	if (suser())
		chdirec(&u.u_rdir);
}

chdirec(ipp)
register struct inode **ipp;
{
	register struct inode *ip;
	struct a {
		char	*fname;
	};

	ip = namei(((struct a *)u.u_ap)->fname, SEGUDATA, &nilargnamei, 1);
	if(ip == NULL)
		return;
	if((ip->i_mode&IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		goto bad;
	}
	if(access(ip, IEXEC))
		goto bad;
	prele(ip);
	if (*ipp) {
		plock(*ipp);
		iput(*ipp);
	}
	*ipp = ip;
	return;

bad:
	iput(ip);
}

fchmod()
{	register struct file *fp;
	register struct a {
		int fd;
		int fmode;
	} *uap;

	uap = (struct a *)u.u_ap;
	if((fp = getf(uap->fd)) == NULL) {
		u.u_error = EBADF;
		return;
	}
	chmod1(fp->f_inode, uap->fmode);
}

chmod()
{
	register struct inode *ip;
	register struct a {
		char	*fname;
		int	fmode;
	} *uap;

	uap = (struct a *)u.u_ap;
	if ((ip = namei(uap->fname, SEGUDATA, &nilargnamei, 1)) == NULL)
		return;
	chmod1(ip, uap->fmode);
	iput(ip);
}

chmod1(ip, mode)
register struct inode *ip;
{

	if (accowner(ip) == 0)
		return;
	ip->i_mode &= ~07777;
	if (!(mode&ICONC) && u.u_uid!=0 && !groupmember(ip->i_gid))
		mode &= ~ISGID;
	ip->i_mode |= mode&07777;
	ip->i_flag |= ICHG;
	iupdat(ip, &time, &time, 0);
}

/* chown with file descriptor*/
fchown()
{	register struct file *fp;
	register struct a {
		int fd;
		int uid;
		int gid;
	} *uap;

	uap = (struct a *)u.u_ap;
	if((fp = getf(uap->fd)) == NULL) {
		u.u_error = EBADF;
		return;
	}
	chown1(fp->f_inode, uap->uid, uap->gid);
}

chown()
{
	register struct inode *ip;
	register struct a {
		char	*fname;
		int	uid;
		int	gid;
	} *uap;

	uap = (struct a *)u.u_ap;
	if ((ip = namei(uap->fname, SEGUDATA, &nilargnamei, 1)) == NULL)
		return;
	chown1(ip, uap->uid, uap->gid);
	iput(ip);
}

chown1(ip, uid, gid)
	register struct inode *ip;
{

	if (accowner(ip) == 0)
		return;
	if(u.u_uid){
		if((ip->i_uid != uid) || !groupmember(gid)){
			u.u_error = EPERM;
			return;
		}
		if((ip->i_mode&ICONC)==0 && gid != ip->i_gid)
			ip->i_mode &=~ ISGID;
	}
	ip->i_uid = uid;
	ip->i_gid = gid;
	ip->i_flag |= ICHG;
	iupdat(ip, &time, &time, 0);
}

ssig()
{
	register int (*f)();
	struct a {
		int	signo;
		int	(*fun)();
	} *uap;
	register struct proc *p = u.u_procp;
	register a;
	register long sigmask;

	uap = (struct a *)u.u_ap;
	a = uap->signo & SIGNUMMASK;
	f = uap->fun;
	if(a<=0 || a>=NSIG || a==SIGKILL || a==SIGSTOP) {
		u.u_error = EINVAL;
		return;
	}
	u.u_r.r_val1 = (int)u.u_signal[a];
	sigmask = SIGMASK(a);
	(void) spl6();
	if (u.u_signal[a] == SIG_IGN)
		p->p_sig &= ~sigmask;		/* never to be seen again */
	u.u_signal[a] = f;
	if (f == SIG_DFL)
		P_SETDFL(p, sigmask);
	else if (f == SIG_IGN)
		P_SETIGN(p, sigmask);
	else if (f == SIG_HOLD)
		P_SETHOLD(p, sigmask);
	else
		P_SETCATCH(p, sigmask);
	(void) spl0();
	if (uap->signo & SIGDOPAUSE)
		pause();
}

kill()
{
	register struct a {
		int	pid;
		int	signo;
	} *uap;

	u.u_error = ESRCH;		/* default error */
	uap = (struct a *)u.u_ap;
	if (uap->signo > NSIG || uap->signo < 0) {
		u.u_error = EINVAL;
		return;
	}
	if (uap->pid == -1)
		killall(uap->signo);
	else if(uap->pid > 0)
		killproc(uap->pid, uap->signo);
	else if (uap->pid < 0)
		killpgrp(-uap->pid, uap->signo);
	else
		killpgrp(u.u_procp->p_pgrp, uap->signo);
}

/*
 *  kill a single process
 */
killproc(pid, sig)
	register int pid, sig;
{
	register struct proc *p;

	for (p = proc; p < procNPROC; p++)
		if (p->p_stat && p->p_pid == pid)
			break;
	if (p == procNPROC)
		return;
	if (u.u_uid && u.u_uid != p->p_uid) {	/* no permission */
		u.u_error = EPERM;
		return;
	}
	if (sig != 0)				/* real signal? */
		psignal(p, sig);		/* yes, send it */
	u.u_error = 0;
}

/*
 *  Kill all processes within a process group but not system processes.
 *  SIGCONT may be sent to any descendants (can you say hack?).
 */
killpgrp(pgrp, sig)
	register int pgrp, sig;
{
	register struct proc *p;

	for(p = proc; p < procNPROC; p++) {
		if(p->p_stat == 0)
			continue;		/* non-existent */
		if (p->p_pgrp!=pgrp || p->p_flag&SSYS)
			continue;
		if(u.u_uid != 0 && u.u_uid != p->p_uid &&
		    (sig != SIGCONT || !inferior(p)))
			continue;
		u.u_error = 0;
		if (sig != 0)				/* real signal? */
			psignal(p, sig);		/* yes, send it */
	}
}

/*
 * Kill all processes except the system processes and the current process
 */
killall(sig)
	register int sig;
{
	register struct proc *p;

	if (!suser())
		return;
	for(p = proc; p < procNPROC; p++) {
		if(p->p_stat == 0)
			continue;
		if (p->p_flag&SSYS || p==u.u_procp)
			continue;
		u.u_error = 0;
		psignal(p, sig);
	}
}

times()
{
	register struct a {
		time_t	(*times)[4];
	} *uap;
	struct tms tms;

	tms.tms_utime = u.u_vm.vm_utime;
	tms.tms_stime = u.u_vm.vm_stime;
	tms.tms_cutime = u.u_cvm.vm_utime;
	tms.tms_cstime = u.u_cvm.vm_stime;
	uap = (struct a *)u.u_ap;
	if (copyout((caddr_t)&tms, (caddr_t)uap->times, sizeof(struct tms)) < 0)
		u.u_error = EFAULT;
}

profil()
{
	register struct a {
		short	*bufbase;
		unsigned bufsize;
		unsigned pcoffset;
		unsigned pcscale;
	} *uap;

	uap = (struct a *)u.u_ap;
	u.u_prof.pr_base = uap->bufbase;
	u.u_prof.pr_size = uap->bufsize;
	u.u_prof.pr_off = uap->pcoffset;
	u.u_prof.pr_scale = uap->pcscale;
}

/*
 * alarm clock signal
 */
alarm()
{
	register struct proc *p;
	register c;
	register struct a {
		int	deltat;
	} *uap;

	uap = (struct a *)u.u_ap;
	p = u.u_procp;
	c = p->p_clktim;
	if (uap->deltat > 65535L)
		uap->deltat = 65535;
	p->p_clktim = uap->deltat;
	u.u_r.r_val1 = c;
}

/*
 * indefinite wait.
 * no one should wakeup(&u)
 */
pause()
{

	for(;;)
		sleep((caddr_t)&u, PSLEP);
}

/*
 * mode mask for creation of files
 */
umask()
{
	register struct a {
		int	mask;
	} *uap;
	register t;

	uap = (struct a *)u.u_ap;
	t = u.u_cmask;
	u.u_cmask = uap->mask & 0777;
	u.u_r.r_val1 = t;
}

/*
 * Set IUPD and IACC times on file.
 * Can't set ICHG.
 */
utime()
{
	register struct a {
		char	*fname;
		time_t	*tptr;
	} *uap;
	register struct inode *ip;
	time_t tv[2];

	uap = (struct a *)u.u_ap;
	if ((ip = namei(uap->fname, SEGUDATA, &nilargnamei, 1)) == NULL)
		return;
	if (accowner(ip) == 0) {
		iput(ip);
		return;
	}
	if (copyin((caddr_t)uap->tptr, (caddr_t)tv, sizeof(tv))) {
		u.u_error = EFAULT;
	} else {
		ip->i_flag |= IACC|IUPD|ICHG;
		iupdat(ip, &tv[0], &tv[1], 0);
	}
	iput(ip);
}

/*
 * Setpgrp on specified process and its descendants.
 * Pid of zero implies current process.
 * Pgrp -1 is getpgrp system call returning
 * current process group.
 */
setpgrp()
{
	register struct proc *top;
	register struct a {
		int	pid;
		int	pgrp;
	} *uap;

	uap = (struct a *)u.u_ap;
	uap->pid = (short)uap->pid;	/* else 0x10000 would make pgrp 0 */
	uap->pgrp = (short)uap->pgrp;
	if (uap->pid == 0)
		top = u.u_procp;
	else {
		top = pfind(uap->pid);
		if (top == 0) {
			u.u_error = ESRCH;
			return;
		}
	}
	if (uap->pgrp == 0 && !suser())
		return;

	if (uap->pgrp < 0) {
		u.u_r.r_val1 = top->p_pgrp;
		return;
	}
	if (top->p_uid != u.u_uid && u.u_uid && !inferior(top))
		u.u_error = EPERM;
	else
		top->p_pgrp = uap->pgrp;
}

spgrp(top, npgrp)
register struct proc *top;
{
	register struct proc *pp, *p;
	int f = 0;

	for (p = top; npgrp == -1 || u.u_uid == p->p_uid ||
	    !u.u_uid || inferior(p); p = pp) {
		if (npgrp == -1) {
#define	bit(a)	(1<<(a-1))
			p->p_sig &= ~(bit(SIGTSTP)|bit(SIGTTIN)|bit(SIGTTOU));
		} else
			p->p_pgrp = npgrp;
		f++;
		/*
		 * Search for children.
		 */
		for (pp = proc; pp < procNPROC; pp++)
			if (pp->p_stat != 0 && pp->p_pptr == p)
				goto cont;
		/*
		 * Search for siblings.
		 */
		for (; p != top; p = p->p_pptr)
			for (pp = p + 1; pp < procNPROC; pp++)
				if (pp->p_stat != 0 && pp->p_pptr == p->p_pptr)
					goto cont;
		break;
	cont:
		;
	}
	return (f);
}

/*
 * Is p an inferior of the current process?
 */
inferior(p)
register struct proc *p;
{

	for (; p != u.u_procp; p = p->p_pptr)
		if (p < &proc[SYSPIDS])
			return (0);
	return (1);
}

sysboot()
{
	register struct a {
		int	opt;
	};

	if (suser())
		boot(((struct a *)u.u_ap)->opt);
}

/*
 * lock user into core as much
 * as possible. swapping may still
 * occur if core grows.
 */
syslock()
{
	register struct proc *p;
	register struct a {
		int	flag;
	} *uap;

	uap = (struct a *)u.u_ap;
	if(suser()) {
		p = u.u_procp;
		p->p_flag &= ~SULOCK;
		if(uap->flag)
			p->p_flag |= SULOCK;
	}
}

/*
 * nap for n clock ticks
 */
#define MAXNAP 120
nap()
{
        register struct a {
                int     nticks;
        } *uap;
        register int n;

        uap = (struct a *)u.u_ap;
        n = uap->nticks;
        if (n < 0)
                n = 0;
        if (n > MAXNAP)
                n = MAXNAP;
        delay (n);
}

/*
 * get/set user's login name
 */

getlogname()
{
	register struct a {
		char *name;
		int flag;
	} *uap;

	uap = (struct a *)u.u_ap;
	if (uap->flag == 0) {
		if (copyout((caddr_t)u.u_logname, (caddr_t)uap->name, sizeof(u.u_logname)) < 0)
			u.u_error = EFAULT;
		return;
	}
	if (suser() == 0)
		return;
	if (copyin((caddr_t)uap->name, (caddr_t)u.u_logname, sizeof(u.u_logname)))
		u.u_error = EFAULT;
}
