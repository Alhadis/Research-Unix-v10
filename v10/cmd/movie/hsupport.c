#include <stdio.h>

#ifdef SYS_V
#include <termio.h>
struct termio tty, svtty;
#else
#include <sgtty.h>
struct sgttyb tty, svtty;
#endif

extern int dbg;

char *emalloc(n)
{
	char *p, *calloc();

	p = calloc(n, 1);
	assert(p != NULL, "out of space in emalloc");
	return p;
}

sendbuf(buf, p)	/* write buf..buf+p-1 to jerq */
	char *buf, *p;
{
	write(1, buf, p-buf);
}

assert(c, s1, s2, s3)	/* poor man's assertion */
	char *s1, *s2, *s3;
{
	if (!c) {
		fprintf(stderr, "assertion failed: ");
		fprintf(stderr, s1, s2, s3);
		fprintf(stderr, "\n");
		reset_tty();
		abort();
	}
}

readbyte()	/* receive one byte */
{
	char buf[2];

 	read(0, buf, 1);
	return buf[0];
}

set_tty()	/* set jerq terminal mode */
{
	if (dbg)
		return;
#ifdef SYS_V
	ioctl(1, TCGETA, &tty);
	svtty = tty;
	tty.c_iflag = IGNBRK;
	tty.c_iflag &= ~(INLCR|IGNCR|ICRNL|IGNPAR|PARMRK);
	tty.c_cflag = (tty.c_cflag & (CBAUD|CLOCAL))| CS8|CREAD;
	tty.c_cc[VMIN] = 1;
	tty.c_lflag &= ~(ECHO|ICANON|ECHOK|ECHONL|ECHOE);
	tty.c_oflag &= ~(ONLCR|OCRNL|ONOCR|ONLRET|OFILL);
	ioctl(1, TCSETAW, &tty);
#else
	ioctl(1, TIOCGETP, &tty);
	svtty = tty;
	tty.sg_flags |= RAW;
	tty.sg_flags &= ~ECHO;
	ioctl(1, TIOCSETN, &tty);
#endif
}

reset_tty()
{
	if (dbg)
		return;
#ifdef SYS_V
	ioctl(1, TCSETAW, &svtty);
#else
	ioctl(1, TIOCSETP, &svtty);	/* SETN??? */
#endif
}
