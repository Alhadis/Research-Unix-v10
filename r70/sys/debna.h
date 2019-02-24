/*
 * definitions for the DEBNA driver
 *
 * each hardware device has eight channels;
 * each channel dedicated to a particular Ethernet protocol
 * one bnachan per channel
 */
#define	BNACHAN	8

struct bnachan {
	short proto;	/* our protocol */
	char needst;	/* channel needs stptdb */
	unsigned char dcnt;	/* number of queued packets */
	char unit;	/* which controller is ours */
	struct queue *rq;
};

/*
 * one bnactl per controller
 */
struct bnactl {
	struct bvp bvp;		/* general port stuff */
	struct dgi *xfree;	/* free xmit buffers */
	char lastx;		/* last channel transmitted -- fairness */
	char needst;		/* at least one channel needs STPTDB */
	char myaddr[6];		/* 6 slightly magic */
	struct bnachan chan[BNACHAN];
};

/*
 * our buffers
 * BNASLOP is there to allow two kinds of alignment in memory:
 * 1.  interlocked queue instructions require quadword-aligned headers.
 * 2.  a bug in the DEBNA causes it to choke if a BVP message or datagram
 * header crosses a page boundary; the longest such header is 28 bytes
 * it is assumed that a header misalignment can happen only once per
 * set of buffers, because the numbers happen to make it true
 * it would be easier, but more wasteful of memory, to make BNABSIZE
 * an even multiple of the page size
 * we have to allow slop and fix it up at runtime because the
 * VAX loader guarantees no more than longword alignment
 */
#define	BNABSIZE 1600	/* must be multiple of eight -- quad-align */
#define	BNASLOP	(8+28)
#define	BNAXBUF	8
#define	BNARBUF	8

struct bnabuf {
	char rbuf[BNARBUF*BNABSIZE + BNASLOP];
	char xbuf[BNAXBUF*BNABSIZE + BNASLOP];
};
