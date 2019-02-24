#include <sys/param.h>
#include <sys/enio.h>
#include <sys/filio.h>
#include <errno.h>
#include <ipc.h>
#include <signal.h>

typedef unsigned char uchar;

/*
 *  one structure per fd to decide what to do with
 *  input on the fd.
 */
typedef struct {
	void (*func)();		/* function to call */
	void *ptr;
} Fdesc;
Fdesc f[NOFILE];

/*
 *  bit mask of fds to listen on
 */
fd_set master;
int etherfd;

/*
 *  name to address translation
 */
typedef struct Address {
	char *name;
	uchar addr[6];
	int ack;
} Address;
Address addr[] = {
	{ "ross",	0x02, 0x07, 0x01, 0x00, 0x5e, 0xff,	0 },
	{ "research",	0x02, 0x07, 0x01, 0x00, 0x62, 0x8a,	1 },
	{ "ara",	0x08, 0x00, 0x47, 0x00, 0x09, 0xfa,	1 },
	{ "shamash",	0x08, 0x00, 0x20, 0x01, 0x37, 0x78,	1 },
	{ "ra",		0x08, 0x00, 0x20, 0x01, 0x34, 0x13,	1 },
	{ "aten",	0x08, 0x00, 0x20, 0x01, 0x3b, 0x74,	1 },
	{ "pipe",	0x08, 0x00, 0x2b, 0x09, 0x57, 0x94,	1 },
	{ "sol",	0x08, 0x00, 0x20, 0x01, 0x64, 0xab,	1 },
	{ "bowell",	0x08, 0x00, 0x2b, 0x0b, 0xce, 0xda,	1 },
	{ "pyxis",	0x08, 0x00, 0x69, 0x02, 0x01, 0x4e,	1 },
	{ "r70", 	0x08, 0x00, 0x2b, 0x04, 0x26, 0x5d,	1 },
	{ "nun",	0x08, 0x00, 0x20, 0x00, 0x24, 0xc4,	1 },
	{ "wild",	0x02, 0x07, 0x01, 0x00, 0x4e, 0x05,	1 },
	{ "coma",	0x08, 0x00, 0x2b, 0x0b, 0x1d, 0x9b,	1 },
	{ "tempel",	0x02, 0xcf, 0x1f, 0x11, 0x73, 0x13,	1 },
	{ "bootes",	0x08, 0x00, 0x69, 0x02, 0x02, 0x05,	0 },
	{ "helix",	0x08, 0x00, 0x69, 0x02, 0x04, 0x27,	1 },
	{ "spindle",	0x08, 0x00, 0x69, 0x02, 0x01, 0x9e,	1 },
	{ "hr4784",	0x00, 0x00, 0xa7, 0x00, 0x08, 0x28,	1 },
	{ 0 }
};

/*
 *  ethernet header
 */
typedef struct Etherhdr {
	uchar	d[6];
	uchar	s[6];
	uchar	type[2];
	uchar	circuit[3];	/* circuit number */
	uchar	flag;
	uchar	mid;		/* message id */
	uchar	ack;		/* piggy back ack */
	uchar	remain[2];	/* count of remaing bytes of data */
	uchar	sum[2];		/* checksum (0 means none) */
} Etherhdr;

/*
 *  ethernet packet
 */
typedef struct Ethermsg {
	uchar	d[6];
	uchar	s[6];
	uchar	type[2];
	uchar	circuit[3];	/* circuit number */
	uchar	flag;
	uchar	mid;		/* message id */
	uchar	ack;		/* piggy back ack */
	uchar	remain[2];	/* count of remaing bytes of data */
	uchar	sum[2];		/* checksum (0 means none) */
	uchar	data[1500 - 10];
} Ethermsg;
#define HDRSIZE 24
#define NEWCALL	0x1		/* flag bit marking a new circuit */
#define HANGUP 0x2		/* flag bit requesting hangup */
#define ACKME 0x4		/* acknowledge this message */
#define ETHER_TYPE 0x9		/* most significant byte last */

char *dev[] = {
	"/dev/qe02",
	"/dev/il02",
	"/dev/bna02",
	0,
};

/*
 *  nonet conversations
 */
#define NCIRC (NOFILE/2)
#define MAXMSG (8*1024+100)
typedef struct Circuit {
	Fdesc	*f;
	int	hangingup;
	int	hungup;
	int	version;

	uchar	rcved;		/* last msg rcved */
	int 	mid;		/* id of current input message */
	int 	rem;		/* remaining bytes for input current message */
	int	rcvcircuit;	/* circuit number of incoming packets */
	uchar	ibuf[MAXMSG];
	int	ibi;
	int	eofs;

	uchar	sent;		/* last msg sent */
	uchar	ackrcved;	/* last ack received */
	int	resent;		/* number of times a message is transmitted */
	Etherhdr eh;		/* prototype ethernet header */
	uchar	obuf[MAXMSG];
	int	obi;
} Circuit;
Circuit circ[NCIRC];

void announce();
void openether();
void newcall();
void fromclient();
void fromether();
void timeout();
void fillhdr();
void send();
void hangup();
void ack();
void resend();
void ackdead();

int debug;

main(ac, av)
	int ac;
	char *av[];
{
	fd_set set;
	int fds;
	int i;

	debug = 0;
	if(ac > 1 && strcmp(av[1], "-d")==0)
		debug = 1;
	chdir("/cs");
	FD_ZERO(master);
	if(!debug)
		detach("nonet");
	announce();
	openether();
	signal(SIGPIPE, SIG_IGN);
	for(;;){
		/*
		 *  keep polling the fds for input.  call an
		 *  fd's input routine whenever it has any input.
		 */
		set = master;
		switch(fds = select(NOFILE, &set, 0, 1000)){
		case -1:
			logevent("switch returned -1");
			exit(1);
		case 0:
			timeout();
			continue;
		}
		for(i = 0; fds && i < NOFILE; i++){
			if(FD_ISSET(i, set)) {
				fds--;
				if(f[i].func != 0)
					(*f[i].func)(i);
				else {
					logevent("io on bad fd %d\n", i);
					FD_CLR(i, master);
					close(i);
				}
			}
		}
	}
}

/*
 *  set up a connected pair of fd's to listen on.
 *  if fd2 < 0, then there is only 1 fd.
 */
newf(fd, func)
	int fd;
	void (*func)();
{
	f[fd].func = func;
	FD_SET(fd, master);
}

/*
 *  close an fd, stop listening to it, and close off it's related fd.
 */
delf(fd)
	int fd;
{
	if(fd < 0)
		return;
	if(f[fd].func == 0)
		return;
	f[fd].func = 0;
	close(fd);
	FD_CLR(fd, master);
}

/*
 *  plug into the name space
 */
void
announce()
{
	int fd;

	fd = ipccreat("/cs/nonet", "");
	if(fd<0){
		logconsole("error announcing /cs/nonet\n");
		exit(1);
	}
	newf(fd, newcall);
	logconsole("/cs/nonet announced\n");
}

/*
 *  listen to the nonet ether type
 */
void
openether()
{
	int fd;
	int i;
	extern int buf_ld;

	for(i = 0; dev[i]; i++){
		fd = open(dev[i], 2);
		if(fd >= 0)
			break;
	}
	if(fd < 0){
		logevent("opening ethernet device\n");
		exit(1);
	}
	i = ETHER_TYPE;
	if(ioctl(fd, ENIOTYPE, &i) < 0){
		perror("ENIOTYPE");
		exit(1);
	}
	newf(fd, fromether);
	etherfd = fd;
	if(ioctl(etherfd, FIOPUSHLD, &buf_ld)<0){
		perror("pushing buf_ld");
	}
	if(ioctl(etherfd, FIOPUSHLD, &buf_ld)<0){
		perror("pushing buf_ld");
	}
}

/*
 *  there is input on the announcement channel.  see if a
 *  new call has come in.
 */
void
newcall(fd)
	int fd;
{
	ipcinfo *ipc;
	Circuit *cp;
	Address *a;
	int circuit;
	int nfd;
	char *service;
	char *strchr();

	logevent("newcall\n");
	ipc = ipclisten(fd);
	if(ipc == 0){
		logevent("listening %s\n", errstr);
		return;
	}

	/*
	 *  Find a new circuit for the call.
	 */
	for(cp = circ; cp < &circ[NCIRC]; cp++){
		if(cp->f == 0)
			break;
	}
	if(cp == &circ[NCIRC]){
		logevent("call from %s!%s busy\n", ipc->machine, ipc->user);
		ipcreject(ipc, EBUSY, "out of output channels");
		return;
	}
	logevent("call from %s!%s on %d\n", ipc->machine, ipc->user, cp - circ);

	/*
	 * separate name and service
	 */
	service = strchr(ipc->name, '.');
	if(service)
		*service++ = 0;
	else{
		service = strchr(ipc->name, '!');
		if(service)
			*service++ = 0;
	}
	if(service==0)
		service = "";

	/*
	 *  see if we know the destination
	 */
	for(a = addr; a->name; a++){
		if(strcmp(a->name, ipc->name) == 0)
			break;
	}
	if(a->name == 0){
		logevent("call from %s!%s, %s!%s unknown dest\n", ipc->machine, ipc->user,
			ipc->name, service);
		ipcreject(ipc, EEXIST, "unknown destination");
		return;
	}
	nfd = ipcaccept(ipc);
	if(nfd < 0){
		logevent("call from %s!%s error accepting\n", ipc->machine, ipc->user);
		return;
	}
	logevent("call from %s!%s to %s!%s accepted\n", ipc->machine, ipc->user, ipc->name,
		service);

	/*
	 *  init the circuit
	 */
	f[nfd].ptr = (void *)cp;
	cp->f = &f[nfd];
	cp->hangingup = 0;
	cp->hungup = 0;
	cp->eofs = 0;
	cp->ackrcved = 0;
	cp->sent = 0;
	cp->ibi = 0;
	cp->obi = 0;
	cp->rcved = 0;
	cp->mid = 0;
	circuit = 2*((cp - circ)+cp->version*NCIRC);
	cp->rcvcircuit = circuit+1;
	cp->version++;
	if(a->ack)
		cp->eh.flag = NEWCALL|ACKME;
	else
		cp->eh.flag = NEWCALL;
	cp->eh.circuit[2] = circuit>>16;
	cp->eh.circuit[1] = circuit>>8;
	cp->eh.circuit[0] = circuit;
	memcpy(cp->eh.d, a->addr, sizeof(a->addr));
	cp->eh.type[1] = ETHER_TYPE>>8;
	cp->eh.type[0] = ETHER_TYPE;
	newf(nfd, fromclient);

	if(strcmp(service, "bootfs") == 0){
		/*
		 *  send ok to the caller
		 */
		write(nfd, "OK", 2);
	} else	if(strcmp(service, "fs") != 0){
		/*
		 *  send user id
		 */
		sprint(cp->obuf, "%s %s", ipc->user, service);
		cp->obi += strlen(cp->obuf);
		send(cp);
	}
}

/*
 *  input from the ethernet
 */
void
fromether(fd)
	int fd;
{
	int len;
	short r;
	int c;
	Circuit *cp;
	Ethermsg em;
	int hungup;

	/*
 	 *  read/parse an ethernet message
	 */
	len = read(fd, &em, sizeof(Ethermsg));
	if(len <= 0){
		logevent("error reading from ether\n");
		return;
	}
	if(debug)
		printpacket("in", &em);
	r = (em.remain[1]<<8) | em.remain[0];
	c = (em.circuit[2]<<16) | (em.circuit[1]<<8) | em.circuit[0];
	hungup = em.flag & HANGUP;
	
	/*
	 *  look for an existing client
	 */
	for(cp = circ; cp < &circ[NCIRC]; cp++){
		if(cp->f && c == cp->rcvcircuit
		&& memcmp(cp->eh.d, em.s, sizeof(em.s)) == 0)
			break;
	}
	if(cp == &circ[NCIRC]) {
		logevent("no client found\n");
		ackdead(&em);
		return;
	}

	len -= HDRSIZE;
	if(r>=0){
		/*
		 *  a new message, forget any old messages
		 */
		cp->ibi = 0;
		cp->mid = em.mid;
		cp->rem = r;
	} else {
		/*
		 *  a continuation
		 */
		if(cp->mid != em.mid || -r != cp->rem) {
			/* bad fragment -- drop anything ibuffered */
			logevent("cp->mid %d em.mid %d r %d cp->rem %d\n", cp->mid, em.mid,
				r, cp->rem);
			cp->ibi = 0;
			return;
		}
	}
	cp->rem -= len;

	/*
	 *  last packet may be ``padded out''.  adjust size if it is.
	 */
	if(cp->rem < 0){
		if(-cp->rem <= len){
			len += cp->rem;
			cp->rem = 0;
		} else {
			logevent("short packet\n");
			return;
		}
	}

	/*
	 *  copy the packet into the buffer
	 */
	memcpy(&(cp->ibuf[cp->ibi]), em.data, len);
	cp->ibi += len;

	/*
	 *  if last packet of a message...
	 */
	if(cp->rem == 0){
		/*
		 * ... is it what we expected?
		 */
		if(((cp->rcved+1)&0xff) != cp->mid){
			if(cp->rcved == cp->mid){
				cp->ackrcved = em.ack;
				if(cp->ibi)
					ack(cp);		/* retransmission */
				fd = cp->f - f;
				if(cp->ackrcved == cp->sent && f[fd].func)
					FD_SET(fd, master);
				return;
			}
			logevent("expected %d got %d\n", (cp->rcved+1)&0xff,  cp->mid);
			cp->ibi = 0;
			return;
		}
		/*
		 *  ... update state and send packet to client
		 */
		cp->eh.flag &= ~NEWCALL;
		cp->rcved = cp->mid;
		cp->ackrcved = em.ack;
		fd = cp->f - f;
		if(cp->ackrcved == cp->sent && f[fd].func)
			FD_SET(fd, master);
		if(cp->ibi && write(fd, cp->ibuf, cp->ibi) != cp->ibi){
			logevent("error writing to client %d %d\n", fd, cp->ibi);
			hangup(cp);
		}
		if(hungup){
			cp->hungup = 1;
			hangup(cp);
		}
		if(cp->eh.flag & ACKME)
			ack(cp);
	}
}

/*
 *  collect a message from a client.  4 eofs in a row mean the connection
 *  is closed.
 */
void
fromclient(fd)
	int fd;
{
	int len;
	Circuit *cp;

	cp = (Circuit *)(f[fd].ptr);

	/*
	 *  read in the message
	 */
	len = read(fd, cp->obuf, sizeof(cp->obuf));
	if(len < 0){
		hangup(cp);
		return;
	}
	if(len == 0){
		if(++(cp->eofs) == 4){
			hangup(cp);
			return;
		}
	} else
		cp->eofs = 0;
	cp->obi = len;

	/*
	 *  send message
	 */
	send(cp);
}

/*
 *  send a message on a circuit
 */
void
send(cp)
	Circuit *cp;
{
	cp->sent++;
	cp->resent = 0;
	FD_CLR(cp->f - f, master);
	resend(cp);
}

/*
 *  resend a message on a circuit
 */
void
resend(cp)
	Circuit *cp;
{
	Ethermsg em;
	int rem, len, first;

	/*
	 *  break up the message into some number of
	 *  ethernet packets and send them.
	 */
	first = 1;
	for (rem = cp->obi; first || rem > 0; rem -= len){
		first = 0;
		fillhdr(cp, &em, rem == cp->obi ? rem : -rem);
		len = rem > sizeof(em.data) ? sizeof(em.data) : rem;
		memcpy(em.data, &(cp->obuf[cp->obi - rem]), len);
		if(len < 128-HDRSIZE)
			len = 128-HDRSIZE;
		if(debug)
			printpacket("out", &em);
		if(write(etherfd, &em, len + HDRSIZE) != len + HDRSIZE){
			logevent("error writing to ether\n");
			return;
		}
	}
}

/*
 *  ack a dead client
 */
void
ackdead(e)
	Ethermsg *e;
{
	Ethermsg em;

	memcpy(em.d, e->s, sizeof(em.d));
	memcpy(em.s, e->d, sizeof(em.s));
	em.mid = e->ack;
	em.ack = e->mid;
	em.flag = HANGUP;
	e->remain[1] = 0;
	e->remain[0] = 0;
	e->sum[0] = e->sum[1] = 0;

	if(write(etherfd, &em, HDRSIZE) != HDRSIZE){
		logevent("error writing to ether\n");
		return;
	}
}

/*
 *  fill in an ethernet/nonet header
 */
void
fillhdr(cp, e, rem)
	Circuit *cp;
	Ethermsg *e;
	int rem;
{
	memcpy(e, &cp->eh, HDRSIZE);
	e->mid = cp->sent;
	e->ack = cp->rcved;
	e->remain[1] = rem>>8;
	e->remain[0] = rem;
	e->sum[0] = e->sum[1] = 0;
}

/*
 *  retransmit any unacknowledged messages
 */
void
timeout(sig)
	int sig;
{
	Circuit *cp;

	for(cp = circ; cp < &circ[NCIRC]; cp++){
		if(cp->f && cp->sent != cp->ackrcved){
			if(++(cp->resent) > 15) {
				logevent("too many rexmits, giving up %d\n", cp - circ);
				delf(cp->f - f);
				cp->f = 0;
			} else
				resend(cp);
		}
		if(cp->f && cp->hungup && cp->hangingup && cp->sent == cp->ackrcved)
			cp->f = 0;
	}
}

/*
 *  hangup a connection
 */
void
hangup(cp)
	Circuit *cp;
{
	Ethermsg em;

	logevent("hanging up %d\n", cp - circ);
	if(cp->hangingup)
		return;
	cp->hangingup = 1;
	cp->eh.flag |= HANGUP;
	cp->obi = 0;
	delf(cp->f - f);
	send(cp);
	cp->f = 0;
}

/*
 *  ack a message
 */
void
ack(cp)
	Circuit *cp;
{
	Ethermsg em;

	fillhdr(cp, &em, 0);
	if(write(etherfd, &em, HDRSIZE) != HDRSIZE){
		logevent("error writing to ether\n");
		return;
	}
}

printpacket(tag, p)
	char *tag;
	Ethermsg *p;
{
	uchar *cp;
	cp = p->circuit;
  fprint(2, "%s: d(%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux)s(%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux)t(%ux %ux)d(%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux%.2ux)\n",
	tag,
	p->d[0], p->d[1], p->d[2], p->d[3], p->d[4], p->d[5],
	p->s[0], p->s[1], p->s[2], p->s[3], p->s[4], p->s[5], p->type[0], p->type[1],
	cp[0], cp[1], cp[2], cp[3], cp[4], cp[5],
	cp[6], cp[7], cp[8], cp[9], cp[10], cp[11]);
}

