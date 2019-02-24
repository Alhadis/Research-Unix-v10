#ifdef	XT
#ifndef	NPCBUFS
#include	"sys/xtproto.h"
#endif
#define	freepkts	outen
#define	user		flags
#else

/*
**	Definition of a structure to hold status information
**	for a conversation with a channel.
*/

#define	NPCBUFS		2			/* Double buffered protocol */

struct Pktstate
{
	struct Packet	pkt;			/* The packet */
	short		timo;			/* Timeout count */
	unsigned char	state;			/* Protocol state */
	unsigned char	size;			/* Packet size */
};

typedef struct Pktstate	*Pks_p;

struct Pchannel
{
	struct Pktstate	pkts[NPCBUFS];		/* The packets */
	Pks_p		nextpkt;		/* Next packet to be acknowledged */
	unsigned char	cdata[SEQMOD];		/* Remember control data */
	unsigned char	rseq;			/* Next receive sequence number */
	unsigned char	xseq;			/* Next transmit sequence number */
	char		freepkts;		/* Number of free buffers */
	char		user;			/* Spare byte for users */
};

typedef struct Pchannel	*	Pch_p;

/**	Transmit packet states	**/

enum pxenum /* required to have name for m32 sgs */ {	px_null, px_wait, px_ok	};

#define	PX_NULL		(int)px_null		/* Empty packet */
#define	PX_WAIT		(int)px_wait		/* Packet awaiting acknowledgement */
#define	PX_OK		(int)px_ok		/* Packet has been acknowledged */

/**	Receive packet states	**/

enum prenum /* required to have name for m32 sgs */ { pr_null, pr_size, pr_data };

#define	PR_NULL		(int)pr_null		/* New packet expected */
#define	PR_SIZE		(int)pr_size		/* Size byte next */
#define	PR_DATA		(int)pr_data		/* Receiving data */
#endif

/*
**	Interface routines
*/

extern int		psend();		/* Send data to channel */
extern void		precv();		/* Receive data/control from channel */
extern void		ptimeout();		/* Catch alarm timeouts */
extern int		pinit();		/* Initialise conversations */

/*
**	Pointers to externally declared data and functions
*/

extern struct Pchannel	pconvs[];		/* Array of conversations */
extern Pch_p		pconvsend;		/* Pointer to end of pconvs */

/*
**	Receiver data
*/

extern struct Pktstate	precvpkt;		/* Current receive packet */
extern Pbyte		pseqtable[];		/* Table of ordered sequence numbers */

#ifdef	EBUG
/*
**	Debugging
*/

#define	PDEBUG		1
#ifndef	PSTATISTICS
#define	PSTATISTICS	1
#endif
#ifndef	PSTATSDESC
#define	PSTATSDESC	1
#endif
#define	PKTHIST		40
#define	PLOGIN		0
#define	PLOGOUT		1

#include		<stdio.h>

extern FILE *		ptracefd;
extern void		ptracepkt();
extern void		plogpkt();
extern void		pdumphist();

#define	ptrace(A)	fprintf(ptracefd,"trace: %s\n",A)

#else

#define	ptrace(A)
#define	ptracepkt(A,B)
#define	plogpkt(A,B)
#define	pdumphist(A)

#endif
