/*
 * things users need to know to talk to /dev/udp*
 * open a free udp device, write a udpuser struct on it,
 * then wait for a udpreply.
 */

/* the following is defined in kernel .h's */
#ifndef KERNEL
#include <sys/inet/in.h>
#include <sys/inet/udp.h>
#endif

struct udpuser{
	int cmd;
	int sport, dport;
	in_addr dst;
};
#define UDPC_LISTEN	1
#define UDPC_CONNECT	2
#define UDPC_DATAGRAM	3

struct udpreply{
	int reply;
	/* for listen: */
	int dport;
	in_addr dst;
	int udpdev;	/* minor device # */
};
#define UDPR_OK		1
#define UDPR_SORRY	2

struct udpaddr{
	in_addr	host;
	int port;
};
