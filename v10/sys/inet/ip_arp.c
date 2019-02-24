#include "sys/param.h"
#include "sys/systm.h"
#include "sys/ethernet.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip_var.h"

/*
 * Address resolution code. ip_ldout() calls arp_resolve if IFF_ARP
 * is set to map an internet address into a 48 bit ethernet address.
 * If arp_resolve finds the address in its tables, it prepends an
 * ethernet header to the packet and returns it. Otherwise, it sends
 * a message to the ipconfig waiting on the specified queue asking
 * it to do all the hard work. The user process at some point will use
 * the IPIORESOLVE ioctl to update the tabes.
 *
 * N.B. If we ever get a medium other than ethernet that needs arping,
 *      this code will need to be made a bit more general.
 */

/*
 * Hash table for arp entries.  Collision resolution is linear search until
 * encountering a hole.  Replacement is LRU.
 */
/* #define NARP 128			/* number of arp entries */
extern int arpcnt;			/* number of arp entries */
extern struct ip_arp ip_arps[];		/* arping table */
#define HASH(x) ((x)%arpcnt)

arp_install(in, en)
	register unsigned long in;
	unsigned char *en;
{
	register struct ip_arp *ap, *rp, *op;

	/* find matching entry, empty entry, or oldest entry */
	op = ap = rp = &ip_arps[HASH(in)];
	do {
		if (ap->inaddr==0 || ap->inaddr==in) {
			op = ap;
			break;
		}
		if (ap->time < op->time)
			op = ap;
		if(++ap==&ip_arps[arpcnt])
			ap = ip_arps;
	} while(ap!=rp);

	/* fill in new entry */
	op->time = time;
	op->inaddr = in;
	bcopy((caddr_t)en, (caddr_t)op->enaddr, sizeof(op->enaddr));
	return(0);
}

struct block *
arp_resolve(q, bp, dst)
	struct queue *q;
	register struct block *bp;
	unsigned long dst;
{
	register struct block *bp1;
	struct etherpup *hp;
	register struct ip_arp *ap, *rp;

	/* find matching entry */
	ap = rp = &ip_arps[HASH(dst)];
	do {
		if (ap->inaddr==0 || ap->inaddr==dst)
			break;
		if(++ap==&ip_arps[arpcnt])
			ap = ip_arps;
	} while(ap!=rp);
	if (ap->inaddr!=dst) {
		arp_request(q, dst);	/* request an address resolution */
		bp_free(bp);		/* free the packet */
		return(0);
	}

	/* make a block with the ether info */
	bp1 = allocb(sizeof(struct etherpup));
	if(bp1 == 0){
		printf("no bp for arp_resolve\n");
		bp_free(bp);
		return(0);
	}
	ap->time = time;
	bp1->type = M_DATA;
	bp1->wptr = bp1->rptr + sizeof(struct etherpup);
	bp1->next = bp;
	hp = (struct etherpup *)(bp1->rptr);
	hp->type = htons(ETHERPUP_IPTYPE);
	bcopy((caddr_t)(ap->enaddr), (caddr_t)(hp->dhost), 6);
	return(bp1);
}

arp_request(q, dst)
register struct queue *q;
unsigned long dst;
{
	struct block *bp;

	if(q->next->flag & QFULL){
		printf("arp q full\n");
		return;
	}
	bp = allocb(4);
	if(bp == 0)
		return;
	bp->type = M_DATA;
	bp->wptr = bp->rptr + 4;
	bp->class |= S_DELIM;
	*((u_long *)(bp->rptr)) = dst;
	(*q->next->qinfo->putp)(q->next, bp);
}
