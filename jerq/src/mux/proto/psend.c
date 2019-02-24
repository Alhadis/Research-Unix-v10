/*
**	Send data to channel
**
**	Assumes count <= MAXPKTDSIZE
**	      & channel <= NLAYERS
**
**	Returns -1 if output queue full,
**		else value of Pxfunc().
*/

#include	"pconfig.h"
#include	"proto.h"
#include	"packets.h"
#include	"pstats.h"

/* extern int	crc(); */


int
#ifndef	Blit
psend(channel, bufp, count)
#else
psend(channel, bufp, count, type)
#endif
	int		channel;
#	ifdef	vax
	char *		bufp;
#	else
	register char *	bufp;
#	endif
	int		count;
#	ifdef	Blit
	char		type;
#	endif
{
	register int	i;
#	ifndef	vax
	register Pbyte *cp;
#	endif
	register Pkt_p	pkp;				/* WARNING *** used as r10 in "asm" below */
	register Pch_p	pcp = &pconvs[channel];
	register Pks_p	psp;
#	ifdef	Blit
	register int	x = spl1();
#	endif

	pcp->freepkts = 0;

	for ( pkp = 0, psp = pcp->nextpkt, i = NPCBUFS ; i-- ; )
	{
		if ( psp->state != PX_WAIT )
			if ( pkp )
				pcp->freepkts++;
			else
			{
				pkp = &psp->pkt;
				psp->state = PX_WAIT;
				psp->timo = Pxtimeout;
				if ( !Ptflag )
				{
					Ptflag++;
#					ifndef	Blit
					(void)alarm(Pscanrate);
#					endif
				}
			}
		if ( ++psp >= &pcp->pkts[NPCBUFS] )
			psp = pcp->pkts;
	}
	if ( pkp == 0 )
	{
#		ifdef	Blit
		splx(x);
#		endif
		return(-1);
	}

	pkp->header = P_PTYPE;
	pkp->header |= (channel & (MAXPCHAN-1)) << SBITS;
	pkp->header |= (pcp->xseq++ & (SEQMOD-1));

#	ifdef	Blit
	splx(x);
	cp = pkp->data;
	*cp++ = type;
	if ( i = count )
		do *cp++ = *bufp++; while ( --i );
	count++;
#	else
#		ifdef	vax
	{asm("	movc3	12(ap),*8(ap),2(r10)	");}	/* WARNING *** assumes "pkp" in r10 */
#		else
	for ( i = count, cp = pkp->data ; i-- ; )
		*cp++ = *bufp++;
#		endif
#	endif
#	ifdef	PSTATISTICS
	pstats[PS_XBYTES].count += count;
#	endif

	pkp->dsize = count;
	count += 2;		/* if bit fields were independent */

	(void)crc((Pbyte *)pkp, count);
	count += EDSIZE;
	((Pks_p)pkp)->size = count;

	plogpkt(pkp, PLOGOUT);
	return Pxfunc(pkp, count);
}
