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
	register struct cbuf *p, *q;

	q=0;
	for(p=cbufs; p<&cbufs[NCHARS]; p++){
		p->next=q;
		q=p;
	}
	freelist=q;
}
qputc(q, w)
	register struct clist *q;
	register w;
{
	register cc;
	register struct cbuf *p;
	pswmax();
	p=freelist;
	if(p==0){
		pswback();
		if(!VALDWNLDFLAG )
			ringbell();	/* send a warning to the user */
		return(0);
	}
	freelist=p->next;
	p->next=0;
	p->word=w;
	if(q->c_cc==0)
		q->c_head=q->c_tail=p;
	else{
		q->c_tail->next=p;
		q->c_tail=p;
	}
	cc= ++q->c_cc;
	pswback();
	return(cc);
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
	register struct cbuf *p;
	register x;
	pswmax();
	if((p=q->c_head)==0){
		pswback();
		return(-1);
	}
	if(--q->c_cc==0)
		q->c_head=q->c_tail=0;
	else
		q->c_head=p->next;
	p->next=freelist;
	x=p->word;
	freelist=p;
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
	q->c_cc=0;
	q->c_tail->next=freelist;
	freelist=q->c_head;
	q->c_head=0;
	q->c_tail=0;
	pswback();
}
