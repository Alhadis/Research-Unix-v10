/*
**	Initialise structures for Channel talking
*/

#ifndef Blit
#include	<signal.h>
#endif
#include	"pconfig.h"
#include	"proto.h"
#include	"packets.h"
#define	DECLARE				/* Define statistics array here */
#include	"pstats.h"


Pch_p		pconvsend;		/* Pointer to end of pconvs */
Pbyte		pseqtable[2*SEQMOD];	/* Table of sequence numbers */



int
pinit(channels)
register int	channels;
{
	register int	i;
	register Pch_p pcp;	/*SFBOTCH*/

#	ifndef	Blit
	if ( channels > MAXPCHAN )
		return(-1);
#	endif

	if ( Pscanrate == 0 || Prtimeout == 0 || Pxtimeout == 0 ) {
		Pscanrate = PSCANRATE;
		Prtimeout = PRTIMEOUT;
		Pxtimeout = PXTIMEOUT;
	}

	pconvsend = &pconvs[channels];

	while ( channels-- > 0 ) {
		/*SFBOTCHregister Pch_p*/	pcp = &pconvs[channels];

		pcp->nextpkt = pcp->pkts;
		pcp->freepkts = NPCBUFS;

		for ( i = 0 ; i < NPCBUFS ; i++) {

			pcp->pkts[i].state = PX_NULL;
			pcp->pkts[i].timo = 0;
			pcp->pkts[i].pkt.header = 0;
			pcp->pkts[i].pkt.dsize = 0;
		}
	}

	for ( i = 0 ; i < 2 * SEQMOD ; i++)
		pseqtable[i] = i % SEQMOD;

#	ifndef	Blit
	signal(SIGALRM, ptimeout);
	Ptflag = 0;
#	endif
	return(0);
}



#ifdef	PDEBUG

FILE *	ptracefd	 = stderr;

void
ptracepkt(pkp, s)
	Pkt_p		pkp;
	char	*		s;
{
	register Pbyte *ucp;
	register int	size;

	fprintf(ptracefd, "tracepkt: %s -- ", s);
	if ( (size = pkp->dsize) > 10 )
		size = 10;
	size += 2 + EDSIZE;
	for ( ucp = (Pbyte * )pkp ; size--; )
		fprintf(ptracefd, "<%o>", *ucp++);
	fprintf(ptracefd, "\n");
}



static struct Pktstate log[PKTHIST];
static Pks_p		inlog	 = log;
static Pks_p		outlog	 = log;
static short	logflag;

void
plogpkt(pkp, ident)
	Pkt_p		pkp;
	unsigned char	ident;
{
	inlog->pkt = *pkp;
	inlog->state = ident;
	if ( ++inlog >= &log[PKTHIST] )
		inlog = log;
	if ( logflag ) {
		outlog = inlog;
		logflag = 0;
	}
	if ( inlog == outlog )
		logflag++;
}



void
pdumphist(s)
	char	*	s;
{
	fprintf(ptracefd, "\nDumphist: %s\n", s);
	while ( outlog != inlog || logflag ) {
		logflag = 0;
		ptracepkt(&outlog->pkt, outlog->state == PLOGIN ? "recv" : "xmit");
		if ( ++outlog >= &log[PKTHIST] )
			outlog = log;
	}
	fflush(ptracefd);
}


#endif
