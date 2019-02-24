#include <sys/param.h>
#include <sys/inet/ip.h>
#include <ipc.h>

int printip();

typedef struct {
	int code;
	char *meaning;
	int (*action)();
	int needid;
} Code;

typedef struct {
	int type;
	Code *c;
} Msg;

typedef struct {
	int type;
	int inuse;
	int id;
} Client;

/* Destination Unreachable Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                             unused                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/

Code unreachc[] = {
	{ 0, "net unreachable", printip, 0 },
	{ 1, "host unreachable", printip, 0 },
	{ 2, "protocol unreachable", printip, 0 },
	{ 3, "port unreachable", printip, 0 },
	{ 4, "fragment needed and DF set", printip, 0 },
	{ 5, "source route failed", printip, 0 },
	{ -1, 0, 0 }
};

typedef struct {
	u_char param[4];
	struct ip ip;
} IpMsg;


/* Time Exceeded Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                             unused                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code timec[] = {
	{ 0, "time to live exceeded in transit", printip, 0 },
	{ 1, "fragment reassembly time exceeded", printip, 0 },
	{ -1, 0, 0 }
};



/* Parameter Problem Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |    Pointer    |                   unused                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code paramc[] = {
	{ 0, "parameter problem", printip, 0 },
	{ -1, 0, 0 }
};

/* Source Quench Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                             unused                            |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code quenchc[] = {
	{ 0, "source quench", printip, 0 },
	{ -1, 0, 0 }
};


/* Redirect Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                 Gateway Internet Address                      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |      Internet Header + 64 bits of Original Data Datagram      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code redirectc[] = {
	{ 0, "redirect datagrams for the network", printip, 0 },
	{ 1, "redirect datagrams for the host", printip, 0 },
	{ 2, "redirect datagrams for the type of service and network", printip, 0},
	{ 3, "redirect datagrams for the type of service and host", printip, 0 },
	{ -1, 0, 0 }
};


/* Echo or Echo Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |     Code      |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Data ...
   +-+-+-+-+-
*/

int echo();
Code echoc[] = {
	{ 0, "echo", echo, 1 },
	{ -1, 0, 0 }
};
Code echorc[] = {
	{ 0, "echo reply", 0, 1 },
	{ -1, 0, 0 }
};

typedef struct {
	u_char id[2];
	u_char seqno[2];
} EchoMsg;

/* Timestamp or Timestamp Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |      Code     |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Originate Timestamp                                       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Receive Timestamp                                         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Transmit Timestamp                                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code timestampc[] = {
	{ 0, "timestamp", 0, 1 },
	{ -1, 0, 0 }
};
Code timestamprc[] = {
	{ 0, "timestamp reply", 0, 1 },
	{ -1, 0, 0 }
};

typedef struct {
	u_char id[2];
	u_char seqno[2];
	u_char orig[4];
	u_char recv[4];
	u_char xmit[4];
} TsMsg;


/* Information Request or Information Reply Message

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |     Type      |      Code     |          Checksum             |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |           Identifier          |        Sequence Number        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

Code infoc[] = {
	{ 0, "info", 0, 1 },
	{ -1, 0, 0 }
};
Code inforc[] = {
	{ 0, "info reply", 0, 1 },
	{ -1, 0, 0 }
};

Msg msgs[] = {
	 { 0, echorc },
	 { 3, unreachc },
	 { 4, quenchc },
	 { 5, redirectc },
	 { 8, echoc },
	 { 11, timec },
	 { 12, paramc },
	 { 13, timestampc },
	 { 14, timestamprc },
	 { 15, infoc },
	 { 16, inforc },
};

typedef struct {
	struct ip ip;
	u_char type;
	u_char code;
	u_char cksum[2];
	union {
		IpMsg i;
		EchoMsg e;
		TsMsg t;
	} u;
	u_char buffer[4096];
} IcmpMsg;

/*
 *  globals
 */
char *av0;
Client clients[NOFILE];
fd_set cfds;
int debug;

ding()
{
	signal(SIGALRM, ding);
	alarm(10);
}

/*
 *  icmp mgr
 */
main(ac, av)
	int ac;
	char *av[];
{
	char *mtpt="icmp", *cp;
	int i;
	int ipfd, fsfd;
	fd_set fds;

	av0 = av[0];
	chdir("/cs");
	for (i=1; i<ac; i++) {
		if (av[i][0] == '-') {
			for (cp=&av[i][1]; *cp; cp++) {
				switch(*cp) {
				case 'd':
					debug = 1;
					break;
				case 'm':
					if (i+1>=ac)
						usage(av0);
					mtpt = av[++i];
					break;
				default:
					usage(av0);
				}
			}
		}
	}

	/*
	 * open the ip protocol
	 */
	ipfd = open("/dev/ipicmp", 2);
	if(ipfd < 0){
		logevent("error opening ip device\n");
		exit(1);
	};

	/*
	 * plug ourselves into the file system
	 */
	fsfd = ipccreat(mtpt, "");
	if(ipfd < 0){
		logevent("error mounting into fs\n");
		exit(1);
	};

	if(!debug)
		detach(mtpt);

	/*
	 * clear out the clients
	 */
	for(i=0; i<NOFILE; i++){
		clients[i].id = i;
		clients[i].inuse = 0;
	}

	/*
	 * spin honoring requests
	 */
	FD_ZERO(fds);
	for(;;) {
		int n;

		fds = cfds;
		FD_SET(ipfd, fds);
		FD_SET(fsfd, fds);
		switch(n = select(NOFILE, &fds, 0, 10000)){
		case -1:
			logevent("select failed\n");
			exit(1);
		case 0:
			continue;
		}
		signal(SIGALRM, ding);
		alarm(10);
		if(FD_ISSET(ipfd, fds)) {
			readnet(ipfd);
			n--;
		}
		if(n && FD_ISSET(fsfd, fds)) {
			newclient(fsfd);
			n--;
		}
		for(i = 0; n && i<NOFILE; i++){
			if(FD_ISSET(i, fds)){
				readclient(i, ipfd);
				n--;
			}
		}
		alarm(0);
	}
}

/*
 * read an icmp message from the network
 */
readnet(fd)
{
	Msg *mp;
	IcmpMsg m;
	int n;
	char fromsys[32], tosys[32];
	u_char c[2];
	Code *cp;

	if((n = read(fd, &m, sizeof(m))) > 0){
		strcpy(fromsys, in_ntoa(m.ip.ip_src));
		strcpy(tosys, in_ntoa(m.ip.ip_dst));
		logevent("in %s -> %s, type %d code %d\n", fromsys, tosys,
			m.type, m.code);
		c[0] = m.cksum[0];
		c[1] = m.cksum[1];
		cksum(&m, n);
		if(c[0] != m.cksum[0] || c[1] != m.cksum[1]){
			fprint(2, "checksum error %.2x%.2x/%.2x%.2x\n", c[0],
				c[1], m.cksum[0], m.cksum[1]);
			return;
		}
		for(mp = msgs; mp->type >= 0; mp++){
			if(mp->type == m.type){
				for(cp = mp->c; cp->code >= 0; cp++)
					if(cp->code == m.code)
						break;
				if(cp->code < 0)
					fprint(2, "undefined code\n");
				else if(cp->action)
					(*cp->action)(fd, &m, n);
				break;
			}
		}
		if(mp->type < 0)
			fprint(2, "undefined type\n");
	} else {
		logevent("error reading from network %d\n", errno);
		exit(1);
	}
}

/*
 * a call from a new client
 */
newclient(fd)
{
	int cfd;
	ipcinfo *ipc;

	/*
	 *  get the new call (protected by a timeout)
	 */
	ipc = ipclisten(fd);
	if(fd==0)
		return;
	cfd = ipcaccept(ipc);
	if(cfd<0)
		return;

	/*
	 *  set up a new id (for echo/timestamp/etc)
	 */
	clients[cfd].id += NOFILE;
	clients[cfd].inuse = 1;
	FD_SET(cfd, cfds);
	logevent("new client %d %s!%s\n", cfd, ipc->machine, ipc->user);
}

/*
 * read an icmp message from a client (protected by an timeout).
 * if this is a message that requires an id, put one in.
 */
readclient(fd, ipfd)
{
	IcmpMsg m;
	int n;
	char fromsys[32], tosys[32];
	Msg *mp;
	Code *cp;

	if((n = read(fd, &m, sizeof(m))) > 0){
		strcpy(fromsys, in_ntoa(m.ip.ip_src));
		strcpy(tosys, in_ntoa(m.ip.ip_dst));
		logevent("out(%d) %s -> %s, type %d code %d\n", fd, fromsys,
			tosys, m.type, m.code);
		for(mp = msgs; mp->type >= 0; mp++){
			if(mp->type == m.type){
				for(cp = mp->c; cp->code >= 0; cp++)
					if(cp->code == m.code)
						break;
				if(cp->code < 0)
					fprint(2, "undefined code\n");
				else {
					if(cp->needid){
						m.u.e.id[0] = clients[fd].id;
						m.u.e.id[1] = clients[fd].id>>8;
					}
					ipsend(ipfd, &m, n, m.ip.ip_src,
						m.ip.ip_dst);
				}
				break;
			}
		}
		if(mp->type < 0)
			fprint(2, "undefined type\n");
	} else {
		logevent("losing client %d\n", fd);
		FD_CLR(fd, cfds);
		close(fd);
	}
}

/*
 * print a message with an ip body
 */
printip(fd, mp, len)
	IcmpMsg *mp;
{
}

/*
 * send an ip datagram
 */
ipsend(fd, mp, len, src, dst)
	IcmpMsg *mp;
	u_long src;
	u_long dst;
{
	mp->ip.ip_dst = dst;
	mp->ip.ip_src = src;
	mp->ip.ip_len = len;
	mp->ip.ip_hl = sizeof(struct ip)>>2;
	mp->ip.ip_off = 0;
	mp->ip.ip_ttl = 255;
	mp->ip.ip_p = 1;
	cksum(mp, len);
	return write(fd, mp, len)!=len ? -1 : 0;
}

/*
 * echo a datagram
 */
echo(fd, mp, len)
	IcmpMsg *mp;
{
	mp->type = 0;
	if(ipsend(fd, mp, len, mp->ip.ip_dst, mp->ip.ip_src) < 0)
		perror("sending echo");
}

usage(name)
	char *name;
{
	fprint(2, "usage: %s [-d] [-m mount-pt]\n", name);
	exit(1);
}

/*
 * internet checksum routine
 */
cksum(mp, len)
	IcmpMsg *mp;
{
	u_char *cp;
	u_short *sp;
	u_long sum;

	sp = (u_short *)&mp->type;
	len -= sizeof(struct ip);
	mp->cksum[0] = mp->cksum[1] = 0;
	for(sum = 0; len >=2; len-=2)
		sum += *sp++;
	if(len){
		cp = (u_char *)sp;
		sum += *cp;
	}
	while(sum & 0xffff0000)
		sum = (sum & 0xffff) + (sum>>16);
	sum ^= 0xffff;
	mp->cksum[1] = sum>>8;
	mp->cksum[0] = sum;
}
