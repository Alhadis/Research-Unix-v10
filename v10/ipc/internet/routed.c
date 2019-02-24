#include <sys/param.h>
#include <fio.h>
#include <signal.h>
#include <errno.h>
#include <sys/filio.h>
#include <sys/inet/in.h>
#include <sys/inet/ip_var.h>
#include <sys/inet/udp_user.h>
#include "config.h"

#define IPDEVICE "/dev/ip16"	/* device for getting ip info from */
#define RPORT 520

int udpfd, ipfd, verbose, trace, quiet, traceinput, hopoffset;
extern errno;
extern	int buf_ld;
extern char *in_ntoa();
extern u_short cksum();

struct udppacket {
	struct udpaddr addr;
	char buf[4096];
};

main(argc, argv)
char *argv[];
{
	int n, i;
	struct udppacket packet;
	char *cp;

	for(i = 1; i < argc; i++){
		if(argv[i][0] == '-'){
			for(cp = argv[i]+1; *cp; cp++){
				switch(*cp){
				case 'v':
					verbose++;
					break;
				case 'q':
					quiet++;
					break;
				case 't':
					trace++;
					break;
				case 'T':
					traceinput++;
					break;
				default:
					if((*cp >= '0' && *cp <= '9') || *cp == '-') {
						hopoffset = atoi(cp);
						while(*cp >= '0' && *cp <= '9')
							cp++;
						cp--;
					} else {
						fprint(2, "usage: routed [-qtvT#]\n");								exit(1);
					}
					break;
				}
			}
		} else
			recsafe(argv[i]);
	}

	if(verbose || traceinput || trace) {
		if(hopoffset)
			fprint(2, "%s: adding %d to each metric\n", argv[0], hopoffset);
	}

	if((udpfd = udp_datagram(RPORT)) < 0) {
		perror("udp route daemon: no connection");
		exit(1);
	}

	/*
	 *  try pushing bufld, complain but don't exit if unable
	 */
/*
	if(ioctl(udpfd, FIOPUSHLD, &buf_ld)<0){
		perror("pushing buf_ld");
	}
	if(ioctl(udpfd, FIOPUSHLD, &buf_ld)<0){
		perror("pushing buf_ld");
	}
	if(ioctl(udpfd, FIOPUSHLD, &buf_ld)<0){
		perror("pushing buf_ld");
	}
*/

	if((ipfd = open(IPDEVICE, 2)) < 0) {
		perror(IPDEVICE);
		exit(1);
	}

	readgateways();
	if(!(verbose || traceinput || trace)) {
		detach("routed");
		nice(-20);
	}
	timer();

again:
	while((n = read(udpfd, &packet, sizeof(struct udppacket))) > 0) {
		routed(&packet, n - sizeof(struct udpaddr));
	}
	if(n < 0 && errno == EINTR)
		goto again;
}


/*	protocol.h	4.10	83/08/11	*/
/*
 * Routing Information Protocol
 *
 * Derived from Xerox NS Routing Information Protocol
 * by changing 32-bit net numbers to sockaddr's and
 * padding stuff to 32-bit boundaries.
 */
#define	RIPVERSION	1
#define AF_INET		2
#define AF_UNSPEC	0

struct sockaddr{
	short sin_family;
	u_short sin_port;
	u_long sin_addr;
	char sin_zero[8];
};
struct netinfo {
	struct	sockaddr rip_dst;	/* destination net/host */
	int	rip_metric;		/* cost of route */
};

struct rip {
	u_char	rip_cmd;		/* request/response */
	u_char	rip_vers;		/* protocol version # */
	u_char	rip_res1[2];		/* pad to 32-bit boundary */
	union {
		struct	netinfo ru_nets[1];	/* variable length... */
		char	ru_tracefile[1];	/* ditto ... */
	} ripun;
#define	rip_nets	ripun.ru_nets
#define	rip_tracefile	ripun.ru_tracefile
};
 
/*
 * Packet types.
 */
#define	RIPCMD_REQUEST		1	/* want info */
#define	RIPCMD_RESPONSE		2	/* responding to request */
#define	RIPCMD_TRACEON		3	/* turn tracing on */
#define	RIPCMD_TRACEOFF		4	/* turn it off */

#define	RIPCMD_MAX		5
#ifdef RIPCMDS
char *ripcmds[RIPCMD_MAX] =
  { "#0", "REQUEST", "RESPONSE", "TRACEON", "TRACEOFF" };
#endif

#define	HOPCNT_INFINITY		16	/* per Xerox NS */
#define	MAXPACKETSIZE		488	/* max broadcast size */
#define MAXROUTESPERPACKET	(MAXPACKETSIZE-4)/sizeof(struct netinfo)

/*
 * Timer values used in managing the routing table.
 * Every update forces an entry's timer to be reset.  After
 * EXPIRE_TIME without updates, the entry is marked invalid,
 * but held onto until GARBAGE_TIME so that others may
 * see it "be deleted".
 */
#define	TIMER_RATE		60	/* interval in seconds over which we bcast to all
					 * directly connected interfaces */

#define	EXPIRE_TIME		180	/* time to mark entry invalid */
#define	GARBAGE_TIME		240	/* time to garbage collect */

#define NIFS TIMER_RATE
struct ipif ifs[NIFS];
int nextifs;			/* next interface to broadcast to */
int nifs;			/* number of functioning interfaces */

routed(up, len)
	struct udppacket *up;
	u_int len;
{
	struct rip *rip;

	if(traceinput)
		logevent("from %s\n", in_ntoa(up->addr.host));
	rip = (struct rip *)(up->buf);
	if(rip->rip_vers != RIPVERSION)
		return;
	switch(rip->rip_cmd){
	case RIPCMD_RESPONSE:
		if(up->addr.port != RPORT)
			logevent("routed: %s %d masquerading as routed\n",
				in_host(up->addr.host), up->addr.port);
		else
			response(up, len);
		break;
	case RIPCMD_REQUEST:
		request(up, len);
		break;
	default:
		logevent("routed: %s %d sent strange command %d\n",
			in_host(up->addr.host), up->addr.port, rip->rip_cmd);
		return;
	}
}

/*
 *  don't believe responses from this host.  no use infinitely
 *  recursing.
 */
response(up, len)
	struct udppacket *up;
	u_int len;
{
	struct rip *rip;
	struct sockaddr *sin;
	struct netinfo *np;

	rip = (struct rip *)(up->buf);
	if(myaddr(up->addr.host))
		return;
	np = rip->rip_nets;
	while(np < (struct netinfo *)(&(up->buf[len]))){
		sin = &(np->rip_dst);
		np->rip_metric = ntohl(np->rip_metric);
		sin->sin_port = ntohs(sin->sin_port);
		sin->sin_addr = ntohl(sin->sin_addr);
		if(traceinput) {
			logevent(" %s %d\n", in_ntoa(sin->sin_addr), np->rip_metric);
		}
		if (np->rip_metric > 0 && saferoute(sin->sin_addr) == 0)
			rtinstall(sin->sin_addr, up->addr.host, np->rip_metric, 0, -1);
		np++;
	}
}

/*
 *  Only accept requests for all routes or for internet routes.
 *  In all cases send back everything.
 */
request(up, len)
	struct udppacket *up;
	u_int len;
{
	struct rip *rip;
	struct sockaddr *sin;
	struct netinfo *np;

	rip = (struct rip *)(up->buf);
	np = rip->rip_nets;
	if(traceinput)
		logevent("request from %s\n", in_ntoa(up->addr.host));

	sin = &(np->rip_dst);
	np->rip_metric = ntohl(np->rip_metric);
	sin->sin_family = ntohs(sin->sin_family);
	if((sin->sin_family == AF_UNSPEC && np->rip_metric == HOPCNT_INFINITY)
	|| sin->sin_family == AF_INET)
		send(up);
}

struct route{
	u_long dst;
	u_long gate;
	int metric;
	int age;
	int nif;
};
#define NROUTES 720
struct route routes[NROUTES];

rtinstall(dst, gate, metric, age, nif)
u_long dst, gate;
{
	struct route *rp, *save = 0;
	char hname[128];

	/*
	 *  don't let a default route get set unless we are quiet
	 */
	if(dst==0 && !quiet)
		return;

	/*
	 *  ignore any route with inifinite hop counts
	 */
	if(metric >= HOPCNT_INFINITY)
		return;

	/*
	 *  look up the route in our local tables (not the kernel's)
	 */
	for(rp = routes; rp < &routes[NROUTES]; rp++){
		if(rp->dst == dst)
			break;
		if(rp->dst == 0 && save == 0)
			save = rp;
	}
	if(rp >= &routes[NROUTES] && (rp = save) == 0){
		logevent("routed: out of routes?\n");
		return;
	}

	/*
	 *  if this is a new route, make sure the new count is smaller
	 */
	if(rp->dst == 0)
		rp->metric = HOPCNT_INFINITY + 1;

	/*
	 *  if new route is closer than the old route
	 *  or the gate is the same but the distance has changed
	 *  or the gate is the same but the age has changed,
	 *  then use the new gate
	 */
	if(metric < rp->metric
	   || (gate == rp->gate && metric != rp->metric)
	   || (gate == rp->gate && age != rp->age)){
		rp->metric = metric;
		rp->age = age;
		if(rp->dst != dst || rp->gate != gate){
			if(verbose){
				strcpy(hname, in_host(gate));
				logevent("%s installing as route to %s, metric %d\n",
					hname, in_host(dst), metric);
			}
			rp->dst = dst;
			rp->gate = gate;
			rp->nif = nif;
			if(ioctl(ipfd, IPIOROUTE, rp) < 0)
				logevent("can't install routed %s\n", in_ntoa(rp->gate));
		} else {
			if(verbose > 1){
				strcpy(hname, in_host(gate));
				logevent("%s confirmed as route to %s, metric %d\n",
					hname, in_host(dst), metric);
			}
			rp->dst = dst;
			rp->gate = gate;
		}
	}
}

/*
 * keep a list of routes
 * that belong to us;
 * don't let anyone claim them
 */

#define	NSAFE	20

u_long safer[NSAFE];
int nsafe = 0;

recsafe(s)
char *s;
{
	if (nsafe < NSAFE)
		safer[nsafe++] = in_address(s);
}

saferoute(dst)
u_long dst;
{
	register int i;

	for (i = 0; i < nsafe; i++)
		if (dst == safer[i])
			return (1);
	return (0);
}

readgateways()
{
	int fd;
	char *cp;
	char net[32], gateway[32], which[32];
	u_long dst, gate;
	int metric;

	if((fd = open(GATEWAYS, 0)) == 0)
		return;
	Finit(fd, 0);
	while(cp=Frdline(fd)){
		if(sscanf(cp, "%s %s gateway %s metric %d",
			  which, net, gateway, &metric) != 4)
			continue;
		dst = in_address(net);
		gate = in_address(gateway);
		rtinstall(dst, gate, metric, -1, -1);
	}
}

timer()
{
	int i;
	struct route *rp;
	struct ipif ipif;
	u_long net;
	char haddr1[32], haddr2[322];

	if(nextifs == 0){
		/*
		 *  get interfaces from kernel.  ignore interfaces that are down,
		 *  interfaces that are loop place holders (this==that), and the
		 *  loop-back network
		 */
		for(i = nifs = 0; nifs < NIFS; i++){
			*(int *)&ipif = i;
			if(ioctl(ipfd, IPIOGETIFS, &ipif) < 0)
				break;
			if((ipif.flags&IFF_UP) && ipif.thishost != ipif.that &&
			   ipif.that != 0x7f000000) {
				if(ipif.bcast[0] == 0)
					ipif.bcast[0] = ipif.that;
				ifs[nifs++] = ipif;
				if(trace){
					strcpy(haddr1, in_ntoa(ipif.that));
					strcpy(haddr2, in_ntoa(ipif.thishost));
					logevent("ifs[%d] %s %s %s\n", nifs, haddr1,
						haddr2, in_ntoa(ipif.bcast[0]));
				}
				rtinstall(ipif.that, ipif.that, 0, 0, nifs - 1);
			}
		}
	}
	
	if(!quiet)
		broadcast();

	/*
	 *  age routes && get rid of too old routes
	 */
	if(nextifs == 0)
		for(rp = routes; rp < &routes[NROUTES]; rp++){
			if(rp->dst == 0)
				continue;
			if(rp->age > 10){
				rtdelete(rp);
			} else if(rp->age >= 0){
				rp->age++;
			}
		}

	/*
	 *  post alarm for broadcast to next interface
	 */
	signal(SIGALRM, timer);
	i = nifs>0 ? TIMER_RATE/nifs : TIMER_RATE;
	alarm(i+1);
}

/*
 *  Send a broadcast to the next interface.  This happens once every TIMER_RATE/nifs
 *  seconds so that n TIMER_RATE seconds we shoulc hit every interface.
 */
broadcast()
{
	static long last;
	long now;
	int reps;
	struct udppacket p;

	/*
	 *  broadcast to interfaces.  approximate one broadcast every
	 *  TIMER_RATE/nifs seconds.
	 */
	now = time((long *)0);
	reps = nifs ? (now-last)/(TIMER_RATE/nifs) : 0;
	if(reps > nifs)
		reps = nifs;
	for(; reps>0; reps--){
		if(++nextifs >= nifs)
			nextifs = 0;

		p.addr.host = ifs[nextifs].bcast[0];
		p.addr.port = RPORT;
		send(&p);
	}
	last = now;
}

/*
 *  put an entry into the bradcast message
 */
struct netinfo *
insert(rp, up, np, rip)
	struct udppacket *up;
	struct route *rp;
	struct netinfo *np;
	struct rip *rip;
{
	struct sockaddr *sin;
	char haddr[32];

 	/*
	 *  don't send routes back onto the network from which they
	 *  came
	 */
	if((rp->gate & ifs[nextifs].mask) == ifs[nextifs].that)
		return np;

	/*
	 *  put the routing entry into the message.
	 */
	bzero(np, sizeof(struct netinfo));
	sin = &(np->rip_dst);
	np->rip_metric = htonl(rp->metric + 1 + hopoffset);
	sin->sin_port = 0;

	/*
	 *  don't send information about one network's subnets to other networks
	 */
	if(in_netof(rp->dst) == in_netof(ifs[nextifs].that))
		sin->sin_addr = htonl(rp->dst);
	else
		sin->sin_addr = htonl(in_netof(rp->dst));

	sin->sin_family = htons((u_short)AF_INET);
	if(trace){
		strcpy(haddr, in_ntoa(ntohl(sin->sin_addr)));
		logevent(" %s %s %d\n",
			haddr,
			in_ntoa(rp->dst),
			ntohl(np->rip_metric));
	}
	np++;

	/*
	 *  if the message is at maximum size, send it and start a new one
	 */
	if (np-rip->rip_nets == MAXROUTESPERPACKET) {
		psend(up, (char *)np);
		np = rip->rip_nets;
	}
	return np;
}

/*
 *  send info about all routes
 */
send(up)
	struct udppacket *up;
{
	struct route *rp;
	struct rip *rip;
	struct netinfo *np;
	long net;

	net = in_netof(ifs[nextifs].bcast[0]);
	rip = (struct rip *)up->buf;
	rip->rip_cmd = RIPCMD_RESPONSE;
	rip->rip_vers = RIPVERSION;
	np = rip->rip_nets;
	if(trace)
		logevent("BROADCAST:\n");

	/*
	 *  first send info about directly connected networks
	 */
	for(rp = routes; rp < &routes[NROUTES]; rp++){
		if(rp->dst == 0 || rp->metric > 0)
			continue;
		if(rp->metric + hopoffset + 1 >= HOPCNT_INFINITY)
			continue;
		np = insert(rp, up, np, rip);
	}

	/* 
	 *  now send info about the rest
	 */
	for(rp = routes; rp < &routes[NROUTES]; rp++){
		if(rp->dst==0 || rp->metric<=0)
			continue;
		if(rp->metric + hopoffset + 1 >= HOPCNT_INFINITY)
			continue;
		np = insert(rp, up, np, rip);
	}
	if (np > rip->rip_nets)
		psend(up, (char *)np);
}

/* send one routing packet to one network */
psend(pp, cp)
	struct udppacket *pp;
	char *cp;
{
	int len=cp-(char *)pp;
	int i;

	if(trace)
		logevent("to %s %d\n", in_ntoa(pp->addr.host), len);
	if (write(udpfd, pp, len) !=len)
		logevent("udp write error\n");
}

myaddr(x)
u_long x;
{
	int i;

	for(i = 0; i < nifs; i ++)
		if(ifs[i].thishost == x)
			return(1);
	return(0);
}

rtdelete(rp)
struct route *rp;
{
	if(verbose) {
		logevent("deleting %s %d\n", in_ntoa(rp->dst), rp->metric);
	}
	if(rp->gate != rp->dst){
		rp->gate = 0;
		if(ioctl(ipfd, IPIOROUTE, rp) < 0)
			logevent("can't remove route %s\n", in_ntoa(rp->gate));
	}
	rp->gate = rp->dst = 0;
	rp->age = rp->metric = 0;
}
