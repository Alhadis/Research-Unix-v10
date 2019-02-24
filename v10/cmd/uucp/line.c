/*	%W%
*/
#include "uucp.h"
VERSION(%W%);

#ifdef V8
#include <sys/ttyio.h>
#else
#include <sgtty.h>
#endif

static struct sg_spds {
	int	sp_val,
		sp_name;
} spds[] = {
	{ 300,  B300},
	{ 600,  B600},
	{1200, B1200},
	{2400, B2400},
	{4800, B4800},
	{9600, B9600},
#ifdef EXTA
	{19200,	EXTA},
#endif
#ifdef B19200
	{19200,	B19200},
#endif
#ifdef B38400
	{38400,	B38400},
#endif
	{0,    0}
};

#define PACKSIZE	64
#define HEADERSIZE	6
#define SNDFILE	'S'
#define RCVFILE 'R'
#define RESET	'X'

int	linebaudrate = 0;	/* for speedup hook in pk (unused in ATTSV) */

#ifdef ATTSV

static struct termio Savettyb;
/*
 * set speed/echo/mode...
 *	tty 	-> terminal name
 *	spwant 	-> speed
 *	type	-> type
 *
 *	if spwant == 0, speed is untouched
 *	type is unused, but needed for compatibility
 *
 * return:  
 *	none
 */
fixline(tty, spwant, type)
int	tty, spwant, type;
{
	register struct sg_spds	*ps;
	struct termio		ttbuf;
	int			speed = -1;

	DEBUG(6, "fixline(%d, ", tty);
	DEBUG(6, "%d)\n", spwant);
	if (ioctl(tty, TCGETA, &ttbuf) != 0)
		return;
	if (spwant > 0) {
		for (ps = spds; ps->sp_val; ps++)
			if (ps->sp_val == spwant) {
				speed = ps->sp_name;
				break;
			}
		ASSERT(speed >= 0, "BAD SPEED", "", speed);
		ttbuf.c_cflag = speed;
	} else
		ttbuf.c_cflag &= CBAUD;
	ttbuf.c_iflag = ttbuf.c_oflag = ttbuf.c_lflag = (ushort)0;

#ifdef NO_MODEM_CTRL
	/*   CLOCAL may cause problems on pdp11s with DHs */
	if (type == D_DIRECT) {
		DEBUG(4, "fixline - direct\n", "");
		ttbuf.c_cflag |= CLOCAL;
	} else
#endif NO_MODEM_CTRL

		ttbuf.c_cflag &= ~CLOCAL;
	ttbuf.c_cflag |= (CS8 | CREAD | (speed ? HUPCL : 0));
	ttbuf.c_cc[VMIN] = HEADERSIZE;
	ttbuf.c_cc[VTIME] = 1;
	
	ASSERT(ioctl(tty, TCSETA, &ttbuf) >= 0,
	    "RETURN FROM fixline ioctl", "", errno);
	return;
}

sethup(dcf)
int	dcf;
{
	struct termio ttbuf;

	if (ioctl(dcf, TCGETA, &ttbuf) != 0)
		return;
	if (!(ttbuf.c_cflag & HUPCL)) {
		ttbuf.c_cflag |= HUPCL;
		(void) ioctl(dcf, TCSETA, &ttbuf);
	}
}

genbrk(fn)
register int	fn;
{
	if (isatty(fn)) 
		(void) ioctl(fn, TCSBRK, 0);
}


/*
 * optimize line setting for sending or receiving files
 * return:
 *	none
 */
setline(type)
register char	type;
{
	static struct termio tbuf;
	
	if (ioctl(Ifn, TCGETA, &tbuf) != 0)
		return;
	DEBUG(2, "setline - %c\n", type);
	switch (type) {
	case RCVFILE:
		if (tbuf.c_cc[VMIN] != PACKSIZE) {
		    tbuf.c_cc[VMIN] = PACKSIZE;
		    (void) ioctl(Ifn, TCSETAW, &tbuf);
		}
		break;

	case SNDFILE:
	case RESET:
		if (tbuf.c_cc[VMIN] != HEADERSIZE) {
		    tbuf.c_cc[VMIN] = HEADERSIZE;
		    (void) ioctl(Ifn, TCSETAW, &tbuf);
		}
		break;
	}
}

savline()
{
	int ret;

	ret = ioctl(0, TCGETA, &Savettyb);
	Savettyb.c_cflag = (Savettyb.c_cflag & ~CS8) | CS7;
	Savettyb.c_oflag |= OPOST;
	Savettyb.c_lflag |= (ISIG|ICANON|ECHO);
	return(ret);
}

/***
 *	sytfixline(tty, spwant)	set speed/echo/mode...
 *	int tty, spwant;
 *
 *	return codes:  none
 */

sytfixline(tty, spwant)
int tty, spwant;
{
	struct termio ttbuf;
	struct sg_spds *ps;
	int speed = -1;
	int ret;

	for (ps = spds; ps->sp_val >= 0; ps++)
		if (ps->sp_val == spwant)
			speed = ps->sp_name;
	DEBUG(4, "sytfixline - speed= %d\n", speed);
	ASSERT(speed >= 0, "BAD SPEED", "", speed);
	(void) ioctl(tty, TCGETA, &ttbuf);
	ttbuf.c_iflag = (ushort)0;
	ttbuf.c_oflag = (ushort)0;
	ttbuf.c_lflag = (ushort)0;
	ttbuf.c_cflag = speed;
	ttbuf.c_cflag |= (CS8|CLOCAL);
	ttbuf.c_cc[VMIN] = 6;
	ttbuf.c_cc[VTIME] = 1;
	ret = ioctl(tty, TCSETAW, &ttbuf);
	ASSERT(ret >= 0, "RETURN FROM sytfixline", "", ret);
	return;
}

sytfix2line(tty)
int tty;
{
	struct termio ttbuf;
	int ret;

	(void) ioctl(tty, TCGETA, &ttbuf);
	ttbuf.c_cflag &= ~CLOCAL;
	ttbuf.c_cflag |= CREAD|HUPCL;
	ret = ioctl(tty, TCSETAW, &ttbuf);
	ASSERT(ret >= 0, "RETURN FROM sytfix2line", "", ret);
	return;
}


restline()
{
	return(ioctl(0, TCSETA, &Savettyb));
}

#else

static struct sgttyb Savettyb;
#ifdef V8
static struct ttydevb Savedevb;
#endif

/***
 *	fixline(tty, spwant, type)	set speed/echo/mode...
 *	int tty, spwant;
 *
 *	if spwant == 0, speed is untouched
 *	type is unused, but needed for compatibility
 *
 *	return codes:  none
 */

/*ARGSUSED*/
fixline(tty, spwant, type)
int tty, spwant, type;
{
	struct sgttyb	ttbuf;
#ifdef V8
	struct ttydevb	dvbuf;
#endif
	struct sg_spds	*ps;
	int		 speed = -1;

	DEBUG(6, "fixline(%d, ", tty);
	DEBUG(6, "%d)\n", spwant);

	ioctl(tty, TIOCGETP, &ttbuf);
#ifdef V8
	if (ioctl(tty, TIOCGDEV, &dvbuf) < 0) {
		dvbuf.ospeed = ttbuf.sg_ospeed;
		dvbuf.ispeed = ttbuf.sg_ispeed;
	}
#endif
	if (spwant > 0) {
		for (ps = spds; ps->sp_val; ps++)
			if (ps->sp_val == spwant) {
				speed = ps->sp_name;
				break;
			}
		ASSERT(speed >= 0, "BAD SPEED", "", speed);
#ifdef V8
		dvbuf.ispeed = dvbuf.ospeed = speed;
#endif
		ttbuf.sg_ispeed = ttbuf.sg_ospeed = speed;
	} else {
		for (ps = spds; ps->sp_val; ps++)
			if (ps->sp_name == dvbuf.ispeed) {
				spwant = ps->sp_val;
				break;
			}
		ASSERT(spwant >= 0, "BAD SPEED", "", spwant);
	}
	ttbuf.sg_flags &=~ ECHO;
	ttbuf.sg_flags |= RAW|ANYP;
	(void) ioctl(tty, TIOCSETP, &ttbuf);
#ifdef V8
	dvbuf.flags |= F8BIT;
	(void) ioctl(tty, TIOCSDEV, &dvbuf);
#endif
	(void) ioctl(tty, TIOCHPCL, STBNULL);
	(void) ioctl(tty, TIOCEXCL, STBNULL);
	linebaudrate = spwant;		/* for hacks in pk driver */
	return;
}

sethup(dcf)
int	dcf;
{
	if (isatty(dcf)) 
		(void) ioctl(dcf, TIOCHPCL, STBNULL);
}

/***
 *	genbrk		send a break
 *
 *	return codes;  none
 */

genbrk(fn)
{
	(void) ioctl(fn, TIOCSBRK, 0);
#ifndef V8
	nap(HZ/10);				/* 0.1 second break */
	(void) ioctl(fn, TIOCCBRK, 0);
#endif
}

/*
 * V7 and RT aren't smart enough for this -- linebaudrate is the best
 * they can do.
 */
/*ARGSUSED*/
setline(dummy) { }

savline()
{
	int	ret;

	ret = ioctl(0, TIOCGETP, &Savettyb);
	Savettyb.sg_flags |= ECHO;
	Savettyb.sg_flags &= ~RAW;
#ifdef V8
	ioctl(0, TIOCGDEV, &Savedevb);
#endif
	return(ret);
}

restline()
{
	if (ioctl(0, TIOCSETP, &Savettyb) < 0)
		return (-1);
#ifdef V8
	if (ioctl(0, TIOCSDEV, &Savedevb) < 0)
		return (-1);
#endif
	return (0);
}
#endif
