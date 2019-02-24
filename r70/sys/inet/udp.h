/*	udp.h	6.1	83/07/29	*/

typedef unsigned short udp_port;
#define UDPPORT_ANY	0x8000	/* wild card for port number */

#ifdef KERNEL
/*
 * Udp protocol header.
 * Per RFC 768, September, 1981.
 */
struct udphdr {
	u_short	uh_sport;		/* source port */
	u_short	uh_dport;		/* destination port */
	short	uh_ulen;		/* udp length */
	u_short	uh_sum;			/* udp checksum */
};

struct udp {
	int	flags;
	u_short	dport;
	u_long	dst;
	u_short	sport;
	u_long	src;
	struct queue *rq;
	struct block *head;
	struct block *tail;
};
/* values for udp flags */
#define	UDP_INIT	1
#define	UDP_LISTEN	2
#define UDP_CONNECTED	4

#endif KERNEL

#define UDP_BODY_LIMIT 512	/* maximum size of a UDP message body */
