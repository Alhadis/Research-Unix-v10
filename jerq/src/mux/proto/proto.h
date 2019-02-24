#ifdef	XT
#ifndef	MAXPCHAN
#include	"sys/xtproto.h"
#endif
#else
/*
**	Mpx -- Blit packet protocol definition
*/

typedef	unsigned char	Pbyte;			/* The unit of communication */

/*
**	Packet header
**	(if only bit fields in C were m/c independant, sigh...)
*/

#define	P_PTYPE		0x80			/* always set */
#define	P_CNTL		0x40			/* TRUE if control packet */
#define	MAXPCHAN	16			/* Maximum channel number */
#define	CBITS		4			/* Bits for channel number */
#define	SEQMOD		4			/* Sequence number modulus */
#define	SBITS		2			/* Bits for sequence number */

/* was 64 */
#define	MAXPKTDSIZE	(124 * sizeof(Pbyte))	/* Maximum data part size */
#define	EDSIZE		(2 * sizeof(Pbyte))	/* Error detection part size */

#define P_channel(b)	(((b)>>SBITS) & (MAXPCHAN-1))
#define P_seq(b)	((b) & (SEQMOD-1))

/*
**	Packet definition for maximum sized packet for transmission
*/

struct Packet
{
	Pbyte		header, dsize;		/* Header part */
	Pbyte		data[MAXPKTDSIZE];	/* Data part */
	Pbyte		edb[EDSIZE];		/* Error detection part */
};

typedef struct Packet *	Pkt_p;

/*
**	Control codes
*/

#define	ACK	(Pbyte)(006)	/* Last packet with same seq. ok and in seq. */
#define	NAK	(Pbyte)(025)	/* Last packet with same seq. rec'd out of seq. */
#define	PCDATA	(Pbyte)(002)	/* Data only control packet */
#endif
