/*
 * magic numbers of line disciplines
 */

int	tty_ld	= 0;	/* tty processing */
int	cdkp_ld	= 1;	/* URP protocol -- character mode (same as 1) */
int	rdk_ld	= 2;	/* `raw' datakit -- really CMC supervisor protocol */
int	mesg_ld	= 4;	/* data message protocol */
int	dkp_ld	= 5;	/* URP protocol -- block mode */
int	ntty_ld	= 6;	/* new (UCB) tty processing */
int	buf_ld	= 7;	/* buffer up characters till timeout */
int	rmesg_ld= 9;	/* reverse message processing */
int	ip_ld	= 10;	/* IP - push on net interfaces (il, ec, ...) */
int	tcp_ld	= 11;	/* TCP (inet) - only one instance, on /dev/ip6 */
int	arp_ld	= 13;	/* Ethernet address resolution - on net interfaces */
int	udp_ld	= 14;	/* UDP (inet) - only one instance, on /dev/ip */
int	conn_ld	= 18;	/* Connection line discipline */
int	uxp_ld	= 19;	/* unix common control protocol */
