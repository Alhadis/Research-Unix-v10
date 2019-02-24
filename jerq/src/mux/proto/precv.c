/*
**	Process receiver buffers
*/

#include	"pconfig.h"
#include	"proto.h"
#include	"packets.h"
#include	"pstats.h"


/* extern int	crc(); */
void	Reply(), Control();
int	Retry();

struct Pktstate precvpkt;
/* static */ Pkt_p	Pkp;
/* static */ char *	pbufp;
/* static */ char *	Sbufp;
/* static */ short	dcount;
/* static */ short	Scount;

#define	Pkt	precvpkt.pkt
#define	State	precvpkt.state
#define	Timo	precvpkt.timo
#define	Header	Pkt.header
#define	Dsize	Pkt.dsize
#define	Data	Pkp->data

#define	Ptyp	(Pkt.header & P_PTYPE)
#define	Cntl	(Pkt.header & P_CNTL)
#define	Channel	P_channel(Pkt.header)
#define	Seq	P_seq(Pkt.header)
#define	Nextseq	pconvs[Channel].rseq



void
#ifndef	Blit
precv(bufp, count)
	register char *	bufp;
	register int	count;
{
	int		haveheader = 0;

	while ( count-- )
#else
precv(c)
	char		c;
#endif
	{
		switch ( State )
		{
		 case PR_NULL:
#			ifndef	Blit
			Pkp = (Pkt_p)bufp;
			haveheader++;
			Header = *bufp++;
#			else
			Header = c;
#			endif
			if ( !Ptyp )
			{
				PSTATS(PS_BADHDR);
				ptracepkt(Pkp, pstats[PS_BADHDR].descp);
				break;
			}
			Timo = Prtimeout;
			if ( !Ptflag )
			{
				Ptflag++;
#				ifndef	Blit
				(void)alarm(Pscanrate);
#				endif
			}
			State = PR_SIZE;
#			ifndef	Blit
			continue;
#			else
			return;
#			endif

		 case PR_SIZE:
#			ifndef	Blit
			Dsize = *bufp++;
#			else
			Dsize = c;
#			endif
			Scount = Dsize;
			if ( Scount > MAXPKTDSIZE )
			{
				PSTATS(PS_BADSIZE);
				ptracepkt((haveheader?Pkp:&Pkt), pstats[PS_BADSIZE].descp);
				break;
			}
			dcount = Scount + EDSIZE;
#			ifndef	Blit
			if ( dcount <= count && haveheader )
			{
				/* Don't move data */
				Sbufp = bufp;
				bufp += dcount;
				count -= dcount;
				goto check;
			}
#			endif
			Pkp = &Pkt;
			Sbufp = (char *)Data;
			pbufp = Sbufp;
			State = PR_DATA;
#			ifndef	Blit
			continue;
#			else
			return;
#			endif

		 case PR_DATA:
#			ifndef	Blit
			*pbufp++ = *bufp++;
#			else
			*pbufp++ = c;
#			endif
			if ( --dcount > 0 )
#				ifndef	Blit
				continue;
#				else
				return;
#				endif

check:
			/** Now at CRC **/

			plogpkt(Pkp, PLOGIN);

			if ( crc((uchar *)Pkp, (int)(Scount+2)) ) /* if bit fields were independent */
			{
				PSTATS(PS_BADCRC);
				ptracepkt(Pkp, pstats[PS_BADCRC].descp);
			}
			else
			{
				if ( Cntl )
					Control();
				else
				{
					Pcdata = (uchar)0;
					if ( Seq == Nextseq )
					{
						if ( (*Prfuncp)(Channel, Sbufp, Scount) )
						{
							PSTATS(PS_BUSY);
							/* Better to let this timeout,
							** as a following sequence will
							** generate a second retransmission
							*/
							Reply(NAK);
						}
						else
						{
							Nextseq = (Nextseq+1) & (SEQMOD-1);	/* NB rseq is a byte, not a bit field, for efficiency */
							PSTATS(PS_RPKTS);
#							ifdef	PSTATISTICS
							pstats[PS_RBYTES].count += Scount;
#							endif
							pconvs[Channel].cdata[Seq] = Pcdata;
							Reply(ACK);
						}
					}
					else
					if ( Retry() )
					{
						PSTATS(PS_RDUP);
						Reply(ACK);
					}
					else
					{
						PSTATS(PS_OUTSEQ);
						Reply(NAK);
						pdumphist(pstats[PS_OUTSEQ].descp);
					}
				}
			}
		}

		Timo = 0;
		State = PR_NULL;
#ifndef	Blit
	}
#endif
}



/*
**	Deal with control packet
*/

void
Control()
{
	register Pch_p	pcp = &pconvs[Channel];
	register Pks_p	psp = pcp->nextpkt;
	register Pbyte *lastseqp = &pseqtable[Seq+SEQMOD];
	register Pbyte *seqp = lastseqp - (NPCBUFS-1);
	register int	hit = 0;
#	ifdef	Blit
	register int	x = spl1();
#	endif

	if ( Scount == 0 )
		goto ack;

	switch ( Data[0] )
	{
	 case ACK:
		/** This and all lesser sequenced packets ok **/
ack:
		do
		{
			if ( *seqp == P_seq(psp->pkt.header) )
			{
				if ( psp->state != PX_WAIT )
				{
#					if	PDEBUG == 1 || PSTATISTICS == 1
					if ( psp->state != PX_OK )
					{
						PSTATS(PS_BADXST);
						pdumphist(pstats[PS_BADXST].descp);
					}
#					endif
				}
				else
				{
					psp->state = PX_OK;
					psp->timo = 0;
					hit++;
				}
				if ( ++psp >= &pcp->pkts[NPCBUFS] )
					psp = pcp->pkts;
			}
		} while
			( ++seqp <= lastseqp );
		if ( hit )
		{
			pcp->nextpkt = psp;
			pcp->freepkts += hit;
#			ifdef	PSTATISTICS
			pstats[PS_XPKTS].count += hit;
			if ( hit > 1 )
				PSTATS(PS_LOSTACK);
#			endif
			break;
		}
#		if	PDEBUG == 1 || PSTATISTICS == 1
		PSTATS(PS_BADACK);
		pdumphist(pstats[PS_BADACK].descp);
#		endif
#		ifdef	Blit
		splx(x);
#		endif
		return;

	 case NAK:
		/** Retransmit this and all lesser sequenced packets **/

		do
		{
			if ( *seqp == P_seq(psp->pkt.header) )
			{
				if ( psp->state != PX_WAIT )
				{
#					if	PSTATISTICS == 1 || PDEBUG == 1
					if ( psp->state != PX_OK )
					{
						PSTATS(PS_BADXST);
						pdumphist(pstats[PS_BADXST].descp);
					}
#					endif
				}
				else
				{
					psp->timo = Pxtimeout;
					(void)Pxfunc(&psp->pkt, psp->size);
					PSTATS(PS_NAKPKT);
					hit++;
					plogpkt(&psp->pkt, PLOGOUT);
				}
				if ( ++psp >= &pcp->pkts[NPCBUFS] )
					psp = pcp->pkts;
			}
		} while
			( ++seqp <= lastseqp );
		if ( !hit )
		{
			PSTATS(PS_BADNAK);
			pdumphist(pstats[PS_BADNAK].descp);
#			ifdef	Blit
			splx(x);
#			endif
			return;
		}
		break;

	 case PCDATA:
		break;

	 default:
		PSTATS(PS_BADCNTL);
		ptracepkt(Pkp, pstats[PS_BADCNTL].descp);
#		ifdef	Blit
		splx(x);
#		endif
		return;
	}
#	ifdef	Blit
	splx(x);
#	endif

	if ( --Scount > 0 )
	{
		(*Prcfuncp)(Channel, Sbufp+1, Scount);
#		ifdef	PSTATISTICS
		pstats[PS_RCBYTES].count += Scount;
#		endif
	}
}



/*
**	Reply to good packet
*/

void
Reply(ctl)
	Pbyte		ctl;
{
	register int	count;

	Pkt.header |= P_CNTL;
	if ( Pcdata )
	{
		PSTATS(PS_XCBYTES);
		count = 2;
		Pkt.data[1] = Pcdata;
		Pkt.data[0] = ctl;
	}
	else
	if ( ctl != ACK )
	{
		count = 1;
		Pkt.data[0] = ctl;
	}
	else
		count = 0;
	Dsize = count;
	count += 2;	/* if bit fields were independent		*/

	(void)crc((Pbyte *)&Pkt, count);
	count += EDSIZE;

	(void)Pxfunc(&Pkt, count);
	plogpkt(&Pkt, PLOGOUT);
}



/*
**	Non trivial sequence number validation:
**	 is this a valid retransmission?
*/

int
Retry()
{
	register Pbyte *lastseqp = &pseqtable[Nextseq+SEQMOD-1];
	register Pbyte *seqp = lastseqp - (NPCBUFS-1);

	do
		if ( *seqp == Seq )
		{
			Pcdata = pconvs[Channel].cdata[Seq];
			return 1;
		}
	while
		( ++seqp <= lastseqp );

	return 0;
}
