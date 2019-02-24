#include "ps.h"

char *
printp(cp, fd, pp, up)
register char *cp; register struct proc *pp; register struct user *up;
{
	char sstr[64], *args; int nargs = 56;
	register time;

	if (fd < 0)
		return cp;

	if (pp->p_flag & SPAGE)
		sstr[0] = 'P';
	else if (pp->p_stat == SSLEEP)
		sstr[0] = "IS"[pp->p_clktim && pp->p_clktim < 20];
	else
		sstr[0] = "?swRLZT?"[minmax(pp->p_stat, 0, 7)];
	sstr[1] = (pp->p_flag&SLOAD) ? ' ' : 'W';
	sstr[2] = " N"[pp->p_nice > NZERO];
	sstr[3] = 0;
	if (Tflag) {
		sstr[3] = ' ';
		strcpy(sstr+4,ctime(&up->u_start)+4);
		sstr[16] = 0;
		sprintf(sstr+16, " %x,%x", up->u_ttydev, up->u_ttyino);
	}
	time = (up->u_vm.vm_utime+up->u_vm.vm_stime)/60;

	if (uflag) {
		cp += sprintf(cp, "%-7.7s %4.1f ",
			getuname(pp->p_uid), 100.0*pp->p_pctcpu);
		nargs -= 13;
	}

	if (lflag) {
		cp += sprintf(cp, "%4d %4d %5d %8x ",
			(pp->p_dsize+pp->p_ssize)*NBPG/1024,
			pp->p_rssize*NBPG/1024,
			pp->p_ppid, (int)pp->p_wchan);
		nargs -= 25;
	}
	args = getargs(fd, pp, up);
	cp += sprintf(cp, "%5d %-5.5s %s %3d:%02d %.*s\n",
		pp->p_pid, gettty(up->u_ttydev, up->u_ttyino), sstr,
		time/60, time%60, nargs, args);

	if (fflag)
		cp = fdprint(cp, pp, up);

	close(fd);
	return cp;
}
