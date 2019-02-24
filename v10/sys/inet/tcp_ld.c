/*
 * tcp line discipline; only one, to be pushed on /dev/ip6.
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/inio.h"
#include "sys/conf.h"

#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/tcp.h"
#include "sys/inet/tcp_fsm.h"
#include "sys/inet/tcp_seq.h"
#include "sys/inet/tcp_timer.h"
#include "sys/inet/tcp_var.h"
#include "sys/inet/tcpip.h"

extern int tcp_busy;	/* set to discourage timers */
int tcp_maxseg = 4096-sizeof(struct tcpiphdr);	/* to fit in a 4k block */
struct queue *tcpqueue;

int	tcpiput(), tcpisrv(), tcpclose();
long	tcpopen();
int	tcposrv();
static	struct qinit tcprinit = { tcpiput, tcpisrv, tcpopen, tcpclose, 4096, 64 };
static	struct qinit tcpwinit = { putq, tcposrv, tcpopen, tcpclose, 512, 64 };
struct streamtab tcpstream = { &tcprinit, &tcpwinit };

long
tcpopen(q, dev)
register struct queue *q;
{
	static int timing;

	if (q->ptr)
		return(0);
	tcpqueue = q;	/* RD queue */
	if(!timing){
		timing = 1;
		tcp_fasttimo();
		tcp_slowtimo();
	}
	q->flag |= QDELIM;
	WR(q)->flag |= QDELIM;
	q->ptr = (caddr_t)1;
	WR(q)->ptr = (caddr_t)1;
	q->flag |= QNOENB;	/* ipiput calls qenable() */
	return(1);
}

tcpclose(q)
register struct queue *q;
{
	if(tcpqueue == q)
		tcpqueue = NULL;
}

/*
 * pass a whole packet to tcp_input
 */
tcpisrv(q)
register struct queue *q;
{
	register struct block *bp, *head, *tail;

	head = tail = NULL;
	while(bp = getq(q)){
		switch (bp->type) {
		case M_DATA:
			bp->next = NULL;
			if (head == NULL)
				head = bp;
			else
				tail->next = bp;
			tail = bp;
			if (bp->class&S_DELIM) {
				bp->class &=~ S_DELIM;
				tcp_busy++;
				MCHECK(head);
				tcp_input(head);
				--tcp_busy;
				head = tail = NULL;
			}
			break;

		default:
			panic("tcpisrv");
		}
	}
	if (head)
		bp_putback(q, head);
}

tcpiput(q, bp)
register struct queue *q;
register struct block *bp;
{
	switch(bp->type){
	case M_DATA:
		putq(q, bp);
		if (bp->class&S_DELIM)
			qenable(q);
		break;
	default:
		(*q->next->qinfo->putp)(q->next, bp);
		break;
	}
		
}

tcposrv(q)
register struct queue *q;
{
	register struct block *bp;

	while(bp = getq(q)){
		switch (bp->type) {
		case M_IOCTL:
			switch(stiocom(bp)){
			case TCPIOMAXSEG:
				tcp_maxseg = *(int *)stiodata(bp);
				bp->type = M_IOCACK;
				qreply(q, bp);
				continue;
			}
			/* default: fall through and put if room */

		default:
			if (q->next->flag & QFULL) {
				putbq(q, bp);
				return;
			}
			(*q->next->qinfo->putp)(q->next, bp);
			continue;
		}
	}
}

/*
 * hand list bp (an IP packet)
 * to the next stream module,
 * usually IP
 */

tcp_ldout(bp)
register struct block *bp;
{
	register struct block *bp1;
	register struct queue *nq;

	if(tcpqueue == NULL){
		bp_free(bp);
		return(1);
	}
	nq = WR(tcpqueue)->next;
	if(nq->flag&QFULL){
		printf("tcp_ldout: QFULL\n");
		bp_free(bp);
		return(1);
	}
	MCHECK(bp);
	while(bp){
		bp1 = bp->next;
		if (bp1==NULL)
			bp->class |= S_DELIM;
		(*nq->qinfo->putp)(nq, bp);
		bp = bp1;
	}
	return(0);
}
