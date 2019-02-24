#include <stdio.h>
#include "ps.h"
#include <sys/text.h>
#include <sys/inode.h>
#include <sys/file.h>

#define Error(msg)	return (fprintf(stderr, "%s %s: %s\n", progname, s, msg), \
				close(fd), -1)
#define Reject		return (close(fd), -1)
#define Accept		return fd

doselect(s, pp, up)
char *s; register struct proc *pp; register struct user *up;
{
	char sstr[32], *error, *getuarea(); register Select *sp;
	register fd;

	up->u_procp = 0;

	if ((fd = open(strcat(strcpy(sstr, "/proc/"), s), 0)) < 0)
		return -1;
	if (!Ioctl(fd, PIOCGETPR, pp))
		Error("/proc ioctl error");

	if (!Nflag && !tflag && !Fflag) {
		if (!aflag && !rflag && pp->p_uid != myuid && pp->p_pid != mypid)
			Reject;
		if (error = getuarea(fd, pp, up))
			Error(error);
		if (!aflag && rflag && up->u_uid != myuid && up->u_ruid != myuid)
			Reject;
		if (!xflag && xreject(pp, up))
			Reject;
		Accept;
	}
	for (sp=selbeg; sp<selend; sp++) {
		if (sp->flag & SELTTY) {
			if (error = getuarea(fd, pp, up))
				Error(error);
			if (strncmp(sp->id, gettty(up->u_ttydev, up->u_ttyino), 5))
				continue;
			if (!(sp->flag & SELXFL) && xreject(pp, up))
				continue;
			Accept;
		} else if (sp->flag & SELFIL) {
			if (error = getuarea(fd, pp, up))
				Error(error);
			if (fdselect(sp->dev, sp->ino, pp, up))
				Accept;
		} else if (atoi(sp->id) == pp->p_pid) {
			if (error = getuarea(fd, pp, up))
				Error(error);
			Accept;
		}
	}
	Reject;
}

char *
getuarea(fd, pp, up)
register struct proc *pp; register struct user *up;
{
	extern errno;
	if (up->u_procp)
		return 0;
	if (pp->p_flag & SLOAD) {
		register ntry = 4;
		do
			if (Sread(fd, UBASE, up))
				return 0;
		while (errno == EBUSY && --ntry > 0);
		return "/proc read error";
	}
	if (Sread(drum, pp->p_swaddr*NBPG, up))
		return 0;
	return "drum read error";
}

xreject(pp, up)
register struct proc *pp; register struct user *up;
{
	return (pp->p_stat != SSTOP &&
		(strcmp(up->u_comm, "sh") == 0 ||
		 strcmp(up->u_comm, "csh") == 0 ||
		 strcmp(up->u_comm, "init") == 0 ||
		 up->u_comm[0] == 0));
}

fdselect(dev, ino, pp, up)
register struct proc *pp; register struct user *up;
{
	register k;
	struct text x; struct file f; struct inode i;

	if (Kread(pp->p_textp, &x) && Kread(x.x_iptr, &i) &&
	    dev == i.i_dev && ino == i.i_number)
		return 1;
	if (Kread(up->u_cdir, &i) && dev == i.i_dev && ino == i.i_number)
		return 1;
	if (Kread(up->u_rdir, &i) && dev == i.i_dev && ino == i.i_number)
		return 1;

	for (k = 0; k < NOFILE; k++) {
		if (!Kread(up->u_ofile[k], &f) || !Kread(f.f_inode, &i))
			continue;
		if (dev == i.i_dev && ino == i.i_number)
			return 1;
	}
	return 0;
}
