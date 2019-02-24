/*	udp_output.c	6.1	83/07/29	*/

#include "sys/param.h"
#include "sys/stream.h"

#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/udp.h"
#include "sys/inet/udp_var.h"

#define	IPPROTO_UDP	17
#define	UDP_DATA_LEN	(1500-sizeof(struct udpiphdr))

/*
 * send out list data,
 * as a UDP packet
 * allocate, prepend, and fill in UDP headers here
 */
udp_output(data, udp)
register struct block *data;
register struct udp *udp;
{
	register struct block *head;
	register struct udpiphdr *ui;
	register int len;

	len = bp_len(data);
	if ((head = allocb(sizeof(struct udpiphdr))) == NULL) {
		bp_free(data);
		return;
	}
if (head == data) panic("udp_output");
	head->wptr += sizeof(struct udpiphdr);
	head->next = data;
	/*
	 * Fill in block with extended UDP header
	 * and addresses and length put into network format.
	 */
	ui = (struct udpiphdr *)head->rptr;
	ui->ui_next = 0;
	ui->ui_bp = 0;
	ui->ui_x1 = 0;
	ui->ui_pr = IPPROTO_UDP;
	ui->ui_len = (u_short)len + sizeof (struct udphdr);
	ui->ui_len = htons((u_short)(ui->ui_len));
	ui->ui_src = htonl(udp->src);
	ui->ui_dst = htonl(udp->dst);
	ui->ui_sport = htons(udp->sport);
	ui->ui_dport = htons(udp->dport);
	ui->ui_ulen = ui->ui_len;

	/* Stuff checksum and output datagram. */
	ui->ui_sum = 0;
	if ((ui->ui_sum = in_cksum(head, sizeof(struct udpiphdr)+len)) == 0)
		ui->ui_sum = -1;


	/* Put length and timeout time into the `real' ip header. */
	((struct ip *)ui)->ip_len = sizeof(struct udpiphdr)+len;
	((struct ip *)ui)->ip_ttl = MAXTTL;

	/* Ip expects internet addresses in host order. */
	ui->ui_src = ntohl(ui->ui_src);
	ui->ui_dst = ntohl(ui->ui_dst);
	udp_ldout(head);
}
