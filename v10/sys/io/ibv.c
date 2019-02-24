/*
 *	ibv.c	-	DEC IBV-11A  IEEE INTERFACE DRIVER
 *		Version III (modified for portability, burst reads,
 *		and easier secondary addressing)
 *			DLA	8/1/85
 *
 *
		changed for the Qbus VAX under
		unix 10e 	T. Siegrist 19-Nov-94

	the base address is needed for two registers, the csr and ibs

	the board generates 4 interrupts:
		- err  error		420
		- srq  service request	424	
		- tlr  talker		430
		- lnr  listener		434

To Install:

low core section:
. = ZERO + 420

	ibverr; br5
	ibvsrq; br5
	ibvtlr; br5
	ibvlnr; br5


.globl _iberr
ibverr:	jsr	r0,call; jmp _iberr

.globl _ibsrq 
ibvsrq: jsr	r0,call; jmp _ibsrq

.globl _ibwint
ibvtlr: jsr	r0,call; jmp _ibwint

.globl _ibrint
ibvlnr: jsr	r0,call; jmp _ibrint

c.c section:
int	ibopen(), ibclose(), ibread(), ibwrite(), ibioctl();
in cdevsw, add:
	ibopen, ibclose, ibread, ibwrite, ibioctl, nulldev, 0, 
then add other code

Install ibv.h in /usr/include for user access to regular IEEE commands
 */


/* define NOSDC for no SDC command sent with device open.
 * allows full use of Shell - level I/O without resetting device
 * each time.  "friendly" systems only!
*/
#define	NOSDC

/* 
#include "../h/param.h"
#include "../h/dir.h"
#include "../h/user.h"
#include "../h/proc.h"
#include "../h/var.h"
*/

#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>
#include <sys/ubaddr.h>
#include <sys/proc.h>
#include <sys/systm.h>
#include <sys/ibv.h>

#define	IB_ADDR		0775000		/* IBV-11 base address */
#define IB_VEC		420		/* Interrupt vector */
#define	NIBDEV		31		/* max devices on IEEE bus */
/* (Note the IBV-11 is limited to 15 devs online at a time, but
 * minors for these may range 1-30.  0 will be for the raw bus.
 * Attempting an open on any minor >= NIBDEV sends kill to all bus
 * processes and unconditionally resets bus.)
 */

#define	QMAX		75		/* max queue characters */
#define	TTIME		1800	/* ibtimer return interval in system Hz */
#define	IQLOOP		750		/* max untimed inqueue wait loop */
#define	OQLOOP		750		/* max untimed outqueue wait loop */
/* Pri range while awaiting IEEE bus, > PZERO allows proc to be signaled.
 * Range is reset to IBSPRI when IBHPRI is reached.
 */
#define	IBLPRI		PZERO+5		/* multi-user low value */
#define	IBSPRI		PZERO+15	/* multi-user start value */
#define	IBHPRI		PZERO+25	/* multi-user high value */
#define	IBXPRI		IBLPRI+5	/* exclusive-use wait value */


	/*	IB STATUS FLAGS		*/
#define	IBLOCK		001	/* locked */
#define	IBATEOF		002	/* at EOF */
#define	IBERR		010	/* bus error */
#define	IBULOCK		070	/* unlocking mask - LOCK and EOF bits only! */

	/*	HARDWARE CONTROL	*/
#define	IBSRQ	(1<<15)		/* csr definitions: */
#define	IBER2	(1<<14)
#define	IBER1	(1<<13)
#define	IBCMD	(1<<10)
#define	IBTKR	(1<<9)
#define	IBLNR	(1<<8)
#define	IBACC	(1<<7)
#define	IBIEN	(1<<6)
#define	IBTON	(1<<5)
#define	IBLON	(1<<4)
#define	IBIBC	(1<<3)
#define	IBREM	(1<<2)
#define	IBEOP	(1<<1)
#define	IBTCS	(1<<0)

#define	BEOI	(1<<15)		/* ibs definitions: */
#define	BATN	(1<<14)
#define	BIFC	(1<<13)
#define	BREN	(1<<12)
#define	BSRQ	(1<<11)
#define	BRFD	(1<<10)
#define	BDAV	(1<<9)
#define	BDAC	(1<<8)

#define	IBATT		(IBREM | IBIEN | IBTCS)	/* IB11 take control */
#define	IBTALK		(IBREM | IBIEN | IBTON)	/* IB11 talker */
#define	IBLISTEN	(IBREM | IBIEN | IBLON | IBACC)	/* IB11 listener */
#define	IBIFC		(IBREM | IBIEN | IBIBC)	/* IB11 send IFC */
#define	IBWAIT		(IBREM | IBIEN)		/* IB11 standby */


	/*	structure definitions	*/
struct	clist	{			/* standard character queue */
	int	c_cc;		/* character count */
	char	*c_cf;		/* pointer to first char */
	char	*c_cl;		/* pointer to last char */
};

struct	ibdevs	{			/* bus devices */
	short	pid;		/* proc id controlling dev */
	short	signo;		/* sig no. to be sent on SRQ */
	char	sbyte;		/* SRQ status return byte (returned as int) */
	char	pri;		/* bus use priority penalty */
} bdev[NIBDEV];

struct	device	{			/* the IBV-11 hardware itself */
	unsigned short	csr;		/* control & status register */
	unsigned short 	ibs;		/* instrument bus status register */
					/* these are 16-bit registers */
};

struct	ib	{			/* IBV-11 driver */
	unsigned	state;	/* current status flags */
	int		count;	/* number of currently open devices */
	struct	clist	iq;	/* char in queue */
	struct	clist	oq;	/* char out queue */
} ib;


/*	this is the absolute addressing scheme of the PDP/11
	it is now replaced by a relative addressing scheme, where
	the address is fetched from the address array ubaddr

#define	ibc	((struct device *)IB_ADDR)		/* the IBV-11 card */
*/

#define	TEST(flag)	(ib.state & flag)		/* shorthand */

short	ibrcount;		/* read count */
short	mtq;			/* queue activity test flags */
short	omtq;
short	timer;			/* timer active flag */
extern	int	ibtimer();	/* ib timeout routine */



ibopen(dev,flag)
{	register	struct	ibdevs	*pbdev;
	register	s;

	if( minor(dev)  >=  NIBDEV )	/* unconditional bus reset */
	{	for( pbdev = bdev; pbdev < &bdev[NIBDEV]; pbdev++ )
		{	ibpsig( pbdev->pid, 9 );  /* SIGKILL all procs */
			pbdev->pid = 0;
		}
		ib.state = 0;
		ib.count = 0;
		ibc->csr = 0;
		wakeup((caddr_t)&ib.state);
err:		u.u_error = ENXIO;
		return;
	}

	/* Check this bus dev not inuse.  Note bdev[0] is interface itself,
	 * with a nonzero pid if exclusive use.  Since ibpsig returns 0
	 * for a pid = 0, the double test is ok.
	 */
	pbdev = &bdev[minor(dev)];
	if( ibpsig(pbdev->pid, 0)  ||  ibpsig(bdev[0].pid, 0) )
	{	u.u_error = EBUSY;
		return;
	}

	/*
	 * Since polling of IEEE devices is so device dependent, there is
	 * no general way to tell if a particular one is online. Send cmds
	 * to clear.  If interrupt has set error, there are NONE active.
	 */

	pbdev->pri = IBLPRI;		/* open gets highest priority */
	if( minor(dev) )		/* address device for clear */
	{	bdev[0].pid = 0;	/* should be! */
		ibsetup(dev, 1);	/* await free bus, setup to listen */
#ifndef NOSDC		/* send SDC to dev, else just address & unaddress */
		putc(SDC, &ib.oq);
#endif
	} else		/* exclusive: await bus empty */
	{	s = spl5();	/* allow to sleep before wakeup call */
		while( ib.count || TEST(IBLOCK) )
			sleep((caddr_t)&ib.state,IBXPRI);
		ib.state = IBLOCK;
		splx(s);
		ibqdrain();
		putc(UNTALK, &ib.oq);
	}

	ib.state |= IBATEOF;		/* flag to free after last output */

	if( ib.count )		s = IBATT;	/* start command output */
	else			s = IBIFC;	/* open with IFC */

	if( ibstart(s) )	/* iberr or timeout */
	{	printf("\nNo active IEEE devices");
		return;
	}

	pbdev->pid = u.u_procp->p_pid;  /* this pid now owns dev */
	pbdev->signo = 0;		/* clear SRQ returned signal */
	pbdev->sbyte = 0;		/* clear SRQ status byte */
	pbdev->pri = IBSPRI;		/* set starting I/O priority */
	ib.count++;			/* update open count */
}


ibclose(dev)
{	bdev[minor(dev)].pid = 0;
	if( --ib.count == 0 )		wakeup((caddr_t)&ib.state);
}


ibread(dev)
{	register	c;

	ibsetup(dev, 0);	/* steal and setup bus */
	if( minor(dev) )
		if( ibstart(IBATT) )	return;	/* addressing sequence */

	/* ibrint() keeps track of count via ibrcount.  If EOI observed
	 * on bus, ibrcount set to 0 to flag transaction complete.
	 */
	ibrcount = u.u_count;

	do{		/* break if usr read filled or EOI on bus: */
		if(ib.iq.c_cc == 0)
		{	if( ibrcount == 0 )	break;
			if( ibstart(IBLISTEN) )		return;
		}
	} while( passc(getc(&ib.iq)) == 0 );

	if( minor(dev) )
	{	ib.state |= IBATEOF;
		putc(UNTALK,&ib.oq);
		ibstart(IBATT);
	} else	ibfree();	/* exclusive-use mode - no unaddressing */
}


ibwrite(dev)
{	register	c;
	register	s;

	ibsetup(dev, 1);
	if( minor(dev) )
		if( ibstart(IBATT) )	return;

	/* get chars from user buf.  try at least once since u.u_count = 0
	 * is not necessarily EOF at start .  cpass will set u.u_error if
	 * bad buffer read occurs.
	 */

	if( (c = cpass())  <  0 )
	{	putc(UNLISTEN, &ib.oq);
		s = IBATT;
		goto out;
	}
	s = IBTALK;
	putc(c, &ib.oq);

	do {	if( (c = cpass())  <  0 )	break;
		if( ib.oq.c_cc > QMAX )	/* restart ints and sleep */
			if( ibstart(IBTALK) )		return;

		putc(c, &ib.oq);
	} while( u.u_count ) ;

out:	ib.state |= IBATEOF;
	ibstart(s);
}



ibioctl(dev, mode, arg, flag)
dev_t	dev;
int	mode;
caddr_t	arg;
{	register	struct	ibdevs	*pbdev;
	register	s;
	int	cmd;

	pbdev = &bdev[minor(dev)];

	switch( mode )
	{	case	IBSETPID:		/* set new controlling pid */
			if( copyin( arg, (caddr_t)&pbdev->pid,
			    sizeof(pbdev->pid)) )
			{
err1:				u.u_error = EFAULT;
				return;
			}
			break;
		case	IBSETSIG:		/* set SRQ return signal */
			if( copyin( arg, (caddr_t)&pbdev->signo,
			    sizeof(pbdev->signo)) )	goto err1;
			break;
		case	IBGETSTAT:		/* return SRQ status byte */
			/* insure no SRQ int while current byte is being
			 * cleared.  sbyte is int for copyout() portability
			 */
			s = spl5();
			cmd = pbdev->sbyte;
			if( copyout( (caddr_t)&cmd, arg, sizeof(cmd)) )
				u.u_error = EFAULT;
			else	pbdev->sbyte = 0;	/* clear status byte */
			splx(s);
			break;
		case	IBSENDC:		/* send command */
			ibsetup(dev, 1);
			if( copyin( arg, (caddr_t)&cmd, sizeof(cmd)) )
			{	ibfree();
				goto err1;
			}
			putc(cmd, &ib.oq);

			if( minor(dev) )	/* addr cmds (less TCT) only: */
			{	if( cmd & 020  ||  cmd == TCT )
				{	ibfree();
					goto err2;
				}
				putc(UNLISTEN, &ib.oq);
			} else	/* "raw" bus mode: */
			{	if( cmd == TCT )
				{	/* release bus until EOI or signaled */
					if( ibstart(IBATT) )	return;
					ibc->csr = IBREM;
					mtq++;	/* allow timeout wakeups */
			  while( !(ibc->ibs & BEOI)  &&  !issig() )
						sleep((caddr_t)&mtq, IBLPRI);
				} else
				{ /* if extended (2byte) cmd, add to queue: */
					cmd >>= 8;
					if( cmd )	putc(cmd,&ib.oq);
				}
			}
			ib.state |= IBATEOF;
			ibstart(IBATT);
			break;
		default:
err2:			u.u_error = EINVAL;
	}
}



ibrint()
{
	putc(ibc->ibs, &ib.iq);

	/* if not last char or buffer overflow, complete handshake
	 * and return.  Otherwise, stop I/O by not completing handshake:
	 * ibstart() will complete and restart if more chars expected.
	 */

	if( ibc->ibs & BEOI )	ibrcount = 0;	/* last byte from sender */
	else			ibrcount--;

	if( ibrcount  &&  ib.iq.c_cc < QMAX )
	{	ibc->ibs = 0;	/* complete DAC handshake */
		mtq++;		/* show activity to preclude timeout */
	} else
	{	omtq = mtq = 0;		/* invalidate timer */
		wakeup((caddr_t)&mtq);
	}

}



ibwint()
{	if( ib.oq.c_cc == 0 )	/* empty queue - alert process */
	{	omtq = mtq = 0;		/* invalidate timer */
		wakeup((caddr_t)&mtq);
		if( TEST(IBATEOF) )	ibfree();
		return;
	}

	mtq++;		/* show activity */

	/* if last char on queue, set EOI and add UNLISTEN if reqd: */
	if( (ib.oq.c_cc == 1)  &&  TEST(IBATEOF)  &&  !(ibc->ibs & BATN) )
	{	if( ibc->ibs & BEOI )
		{	/* previous was last data in normal-use mode,
			 * and UNLISTEN is char on queue.
			 * remove EOI, set ATT for UNLISTEN cmd
			 * and output next loop
			 */
			ibc->csr = IBATT;
			return;
		} else
		{	/* last data: add EOI to output and queue
			 * UNLISTEN if normal use mode.
			 * Since ATEOF, bus will be freed afterwards
			 */
			ibc->csr |= IBEOP;
			if( !bdev[0].pid )	putc(UNLISTEN, &ib.oq);
		}
	}

	ibc->ibs = getc(&ib.oq) & 0377;
}



iberr()
{	if( ibc->csr & IBER1 )	printf("\nIEEE hardw error");
	ibterror();
}


ibsrq()
{	/* If bus locked, SRQ poll will be done during ibfree().
	 * Otherwise, if bus open call ibfree to do poll now.
	 * ibpollsrq will show bus locked when running.
	 */
	if( !TEST(IBLOCK)  &&  ib.count )
	{	ib.state |= IBLOCK;
		ibfree();	/* run srq, etc */
	}
}



ibpollsrq()	/* called from ibfree() to do SRQ serial poll */
{	register	struct	ibdevs	*pbdev;
	register	i, flag;
	int	s, twice;
	char	c;

	/* if exclusive-use, signal and return */
	if( ibpsig( bdev[0].pid, bdev[0].signo) )	return;

	/* Serial poll active devices first.  If SRQ, record status byte and
	 * signal process.  If SRQ uncleared, try all devices.  Timeouts are
	 * loops and spl is reset since poll might be called from interrupt.
	 */
	ibc->csr = IBWAIT;
	ib.state &= ~IBATEOF;
	s = spl1();
	ibqdrain();
	ibunq();
	putc(SPE, &ib.oq);
	flag = 1;
	twice = 0;

loop:	while( ibc->ibs & BSRQ )
	{	for(pbdev = &bdev[1], i = 1; i < NIBDEV; pbdev++, i++)
		{	if( flag )
			{	if( pbdev->pid == 0 )	continue;
			} else	if( pbdev->pid )	continue;
			putc((i | TALK), &ib.oq);
			mtq++;
			ibc->csr = IBWAIT;
			ibc->csr = IBATT;
			if( iboqloop() )	break;
			ibrcount = 1;	/* 1 byte anticipated */
			ibc->csr = IBLISTEN;
			if( ibiqloop() )	continue;  /* LISTEN timeout */

			if( (c = getc(&ib.iq))  &  0100 )  /* set bit = SRQ */
			{	if( twice == i )	/* bad device: */
				{	printf("\nIEEE bad dev %d",i);
					flag = 0;
					break;
				}

				twice = i;
				pbdev->sbyte = c;	/* store status */
				if( pbdev->signo )  /* send sig if wanted */
					ibpsig(pbdev->pid, pbdev->signo);
				goto loop;	/* search for more SRQ's */
			}
		}
		if(flag)	flag = 0;
		else
		{	printf("\nIEEE uncleared SRQ");
			break;
		}
	}

	/* end poll.  Note if SRQ has not been cleared, bus ops can
	 * continue, but runsrq will occur again at every ibfree().
	 */
	putc(SPD, &ib.oq);
	ibunq();
	mtq++;
	ibc->csr = IBWAIT;
	ibc->csr = IBATT;
	if( iboqloop() )	printf("\nIEEE bad SRQ poll");
	splx(s);
}


ibsetup(dev, flag)		/* await bus, setup, queue dev address */
dev_t	dev;
int	flag;
{	register struct ibdevs *pbdev;
	register	s;

	pbdev = &bdev[minor(dev)];
	if( pbdev->pri < IBHPRI )	pbdev->pri++;	/* use penalty */
	else				pbdev->pri = IBSPRI;	/* reset */

	s = spl5();	/* hold off intrs until test and sleep */
	while( TEST(IBLOCK) )
	{	if( pbdev->pri > IBLPRI )	pbdev->pri--;
		sleep((caddr_t)&ib.state, pbdev->pri);
	}

	ib.state = IBLOCK;	/* show now inuse */
	splx(s);
	ibqdrain();
	if( minor(dev) )	/* not exclusive-use: q dev addr for I/O */
	{	ibunq();	/* queue bus UN- commands */
		if(flag)	putc((minor(dev) | LISTEN),&ib.oq);
		else		putc((minor(dev) | TALK),&ib.oq);
	}
}



ibstart(csr)	/* start interrupts, sleep on mtq & test error */
int	csr;
{	register	s;

	s = spl5();	/* disable ints until reallowed as part of sleep() */
	if( timer == 0 )	/* start ibtimer if not running */
	{	timer++;
		timeout( ibtimer, (caddr_t)0, TTIME);
	}
	omtq = mtq = 0;		/* guarantee fresh timeout */
	mtq++;
	ib.state &= ~IBERR;	/* clear ERR flag */
	/* csr bits should not be toggled for LISTEN.  toggling is
	 * required for TALK and ATT to trigger first interrupt.
	 * write 0 to ibc->ibs to complete a LISTEN handshake - reqd
	 * if listening interrupted due to buffer fill.
	 */
	if( csr == IBLISTEN )	ibc->ibs = 0;
	else			ibc->csr = IBWAIT;

	ibc->csr = csr;
	sleep((caddr_t)&mtq, PZERO);	/* not awakened by signal */

	splx(s);
	if( TEST(IBERR) )	/* timeout or iberr */
	{	u.u_error = ENXIO;		/* default error */
		return(1);
	}

	return(0);
}



ibtimer()		/* timeout routine */
{	if( TEST(IBLOCK) )
	{	if( mtq  &&  (mtq == omtq) )
		{	printf("\nIEEE timeout ");
			ibterror();
		} else			/* continue timer */
		{	omtq = mtq;
			timeout( ibtimer, (caddr_t)0, TTIME);
			return;
		}
	}
	timer = 0;		/* turn timer off */
}



ibterror()		/* bus or timeout error close routine */
{	ib.state |= IBERR;
	omtq = mtq = 0;
	wakeup((caddr_t)&mtq);
	if( TEST(IBLOCK) )	ibfree();
}



ibqdrain()		/* flush I/O queues */
{	while( getc(&ib.iq) >= 0 )	;
	while( getc(&ib.oq) >= 0 )	;
}


ibunq()		/* add UN- bus commands to output queue */
{	putc(UNTALK, &ib.oq);
	putc(UNLISTEN, &ib.oq);
}



iboqloop()		/* loop timeout on outqueue activity */
{	int	i = 0;

	while( mtq )
		if( i++ > OQLOOP )	return(1);
	return(0);
}

ibiqloop()		/* loop timeout on inqueue activity */
{	int	i = 0;

	while( ib.iq.c_cc == 0 )
		if( i++ > IQLOOP )	return(1);
	return(0);
}


ibfree()
{	if( ibc->ibs & BSRQ )	ibpollsrq();	/* service any SRQ request */
	ib.state &= IBULOCK;		/* clear EOF & LOCK flags */
	ibc->csr = IBWAIT;		/* leave csr in standby */
	wakeup((caddr_t)&ib.state);	/* alert any sleepers */
}


/*	ibpsig() - checks pid active in sys proc table, sends sig if nonzero.
 *			returns 1 if process was found, 0 if not
 */
ibpsig(pid, sig)
short	pid;
short	sig;
{	register struct proc *pp;

	if( pid == 0 )		return(0);

	for(pp = &proc[0]; pp < u.u_procp; pp++)  /* changed TSI*/
/*	for(pp = &proc[0]; pp < maxproc; pp++) */
		if( pp->p_pid == pid )
		{	if( sig )	psignal(pp, sig);
			return(1);
		}

	return(0);
}


