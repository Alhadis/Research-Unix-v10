#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/stream.h"
#include "sys/inode.h"
#include "sys/filio.h"
#include "sys/ttyio.h"
#include "sys/conf.h"
#include "sys/proc.h"
#include "sys/file.h"

extern struct queue queue[];

#define	STIPRI	28
#define	STOPRI	29

extern struct stdata streams[];
extern	int streamcnt;

struct  stdata *stenter();
int	strput(), stwsrv(), nulldev(), nilput();
long	nilopen();

struct	qinit strdata = { strput, NULL, nilopen, nulldev, 512, 256 };
struct	qinit stwdata = { nulldev, stwsrv, nilopen, nulldev, 0, 0};
struct	qinit nilw = { nilput, NULL, nilopen, nulldev, 1, 0 };
struct	streamtab nilinfo = { &nilw, &nilw };

/*
 * open a stream device
 * returns the inode,
 * which may be different from ip,
 * in which event ip has been put.
 * return NULL on error,
 * in which event ip has also been put.
 */
struct inode *
stopen(qinfo, dev, flag, ip)
struct streamtab *qinfo;
dev_t dev;
struct inode *ip;
{
	register struct queue *qp;
	register struct stdata *sp;
	register struct inode *nip;
	register s;

	if (sp = ip->i_sptr) {		/* already streaming? */
		if (sp->flag & EXCL && u.u_uid!=0
		   && (sp->pgrp==0 || sp->pgrp != u.u_procp->p_pgrp)) {
			u.u_error = ENXIO;
			iput(ip);
			return(NULL);
		}
		if (stenter(ip)==NULL) {
			iput(ip);
			return(NULL);
		}
		while (sp->flag&STWOPEN) {
			if (tsleep((caddr_t)sp, STOPRI, 0)!=TS_OK) {
				stexit(ip);
				u.u_error = ENXIO;
				iput(ip);
				return(NULL);
			}
		}
		if (sp->flag&HUNGUP) {
			flushq(RD(sp->wrq), 1);
			stexit(ip);
			u.u_error = ENXIO;
			iput(ip);
			return(NULL);
		}
		for (qp = sp->wrq; qp->next && (qp->next->flag&QREADR)==0; qp = qp->next) {
			/* device-cloning;  qopen s.b. struct inode * */
			nip = (struct inode *)
			  (*qp->next->qinfo->qopen)(RD(qp->next), dev);
			if (nip == NULL) {	/* open failed */
				stexit(ip);
				u.u_error = ENXIO;
				iput(ip);
				return(NULL);
			}
			if ((long)nip != 1) {	/* changed inode, so stop now */
				stexit(ip);
				iput(ip);
				return(nip);
			}
		}
		stexit(ip);
		return(ip);
	} else {		/* new stream */
		register struct stdata *esp = &streams[streamcnt];
		register struct queue *nq;

		for (sp = streams; sp < esp; sp++)
			if (sp->wrq==NULL)
				break;
		if (sp >= esp) {
			printf("out of streams\n");
			u.u_error = ENFILE;
			iput(ip);
			return(NULL);
		}
		if ((qp = allocq()) == NULL) {
			printf("Out of queues in stopen\n");
			u.u_error = ENFILE;
			iput(ip);
			return(NULL);
		}
		sp->wrq = WR(qp);
		sp->pgrp = 0;
		sp->inode = ip;
		sp->flag = 0;
		sp->iocblk = NULL;
		sp->count = 1;
		qp->ptr = (caddr_t)sp;
		WR(qp)->ptr = (caddr_t)sp;
		qp->qinfo = &strdata;
		WR(qp)->qinfo = &stwdata;
		qp->flag |= QBIGB;
		WR(qp)->flag |= QDELIM;
		ip->i_sptr = sp;
		s = spl6();
		if (qattach(qinfo, qp, dev) == 0) {
			sp->flag |= HUNGUP;
			splx(s);
			stexit(ip);
			u.u_error = ENXIO;
			iput(ip);
			return(NULL);
		}
		nq = RD(WR(qp)->next);
		sp->flag |= STWOPEN;
		nip = (struct inode *)(*nq->qinfo->qopen)(nq, dev);
		sp->flag &= ~STWOPEN;
		splx(s);
		wakeup((caddr_t)sp);
		if (nip == NULL) {
			qdetach(nq, 0);
			sp->flag |= HUNGUP;
			stexit(ip);
			u.u_error = ENXIO;
			iput(ip);
			return(NULL);
		}
		if ((long)nip != 1) {
			stexit(ip);
			iput(ip);
			return(nip);
		}
		stexit(ip);
		return(ip);
	}
}

/*
 * Shut down a stream
 *  -- pop all line disciplines
 *  -- shut down the driver
 */
stclose(ip, sleepOK)
struct inode *ip;
{
	register struct stdata *stp;
	register struct queue *qp;
	register s;
	register struct block *bp;

	s = spl6();
	if ((stp = ip->i_sptr) == NULL) {
		splx(s);
		return;
	}
	forceclose(stp, FREAD|FWRITE);
	if (stp->count != 0) {
		printf("count is %d in stclose, stream %x\n", stp->count, stp);
		splx(s);
		return;
	}
	ip->i_sptr = NULL;
	splx(s);
	qp = stp->wrq;
	while (qp->next) {
		register i;
		/* detect cross-connects, and seal off other end */
		if (qp->next->flag & QREADR) {
			putctl(qp->next, M_HANGUP);
			if (qattach(&nilinfo, qp->next, (dev_t)-1) == 0)
				panic("close/qattach");
			WR(qp->next)->next = NULL;
			break;
		}
		if (sleepOK && (stp->flag&HUNGUP)==0)
			for (i=0; qp->next->count!=0 && i<60; i++)
				tsleep((caddr_t)qp, STIPRI, 1);
		qdetach(RD(qp->next), 1);
	}
	flushq(qp, 1);
	while (bp = getq(RD(qp))) {
		if (bp->type == M_PASS)
			closef(((struct kpassfd *)bp->rptr)->f.fp);
		freeb(bp);
	}
	stp->wrq = NULL;
	RD(qp)->flag = 0;
	qp->flag = 0;
}

/*
 * Read a stream
 */
stread(ip)
struct inode *ip;
{
	register struct stdata *stq;
	register struct block *bp;
	register n;
	register s, nc = 0;

	if ((stq = stenter(ip)) == NULL) {
		u.u_error = 0;			/* return EOF not error */
		return;
	}
	/*
	 * Read is complete when:
	 * Q is empty and some data has been read and next Q might not generate
	 * delimiters, or
	 * There is data on Q and count has been satisfied, or
	 * A delimiter is taken from the Q, or
	 * There was an error.
	 */
	for (;;) {
		s = spl6();
		if ((bp = getq(RD(stq->wrq))) == NULL) {
			if ((nc && (OTHERQ(stq->wrq->next)->flag&QDELIM)==0)
			 || stq->flag&HUNGUP) {
				splx(s);
				stexit(ip);
				return;
			}
			stq->flag |= RSLEEP;
			n = tsleep((caddr_t)RD(stq->wrq), STIPRI, 0);
			splx(s);
			if (n == TS_SIG) {
				stexit(ip);
				longjmp(u.u_qsav);
			}
			continue;
		}
		splx(s);
		switch (bp->type) {

		case M_DATA:
			n = min(u.u_count, bp->wptr - bp->rptr);
			if (n)
				iomove(bp->rptr, n, B_READ);
			bp->rptr += n;
			nc += n;
			if (bp->rptr >= bp->wptr) {
				n = bp->class;
				freeb(bp);
				if (n&S_DELIM)
					goto out;
			} else {
				putbq(RD(stq->wrq), bp);
				if (u.u_count == 0)
					goto out;
			}
			if (u.u_error)
				goto out;
			continue;

		case M_PASS:
			putbq(RD(stq->wrq), bp);
			goto out;

		default:
			printf("stread: msg %d\n", bp->type);
			freeb(bp);
			continue;
		}
	}
out:
	stexit(ip);
}

/*
 *  Internal version of stread: transfer to given address.
 *  Always returns; >= 0 is # bytes transferred, -1 is error.
 *  Always reads a whole record; excess discarded.
 */
istread(ip, addr, count, flag)	/* flag is for timeout debugging */
struct inode *ip;
caddr_t addr;
{
	register struct stdata *stq;
	register struct block *bp;
	register n;
	register s, nc = 0;

	if ((stq = stenter(ip)) == NULL)
		return(-1);
	for (;;) {
		s = spl6();
		if ((bp = getq(RD(stq->wrq))) == NULL) {
			if ((nc && (OTHERQ(stq->wrq->next)->flag&QDELIM)==0)
			 || stq->flag&HUNGUP) {
				splx(s);
				stexit(ip);
				return(nc);
			}
			stq->flag |= RSLEEP;
			n = tsleep((caddr_t)RD(stq->wrq), PRIBIO, 30);
			splx(s);
			if (n == TS_TIME) {
				if(flag)
					printf("nb timeout nc %d count %d addr #%x\n",
						nc, count, addr);
				stexit(ip);
				return(-1);
			}
			continue;
		}
		splx(s);
		switch (bp->type) {

		case M_DATA:
			n = min(count, bp->wptr - bp->rptr);
			if (n)
				bcopy(bp->rptr, addr, n);
			addr += n;
			nc += n;
			count -= n;
			n = bp->class;
			freeb(bp);
			if (n&S_DELIM) {
				stexit(ip);
				return(nc);
			}
			continue;

		case M_PASS:
			putbq(RD(stq->wrq), bp);
			stexit(ip);
			return(nc);

		default:
			printf("istread: msg %d\n", bp->type);
			freeb(bp);
			continue;
		}
	}
}

/*
 * Stream input put proc
 *  -- collect signals and ioctl acks
    -- put data and delims on data queue
    -- see hangups and seal off channel
 */
strput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct stdata *stp = (struct stdata *)q->ptr;

	switch (bp->type) {

	case M_DATA:
	default:
		putq(q, bp);
		if (stp->flag & RSLEEP) {
			stp->flag &= ~RSLEEP;
			wakeup((caddr_t)q);
		}
		break;

	case M_HANGUP:
		stp->flag |= HUNGUP;
		freeb(bp);
		if (stp->pgrp)
			gsignal(stp->pgrp, SIGHUP);
		forceclose(stp, FWRITE);
		wakeup((caddr_t)q);	/* the readers */
		wakeup((caddr_t)WR(q));	/* the writers */
		wakeup((caddr_t)stp);	/* the ioctllers */
		break;

	case M_SIGNAL:
		gsignal(stp->pgrp, *bp->rptr);
	case M_FLUSH:
		flushq(q, 0);		/* flush races with select */
	case M_BREAK:
	case M_YDEL:
	case M_NDEL:
	case M_CTL:
		freeb(bp);
		return;

	case M_IOCACK:
	case M_IOCNAK:
	case M_IOCWAIT:
		if(stp->iocblk && stp->iocblk->type==M_IOCWAIT) {
			freeb(stp->iocblk);
			stp->iocblk = NULL;
		}
		if ((stp->flag&IOCWAIT)==0 || stp->iocblk) {
			freeb(bp);
			return;
		}
		stp->iocblk = bp;
		wakeup((caddr_t)stp);
		return;

	case M_IOCTL:
		bp->type = M_IOCNAK;
		bp->wptr = bp->rptr;	/* data means error */
		qreply(q, bp);
		return;
	}
	if (stp->rsel) {
		selwakeup(stp->rsel, stp->flag&RSEL);
		stp->flag &= ~RSEL;
		stp->rsel = NULL;
	}
}


/*
 * Write on a stream
 */
stwrite(ip)
struct inode *ip;
{
	register struct stdata *stq;
	register struct block *bp;
	register n;
	register s;

	if ((stq = stenter(ip)) == NULL)
		return;
	do {
		s = spl6();
		while ((stq->flag&HUNGUP)==0 && stq->wrq->next->flag&QFULL) {
			stq->flag |= WSLEEP;
			if (tsleep((caddr_t)stq->wrq, STOPRI, 0)==TS_SIG) {
				splx(s);
				stexit(ip);
				longjmp(u.u_qsav);
			}
		}
		splx(s);
		if (stq->flag & HUNGUP) {
			psignal(u.u_procp, SIGPIPE);
			u.u_error = ENXIO;
			break;
		}
		n = QBSIZE;
		if (u.u_count >= 512 && stq->wrq->next->flag&QBIGB)
			n = 1024;
		if ((bp = allocb(n)) == NULL) {
			printf("stwrite can't alloc\n");
			continue;		/* temp */
		}
		bp->type = M_DATA;
		n = min(bp->lim - bp->wptr, u.u_count);
		iomove(bp->wptr, n, B_WRITE);
		if (u.u_error) {
			freeb(bp);
			break;
		}
		bp->wptr += n;
		if (u.u_count==0)
			bp->class |= S_DELIM;
		(*stq->wrq->next->qinfo->putp)(stq->wrq->next, bp);
	} while (u.u_count);
	stexit(ip);
}

/*
 * Stream output -- internal version
 *  return -1 on error, 0 OK
 */
istwrite(ip, addr, count)
register count;
struct inode *ip;
caddr_t addr;
{
	register struct stdata *stq;
	register struct block *bp;
	register n;
	register s;

	if ((stq = stenter(ip)) == NULL)
		return(-1);
	do {
		s = spl6();
		while (stq->wrq->next->flag&QFULL && (stq->flag&HUNGUP)==0) {
			stq->flag |= WSLEEP;
			if (tsleep((caddr_t)stq->wrq, PRIBIO, 30)==TS_TIME) {
				splx(s);
				stexit(ip);
				return(-1);
			}
		}
		splx(s);
		if (stq->flag & HUNGUP) {
			stexit(ip);
			return(-1);
		}
		n = QBSIZE;
		if (count >= 512 && stq->wrq->next->flag&QBIGB)
			n = 1024;
		if ((bp = allocb(n)) == NULL) {
			printf("istwrite can't alloc\n");
			continue;		/* temp */
		}
		bp->type = M_DATA;
		n = min(bp->lim - bp->wptr, count);
		bcopy(addr, bp->wptr, n);
		bp->wptr += n;
		addr += n;
		count -= n;
		if (count==0)
			bp->class |= S_DELIM;
		(*stq->wrq->next->qinfo->putp)(stq->wrq->next, bp);
	} while (count);
	stexit(ip);
	return(0);
}

/*
 * Stream output server (when full queue empties)
 */
stwsrv(q)
register struct queue *q;
{
	register struct stdata *stq = (struct stdata *)q->ptr;

	if (stq->flag & WSLEEP) {
		stq->flag &= ~WSLEEP;
		wakeup((caddr_t)q);
	}
	if (stq->wsel) {
		selwakeup(stq->wsel, stq->flag&WSEL);
		stq->flag &= ~WSEL;
		stq->wsel = NULL;
	}
}

/*
 * ioctl for streams
 */
stioctl(ip, cmd, arg)
struct inode *ip;
caddr_t arg;
{
	register struct stdata *stq;
	register struct block *bp;
	register struct queue *q;
	register s;
	int fmt, nld, ioctime;
	struct insld ld;
	extern nstreamtab;

	if ((stq = stenter(ip)) == NULL)
		return;
	switch(cmd) {

	case FIONREAD:
		nld = 0;
		bp = RD(stq->wrq)->first;
		if (bp && bp->type==M_DATA)
			nld = bp->wptr - bp->rptr;
		if (copyout((caddr_t)&nld, arg, sizeof(nld)))
			u.u_error = EFAULT;
		goto out;

	case TIOCGPGRP:
		if (copyout ((caddr_t) &stq->pgrp, arg, sizeof (stq->pgrp)))
			u.u_error = EFAULT;
		goto out;

	case TIOCSPGRP:
		if (arg==0) {
			stq->pgrp = u.u_procp->p_pgrp = u.u_procp->p_pid;
			u.u_ttydev = ip->i_dev;
			u.u_ttyino = ip->i_number;
		} else if (copyin(arg, (caddr_t)&stq->pgrp, sizeof(stq->pgrp)))
			u.u_error = EFAULT;
		goto out;

	case TIOCEXCL:
		stq->flag |= EXCL;
		goto out;

	case TIOCNXCL:
		stq->flag &= ~EXCL;
		goto out;

	case TIOCFLUSH:
		if (stq->flag & HUNGUP) {
			u.u_error = ENXIO;
			goto out;
		}
		flushq(RD(stq->wrq), 0);
		putctl(stq->wrq->next, M_FLUSH);
		goto out;

	case TIOCSBRK:
		if (stq->flag & HUNGUP) {
			u.u_error = ENXIO;
			goto out;
		}
		putctl(stq->wrq->next, M_BREAK);
		goto out;

	case FIOSNDFD:
		if (stq->flag & HUNGUP) {
			u.u_error = ENXIO;
			goto out;
		}
		usndfile(stq, arg);
		goto out;

	case FIORCVFD:
		urcvfile(stq, arg);
		goto out;

	case FIOPUSHLD:			/* add a line discipline */
	case FIOINSLD:
		if (stq->flag & HUNGUP) {
			u.u_error = ENXIO;
			goto out;
		}
		if (copyin(arg, (caddr_t)&ld, sizeof(ld))) {
			u.u_error = EFAULT;
			goto out;
		}
		if(ld.ld<0 || ld.ld>=nstreamtab || streamtab[ld.ld]==NULL){
			u.u_error = EINVAL;
			goto out;
		}
		if (cmd==FIOPUSHLD)
			ld.level = 0;
		q = stq->wrq;
		while (ld.level > 0) {
			if (q->next == NULL) {
				u.u_error = EINVAL;
				goto out;
			}
			q = q->next;
			ld.level--;
		}
		s = spl6();
		if (qattach(streamtab[ld.ld], RD(q), ip->i_un.i_rdev)) {
			long nip =
			  (*q->next->qinfo->qopen)(RD(q->next), ip->i_un.i_rdev);
			if (nip==0) {
				qdetach(RD(q->next), 0);
				u.u_error = ENXIO;
			} else if (nip!=1)
				panic("pushld qopen returns inode");	
			splx(s);
			goto out;
		}
		splx(s);
		u.u_error = ENOMEM;
		goto out;

	case FIOPOPLD:
		if (stq->flag & HUNGUP) {
			u.u_error = ENXIO;
			goto out;
		}
		nld = 0;
		if (arg) {
			if (copyin(arg, (caddr_t)&nld, sizeof(nld))) {
				u.u_error = EFAULT;
				goto out;
			}
		}
		q = stq->wrq;
		while (nld > 0) {
			if (q == NULL || q->next==NULL
			 || q->next->flag&QREADR) {
				u.u_error = EINVAL;
				goto out;
			}
			q = q->next;
			nld--;
		}
		if (q->next->next && (q->next->flag&QREADR) == 0) {	/* LD exists? */
			qdetach(RD(q->next), 1);
			goto out;
		}
		u.u_error = EINVAL;
		goto out;

	case FIOLOOKLD:
		nld = 0;
		if (arg) {
			if (copyin(arg, (caddr_t)&nld, sizeof(nld))) {
				u.u_error = EFAULT;
				goto out;
			}
		}
		q = stq->wrq;
		for (;;) {
			if (q == NULL || q->next==NULL || q->next->next == NULL
			 || q->next->flag&QREADR) {
				u.u_error = EINVAL;
				goto out;
			}
			if (--nld < 0)
				break;
			q = q->next;
		}
		for (fmt=0; fmt<nstreamtab; fmt++)
			if(streamtab[fmt] && streamtab[fmt]->wrinit==q->next->qinfo)
				break;
		if (fmt >= nstreamtab) {
			u.u_error = ENXIO;
			goto out;
		}
		if (arg) {
			if (copyout((caddr_t)&fmt, arg, sizeof(arg))) {
				u.u_error = EINVAL;
				goto out;
			}
		}
		u.u_r.r_val1 = fmt;
		goto out;

	}
	if (stq->flag & HUNGUP) {
		u.u_error = ENXIO;
		goto out;
	}
	if ((bp = allocb(STIOCHDR+(arg?STIOCSIZE:0))) == NULL) {
		u.u_error = ENOSPC;
		goto out;
	}
	((struct stioctl *)bp->wptr)->com[0] = cmd;
	((struct stioctl *)bp->wptr)->com[1] = cmd>>8;
	((struct stioctl *)bp->wptr)->com[2] = cmd>>16;
	((struct stioctl *)bp->wptr)->com[3] = cmd>>24;
	bp->wptr += STIOCHDR;
	if (arg) {
		if (copyin(arg, (caddr_t)stiodata(bp), STIOCSIZE)) {
			u.u_error = EFAULT;
			freeb(bp);
			goto out;
		}
		bp->wptr += STIOCSIZE;
	}
	bp->type = M_IOCTL;
	s = spl6();
	while (stq->flag & IOCWAIT) {
		if (tsleep((caddr_t)stq,STIPRI,0)!=TS_OK || stq->flag&HUNGUP) {
			splx(s);
			u.u_error = EIO;
			freeb(bp);
			goto out;
		}
	}
	stq->flag |= IOCWAIT;
	splx(s);
	(*stq->wrq->next->qinfo->putp)(stq->wrq->next, bp);
	/* wait for acknowledgment */
	s = spl6();
	ioctime = 15;
	while ((bp = stq->iocblk) == NULL || bp->type == M_IOCWAIT) {
		if (bp)
			ioctime = 0;
		if (tsleep((caddr_t)stq, STIPRI, ioctime) != TS_OK
				|| stq->flag & HUNGUP) {
			if (stq->iocblk)
				freeb(stq->iocblk);
			stq->iocblk = NULL;
			u.u_error = EIO;
			stq->flag &= ~IOCWAIT;
			splx(s);
			goto out;
		}
	}
	stq->iocblk = NULL;
	stq->flag &= ~IOCWAIT;
	splx(s);
	switch (bp->type) {
	case M_IOCACK:
		if (bp->wptr > bp->rptr) {
			bp->rptr = (u_char *)((struct stioctl *)(bp->rptr))->data;
			if (copyout((caddr_t)bp->rptr, arg, bp->wptr-bp->rptr))
				u.u_error = EFAULT;
		}
		freeb(bp);
		break;

	case M_IOCNAK:
		if (bp->rptr < bp->wptr)
			u.u_error = *bp->rptr;
		freeb(bp);
		if (u.u_error==0)
			u.u_error = ENOTTY;
		break;

	default:
		printf("strange stuff on ioctl ack cell %x\n", stq);
		break;
	}
	wakeup((caddr_t)stq);
out:
	stexit(ip);
}

/*
 * attach a stream device or line discipline
 *   qp is a read queue; the new queue goes in so its next
 *   read ptr is the argument, and the write queue corresponding
 *   to the argument points to this queue.
 */
qattach(qinfo, qp, dev)
register struct streamtab *qinfo;
register struct queue *qp;
dev_t dev;
{
	register struct queue *nq;
	register s;
	extern putq();

	if ((nq = allocq()) == NULL) {
		printf("allocq NULL\n");
		return(0);
	}
	s = spl6();
	nq->next = qp;
	WR(nq)->next = WR(qp)->next;
	if (WR(qp)->next)
		OTHERQ(WR(qp)->next)->next = nq;
	WR(qp)->next = WR(nq);
	nq->qinfo = qinfo->rdinit;
	WR(nq)->qinfo = qinfo->wrinit;
	nq->flag |= QREADR|QWANTR;
	WR(nq)->flag |= QWANTR;
	nq->ptr = NULL;
	WR(nq)->ptr = NULL;
	splx(s);
	return(1);
}

/*
 * Detach a stream device or line discipline.
 *  Call its close routine, then evict it.
 *  The close routine is required to return.
 *  The flag (if 1) indicates that the close routine
 *  should be called (real shutdown) otherwise this
 *  is merely a failed open.
 */
qdetach(qp, flag)
register struct queue *qp;
{
	register s = spl6();
	register i;

	if (flag) {
		queuerun();
		(*qp->qinfo->qclose)(qp);
		for (i=0; (qp->flag|WR(qp)->flag)&QENAB; i++) {
			queuerun();
			if (i>10)
				panic("queue won't give up");
		}
		flushq(qp, 1);
		flushq(WR(qp), 1);
	}
	if (WR(qp)->next)
		backq(qp)->next = qp->next;
	if (qp->next)
		backq(WR(qp))->next = WR(qp)->next;
	qp->flag = 0;
	WR(qp)->flag = 0;
	splx(s);
}

/*
 * Count entries/exits to the routines that process streams.
 * If a stream receives a HANGUP, it should be shut down
 * forcibly, and this makes sure that it does not happen while
 * pointers are still floating about.
 */
struct stdata *
stenter(ip)
register struct inode *ip;
{
	register struct stdata *stp;
	register s = spl6();

	if ((stp = ip->i_sptr) == NULL) {
		u.u_error = ENXIO;
		splx(s);
		return(NULL);
	}
	stp->count++;
	splx(s);
	return(stp);
}

stexit(ip)
register struct inode *ip;
{
	register struct stdata *stp = ip->i_sptr;

	if(stp == 0) {
		printf("null stp in stexit\n");
		return;
	}	/* can't happen, of course */
	if (--stp->count==0 && stp->flag&HUNGUP && RD(stp->wrq)->count==0)
		stclose(ip, 1);
}

/*
 * create a message transferring a file to process on the other end of a stream
 */
usndfile(stq, arg)
register struct stdata *stq;
caddr_t arg;
{
	int f;
	register struct file *fp;

	if (copyin(arg, (caddr_t)&f, sizeof(f))) {
		u.u_error = EFAULT;
		return;
	}
	if ((fp = getf(f)) == NULL) {
		u.u_error = EBADF;
		return;
	}
	sndfile(stq->wrq, fp);
}

sndfile(q, fp)
register struct queue *q;
register struct file *fp;
{
	register struct block *bp;

	for (;;) {
		if (q==NULL) {
			u.u_error = ENXIO;
			return(0);
		}
		if (q->qinfo == &strdata)
			break;
		/* follow pt's here */
		q = q->next;
	}
	if (q->flag&QFULL) {
		u.u_error = ENOMEM;
		return(0);
	}
	if ((bp = allocb(sizeof(struct kpassfd))) == NULL) {
		u.u_error = ENOMEM;
		return(0);
	}
	bp->type = M_PASS;
	((struct kpassfd *)bp->rptr)->uid = u.u_uid;
	((struct kpassfd *)bp->rptr)->gid = u.u_gid;
	((struct kpassfd *)bp->rptr)->nice = u.u_procp->p_nice-NZERO;
	bcopy(u.u_logname,((struct kpassfd *)bp->rptr)->logname,
		sizeof(u.u_logname));
	((struct kpassfd *)bp->rptr)->f.fp = fp;
	bp->wptr += sizeof(struct kpassfd);
	fp->f_count++;
	strput(q, bp);
	return(1);
}

urcvfile(stq, arg)
register struct stdata *stq;
caddr_t arg;
{
	register struct block *bp;
	register i;
	register struct file *fp;
	register struct kpassfd *kp;
	struct passfd pfd;

	while ((bp = getq(RD(stq->wrq))) == NULL) {
		if (stq->flag&HUNGUP) {
			u.u_error = ENXIO;
			return;
		}
		stq->flag |= RSLEEP;
		if (tsleep((caddr_t)RD(stq->wrq), STIPRI, 0)==TS_SIG) {
			stexit(stq->inode);
			longjmp(u.u_qsav);
		}
	}
	if (bp->type != M_PASS) {
		putbq(RD(stq->wrq), bp);
		u.u_error = EIO;
		return;
	}
	kp = (struct kpassfd *)bp->rptr;
	fp = kp->f.fp;
	i = ufalloc();
	if (i < 0) {
		closef(fp);
		freeb(bp);
		return;
	}
	u.u_ofile[i] = fp;
	pfd.fd = i;
	pfd.uid = kp->uid;
	pfd.gid = kp->gid;
	bcopy(kp->logname, pfd.logname, sizeof(kp->logname));
	if (copyout((caddr_t)&pfd, arg, sizeof(struct passfd)))
		u.u_error = EFAULT;
	freeb(bp);
}

stselect(stp, rw, anyyet)
register struct stdata *stp;
{
	register ret = 0;
	register s = spl6();
	extern int selwait;

	if (rw==FWRITE) {
		if ((stp->wrq->next->flag&QFULL) == 0)
			ret = 1;
		else if (!anyyet) {
			if (stp->wsel && stp->wsel->p_wchan==(caddr_t)&selwait)
				stp->flag |= WSEL;
			else
				stp->wsel = u.u_procp;
		}
	} else {
		if (streadable(RD(stp->wrq)) || stp->flag&HUNGUP)
			ret = 1;
		else if (!anyyet) {
			if (stp->rsel && stp->rsel->p_wchan==(caddr_t)&selwait)
				stp->flag |= RSEL;
			else
				stp->rsel = u.u_procp;
		}
	}
	splx(s);
	return(ret);
}

/*
 * poison the flags in the file table leading to this stream
 */
forceclose(sptr, whichflags)
register struct stdata *sptr;
{
	register struct file *fp;

	for (fp = &file[0]; fp < fileNFILE; fp++) {
		if (fp->f_count==0)
			continue;
		if (fp->f_inode==NULL || fp->f_inode->i_sptr != sptr)
			continue;
		fp->f_flag &= ~whichflags;
		fp->f_flag |= FHUNGUP;
	}
}

/*
 * check if a queue is likely to be readable
 */
streadable(q)
register struct queue *q;
{
	register struct block *bp;

	if (q->first == 0)
		return(0);
	if ((backq(q)->flag&QDELIM) == 0 || q->count >= q->qinfo->lolimit)
		return(1);
	for (bp = q->first; bp; bp = bp->next)
		if (bp->class&S_DELIM || bp->type==M_PASS)
			return(1);
	return(0);
}


/*
 * Black hole to seal off closed pipes, and serve as a stub device.
 */
long
nilopen(q, dev)
struct queue *q;
dev_t dev;
{
	return(1);
}

nilput(q, bp)
struct queue *q;
struct block *bp;
{
	if (bp->type == M_IOCTL) {
		bp->type = M_IOCNAK;
		bp->wptr = bp->rptr;
		qreply(q, bp);
		return;
	}
	freeb(bp);
}
