/*	tcp.h	6.1	83/07/29	*/

typedef unsigned short tcp_port;
#define TCPPORT_ANY	0x8000	/* wild card for port number */
typedef	unsigned long	tcp_seq;

#define TCP_DEFMAXSEG 512	/* default max segment size */

#ifdef KERNEL
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr {
	tcp_port	th_sport;	/* source port */
	tcp_port	th_dport;	/* destination port */
	tcp_seq		th_seq;		/* sequence number */
	tcp_seq		th_ack;		/* acknowledgement number */
#ifdef vax
	u_int	th_x2:4,		/* (unused) */
		th_off:4;		/* data offset */
#endif vax
	u_char	th_flags;
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
	u_short	th_win;			/* window */
	u_short	th_sum;			/* checksum */
	u_short	th_urp;			/* urgent pointer */
};

#define	TCPOPT_EOL	0
#define	TCPOPT_NOP	1
#define	TCPOPT_MAXSEG	2
#endif KERNEL
