/*	ip_var.h	6.1	83/07/29	*/

/*
 * Overlay for ip header used by other protocols (tcp, udp).
 */
struct ipovly {
	struct	tcpiphdr *ih_next;	/* for protocol sequence q's */
	struct	block *ih_bp;		/* bookkeeping for above queue seq */
	u_char	ih_x1;			/* (unused) */
	u_char	ih_pr;			/* protocol */
	short	ih_len;			/* protocol length */
	in_addr	ih_src;		/* source internet address */
	in_addr	ih_dst;		/* destination internet address */
};

/*
 * Ipq points to the ip reassembly queue.
 *
 * Ip reassembly queue structure.  Each fragment being reassembled
 * has one of these structures as the data in the header block structure.
 *
 * They are timed out after ipq_ttl drops to 0, and may also
 * be reclaimed if memory becomes tight.
 */
struct ipq {
	struct	block *next,*prev;	/* to other reass headers */
	u_char	ipq_ttl;		/* time for reass q to live */
	u_char	ipq_p;			/* protocol of this fragment */
	u_short	ipq_id;			/* sequence id for reassembly */
	in_addr	ipq_src,ipq_dst;
};

/*
 * Ip header, when holding a fragment.
 *
 * Note: ipf_next must be at same offset as ipq_next above
 */
struct	ipasfrag {
#ifdef vax
	u_int	ip_hl:4,
		ip_v:4;
#endif
	u_char	ip_tos;
	short	ip_len;
	u_short	ip_id;
	short	ip_off;
	u_char	ip_ttl;
	u_char	ip_p;
	u_short	ip_sum;
	struct	block *ipf_next;	/* next fragment */
	struct	block *ipf_prev;	/* previous fragment */
};

struct	ipstat {
	int	ips_badsum;		/* checksum bad */
	int	ips_tooshort;		/* packet too short */
	int	ips_toosmall;		/* not enough data */
	int	ips_badhlen;		/* ip header length < data size */
	int	ips_badlen;		/* ip length < ip header length */
	int	ips_qfull;
	int	ips_route;		/* routing output errors */
	int	ips_fragout;		/* fragmented packets */
};

/* flags passed to ip_output as last parameter */
#define	IP_FORWARDING		0x1	/* most of ip header exists */
#define	IP_ROUTETOIF		0x10	/* same as SO_DONTROUTE */

#ifdef KERNEL
struct	ipstat	ipstat;
u_short	ip_id;				/* ip packet ctr, for ids */
#endif

/*
 * interface stuff
 */
struct ipif{
	struct queue *queue;
	int flags;
	int mtu;
	in_addr thishost;
	in_addr that;
	in_addr mask;
	in_addr bcast[6];		/* possible broadcast addresses */
	int ipackets, ierrors;
	int opackets, oerrors;
	int arp;
	int dev;
};
#define IFF_UP		0x1
#define IFF_HOST	0x2
#define IFF_ARP		0x4

#ifdef KERNEL
extern struct ipif ipif[];
extern struct ipif *ip_ifwithaddr(), *ip_ifonnetof();
#endif

#define IPIOHOST	(('i'<<8)|1)
#define IPIONET		(('i'<<8)|2)
#define IPIOLOCAL	(('i'<<8)|3)
#define IPIOARP		(('i'<<8)|4)
#define IPIORESOLVE	(('i'<<8)|5)
#define IPIOMTU		(('i'<<8)|6)
#define IPIOROUTE	(('i'<<8)|7)
#define IPIOGETIFS	(('i'<<8)|8)
#define IPIOMASK	(('i'<<8)|9)

#define IP_BODY_LIMIT 8192
#define IP_MSG_LIMIT (sizeof(struct ipovly) + IP_BODY_LIMIT)

/* an ip routing record */
struct ip_route{
	long time;		/* time of last access */
	in_addr dst;		/* destination */
	in_addr gate;		/* gate to use for this destination */
};

#ifdef KERNEL
/* structure returned by "ip_route" */
struct ip_route_info {
	in_addr	addr;
	struct ipif *ifp;
};
struct ip_route_info ip_route();
#endif KERNEL

/* an ip to ethernet address mapping */
struct ip_arp{
	long time;			/* time of last access */
	in_addr inaddr;			/* inet address */
	unsigned char enaddr[6];	/* ethernet address */
};

#define BLEN(bp) ((bp)->wptr - (bp)->rptr)
#define BSZ(bp) ((bp)->lim - (bp)->base)
#define MAXBLEN	64

/* #define MGET(m, x, y)	((m) = bp_get(x, y)) */

#ifdef KERNEL
extern struct block *bp_copy();
extern struct block *bp_get();
#endif

#ifdef IN_PARANOID
extern struct block cblock[];
extern int blkcnt;
#define BLOCKCHK(bp)\
	if(bp < cblock || cblock >= &cblock[blkcnt])\
		panic("bp_check bad bp");\
	if(bp->base == 0 || bp->lim == 0)\
		panic("bp_check 0");\
	if(bp->rptr > bp->lim || bp->rptr < bp->base)\
		panic("bp_check rptr");\
	if(bp->wptr > bp->lim || bp->wptr < bp->base)\
		panic("bp_check wptr");\
	if(bp->rptr > bp->wptr)\
		panic("bp_check rptr > wptr")
#define MCHECK(bp) bp_check(bp)
#else
#define BLOCKCHK(bp)
#define MCHECK(bp)
#endif
