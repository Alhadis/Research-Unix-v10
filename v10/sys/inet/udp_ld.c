/*
 * udp line discipline; only one, to be pushed on /dev/ip17.
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"

#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/udp.h"
#include "sys/inet/udp_var.h"

struct queue *udpqueue;

int	udpiput(), udpisrv(), udpclose();
long	udpopen();
int	udposrv(), udpoput();
static	struct qinit udprinit = { udpiput, udpisrv, udpopen, udpclose, 
				  4*UDP_MSG_LIMIT, 512 };
static	struct qinit udpwinit = { putq, udposrv, udpopen, udpclose, 
				  UDP_MSG_LIMIT, 64 };
struct streamtab udpstream = { &udprinit, &udpwinit };

long
udpopen(q, dev)
register struct queue *q;
{
	if (q->ptr)
		return(0);
	udpqueue = q;	/* RD queue */
	q->flag |= QDELIM;
	WR(q)->flag |= QDELIM;
	q->ptr = (caddr_t)1;
	WR(q)->ptr = (caddr_t)1;
	q->flag |= QNOENB;	/* ipiput calls qenable() */
	return(1);
}

udpclose(q)
register struct queue *q;
{
	if(udpqueue == q)
		udpqueue = 0;
}

udpisrv(q)
register struct queue *q;
{
	register struct block *bp, *head, *tail;

	head = tail = NULL;
	while(bp = getq(q)){
		if (bp->type != M_DATA)
			panic("udpisrv");
		bp->next = NULL;
		if (head == NULL)
			head = bp;
		else
			tail->next = bp;
		tail = bp;
		if (bp->class&S_DELIM) {
			bp->class &=~ S_DELIM;
			MCHECK(head);
			udp_input(head);
			head = tail = NULL;
		}
	}
	if (head)
		bp_putback(q, head);
}


udpiput(q, bp)
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

udposrv(q)
register struct queue *q;
{
	register struct block *bp;

	while(bp = getq(q)){
		if (q->next->flag & QFULL) {
			putbq(q, bp);
			break;
		}
		(*q->next->qinfo->putp)(q->next, bp);
	}
}

udp_ldout(bp)
register struct block *bp;
{
	register struct block *bp1;
	register struct queue *q;

	if(udpqueue == 0){
		bp_free(bp);
		return;
	}
	q = WR(udpqueue);
	if(q->next->flag&QFULL){
		printf("udp_ldout: QFULL\n");
		bp_free(bp);
		return;
	}
	MCHECK(bp);
	while(bp){
		bp1 = bp->next;
		if (bp1==NULL)
			bp->class |= S_DELIM;
		(*q->next->qinfo->putp)(q->next, bp);
		bp = bp1;
	}
}
