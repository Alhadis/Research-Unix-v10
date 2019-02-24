#include "sys/param.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"

extern struct ipif ipif[];
extern struct ipif *ipifsort[];
extern int ipcnt;			/* number of ip's */
extern long time;

/*
 *  decoding the top two bits of an internet address
 */
u_long in_class_nmask[4] = {
	IN_CLASSA_NET,		/* 00 */
	IN_CLASSA_NET,		/* 01 */
	IN_CLASSB_NET,		/* 10 */
	IN_CLASSC_NET		/* 11 */
};
u_long in_class_hmask[4] = {
	IN_CLASSA_HOST,		/* 00 */
	IN_CLASSA_HOST,		/* 01 */
	IN_CLASSB_HOST,		/* 10 */
	IN_CLASSC_HOST		/* 11 */
};

struct block *
bp_get()
{
#if BLKOWNER
	int			x;		/* for debuging only */
	extern struct block	cblock[];
	extern long		blkowner[];
#endif
	register struct block *bp;

	bp = allocb(64);
	if(bp) {
		bp->next = 0;
#if BLKOWNER
		blkowner[bp-cblock] = *(&x+5);
#endif
	}
	return(bp);
}

bp_check(bp)
register struct block *bp;
{
	while(bp){
		BLOCKCHK(bp);
		bp = bp->next;
	}
}

/* bp_pullup: make the first block have at least len bytes */
struct block *
bp_pullup(bp, len)
register struct block *bp;
register int len;
{
	register struct block *newbp;
	register int count, wanted;

	MCHECK(bp);
	if (BLEN(bp) >= len)
		return (bp);
	wanted = len;	/* debugging */
	if ((newbp = allocb(len)) == NULL) {
		printf("allocb(%d)", len);
		goto bad;
	}
	if (newbp->lim - newbp->wptr < len) {
		printf("allocb(%d) got %d", len, newbp->lim - newbp->wptr);
		goto bad;
	}
	newbp->next = bp;
	while (len > 0 && bp) {
		count = bp->wptr - bp->rptr;
		if (count > len)
			count = len;
		bcopy(bp->rptr, newbp->wptr, count);
		newbp->wptr += count;
		len -= count;
		bp->rptr += count;
		if (bp->rptr >= bp->wptr) {
			newbp->next = bp->next;
			freeb(bp);
			bp = newbp->next;
		}
	}
	if (len) {
		printf("for %d left %d", wanted, len);
		goto bad;
	}
	return (newbp);

bad:
	printf(": bp_pullup err\n");
	if (newbp)
		bp_free(newbp);
	return (NULL);
}

bp_free(bp)
register struct block *bp;
{
	register struct block *p;

	while(bp){
		p = bp->next;
		BLOCKCHK(bp);
		freeb(bp);
		bp = p;
	}
}

struct block *
bp_copy(m, off, len)
register struct block *m;
int off;
register int len;
{
	register struct block *n, **np;
	struct block *top;
	register int clen;

	MCHECK(m);
	if(len == 0)
		return(0);
	if(off < 0 || len < 0)
		panic("m_copy");
	while(off > 0){
		if(m == 0)
			panic("m_copy 1");
		if(off < BLEN(m))
			break;
		off -= BLEN(m);
		m = m->next;
	}
	np = &top;
	top = 0;
	while(len > 0){
		if(m == 0)
			panic("m_copy 2");
		n = allocb(len);
		*np = n;
		if(n == 0)
			goto nospace;
		n->next = 0;
		np = &n->next;
		do {
			clen = len;
			if (BLEN(m) - off < clen)
				clen = BLEN(m) - off;
			if (n->lim - n->wptr < clen)
				clen = n->lim - n->wptr;
			bcopy((caddr_t)m->rptr+off, (caddr_t)n->wptr, clen);
			n->wptr += clen;
			len -= clen;
			if (len <= 0) {
				MCHECK(top);
				return (top);
			}
			if (m->rptr + off + clen < m->wptr)
				off += clen;
			else {
				off = 0;
				m = m->next;
			}
		} while (n->wptr < n->lim);
	}
	MCHECK(top);
	return(top);
nospace:
	bp_free(top);
	return(0);
}

bp_adj(m, len)
register struct block *m;
register int len;
{

	if (m == NULL)
		return;
	if (len > 0) {
		while(m && len > 0){
			if(BLEN(m) <= len){
				len -= BLEN(m);
				m->wptr = m->rptr;
				m = m->next;
			} else {
				m->rptr += len;
				break;
			}
		}
	}
	else if (len < 0) {
		len = bp_len(m) + len;
		if (len <= 0) {
			if (m->next)
				bp_free(m->next);
			m->next = 0;
			m->wptr = m->rptr;
			return;
		}
		while ((len -= BLEN(m)) > 0) {
			if ((m = m->next) == NULL)
				return;
		}
		m->wptr += len;		/* len is <= 0 */
		bp_free(m->next);
		m->next = 0;
	}
}

bp_cat(m, n)
register struct block *m, *n;
{
	struct block *xn;

	while(m->next)
		m = m->next;
	while(n){
		if((m->wptr + BLEN(n)) >= m->lim){
			/* just join the two chains */
			m->next = n;
			break;
		}
		/* splat the data from one into the other */
		bcopy(n->rptr, m->wptr, BLEN(n));
		m->wptr += BLEN(n);
		xn = n->next;
		freeb(n);
		n = xn;
	}
}

/* C version of 4.2bsd's Internet checksum routine */
/* This version assumes that no message exceeds 2^16 words */
in_cksum(bp, len)
	register struct block *bp;
	register int len;
{
	register u_short *w;
	register u_long sum = 0;
	register int mlen = 0;
	register int seen = 0;

	MCHECK(bp);
	for (; seen<len; bp=bp->next) {
		if (bp == NULL) {
			printf("cksum: out of data\n");
			break;
		}
		w = (u_short *) bp->rptr;
		if (seen & 01) {
			/* this block begins with an odd numbered byte */
			sum += *(u_char *)w << 8;
			w = (u_short *)((char *)w + 1);
			mlen = BLEN(bp) - 1;
			len--;
		} else
			mlen = BLEN(bp);
		if ((len-seen) < mlen)
			mlen = len-seen;
		seen += mlen;
		/* vecadd returns a 16-bit checksum of the block + sum */
		sum = vecadd(w, mlen, sum);
	}
	/* return complement of sum */
	return sum^0xffff;
}

in_addr
in_netof(x)
	register in_addr x;
{
	register struct ipif *ifp;
	register int i;
	register in_addr net, mask;
	register int Ipcnt = ipcnt;	/* optimization */

	/*
	 *  look for an interface on same subnet and use its subnet mask
	 */
	for(i=0; i < Ipcnt; i++){
		if((ifp = ipifsort[i])==0)
			break;
		if(ifp->flags & IFF_HOST)
			continue;
		if((x&ifp->mask) == ifp->that)
			return ifp->that;
	}

	/*
	 *  look for an interface on same net and use its subnet mask
	 */
	mask = IN_CLASS_NMASK(x);
	net = x&mask;
	for(i=0; i < Ipcnt; i++){
		if((ifp = ipifsort[i])==0)
			break;
		if(ifp->flags & IFF_HOST)
			continue;
		if(net == (ifp->that&mask))
			return x&ifp->mask;
	}

	/*
	 *  no interface for this network, assume no subnetting
	 */
	return net;
}

/*
 * Hash table for route entries.  Collision resolution is linear search until
 * encountering a hole.  Replacement is LRU.
 */
#define NROUTES 1024
struct ip_route ip_routes[NROUTES];
int Nip_route = NROUTES;		/* let netstat know number of routes */
#define HASH(x) (((x)+((x)>>8))%NROUTES)
struct ip_route ip_default_route;

ip_doroute(dst, gate)
	in_addr dst, gate;
{
	register struct ip_route *rp, *sp, *op;
	register struct ipif *ifp;
	register struct ipif *ifend;

	/* default is a special case */
	if (dst == 0){
		ip_default_route.gate = gate;
		return(0);
	}

	/* look for what should be a noop */
	if(gate){
		/* don't accept an indirect route, if we have a direct one */
		ifend = &ipif[ipcnt];
		for(ifp = ipif; ifp < ifend; ifp++){
			if((ifp->flags&IFF_UP) && ifp->that==dst)
				return(0);
		}
	} else
		gate = dst;

	/* look through existing routes */
	op = sp = rp = &ip_routes[HASH(dst)];
	do {
		if (rp->dst==0 || rp->dst==dst) {
			op = rp;
			break;
		}
		if (rp->time<op->time)
			op = rp;
		if (++rp==&ip_routes[NROUTES])
			rp = ip_routes;
	} while (rp != sp);

	/* add a new route */
	op->dst = dst;
	op->gate = gate;
	op->time = time;
	return(0);
}

/*
 *  Look for a route in the hash table.
 */
struct ip_route_info
ip_route(dst)
	in_addr dst;
{
	register struct ip_route *rp, *sp;
	extern unsigned long in_netof();
	unsigned long netof_dst;
	struct ip_route_info info;

	/* try a network to which we are directly connected */
	info.addr = dst;
	info.ifp = ip_ifonnetof(dst);
	if (info.ifp)
		return info;

	/* look for host routes */
	sp = rp = &ip_routes[HASH(dst)];
	do {
		if (dst==rp->dst && rp->dst!=rp->gate) {
			info.addr = rp->gate;
			info.ifp = ip_ifonnetof(info.addr);
			rp->time = time;
			return(info);
		}
		if (rp->dst==0)
			break;
		if (++rp==&ip_routes[NROUTES])
			rp = ip_routes;
	} while (rp != sp);

	/* now try nets */
	netof_dst = in_netof(dst);
	sp = rp = &ip_routes[HASH(netof_dst)];
	do {
		if (netof_dst==rp->dst && rp->dst!=rp->gate) {
			info.addr = rp->gate;
			info.ifp = ip_ifonnetof(info.addr);
			rp->time = time;
			return(info);
		}
		if (rp->dst==0)
			break;
		if (++rp==&ip_routes[NROUTES])
			rp = ip_routes;
	} while (rp != sp);

	/* if all else fails, use default route */
	/* N.B.  If the gate is a network, don't change the destination
	 *	address.  This allows multiple networks on one wire by
	 *	making that wire the default.
	 */
	if (ip_default_route.gate!=in_netof(ip_default_route.gate))
		info.addr = ip_default_route.gate;
	else
		info.addr = dst;
	info.ifp = ip_ifonnetof(ip_default_route.gate);
	return(info);
}

bp_len(bp)
register struct block *bp;
{
	register int n;

	n = 0;
	while(bp){
		n += BLEN(bp);
		bp = bp->next;
	}
	return(n);
}

bp_putback(q, list)
struct queue *q;
struct block *list;
{
	register struct block *bp;
	register struct block *prev, *next;

	/*
	 * reverse the list, to keep data in order
	 */
	prev = next = NULL;
	for (bp = list; bp; bp = next) {
		next = bp->next;
		bp->next = prev;
		prev = bp;
	}
	for (bp = prev; bp; bp = next) {
		next = bp->next;
		putbq(q, bp);
	}
}

in_addr
ip_hoston(dst)
in_addr dst;
{
	struct ip_route_info info;

	info = ip_route(dst);
	if(info.ifp == 0)
		return(0);
	return(info.ifp->thishost);
}

/*
 *  return the host part of the address
 */
in_lnaof(i)
	register u_long i;
{
	return i&IN_CLASS_HMASK(i);
}

/*
 *  return true if this is a broadcast address
 */
in_broadcast(ifp, a)
	register struct ipif *ifp;
	register u_long a;
{
	int i;

	for(i = 0; i < 6; i++)
		if(a == ifp->bcast[i])
			return 1;
	return 0;
}
