/*	tcp_timer.c	6.1	83/07/29	*/

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

extern struct tcpcb tcpcb[];
extern int tcpcnt;
int	tcpnodelack = 0;
/*
 * Fast timeout routine for processing delayed acks
 */
tcp_fasttimo()
{
	register struct tcpcb *tp;
	register struct tcpcb *tend;
	int s = spl6();
	extern tcp_busy;

	if(tcp_busy)
		goto busy;
	tcp_busy++;
	tend = &tcpcb[tcpcnt];
	for(tp = tcpcb; tp < tend; tp++)
		if ((tp->t_flags & TF_DELACK)) {
			tp->t_flags &= ~TF_DELACK;
			tp->t_flags |= TF_ACKNOW;
			(void) tcp_output(tp);
		}
	--tcp_busy;
busy:
	timeout(tcp_fasttimo, (caddr_t) 0, HZ / 5);
	splx(s);
}

/*
 * Tcp protocol timeout routine called every 500 ms.
 * Updates the timers in all active tcb's and
 * causes finite state machine actions if timers expire.
 */
tcp_slowtimo()
{
	extern int tcp_busy;
	register struct tcpcb *tp;
	register struct tcpcb *tend;
	int s = spl6();
	register int i;

	if(tcp_busy)
		goto busy;
	/*
	 * Search through tcb's and update active timers.
	 */
	tcp_busy++;
	tend = &tcpcb[tcpcnt];
	for(tp = tcpcb; tp < tend; tp++){
		if(tp->t_state == TCPS_CLOSED)
			continue;
		for (i = 0; i < TCPT_NTIMERS; i++) {
			if (tp->t_timer[i] && --tp->t_timer[i] == 0) {
				tcp_timers(tp, i);
			}
		}
		tp->t_idle++;
		if (tp->t_rtt)
			tp->t_rtt++;
	}
	--tcp_busy;
busy:
	tcp_iss += TCP_ISSINCR/PR_SLOWHZ;	
	splx(s);
	timeout(tcp_slowtimo, (caddr_t)0, HZ/PR_SLOWHZ);
}

/*
 * Cancel all timers for TCP tp.
 */
tcp_canceltimers(tp)
	struct tcpcb *tp;
{
	register int i;

	for (i = 0; i < TCPT_NTIMERS; i++)
		tp->t_timer[i] = 0;
}

float	tcp_backoff[TCP_MAXRXTSHIFT] =
    { 1.0, 1.2, 1.4, 1.7, 2.0, 3.0, 5.0, 8.0, 16.0, 32.0, 64.0, 128.0 };
int	tcpexprexmtbackoff = 0;
/*
 * TCP timer processing.
 */
struct tcpcb *
tcp_timers(tp, timer)
	register struct tcpcb *tp;
	int timer;
{

	if(tp->t_state == TCPS_ESTABLISHED)
		tcpstat.tcps_timeouts[timer]++;
	switch (timer) {

	/*
	 * 2 MSL timeout in shutdown went off.  Delete connection
	 * control block.
	 */
	case TCPT_2MSL:
		tp = tcp_close(tp);
		break;

	/*
	 * Retransmission timer went off.  Message has not
	 * been acked within retransmit interval.  Back off
	 * to a longer retransmit interval and retransmit all
	 * unacknowledged messages in the window.
	 */
	case TCPT_REXMT:
		tp->t_rxtshift++;
		if (tp->t_rxtshift > TCP_MAXRXTSHIFT) {
			tp = tcp_drop(tp);
			break;
		}
		TCPT_RANGESET(tp->t_timer[TCPT_REXMT],
		    (int)tp->t_srtt, TCPTV_MIN, TCPTV_MAX);
		if (tcpexprexmtbackoff) {
			TCPT_RANGESET(tp->t_timer[TCPT_REXMT],
			    tp->t_timer[TCPT_REXMT] << tp->t_rxtshift,
			    TCPTV_MIN, TCPTV_MAX);
		} else {
			TCPT_RANGESET(tp->t_timer[TCPT_REXMT],
			    tp->t_timer[TCPT_REXMT] *
			        tcp_backoff[tp->t_rxtshift - 1],
			    TCPTV_MIN, TCPTV_MAX);
		}
		tp->snd_nxt = tp->snd_una;
		/* this only transmits one segment! */
		(void) tcp_output(tp);
		break;

	/*
	 * Persistance timer into zero window.
	 * Force a byte to be output, if possible.
	 */
	case TCPT_PERSIST:
		tcp_setpersist(tp);
		tp->t_force = 1;
		(void) tcp_output(tp);
		tp->t_force = 0;
		break;

	/*
	 * Keep-alive timer went off; send something
	 * or drop connection if idle for too long.
	 */
	case TCPT_KEEP:
		if (tp->t_state < TCPS_ESTABLISHED)
			goto dropit;
		if (tp->so_options & SO_KEEPALIVE) {
		    	if (tp->t_idle >= TCPTV_MAXIDLE)
				goto dropit;
			/*
			 * Saying tp->rcv_nxt-1 lies about what
			 * we have received, and by the protocol spec
			 * requires the correspondent TCP to respond.
			 * Saying tp->snd_una-1 causes the transmitted
			 * byte to lie outside the receive window; this
			 * is important because we don't necessarily
			 * have a byte in the window to send (consider
			 * a one-way stream!)
			 */
			tcp_respond(tp, (struct tcpiphdr *)tp->t_template->rptr,
					tp->rcv_nxt-1, tp->snd_una-1, 0);
		} else
			tp->t_idle = 0;
		tp->t_timer[TCPT_KEEP] = TCPTV_KEEP;
		break;
	dropit:
		tp = tcp_drop(tp);
		break;
	}
	return (tp);
}
