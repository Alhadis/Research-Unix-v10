/*	tcp_subr.c	6.1	83/07/29	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/tcp.h"
#include "sys/inet/tcp_fsm.h"
#include "sys/inet/tcp_seq.h"
#include "sys/inet/tcp_timer.h"
#include "sys/inet/tcp_var.h"
#include "sys/inet/tcpip.h"
#include "sys/inet/tcpdebug.h"

extern int tcp_maxseg;

/*
 * Create template to be used to send tcp packets on a connection.
 * Call after host entry created, allocates a block and fills
 * in a skeletal tcp/ip header, minimizing the amount of work
 * necessary when the connection is used.
 */
tcp_template(tp)
	struct tcpcb *tp;
{
	register struct block *bp;
	register struct tcpiphdr *n;

	if ((bp=tp->t_template) == (struct block *)0)
		bp = bp_get();
	if (bp == NULL)
		return;
	tp->t_template = bp;
	bp->next = 0;
	bp->wptr += sizeof (struct tcpiphdr);
	n = (struct tcpiphdr *)bp->rptr;
	n->ti_bp = 0;
	n->ti_next = 0;
	n->ti_x1 = 0;
	n->ti_pr = 6;	/* tcp protocol number */
	n->ti_len = htons(sizeof (struct tcpiphdr) - sizeof (struct ip));
	n->ti_src = tp->so_laddr;
	n->ti_dst = tp->so_faddr;
	n->ti_sport = tp->so_lport;
	n->ti_dport = tp->so_fport;
	n->ti_seq = 0;
	n->ti_ack = 0;
	n->ti_x2 = 0;
	n->ti_off = 5;
	n->ti_flags = 0;
	n->ti_win = 0;
	n->ti_sum = 0;
	n->ti_urp = 0;
}

/*
 * Send a single message to the TCP at address specified by
 * the given TCP/IP header.  If flags==0, then we make a copy
 * of the tcpiphdr at ti and send directly to the addressed host.
 * This is used to force keep alive messages out using the TCP
 * template for a connection tp->t_template.  If flags are given
 * then we send a message back to the TCP which originated the
 * segment ti, and discard the block containing it and any other
 * attached blocks.
 *
 * In any case the ack and sequence number of the transmitted
 * segment are as specified by the parameters.
 */
tcp_respond(tp, ti, ack, seq, flags)
	struct tcpcb *tp;
	register struct tcpiphdr *ti;
	tcp_seq ack, seq;
	int flags;
{
	struct block *tibp;
	int win = 0, tlen;

	if (tp) {
		win = sbrcvspace(tp);
	}
	if (flags == 0) {
		tibp = bp_get();
		if (tibp == NULL)
			return;
		tibp->next = 0;
		tibp->wptr += sizeof (struct tcpiphdr) + 1;
		*(struct tcpiphdr *)(tibp->rptr) = *ti;
		ti = (struct tcpiphdr *)(tibp->rptr);
		ti->ti_bp = tibp;
		flags = TH_ACK;
		tlen = 1;
	} else {
		in_addr taddr;
		tcp_port tport;

		tibp = ti->ti_bp;
		bp_check(tibp);
		bp_free(tibp->next);
		tibp->next = 0;
		tibp->rptr = (u_char *)ti;
		tibp->wptr = tibp->rptr + sizeof (struct tcpiphdr);
		taddr = ti->ti_src;
		ti->ti_src = ti->ti_dst;
		ti->ti_dst = taddr;
		tport = ti->ti_sport;
		ti->ti_sport = ti->ti_dport;
		ti->ti_dport = tport;
		tlen = 0;
	}
	ti->ti_next = 0;
	ti->ti_bp = 0;
	ti->ti_x1 = 0;
	ti->ti_pr = 6;	/* tcp protocol number */
	ti->ti_len = (u_short)(sizeof (struct tcphdr) + tlen);
	ti->ti_len = htons(ti->ti_len);
	ti->ti_seq = htonl(seq);
	ti->ti_ack = htonl(ack);
	ti->ti_x2 = 0;
	ti->ti_off = sizeof (struct tcphdr) >> 2;
	ti->ti_flags = flags;
	ti->ti_win = htons((u_short)win);
	ti->ti_urp = 0;
	ti->ti_src = htonl(ti->ti_src);
	ti->ti_dst = htonl(ti->ti_dst);
	ti->ti_sport = htons(ti->ti_sport);
	ti->ti_dport = htons(ti->ti_dport);
	ti->ti_sum = 0;
	ti->ti_sum = in_cksum(tibp, sizeof(struct tcpiphdr) + tlen);
	tcp_debug(ti, 1);
	((struct ip *)ti)->ip_len = sizeof (struct tcpiphdr) + tlen;
	((struct ip *)ti)->ip_ttl = TCP_TTL;
	ti->ti_dst = ntohl(ti->ti_dst);
	ti->ti_src = ntohl(ti->ti_src);
	tcp_ldout(tibp);
}

/*
 * Drop a TCP connection.
 * If connection is synchronized,
 * then send a RST to peer.
 */
struct tcpcb *
tcp_drop(tp)
	register struct tcpcb *tp;
{

	if (TCPS_HAVERCVDSYN(tp->t_state)) {
		tp->t_state = TCPS_CLOSED;
		(void) tcp_output(tp);
	}
	return (tcp_close(tp));
}

/*
 * Close a TCP control block:
 *	discard all space held by the tcp
 *	discard internet protocol block
 *	wake up any sleepers
 */
struct tcpcb *
tcp_close(tp)
	register struct tcpcb *tp;
{
	register struct tcpiphdr *ti, *nextti;

	if(!tp)
		panic("tcp_close");
	tcp_canceltimers(tp);
	ti = tp->seg_next;
	while (ti != (struct tcpiphdr *)0) {
		nextti = ti->ti_next;
		bp_free(ti->ti_bp);
		ti = nextti;
	}
	tp->seg_next = 0;
	if (tp->t_template)
		freeb(tp->t_template);
	tp->t_template = 0;
	if (tp->t_tcpopt)
		freeb(tp->t_tcpopt);
	tp->t_tcpopt = 0;
	if (tp->t_ipopt)
		freeb(tp->t_ipopt);
	tp->t_ipopt = 0;

/*	soisdisconnected(tp);	*/
	tp->so_state &= ~(SS_PLEASEOPEN|SS_RCVATMARK);
	if(tp->so_state&SS_OPEN && !(tp->so_state&SS_HUNGUP)) {
		tp->so_state |= SS_HUNGUP;
		tcp_hungup(tp);
	}
	tp->so_laddr = tp->so_faddr = 0;
	tp->so_lport = tp->so_fport = 0;
	tp->t_state = TCPS_CLOSED;
	return ((struct tcpcb *)0);
}

/*
 * For debugging save time event occurred, the direction of the message,
 * and the tcp header.  Then increment the pointer to the tcp
 * debug queue.
 */
int Nbugarr=SIZDEBUG;
struct tcpdebug bugarr[SIZDEBUG];	/* buffer to store the debug info in */
int tcpdbg_ind=0;			/* index into bugarr at last entry */
unsigned long tcpdbg_seq = 0;		/* sequence number */

tcp_debug(ti, code)
struct tcpiphdr *ti;
{
	register int i;
	i = tcpdbg_ind;
	bugarr[i].seq = tcpdbg_seq++;
	bugarr[i].stamp = time;
	bugarr[i].inout = code;
	bugarr[i].hdr = *ti;
	tcpdbg_ind = (i + 1) % SIZDEBUG;
}
