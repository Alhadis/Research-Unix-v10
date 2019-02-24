#include <signal.h>
#include <stdio.h>
#include "proto.h"

struct Packet{
	char	packet[MAXPKTSIZE];
	short	size;
	short	timo;
	short	state;
};

typedef struct Packet *	Pkp_t;

struct Packet	packets[NPBUFS];
int		ptype;
char		xseq;
char		freepkts;
char		nulls[MAXPKTSIZE-PKTHDRSIZE];
short		xtimo;

enum{
	badack, rack, unkack, skack, xpkts, timeout, dsize, speed, nstats
};

#define	BADACK		(int)badack
#define	RACK		(int)rack
#define	UNKACK		(int)unkack
#define	SKACK		(int)skack
#define	XPKTS		(int)xpkts
#define	TIMEOUT		(int)timeout
#define	DSIZE		(int)dsize
#define	SPEED		(int)speed
#define	NSTATS		(int)nstats

struct{
	char	*desc;
	long	count;
} stats[NSTATS]={
	{"unrecognised ack"},
	{"acks received"},
	{"unknown ack"},
	{"packets accepted by skipped sequence ack"},
	{"packets transmitted"},
	{"packets retransmitted by timeout"},
	{"max packet data size"},
	{"bytes/sec."},
};

#define	STATS(A)	stats[A].count++

void
pinit(lspeed, maxpktdsize, aptype)
{
	freepkts = NPBUFS;
	xtimo = (NPBUFS*(PKTHDRSIZE+PKTCRCSIZE+PKTASIZE+maxpktdsize)+lspeed-1)/lspeed+2;
	if((ptype=aptype) == ACKON)
		ptimeout();
	stats[DSIZE].count = maxpktdsize;
	stats[SPEED].count = lspeed;
}

void
precv(c)
	char		c;
{
	register Pkp_t	pkp;
	register char	pseq, seq = c & SEQMASK;
	register int	hit = 0;

	if((c&PTYP) != ptype){
		STATS(BADACK);
		return;
	}
	for (pkp = packets; pkp < &packets[NPBUFS]; pkp++)
		if(pkp->state == WAIT)
		{	pseq = (pkp->packet[0])&SEQMASK;
			if (seq == pseq)
			{	STATS(RACK);
				pkp->state = OK;
				freepkts++;
				hit++;
			} else
			if ((seq > pseq && seq < pseq+NPBUFS)
			||  (seq < pseq && seq+SEQMOD < pseq+NPBUFS))
			{	STATS(SKACK);
				pkp->state = OK;
				freepkts++;
				hit++;

		}	}
	if (!hit)
		STATS(UNKACK);
}

int
psend(bufp, count)
	char *		bufp;
	register int	count;
{
	register Pkp_t	pkp;
	register int	i;

	if(ptype == ACKON){
		for(pkp=(Pkp_t)0, i=0; i<NPBUFS; i++ )
			if(packets[i].state != WAIT){
				if(pkp == (Pkp_t)0){
					pkp = &packets[i];
					pkp->state = WAIT;
					pkp->timo = xtimo;
					freepkts--;
				}
			}
		if(pkp == (Pkp_t)0)
			return -1;
	} else
		pkp = packets;
	swab(bufp, &pkp->packet[PKTHDRSIZE], count);
	pkp->packet[0] = ptype | ((xseq++)&SEQMASK);
	pkp->packet[1] = count;
	count += PKTHDRSIZE;
	if(ptype != NOCRC){
		(void)crc(pkp->packet, count);
		count += PKTCRCSIZE;
	}
	pkp->size = count;
	Write(pkp->packet, count);
	STATS(XPKTS);
	return 0;
}

ptimeout()
{	register Pkp_t	pkp;

	(void)signal(SIGALRM, SIG_IGN);
	for (pkp = packets; pkp < &packets[NPBUFS]; pkp++)
		if (pkp->state == WAIT && --pkp->timo <= 0){
			STATS(TIMEOUT);
			pkp->timo = xtimo;
			Write(nulls, pkp->size-PKTHDRSIZE);
			Write(pkp->packet, pkp->size);
		}
	(void)signal(SIGALRM, ptimeout);
}

void
pstats(fd)
	FILE *	fd;
{
	register int	i;
	register int	count = 0;

	if (fd != NULL)
	for(i=0; i<NSTATS; i++ )
		if(stats[i].count){
			if(count++ == 0)
				fprintf(fd, "\nStatistics:\n");
			fprintf(fd, "\t%ld %s\n", (long)stats[i].count, stats[i].desc);
		}
}
