/*
 * tcp_device.c
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/inio.h"
#include "sys/ttyio.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/inet/in.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/tcp.h"
#include "sys/inet/tcp_timer.h"
#include "sys/inet/tcp_seq.h"
#include "sys/inet/tcp_var.h"
#include "sys/inet/tcpip.h"
#include "sys/inet/tcp_user.h"
#include "sys/inet/tcp_fsm.h"

extern int tcp_busy;	/* set to discourage timers & ensuing panics */
extern int tcp_maxseg;

long	tcpdopen();
int	tcpdclose(), tcpdput();
int	tcpdosrv(), tcpdisrv();
static	struct qinit tcpdrinit = { noput, tcpdisrv, tcpdopen, tcpdclose, 2048, 64 };
	struct qinit tcpdwinit = { tcpdput, tcpdosrv, tcpdopen, tcpdclose, 2048, 64};
struct	streamtab tcpdinfo = { &tcpdrinit, &tcpdwinit };

struct cdevsw tcpcdev = cstrinit(&tcpdinfo);

extern int tcpcnt;
extern struct tcpcb tcpcb[];

long
tcpdopen(q, dev)
register struct queue *q;
dev_t dev;
{
	struct tcpcb *tp;

	dev = minor(dev);
	if(dev >= tcpcnt)
		return(0);
	tp = &tcpcb[dev];
	if(dev&01){
		/* outgoing channel */
		/*
		 *  disallow if active
		 */
		if(tp->t_state != TCPS_CLOSED)
			return(0);
		/*
		 *  only one opener at a time for outgoing channels
		 */
		if(q->ptr)
			return(0);
	} else {
		/* incoming channel */
		/*
		 *  disallow if inactive
		 */
		if(tp->t_state == TCPS_CLOSED) {
printf("tcp%d closed\n", dev);
			return(0);
		}
		/*
		 *  disallow if it's not open or waiting to be opened
		 */
		if((tp->so_state&(SS_OPEN|SS_PLEASEOPEN))==0) {
printf("tcp%d state %x\n", dev, tp->so_state);
			return(0);
		}
		/*
		 *  if already open, just accept
		 */
		if(q->ptr)
			return(1);
	}
	tcp_busy++;
	if((tp->so_state & SS_PLEASEOPEN) == 0){
		tp->so_rcount = tp->so_wcount = tp->so_options = 0;
		tp->so_laddr = tp->so_faddr = (in_addr)0;
		tp->so_lport = tp->so_fport = (tcp_port)0;
		tp->so_oobmark = tp->so_delimcnt = 0;
		tp->so_head = (struct tcpcb *)0;
		tp->so_state = SS_WAITING;
		tp->t_maxseg = TCP_DEFMAXSEG;	/* until changed by other end */
	}
	tp->so_state |= SS_OPEN;
	tp->so_dev = dev;
	tp->so_rq = q;
	tp->so_wq = WR(q);
	q->ptr = (caddr_t)tp;
	WR(q)->flag |= QBIGB;
	WR(q)->ptr = (caddr_t)tp;
	--tcp_busy;
	if(tp->so_state & SS_PLEASEOPEN){
		tp->so_state &= ~SS_PLEASEOPEN;
		qenable(WR(q));		/* to force out rcv wnd update */
	}
	return(1);
}

tcpdclose(q)
register struct queue *q;
{
	struct tcpcb *tp;

	tp = (struct tcpcb *)q->ptr;
	tcp_busy++;
	if(tp == 0){
		--tcp_busy;
		return;
	}
	tp->so_state &= ~(SS_OPEN|SS_WAITING);
	tp->so_rq = tp->so_wq = (struct queue *) 0;
	tp->so_wcount = tp->so_rcount = 0;
	if(tp->t_state > TCPS_LISTEN)
		tcp_disconnect(tp);
	else
		tcp_close(tp);
	--tcp_busy;
}

tcpdput(q, bp)
register struct queue *q;
register struct block *bp;
{
	int s, x;
	struct tcpcb *tp;
	struct block *nbp;
	struct foo {
		int com;
		struct tcpuser rep;
	};
	struct foo *fp;

	tp = (struct tcpcb *)q->ptr;
	switch(bp->type){
	case M_IOCTL:
		bp->type = M_IOCACK;
		switch(stiocom(bp)){
		case TIOCSETP:
		case TIOCSETN:
			x = ((struct ttydevb *)stiodata(bp))->ispeed;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			if(x == 0)
				qpctl(OTHERQ(q), M_HANGUP);
			return;
		case TIOCGETP:
			((struct ttydevb *)stiodata(bp))->ispeed =
			  ((struct ttydevb *)stiodata(bp))->ospeed = B9600;
			break;
		case TCPGETADDR:
			nbp = allocb(sizeof(struct foo));
			if (nbp == NULL) {
				bp->type = M_IOCNAK;
				break;
			}
			nbp->type = M_IOCACK;
			fp = (struct foo *)nbp->rptr;
			fp->com = stiocom(bp);
			fp->rep.lport = tp->so_lport;
			fp->rep.fport = tp->so_fport;
			fp->rep.laddr = tp->so_laddr;
			fp->rep.faddr = tp->so_faddr;
			nbp->wptr = nbp->rptr + sizeof(struct foo);
			freeb(bp);
			bp = nbp;
			break;
		case TCPIOHUP:
			tp->so_state |= SS_HANGUP;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;
		default:
			bp->type = M_IOCNAK;
		}
		qreply(q, bp);
		return;
	case M_DATA:
		if(socantsendmore(tp)){
			freeb(bp);
			return;
		}
		s = spl6();
		if (bp->wptr!=bp->rptr)
			tp->so_delimcnt = 0;
		if (bp->class&S_DELIM)
			tp->so_delimcnt++;	/* 2 delims are logical EOF */
		if (bp->wptr==bp->rptr){
			freeb(bp);
		} else {
			tp->so_wcount += BLEN(bp);	/* BEFORE the putq */
			putq(q, bp);
		}
		splx(s);
		if(tp->so_delimcnt
		 || (tp->so_options&(SO_ACCEPTCONN|SS_WAITING)) == 0)
			qenable(q);
		break;
	default:
		freeb(bp);
		break;
	}
}

tcpdosrv(q)
struct queue *q;
{
	register struct tcpcb *tp;

	if((tp = (struct tcpcb *)q->ptr) == 0)
		return;
	if (tp->so_state&SS_WCLOSED)
		return;
	tcp_busy++;
	if(tp->so_delimcnt > 1){
		tp->so_state |= SS_WCLOSED;
		tp = tcp_usrclosed(tp);
		if(tp)
			tcp_output(tp);
	} else if ((tp->so_options & SO_ACCEPTCONN) == 0
	       &&  (tp->so_state & SS_WAITING) == 0)
		tcp_output(tp);
	else
		tcpduser(tp);
	--tcp_busy;
}

tcpdrint(bp, tp)
register struct block *bp;
register struct tcpcb *tp;
{
	register struct block *bp1;

	if (tp->so_rq == NULL) {
		printf("tcpdrint but no tp->so_rq\n");
		bp_free(bp);
		return -1;
	}
	while(bp){
		bp1 = bp->next;
		tp->so_rcount += bp->wptr - bp->rptr;
		if(bp->wptr <= bp->rptr)
			freeb(bp);
		else
			putq(tp->so_rq, bp);
		bp = bp1;
	}
	return 0;
}

tcpdisrv(q)
register struct queue *q;
{
	register struct tcpcb *tp = (struct tcpcb *)(q->ptr);
	register struct block *bp;

	while ((q->next->flag&QFULL) == 0 && (bp = getq(q)) != NULL) {
		if(bp->type == M_DATA)
			tp->so_rcount -= bp->wptr - bp->rptr;
		if(tp->so_rcount < 0){
			printf("so_rcount %d\n", tp->so_rcount);
			if(tp->so_state & SS_OPEN)
				panic("so_rcount");
		}
		(*q->next->qinfo->putp)(q->next, bp);
	}
	if(q->count <= q->qinfo->lolimit)
		qenable(OTHERQ(q));	/* update remote send window */
}

/*
 * here if channel isn't connected yet:
 * output `data' is a user command
 * grab all the data written so far,
 * even though we'll discard the part we don't use
 */
tcpduser(tp)
register struct tcpcb *tp;
{
	extern struct ipif *ip_ifwithaddr();
	struct tcpuser *tu;
	struct block *bp, *tail, *head;

	tail = head = NULL;
	while(bp = getq(tp->so_wq)){
		if(bp->type != M_DATA){
			freeb(bp);
			continue;
		}
		bp->next = NULL;
		if (head == NULL)
			head = bp;
		else
			tail->next = bp;
		tail = bp;
	}
	if(head == NULL)
		return;
	tp->so_wcount = 0;
	bp = head;
	if(BLEN(bp) < sizeof(struct tcpuser)){
		bp_free(bp);
		return;
	}

	/*
	 *  tu->code is a request to bind this channel
	 */
	tu = (struct tcpuser *)bp->rptr;
	switch(tu->code){
	case TCPC_CONNECT:
		if(tp->t_state != TCPS_CLOSED){
			tu->code = TCPC_BADDEV;
			goto bad;
		}
		if (tu->laddr != INADDR_ANY) {
			/* has the user specified a legal local address? */
			if (ip_ifwithaddr(tu->laddr) == 0) {
				tu->code = TCPC_BADLOCAL;
				goto bad;
			}
		} else {
			/* pick a local address related to the destination */
			tu->laddr = ip_hoston(tu->faddr);
			if(tu->laddr == INADDR_ANY) {
				tu->code = TCPC_NOROUTE;
				goto bad;
			}
		}
		tp->so_options = tu->param & ~SO_ACCEPTCONN;
		if(tcpcb_bind(tp, tu->laddr, tu->lport)) {
			tu->code = TCPC_BOUND;
			goto bad;
		}
		tp->so_fport = tu->fport;
		tp->so_faddr = tu->faddr;
		tcp_template(tp);
		if(tp->t_template == (struct block *)0) {
			printf("no template - ");
			tu->code = TCPC_BADDEV;
			goto bad;
		}
		if (tp->so_options & SO_KEEPALIVE)
			tcp_timers(tp, TCPT_KEEP);
		tp->t_state = TCPS_SYN_SENT;
		tp->iss = tcp_iss; tcp_iss += TCP_ISSINCR/2;
		tcp_sendseqinit(tp);
		tp->so_state &= ~SS_WAITING;
		tcp_output(tp);
		break;
	case TCPC_LISTEN:
		if(tp->t_state != TCPS_CLOSED){
			tu->code = TCPC_BADDEV;
			goto bad;
		}
		if (tu->laddr != INADDR_ANY) {
			/* has the user specified a legal local address? */
			if (ip_ifwithaddr(tu->laddr) == 0) {
				tu->code = TCPC_BADLOCAL;
				goto bad;
			}
		}
		tp->so_options = tu->param & ~SO_ACCEPTCONN;
		if(tcpcb_bind(tp, tu->laddr, tu->lport)) {
			tu->code = TCPC_BOUND;
			goto bad;
		}
		tp->t_state = TCPS_LISTEN;
		tp->so_options |= SO_ACCEPTCONN;
		tp->so_fport = tu->fport==0 ? TCPPORT_ANY : tu->fport;
		tp->so_faddr = tu->faddr;
		tcp_template(tp);
		if(tp->t_template == 0) {
			tu->code = TCPC_BADDEV;
			printf("no template - ");
			goto bad;
		}
		tp->so_state &= ~SS_WAITING;
		if (tcp_isconnected(tp)<0) {
			tu->code = TCPC_BADDEV;
			goto bad;
		}
		break;
	default:
		tu->code = TCPC_SORRY;
		goto bad;
	}
	bp_free(bp);
	return;
bad:
	/*
	 *  send reason for failure back to user.
	 */
	if(bp->next) {
		bp_free(bp->next);
		bp->next = 0;
	}
	(*tp->so_rq->next->qinfo->putp)(tp->so_rq->next, bp);
	tcp_hungup(tp);
}

struct tcpcb *
tcp_disconnect(tp)
register struct tcpcb *tp;
{

	if(tp->t_state < TCPS_ESTABLISHED)
		tp = tcp_close(tp);
	else {
		tp->so_state &= ~SS_PLEASEOPEN;
		tcp_hungup(tp);	/* sends M_HANGUP */
		tp = tcp_usrclosed(tp);
		if(tp)
			tcp_output(tp);
	}
	return(tp);
}

struct tcpcb *
tcp_usrclosed(tp)
register struct tcpcb *tp;
{

	switch(tp->t_state){

	case TCPS_CLOSED:
	case TCPS_LISTEN:
	case TCPS_SYN_SENT:
		tp->t_state = TCPS_CLOSED;
		tp = tcp_close(tp);
		break;

	case TCPS_SYN_RECEIVED:
	case TCPS_ESTABLISHED:
		tp->t_state = TCPS_FIN_WAIT_1;
		tp->so_options |= SO_KEEPALIVE;
		tcp_timers(tp, TCPT_KEEP);
		break;

	case TCPS_CLOSE_WAIT:
		tp->t_state = TCPS_LAST_ACK;
		tp->so_options |= SO_KEEPALIVE;
		tcp_timers(tp, TCPT_KEEP);
		break;
	}
	if(tp && tp->t_state >= TCPS_FIN_WAIT_2) {
		tp->so_state &= ~(SS_PLEASEOPEN|SS_RCVATMARK);
		if(tp->so_state&SS_OPEN && !(tp->so_state&SS_HUNGUP)) {
			tp->so_state |= SS_HUNGUP;
			tcp_hungup(tp);
		}
	}
	return(tp);
}

tcp_isconnected(tp)
struct tcpcb *tp;
{
	struct block *bp;
	struct tcpuser *tu;
	struct tcpcb *rtp;

	if(tp->so_head)
		rtp = tp->so_head;
	else
		rtp = tp;
	if((rtp->so_state & SS_OPEN) == 0){
		printf("isconnected, no fd ref\n");
		return -1;
	}
	bp = allocb(sizeof(struct tcpuser));
	if(bp == 0)
		return -1;
	bp->wptr += sizeof(struct tcpuser);
	tu = (struct tcpuser *)bp->rptr;
	tu->code = TCPC_OK;
	tu->fport = tp->so_fport;
	tu->faddr = tp->so_faddr;
	tu->lport = tp->so_lport;
	tu->laddr = tp->so_laddr;
	tu->param = tp->so_dev;
	bp->class |= S_DELIM;
	return tcpdrint(bp, rtp);
}

tcp_hungup(tp)
register struct tcpcb *tp;
{
	register struct queue *q;

	q = tp->so_rq;
	if(q == 0)
		return;
	qpctl(q, M_HANGUP);
}

/*
 * find a spare even-numbered tcp device for a new passive-end
 * connection.
 */
struct tcpcb *
tcp_newconn(tp)
register struct tcpcb *tp;
{
	register struct tcpcb *ntp;
	register struct tcpcb *tend;

	if(tp->so_rq && (tp->so_rq->flag&QFULL)){
		printf("listen %d q full\n", tp->so_lport);
		return(0);
	}
	tend = &tcpcb[tcpcnt];
	for(ntp = &tcpcb[0]; ntp < tend; ntp += 2){
		if(ntp->t_state == TCPS_CLOSED){
			bzero(ntp, sizeof(struct tcpcb));
			ntp->t_maxseg = TCP_DEFMAXSEG;
			ntp->so_options = tp->so_options & SO_KEEPALIVE;
			ntp->so_head = tp;
			ntp->so_dev = ntp - tcpcb;
			return(ntp);
		}
	}
	return(0);
}

struct tcpcb *
tcpcb_lookup(faddr, fport, laddr, lport)
register in_addr faddr, laddr;
register tcp_port fport, lport;
{
	register struct tcpcb *tp;
	int highscore = 0, score;
	struct tcpcb *match = 0;
	struct tcpcb *tend;

	tend = &tcpcb[tcpcnt];
	for(tp = &tcpcb[0]; tp < tend; tp++){
		if(tp->t_state == TCPS_CLOSED)
			continue;
		score = 22;
		if(tp->so_faddr != faddr) {
			if(tp->so_faddr != INADDR_ANY)
				continue;
			else
				score -=8;
		}
		if(tp->so_fport != fport) {
			if(tp->so_fport != TCPPORT_ANY)
				continue;
			else
				score -=8;
		}
		if(tp->so_laddr != laddr) {
			if(tp->so_laddr == in_netof(laddr))
				score -=1;
			else if(tp->so_laddr != INADDR_ANY)
				continue;
			else
				score -=2;
		}
		if(tp->so_lport != lport) {
			if(tp->so_lport != TCPPORT_ANY)
				continue;
			else
				score -=4;
		}
		if (score==22)
			return tp;
		if (score<highscore)
			continue;
		match = tp;
		highscore = score;
	}
	return(match);
}

/* n chars were acked; drop them now */
sbsnddrop(tp, n)
register struct tcpcb *tp;
register int n;
{
	register struct queue *q;
	register int i;
	register struct block *bp;

	q = tp->so_wq;
	if(q == 0)
		return;
	bp = 0;
	while(n > 0 && (bp = getq(q))){
		i = MIN(BLEN(bp), n);
		bp->rptr += i;
		n -= i;
		tp->so_wcount -= i;
		if(bp->rptr >= bp->wptr){
			freeb(bp);
			bp = 0;
		} else if(n > 0){
			panic("sbsnddrop");
		}
	}
	if(bp)
		putbq(q, bp);
}

static tcp_port portnext[] = { 600, 1024 };
static tcp_port portlow[] = { 600, 1024 };
static tcp_port porthigh[] = { 1024, 2048 };

tcpcb_bind(tp, addr, port)
register struct tcpcb *tp;
register in_addr addr;
register tcp_port port;
{
	register int i;

	tp->so_lport = 0;
	if(port){
		/*
		 *  Don't let just anyone get a trusted port.
		 */
		if(port<porthigh[0] && u.u_uid!=0)
			return(1);
		tp->so_lport = port;
		tp->so_laddr = addr;
		return(0);
	}
	/*
	 *  No specific port, pick one.  Root only gets a trusted
	 *  port if explicitly requested, so privileged programs
	 *  can make non-privileged calls.
	 */
	i = (u.u_uid==0 && (tp->so_options&SO_TRUSTED)) ? 0 : 1;
	if(portnext[i] >= porthigh[i])
		portnext[i] = portlow[i];
	port = portnext[i];
	while(1){
		if(tcpcb_bind(tp, addr, portnext[i]) == 0){
			portnext[i]++;
			return(0);
		}
		portnext[i]++;
		if(portnext[i] >= porthigh[i])
			portnext[i] = portlow[i];
		if(portnext[i] == port)	/* tried them all */
			break;
	}
	return(1);
}

tcp_cantrcvmore(tp)
register struct tcpcb *tp;
{
	register struct queue *q;

	q = tp->so_rq;
	if(q == NULL)
		return;
	if(tp->so_state & SS_HANGUP)
		qpctl(q, M_HANGUP);
	else {
		/* two delims ensure a zero length read at the process */
		qpctld(q, M_DATA);
		qpctld(q, M_DATA);
	}
}
