#include	<jerq.h>
#include	"../comm.h"
#include	"term.h"

struct pkt pkt;
static pksize;

#define		PKTNEW		0
#define		PKTSTR		1
#define		PKTSHORT	2
#define		PKTFS1		3
#define		PKTTNP		4
#define		PKTOFF		5
#define		PKTSIZE		6
#define		PKTDD		7
#define		PKTSH1		8
#define		PKTDATA		9
#define		PKTDONE		10

buildpkt()
{
	register k;
	static state = PKTNEW, nstate;
	static short *shortp;

	while((state != PKTDONE) && ((k = rcvchar()) != -1))
	{
	swtch:
		switch(state)
		{
		case PKTNEW:
			switch(pkt.type = k)
			{
			case TEXIT:
			case TTERM:
				state = PKTDONE;
				break;
			case TFONT:
				state = PKTSTR;
				nstate = PKTFS1;
				pkt.next = pkt.data;
				break;
			case TTEXT:
				state = PKTSHORT;
				nstate = PKTTNP;
				shortp = &pkt.page;
				break;
			}
			break;
		case PKTFS1:
			state = PKTSTR;
			nstate = PKTDONE;
			/* pkt.next already okay */
			goto swtch;
		case PKTTNP:
			state = PKTSHORT;
			nstate = PKTOFF;
			shortp = &pkt.nextpage;
			goto swtch;
		case PKTOFF:
			state = PKTSHORT;
			nstate = PKTSIZE;
			shortp = &pkt.offset;
			goto swtch;
		case PKTSIZE:
			state = PKTSHORT;
			nstate = PKTDD;
			shortp = &pkt.size;
			goto swtch;
		case PKTDD:
			state = PKTDATA;
			pkt.next = pkt.data;
			pksize = pkt.size;
			goto swtch;
		case PKTSTR:
			if((*(pkt.next++) = k) == 0)
				state = nstate;
			break;
		case PKTSHORT:
			*shortp = k<<8;
			state = PKTSH1;
			break;
		case PKTSH1:
			*shortp |= k&0xFF;
			state = nstate;
			break;
		case PKTDATA:
			*(pkt.next++) = k;
			if(--pksize == 0)
				state = PKTDONE;
			break;
		}
	}
	if(state == PKTDONE)
	{
		state = PKTNEW;
		return(1);
	}
	else
		return(0);
}
