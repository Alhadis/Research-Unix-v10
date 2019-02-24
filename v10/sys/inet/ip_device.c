/*
 * ip device driver; each minor device is one protocol.
 * so tcp would be placed on top of ip minor device #6.
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/inet/in.h"
#include "sys/inet/ip_var.h"

extern	int ipcnt;		/* number of ip devices */
long	ipdopen();
int	ipdclose(), ipdput(), ipdosrv();
static	struct qinit ipdrinit = { noput, NULL, ipdopen, ipdclose, 0, 0 };
	struct qinit ipdwinit = { ipdput, ipdosrv, ipdopen, ipdclose,
				  IP_BODY_LIMIT, 129 };
struct	streamtab ipdinfo = { &ipdrinit, &ipdwinit };

struct cdevsw ipcdev = cstrinit(&ipdinfo);

struct queue *ipdstate[256];
int ipprintfs;

long
ipdopen(q, dev)
register struct queue *q;
dev_t dev;
{
	dev = minor(dev);
	if(ipdstate[dev])
		return(0);
	ipdstate[dev] = q;
	q->ptr = (caddr_t)dev;
	q->flag |= QDELIM;
	WR(q)->ptr = (caddr_t)dev;
	WR(q)->flag |= QNOENB;
	return(1);
}

ipdclose(q)
register struct queue *q;
{
	ipdstate[(int)q->ptr] = 0;
}

ipdput(q, bp)
register struct queue *q;
register struct block *bp;
{
	struct foo{
		u_long dst;
		u_long gate;
	} foo;
	int i;
	struct block *bp1;

	switch(bp->type){
	case M_DATA:
		putq(q, bp);
		if (bp->class&S_DELIM)
			qenable(q);
		return;

	case M_IOCTL:
		bp->type = M_IOCACK;
		switch(stiocom(bp)){
		case IPIOROUTE:
			bcopy(stiodata(bp), &foo, sizeof(struct foo));
			if(ip_doroute(foo.dst, foo.gate))
				bp->type = M_IOCNAK;
			break;
		case IPIOGETIFS:
			i = *(int *)(stiodata(bp));
			if(i>=ipcnt) {
				bp->type = M_IOCNAK;
				break;
			}
			bp1 = allocb(sizeof(struct ipif));
			if (bp1 == 0) {
				bp->type = M_IOCNAK;
				break;
			}
			freeb(bp);
			bp = bp1;
			bp->type = M_IOCACK;
			bp->wptr = (u_char *)stiodata(bp);
			*(struct ipif *)(bp->wptr) = ipif[i];
			bp->wptr += sizeof(struct ipif);
			break;
		default:
			bp->type = M_IOCNAK;
			break;
		}
		qreply(q, bp);
		return;

	default:
		freeb(bp);
		return;
	}
}

ipdrint(bp, dev)
register struct block *bp;
unsigned dev;
{
	register struct block *bp1;
	register struct queue *nq;

	if ((nq = ipdstate[dev]) == NULL) {
		bp_free(bp);
		return;
	}
	nq = nq->next;	/* optimization */
	if(nq->flag&QFULL){
		bp_free(bp);
		if(ipprintfs)
			printf("ipdrint: QFULL\n");
		ipstat.ips_qfull++;
		return;
	}
	while(bp){
		bp1 = bp->next;
		if (bp1==NULL)
			bp->class |= S_DELIM;
		else
			bp->class &=~ S_DELIM;
		(*nq->qinfo->putp)(nq, bp);
		bp = bp1;
	}
}

ipdosrv(q)
struct queue *q;
{
	register struct block *bp, *tail, *head;

	head = tail = NULL;
	while(bp = getq(q)){
		if (bp->type != M_DATA)
			panic("ipdosrv");
		bp->next = NULL;
		if (head == NULL)
			head = bp;
		else
			tail->next = bp;
		tail = bp;
		if (bp->class&S_DELIM) {
			ip_output(head, (struct block *)0, 0);
			head = tail = NULL;
		}
	}
	if (head)
		bp_putback(q, head);
}
