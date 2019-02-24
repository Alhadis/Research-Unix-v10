#include	"u.h"
#include	"lib.h"
#include	"mem.h"
#include	"dat.h"
#include	"fns.h"
#include	"io.h"
#include	"errno.h"

#define NOW (MACHP(0)->ticks*MS2HZ)

enum {
	/*
	 *  configuration parameters
	 */
	Nconv = 32,		/* total number of active circuits */
	Nifc = 2,		/* number of interfaces */
	MSrexmit = 300,		/* retranmission interval in ms */
	MSack = 50,		/* ms to sit on an ack */

	/*
	 *  relative or immutable
	 */
	Ndir = Nconv + 1,	/* entries in the nonet directory */
	Nsubdir = 2,		/* entries in the nonet directory */
	Nmsg = 128,		/* max number of outstanding messages */
	Nmask = Nmsg - 1,	/* mask for log(Nmsg) bits */
};

typedef struct Hdr		Hdr;
typedef struct Msg		Msg;
typedef struct Conversation	Conversation;
typedef struct Interface	Interface;
typedef struct Etherhdr		Etherhdr;

/*
 *  generic nonet header
 */
struct Hdr {
	uchar	circuit[3];	/* circuit number */
	uchar	flag;
	uchar	mid;		/* message id */
	uchar	ack;		/* piggy back ack */
	uchar	remain[2];	/* count of remaing bytes of data */
	uchar	sum[2];		/* checksum (0 means none) */
};
#define HDRSIZE 10
#define NEWCALL	0x1		/* flag bit marking a new circuit */
#define HANGUP 0x2		/* flag bit requesting hangup */
#define ACKME 0x4		/* acknowledge this message */

/*
 *  a buffer describing a nonet message
 */
struct Msg {
	QLock;
	Blist;
	int	mid;		/* sequence number */
	int	rem;		/* remaining */
	long	time;
	int	acked;
};

/*
 *  Nonet conversation states (for Conversation.state)
 */
enum {
	Cclosed,
	Copen,
	Clistening,
	Cconnected,
	Cconnecting,
	Chungup,
	Cclosing,
};

/*
 *  one exists for each Nonet conversation.
 */
struct Conversation {
	QLock;

	Queue	*rq;		/* input queue */
	int	version;	/* incremented each time struct is changed */
	int	state;		/* true if listening */

	Msg	in[Nmsg];	/* messages being received */
	int	rcvcircuit;	/* circuit number of incoming packets */

	uchar	ack[Nmsg];	/* acknowledgements waiting to be sent */
	long	atime[Nmsg];
	int	afirst;
	int	anext;

	QLock	xlock;		/* one trasmitter at a time */
	Rendez	r;		/* process waiting for an output mid */
	Msg	ctl;		/* for control messages */
	Msg	out[Nmsg];	/* messages being sent */
	int	first;		/* first unacknowledged message */
	int	next;		/* next message buffer to use */
	int	lastacked;	/* last message acked */		
	Block	*media;		/* prototype media output header */
	Hdr	*hdr;		/* nonet header inside of media header */

	Interface *ifc;
	int	kstarted;
	char	raddr[64];	/* remote address */
	int	rexmit;		/* statistics */
	int	retry;
	int	bad;
	int	sent;
	int	rcvd;
};

/*
 *  a nonet interface.  one exists for every stream that a 
 *  nonet multiplexor is pushed onto.
 */
struct Interface {
	QLock;
	int	ref;
	char	name[64];	/* interface name */		
	int	maxtu;		/* maximum transfer unit */
	int	mintu;		/* minimum transfer unit */
	int	hsize;		/* media header size */
	Queue	*wq;		/* interface output queue */
	void	(*connect)(Conversation *, char *);
	Conversation conv[Nconv];
};
static Interface interface[Nifc];

void nonetkproc(void *);
int cksum(Block*, int);
extern Qinfo noetherinfo;

/*
 *  start a new conversation.  start an ack/retransmit process if
 *  none already exists for this circuit.
 */
static void
startconv(Conversation *cp, int circuit, int state)
{
	int i;
	char name[32];
	Interface *ifc;

	ifc = cp->ifc;

	/*
	 *  allocate the prototype header
	 */
	cp->media = allocb(ifc->hsize + HDRSIZE);
	cp->media->wptr += ifc->hsize + HDRSIZE;
	cp->hdr = (Hdr *)(cp->media->rptr + ifc->hsize);

	/*
	 *  fill in the circuit number
	 */
	cp->hdr->flag = NEWCALL|ACKME;
	cp->hdr->circuit[2] = circuit>>16;
	cp->hdr->circuit[1] = circuit>>8;
	cp->hdr->circuit[0] = circuit;

	/*
	 *  set the state variables
	 */
	cp->state = state;
	cp->retry = 0;
	for(i = 1; i < Nmsg; i++){
		cp->in[i].mid = i;
		cp->in[i].acked = 0;
		cp->in[i].rem = 0;
		cp->out[i].mid = i | Nmsg;
		cp->out[i].acked = 1;
		cp->out[i].rem = 0;
	}
	cp->in[0].mid = Nmsg;
	cp->in[0].acked = 0;
	cp->in[0].rem = 0;
	cp->out[0].mid = 0;
	cp->out[0].acked = 1;
	cp->out[0].rem = 0;
	cp->first = cp->next = 1;
	cp->rexmit = cp->bad = cp->sent = cp->rcvd = cp->lastacked = 0;

	/*
	 *  used for demultiplexing
	 */
	cp->rcvcircuit = circuit ^ 1;

	/*
	 *  start the ack/rexmit process
	 */
	if(cp->kstarted == 0){
		cp->kstarted = 1;
		sprint(name, "**nonet%d**", cp - ifc->conv);
		kproc(name, nonetkproc, cp);
	}
}

/*
 *  connect to the destination whose name is pointed to by bp->rptr.
 */
void
connect(Conversation *cp, Block *bp)
{
	Interface *ifc;
 	char *iname;
	int len;
	int circuit;

	ifc = cp->ifc;
	qlock(ifc);
	if(waserror()){
		qunlock(ifc);
		if(cp->media){
			freeb(cp->media);
			cp->media = 0;
		}
		freeb(bp);
		nexterror();
	}

	/*
	 *  init
	 */
	startconv(cp, (++(cp->version) * Nconv) + 2*(cp - ifc->conv), Cconnecting);
	(*ifc->connect)(cp, (char *)bp->rptr);
	strncpy(cp->raddr, (char *)bp->rptr, sizeof(cp->raddr));

	qunlock(ifc);
	freeb(bp);
	poperror();
}

/*
 *  listen for calls from any interface
 */
static int
listendone(void *a)
{
	Conversation *cp;

	cp = (Conversation *)a;
	return cp->state != Clistening;
}
static void
listen(Conversation *cp, Block *bp)
{
	freeb(bp);
	if(cp->state >= Clistening){
		print("listen in use %d %ux %ux\n", cp->state, cp->ifc, interface);
		error(0, Einuse);
	}
	cp->state = Clistening;
	sleep(&cp->r, listendone, cp);
}

/*
 *  send a hangup signal up the stream to get all line disciplines
 *  to cease and desist
 */
static void
hangup(Conversation *cp)
{
	Block *bp;
	Queue *q;

	cp->state = Chungup;
	bp = allocb(0);
	bp->type = M_HANGUP;
	q = cp->rq;
	PUTNEXT(q, bp);
	wakeup(&cp->r);
}

/*
 *  process a message acknowledgement.  if the message
 *  has any xmit buffers queued, free them.
 */
static void
rcvack(Conversation *cp, int mid)
{
	Msg *mp;
	Block *bp;
	int i;

	mp = &cp->out[mid & Nmask];

	/*
	 *  if already acked, ignore
	 */
	if(mp->acked || mp->mid != mid)
		return;
	mp->acked = 1;
	cp->lastacked = mid;

	/*
	 *  free buffers
	 */
	qlock(mp);
	while(bp = getb(mp))
		freeb(bp);
	qunlock(mp);

	/*
	 *  advance the first pointer and wakeup any processes waiting for a mid
	 */
	if((mid&Nmask) == cp->first){
		cp->retry = 0;
		for(i = cp->first; i!=cp->next && cp->out[i].acked; i = (i+1)&Nmask)
			;
		cp->first = i;
		wakeup(&cp->r);
	}
}

/*
 *  queue an acknowledgement to be sent.  ignore it if we already have Nmsg
 *  acknowledgements queued.
 */
static void
queueack(Conversation *cp, int mid)
{
	int next;
	ulong now;

	now = NOW;
	next = (cp->anext + 1)&Nmask;
	if(next != cp->afirst){
		cp->ack[cp->anext] = mid;
		cp->atime[cp->anext] = now + MSack;
		cp->anext = next;
	}
	if(now > cp->atime[cp->afirst])
		wakeup(&cp->rq->r);
}

/*
 *  make a packet header
 */
Block *
mkhdr(Conversation *cp, int rem)
{
	Block *bp;
	Hdr *hp;

	bp = allocb(cp->ifc->hsize + HDRSIZE + cp->ifc->mintu);
	memcpy(bp->wptr, cp->media->rptr, cp->ifc->hsize + HDRSIZE);
	bp->wptr += cp->ifc->hsize + HDRSIZE;
	hp = (Hdr *)(bp->rptr + cp->ifc->hsize);
	hp->remain[1] = rem>>8;
	hp->remain[0] = rem;
	hp->sum[0] = hp->sum[1] = 0;
	return bp;
}

/*
 *  transmit a message.  this involves breaking a possibly multi-block message into
 *  a train of packets on the media.
 *
 *  called by nonetoput() and nonetkproc().  the qlock(mp) synchronizes these two
 *  processes.
 */
static void
sendmsg(Conversation *cp, Msg *mp)
{
	Interface *ifc;
	Queue *wq;
	int msgrem;
	int pktrem;
	int n;
	Block *bp, *pkt, *last;
	uchar *rptr;

	ifc = cp->ifc;
	if(ifc == 0)
		return;
	wq = ifc->wq->next;

	/*
	 *  one transmitter at a time
	 */
	qlock(&cp->xlock);

	/*
	 *  synchronize with rcvack, don't want to send while the
	 *  message is being freed.
	 */
	qlock(mp);

	if(waserror() || mp->acked){
		qunlock(&cp->xlock);
		qunlock(mp);
		return;
	}

	/*
	 *  get the next acknowledge to use if the next queue up
	 *  is not full.
	 */
	if(cp->afirst != cp->anext && cp->rq->next->len < 16*1024){
		cp->hdr->ack = cp->ack[cp->afirst];
		cp->afirst = (cp->afirst+1)&Nmask;
	}
	cp->hdr->mid = mp->mid;

	if(ifc->mintu > mp->len) {
		/*
		 *  short message:
		 *  copy the whole message into the header block
		 */
		last = pkt = mkhdr(cp, mp->len);
		for(bp = mp->first; bp; bp = bp->next){
			memcpy(pkt->wptr, bp->rptr, n = BLEN(bp));
			pkt->wptr += n;
		}
		memset(pkt->wptr, 0, n = ifc->mintu - mp->len);
		pkt->wptr += n;
	} else {
		/*
		 *  long message:
		 *  break up the message into interface packets and send them.
		 *  once around this loop for each non-header block generated.
		 */
		msgrem = mp->len;
		pktrem = msgrem > ifc->maxtu ? ifc->maxtu : msgrem;
		bp = mp->first;
		if(bp)
			rptr = bp->rptr;
		last = pkt = mkhdr(cp, msgrem);
		while(bp){
			/*
			 *  if pkt full, send and create new header block
			 */
			if(pktrem == 0){
				cksum(pkt, ifc->hsize);
				last->flags |= S_DELIM;
				(*wq->put)(wq, pkt);
				last = pkt = mkhdr(cp, -msgrem);
				pktrem = msgrem > ifc->maxtu ? ifc->maxtu : msgrem;
			}
			n = bp->wptr - rptr;
			if(n > pktrem)
				n = pktrem;
			last = last->next = allocb(0);
			last->rptr = rptr;
			last->wptr = rptr = rptr + n;
			msgrem -= n;
			pktrem -= n;
			if(rptr >= bp->wptr){
				bp = bp->next;
				if(bp)
					rptr = bp->rptr;
			}
		}
	}
	cksum(pkt, ifc->hsize);
	last->flags |= S_DELIM;
	(*wq->put)(wq, pkt);
	mp->time = NOW + MSrexmit;
	qunlock(mp);
	qunlock(&cp->xlock);
	poperror();
}

/*
 *  send a control message (hangup or acknowledgement).
 */
sendctlmsg(Conversation *cp, int flag, int new)
{
	cp->ctl.len = 0;
	cp->ctl.first = 0;
	cp->ctl.acked = 0;
	if(new)
		cp->ctl.mid = Nmsg^cp->out[cp->next].mid;
	else
		cp->ctl.mid = cp->lastacked;
	cp->hdr->flag |= flag;
	sendmsg(cp, &cp->ctl);
}

/*
 *  receive a message (called by the multiplexor; noetheriput, nofddiiput, ...)
 */
static void
rcvmsg(Conversation *cp, Block *bp)
{
	Block *nbp;
	Hdr *h;
	short r;
	int c;
	Msg *mp;
	int f;
	Queue *q;

	q = cp->rq;

	/*
	 *  grab the packet header, push the pointer past the nonet header
	 */
	h = (Hdr *)bp->rptr;
	bp->rptr += HDRSIZE;
	mp = &cp->in[h->mid & Nmask];
	r = (h->remain[1]<<8) | h->remain[0];
	f = h->flag;

	/*
	 *  if a new call request comes in on a connected channel, hang up the call
	 */
	if(h->mid==0 && (f & NEWCALL) && cp->state==Cconnected){
		freeb(bp);
		hangup(cp);
		return;
	}

	/*
	 *  ignore old messages and process the acknowledgement
	 */
	if(h->mid != mp->mid){
		if(r == 0){
			rcvack(cp, h->ack);
			if(f & HANGUP)
				hangup(cp);
		} else {
			if(r>0)
				queueack(cp, h->mid);
			cp->bad++;
		}
		freeb(bp);
		return;
	}

	if(r>=0){
		/*
		 *  start of message packet
		 */
		if(mp->first){
			cp->bad++;
			freeb(bp);
			return;
		}
		mp->rem = r;
	} else {
		/*
		 *  a continuation
		 */
		if(-r != mp->rem) {
			cp->bad++;
			freeb(bp);
			return;
		}
	}

	/*
	 *  take care of packets that were padded up
	 */
	mp->rem -= BLEN(bp);
	if(mp->rem < 0){
		if(-mp->rem <= BLEN(bp)){
			bp->wptr += mp->rem;
			mp->rem = 0;
		} else
			panic("rcvmsg: short packet");
	}
	putb(mp, bp);

	/*
	 *  if the last chunk - pass it up the stream and wake any
	 *  waiting process.
	 *
	 *  if not, strip off the delimiter.
	 */
	if(mp->rem == 0){
		rcvack(cp, h->ack);
		if(f & ACKME)
			queueack(cp, h->mid);
		mp->last->flags |= S_DELIM;
		PUTNEXT(q, mp->first);
		mp->first = mp->last = 0;
		mp->len = 0;
		cp->rcvd++;

		/*
		 *  cycle bufffer to next expected mid
		 */
		mp->mid ^= Nmsg;

		/*
		 *  stop xmitting the NEWCALL flag
		 */
		if(cp->state==Cconnecting && !(f & NEWCALL))
			cp->state = Cconnected;
	} else
		mp->last->flags &= ~S_DELIM;

}

/*
 *  the device stream module definition
 */
static void nonetstopen(Queue *, Stream *);
static void nonetstclose(Queue *);
static void nonetoput(Queue *, Block *);
static void nonetiput(Queue *, Block *);
Qinfo nonetinfo = { nonetiput, nonetoput, nonetstopen, nonetstclose, "nonet" } ;

/*
 *  store the device end of the stream so that the multiplexor can
 *  send blocks upstream.  this is called by streamopen() when a
 *  nonet device steam is created.
 */
static void
nonetstopen(Queue *q, Stream *s)
{
	Interface *ifc;
	Conversation *cp;

	ifc = &interface[s->dev];
	cp = &ifc->conv[s->id];
	cp->ifc = ifc;
	cp->rq = RD(q);
	cp->state = Copen;
	RD(q)->ptr = WR(q)->ptr = (void *)cp;
}

/*
 *  wait until all output has drained or a hangup is received.
 *  then send a hangup message (until one is received).
 */
static int
isflushed(void *a)
{
	Conversation *cp;

	cp = (Conversation *)a;
	return cp->first == cp->next || cp->state == Chungup;
}
static int
ishungup(void *a)
{
	Conversation *cp;

	cp = (Conversation *)a;
	return cp->state == Chungup;
}
static int
isdead(void *a)
{
	Conversation *cp;

	cp = (Conversation *)a;
	return cp->kstarted == 0;
}
static void
nonetstclose(Queue *q)
{
	Conversation *cp;
	Msg *mp;
	int i;

	cp = (Conversation *)q->ptr;

	/*
	 *  wait for all messages to drain
	 */
	while(!isflushed(cp))
		sleep(&cp->r, isflushed, cp);

	/*
	 *  ack all outstanding messages
	 */
	for(; cp->first != cp->next; cp->first = (cp->first+1)&Nmask) {
		mp = &cp->out[cp->first];
		if(!mp->acked)
			rcvack(cp, mp->mid);
	}
	cp->first = cp->next;

	/*
	 *  send hangup messages to the other side
	 *  until it hangs up.
	 */
	if(cp->state >= Cconnected){
		sendctlmsg(cp, HANGUP, 1);
		for(i=0; i<10 && !ishungup(cp); i++){
			sendctlmsg(cp, HANGUP, 1);
			tsleep(&cp->r, ishungup, cp, MSrexmit);
		}
	}

	/*
	 *  kill off the nonetkproc
	 */
	cp->state = Cclosed;
	wakeup(&cp->rq->r);
	sleep(&cp->r, isdead, cp);

	/*
	 *  close down, synchronizing with interface
	 */
	qlock(cp);
	cp->ifc = 0;
	qunlock(cp);
}

/*
 *  send all messages up stream.  this should only be control messages
 */
static void
nonetiput(Queue *q, Block *bp)
{
	Conversation *cp;

	if(bp->type == M_HANGUP){
		cp = (Conversation *)q->ptr;
		cp->state = Chungup;
	}
	PUTNEXT(q, bp);
}

/*
 *  queue a block
 */
static int
windowopen(void *a)
{
	Conversation *cp;

	cp = (Conversation *)a;
	return (cp->next + 1)&Nmask != cp->first;	
}
static void
nonetoput(Queue *q, Block *bp)
{
	Conversation *cp;
	int next;
	Msg *mp;

	cp = (Conversation *)(q->ptr);

	/*
	 *  do all control functions
	 */
	if(bp->type != M_DATA){
		if(streamparse("connect", bp))
			connect(cp, bp);
		else if(streamparse("listen", bp))
			listen(cp, bp);
		else
			freeb(bp);
		return;
	}

	/*
	 *  collect till we see a delim
	 */
	if(!putb(q, bp))
		return;

	/*
	 *  block if we don't have any free mid's
	 */
	while((next = (cp->next + 1)&Nmask) == cp->first)
		sleep(&cp->r, windowopen, cp);

	/*
	 *  stick the message in a Msg structure
	 */
	mp = &cp->out[cp->next];
	mp->time = NOW + MSrexmit;
	mp->first = q->first;
	mp->last = q->last;
	mp->len = q->len;
	mp->mid ^= Nmsg;
	mp->acked = 0;

	/*
	 *  init the queue for new messages
	 */
	q->len = 0;
	q->first = q->last = 0;

	/*
	 *  send it
	 */
	cp->next = next;
	sendmsg(cp, mp);
	cp->sent++;
}

/*
 *  wake up every 250 ms to send and ack or resend a message
 */
static int
always(void *a)
{
	return 0;
}
void
nonetkproc(void *arg)
{
	Conversation *cp;
	Interface *ifc;
	Queue *wq;
	Msg *mp;
	int i;

	cp = (Conversation *)arg;

	for(;;){
		/*
		 *  die on request
		 */
		if(cp->state == Cclosed){
			cp->kstarted = 0;
			wakeup(&cp->r);
			return;
		}

		/*
		 *  retransmit first message
		 */
		if(cp->first != cp->next){
			mp = &cp->out[cp->first];
			if(!mp->acked && NOW >= mp->time){
				if(cp->retry++ > 400)
					hangup(cp);
				else {
					cp->rexmit++;
					sendmsg(cp, mp);
				}
			}
		}

		/*
		 *  send any ack whose time is come
		 */
		while(cp->afirst != cp->anext && NOW >= cp->atime[cp->anext]
		      && cp->rq->next->len < Streamhi)
			sendctlmsg(cp, 0, 0);
		tsleep(&cp->rq->r, always, 0, MSrexmit/2);
	}
}

/*
 *  nonet directory and subdirectory
 */
enum {
	Nraddrqid,
	Nstatsqid,
	Nchanqid,
	Ncloneqid,
};
Dirtab nonetdir[Ndir];
Dirtab nosubdir[]={
	"raddr",	Nraddrqid,	0,	0600,
	"stats",	Nstatsqid,	0,	0600,
};

/*
 *  nonet file system.  most of the calls use dev.c to access the nonet
 *  directory and stream.c to access the nonet devices.
 */
void
nonetreset(void)
{
	newqinfo(&noetherinfo);
}

/*
 *  create the nonet directory.  the files are `clone' and stream
 * directories '1' to '32' (or whatever Nconv is in decimal)
 */
void
nonetinit(void)
{
	int i;

	/*
	 *  create the directory.
	 */
	/*
	 *  the circuits
	 */
	for(i = 0; i < Nconv; i++) {
		sprint(nonetdir[i].name, "%d", i);
		nonetdir[i].qid = CHDIR|STREAMQID(i, Nchanqid);
		nonetdir[i].length = 0;
		nonetdir[i].perm = 0600;
	}

	/*
	 *  the clone device
	 */
	strcpy(nonetdir[i].name, "clone");
	nonetdir[i].qid = Ncloneqid;
	nonetdir[i].length = 0;
	nonetdir[i].perm = 0600;

}

Chan*
nonetattach(char *spec)
{
	Interface *ifc;
	Chan *c;

	/*
	 *  find an interface with the same name
	 */
	for(ifc = interface; ifc < &interface[Nifc]; ifc++){
		qlock(ifc);
		if(strcmp(spec, ifc->name)==0 && ifc->wq) {
			ifc->ref++;
			qunlock(ifc);
			break;
		}
		qunlock(ifc);
	}
	if(ifc == &interface[Nifc])
		error(0, Enoifc);
	c = devattach('n', spec);
	c->dev = 0;
	return c;
}

Chan*
nonetclone(Chan *c, Chan *nc)
{
	Interface *ifc;

	c = devclone(c, nc);
	ifc = &interface[c->dev];
	qlock(ifc);
	ifc->ref++;
	qunlock(ifc);
	return c;
}

int	 
nonetwalk(Chan *c, char *name)
{
	if(c->qid == CHDIR)
		return devwalk(c, name, nonetdir, Ndir, devgen);
	else
		return devwalk(c, name, nosubdir, Nsubdir, streamgen);
}

void	 
nonetstat(Chan *c, char *dp)
{
	if(c->qid == CHDIR)
		devstat(c, dp, nonetdir, Ndir, devgen);
	else
		devstat(c, dp, nosubdir, Nsubdir, streamgen);
}

/*
 *  opening a nonet device allocates a Conversation.  Opening the `clone'
 *  device is a ``macro'' for finding a free Conversation and opening
 *  it's ctl file.
 */
Chan*
nonetopen(Chan *c, int omode)
{
	extern Qinfo nonetinfo;
	Stream *s;
	Conversation *cp;
	Interface *ifc;

	if(c->qid == Ncloneqid){
		ifc = &interface[c->dev];
		for(cp = &ifc->conv[0]; cp < &ifc->conv[Nconv]; cp++){
			if(cp->state == Cclosed && canqlock(cp)){
				if(cp->state != Cclosed){
					qunlock(cp);
					continue;
				}
				c->qid = CHDIR|STREAMQID(cp-ifc->conv, Nchanqid);
				devwalk(c, "ctl", 0, 0, streamgen);
				streamopen(c, &nonetinfo);
				qunlock(cp);
				break;
			}
		}
		if(cp == &ifc->conv[Nconv])
			error(0, Enodev);
	} else if(c->qid != CHDIR)
		streamopen(c, &nonetinfo);

	c->mode = openmode(omode);
	c->flag |= COPEN;
	c->offset = 0;
	return c;
}

void	 
nonetcreate(Chan *c, char *name, int omode, ulong perm)
{
	error(0, Eperm);
}

void	 
nonetclose(Chan *c)
{
	Interface *ifc;

	/* real closing happens in lancestclose */
	if(c->qid != CHDIR)
		streamclose(c);

	ifc = &interface[c->dev];
	qlock(ifc);
	ifc->ref--;
	qunlock(ifc);
}

long	 
nonetread(Chan *c, void *a, long n)
{
	int t;
	Conversation *cp;
	char stats[256];

	t = STREAMTYPE(c->qid);
	if(t >= Slowqid)
		return streamread(c, a, n);

	if(c->qid == CHDIR)
		return devdirread(c, a, n, nonetdir, Ndir, devgen);
	if(c->qid & CHDIR)
		return devdirread(c, a, n, nosubdir, Nsubdir, streamgen);

	cp = &interface[c->dev].conv[STREAMID(c->qid)];
	switch(t){
	case Nraddrqid:
		return stringread(c, a, n, cp->raddr);
	case Nstatsqid:
		sprint(stats, "sent: %d\nrcved: %d\nrexmit: %d\nbad: %d\n",
			cp->sent, cp->rcvd, cp->rexmit, cp->bad);
		return stringread(c, a, n, stats);
	}
	error(0, Eperm);
}

long	 
nonetwrite(Chan *c, void *a, long n)
{
	int t;

	t = STREAMTYPE(c->qid);
	if(t >= Slowqid)
		return streamwrite(c, a, n, 0);
	
	error(0, Eperm);
}

void	 
nonetremove(Chan *c)
{
	error(0, Eperm);
}

void	 
nonetwstat(Chan *c, char *dp)
{
	error(0, Eperm);
}

void	 
noneterrstr(Error *e, char *buf)
{
	rooterrstr(e, buf);
}

void	 
nonetuserstr(Error *e, char *buf)
{
	extern consuserstr(Error *, char *);

	consuserstr(e, buf);
}

/*
 *  interface
 */
/*
 *  Create an interface.  The qlock on ifc prevents a circuit
 *  from connecting to (nonetconnect) or outputting on (nonetoput)
 *  the interface while it is being created.
 */
Interface *
newifc(Queue *q, Stream *s, int maxtu, int mintu, int hsize,
	void (*connect)(Conversation *, char *))
{
	Interface *ifc;

	for(ifc = interface; ifc < &interface[Nifc]; ifc++){
		if(ifc->wq == 0){
			qlock(ifc);
			if(ifc->wq) {
				/* someone was faster than us */
				qunlock(ifc);
				continue;
			}
			RD(q)->ptr = WR(q)->ptr = (void *)ifc;
			ifc->maxtu = maxtu - hsize - HDRSIZE;
			ifc->mintu = mintu - hsize - HDRSIZE;
			ifc->hsize = hsize;
			ifc->connect = connect;
			ifc->wq = WR(q);
			ifc->name[0] = 0;
			qunlock(ifc);
			return ifc;
		}
	}
	error(0, Enoifc);
}

/*
 *  Free an interface.
 */
void
freeifc(Interface *ifc)
{
	qlock(ifc);
	if(ifc->ref){
		qunlock(ifc);
		print("freeifc in use\n");
		error(0, Einuse);
	}
	ifc->wq = 0;
	qunlock(ifc);
}
/*
 *  ethernet specific multiplexor
 */
/*
 *  ethernet header of a packet
 */
struct Etherhdr {
	uchar	d[6];
	uchar	s[6];
	uchar	type[2];
	uchar	circuit[3];	/* circuit number */
	uchar	flag;
	uchar	mid;		/* message id */
	uchar	ack;		/* piggy back ack */
	uchar	remain[2];	/* count of remaing bytes of data */
	uchar	sum[2];		/* checksum (0 means none) */
};
#define EHDRSIZE 24
#define EMAXBODY	(1514-HDRSIZE)	/* maximum ethernet packet body */
#define ETHER_TYPE	0x900	/* most significant byte last */

/*
 *  the ethernet multiplexor stream module definition
 */
static void noetheropen(Queue *, Stream *);
static void noetherclose(Queue *);
static void noetheroput(Queue *, Block *);
static void noetheriput(Queue *, Block *);
Qinfo noetherinfo = { noetheriput, nullput, noetheropen, noetherclose, "noether" };

/*
 *  parse an ethernet address (assumed to be 12 ascii hex digits)
 */
void
etherparse(uchar *to, char *from)
{
	int tdig;
	int fdig;
	int i;

	if(strlen(from) != 12)
		error(0, Ebadnet);

	for(i = 0; i < 6; i++){
		fdig = *from++;
		tdig = fdig > 'a' ? (fdig - 'a' + 10)
				: (fdig > 'A' ? (fdig - 'A' + 10) : (fdig - '0'));
		fdig = *from++;
		tdig <<= 4;
		tdig |= fdig > 'a' ? (fdig - 'a' + 10)
				: (fdig > 'A' ? (fdig - 'A' + 10) : (fdig - '0'));
		*to++ = tdig;
	}
}

/*
 *  perfrorm the ether specific part of nonetconnect.  just stick
 *  the address into the prototype header.
 */
void
noetherconnect(Conversation *cp, char *ea)
{
	Etherhdr *eh;

	/*
	 *  special hack for ross
	 */
	if(strcmp(ea, "020701005eff")==0)
		cp->hdr->flag &= ~ACKME;

	eh = (Etherhdr *)cp->media->rptr;
	etherparse(eh->d, ea);
	eh->type[0] = ETHER_TYPE>>8;
	eh->type[1] = ETHER_TYPE;
}

/*
 *  set up an ether interface
 */
static void
noetheropen(Queue *q, Stream *s)
{
	newifc(q, s, 1514, 60, 14, noetherconnect);
}

/*
 *  tear down an ether interface
 */
static void
noetherclose(Queue *q)
{
	Interface *ifc;

	ifc = (Interface *)(q->ptr);
	freeifc(ifc);
}

/*
 *  Input a packet and use the ether address to select the correct
 *  nonet device to pass it to.
 *
 *  Simplifying assumption:  one put == one packet && the complete header
 *	is in the first block.  If this isn't true, demultiplexing will not work.
 */
static void
noetheriput(Queue *q, Block *bp)
{
	Interface *ifc;
	int circuit;
	Conversation *cp;
	Etherhdr *h;
	Etherhdr *ph;
	ulong s;
	Block *end;

	if(bp->type != M_DATA){
		PUTNEXT(q, bp);
		return;
	}

	ifc = (Interface *)(q->ptr);
	h = (Etherhdr *)(bp->rptr);
	circuit = (h->circuit[2]<<16) | (h->circuit[1]<<8) | h->circuit[0];
	s = (h->sum[1]<<8) | h->sum[0];
	if(s && s != cksum(bp, 14)){
		print("checksum error %ux %ux\n", s, (h->sum[1]<<8) | h->sum[0]);
		freeb(bp);
		return;
	}

	/*
	 *  look for an existing circuit.
	 */
	for(cp = &ifc->conv[0]; cp < &ifc->conv[Nconv]; cp++){
		if(cp->state > Clistening && circuit == cp->rcvcircuit && canqlock(cp)){
			ph = (Etherhdr *)(cp->media->rptr);
			if(ifc == cp->ifc
			&& circuit == cp->rcvcircuit
			&& cp->state > Clistening
			&& memcmp(ph->d, h->s, sizeof(h->s)) == 0){
				cp->hdr->flag &= ~NEWCALL;
				bp->rptr += ifc->hsize;
				rcvmsg(cp, bp);
				qunlock(cp);
				return;
			}
			qunlock(cp);
		}
	}

	/*
	 *  ... or one just listening
	 */
	if((h->flag & NEWCALL) == 0) {
		freeb(bp);
		return;
	}
	for(cp = &ifc->conv[0]; cp < &ifc->conv[Nconv]; cp++){
		if(cp->state == Clistening && canqlock(cp)) {
			/*
			 *  initialize the conversation
			 */
			startconv(cp, circuit^1, Cconnecting);
			wakeup(&cp->r);
			sprint(cp->raddr, "%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux", h->s[0],
				h->s[1], h->s[2], h->s[3], h->s[4], h->s[5]);

			/*
			 *  fill in media dependent prototype header
			 */
			ph = (Etherhdr *)(cp->media->rptr);
			memcpy(ph->d, h->s, sizeof(h->s));
			ph->type[0] = ETHER_TYPE>>8;
			ph->type[1] = ETHER_TYPE;

			/*
			 *  pass on the packet
			 */
			bp->rptr += ifc->hsize;
			rcvmsg(cp, bp);
			qunlock(cp);
			return;
		}
	}

	/*
	 *  not found 
	 */
	freeb(bp);
}

/*
 *  calculate the checksum of a list of blocks.  ignore the first `offset' bytes.
 */
int
cksum(Block *bp, int offset)
{
	Block *nbp = bp;
	uchar *ep, *p;
	int n;
	ulong s;
	Hdr *hp;

	s = 0;
	p = bp->rptr + offset;
	n = bp->wptr - p;
	hp = (Hdr *)p;
	hp->sum[0] = hp->sum[1] = 0;
	for(;;){
		ep = p+(n&~0x7);
		while(p < ep) {
			s = s + s + p[0];
			s = s + s + p[1];
			s = s + s + p[2];
			s = s + s + p[3];
			s = s + s + p[4];
			s = s + s + p[5];
			s = s + s + p[6];
			s = s + s + p[7];
			s = (s&0xffff) + (s>>16);
			p += 8;
		}
		ep = p+(n&0x7);
		while(p < ep) {
			s = s + s + *p;
			p++;
		}
		s = (s&0xffff) + (s>>16);
		bp = bp->next;
		if(bp == 0)
			break;
		p = bp->rptr;
		n = BLEN(bp);
	}
	s = (s&0xffff) + (s>>16);
	hp->sum[1] = s>>8;
	hp->sum[0] = s;
	return s & 0xffff;
}
