/*	tcp_output.c	6.1	83/07/29	*/

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/tcp.h"
#define	TCPOUTFLAGS
#include "sys/inet/tcp_fsm.h"
#include "sys/inet/tcp_seq.h"
#include "sys/inet/tcp_timer.h"
#include "sys/inet/tcp_var.h"
#include "sys/inet/tcpip.h"

int tcp_busy;	/* set/unset by tcp_output callers, to keep out timer */

/*
 * Initial options.
 */
u_char	tcp_initopt[4] = { TCPOPT_MAXSEG, 4, 0x0, 0x0, };
extern	tcpprintfs;

/*
 *  Settable maximum tcp segment size.
 */
extern int tcp_maxseg;

/*
 * Tcp output routine: figure out what should be sent and send it.
 */
tcp_output(tp)
	register struct tcpcb *tp;
{
	register int len;
	struct block *bp0;
	int off, flags, win, error;
	register struct block *bp;
	register struct tcpiphdr *ti;
	u_char *opt;
	unsigned optlen = 0;
	int sendalot;

	/*
	 *  don't output if the template has been removed
	 */
	if (tp->t_template == 0)
		return (0);

	if(tcp_busy != 1)
		panic("tcp_busy");

	/*
	 * Determine length of data that should be transmitted,
	 * and flags that will be used.
	 * If there is some data or critical controls (SYN, RST)
	 * to send, then transmit; otherwise, investigate further.
	 */
again:
	sendalot = 0;
	off = tp->snd_nxt - tp->snd_una;
	len = MIN(sosndcc(tp), tp->snd_wnd+tp->t_force) - off;
	if (len < 0)
		return (0);	/* ??? */	/* past FIN */
	if (len > tp->t_maxseg) {
		len = tp->t_maxseg;
		sendalot = 1;
	}

	flags = tcp_outflags[tp->t_state];
	if (tp->snd_nxt + len < tp->snd_una + sosndcc(tp))
		flags &= ~TH_FIN;
	if (flags & (TH_SYN|TH_RST|TH_FIN))
		goto send;
	if (SEQ_GT(tp->snd_up, tp->snd_una))
		goto send;

	/*
	 * Sender silly window avoidance.  If can send all data,
	 * a maximum segment, at least 1/4 of window do it,
	 * or are forced, do it; otherwise don't bother.
	 */
	if (len) {
		if (len == tp->t_maxseg || off+len >= sosndcc(tp))
			goto send;
		if (len * 4 >= tp->snd_wnd)		/* a lot */
			goto send;
		if (tp->t_force)
			goto send;
	}

	/*
	 * Send if we owe peer an ACK.
	 */
	if (tp->t_flags&TF_ACKNOW)
		goto send;


	/*
	 * Calculate available window in i, and also amount
	 * of window known to peer (as advertised window less
	 * next expected input.)  If this is 35% or more of the
	 * maximum possible window, then want to send a segment to peer.
	 */
	win = sbrcvspace(tp);
	if (win > 0 &&
	    ((100*(win-(tp->rcv_adv-tp->rcv_nxt))/sorcvhiwat(tp)) >= 35))
		goto send;

	/*
	 * TCP window updates are not reliable, rather a polling protocol
	 * using ``persist'' packets is used to insure receipt of window
	 * updates.  The three ``states'' for the output side are:
	 *	idle			not doing retransmits or persists
	 *	persisting		to move a zero window
	 *	(re)transmitting	and thereby not persisting
	 *
	 * tp->t_timer[TCPT_PERSIST]
	 *	is set when we are in persist state.
	 * tp->t_force
	 *	is set when we are called to send a persist packet.
	 * tp->t_timer[TCPT_REXMT]
	 *	is set when we are retransmitting
	 * The output side is idle when both timers are zero.
	 *
	 * If send window is closed, there is data to transmit, and no
	 * retransmit or persist is pending, then go to persist state,
	 * arranging to force out a byte to get more current window information
	 * if nothing happens soon.
	 */
	if (tp->snd_wnd == 0 && sosndcc(tp) &&
	    tp->t_timer[TCPT_REXMT] == 0 && tp->t_timer[TCPT_PERSIST] == 0) {
		tp->t_rxtshift = 0;
		tcp_setpersist(tp);
	}

	/*
	 * No reason to send a segment, just return.
	 */
	return (0);

send:
	/*
	 * Grab a header block, attaching a copy of data to
	 * be transmitted, and initialize the header from
	 * the template for sends on this connection.
	 */
	bp = bp_get();
	if (bp == NULL)
		return (1);
	bp->next = 0;
	bp->wptr += sizeof (struct tcpiphdr);
	if (len) {
		bp->next = bp_copy(tp->so_wq->first, off, len);
		if (bp->next == 0)
			len = 0;
	}
	ti = (struct tcpiphdr *)bp->rptr;
	if (tp->t_template == 0)
		panic("tcp_output");
	bcopy((caddr_t)tp->t_template->rptr,(caddr_t)ti, sizeof (struct tcpiphdr));

	/*
	 * Fill in fields, remembering maximum advertised
	 * window for use in delaying messages about window sizes.
	 */
	ti->ti_seq = tp->snd_nxt;
	ti->ti_ack = tp->rcv_nxt;
	ti->ti_seq = htonl(ti->ti_seq);
	ti->ti_ack = htonl(ti->ti_ack);
	/*
	 * Before ESTABLISHED, force sending of initial options
	 * unless TCP set to not do any options.
	 */
	if (tp->t_state < TCPS_ESTABLISHED) {
		short maxseg;

		if (tp->t_flags&TF_NOOPT)
			goto noopt;
		if (in_netof(ti->ti_src) != in_netof(ti->ti_dst))
			goto noopt;
		opt = tcp_initopt;
		optlen = sizeof (tcp_initopt);
		maxseg = tcp_maxseg;
		*(u_short *)(opt + 2) = htons(maxseg);
	} else {
		if (tp->t_tcpopt == 0)
			goto noopt;
		opt = (u_char *)tp->t_tcpopt->rptr;
		optlen = BLEN(tp->t_tcpopt);
	}
	if (opt) {
		bp0 = bp->next;
		bp->next = bp_get();
		if (bp->next == 0) {
			freeb(bp);
			bp_free(bp0);
			return (1);
		}
		bp->next->next = bp0;
		bp0 = bp->next;
		bp0->wptr += optlen;
		bcopy((caddr_t)opt, (caddr_t)bp0->rptr, optlen);
		opt = (u_char *)((caddr_t)bp0->rptr + optlen);
		while (BLEN(bp0) & 0x3) {
			bp0->wptr++;
			*opt++ = TCPOPT_EOL;
		}
		optlen = BLEN(bp0);
		ti->ti_off = (sizeof (struct tcphdr) + optlen) >> 2;
	}
noopt:
	ti->ti_flags = flags;
	win = sbrcvspace(tp);
	if (win < sorcvhiwat(tp) / 4) {
		/*
		 *  avoid wastefully small packets caused by small windows
		 */
		win = 0;
	}
	if ((tp->so_state & SS_OPEN)==0 && tp->t_state > TCPS_CLOSE_WAIT) {
		/*
		 *  if the local process has disappeared, then open up
		 *  the window to get the remote process to send something
		 *  rather than hang waiting to send
		 */
		win = 1;
	}
	if (win > 0)
		ti->ti_win = htons((u_short)win);
	if (SEQ_GT(tp->snd_up, tp->snd_nxt)) {
		ti->ti_urp = tp->snd_up - tp->snd_nxt;
		ti->ti_urp = htons(ti->ti_urp);
		ti->ti_flags |= TH_URG;
	} else
		/*
		 * If no urgent pointer to send, then we pull
		 * the urgent pointer to the left edge of the send window
		 * so that it doesn't drift into the send window on sequence
		 * number wraparound.
		 */
		tp->snd_up = tp->snd_una;		/* drag it along */
	/*
	 * If anything to send and we can send it all, set PUSH.
	 * (This will keep happy those implementations which only
	 * give data to the user when a buffer fills or a PUSH comes in.)
	 */
	if (len && off+len == sosndcc(tp))
		ti->ti_flags |= TH_PUSH;

	/*
	 * Put TCP length in extended header, and then
	 * checksum extended header and data.
	 */
	if (len + optlen) {
		ti->ti_len = sizeof (struct tcphdr) + optlen + len;
		ti->ti_len = htons((u_short)ti->ti_len);
	}
	ti->ti_src = htonl(ti->ti_src);
	ti->ti_dst = htonl(ti->ti_dst);
	ti->ti_sport = htons(ti->ti_sport);
	ti->ti_dport = htons(ti->ti_dport);
	ti->ti_sum = in_cksum(bp, sizeof (struct tcpiphdr) + (int)optlen + len);
	tcp_debug(ti, 1);
	ti->ti_src = ntohl(ti->ti_src);
	ti->ti_dst = ntohl(ti->ti_dst);

	/*
	 * In transmit state, time the transmission and arrange for
	 * the retransmit.  In persist state, reset persist time for
	 * next persist.
	 */
	if (tp->t_force == 0) {
		/*
		 * Advance snd_nxt over sequence space of this segment.
		 */
		if (flags & (TH_SYN|TH_FIN))
			tp->snd_nxt++;
		tp->snd_nxt += len;
		if (SEQ_GT(tp->snd_nxt, tp->snd_max))
			tp->snd_max = tp->snd_nxt;

		/*
		 * Time this transmission if not a retransmission and
		 * not currently timing anything.
		 */
		if (SEQ_GT(tp->snd_nxt, tp->snd_max) && tp->t_rtt == 0) {
			tp->t_rtt = 1;
			tp->t_rtseq = tp->snd_nxt - len;
		}

		/*
		 * Set retransmit timer if not currently set.
		 * Initial value for retransmit timer to tcp_beta*tp->t_srtt.
		 * Initialize shift counter which is used for exponential
		 * backoff of retransmit time.
		 */
		if (tp->t_timer[TCPT_REXMT] == 0 &&
		    tp->snd_nxt != tp->snd_una) {
			TCPT_RANGESET(tp->t_timer[TCPT_REXMT],
			    tcp_beta * tp->t_srtt, TCPTV_MIN, TCPTV_MAX);
			tp->t_rtt = 0;
			tp->t_rxtshift = 0;
		}
		tp->t_timer[TCPT_PERSIST] = 0;
	} else {
		if (SEQ_GT(tp->snd_una+1, tp->snd_max))
			tp->snd_max = tp->snd_una+1;
	}

	/*
	 * Fill in IP length and desired time to live and
	 * send to IP level.
	 */
	((struct ip *)ti)->ip_len = sizeof (struct tcpiphdr) + optlen + len;
	((struct ip *)ti)->ip_ttl = TCP_TTL;
	error = tcp_ldout(bp);
	if (error)
		return (error);


	/*
	 * Data sent (as far as we can tell).
	 * If this advertises a larger window than any other segment,
	 * then remember the size of the advertised window.
	 * Drop send for purpose of ACK requirements.
	 */
	if (win > 0 && SEQ_GT(tp->rcv_nxt+win, tp->rcv_adv))
		tp->rcv_adv = tp->rcv_nxt + win;
	tp->t_flags &= ~(TF_ACKNOW|TF_DELACK);
	if (sendalot && tp->t_force == 0)
		goto again;
	return (0);
}

tcp_setpersist(tp)
	register struct tcpcb *tp;
{

	if (tp->t_timer[TCPT_REXMT])
		panic("tcp_output REXMT");
	/*
	 * Start/restart persistance timer.
	 */
	TCPT_RANGESET(tp->t_timer[TCPT_PERSIST],
	    ((int)(tcp_beta * tp->t_srtt)) << tp->t_rxtshift,
	    TCPTV_PERSMIN, TCPTV_MAX);
	tp->t_rxtshift++;
	if (tp->t_rxtshift >= TCP_MAXRXTSHIFT)
		tp->t_rxtshift = 0;
}
