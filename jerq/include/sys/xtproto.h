/*
	@(#)  xtproto.h  version 2.1 (Blit) of 7/20/83 15:43:49
	Last Delta:  7/10/83 19:08:18 to /usr/jerq/sccs/src/xt/uts/sys/s.xtproto.h
*/

/*
**	Bx -- Blit packet protocol definition
*/

typedef	unsigned char	Pbyte;			/* The unit of communication */

#define	NPCBUFS		2			/* Double buffered protocol */
#define	MAXPCHAN	8			/* Maximum channel number */
#define	CHANBITS	3			/* Bits for channel number */
#define	CHANMASK	07			/* 2**CHANBITS - 1 */
/*
 *	MAXPKTDSIZE should be CLSIZE - sizeof(struct P_header) - EDSIZE
 */
#define	MAXPKTDSIZE	(32 * sizeof(Pbyte))	/* Maximum data part size */
#define	EDSIZE		(2 * sizeof(Pbyte))	/* Error detection part size */
#define	SEQMOD		8			/* Sequence number modulus */
#define	SEQBITS		3			/* Bits for sequence number */
#define	SEQMASK		07			/* 2**SEQBITS - 1 */

/*
**	Packet header
**	(if only bit fields in C were m/c independent, sigh...)
*/

#ifndef	u3b

struct P_header
{
#	ifdef	vax
#	ifdef	OLDMPX
	Pbyte		channel	:CHANBITS,	/* Channel number */
			seq	:SEQBITS,	/* Sequence number */
#	else	OLDMPX
	Pbyte		seq	:SEQBITS,	/* Sequence number */
			channel	:CHANBITS,	/* Channel number */
#	endif	OLDMPX
			cntl	:1,		/* TRUE if control packet */
			ptyp	:1;		/* Always 1 */
	Pbyte		dsize;			/* Size of data part */
#	endif	vax
#	ifdef	mc68000
	short		ptyp	:1,		/* Always 1 */
			cntl	:1,		/* TRUE if control packet */
			channel	:CHANBITS,	/* Channel number */
			seq	:SEQBITS,	/* Sequence number */
			dsize	:8;		/* Size of data part */
#	endif	mc68000
};

typedef	struct P_header	Ph_t;

/*
**	Packet definition for maximum sized packet
*/

struct Packet
{
	Ph_t		header;			/* Header part */
	Pbyte		data[MAXPKTDSIZE];	/* Data part */
	Pbyte		edb[EDSIZE];		/* Error detection part */
};

#else u3b

/*
**	Packet header (real size) 
*/

typedef short Ph_t;

/*
**	Packet definition for maximum sized packet
*/

struct Packet {
	Pbyte		header;			/* Packet into */
	Pbyte		dsize;			/* Size of data part */
	Pbyte		data[MAXPKTDSIZE];	/* Data part */
	Pbyte		edb[EDSIZE];		/* Error detection part */
};

/*
**	Macros to access fields of header byte
*/

#define SET_CNTL(x)		(x)|=0100
#define GET_CNTL(x)		(((x)>>6)&1)
#define GET_PTYP(x)		(((x)>>7)&1)
#define GET_SEQ(x)		((x)&7)
#define GET_CHAN(x)		(((x)>>3)&7)
#define MK_HDR(x,chan,seq)	(x)=0200|((chan)<<3)|(seq)

#endif u3b

typedef struct Packet *	Pkt_p;

/*
**	Control codes
*/

#define	PCDATA		(Pbyte)002		/* Data only control packet */
#define	ACK		(Pbyte)006		/* Last packet with same sequence ok and in sequence */
#define	NAK		(Pbyte)025		/* Last packet with same sequence received out of sequence */

/*
**	Definition of a structure to hold status information
**	for a conversation with a channel.
*/

struct Pktstate
{
	struct Packet	pkt;			/* The packet */
	short		timo;			/* Timeout count */
	unsigned char	state;			/* Protocol state */
	unsigned char	size;			/* Packet size */
};

typedef struct Pktstate *Pks_p;

struct Pchannel
{
	struct Pktstate	pkts[NPCBUFS];		/* The packets */
	Pks_p		nextpkt;		/* Next packet to be acknowledged */
	Pbyte		cdata[SEQMOD];		/* Remember transmitted control data */
	Pbyte		rseq	:SEQBITS;	/* Next receive sequence number */
	Pbyte		xseq	:SEQBITS;	/* Next transmit sequence number */
	char		outen;			/* Output packets enabled */
	char		flags;			/* Control flags */
#if XTDRIVER == 1
	char		channo;			/* This channel's number */
	char		link;			/* This channel's link */
#	ifdef u3b
	struct clist 	xoutq;			/* Processed output for channel */
	struct clist 	xinq;			/* Processed input for channel */
	char		esc;			/* saw "\" flag for tty input */
#	endif u3b
#endif
};

#define	XACK		1			/* Send ACK */
#define	XNAK		2			/* Send NAK */
#define	XCDATA		4			/* Send control data in ACK packet */

typedef struct Pchannel *Pch_p;

/**	Transmit packet states	**/

enum {	px_null, px_ready, px_wait, px_ok	};

#define	PX_NULL		(int)px_null		/* Empty packet */
#define	PX_READY	(int)px_ready		/* Full packet awaiting transmission */
#define	PX_WAIT		(int)px_wait		/* Packet awaiting acknowledgement */
#define	PX_OK		(int)px_ok		/* Packet has been acknowledged */

/**	Receive packet states	**/

enum { pr_null, pr_size, pr_data };

#define	PR_NULL		(int)pr_null		/* New packet expected */
#define	PR_SIZE		(int)pr_size		/* Size byte next */
#define	PR_DATA		(int)pr_data		/* Receiving data */
