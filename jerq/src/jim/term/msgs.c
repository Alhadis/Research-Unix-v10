#include <jerq.h>
#include "msgs.h"
/*
 * Jerq-side message code for communicating with Unix.
 */
Message m;
send(f, op, posn, n, d)
	unsigned f, n;
	int op, posn;
	register char *d;
{
	char cbuf[sizeof(Message)];	/* big enough, anyway */
	register char *p=cbuf;

	*p++=f;
	*p++=n;
	*p++=op;
	*p++=posn;
	*p++=posn>>8;
	while(n--!=0)
		*p++= *d++;
	sendnchars((int)(p-cbuf), cbuf);
}
/*
 * States of the receiver FSM
 */
#define	FILE		0	/* about to receive file */
#define	NBYTES		1	/* about to receive nbytes */
#define	OP		2	/* etc... */
#define	POSNLO		3
#define	POSNHI		4
#define	DATA		5	/* about to receive, or receiving, data */
rcv()
{
	static state, nbytes;
	static char *bp;
	register c;
	
	for(;;){
		c=rcvchar();
		if(c==-1)
			return 0;
		switch(state){
		case FILE:
			m.file=c;
			break;
		case NBYTES:
			nbytes=m.nbytes=c;
			break;
		case OP:
			m.op=c;
			break;
		case POSNLO:
			m.posn=c&0xFF;
			break;
		case POSNHI:
			m.posn|=(c&0xFF)<<8;
			if(nbytes<=0){
    Return:
				state=FILE;
				return 1;
			}
			bp=m.data;
			break;
		case DATA:
			*bp++=c;
			if(--nbytes<=0)
				goto Return;
			continue;	/* don't update state */
		default:
			mesg("unknown state in rcv()\n", 1);
		}
		state++;
	}
}
