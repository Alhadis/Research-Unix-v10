/*
 *	Perform Standard I/O functions for
 * 	a remote process using the dkxqt protocol
 */
	static char	SCCSID[] = "@(#)dkxstdio.c	2.1 DKHOST 84/08/10";

#include "remfio.h"
#include <errno.h>
#include <signal.h>
#include "sysexits.h"
#include <setjmp.h>
#include <sys/ioctl.h>
#include "Vtermio.h"

#define	F_SGTTYB	"bbbbs"
#define	F_TERMIO	"ssssbbbbbbbbb"
#define	FL_SGTTYB	6
#define	FL_TERMIO	17

	static char		sb[REMSIZE], rb[REMSIZE];

	static struct rem_req	r;
	static struct rem_reply	s;

	/* extern int		dkverbose; */
	extern int		errno ;

#define BSIZE 1024

	static char		buf[BSIZE];

	static union {
		struct sgttyb	ioctl_sgttyb ;
		struct Vtermio	ioctl_termio ;
	} ioctl_buf ;

	static short		sig;
	static short		uSIG;
	static int		exitcode;

	static jmp_buf		wayout;
	static struct sgttyb	origterm;
	struct sgttyb		newterm ;
	static int		otfd;
	static int		eofmark;
	
	extern			dtSIG(), dtuSIG();

dkxstdio(fd)
{
	register int 	n;
	int		(*intwas)(), (*quitwas)(), (*usr1was)();

	exitcode = -EX_IOERR;	/* in case other side hangs up unexpectedly */

	eofmark = 0;

	if((intwas = signal(SIGINT, SIG_IGN)) != SIG_IGN)
		signal(SIGINT, dtSIG);
	if((quitwas = signal(SIGQUIT, SIG_IGN)) != SIG_IGN)
		signal(SIGQUIT, dtSIG);

	
	otfd = -1;


	for(n = 0; n < 3; n++)
		if (ioctl (n, TIOCGETP, &origterm) == 0) {
			otfd = n;
			break;
		}

	if(setjmp(wayout)){
		if(otfd >= 0)
			ioctl (otfd, TIOCSETP, &origterm) ;


		signal(SIGQUIT, quitwas);
		signal(SIGINT, intwas);
		return(exitcode);
	}

	while (1) {
		errno = 0 ;
		uSIG = 0 ;
		if ((read(fd, rb, REMSIZE) != REMSIZE) && (errno == 0)) {
			if (sig == 0)
				longjmp(wayout, 1);
			else
				errno = EINTR ;
		}
		if (errno == 0) {
			dkfcanon(F_REMREQ, rb, &r) ;
			switch(r.r_type) {
			case RREAD:
				dtread(fd) ;
				break ;
			case RWRITE:
				dtwrite(fd) ;
				break ;
			case RIOCTL:
				dtioctl(fd) ;
				if(eofmark)
					longjmp(wayout, 1);
				break ;
			case RCANCEL:
				break ;
			default:
				s.s_length = 0 ;
				s.s_error = EINVAL ;
				s.s_type = r.r_type ;
				dktcanon(F_REMREPLY, &s, sb) ;
				write(fd, sb, REMSIZE) ;
			}
		} else if (sig) {
			dtssig(fd) ;
		} else {
			/* if(dkverbose)
				perror("dkxstdio: dk read") ; */
			exitcode = -EX_PROTOCOL;
			longjmp(wayout, 1);
		}
	}
}

	static
dtread(fd)
{
	register int		rfd, temp;
	register unsigned	len, rlen;

	errno = 0 ;
	len = r.r_var.rread.r_count ;
	if (len > BSIZE)
		len = BSIZE ;
	rfd = (r.r_file < 3) ? r.r_file : 0;
	rlen = read(rfd, buf, len);
	if (sig)
		errno = EINTR ;
	temp = errno ;
	if ((int) rlen == -1)
		rlen = 0 ;
	if (sig)
		dtssig(fd) ;
	s.s_length = rlen ;
	s.s_type = RREAD ;
	s.s_error = temp ;
	if (rlen == len && len != r.r_var.rread.r_count && temp == 0)
		s.s_resid = r.r_var.rread.r_count - len ;
	else
		s.s_resid = 0 ;
	dktcanon(F_REMREPLY, &s, sb) ;
	write(fd, sb, REMSIZE) ;
	if (rlen)
		write(fd, buf, rlen) ;
}

	static
dtwrite(fd)
{
	register int		rfd, temp;
	register unsigned	len, rlen;
	unsigned		slen;

	slen = r.r_length ;
	while (len = r.r_length) {
		errno = 0 ;
		if (len > BSIZE)
			len = BSIZE ;
		rlen = read(fd, buf, len) ;
		if (rlen != len) {
			/* if(dkverbose)
				perror("dtwrite: dk read error") ; */
			exitcode = -EX_PROTOCOL;
			longjmp(wayout, 1);
		}
		r.r_length -= len ;
		rfd = (r.r_file < 3) ? r.r_file : 1;
		write(rfd, buf, len);
		if (sig) {
			dtssig(fd) ;
			dtdiscard(fd, r.r_length) ;
			r.r_length = 0 ;
		}
	}
	s.s_length = 0 ;
	s.s_type = RWRITE ;
	s.s_error = 0 ;
	s.s_count = slen ;
	dktcanon(F_REMREPLY, &s, sb) ;
	write(fd, sb, REMSIZE) ;
}

	static
dtioctl(fd)
{
	short need ;
	short give ;
	int narg ;
	char * fmt ;
	short fmtl ;
	
	fmtl = need = give = 0 ;
	narg = r.r_var.rioctl.r_arg ;
	switch (r.r_var.rioctl.r_cmd) {
	case TCSETAF:
	case TCSETAW:
	case TCSETA:
		need = sizeof (struct Vtermio) ;
		fmt = F_TERMIO ;
		fmtl = FL_TERMIO ;
		break ;
	case TIOCSETP:
		need = sizeof(struct sgttyb) ;
		fmt = F_SGTTYB ;
		fmtl = FL_SGTTYB ;
		break ;
	case TCGETA:
		give = sizeof(struct Vtermio) ;
		fmt = F_TERMIO ;
		fmtl = FL_TERMIO ;
		break ;
	case TIOCGETP:
		give = sizeof(struct sgttyb) ;
		fmt = F_SGTTYB ;
		fmtl = FL_SGTTYB ;
		break ;
	case (('D'<<8)|'T'):
		exitcode = narg;
		eofmark++;
	}
	if (need || give)
		narg = (int)&ioctl_buf ;
	if (need > r.r_length) {
		dtdiscard(fd, r.r_length) ;
		s.s_length = fmtl ;
		s.s_type = RIOCTL ;
		s.s_error = 0 ;
		s.s_count = need ;
		s.s_resid = fmtl ;
		dktcanon(F_REMREPLY, &s, sb) ;
		write(fd, sb, REMSIZE) ;
		if (fmtl) {
			write(fd, fmt, fmtl) ;
		}
		return ;
	}
	if (need) {
		read(fd, &ioctl_buf, need) ;
		dkfcanon(fmt, &ioctl_buf, &ioctl_buf) ;
	}
	dtdiscard(fd, r.r_length - need) ;
	errno = 0 ;
	if(!eofmark) {
		ioctl ((r.r_file <= 2)?r.r_file:0, TIOCGETP, &newterm) ;
		switch(r.r_var.rioctl.r_cmd) {
		case TCSETAF:
		case TCSETAW:
		case TCSETA:
			if (ioctl_buf.ioctl_termio.c_lflag & V__ECHO)
				newterm.sg_flags |= ECHO ;
			else
				newterm.sg_flags &= ~ECHO ;
			ioctl ((r.r_file <= 2)?r.r_file:0, TIOCSETP, &newterm) ;
			break ;
		case TIOCSETP:
			if (ioctl_buf.ioctl_sgttyb.sg_flags & VO_ECHO)
				newterm.sg_flags |= ECHO ;
			else
				newterm.sg_flags &= ~ECHO ;
			ioctl ((r.r_file <= 2)?r.r_file:0, TIOCSETP, &newterm) ;
			break ;
		case TIOCGETP:
			ioctl_buf.ioctl_sgttyb.sg_ispeed = newterm.sg_ispeed ;
			ioctl_buf.ioctl_sgttyb.sg_ospeed = newterm.sg_ospeed ;
			ioctl_buf.ioctl_sgttyb.sg_erase = newterm.sg_erase ;
			ioctl_buf.ioctl_sgttyb.sg_kill = newterm.sg_kill ;
			ioctl_buf.ioctl_sgttyb.sg_flags = VO_ECHO|VO_EVENP ;
			if ((newterm.sg_flags & ECHO) == 0)
				ioctl_buf.ioctl_sgttyb.sg_flags &= ~VO_ECHO ;
			break ;
		case TCGETA:
			ioctl_buf.ioctl_termio.c_iflag = V__BRKINT|V__IGNPAR|V__ISTRIP|V__ICRNL|V__IXON|V__IXANY ;
			ioctl_buf.ioctl_termio.c_oflag = V__OPOST|V__ONLCR|V__TAB3 ;
			ioctl_buf.ioctl_termio.c_cflag = V__B9600|V__CS7|V__CREAD|V__PARENB ;
			ioctl_buf.ioctl_termio.c_lflag = V__ISIG|V__ICANON ;
			if (newterm.sg_flags & ECHO)
				ioctl_buf.ioctl_termio.c_lflag |= V__ECHO|V__ECHOK ;
			break ;
		case (('X'<<8)|0):
			errno = EINVAL ;
			break ;
		}
	}
	s.s_length = give + fmtl ;
	s.s_type = RIOCTL ;
	s.s_error = errno ;
	s.s_count = 0 ;
	s.s_resid = fmtl ;
	dktcanon(F_REMREPLY, &s, sb) ;
	write(fd, sb, REMSIZE) ;
	if (fmtl) {
		write(fd, fmt, fmtl) ;
	}
	if (give) {
		dktcanon(fmt, &ioctl_buf, &ioctl_buf) ;
		write(fd, &ioctl_buf, give) ;
	}
}

	static
dtdiscard(fd, len)
	unsigned len ;
{
	register unsigned rlen ;

	while (len) {
		rlen = (len > BSIZE)? BSIZE : len ;
		errno = 0 ;
		rlen = read(fd, buf, rlen) ;
		len -= rlen ;
	}
}

	static
dtssig(fd)
{
	if (sig) {
		s.s_length = 0 ;
		s.s_type = RSIGNAL ;
		s.s_error = sig ;
		sig = 0 ;
		dktcanon(F_REMREPLY, &s, sb) ;
		write(fd, sb, REMSIZE) ;
	}
}

	static
dtSIG(signo)
{
	static long	lasttime;
	long	now, time();

	sig = signo ;
	signal(signo, dtSIG) ;
	if(signo == SIGQUIT){
		time(&now);
		if((now - lasttime) < 2){
			exitcode = -EX_NOINPUT;
			longjmp(wayout, 1);
		}
		lasttime = now;
	}
}

	static
dtuSIG(signo)
{
	uSIG++ ;
	signal(signo, dtuSIG) ;
}

xprint(buf, len)
register char *buf ;
{
	while (len) {
		printf("%o ", *buf++&0377) ;
		len-- ;
	}
	printf("\n") ;
}
