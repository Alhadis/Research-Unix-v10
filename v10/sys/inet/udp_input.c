/*	udp_input.c	6.1	83/07/29	*/

#include "sys/param.h"
#include "sys/stream.h"

#include "sys/inet/in.h"
#include "sys/inet/ip.h"
#include "sys/inet/ip_var.h"
#include "sys/inet/udp.h"
#include "sys/inet/udp_var.h"

int	udpcksum = 0;

/*
 * UDP input routine
 */
udp_input(bp)
struct block *bp;
{
	register struct udpiphdr *ui;
	register struct block *head;
	int len, ulen;

	/*
	 * Get IP and UDP header together in first block.
	 * Note: IP leaves IP header in first block.
	 */
	ui = (struct udpiphdr *)bp->rptr;
/*
 * must strip options so udpiphdr is in the right place
 */
	if (((struct ip *)ui)->ip_hl > (sizeof (struct ip) >> 2))
		ip_stripoptions(bp, (struct block *)0);

	if (BLEN(bp) < sizeof (struct udpiphdr)) {
		if ((bp = bp_pullup(bp, sizeof (struct udpiphdr))) == 0) {
			udpstat.udps_hdrops++;
			return;
		}
		ui = (struct udpiphdr *)bp->rptr;
	}

	/*
	 * Checksum extended UDP header and data.
	 */
	ulen = ((struct ip *)ui)->ip_len;
	len = sizeof (struct ip) + ulen;
	ui->ui_src = htonl(ui->ui_src);
	ui->ui_dst = htonl(ui->ui_dst);
	if (udpcksum) {
		ui->ui_next = 0;
		ui->ui_x1 = 0;
		ui->ui_len = htons((u_short)ulen);
		if (ui->ui_sum = in_cksum(bp, len)) {
			printf("udp_cksum: sum %x len %d src %x\n", ui->ui_sum,
				len, ui->ui_src);
			udpstat.udps_badsum++;
			bp_free(bp);
			return;
		}
	}
	udpstat.udps_ipackets++;

	/*
	 * Drop UDP and IP headers.
	 */
	bp->rptr += sizeof(struct udpiphdr);

	/*
	 * adjust size to reflect length 
	 * find end of queue and adjust wptr accordingly
	 */
	head = bp;
	len = 0;
/*
 * ulen is length of udphdr+data. since we've already shortened first buffer
 * by sizeof(udpiphdr), shorten ulen by sizeof(udphdr)
 */
	ulen -= sizeof(struct udphdr);
	while(bp) {
		len += bp->wptr - bp->rptr;
		if (len > ulen) {
			bp->wptr -= len-ulen;
			bp_free(bp->next);
			bp->next = 0;
		}
		bp = bp->next;
	}

	/*
	 * Convert UDP protocol specific fields to host format.
	 */
	ui->ui_src = ntohl(ui->ui_src);
	ui->ui_dst = ntohl(ui->ui_dst);
	ui->ui_sport = ntohs(ui->ui_sport);
	ui->ui_dport = ntohs(ui->ui_dport);

	udpdrint(head, ui);
}
