/*
 * ip line discipline, to be pushed on an ethernet controller.
 * collects data till a delim, passes it to ip_input().
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/inet/in.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/ethernet.h"

extern struct ipif ipif[];
extern struct ipif *ipifsort[];
extern int ipcnt;			/* number of ip's */
extern int arpcnt;

int	 ipiput(), ipisrv(), ipclose();
long	ipopen();
int	iposrv();
static	struct qinit iprinit = { ipiput, ipisrv, ipopen, ipclose, IP_MSG_LIMIT, 64};
static	struct qinit ipwinit = { putq, iposrv, ipopen, ipclose, 2*IP_MSG_LIMIT, 64 };
struct streamtab ipstream = { &iprinit, &ipwinit };

long
ipopen(q, dev)
register struct queue *q;
{
	static int timing;
	register struct ipif *fp, *ifend;

	if (q->ptr)
		return(1);
	if(!timing){
		timing = 1;
		ip_slowtimo();
	}
	ifend = &ipif[ipcnt];
	for (fp = ipif; fp < ifend; fp++)
		if (fp->queue == NULL)
			break;
	if (fp >= ifend)
		return(0);
	fp->queue = q;	/* that's the RD q */
	fp->flags = IFF_UP;
	fp->that = fp->thishost = 0;
	fp->ipackets = fp->opackets = fp->ierrors = fp->oerrors = 0;
	fp->mtu = 1500;
	fp->arp = -1;
	fp->dev = dev;
	q->flag |= QDELIM;
	WR(q)->flag |= QDELIM;
	q->ptr = (caddr_t)fp;
	WR(q)->ptr = (caddr_t)fp;
	q->flag |= QNOENB;	/* ipiput calls qenable() */
	return(1);
}

ipclose(q)
struct queue *q;
{
	register struct ipif *ifp;

	ifp = (struct ipif *)q->ptr;
	ip_ifremove(ifp);
	ifp->queue = 0;
	ifp->flags = 0;
}

ipisrv(q)
register struct queue *q;
{
	register struct block *bp, *head, *tail;
	register struct ipif *ifp;

	/* there is now a whole packet waiting
	 * on this queue; strip it off and pass to ip_input().
	 * things other than data or delims are forwarded directly
	 * by ipiput().
	 */
	head = tail = (struct block *) 0;
	ifp = (struct ipif *)q->ptr;
	while(bp = getq(q)){
		if (bp->type != M_DATA)
			panic("ipisrv");
		bp->next = NULL;
		if (head == NULL)
			head = bp;
		else
			tail->next = bp;
		tail = bp;
		if (bp->class&S_DELIM) {
			bp->class &=~ S_DELIM;
			MCHECK(head);
			if((ifp->flags & IFF_ARP)
			   && (head->wptr-head->rptr) >= sizeof(struct etherpup)){
				/* strip off ether header */
				head->rptr += sizeof(struct etherpup);
			}
			ip_input(head);
			ifp->ipackets++;
			head = tail = NULL;
		}
	}
	if(head)
		bp_putback(q, head);
}


ipiput(q, bp)
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


/*
 *  set our many broadcast addresses
 */
static
setbcast(ifp)
	struct ipif *ifp;
{
	int i;

	/* official style */
	ifp->bcast[0] = ifp->that|~ifp->mask;			/* subnet */
	ifp->bcast[1] = ifp->that|~IN_CLASS_NMASK(ifp->that);	/* net */
	ifp->bcast[2] = 0xffffffff;				/* undirected */

	/* old BSD style */
	ifp->bcast[3] = ifp->that&ifp->mask;			/* subnet */
	ifp->bcast[4] = ifp->that&IN_CLASS_NMASK(ifp->that);	/* net */
	ifp->bcast[5] = 0;					/* undirected */

	if(ifp->flags & IFF_HOST)
		for(i = 0; i < 6; i++)
			ifp->bcast[i] = 0;
}

iposrv(q)
register struct queue *q;
{
	struct x{
		unsigned int in;
		unsigned char en[6];
	} *xp;
	register struct block *bp;
	register struct ipif *ifp;
	register long *intp;

	ifp = (struct ipif *)q->ptr;
	while(bp = getq(q)){
		switch (bp->type) {
		case M_IOCTL:
			switch(stiocom(bp)){
			case IPIOARP:
				ifp->flags |= IFF_ARP;
				bp->type = M_IOCACK;
				bp->wptr = bp->rptr;
				qreply(q, bp);
				break;
			case IPIORESOLVE:
				xp = (struct x *)(stiodata(bp));
				if (arpcnt > 0)
					arp_install(xp->in, xp->en);
				bp->wptr = bp->rptr;
				bp->type = M_IOCACK;
				qreply(q, bp);
				break;
			case IPIOHOST:
				intp = (long *)(stiodata(bp));
				ifp->that = *intp;
				ifp->flags |= IFF_HOST;
				ifp->mask = 0xffffffff;
				setbcast(ifp);
				ip_ifinsert(ifp);
				bp->type = M_IOCACK;
				qreply(q, bp);
				ip_doroute(ifp->that, 0);
				break;
			case IPIOMTU:
				intp = (long *)(stiodata(bp));
				ifp->mtu = *intp;
				bp->type = M_IOCACK;
				qreply(q, bp);
				break;
			case IPIONET:
				intp = (long *)(stiodata(bp));
				ifp->that = *intp;
				ifp->mask = IN_CLASS_NMASK(ifp->that);
				setbcast(ifp);
				ip_ifinsert(ifp);
				ifp->flags &= (~IFF_HOST);
				bp->type = M_IOCACK;
				qreply(q, bp);
				ip_doroute(ifp->that, 0);
				break;
			case IPIOMASK:
				intp = (long *)(stiodata(bp));
				/*
				 *  the mask has to be a superset of the class mask
				 */
				if((*intp&ifp->mask)==ifp->mask)
					ifp->mask = *intp;
				setbcast(ifp);
				ip_ifinsert(ifp);
				bp->type = M_IOCACK;
				qreply(q, bp);
				break;
			case IPIOLOCAL:
				intp = (long *)(stiodata(bp));
				ifp->thishost = *intp;
				setbcast(ifp);
				bp->type = M_IOCACK;
				qreply(q, bp);
				break;
			default:
				(*q->next->qinfo->putp)(q->next, bp);
				break;
			}
			continue;

		default:
			if (q->next->flag & QFULL) {
				putbq(q, bp);
				return;
			}
			if(bp->class&S_DELIM)
				ifp->opackets++;
			(*q->next->qinfo->putp)(q->next, bp);
			continue;
		}
	}
}

/*
 *  Insert an entry into ipifsort.  Entries are sorted by mask length,
 *  longest first.
 */
ip_ifinsert(ifp)
	struct ipif *ifp;
{
	int s = spl6();
	register int i, j;

	/*
	 *  First try to remove it.  This may be a reordering.
	 */
	ip_ifremove(ifp);

	/*
	 *  Now (re)insert it in the correct place
	 */
	for(i=0; i<ipcnt; i++){
		if(ipifsort[i]==ifp)
			panic("ip_ifinsert duplcate");
		if(ipifsort[i]==0 || (ifp->mask & ipifsort[i]->mask)!=ifp->mask)
			break;
	}
	if(i>=ipcnt)
		panic("ip_ifinsert no room");
	for(j=ipcnt-1; j>i; j--)
		ipifsort[j] = ipifsort[j-1];
	ipifsort[i] = ifp;
	splx(s);
}

/*
 *  Remove an entry from ipifsort.  Compress list to fill gap.
 *  It may not already be there.
 */
ip_ifremove(ifp)
	struct ipif *ifp;
{
	int s = spl6();
	register int i;

	for(i=0; i<ipcnt; i++)
		if(ipifsort[i]==ifp)
			break;
	if(i<ipcnt){
		for(; i<ipcnt-1; i++)
			ipifsort[i] = ipifsort[i+1];
		ipifsort[i] = 0;
	}
	splx(s);
}

/*
 *  Find the interface to use for sending messages to `dst'.
 *  ipifsort is sorted into priority order;
 *  first match found is best.
 */
struct ipif *
ip_ifonnetof(dst)
	register unsigned long dst;
{
	extern ipprintfs;
	register struct ipif *ifp;
	register int i;
	register int Ipcnt = ipcnt;	/* optimization */

	/*
	 *  first look for a match against addresses
	 */
	for(i=0; i < Ipcnt; i++){
		if((ifp = ipifsort[i])==0)
			break;
		if(ifp->flags & IFF_UP)
			if((dst & ifp->mask) == ifp->that)
				return(ifp);
	}

	/*
	 *  now try to match against the local host's addresses
	 */
	ifp = ip_ifwithaddr(dst);
	if(ifp)
		return(ifp);

	/*
	 *  no match
	 */
	if(ipprintfs)
		printf("ifonnetof %x?\n", dst);
	return(0);
}

/*
 *  return the interface for which addr is a local address.  if non
 *  such exists, return 0.
 *
 *  This routine assumes that ipifsort is sorted in priority order
 *  so that the first match found is the best match.
 */
struct ipif *
ip_ifwithaddr(addr)
	register u_long addr;
{
	register int i, j;
	register u_long net, mask;
	register struct ipif *ifp;
	register int Ipcnt = ipcnt;	/* optimization */

	net = in_netof(addr);
	for(i=0; i < Ipcnt; i++){
		if((ifp = ipifsort[i])==0)
			break;
		if(ifp->flags & IFF_UP) {

			/* address of this host */
			if(addr == ifp->thishost)
				return(ifp);

			for(j = 0; j < 6; j++)
				if(addr == ifp->bcast[j])
					return(ifp);

			/* address on a network simulated by this node */
			if(net == ifp->thishost)
				return(ifp);
		}
	}
	return(0);
}

/*
 * output list bp onto interface ifp
 * for better buffering, put it on ip's queue first;
 * iposrv will dribble it out as there's room
 */
ip_ldout(bp, dst, ifp)
register struct block *bp;
unsigned long dst;		/* host byte order */
register struct ipif *ifp;
{
	extern struct block *arp_resolve();
	register struct block *bp1;
	register struct queue *q;

	if(ifp->queue == 0){
		printf("ifp but no queue in ip_ldout\n");
		bp_free(bp);
		return(0);
	}
	q = WR(ifp->queue);
	if(q->flag & QFULL){
		bp_free(bp);
		ifp->oerrors++;
		return(1);
	}
	if(arpcnt > 0 && (ifp->flags & IFF_ARP)){
		bp = arp_resolve(ifp->queue, bp, dst);
		if(bp == 0)
			return(1);
	}
	MCHECK(bp);
	while(bp){
		bp1 = bp->next;
		if (bp1==NULL)
			bp->class |= S_DELIM;
		(*q->qinfo->putp)(q, bp);
		bp = bp1;
	}
	return(0);
}
