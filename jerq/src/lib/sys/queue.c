#define QUEUE_C
#include <queue.h>
#include <setup.h>

/*
 * These macros are for speed, not size.
 */
#define pswmax()	asm("	PUSHW	%psw	");\
			asm("	ORW2	&0x1e000,%psw ");\
			asm("	TSTW	%r0	")

#define pswback()	asm("	POPW	%psw	");\
			asm("   TSTW    %r0     ")
qinit(){
	register i;

	for(i=1; i<NCHARS-1; i++)
		cbuf_next[i] = i+1;
	cbuf_next[i] = 0;
	freelist=1;
	freemark = (9*NCHARS)/10;
}
qputc(q, w)
	register struct clist *q;
	int w;
{
	register p;
	pswmax();
	p=freelist;
	if(p==0 || (freemark <= 0 && !(q->state & QPRIORITY))){
		pswback();
		if(!VALDWNLDFLAG )
			ringbell();	/* send a warning to the user */
		return(0);
	}
	freelist=cbuf_next[p];
	--freemark;
	cbuf_next[p]=0;
	cbuf_word[p]=w;
	if(q->c_cc==0)
		q->c_head=q->c_tail=p;
	else{
		cbuf_next[q->c_tail]=p;
		q->c_tail=p;
	}
	p = ++q->c_cc;
	pswback();
	return(p);
}
qputstr(q, str)
	register struct clist *q;
	register char *str;
{
	register retval;
	do; while(*str && ((retval=qputc(q, *str++))>=0));
	return retval;
}
qgetc(q)
	register struct clist *q;
{
	register p;
	register x;
	pswmax();
	if((p=q->c_head)==0){
		pswback();
		return(-1);
	}
	if(--q->c_cc==0)
		q->c_head=q->c_tail=0;
	else
		q->c_head=cbuf_next[p];
	cbuf_next[p]=freelist;
	x=cbuf_word[p];
	freelist=p;
	++freemark;
	pswback();
	return(x);
}
qclear(q)
	register struct clist *q;
{
	pswmax();
	if(q->c_cc == 0){
		pswback();
		return;
	}
	freemark += q->c_cc;
	q->c_cc=0;
	cbuf_next[q->c_tail]=freelist;
	freelist=q->c_head;
	q->c_head=0;
	q->c_tail=0;
	q->state=0;
	pswback();
}
qpeekc(q)
	register struct clist *q;
{
	register p;
	return (p=q->c_head) ? cbuf_word[p] : -1;
}
