#include "sys/param.h"
#include "sys/stream.h"
#include "sys/ttyio.h"
#include "sys/nttyio.h"
#include "sys/ttyld.h"
#include "sys/nttyld.h"
#include "sys/conf.h"
#include "sys/file.h"

extern char	maptab[],	/* see dev/ttyld.c */
		partab[];	/* see sys/partab.c */


extern struct nttyld ntty[];
extern int nttycnt;

int	ntclose(), 	/* queue close routine */
	ntin(), 	/* reader queue put routine */
	ntinsrv(), 	/* reader queue service routine */
	ntout(),	/* writer queue put routine */
	ntoutsrv();	/* writer queue service routine */
long	ntopen(); 	/* queue open routine */

static struct qinit	rinit = { ntin, ntinsrv, ntopen, ntclose, 300, 0 },
			winit = { ntout, ntoutsrv, ntopen, ntclose, 300, 200 };
struct streamtab	nttystream = { &rinit, &winit };

#define	CANBSIZ	256	/* why not NT_NIN? */

/*
 * Backspace over "n" characters, perhaps erasing them.
 * Called from ntrubout().
 */

ntbs (nt, n)
	register struct nttyld	*nt;
	register int		n;
{
	while (--n >= 0)
		if (putd (putq, nt->nt_outq, '\b')) {
			nt->nt_state |= NT_ECHO;
			if (nt->nt_local & LCRTERA) {
				(void) putd (putq, nt->nt_outq, ' ');
				(void) putd (putq, nt->nt_outq, '\b');
			}
		}
	return;
}


/*
 * Set default control characters.
 * Called by ntopen().
 */

ntchars (nt)
	register struct nttyld	*nt;
{
	static struct tchars	tchars = { CINTR, CQUIT, CSTART, CSTOP, CEOT, 
				    0377 };
	static struct ltchars	ltchars = { CSUSP, CDSUSP, CRPRNT, CFLUSH,
				    CWERAS, CLNEXT };

	nt->nt_erase = CERASE;
	nt->nt_kill = CKILL;
	nt->nt_tchr = tchars;
	nt->nt_ltchr = ltchars;
	return;
}


ntclose (q)
	register struct queue	*q;
{
	register struct nttyld	*nt = (struct nttyld *) q->ptr;

	nt->nt_state = 0;
	return;
}


/*
 * Put input buffer on read queue followed by a delimiter.
 * Called from ntinc() when a line break character is read,
 * and from ntioctl() when switching from cooked to raw or cbreak mode.
 */

ntcooked (nt)
	register struct nttyld	*nt;
{
	register char		*s, *t;
	register struct block	*b;
	register struct queue	*q = RD (nt->nt_outq);

	if (!(q->flag & QFULL) && (b = allocb (1))) {
		for (t = (s = nt->nt_in) + nt->nt_nin; s < t; s++)
			(void) putd (putq, q, *s);
		b->class |= S_DELIM;
		putq (q, b);
		nt->nt_delct++;
	}
	qenable (q);
	nt->nt_nin = 0;
	nt->nt_trash = 0;
	return;
}


/*
 * Echo a typed character to a terminal.
 */

ntecho (c, nt)
	register int		c;
	register struct nttyld	*nt;
{
	register struct queue	*echoq = nt->nt_outq;

	nt->nt_local &= ~LFLUSHO;
	if (!(nt->nt_flags & ECHO) || echoq->flag & QFULL)
		return;
	c &= 0377;
	if (nt->nt_flags & RAW)
		goto out;
	if (c == '\r' && nt->nt_flags & CRMOD)
		c = '\n';
	if (c != '\n' && c != '\t' && nt->nt_local & LCTLECH)
		if ((c &= 0177) <= 037 || c == 0177) {
			(void) putd (putq, echoq, '^');
			if (c == 0177)
				c = '?';
			else if (nt->nt_flags & LCASE)
				c += 'a' - 1;
			else	c += 'A' - 1;
			goto out;
		}

	if ((c &= 0177) == CEOT)	/* terminals don't like it */
		return;

out:
	if (putd (putq, echoq, c))
		nt->nt_state |= NT_ECHO;
	return;
}


/*
 * Flush all input and/or all output tty queues.
 */

ntflush (nt, rw)
	struct nttyld	*nt;
	int		rw;
{
	struct queue	*wrq = nt->nt_outq, 
			*rdq = RD (wrq);

	if (rw & FREAD) {
		flushq (rdq, 0);
		(void) putctl (rdq->next, M_FLUSH);
		nt->nt_delct = 0;
		nt->nt_nin = 0;
		nt->nt_trash = 0;
		nt->nt_lstate = 0;
	}
	if (rw & FWRITE) {
		flushq (wrq, 0);
		(void) putctl (wrq->next, M_FLUSH);
	}
	return;
}


/*
 * Reader queue (input from tty) put routine.
 */

ntin (q, b)
	struct queue		*q;
	register struct block	*b;
{
	register struct nttyld	*nt = (struct nttyld *) q->ptr;

	switch (b->type) {
	case M_DATA:
		if (nt->nt_flags & RAW && !(nt->nt_flags & ECHO))
			break;
		nt->nt_state &= ~NT_ECHO;
		while (b->rptr < b->wptr)
			ntinc ((int) *b->rptr++, nt);
		freeb (b);
		if (nt->nt_state & NT_ECHO && nt->nt_outq->next->flag & QDELIM)
			(void) qpctld (nt->nt_outq, M_DATA);
		return;
	case M_BREAK:
		if (nt->nt_flags & RAW) {		/* speed-change hack */
			b->type = M_DATA;
			if (b->wptr < b->lim)
				*b->wptr++ = '\0';
			break;
		}
		(void) putctl1 (q->next, M_SIGNAL, SIGINT);
		ntflush (nt, FREAD | FWRITE);
		freeb (b);
		return;
	case M_HANGUP:
		(*q->next->qinfo->putp)(q->next, b);
		return;
	}

	if (q->next->flag & QFULL)
		freeb (b);
	else	(*q->next->qinfo->putp)(q->next, b);
	return;
}


/*
 * Process a single input character.
 * Called (at interrupt level) by ntin().
 */

ntinc (c, nt)
	register 		c;
	register struct nttyld	*nt;
{
	register int	t_flags;

	c &= 0377;
	t_flags = nt->nt_flags;

	if (t_flags & RAW) {
		if (!(nt->nt_readq->next->flag & QFULL))
			if (putd (nt->nt_readq->next->qinfo->putp, nt->nt_readq->next, c))
				ntecho (c, nt);
		return;
	}

	if (!(nt->nt_lstate & LSTYPEN))
		c &= 0177;

	/*
	 * Interpret literal-next-character control character (^V).
	 */
	if (nt->nt_lstate & LSLNCH) {
		c |= 0200;
		nt->nt_lstate &= ~LSLNCH;
	}
	if (c == nt->nt_lnextc) {
		nt->nt_lstate |= LSLNCH;
		if (nt->nt_flags & ECHO)
			if (putd (putq, nt->nt_outq, '^')) {
				nt->nt_state |= NT_ECHO;
				(void) putd (putq, nt->nt_outq, '\b');
			}
		return;
	}

	if (ntstst (c, nt) ||		/* start, stop */
	    ntoflush (c, nt) ||		/* flush output */
	    ntsigc (c, nt))		/* interrupt/quit/stop */
		return;

	if (c == '\r' && t_flags & CRMOD)
		c = '\n';
	if (t_flags & LCASE && c >= 'A' && c <= 'Z')
		c += 'a' - 'A';

	if (t_flags & CBREAK) {
		if (nt->nt_readq->next->flag & QFULL) {
			if (putd (putq, nt->nt_outq, CTRL('g')))
				nt->nt_state |= NT_ECHO;
		}
#ifdef notdef
		else if (c == nt->nt_dsuspc)
			(void) putctl1 (nt->nt_readq->next, M_SSIGNAL, SIGTSTP);
#endif
		else if (putd (nt->nt_readq->next->qinfo->putp, nt->nt_readq->next, c&0177))
			ntecho (c, nt);
		return;
	}

	if (nt->nt_lstate & LSQUOT) {
		nt->nt_lstate &= ~LSQUOT;
		if (c == nt->nt_erase || c == nt->nt_kill) {
			c |= 0200;
			ntrubout (nt);
		}
	}
	if (c == '\\')
		nt->nt_lstate |= LSQUOT;

	if (c == nt->nt_erase)
		ntrubout (nt);		/* erase character */
	else if (c == nt->nt_kill)
		ntkill (nt);		/* erase line */
	else if (c == nt->nt_werasc)
		ntwerase (nt);		/* erase word */
	else if (c == nt->nt_rprntc)
		ntreprint (nt);		/* reprint line */
	else {
		if (nt->nt_nin < NT_NIN) {
			if (nt->nt_nin == 0)
				nt->nt_rocol = nt->nt_col;
			nt->nt_in[nt->nt_nin++] = c;
		}
		else {
			if (putd (putq, nt->nt_outq, CTRL('g')))
				nt->nt_state |= NT_ECHO;
			return;
		}

		if (c == '\n' || c == nt->nt_eofc || c == nt->nt_brkc ||
		    c == '\r' && (nt->nt_flags & CRMOD))
			ntcooked (nt);

		if (nt->nt_lstate & LSERASE) {
			nt->nt_lstate &= ~LSERASE;
			if (putd (putq, nt->nt_outq, '/'))
				nt->nt_state |= NT_ECHO;
		}

		ntecho (c, nt);
		if (c == nt->nt_eofc && nt->nt_flags & ECHO && 
		    nt->nt_local & LCTLECH)
			if (putd (putq, nt->nt_outq, '\b')) {
				nt->nt_state |= NT_ECHO;
				(void) putd (putq, nt->nt_outq, '\b');
			}
	}
	return;
}


/*
 * Reader queue (tty input) service routine.
 * Only cooked mode data and delimiters come through here, via ntcooked().
 */

ntinsrv (q)
	register struct queue	*q;
{
	register struct nttyld	*nt;
	register struct block	*b;
	register char		*op;
	register int		c;
	static char		canonb[CANBSIZ];

	if (q->next->flag & QFULL)
		return;
	nt = (struct nttyld *) q->ptr;
	op = canonb;
	while (nt->nt_delct) {
		b = getq(q);
		if (b == NULL) {
			if (op > canonb)
				putcpy (q->next, canonb, op - canonb);
			return;
		}
		while (b->rptr < b->wptr) {
			c = *b->rptr++;
			if (c & 0200) {		/* escaped */
				c &= 0177;
				if (nt->nt_flags & LCASE && maptab[c])
					c = maptab[c];
				else if (c != nt->nt_erase &&
				       c != nt->nt_kill && c != nt->nt_eofc)
					*op++ = '\\';
			}
			else {
#ifdef notdef
				/*
				 * Interpret delayed stop process
				 * control character (^Y).
				 */
				if (c == nt->nt_dsuspc) {
					putcpy (q->next, canonb, op - canonb);
					op = canonb;
					(void) putctl1 (q->next, M_SSIGNAL, 
					    SIGTSTP);
					continue;
				}
#endif
				if (c == nt->nt_eofc)
					continue;
			}
			*op++ = c;
			if (op >= &canonb[CANBSIZ-1]) {
				putcpy (q->next, canonb, op - canonb);
				op = canonb;
			}
		}
		if ((b->class & S_DELIM) == 0)
			freeb(b);
		else {		/* end of line */
			if (op > canonb)
				putcpy(q->next, canonb, op - canonb);
			(*q->next->qinfo->putp)(q->next, b);	/* empty S_DELIM */
			nt->nt_delct--;
			op = canonb;
		}
	}

	return;
}


/*
 * Acknowledge ioctl message.
 * Called by ntioctl().
 */

ntiocack (q, b, n)
	register struct queue	*q;
	register struct block	*b;
	register int		n;
{
	if (n > 0)
		n += sizeof (int);	/* for ioctl command */
	b->wptr = b->rptr + n;
	b->type = M_IOCACK;
	qreply (q, b);
	return;
}


/*
 * Terminal I/O control operations.
 * Called by ntoutsrv().
 */

ntioctl (q, b)
	register struct queue	*q;
	register struct block	*b;
{
	register struct nttyld	*nt = (struct nttyld *) q->ptr;
	register struct ntioc	*ioc = (struct ntioc *) b->rptr;
	int			s;

	switch (ioc->command) {

	/*
	 * Set new parameters
	 */
	case TIOCSETP:
	case TIOCSETN:
		s = spl6();
		if (ioc->arg.sg.sg_flags & (RAW|CBREAK) &&
		    !(nt->nt_flags & (RAW|CBREAK))) {
			/*
			 * Before leaving cooked mode, push through
			 * any characters that made it to ntin().
			 */
			if (nt->nt_nin > 0)
				ntcooked (nt);
			ntinsrv (RD (nt->nt_outq));
		}
		nt->nt_erase = ioc->arg.sg.sg_erase;
		nt->nt_kill = ioc->arg.sg.sg_kill;
		nt->nt_flags = ioc->arg.sg.sg_flags;
		nt->nt_readq->flag &= ~QDELIM;
		if ((nt->nt_flags & (RAW|CBREAK))==0)
			nt->nt_readq->flag |= QDELIM;
		splx (s);

		ntiocack (q, b, 0);
		break;

	/*
	 * Send current parameters to user
	 */
	case TIOCGETP:
		ioc->arg.sg.sg_erase = nt->nt_erase;
		ioc->arg.sg.sg_kill = nt->nt_kill;
		ioc->arg.sg.sg_flags = nt->nt_flags;
		ioc->arg.sg.sg_ispeed =
		 ioc->arg.sg.sg_ospeed = B9600;

		b->wptr = b->rptr + sizeof (int) + sizeof (struct sgttyb);
		b->wptr = 2 + b->rptr + sizeof (int) + sizeof (struct sgttyb);
		ntiocack (q, b, sizeof (struct sgttyb));
		break;

	/*
	 * Set/get special characters
	 */
	case TIOCSETC:
		nt->nt_tchr = ioc->arg.tchr;
		ntiocack (q, b, 0);
		break;

	case TIOCGETC:
		ioc->arg.tchr = nt->nt_tchr;
		ntiocack (q, b, sizeof (struct tchars));
		break;

	/*
	 * Set/get local special characters.
	 */
	case TIOCSLTC:
		nt->nt_ltchr = ioc->arg.ltchr;
		ntiocack (q, b, 0);
		break;

	case TIOCGLTC:
		ioc->arg.ltchr = nt->nt_ltchr;
		ntiocack (q, b, sizeof (struct ltchars));
		break;

	/*
	 * Modify local mode word.
	 */
	case TIOCLBIS:
		nt->nt_local |= ioc->arg.local;
		ntiocack (q, b, 0);
		break;

	case TIOCLBIC:
		nt->nt_local &= ~ioc->arg.local;
		ntiocack (q, b, 0);
		break;

	case TIOCLSET:
		nt->nt_local = ioc->arg.local;
		ntiocack (q, b, 0);
		break;

	case TIOCLGET:
		ioc->arg.local = nt->nt_local;
		ntiocack (q, b, sizeof (short));
		break;

	/*
	 * Return number of characters in the output.
	 */
	case TIOCOUTQ:

	default:
		(*q->next->qinfo->putp)(q->next, b);
		break;
	}

	return;
}


/*
 * Erase entire input buffer.
 * Called by ntinc().
 */

ntkill (nt)
	register struct nttyld	*nt;
{
	if (nt->nt_local & LCRTKIL && !nt->nt_trash) {
		while (nt->nt_nin > 0)
			ntrubout (nt);
	}
	else {
		ntecho (nt->nt_kill, nt);
		ntecho ('\n', nt);
		nt->nt_nin = 0;
		nt->nt_trash = 0;
	}
	nt->nt_lstate = 0;
	return;
}


/*
 * Interpret the flush-output (^O) control character.
 * Called by ntinc().
 */

int
ntoflush (c, nt)
	register int		c;
	register struct nttyld	*nt;
{
	if (nt->nt_local & LFLUSHO) {
		nt->nt_local &= ~LFLUSHO;
		if (c == nt->nt_flushc)
			return (1);
	}
	else	if (c == nt->nt_flushc) {
			ntflush (nt, FWRITE);
			ntecho (c, nt);
			ntreprint (nt);
			nt->nt_local |= LFLUSHO;
			return (1);
		}

	return (0);
}


/*ARGSUSED*/

long
ntopen (q, dev)
	register struct queue	*q;
	int			dev;
{
	register struct nttyld	*nt;

	if (q->ptr)
		return (1);		/* already attached */

	for (nt = ntty; nt->nt_state & NT_USE; nt++)
		if (nt >= &ntty[nttycnt-1])
			return (0);

	WR (q)->ptr = q->ptr = (caddr_t) nt;
	nt->nt_outq = WR (q);
	nt->nt_readq = q;
	nt->nt_nin = 0;
	nt->nt_state = NT_USE;
	nt->nt_flags = ECHO | CRMOD;
	nt->nt_col = 0;
	nt->nt_delct = 0;
	ntchars (nt);			/* set default control characters */
	nt->nt_trash = 0;
	nt->nt_local = 0;
	nt->nt_lstate = 0;
	q->flag |= QDELIM;
	return (1);
}


/*
 * Writer queue (output to tty) put routine.
 */

ntout (wrq, b)
	register struct queue	*wrq;
	register struct block	*b;
{
	if (b->type == M_DATA)
		b->type = M_CTL;		/* see ntoutsrv() */
	putq (wrq, b);
	return;
}


/*
 * Output a block of data to a terminal.
 * Handle tab expansion, case conversion, turning CR to CRLF, delays, etc.
 * Called from ntoutsrv().
 */

ntoutb (nt, ib)
	register struct nttyld	*nt;
	register struct block	*ib;
{
	register struct block	*ob = 0;
	register struct queue	*q = nt->nt_outq;
	register int		c, delay, ctype;
	char			*colp;

	while (ib->rptr < ib->wptr) {
		if (!ob || ob->wptr >= ob->lim) {
			if (ob) {
				if (nt->nt_local & LFLUSHO)
					break;
				(*q->next->qinfo->putp)(q->next, ob);
			}
			if (q->next->flag & QFULL || !(ob = allocb (QBSIZE))) {
				putbq (q, ib);
				return;
			}
		}

		switch (c = *ib->rptr++ & 0177) {
		case '\t':
			if ((nt->nt_flags & TBDELAY) != XTABS)
				break;

			/*
			 * Expand tabs to spaces.
			 */
			for (;;) {
				*ob->wptr++ = ' ';
				nt->nt_col++;
				if ((nt->nt_col & 07) == 0)	/* every 8 */
					break;
				if (ob->wptr >= ob->lim) {
					ib->rptr--;
					break;
				}
			}
			continue;
		case '\n':
			if (!(nt->nt_flags & CRMOD))
				break;

			/*
			 * Turn <nl> to <cr><lf>.
			 */
			if (nt->nt_state & NT_CR)
				nt->nt_state &= ~NT_CR;
			else {
				nt->nt_state |= NT_CR;
				c = '\r';
				ib->rptr--;
			}
			break;
		case '~':
			if (nt->nt_local & LTILDE)
				c = '`';
			/* no break */
		default:
			if (!(nt->nt_flags & LCASE))
				break;

			/*
			 * Generate escapes for upper-case-only terminals.
			 */
			if (nt->nt_state & NT_CASE) {
				nt->nt_state &= ~NT_CASE;
				break;
			}

			for (colp = "{(})|!~^`'"; *colp && c != *colp; 
			    colp += 2);

			if (*colp || c >= 'A' && c <= 'Z') {
				*--ib->rptr = *colp ? colp[1] : c;
				nt->nt_state |= NT_CASE;
				c = '\\';
			}
			else if (c >= 'a' && c <= 'z')
				c += 'A' - 'a';
			break;
		}

		/*
		 * Store character.
		 */
		*ob->wptr++ = c;

		/*
		 * Calculate delays and column movement.
		 * The delay values are in clock ticks and aren't
		 * necessarily optimal for all terminals.
		 */
		delay = 0;

		switch (ctype = partab[c] & 077) {
		case ORDINARY:
			nt->nt_col++;
			break;
		case CONTROL:
			break;
		case BACKSPACE:
			if (nt->nt_col)
				nt->nt_col--;
			break;
		case NEWLINE:
			ctype = (nt->nt_flags >> 8) & 03;
			if (ctype == 1) {	/* tty 37 */
				if (nt->nt_col)
					if ((delay = nt->nt_col >> 4) < 6)
						delay = 6;
			}
			else if (ctype == 2)	/* vt05 */
				delay = 6;
			if (!(nt->nt_flags & CRMOD))
				nt->nt_col = 0;
			break;
		case TAB:
			ctype = (nt->nt_flags >> 10) & 03;
			if (ctype == 1)		/* tty 37 */
				if ((delay = 1 - (nt->nt_col | ~07)) < 5)
					delay = 0;
			nt->nt_col |= 07;
			nt->nt_col++;
			break;
		case VTAB:
			if (nt->nt_flags & VTDELAY)
				delay = 127;
			break;
		case RETURN:
			ctype = (nt->nt_flags >> 12) & 03;
			if (ctype == 1) 	 /* tn 300 */
				delay = 5;
			else if (ctype == 2)	/* ti 700 */
				delay = 10;
			else if (ctype == 3)	/* concept 100 */
				if ((delay = 9 - nt->nt_col) < 0)
					delay = 0;
			nt->nt_col = 0;
			break;
		}

		if (delay) {
			if (nt->nt_local & LFLUSHO)
				break;
			(*q->next->qinfo->putp)(q->next, ob);
			if (ob = allocb(1)) {
				ob->type = M_DELAY;
				*ob->wptr++ = delay;
				(*q->next->qinfo->putp)(q->next, ob);
			}
			ob = 0;
		}
	}

	if (ib->class & S_DELIM) {
		if (ob == NULL)
			ob = allocb(0);
		if (ob)
			ob->class |= S_DELIM;
	}
	freeb(ib);
	if (ob) {
		if (nt->nt_local & LFLUSHO)
			freeb(ob);
		else
			(*q->next->qinfo->putp)(q->next, ob);
	}
}


/*
 * Writer queue (tty output) service routine.
 * All tty output comes through here.  Upstream data comes in M_CTL
 * blocks via ntout(); echoed data comes in M_DATA blocks.
 */

ntoutsrv (q)
	register struct queue	*q;
{
	register struct nttyld	*nt = (struct nttyld *) q->ptr;
	register struct block	*b;

	while (b = getq (q))
		switch (b->type) {
		default:
			freeb (b);
			continue;
		case M_BREAK:
			if (q->next->flag & QFULL) {
				putbq (q, b);
				return;
			}
			(*q->next->qinfo->putp)(q->next, b);
			continue;
		case M_IOCTL:
			if (q->next->flag & QFULL) {
				putbq (q, b);
				return;
			}
			ntioctl (q, b);
			continue;
		case M_FLUSH:
			flushq (q, 0);
			/* no break */
		case M_IOCNAK:
		case M_IOCACK:
			(*q->next->qinfo->putp)(q->next, b);
			continue;
		case M_CTL:
		case M_DATA:
			if (nt->nt_local & LFLUSHO) {
				freeb (b);
				continue;
			}
			if (q->next->flag & QFULL) {
				putbq (q, b);
				return;
			}
			if (b->type == M_CTL) {
				b->type = M_DATA;
				nt->nt_trash = nt->nt_nin;
			}
			if (nt->nt_flags & RAW || nt->nt_local & LLITOUT) {
				(*q->next->qinfo->putp)(q->next, b);
			}
			else	ntoutb (nt, b);
			continue;
		}

	return;
}


/*
 * Reprint input buffer.
 */

ntreprint (nt)
	register struct nttyld	*nt;
{
	register char	*in;
	register int	nin;

	if (nt->nt_rprntc != 0377)
		ntecho ((int) nt->nt_rprntc, nt);
	(void) putd (putq, nt->nt_outq, '\n');
	for (in = nt->nt_in, nin = nt->nt_nin; nin > 0; in++, nin--)
		ntecho (*in, nt);
	nt->nt_lstate &= ~LSERASE;
	nt->nt_trash = 0;
	return;
}


/*
 * Rubout the last character of the input buffer.
 */

ntrubout (nt)
	register struct nttyld	*nt;
{
	register int	c, cc, col, i;

	if (nt->nt_nin <= 0)
		return;
	c = nt->nt_in[--nt->nt_nin];

	if (!(nt->nt_flags & ECHO))
		return;

	nt->nt_local &= ~LFLUSHO;

	if (nt->nt_local & LPRTERA) {
		if (!(nt->nt_lstate & LSERASE)) {
			(void) putd (putq, nt->nt_outq, '\\');
			nt->nt_lstate |= LSERASE;
		}
		ntecho (c, nt);
		return;
	}

	if (!(nt->nt_local & LCRTBS)) {
		ntecho (nt->nt_erase, nt);
		return;
	}

	if (nt->nt_trash > nt->nt_nin) {
		ntreprint (nt);
		return;
	}

	if (c == ('\t'|0200) || c == ('\n'|0200)) {
		ntbs (nt, 2);
		return;
	}

	switch (partab[c&=0177] & 0177) {
	case ORDINARY:
		if (nt->nt_flags&LCASE && c >= 'A' && c <= 'Z')
			ntbs (nt, 2);
		else	ntbs (nt, 1);
		break;
	case TAB:
		for (col = nt->nt_rocol, i = 0; i < nt->nt_nin; i++) {
			cc = nt->nt_in[i];
			if (cc == ('\t'|0200) || cc == ('\n'|0200)) {
				col += 2;
				continue;
			}
			switch (partab[cc&=0177] & 0177) {
			case ORDINARY:
				if (nt->nt_flags&LCASE && cc>='A' && cc<='Z')
					col += 2;
				else	col++;
				break;
			case TAB:
				col = (col + 8) & ~7;
				break;
			default:
				if (nt->nt_local & LCTLECH)
					col += 2;
				break;
			}
		}
		ntbs (nt, 8 - (col & 7));
		break;
	default:
		if (nt->nt_local & LCTLECH)
			ntbs (nt, 2);
		break;
	}

	return;
}


/*
 * Interpret the control characters that cause signals to be generated
 * immediately: interrupt (^?), quit (^\), stop (^Z).
 * Called from ntinc().
 */

int
ntsigc (c, nt)
	register 		c;
	register struct nttyld	*nt;
{
	if (c == nt->nt_intrc) {
		ntflush (nt, FREAD | FWRITE);
		(void) putctl1 (nt->nt_readq->next, M_SIGNAL, SIGINT);
	}
	else if (c == nt->nt_quitc) {
		ntflush (nt, FREAD | FWRITE);
		(void) putctl1 (nt->nt_readq->next, M_SIGNAL, SIGQUIT);
	}
	else if (c == nt->nt_suspc) {
		ntflush (nt, FREAD);
		(void) putctl1 (nt->nt_readq->next, M_SIGNAL, SIGTSTP);
	}
	else	return (0);

	ntecho (c, nt);
	return (1);
}


/*
 * Interpret the start (^S) and stop (^Q) control characters.
 * Called from ntinc().
 */

int
ntstst (c, nt)
	register int		c;
	register struct nttyld	*nt;
{
	if (nt->nt_state & NT_STOP) {
		if (c == nt->nt_startc || 
		    !(nt->nt_local & LDECCTQ) &&
		    (c != nt->nt_stopc || nt->nt_stopc == nt->nt_startc)) {
			if (putctl (nt->nt_outq->next, M_START))
				nt->nt_state &= ~NT_STOP;
		}
	}
	else	if (c == nt->nt_stopc) {
			if (putctl (nt->nt_outq->next, M_STOP))
				nt->nt_state |= NT_STOP;
		}

	return (c == nt->nt_startc || c == nt->nt_stopc);
}


/*
 * Erase the last word of the input buffer.
 * Called from ntinc().
 */

ntwerase (nt)
	register struct nttyld	*nt;
{
	register char	*s;

	for (s = nt->nt_in + nt->nt_nin - 1; s >= nt->nt_in; s--)
		if (*s == ' ' || *s == '\t')
			ntrubout (nt);
		else	break;

	for (; s >= nt->nt_in; s--)
		if (*s == ' ' || *s == '\t')
			break;
		else	ntrubout (nt);

	return;
}
