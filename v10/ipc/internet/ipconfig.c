#include <sys/param.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/filio.h>
#include <sys/inio.h>
#include <sys/enio.h>
#include <sys/inet/ethernet.h>
#include <sys/inet/in.h>
#include "config.h"

extern errno;
extern unsigned short htons();
extern int ip_ld;
extern char *optarg;
extern int optind;
char *enpr();

/*
 *  global because we're too lazy to pass them as args
 */
int debug, fool;
in_addr myinaddr, hisinaddr, mask, broadcast;
int superarp;
u_char myenaddr[6];
u_char noether[6];

usage(name)
	char *name;
{
	fprint(2,
	   "Usage: %s [-m subnet-mask] [-s] device myinaddr hisinaddr [arp-device]\n",
	   name);
	fprint(2,
	   "       %s [-m subnet-mask] [-s] myinaddr hisinaddr\n",
	   name);
	fprint(2,
	   "       %s [-m subnet-mask] [-s] myinaddr\n",
	   name);
	exit(1);
}

main(argc, argv)
char *argv[];
{
	char *dev, *me, *it, *arp;
	int ipfd, enfd, x, ld;
	int c, mtu;

	/*
	 *  process arguments
	 */
	while((c = getopt(argc, argv, "dfm:s")) != -1)
		switch(c){
		case 'd':
			/*
			 *  debugging
			 */
			debug = 1;
			break;
		case 'f':
			/*
			 *  fool people not understanding the
			 *  broadcast IP address to shut them up
			 */
			fool = 1;
			break;
		case 'm':
			/*
			 *  subnet mask
			 */
			if(strchr(optarg, '.'))
				mask = in_aton(optarg);
			else
				sscanf(optarg, "%x", &mask);
			break;
		case 's':
			/*
			 *  answer all arp requests for this net ignoring mask
			 */
			superarp = 1;
			break;
		default:
			usage(argv[0]);
			break;
		}

	switch(argc-optind){
	case 1:
		dev = 0;
		me = argv[optind++];
		it = me;
		arp = 0;
		break;
	case 2:
		dev = 0;
		me = argv[optind++];
		it = argv[optind++];
		arp = 0;
		break;
	case 3:
		dev = argv[optind++];
		me = argv[optind++];
		it = argv[optind++];
		arp = 0;
		break;
	case 4:
		dev = argv[optind++];
		me = argv[optind++];
		it = argv[optind++];
		arp = argv[optind++];
		break;
	default:
		usage(argv[0]);
		break;
	}

	/*
	 *  look up the addresses
	 */
	hisinaddr = in_address(it);
	if(hisinaddr == 0){
		logevent("ipconfig: unknown host/net %s\n", it);
		exit(1);
	}
	myinaddr = in_addronnet(me, hisinaddr, mask);
	if(myinaddr == 0){
		logevent("ipconfig: no host %s on same net as %s\n", me,it);
		exit(1);
	}

	/*
	 *  push disciplines onto the right device
	 */
	signal(SIGHUP, SIG_IGN);
	if(dev) {
		ipfd = open(dev, 2);
		if(ipfd < 0){
			perror(dev);
			exit(1);
		}
		if(arp){
			x = htons((unsigned short)ETHERPUP_IPTYPE);
			if(ioctl(ipfd, ENIOTYPE, &x) < 0){
				logevent("ENIOTYPE\n");
				exit(1);
			}
		}
		flavor(ipfd, myinaddr, hisinaddr, mtu);
	} else {
		int pfd[2];
		if(pipe(pfd)<0){
			logevent("pipe\n");
			exit(1);
		}
		flavor(pfd[0], myinaddr, hisinaddr, mtu);
		flavor(pfd[1], hisinaddr, myinaddr, mtu);
	}

	/*
	 *  wait for ever or do arping
	 */
	if(arp == 0){
		if(!debug)
			detach("ipconfig");
		pause();	/* forever, hopefully */
		exit(0);
	}
	if(ioctl(ipfd, IPIOARP, 0) < 0){
		logevent("IPIOARP\n");
		exit(1);
	}
	enfd = open(arp, 2);
	if(enfd < 0){
		logevent("can't open arp device %s\n", arp);
		exit(1);
	}
	if(!debug)
		detach("ipconfig");
	doarp(ipfd, enfd);
}

flavor(ipfd, myinaddr, hisinaddr, mtu)
	int ipfd;
	in_addr myinaddr, hisinaddr;
	int mtu;
{
	if(ioctl(ipfd, FIOPUSHLD, &ip_ld) < 0){
		logevent("PUSHLD\n");
		exit(1);
	}

	/*
	 *  tell the line disciplines the identity of the ends
	 */
	if(ioctl(ipfd, IPIOLOCAL, &myinaddr) < 0){
		logevent("IPIOLOCAL\n");
		exit(1);
	}
	if(hisinaddr!=in_netof(hisinaddr) && !mask){
		/*
		 *  host to host connection
		 */
		ioctl(ipfd, IPIOHOST, &hisinaddr);
		broadcast = hisinaddr;
	} else {
		/*
		 *  host to network connection
		 */
		mtu = 1500;
		ioctl(ipfd, IPIOMTU, &mtu);
		ioctl(ipfd, IPIONET, &hisinaddr);
		if(mask) {
			ioctl(ipfd, IPIOMASK, &mask);
			broadcast = hisinaddr | ~mask;
		} else {
			broadcast = hisinaddr |
				    (IN_CLASSA(hisinaddr) ? IN_CLASSA_HOST :
				     (IN_CLASSB(hisinaddr) ? IN_CLASSB_HOST :
				      (IN_CLASSC(hisinaddr) ? IN_CLASSC_HOST :
				       0)));
		}
	}
}

/*
 * Address resolution
 */

struct ether_arp{
	/* driver goo */
	struct etherpup arp_ether;

	/* arp stuff */
	u_short	arp_hrd;
#define ARPHRD_ETHER	1
	u_short	arp_pro;
	u_char	arp_hln;
	u_char	arp_pln;
	u_short	arp_op;
#define ARPOP_REQUEST	1
#define ARPOP_REPLY	2
	u_char	arp_sha[6];	/* sender ether addr */
	u_char	arp_spa[4];	/* sender internet addr */
	u_char	arp_tha[6];	/* target ether addr */
	u_char	arp_tpa[4];	/* target internet addr */
};

u_char broadaddr[6] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/* wait for ARP requests and answers */
doarp(ipfd, enfd)
	int ipfd;
	int enfd;
{
	fd_set rdfds;
	int x;
	char buf[2000];
	struct goo{
		in_addr inaddr;
		u_char enaddr[6];
	} goo;
	in_addr tpa;

	x = htons((u_short)ETHERPUP_ARPTYPE);
	if(ioctl(enfd, ENIOTYPE, &x) < 0){
		perror("ipconfig: ENIOTYPE");
		exit(1);
	}
	if(ioctl(enfd, ENIOADDR, myenaddr) < 0){
		perror("ipconfig: ENIOADDR");
		exit(1);
	}

	FD_ZERO(rdfds);
	while(1){
		FD_SET(ipfd, rdfds);
		FD_SET(enfd, rdfds);

		errno = 0;	/* just in case */
		if(select(20, &rdfds, 0, 2000) < 0){
			if(errno == EINTR)
				continue;
			logevent("ipconfig: select error\n");
			exit(1);
		}
		if(FD_ISSET(ipfd, rdfds)){
			if(read(ipfd, &tpa, sizeof(tpa)) != sizeof(tpa))
				logevent("ipconfig: in read\n");
			arpwhohas(enfd, ipfd, tpa);
		}
		if(FD_ISSET(enfd, rdfds)){
			if(read(enfd, buf, sizeof(buf)) <= 0)
				logevent("ipconfig: en read\n");
			arpinput(ipfd, enfd, buf);
		}
	}
}

/* broadcast an arp request */
arpwhohas(enfd, ipfd, addr)
	in_addr addr;
{
	struct goo{
		in_addr inaddr;
		u_char enaddr[6];
	} goo;
	struct ether_arp a;
	in_addr rddaniym;

	if(addr == myinaddr){
		goo.inaddr = addr;
		bcopy(myenaddr, goo.enaddr, sizeof(goo.enaddr));
		ioctl(ipfd, IPIORESOLVE, &goo);
		return;
	}
	if(addr == hisinaddr || addr == broadcast){
		goo.inaddr = addr;
		bcopy(broadaddr, goo.enaddr, sizeof(goo.enaddr));
		ioctl(ipfd, IPIORESOLVE, &goo);
		return;
	}
	bcopy(broadaddr, a.arp_ether.dhost, sizeof(a.arp_ether.dhost));
	a.arp_ether.type = htons(ETHERPUP_ARPTYPE);

	a.arp_hrd = htons(ARPHRD_ETHER);
	a.arp_pro = htons(ETHERPUP_IPTYPE);
	a.arp_hln = sizeof(goo.enaddr);
	a.arp_pln = sizeof(in_addr);
	a.arp_op = htons(ARPOP_REQUEST);

	bcopy(myenaddr, a.arp_sha, sizeof(a.arp_sha));
	rddaniym = htonl(myinaddr);
	bcopy(&rddaniym, a.arp_spa, sizeof(a.arp_spa));
	addr = htonl(addr);
	bcopy(&addr, a.arp_tpa, sizeof(a.arp_tpa));

	write(enfd, &a, sizeof(a));
}

/* process an arp request */
arpinput(ipfd, enfd, ap)
	struct ether_arp *ap;
{
	struct goo{
		in_addr inaddr;
		u_char enaddr[6];
	} goo;
	in_addr spa, tpa;
	in_addr rddaniym;
	int forgery = 0;

	bcopy(ap->arp_spa, &spa, sizeof(spa));
	bcopy(ap->arp_tpa, &tpa, sizeof(tpa));
	spa = ntohl(spa);
	tpa = ntohl(tpa);

	if(debug){
		print("%s from %s", 
			ap->arp_op == ntohs(ARPOP_REQUEST) ? "request" : "answer",
			in_ntoa(spa));
		print(" for %s\n", in_ntoa(tpa));
	}

	if(ntohs(ap->arp_pro) != ETHERPUP_IPTYPE)
		return;

	/* make sure noone's trying to be me */
	if(memcmp(ap->arp_sha, myenaddr, sizeof(myenaddr)) != 0){
		if(spa == myinaddr){
			/*
			 *  scream on the console and send out an
			 *  arp packet with my correct ether addrss
			 */
			logconsole("ipconfig: machine at ether address %s is forging my IP address\n", enpr(ap->arp_ether.shost));
			tpa = spa;
			bcopy(myenaddr, ap->arp_sha, sizeof(myenaddr));
			forgery = 1;
		}
	}

	if(!forgery){
		/* incorporate sender's address */
		goo.inaddr = spa;
		bcopy(ap->arp_sha, goo.enaddr, sizeof(goo.enaddr));
		if(ioctl(ipfd, IPIORESOLVE, &goo) < 0)
			logevent("ipconfig: IPIORESOLVE");

		/* send reply only to a request */
		if(ap->arp_op != ntohs(ARPOP_REQUEST))
			return;
	}

	/*  accept the request if
	 *  (1) the target is me or
	 *  (2) superarp==1 and (  the target is not on my subnet &&
	 *			   the target is on my net  )
	 */
	if(tpa==myinaddr || (superarp && in_netof(tpa)==in_netof(myinaddr) &&
			     in_subnetof(tpa, mask)!=in_subnetof(myinaddr, mask))){
		if(debug)
			print("answering %s\n", enpr(ap->arp_sha));
		ap->arp_hrd = htons(ARPHRD_ETHER);
		ap->arp_pro = htons(ETHERPUP_IPTYPE);
		ap->arp_op = htons(ARPOP_REPLY);
		rddaniym = htonl(tpa);
		tpa = htonl(spa);
		spa = rddaniym;
		bcopy(&tpa, ap->arp_tpa, sizeof(ap->arp_tpa));
		bcopy(&spa, ap->arp_spa, sizeof(ap->arp_spa));
		bcopy(ap->arp_sha, ap->arp_tha, sizeof(ap->arp_tha));
		bcopy(myenaddr, ap->arp_sha, sizeof(ap->arp_sha));
		bcopy(broadaddr, ap->arp_ether.dhost, sizeof(ap->arp_ether.dhost));
		ap->arp_ether.type = htons(ETHERPUP_ARPTYPE);
		write(enfd, ap, sizeof(struct ether_arp));
	} else if((tpa==hisinaddr || tpa==broadcast)){
		/*
		 *  For various reasons, some systems to not understand
		 *  the correct IP address to use for broadcast.  This
		 *  causes them to send there future arp requests (for that
		 *  address) to a non-existant ether address.
		 */
		logevent("bozo request from %s", in_ntoa(spa));
		fprint(2, " for %s\n", in_ntoa(tpa));
		if(fool){
			logevent("giving him bozoid arp response\n");
			ap->arp_hrd = htons(ARPHRD_ETHER);
			ap->arp_pro = htons(ETHERPUP_IPTYPE);
			ap->arp_op = htons(ARPOP_REPLY);
			rddaniym = htonl(tpa);
			tpa = htonl(spa);
			spa = rddaniym;
			bcopy(&tpa, ap->arp_tpa, sizeof(ap->arp_tpa));
			bcopy(&spa, ap->arp_spa, sizeof(ap->arp_spa));
			bcopy(ap->arp_sha, ap->arp_tha, sizeof(ap->arp_tha));
			bcopy(noether, ap->arp_sha, sizeof(ap->arp_sha));
			bcopy(ap->arp_ether.shost, ap->arp_ether.dhost,sizeof(ap->arp_ether.dhost));
			ap->arp_ether.type = htons(ETHERPUP_ARPTYPE);

			write(enfd, ap, sizeof(struct ether_arp));
		}
	}
}

arppr(a)
struct ether_arp a;
{
	in_addr spa, tpa;

	print("dhost %s\n", enpr(a.arp_ether.dhost));
	print("shost %s\n", enpr(a.arp_ether.shost));
	print("type  %x\n", ntohs(a.arp_ether.type));
	a.arp_hrd = ntohs(a.arp_hrd);
	a.arp_pro = ntohs(a.arp_pro);
	a.arp_op = ntohs(a.arp_op);

	bcopy(a.arp_spa, &spa, sizeof(spa));
	bcopy(a.arp_tpa, &tpa, sizeof(tpa));
	tpa = ntohl(tpa);
	spa = ntohl(spa);

	print("hrd %d pro %x op %d spa %x tpa %x\n",
		a.arp_hrd, a.arp_pro, a.arp_op, spa, tpa);
	print("sha %s\n", enpr(a.arp_sha));
	print("tha %s\n", enpr(a.arp_tha));
}

char *
enpr(en)
u_char *en;
{
	static char a[128];

	sprint(a, "%02x %02x %02x %02x %02x %02x", en[0], en[1], en[2],
		en[3], en[4], en[5]);
	return a;
}
