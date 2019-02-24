/*	tcp_input.c	6.1	83/07/29	*/

#include "sys/param.h"
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

int	tcpprintfs = 0;
int	itcpcksum = 0;
int	tcp_dropcode = 0;
extern	tcpnodelack;
extern int tcp_maxseg;

/*
 * TCP input routine, follows pages 65-76 of the
 * protocol specification dated September, 1981 very closely.
 */
tcp_input(tibp0)
	struct block *tibp0;
{
	register struct tcpiphdr *ti;
	register struct block *tibp;
	struct block *otibp = 0;
	int len, tlen, off;
	register struct tcpcb *tp = 0;
	register int tiflags;
	int todrop, acked;
	int droptcpcb=0;

	/*
	 * Get IP and TCP header together in first block.
	 * Note: IP leaves IP header in first block.
	 */
	tibp = tibp0;
	ti = (struct tcpiphdr *)tibp->rptr;
	if (((struct ip *)ti)->ip_hl > (sizeof (struct ip) >> 2))
		ip_stripoptions(tibp, (struct block *)0);
	if (BLEN(tibp) < sizeof (struct tcpiphdr)) {
		if ((tibp = bp_pullup(tibp, sizeof (struct tcpiphdr))) == 0) {
			tcpstat.tcps_hdrops++;
			return;
		}
		ti = (struct tcpiphdr *)tibp->rptr;
	}

	/*
	 * Checksum extended TCP header and data.
	 */
	tlen = ((struct ip *)ti)->ip_len;
	len = sizeof (struct ip) + tlen;
	ti->ti_src = htonl(ti->ti_src);
	ti->ti_dst = htonl(ti->ti_dst);
	ti->ti_len = (u_short)tlen;
	ti->ti_len = htons((u_short)ti->ti_len);
	ti->ti_next = 0;
	ti->ti_x1 = 0;
	ti->ti_bp = 0;
	tcp_debug(ti, 0);
	if (itcpcksum) {
		if (ti->ti_sum = in_cksum(tibp, len)) {
			if (tcpprintfs)
				printf("tcp sum: src %x, len %d\n", ti->ti_src, len);
			tcpstat.tcps_badsum++;
			goto drop;
		}
	}
	ti->ti_bp = tibp;

	/*
	 * Check that TCP offset makes sense,
	 * pull out TCP options and adjust length.
	 */
	off = ti->ti_off << 2;
	if (off < sizeof (struct tcphdr) || off > tlen) {
		if (tcpprintfs)
			printf("tcp off: src %x off %d\n", ti->ti_src, off);
		tcpstat.tcps_badoff++;
		goto drop;
	}
	tlen -= off;
	ti->ti_len = tlen;
	if (off > sizeof (struct tcphdr)) {
		if ((tibp = bp_pullup(tibp, sizeof (struct ip) + off)) == 0) {
			tcpstat.tcps_hdrops++;
			return;
		}
		ti = (struct tcpiphdr *)tibp->rptr;
		ti->ti_bp = tibp;
		otibp = bp_get();
		if (otibp == 0)
			goto drop;
		otibp->next = 0;
		otibp->wptr += off - sizeof (struct tcphdr);
		{ caddr_t op = (caddr_t)tibp->rptr + sizeof (struct tcpiphdr);
		  bcopy(op, (caddr_t)otibp->rptr, (unsigned)BLEN(otibp));
		  tibp->wptr -= BLEN(otibp);
		  bcopy(op+BLEN(otibp), op,
		   (unsigned)(BLEN(tibp)-sizeof (struct tcpiphdr)));
		}
	}
	tiflags = ti->ti_flags;

	/*
	 * Drop TCP and IP headers.
	 */
	tibp->rptr += sizeof(struct tcpiphdr);

	/*
	 * Convert TCP protocol specific fields to host format.
	 */
	ti->ti_seq = ntohl(ti->ti_seq);
	ti->ti_ack = ntohl(ti->ti_ack);
	ti->ti_win = ntohs(ti->ti_win);
	ti->ti_urp = ntohs(ti->ti_urp);
	ti->ti_src = ntohl(ti->ti_src);
	ti->ti_dst = ntohl(ti->ti_dst);
	ti->ti_sport = ntohs(ti->ti_sport);
	ti->ti_dport = ntohs(ti->ti_dport);

	/*
	 * Locate pcb for segment.
	 * If the state is CLOSED (i.e., TCB does not exist) then
	 * all data in the incoming segment is discarded.
	 */
	tp = tcpcb_lookup(ti->ti_src, ti->ti_sport, ti->ti_dst, ti->ti_dport);
	if(tp == 0) {
		tcp_dropcode = 2;
		goto dropwithreset;
	}

	if (tp->so_options & SO_ACCEPTCONN) {
		tp = tcp_newconn(tp);
		if (tp == 0){
			if (tcpprintfs) printf("tcp_newconn returns 0\n");
			goto drop;
		}
		droptcpcb = 1;
		if (tcpprintfs) printf("tcpcb[%d]\n", tp->so_dev);
		tp->so_laddr = ti->ti_dst;
		tp->so_lport = ti->ti_dport;
		tp->so_faddr = ti->ti_src;
		tp->so_fport = ti->ti_sport;
		tcp_template(tp);
		if (tp->t_template == 0){
			if (tcpprintfs) printf("tp->t_template == 0\n");
			goto drop;
		}
		tp->so_options &= ~SO_ACCEPTCONN;
		tp->so_state = SS_PLEASEOPEN;
		/* simulate PRU_ATTACH */
		tp->t_state = TCPS_LISTEN;
	}

	/*
	 * If the RST bit is set examine the state:
	 *    SYN_RECEIVED STATE:
	 *	If passive open, return to LISTEN state.
	 *	If active open, inform user that connection was refused.
	 *    ESTABLISHED, FIN_WAIT_1, FIN_WAIT2, CLOSE_WAIT STATES:
	 *	Inform user that connection was reset, and close tcb.
	 *    CLOSING, LAST_ACK, TIME_WAIT STATES
	 *	Close the tcb.
	 */
	if (tiflags&TH_RST) switch (tp->t_state) {
	case TCPS_LISTEN:
		goto drop;

	case TCPS_SYN_RECEIVED:
		tp = tcp_drop(tp);
		goto drop;

	case TCPS_ESTABLISHED:
	case TCPS_FIN_WAIT_1:
	case TCPS_FIN_WAIT_2:
	case TCPS_CLOSE_WAIT:
		tp = tcp_drop(tp);
		goto drop;

	case TCPS_CLOSING:
	case TCPS_LAST_ACK:
	case TCPS_TIME_WAIT:
		tp = tcp_close(tp);
		goto drop;
	}

	/*
	 * Segment received on connection.
	 * Reset idle time and keep-alive timer.
	 */
	tp->t_idle = 0;
	tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;

	/*
	 * Process options.
	 */
	if (otibp) {
		tcp_dooptions(tp, otibp);
		otibp = 0;
	}

	/*
	 * Calculate amount of space in receive window,
	 * and then do TCP input processing.
	 */
	tp->rcv_wnd = sbrcvspace(tp);
	if (tp->rcv_wnd < 0)
		tp->rcv_wnd = 0;

	switch (tp->t_state) {

	/*
	 * If the state is LISTEN then ignore segment if it contains an RST.
	 * If the segment contains an ACK then it is bad and send a RST.
	 * If it does not contain a SYN then it is not interesting; drop it.
	 * Otherwise initialize tp->rcv_nxt, and tp->irs, select an initial
	 * tp->iss, and send a segment:
	 *     <SEQ=ISS><ACK=RCV_NXT><CTL=SYN,ACK>
	 * Also initialize tp->snd_nxt to tp->iss+1 and tp->snd_una to tp->iss.
	 * Fill in remote peer address fields if not previously specified.
	 * Enter SYN_RECEIVED state, and process any other fields of this
	 * segment in this state.
	 */
	case TCPS_LISTEN:
		if (tiflags & TH_ACK) {
			tcp_dropcode = 3;
			goto dropwithreset;
		}
		if ((tiflags & TH_SYN) == 0)
			goto drop;

		/* this should be removed */
		if (tp->t_template == (struct block *)0) {
			printf("tcpinput: uh oh, no template\n");
			goto drop;
		}
		tp->iss = tcp_iss; tcp_iss += TCP_ISSINCR/2;
		tp->irs = ti->ti_seq;
		tcp_sendseqinit(tp);
		tcp_rcvseqinit(tp);
		tp->t_state = TCPS_SYN_RECEIVED;
		tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;
		droptcpcb = 0;		/* committed to control block */
		goto trimthenstep6;

	/*
	 * If the state is SYN_SENT:
	 *	if seg contains an ACK, but not for our SYN, drop the input.
	 *	if seg contains a RST, then drop the connection.
	 *	if seg does not contain SYN, then drop it.
	 * Otherwise this is an acceptable SYN segment
	 *	initialize tp->rcv_nxt and tp->irs
	 *	if seg contains ack then advance tp->snd_una
	 *	if SYN has been acked change to ESTABLISHED else SYN_RCVD state
	 *	arrange for segment to be acked (eventually)
	 *	continue processing rest of data/controls, beginning with URG
	 */
	case TCPS_SYN_SENT:
		if ((tiflags & TH_ACK) &&
		    (SEQ_LT(ti->ti_ack, tp->iss) ||
		     SEQ_GT(ti->ti_ack, tp->snd_max))) {
			tcp_dropcode = 4;
			goto dropwithreset;
		}
		if (tiflags & TH_RST) {
			if (tiflags & TH_ACK)
				tp = tcp_drop(tp);
			goto drop;
		}
		if ((tiflags & TH_SYN) == 0)
			goto drop;
		tp->snd_una = ti->ti_ack;
		if (SEQ_LT(tp->snd_nxt, tp->snd_una))
			tp->snd_nxt = tp->snd_una;
		tp->t_timer[TCPT_REXMT] = 0;
		tp->irs = ti->ti_seq;
		tcp_rcvseqinit(tp);
		tp->t_flags |= TF_ACKNOW;
		if (SEQ_GT(tp->snd_una, tp->iss)) {
			if (tcp_isconnected(tp)<0)
				goto dropwithreset;
			tp->t_state = TCPS_ESTABLISHED;
			(void) tcp_reass(tp, (struct tcpiphdr *)0);
		} else
			tp->t_state = TCPS_SYN_RECEIVED;
		goto trimthenstep6;

trimthenstep6:
		/*
		 * Advance ti->ti_seq to correspond to first data byte.
		 * If data, trim to stay within window,
		 * dropping FIN if necessary.
		 */
		ti->ti_seq++;
		if (ti->ti_len > tp->rcv_wnd) {
			todrop = ti->ti_len - tp->rcv_wnd;
			bp_adj(tibp, -todrop);
			ti->ti_len = tp->rcv_wnd;
			ti->ti_flags &= ~TH_FIN;
		}
		tp->snd_wl1 = ti->ti_seq - 1;
		goto step6;
	}

	/*
	 * States other than LISTEN or SYN_SENT.
	 * First check that at least some bytes of segment are within 
	 * receive window.
	 */
	if (tp->rcv_wnd == 0) {
		/*
		 * If window is closed can only take segments at
		 * window edge, and have to drop data and PUSH from
		 * incoming segments.
		 */
		if (tp->rcv_nxt != ti->ti_seq)
			goto dropafterack;
		if (ti->ti_len > 0) {
			bp_adj(tibp, ti->ti_len);
			ti->ti_len = 0;
			ti->ti_flags &= ~(TH_PUSH|TH_FIN);
		}
	} else {
		/*
		 * If segment begins before rcv_nxt, drop leading
		 * data (and SYN); if nothing left, just ack.
		 */
		todrop = tp->rcv_nxt - ti->ti_seq;
		if (todrop > 0) {
			if (tiflags & TH_SYN) {
				tiflags &= ~TH_SYN;
				ti->ti_flags &= ~TH_SYN;
				ti->ti_seq++;
				if (ti->ti_urp > 1) 
					ti->ti_urp--;
				else
					tiflags &= ~TH_URG;
				todrop--;
			}
			if (todrop > ti->ti_len ||
			    (todrop == ti->ti_len && (tiflags&TH_FIN) == 0))
				goto dropafterack2;
			bp_adj(tibp, todrop);
			ti->ti_seq += todrop;
			ti->ti_len -= todrop;
			if (ti->ti_urp > todrop)
				ti->ti_urp -= todrop;
			else {
				tiflags &= ~TH_URG;
				ti->ti_flags &= ~TH_URG;
				ti->ti_urp = 0;
			}
		}
		/*
		 * If segment ends after window, drop trailing data
		 * (and PUSH and FIN); if nothing left, just ACK.
		 */
		todrop = (ti->ti_seq+ti->ti_len) - (tp->rcv_nxt+tp->rcv_wnd);
		if (todrop > 0) {
			if (todrop > ti->ti_len)
				todrop = ti->ti_len;
			bp_adj(tibp, -todrop);
			ti->ti_len -= todrop;
			ti->ti_flags &= ~(TH_PUSH|TH_FIN);
		}
	}

	/*
	 * If data is received on a connection after the
	 * user processes are gone, then RST the other end.
	 */
	if ((tp->so_state & SS_OPEN)==0 && tp->t_state > TCPS_CLOSE_WAIT && tlen) {
		tp = tcp_close(tp);
		tcp_dropcode = 5;
		goto dropwithreset;
	}

	/*
	 * If a SYN is in the window, then this is an
	 * error and we send an RST and drop the connection.
	 */
	if (tiflags & TH_SYN) {
		tp = tcp_drop(tp);
		tcp_dropcode = 6;
		goto dropwithreset;
	}

	/*
	 * If the ACK bit is off we drop the segment and return.
	 */
	if ((tiflags & TH_ACK) == 0)
		goto drop;
	
	/*
	 * Ack processing.
	 */
	switch (tp->t_state) {

	/*
	 * In SYN_RECEIVED state if the ack ACKs our SYN then enter
	 * ESTABLISHED state and continue processing, othewise
	 * send an RST.
	 */
	case TCPS_SYN_RECEIVED:
		if (SEQ_GT(tp->snd_una, ti->ti_ack) ||
		    SEQ_GT(ti->ti_ack, tp->snd_max)) {
			tcp_dropcode = 7;
			goto dropwithreset;
		}
		tp->snd_una++;			/* SYN acked */
		if (SEQ_LT(tp->snd_nxt, tp->snd_una))
			tp->snd_nxt = tp->snd_una;
		tp->t_timer[TCPT_REXMT] = 0;
		if (tcp_isconnected(tp)<0) {
			tcp_dropcode = 8;
			goto dropwithreset;
		}
		tp->t_state = TCPS_ESTABLISHED;
		(void) tcp_reass(tp, (struct tcpiphdr *)0);
		tp->snd_wl1 = ti->ti_seq - 1;
		/* fall into ... */

	/*
	 * In ESTABLISHED state: drop duplicate ACKs; ACK out of range
	 * ACKs.  If the ack is in the range
	 *	tp->snd_una < ti->ti_ack <= tp->snd_max
	 * then advance tp->snd_una to ti->ti_ack and drop
	 * data from the retransmission queue.  If this ACK reflects
	 * more up to date window information we update our window information.
	 */
	case TCPS_ESTABLISHED:
	case TCPS_FIN_WAIT_1:
	case TCPS_FIN_WAIT_2:
	case TCPS_CLOSE_WAIT:
	case TCPS_CLOSING:
	case TCPS_LAST_ACK:
	case TCPS_TIME_WAIT:
#define	ourfinisacked	(acked > 0)

		if (SEQ_LEQ(ti->ti_ack, tp->snd_una))
			break;
		if (SEQ_GT(ti->ti_ack, tp->snd_max))
			goto dropafterack;
		acked = ti->ti_ack - tp->snd_una;

		/*
		 * If transmit timer is running and timed sequence
		 * number was acked, update smoothed round trip time.
		 */
		if (tp->t_rtt && SEQ_GT(ti->ti_ack, tp->t_rtseq)) {
			if (tp->t_srtt == 0)
				tp->t_srtt = tp->t_rtt;
			else
				tp->t_srtt =
				    tcp_alpha * tp->t_srtt +
				    (1 - tcp_alpha) * tp->t_rtt;
			tp->t_rtt = 0;
		}

		if (ti->ti_ack == tp->snd_max)
			tp->t_timer[TCPT_REXMT] = 0;
		else {
			TCPT_RANGESET(tp->t_timer[TCPT_REXMT],
			    tcp_beta * tp->t_srtt, TCPTV_MIN, TCPTV_MAX);
			tp->t_rtt = 1;
			tp->t_rxtshift = 0;
		}
		if (acked > sosndcc(tp)) {
			tp->snd_wnd -= sosndcc(tp);
			sbsnddrop(tp, sosndcc(tp));
		} else {
			sbsnddrop(tp, acked);
			tp->snd_wnd -= acked;
			acked = 0;
		}
		tp->snd_una = ti->ti_ack;
		if (SEQ_LT(tp->snd_nxt, tp->snd_una))
			tp->snd_nxt = tp->snd_una;

		switch (tp->t_state) {

		/*
		 * In FIN_WAIT_1 STATE in addition to the processing
		 * for the ESTABLISHED state if our FIN is now acknowledged
		 * then enter FIN_WAIT_2.
		 */
		case TCPS_FIN_WAIT_1:
			if (ourfinisacked) {
				/*
				 * If we can't receive any more
				 * data, then closing user can proceed,
				 * but don't wait forever.
				 */
				tp->t_state = TCPS_FIN_WAIT_2;
				tcp_canceltimers(tp);
				tp->so_options |= SO_KEEPALIVE;
				tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;
			}
			break;

	 	/*
		 * In CLOSING STATE in addition to the processing for
		 * the ESTABLISHED state if the ACK acknowledges our FIN
		 * then enter the TIME-WAIT state, otherwise ignore
		 * the segment.
		 */
		case TCPS_CLOSING:
			if (ourfinisacked) {
				tp->t_state = TCPS_TIME_WAIT;
				tcp_canceltimers(tp);
				tp->t_timer[TCPT_2MSL] = 2 * TCPTV_MSL;
				tp->so_state &= ~(SS_PLEASEOPEN|SS_RCVATMARK);
				if(tp->so_state&SS_OPEN && !(tp->so_state&SS_HUNGUP)) {
					tp->so_state |= SS_HUNGUP;
					tcp_hungup(tp);
				}
			}
			break;

		/*
		 * The only thing that can arrive in  LAST_ACK state
		 * is an acknowledgment of our FIN.  If our FIN is now
		 * acknowledged, delete the TCB, enter the closed state
		 * and return.
		 */
		case TCPS_LAST_ACK:
			if (ourfinisacked)
				tp = tcp_close(tp);
			goto drop;

		/*
		 * In TIME_WAIT state the only thing that should arrive
		 * is a retransmission of the remote FIN.  Acknowledge
		 * it and restart the finack timer.
		 */
		case TCPS_TIME_WAIT:
			tp->t_timer[TCPT_2MSL] = 2 * TCPTV_MSL;
			goto dropafterack;
		}
#undef ourfinisacked
	}

step6:
	/*
	 * Update window information.
	 */
	if (SEQ_LT(tp->snd_wl1, ti->ti_seq) || tp->snd_wl1 == ti->ti_seq &&
	    (SEQ_LT(tp->snd_wl2, ti->ti_ack) ||
	     tp->snd_wl2 == ti->ti_ack && ti->ti_win > tp->snd_wnd)) {
		tp->snd_wnd = ti->ti_win;
		tp->snd_wl1 = ti->ti_seq;
		tp->snd_wl2 = ti->ti_ack;
		if (tp->snd_wnd != 0)
			tp->t_timer[TCPT_PERSIST] = 0;
	}

	/*
	 * Process segments with URG.
	 */
	if ((tiflags & TH_URG) && ti->ti_urp &&
	    TCPS_HAVERCVDFIN(tp->t_state) == 0) {
		/*
		 * This is a kludge, but if we receive accept
		 * random urgent pointers, we'll crash in
		 * soreceive.  It's hard to imagine someone
		 * actually wanting to send this much urgent data.
		 */
		if (ti->ti_urp > tp->t_maxseg) {	/* XXX */
			ti->ti_urp = 0;			/* XXX */
			tiflags &= ~TH_URG;		/* XXX */
			ti->ti_flags &= ~TH_URG;	/* XXX */
			goto badurp;			/* XXX */
		}
		/*
		 * If this segment advances the known urgent pointer,
		 * then mark the data stream.  This should not happen
		 * in CLOSE_WAIT, CLOSING, LAST_ACK or TIME_WAIT STATES since
		 * a FIN has been received from the remote side. 
		 * In these states we ignore the URG.
		 */
		if (SEQ_GT(ti->ti_seq+ti->ti_urp, tp->rcv_up)) {
			/* M_CTL, maybe? looks like it's put in the data stream */
			tp->rcv_up = ti->ti_seq + ti->ti_urp;
			tp->so_oobmark = 0 +
			    (tp->rcv_up - tp->rcv_nxt) - 1;
			if (tp->so_oobmark == 0)
				tp->so_state |= SS_RCVATMARK;
			sohasoutofband(tp);
			tp->t_oobflags &= ~TCPOOB_HAVEDATA;
		}
		/*
		 * Remove out of band data so doesn't get presented to user.
		 * This can happen independent of advancing the URG pointer,
		 * but if two URG's are pending at once, some out-of-band
		 * data may creep in... ick.
		 */
		if (ti->ti_urp <= ti->ti_len)
			tcp_pulloutofband(tp, ti);
	}
badurp:							/* XXX */

	/*
	 * Process the segment text, merging it into the TCP sequencing queue,
	 * and arranging for acknowledgment of receipt if necessary.
	 * This process logically involves adjusting tp->rcv_wnd as data
	 * is presented to the user (this happens in tcp_usrreq.c,
	 * case PRU_RCVD).  If a FIN has already been received on this
	 * connection then we just ignore the text.
	 */
	if ((ti->ti_len || (tiflags&TH_FIN)) &&
	    TCPS_HAVERCVDFIN(tp->t_state) == 0) {
		tiflags = tcp_reass(tp, ti);
		if (tcpnodelack == 0)
			tp->t_flags |= TF_DELACK;
		else
			tp->t_flags |= TF_ACKNOW;
	} else {
		bp_free(tibp);
		tiflags &= ~TH_FIN;
	}

	/*
	 * If FIN is received ACK the FIN and let the user know
	 * that the connection is closing.
	 */
	if (tiflags & TH_FIN) {
		if (TCPS_HAVERCVDFIN(tp->t_state) == 0) {
			tcp_cantrcvmore(tp);
			tp->t_flags |= TF_ACKNOW;
			tp->rcv_nxt++;
		}
		switch (tp->t_state) {

	 	/*
		 * In SYN_RECEIVED and ESTABLISHED STATES
		 * enter the CLOSE_WAIT state.
		 */
		case TCPS_SYN_RECEIVED:
			/* if caller closes before listener opens tcp device,
			 * act as if listener had opened and closed device.
			 */
			if (tp->so_state & SS_OPEN) {
				tp->t_state = TCPS_CLOSE_WAIT;
				tp->so_state &= ~(SS_PLEASEOPEN|SS_RCVATMARK);
				if(!(tp->so_state&SS_HUNGUP)) {
					tp->so_state |= SS_HUNGUP;
					tcp_hungup(tp);
				}
			} else {
				tp->t_state = TCPS_LAST_ACK;
				tp->so_options |= SO_KEEPALIVE;
				tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;
			}
			break;
		case TCPS_ESTABLISHED:
			/* if caller closes before listener opens tcp device,
			 * act as if listener had opened and closed device.
			 */
			if (tp->so_state & SS_OPEN)
				tp->t_state = TCPS_CLOSE_WAIT;
			else {
				tp->t_state = TCPS_LAST_ACK;
				tp->so_options |= SO_KEEPALIVE;
				tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;
			}
			break;

	 	/*
		 * If still in FIN_WAIT_1 STATE FIN has not been acked so
		 * enter the CLOSING state.
		 */
		case TCPS_FIN_WAIT_1:
			tp->t_state = TCPS_CLOSING;
			break;

	 	/*
		 * In FIN_WAIT_2 state enter the TIME_WAIT state,
		 * starting the time-wait timer, turning off the other 
		 * standard timers.
		 */
		case TCPS_FIN_WAIT_2:
			tp->t_state = TCPS_TIME_WAIT;
			tcp_canceltimers(tp);
			tp->t_timer[TCPT_2MSL] = 2 * TCPTV_MSL;
			tp->so_state &= ~(SS_PLEASEOPEN|SS_RCVATMARK);
			if(tp->so_state&SS_OPEN && !(tp->so_state&SS_HUNGUP)) {
				tp->so_state |= SS_HUNGUP;
				tcp_hungup(tp);
			}
			break;

		/*
		 * In TIME_WAIT state restart the 2 MSL time_wait timer.
		 */
		case TCPS_TIME_WAIT:
			tp->t_timer[TCPT_2MSL] = 2 * TCPTV_MSL;
			break;
		}
	}

	/*
	 * Return any desired output.
	 */
	(void) tcp_output(tp);
	return;

dropafterack:
	/*
	 * Generate an ACK dropping incoming segment if it occupies
	 * sequence space, where the ACK reflects our state.
	 */
	if ((tiflags&TH_RST) ||
	    tlen == 0 && (tiflags&(TH_SYN|TH_FIN)) == 0)
		goto drop;
dropafterack2:
	tcp_respond(tp, ti, tp->rcv_nxt, tp->snd_nxt, TH_ACK);
	return;

dropwithreset:
	if (otibp) {
		freeb(otibp);
		otibp = 0;
	}
	/*
	 * Generate a RST, dropping incoming segment.
	 * Make ACK acceptable to originator of segment.
	 */
	if (tiflags & TH_RST)
		goto drop;
	if (tiflags & TH_ACK)
		tcp_respond(tp, ti, (tcp_seq)0, ti->ti_ack, TH_RST);
	else {
		if (tiflags & TH_SYN)
			ti->ti_len++;
		tcp_respond(tp, ti, ti->ti_seq+ti->ti_len, (tcp_seq)0,
		    TH_RST|TH_ACK);
	}

	/* destroy temporarily created control block */
	if (droptcpcb)
		(void) tcp_close(tp);
	return;

drop:
	/* destroy temporarily created control block */
	if (droptcpcb)
		(void) tcp_close(tp);
	if (otibp)
		freeb(otibp);
	/*
	 * Drop space held by incoming segment and return.
	 */
	bp_free(tibp);
	return;
}

tcp_dooptions(tp, om)
	struct tcpcb *tp;
	struct block *om;
{
	register u_char *cp;
	int opt, optlen, cnt;

	cp = (u_char *)om->rptr;
	cnt = BLEN(om);
	for (; cnt > 0; cnt -= optlen, cp += optlen) {
		opt = cp[0];
		if (opt == TCPOPT_EOL)
			break;
		if (opt == TCPOPT_NOP)
			optlen = 1;
		else {
			optlen = cp[1];
			if (optlen <= 0)
				break;
		}
		switch (opt) {

		default:
			break;

		case TCPOPT_MAXSEG:
			if (optlen != 4)
				continue;
			tp->t_maxseg = *(u_short *)(cp + 2);
			tp->t_maxseg = ntohs((u_short)tp->t_maxseg);
			if (tp->t_maxseg > tcp_maxseg)
				tp->t_maxseg = tcp_maxseg;
			break;
		}
	}
	freeb(om);
}

/*
 * Pull out of band byte out of a segment so
 * it doesn't appear in the user's data queue.
 * It is still reflected in the segment length for
 * sequencing purposes.
 */
tcp_pulloutofband(tp, ti)
	struct tcpcb *tp;
	struct tcpiphdr *ti;
{
	register struct block *bp;
	int cnt = ti->ti_urp - 1;

	bp = ti->ti_bp;

	while (cnt >= 0) {
		if (BLEN(bp) > cnt) {
			char *cp = (char *)(bp->rptr) + cnt;

			tp->t_iobc = *cp;
			tp->t_oobflags |= TCPOOB_HAVEDATA;
			bcopy(cp+1, cp, (unsigned)(BLEN(bp) - cnt - 1));
			bp->wptr--;
			return;
		}
		cnt -= BLEN(bp);
		bp = bp->next;
		if (bp == 0)
			break;
	}
	panic("tcp_pulloutofband");
}

/*
 * Insert segment ti into reassembly queue of tcp with
 * control block tp.  Return TH_FIN if reassembly now includes
 * a segment with FIN.
 */
tcp_reass(tp, ti)
	register struct tcpcb *tp;
	register struct tcpiphdr *ti;
{
	register struct block *bp;		/* for tcpiphdr */
	register struct tcpiphdr *prevq, *q;
	int flags;

	/*
	 * Call with ti==0 after become established to
	 * force pre-ESTABLISHED data up to user socket.
	 */
	if (ti == 0)
		goto present;

	/*
	 * Find a segment which begins after this one does.
	 */
	for (prevq = (struct tcpiphdr *)0, q = tp->seg_next;
	     q != (struct tcpiphdr *)0; prevq = q, q = q->ti_next) {
		if (SEQ_GT(q->ti_seq, ti->ti_seq))
			break;
	}

	/*
	 * If there is a preceding segment, it may provide some of
	 * our data already.  If so, drop the data from the incoming
	 * segment.  If it provides all of our data, drop us.
	 */
	if (prevq != (struct tcpiphdr *)0) {
		register int i;

		q = prevq;
		/* conversion to int (in i) handles seq wraparound */
		i = q->ti_seq + q->ti_len - ti->ti_seq;
		if (i > 0) {
			tcpstat.tcps_duplicates++;
			if (i >= ti->ti_len)
				goto drop;
			bp_adj(ti->ti_bp, i);
			ti->ti_len -= i;
			ti->ti_seq += i;
		}
		q = (struct tcpiphdr *) (q->ti_next);
	}

	/*
	 * While we overlap succeeding segments trim them or,
	 * if they are completely covered, dequeue them.
	 */
	while (q != (struct tcpiphdr *)0) {
		register int i = (ti->ti_seq + ti->ti_len) - q->ti_seq;
		if (i <= 0)
			break;
		tcpstat.tcps_delayed++;
		if (i < q->ti_len) {
			q->ti_seq += i;
			q->ti_len -= i;
			bp_adj(q->ti_bp, i);
			break;
		}
		/* dequeue overlapped segment */
		if (prevq == (struct tcpiphdr *)0)
			tp->seg_next = q->ti_next;
		else
			prevq->ti_next = q->ti_next;
		bp_free(q->ti_bp);
		if (prevq == (struct tcpiphdr *)0)
			q = tp->seg_next;
		else
			q = prevq->ti_next;
	}

	/*
	 * Stick new segment in its place.
	 */
/*	insque(ti, prevq);	*/
	if (prevq == (struct tcpiphdr *)0)
		tp->seg_next = ti;
	else
		prevq->ti_next = ti;
	ti->ti_next = q;


present:
	/*
	 * Present data to user, advancing rcv_nxt through
	 * completed sequence space.
	 */
	if (TCPS_HAVERCVDSYN(tp->t_state) == 0)
		return (0);
	if ((ti = tp->seg_next) == (struct tcpiphdr *)0)
		return (0);
	if (ti->ti_seq != tp->rcv_nxt)
		return (0);
	if (tp->t_state == TCPS_SYN_RECEIVED && ti->ti_len)
		return (0);
	do {
		tp->rcv_nxt += ti->ti_len;

		flags = ti->ti_flags & TH_FIN;

/*		remque(ti);	*/
		tp->seg_next = ti->ti_next;
		bp = ti->ti_bp;
		ti = ti->ti_next;
		if (tp->so_state & SS_OPEN)
			tcpdrint(bp, tp);
		else
			bp_free(bp);

	} while (ti != (struct tcpiphdr *)0 && ti->ti_seq == tp->rcv_nxt);
	return (flags);
drop:
	bp_free(ti->ti_bp);
	return (0);
}
