
/*
Data structures to help debug tcp.
*/

struct tcpdebug {
	unsigned long	seq;	/* sequence number */
	time_t	stamp;		/* time stamp */
	short	inout;		/* flag to note if input (1) or output (0) mess */
	short	length;		/* length in bytes of packet data */
	struct tcpiphdr hdr;	/* header of the sent/received message */
};

#define SIZDEBUG	64	/* number of headers to save in debug queue */
