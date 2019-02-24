/*
**	Process timeouts for packets
*/

#include	"pconfig.h"
#include	"proto.h"
#include	"packets.h"
#include	"pstats.h"

void
ptimeout(sig)
	int		sig;
{
	register Pch_p	pcp;
	register Pks_p	psp;
	register int	i;
	register int	retrys;

#	ifndef	Blit
	signal(sig, ptimeout);
#	endif
	Ptflag = 0;

	if ( precvpkt.timo > 0 && ++Ptflag && (precvpkt.timo -= Pscanrate) <= 0 )
	{
		precvpkt.state = PR_NULL;
		ptrace("RECV TIMEOUT");
	}

	for ( pcp = pconvs, retrys = 0 ; pcp < pconvsend && retrys < MAXTIMORETRYS ; pcp++ )
		for ( psp = pcp->nextpkt, i = NPCBUFS ; i-- ; )
		{
			if ( psp->timo > 0 && ++Ptflag && (psp->timo -= Pscanrate) <= 0 )
			{
				ptrace("XMIT TIMEOUT");
				psp->timo = Pxtimeout;
				Pxfunc(&psp->pkt, psp->size);
				PSTATS(PS_TIMOPKT);
				plogpkt(&psp->pkt, PLOGOUT);
				ptrace("END TIMEOUT");
				if ( ++retrys >= MAXTIMORETRYS )
					break;
			}
			if ( ++psp >= &pcp->pkts[NPCBUFS] )
				psp = pcp->pkts;
		}

#	ifndef	Blit
	if ( Ptflag )
		(void)alarm(Pscanrate);
#	endif
}
