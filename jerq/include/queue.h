#define	QUEUE_H
#define	NCHARS	2560
#define NULL ((char *)0)

#define	STOPPED		1
#define	QPRIORITY	2

struct clist {
	long	c_tail;
	long	c_head;
	short	c_cc;
	short	state;
};

#ifdef QUEUE_C
short	cbuf_next[NCHARS];
char	cbuf_word[NCHARS];
struct clist queues[3];
int freelist, freemark;
#else
extern short	cbuf_next[NCHARS];
extern char	cbuf_word[NCHARS];
extern struct clist queues[3];
extern int freelist, freemark;
#endif

#define	RCVQUEUE	queues[0]	/* chars from host */
#define	KBDQUEUE	queues[1]	/* chars from keyboard */
#define	OUTQUEUE	queues[2]	/* chars to send to host */
