/*	ip_input.c	6.1	83/08/16	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"

int	ipqmaxlen = 50;
struct	ipq	ipq;			/* ip reass. queue */

u_char	ipcksum = 1;
struct	block *ip_reass();

/*
 * Ip input routine.  Checksum and byte swap header.  If fragmented
 * try to reassamble.  If complete and fragment queue exists, discard.
 * Process options.  Pass to next level.
 */
ip_input(bp)
register struct block *bp;
{
	register struct ip *ip;
	register int i;
	int hlen;

	if (bp == 0)
		return;
	if (BLEN(bp) < sizeof (struct ip) &&
	    (bp = bp_pullup(bp, sizeof (struct ip))) == 0) {
		ipstat.ips_toosmall++;
		return;
	}
	ip = (struct ip *)bp->rptr;
	if ((hlen = ip->ip_hl << 2) > BLEN(bp)) {
		if ((bp = bp_pullup(bp, hlen)) == 0) {
			ipstat.ips_badhlen++;
			return;
		}
		ip = (struct ip *)bp->rptr;
	}
	if (ipcksum)
		if (ip->ip_sum = in_cksum(bp, hlen)) {
			ipstat.ips_badsum++;
			goto bad;
		}

	/*
	 * Convert fields to host representation.
	 */
	ip->ip_len = ntohs((u_short)ip->ip_len);
	if (ip->ip_len < hlen) {
		ipstat.ips_badlen++;
		goto bad;
	}
	ip->ip_id = ntohs(ip->ip_id);
	ip->ip_off = ntohs((u_short)ip->ip_off);

	/*
	 * Check that the amount of data in the buffers
	 * is as at least much as the IP header would have us expect.
	 * Trim blocks if longer than we expect.
	 * Drop packet if shorter than we expect.
	 */
	i = bp_len(bp) - ip->ip_len;
	if (i < 0) {
		ipstat.ips_tooshort++;
		goto bad;
	}
	if (i > 0)
		bp_adj(bp, -i);
	/*
	 * Process options and, if not destined for us,
	 * ship it on.  ip_dooptions returns 1 when an
	 * error was detected (causing an icmp message
	 * to be sent).
	 */
	ip->ip_dst = ntohl(ip->ip_dst);
	ip->ip_src = ntohl(ip->ip_src);
	if (hlen > sizeof (struct ip) && ip_dooptions(ip))
		return;

	if (ip_ifwithaddr(ip->ip_dst) == 0) {
		ip_forward(bp);
		return;
	}

	/*
	 * Adjust ip_len to not reflect header,
	 * set ip_tos if more fragments are expected,
	 * convert offset of this to bytes.
	 */
	ip->ip_len -= hlen;
	ip->ip_tos = 0;
	if (ip->ip_off & IP_MF)
		ip->ip_tos = 1;
	ip->ip_off <<= 3;

	/*
	 * If datagram marked as having more fragments
	 * or if this is not the first fragment,
	 * attempt reassembly; if it succeeds, proceed.
	 */
	bp = ip_reass(bp);
	if (bp == 0)
		return;
	ip = (struct ip *) bp->rptr;

	/*
	 * Switch out to protocol's input routine.
	 */
	ipdrint(bp, (unsigned int)(ip->ip_p));
	return;
bad:
	bp_free(bp);
}

/*
 * Take incoming datagram fragment and try to
 * reassemble it into whole datagram.
 */
#define ASIPQ(x) ((struct ipq *)(x)->rptr)
#define ASFRAG(x) ((struct ipasfrag *)(x)->rptr)

struct block *
ip_reass(bp)
	register struct block *bp;
{
	register struct block *fp;
	register struct ip *ip = (struct ip *) bp->rptr;
	register struct block *pqbp, *qbp, *nqbp;
	int i, next;

	/*
	 * Look for queue of fragments
	 * of this datagram.
	 */
	for(fp=ipq.next; fp!=(struct block *)0; fp=ASIPQ(fp)->next)
		if (ip->ip_id == ASIPQ(fp)->ipq_id &&
		    ip->ip_src == ASIPQ(fp)->ipq_src &&
		    ip->ip_dst == ASIPQ(fp)->ipq_dst &&
		    ip->ip_p == ASIPQ(fp)->ipq_p)
			break;

	/* if this datagram is not a fragment then toss out fragments
	 * for this connection.
	 */
	if (!ip->ip_tos && ip->ip_off == 0) {
		if(fp != (struct block *)0)
			ip_freef(fp);
		return(bp);
	}

	/*
	 * If first fragment to arrive, create a reassembly queue.
	 */
	if (fp == 0) {
		if ((fp = bp_get()) == NULL)
			goto dropfrag;
		fp->wptr += sizeof(struct ipq);	/* not really necessary */

		/* no fragments yet */
		fp->next = 0;

		/* insque(fp, &ipq); */
		if (ipq.next != (struct block *)0)
			ASIPQ(ipq.next)->prev = fp;
		else
			ipq.prev = fp;
		ASIPQ(fp)->next = ipq.next;
		ASIPQ(fp)->prev = 0;
		ipq.next = fp;

		ASIPQ(fp)->ipq_ttl = IPFRAGTTL;
		ASIPQ(fp)->ipq_p = ip->ip_p;
		ASIPQ(fp)->ipq_id = ip->ip_id;
		ASIPQ(fp)->ipq_src = ((struct ip *)ip)->ip_src;
		ASIPQ(fp)->ipq_dst = ((struct ip *)ip)->ip_dst;
		pqbp = 0;
		qbp = 0;
		goto insert;
	}

	/*
	 * Find a segment which begins at or after this one does.
	 */
	for (pqbp=(struct block *)0,qbp=fp->next; qbp!=(struct block *)0;
		 pqbp=qbp,qbp=ASFRAG(qbp)->ipf_next)
		if (ASFRAG(qbp)->ip_off >= ip->ip_off)
			break;

	/*
	 * If there is a preceding segment, it may provide some of
	 * our data already.  If so, drop the data from that preceding
	 * segment.  If it provides all of our data, drop us.
	 */
	if (pqbp!=(struct block *)0) {
		i = ASFRAG(pqbp)->ip_off + ASFRAG(pqbp)->ip_len - ip->ip_off;
		if (i > 0) {
			if (i >= ip->ip_len)
				goto dropfrag;
			ASFRAG(pqbp)->ip_len -= i;
			bp_adj(pqbp, -i);
		}
	}

	/*
	 * While we overlap succeeding segments trim us or,
	 * if they are completely covered, dequeue them.
	 */
	while (qbp!=0 && (i = (ip->ip_off + ip->ip_len) - ASFRAG(qbp)->ip_off) > 0) {
		if (i < ASFRAG(qbp)->ip_len) {
			ip->ip_len -= i;
			bp_adj(bp, -i);
			break;
		}
		if (pqbp == (struct block *)0)
			nqbp = fp->next = ASFRAG(qbp)->ipf_next;
		else
			nqbp = ASFRAG(pqbp)->ipf_next = ASFRAG(qbp)->ipf_next;
		bp_free(qbp);
		qbp = nqbp;
	}

insert:
	/*
	 * Stick new segment in its place;
	 * check for complete reassembly.
	 */
	if (pqbp == (struct block *)0) {
		ASFRAG(bp)->ipf_next = fp->next;
		fp->next = bp;
	} else {	
		ASFRAG(bp)->ipf_next = ASFRAG(pqbp)->ipf_next;
		ASFRAG(pqbp)->ipf_next = bp;
	}
	next = 0;
	for (pqbp=0,qbp=fp->next; qbp!=(struct block *)0;
		pqbp=qbp,qbp=ASFRAG(qbp)->ipf_next) {
		if (ASFRAG(qbp)->ip_off != next)
			return (0);
		next += ASFRAG(qbp)->ip_len;
	}
	if (pqbp != (struct block *)0 && ASFRAG(pqbp)->ip_tos)
		return (0);

	/*
	 * Reassembly is complete; concatenate fragments by removing all
	 * ip headers.
	 */
	ASFRAG(fp->next)->ip_len = next;
	for (qbp=ASFRAG(fp->next)->ipf_next; qbp!=(struct block *)0; qbp=nqbp) {
		nqbp = ASFRAG(qbp)->ipf_next;
		qbp->rptr += ((struct ip *)qbp->rptr)->ip_hl<<2;
		bp_cat(fp->next, qbp);
	}
	/*
	 * Create header for new ip packet by
	 * modifying header of first packet;
	 * dequeue and discard fragment reassembly header.
	 * Make header visible.
	 */
	bp = fp->next;
	((struct ip *)bp->rptr)->ip_src = ASIPQ(fp)->ipq_src;
	((struct ip *)bp->rptr)->ip_dst = ASIPQ(fp)->ipq_dst;
	((struct ip *)bp->rptr)->ip_tos = 0;
	fp->next = 0;
	ip_freef(fp);
	return (bp);

dropfrag:
	bp_free(bp);
	return (0);
}

/*
 * Free a fragment reassembly header and all
 * associated datagrams.
 */
ip_freef(fp)
	struct block *fp;
{
	register struct block *q, *p;

	for (p = fp->next; p != 0; p = q) {
		q = ASFRAG(p)->ipf_next;
		bp_free(p);
	}
	p = ASIPQ(fp)->prev;
	q = ASIPQ(fp)->next;
	if (p != (struct block *)0)
		ASIPQ(p)->next = q;
	else
		ipq.next = q;
	if (q != (struct block *)0)
		ASIPQ(q)->prev = p;
	else
		ipq.prev = p;
	freeb(fp);
}

/*
 * IP timer processing;
 * if a timer expires on a reassembly
 * queue, discard it.
 */
ip_slowtimo()
{
	register struct block *fp, *nfp;
	int s = spl6();

	for (fp=ipq.next; fp!=(struct block *)0; fp=nfp) {
		nfp = ASIPQ(fp)->next;
		if (--ASIPQ(fp)->ipq_ttl == 0)
			ip_freef(fp);
	}
	timeout(ip_slowtimo, (caddr_t)0, HZ);
	splx(s);
}

#if NOTDEF
/* who calls this? */
/*
 * Drain off all datagram fragments.
 */
ip_drain()
{

	while (ipq.next != (struct block *)0)
		ip_freef(ipq.next);
}
#endif

/*
 * Do option processing on a datagram,
 * possibly discarding it if bad options
 * are encountered.
 */
ip_dooptions(ip)
	struct ip *ip;
{
	register u_char *cp;
	int opt, optlen, cnt;

	cp = (u_char *)(ip + 1);
	cnt = (ip->ip_hl << 2) - sizeof (struct ip);
	for (; cnt > 0; cnt -= optlen, cp += optlen) {
		opt = cp[0];
		if (opt == IPOPT_EOL)
			break;
		if (opt == IPOPT_NOP)
			optlen = 1;
		else
			optlen = cp[1];
		switch (opt) {

		default:
			break;
#ifdef FAT_CHANCE
		/*
		 * Source routing with record.
		 * Find interface with current destination address.
		 * If none on this machine then drop if strictly routed,
		 * or do nothing if loosely routed.
		 * Record interface address and bring up next address
		 * component.  If strictly routed make sure next
		 * address on directly accessible net.
		 */
		case IPOPT_LSRR:
		case IPOPT_SSRR:
			if (cp[2] < 4 || cp[2] > optlen - (sizeof (long) - 1))
				break;
			sin = (struct in_addr *)(cp + cp[2]);
			ipaddr.sin_addr = *sin;
			ifp = if_ifwithaddr((struct sockaddr *)&ipaddr);
			type = ICMP_UNREACH, code = ICMP_UNREACH_SRCFAIL;
			if (ifp == 0) {
				if (opt == IPOPT_SSRR)
					goto bad;
				break;
			}
			t = ip->ip_dst; ip->ip_dst = *sin; *sin = t;
			cp[2] += 4;
			if (cp[2] > optlen - (sizeof (long) - 1))
				break;
			ip->ip_dst = sin[1];
			if (opt == IPOPT_SSRR &&
			    if_ifonnetof(in_netof(ip->ip_dst)) == 0)
				goto bad;
			break;

		case IPOPT_TS:
			code = cp - (u_char *)ip;
			type = ICMP_PARAMPROB;
			ipt = (struct ip_timestamp *)cp;
			if (ipt->ipt_len < 5)
				goto bad;
			if (ipt->ipt_ptr > ipt->ipt_len - sizeof (long)) {
				if (++ipt->ipt_oflw == 0)
					goto bad;
				break;
			}
			sin = (struct in_addr *)(cp+cp[2]);
			switch (ipt->ipt_flg) {

			case IPOPT_TS_TSONLY:
				break;

			case IPOPT_TS_TSANDADDR:
				if (ipt->ipt_ptr + 8 > ipt->ipt_len)
					goto bad;
				if (ifinet == 0)
					goto bad;	/* ??? */
				*sin++ = ((struct sockaddr_in *)&ifinet->if_addr)->sin_addr;
				break;

			case IPOPT_TS_PRESPEC:
				ipaddr.sin_addr = *sin;
				if (if_ifwithaddr((struct sockaddr *)&ipaddr) == 0)
					continue;
				if (ipt->ipt_ptr + 8 > ipt->ipt_len)
					goto bad;
				ipt->ipt_ptr += 4;
				break;

			default:
				goto bad;
			}
			*(n_time *)sin = iptime();
			ipt->ipt_ptr += 4;
#endif FATCHANCE
		}
	}
	return (0);
}

/*
 * Strip out IP options, at higher
 * level protocol in the kernel.
 * Second argument is buffer to which options
 * will be moved, and return value is their length.
 */
ip_stripoptions(bp, blopt)
	register struct block *bp, *blopt;
{
	struct ip *ip = (struct ip *) bp->rptr;
	register int i;
	int olen;

	olen = (ip->ip_hl<<2) - sizeof (struct ip);
	ip++;
	if (blopt) {
		blopt->wptr = blopt->base + olen;
		blopt->rptr = blopt->base;
		bcopy((caddr_t)ip, (caddr_t)blopt->rptr, (unsigned)olen);
	}
	i = BLEN(bp) - (sizeof (struct ip) + olen);
	bcopy((caddr_t)ip+olen, (caddr_t)ip, (unsigned)i);
	bp->wptr -= olen;
}

int	ipforwarding = 1;
extern	ipprintfs;
/*
 * Forward a packet.  If some error occurs return the sender
 * and icmp packet.  Note we can't always generate a meaningful
 * icmp message because icmp doesn't have a large enough repetoire
 * of codes and types.
 */
ip_forward(bp)
	register struct block *bp;
{
	register struct ip *ip = (struct ip *) bp->rptr;
	struct block *blopt;

	if(ipprintfs)
		printf("forward: src %x dst %x ttl %x\n", ip->ip_src,
			ip->ip_dst, ip->ip_ttl);
	if (ipforwarding == 0) {
		bp_free(bp);
		return;
	}
	if (ip->ip_ttl < IPTTLDEC) {
		bp_free(bp);
		return;
	}
	ip->ip_ttl -= IPTTLDEC;
	blopt = bp_get();
	if (blopt == NULL) {
		bp_free(bp);
		return;
	}
	blopt->next = 0;

	ip_stripoptions(bp, blopt);

	ip_output(bp, blopt, IP_FORWARDING);
}
