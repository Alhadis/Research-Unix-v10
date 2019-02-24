#include <sys/types.h>
#include <sys/inet/udp_user.h>
#include <errno.h>

extern int errno;

udp_connect(sport, dhost, dport)
in_addr dhost;
udp_port sport, dport;
{
	return udp_open(UDPC_CONNECT, sport, dhost, dport, (struct udpreply *)0);
}

udp_listen(sport, reply)
udp_port sport;
struct udpreply *reply;
{
	return udp_open(UDPC_LISTEN, sport, 0L, 0, reply);
}

udp_datagram(sport)
udp_port sport;
{
	return udp_open(UDPC_DATAGRAM, sport, 0L, 0, (struct udpreply *)0);
}


udp_open(cmd, sport, dhost, dport, reply)
int cmd;
u_short sport, dport;
in_addr dhost;
struct udpreply *reply;
{
	int fd, n;
	struct udpuser uu;
	struct udpreply ur;
	char name[16];

	uu.cmd = cmd;
	uu.sport = sport;
	uu.dport = dport;
	uu.dst = dhost;

	for(n = 0; n < 100; n++){
		sprintf(name, "/dev/udp%02d", n);
		fd = open(name, 2);
		if(fd >= 0) {
			flabclr(fd);	/* for security unix */
			break;
		}
	}
	if (fd < 0)
		return -1;

	if (write(fd, (char *)&uu, sizeof(uu)) < 0) {
err:		close(fd);
		return -1;
	}
	if (read (fd, (char *)&ur, sizeof(ur)) != sizeof(ur))
		goto err;
	if (ur.reply != UDPR_OK)
		goto err;
	if (reply)
		*reply = ur;
	return fd;
}
