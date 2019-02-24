#ifdef TRACE
#include <stdio.h>
char tracebuf[256];
char *top[]={
	"DIAGNOSTIC",
	"INSERT",
	"BACKSPACE",
	"3",
	"CUT",
	"PASTE1",
	"SELECT",
	"SNARF",
	"8",
	"REQUEST",
	"SEEK",
	"SCROLL",
	"DONE",
	"RESET",
	"MOVE",
	"FILENAME",
	"CHARSONSCREEN",
	"PASTE2",
	"18",
	"MODIFIED",
	"WRITE",
};
#endif
#include "msgs.h"
/*
 * Unix-side message code for communicating with Jerq.
 */
Message m;
send(f, op, posn, n, d)
	unsigned f, n;
	int op, posn;
	register char *d;
{
	char cbuf[sizeof(Message)];	/* big enough, anyway */
#ifdef	TRACE
	int nn=n;
#endif
	register char *p=cbuf;

	*p++=f;
	*p++=n;
	*p++=op;
	*p++=posn;
	*p++=posn>>8;
	while(n--!=0)
		*p++= *d++;
#ifdef	TRACE
	sprintf(tracebuf, "to:   f:%d n:%d op:%s posn:%d chars:",
		f, nn, top[op], posn);
	write(2, tracebuf, strlen(tracebuf));
	write(2, cbuf, p-cbuf);
	write(2, "\n", 1);
#endif
	if(write(1, cbuf, p-cbuf)!=p-cbuf)	/* trouble! */
		rescue();
}
/*
 * States of the receiver FSM
 */
#define	FFILE		0	/* about to receive file */
#define	NBYTES		1	/* about to receive nbytes */
#define	OP		2	/* etc... */
#define	POSNLO		3
#define	POSNHI		4
#define	DATA		5	/* about to receive, or receiving, data */
rcv()
{
	static state, nbytes, nread;
	static char buf[sizeof(Message)];
	static char *bp, *p;
	
	for(;;){
		if(nread <= 0){
			nread=read(0, buf, sizeof buf);
			if(nread<=0)
				ioerr("read", "from jerq");
			p=buf;
		}
		nread--;
		switch(state){
		case FFILE:
			m.file= *p;
			break;
		case NBYTES:
			nbytes=m.nbytes= *p;
			break;
		case OP:
			m.op= *p;
			break;
		case POSNLO:
			m.posn= *p&0xFF;
			break;
		case POSNHI:
			m.posn|=(*p&0xFF)<<8;
			if(nbytes<=0){
				p++;
    Return:
#ifdef	TRACE
	sprintf(tracebuf, "from: f:%d n:%d op:%s posn:%d chars:",
		m.file, m.nbytes, top[m.op], m.posn);
	write(2, tracebuf, strlen(tracebuf));
	write(2, (char *)m.data, m.nbytes);
	write(2, "\n", 1);
#endif
				state=FFILE;
				return;
			}
			bp=m.data;
			break;
		case DATA:
			*bp++= *p++;
			if(--nbytes<=0)
				goto Return;
			continue;	/* don't update state */
		default:
			mesg("unknown state in rcv()", "");
		}
		state++;
		p++;
	}
}
