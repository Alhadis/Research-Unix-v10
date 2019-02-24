/*
**	Channel conversation statistics
*/

#ifdef	PSTATISTICS

struct Pstatistics
{
#	ifdef	PSTATSDESC
	char *	descp;
#	endif
	long	count;
};

enum
{
	 badhdr
	,badsize, badcrc
	,outseq, busy, rdup
	,lostack, badack, badnak
	,nakpkt, timopkt
	,badcntl, badxst
	,rpkts, xpkts
	,rbytes, xbytes
	,rcbytes, xcbytes
	,nstats
};

#define	PS_BADHDR	(int)badhdr		/* Header inconsistent */
#define	PS_BADSIZE	(int)badsize		/* Packet size too large */
#define	PS_BADCRC	(int)badcrc		/* CRC error */
#define	PS_OUTSEQ	(int)outseq		/* Packet out of sequence */
#define	PS_BUSY		(int)busy		/* Data receiver busy */
#define	PS_RDUP		(int)rdup		/* Duplicate packet received */
#define	PS_LOSTACK	(int)lostack		/* ACK for packet lost */
#define	PS_BADACK	(int)badack		/* ACK for non-existent packet */
#define	PS_BADNAK	(int)badnak		/* NAK for non-existent packet */
#define	PS_NAKPKT	(int)nakpkt		/* Retransmitted by NAK */
#define	PS_TIMOPKT	(int)timopkt		/* Retransmitted by timeout */
#define	PS_BADCNTL	(int)badcntl		/* Unrecognised control code */
#define	PS_BADXST	(int)badxst		/* State/acknowledge out of sync */
#define	PS_RPKTS	(int)rpkts		/* Packets received */
#define	PS_XPKTS	(int)xpkts		/* Packets transmitted */
#define	PS_RBYTES	(int)rbytes		/* Bytes received */
#define	PS_XBYTES	(int)xbytes		/* Bytes transmitted */
#define	PS_RCBYTES	(int)rcbytes		/* Control bytes received */
#define	PS_XCBYTES	(int)xcbytes		/* Control bytes transmitted */
#define	PS_NSTATS	(int)nstats

#ifndef	DECLARE
extern struct Pstatistics	pstats[PS_NSTATS];
#else
struct Pstatistics		pstats[PS_NSTATS]
#ifdef	PSTATSDESC
=
{
	 {"bad header"}
	,{"bad size"}
	,{"bad crc"}
	,{"out of sequence"}
	,{"data receiver busy"}
	,{"duplicate packets received"}
	,{"lost ack"}
	,{"bad ack"}
	,{"bad nak"}
	,{"retransmitted by NAK"}
	,{"retransmitted by timeout"}
	,{"bad control"}
	,{"bad xstate"}
	,{"packets received"}
	,{"packets transmitted"}
	,{"bytes received"}
	,{"bytes transmitted"}
	,{"control bytes received"}
	,{"control bytes transmitted"}
}
#endif
;
#endif

#define	PSTATS(A)	pstats[A].count++

#else

#define	PSTATS(A)

#endif
