#include "sys/param.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"

int ip_errors[8];

ip_output(m, opt, flags)
	struct block *m;
	struct block *opt;
	int flags;
{
	register struct ip *ip = (struct ip *)(m->rptr);
	register struct ipif *ifp;
	int len, hlen = sizeof (struct ip), off;
	in_addr dst;
	struct ip_route_info info;

	if (opt)				/* XXX */
		(void) freeb(opt);		/* XXX */
	/*
	 * Fill in IP header.
	 */
	ip->ip_hl = hlen >> 2;
	if ((flags & IP_FORWARDING) == 0) {
		ip->ip_v = IPVERSION;
		ip->ip_off &= IP_DF;
		ip->ip_id = htons(ip_id++);
	}

	dst = ip->ip_dst;
	
	/*
	 * If routing to interface only,
	 * short circuit routing lookup.
	 */
	if (flags & IP_ROUTETOIF) {
		ifp = ip_ifonnetof(dst);
		if (ifp == 0)
			goto done;
		goto gotif;
	}
	info = ip_route(dst);
	if(info.ifp == 0){
		ipstat.ips_route++;
		goto done;
	}
	ifp = info.ifp;
	dst = info.addr;

gotif:
#ifndef notdef
	/*
	 * If source address not specified yet, use address
	 * of outgoing interface.
	 */
	if (in_lnaof(ip->ip_src) == INADDR_ANY)
		ip->ip_src = ifp->thishost;
#endif

	/*
	 * Look for broadcast address
	 * and verify user is allowed to send
	 * such a packet.
	 */
	if (in_broadcast(ifp, dst)) {
		/* don't allow broadcast messages to be fragmented */
		if (ip->ip_len > ifp->mtu) {
			ip_errors[0]++;
			goto done;
		}
	}

	/*
	 * If small enough for interface, can just send directly.
	 */
	if (ip->ip_len <= ifp->mtu) {
		ip->ip_src = htonl(ip->ip_src);
		ip->ip_dst = htonl(ip->ip_dst);
		ip->ip_len = htons((u_short)ip->ip_len);
		ip->ip_off = htons((u_short)ip->ip_off);
		ip->ip_sum = 0;
		ip->ip_sum = in_cksum(m, hlen);
		ip_ldout(m, dst, ifp);
		return;
	}
	ipstat.ips_fragout++;

	/*
	 * Too large for interface; fragment if possible.
	 * Must be able to put at least 8 bytes per fragment.
	 */
	if (ip->ip_off & IP_DF)
		goto done;
	len = (ifp->mtu - hlen) &~ 7;
	if (len < 8) {
		ip_errors[1]++;
		goto done;
	}

	/*
	 * Discard IP header from logical block for b_copy's sake.
	 * Loop through length of segment, make a copy of each
	 * part and output.
	 */
	m->rptr += sizeof (struct ip);
	for (off = 0; off < ip->ip_len-hlen; off += len) {
		struct block *mh = bp_get();
		struct ip *mhip;

		if (mh == 0)
			goto done;
		mh->next = 0;
		mh->rptr = mh->lim - hlen;
		mhip = (struct ip *)mh->rptr;
		*mhip = *ip;
		if (hlen > sizeof (struct ip)) {
			int olen = ip_optcopy(ip, mhip, off);
			mh->wptr = mh->rptr + sizeof (struct ip) + olen;
		} else
			mh->wptr = mh->rptr + sizeof (struct ip);
		mhip->ip_off = off >> 3;
/* HOMER fix: */
		if(ip->ip_off & IP_MF)
			mhip->ip_off |= IP_MF;
/* :xif REMOH */
		if (off + len >= ip->ip_len-hlen)
			len = mhip->ip_len = ip->ip_len - hlen - off;
		else {
			mhip->ip_len = len;
			mhip->ip_off |= IP_MF;
		}
		mhip->ip_len += sizeof (struct ip);
		mhip->ip_len = htons((u_short)mhip->ip_len);
		mh->next = bp_copy(m, off, len);
		if (mh->next == 0) {
			(void) freeb(mh);
			goto done;
		}
		mhip->ip_off = htons((u_short)mhip->ip_off);
		mhip->ip_src = htonl(mhip->ip_src);
		mhip->ip_dst = htonl(mhip->ip_dst);
		mhip->ip_sum = 0;
		mhip->ip_sum = in_cksum(mh, hlen);
		if (ip_ldout(mh, dst, ifp))
			break;
	}
done:
	bp_free(m);
}

/*
 * Copy options from ip to jp.
 * If off is 0 all options are copied
 * otherwise copy selectively.
 */
ip_optcopy(ip, jp, off)
	struct ip *ip, *jp;
	int off;
{
	register u_char *cp, *dp;
	int opt, optlen, cnt;

	cp = (u_char *)(ip + 1);
	dp = (u_char *)(jp + 1);
	cnt = (ip->ip_hl << 2) - sizeof (struct ip);
	for (; cnt > 0; cnt -= optlen, cp += optlen) {
		opt = cp[0];
		if (opt == IPOPT_EOL)
			break;
		if (opt == IPOPT_NOP)
			optlen = 1;
		else
			optlen = cp[1];
		if (optlen > cnt)			/* XXX */
			optlen = cnt;			/* XXX */
		if (off == 0 || IPOPT_COPIED(opt)) {
			bcopy((caddr_t)cp, (caddr_t)dp, (unsigned)optlen);
			dp += optlen;
		}
	}
	for (optlen = dp - (u_char *)(jp+1); optlen & 0x3; optlen++)
		*dp++ = IPOPT_EOL;
	return (optlen);
}
