/*	tcp_var.h	6.1	83/07/29	*/

/*
 * Kernel variables for tcp.
 */

/*
 * Tcp control block, one per tcp; fields:
 */
struct tcpcb {
	struct tcpiphdr	*seg_next;	/* sequencing queue for tcpiphdr */
	struct queue	*so_rq;
	struct queue	*so_wq;
	int		so_rcount;
	int		so_wcount;	/* because q->count is inaccurate */
	struct	tcpcb	*so_head;	/* parent who listened */
	int		so_dev;
	int		so_state;
	int		so_options;
	in_addr		so_laddr;
	in_addr		so_faddr;
	tcp_port	so_lport;
	tcp_port	so_fport;
	int		so_oobmark;
	int		so_delimcnt;	/* to detect logical eof */
	short	t_state;		/* state of this connection */
	short	t_timer[TCPT_NTIMERS];	/* tcp timers */
	short	t_rxtshift;		/* log(2) of rexmt exp. backoff */
	struct block	*t_tcpopt;	/* tcp options */
	struct block	*t_ipopt;	/* ip options */
	short	t_maxseg;		/* maximum segment size */
	char	t_force;		/* 1 if forcing out a byte */
	u_char	t_flags;
#define	TF_ACKNOW	0x01			/* ack peer immediately */
#define	TF_DELACK	0x02			/* ack, but try to delay it */
#define	TF_DONTKEEP	0x04			/* don't use keep-alives */
#define	TF_NOOPT	0x08			/* don't use tcp options */
	struct block	*t_template;	/* skeletal tcpiphdr - packet for transmit */
	struct inpcb	*t_inpcb;	/* back pointer to internet pcb */
/*
 * The following fields are used as in the protocol specification.
 * See RFC783, Dec. 1981, page 21.
 */
/* send sequence variables */
	tcp_seq	snd_una;		/* send unacknowledged */
	tcp_seq	snd_nxt;		/* send next */
	tcp_seq	snd_up;			/* send urgent pointer */
	tcp_seq	snd_wl1;		/* window update seg seq number */
	tcp_seq	snd_wl2;		/* window update seg ack number */
	tcp_seq	iss;			/* initial send sequence number */
	u_short	snd_wnd;		/* send window */
/* receive sequence variables */
	short	rcv_wnd;		/* receive window */
	tcp_seq	rcv_nxt;		/* receive next */
	tcp_seq	rcv_up;			/* receive urgent pointer */
	tcp_seq	irs;			/* initial receive sequence number */
/*
 * Additional variables for this implementation.
 */
/* receive variables */
	tcp_seq	rcv_adv;		/* advertised window */
/* retransmit variables */
	tcp_seq	snd_max;		/* highest sequence number sent
					   used to recognize retransmits */
/* transmit timing stuff */
	short	t_idle;			/* inactivity time */
	short	t_rtt;			/* round trip time */
	tcp_seq	t_rtseq;		/* sequence number being timed */
	float	t_srtt;			/* smoothed round-trip time */
/* out-of-band data */
	char	t_oobflags;		/* have some */
	char	t_iobc;			/* input character */
#define	TCPOOB_HAVEDATA	0x01
};

struct	tcpstat {
	int	tcps_badsum;
	int	tcps_badoff;
	int	tcps_hdrops;
	int	tcps_badsegs;
	int	tcps_unack;
	int	tcps_timeouts[4];	/* timeouts while ESTABLISHED */
	int	tcps_duplicates;	/* other side retransmitting */
	int	tcps_delayed;		/* packets out of sequence? */
};

#define SO_DONTROUTE	0x1
#define SO_KEEPALIVE	0x2
#define SO_ACCEPTCONN	0x4	/* this is real */
#define SO_TRUSTED	0x8	/* use a trusted < 1024 port */

#define SS_OPEN		0x1	/* by user */
#define SS_PLEASEOPEN	0x2	/* waiting for user open */
#define SS_RCVATMARK	0x4	/* some kind of OOB */
#define SS_WAITING	0x8	/* wait for user control */
#define SS_HANGUP	0x20	/* HANGUP on TH_FIN */
#define SS_HUNGUP	0x40	/* socket has been hung up (avoid multiple) */
#define SS_WCLOSED	0x80	/* write side is closed */

#define socantsendmore(tp) (tp->so_delimcnt>1 || tp->so_state&SS_WCLOSED)
#define sbrcvspace(tp)	(tp->so_rq ?\
			 (sorcvhiwat(tp) - tp->so_rcount)\
			 : 0)
#define sosndcc(tp)	(tp->so_wq ? (tp->so_wcount) : 0)
#define sototcpcb(tp)	(tp->so_tcpcb)
#define sorcvhiwat(tp)	(tp->so_rq ? (tp->so_rq->qinfo->limit)\
			: 0)
#define sohasoutofband(tp)

#ifdef KERNEL
extern struct tcpcb *tcpcb_lookup();
extern struct tcpcb *tcp_newconn();
#endif
#ifdef KERNEL
struct	tcpstat tcpstat;	/* tcp statistics */
struct	tcpcb *tcp_close(), *tcp_drop();
struct	tcpcb *tcp_timers(), *tcp_disconnect(), *tcp_usrclosed();
#endif
