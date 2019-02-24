/*
 *	Packet format:
 *
 *	0x80^Seq | Size | 4 bytes of address | 0 or more data bytes | CRC1 | CRC2
 *
 *	where PKTASIZE <= Size <= (PKTASIZE+MAXPKTDSIZE).
 *
 *	If Size == PKTASIZE then the address is the start address.
 *	If Size > PKTASIZE then load data contiguously at the address.
 *
 *	Acknowledgements consist of the first byte
 *	from each correctly received packet (Seq)
 *	acks apply to all packets sent, up to and including the one acked.
 *	Timeouts cause retransmission of all unacknowledged packets
 */

#define	MAXPKTSIZE	128		/* Efficient size for system */
#define	PKTHDRSIZE	2		/* Seq + Size */
#define	PKTCRCSIZE	2		/* CRC 16 */
#define	PKTASIZE	4		/* sizeof(long) on MC68000 */
#define	MAXPKTDSIZE	MAXPKTSIZE-(PKTHDRSIZE+PKTCRCSIZE+PKTASIZE)

#define	PTYP		0xc0		/* Masked with Seq to guarantee non-zero */
#define	ACKON		0x80		/* Ack expected for this packet */
#define	ACKOFF		0xc0		/* No ack expected, but remember errors */
#define	NOCRC		0x40		/* No error detection on packet */
#define	SEQMASK		(~PTYP)
#define SEQMOD		0x40		/* SEQMASK+1 */

#define	WAIT	1
#define	OK	2

#define NPBUFS	2

extern char freepkts;			/* 0 or 1 */
