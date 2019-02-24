#include <sys/param.h>
#include <sys/inet/ip.h>
#include <ipc.h>

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

typedef struct {
	struct ip ip;
	u_char type;
	u_char code;
	u_char cksum[2];
	u_char id[2];
	u_char seqno[2];
	u_char data[128];
} EchoMsg;

main(ac, av)
	int ac;
	char *av[];
{
	long addr, myaddr;
	int i, fd;

	if(ac!=2){
		fprint(2, "usage: %s host\n", av[0]);
		exit(1);
	}
	/*
	 *  translate his name into an address
	 */
	addr = in_address(av[1]);
	if(addr == 0){
		fprint(2, "internet address for %s unknown\n", av[1]);
		exit(1);
	}

	/*
	 *  find out my own name and get an address for it.  preferably
	 *  use an address for his network (how do we handle subnets?).
	 */
	myaddr = in_addronnet(me, addr, 0);
	if(myaddr == 0){
		addr = in_address(me);
		if(addr == 0){
			fprint(2, "internet address for %s unknown\n", me);
			exit(1);
		}
	}

	/*
	 *  get a connection to the icmp manager
	 */
	fd = ipcopen("/cs/icmp", "");

	/*
	 *  send 15 messages one second apart
	 */
	for(i=0; i<15; i++){
		switch(select(NOFILE, &fds, 0, 1000)){
		}
	}
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

