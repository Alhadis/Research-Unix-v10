#include <jerq.h>
#include <layer.h>
#define	TASK_C
#include <jerqproc.h>

#define STACK	16
int	swstk[STACK];
int	swpcbp[20];
extern int (*Sys[])();		/* pcb for the switcher */
#define	PSW	0x281A100	/* ipl 13 */

struct Proc *
newproc(f)
	int (*f)();
{
	register struct Proc *u;
	register i;
	for(i=0,u=proctab; i<NPROC; i++,u++) {
		if((u->state&BUSY)==0){
			u->state |= BUSY;
			restart(u, f);
			u->layer=0;
			u->nchars=0;
			u->cbufpin=u->cbuf;
			u->cbufpout=u->cbuf;
			u->traptype=0;
			u->traploc=0;
			u->defaultfont = (struct Font*)Sys[1];
			return u;
		}
	}
	return 0;
}
restart(p, loc)
	register struct Proc *p;
	register (*loc)();
{
	register i;
	p->pcb.psw=PSW;
	p->pcb.pc=(char(*)())loc;
	p->pcb.sp=(int)(p->stack);
	p->pcb.lower=p->pcb.sp;
	p->pcb.upper=p->pcb.lower+STKSIZ;
	p->pcb.ap=p->pcb.sp;
	p->pcb.fp=p->pcb.sp;
	for(i=8; i>=0; i--)
		p->pcb.reg[i]=0;
	p->pcb.blkmv=0;	/* no block moves */
	p->text=(char *)(loc);
}
sw(run){
	if(!run)
		setnorun(P);
	callps(swpcbp);
	return;
}
switcher()
{
	register struct Proc *p=P;
	for(;;){
		p++;	/* 'cos we are the current p */
		if(p>=&proctab[NPROC])
			p=proctab;
		if((p->state&(RUN|ZOMBIE))==RUN)
			resume(P=p);
	}
}
swinit()
{
	register int * pcbptr = swpcbp;

	*pcbptr=0x281a180; 		/* psw, r+i bits set only */
	*(pcbptr+1)=(int)switcher; 	/* initial pc */
	*(pcbptr+2)=(int)swstk; 	/* initial stack */
	*(pcbptr+3)=0x1a100;
	*(pcbptr+4)=(int)switcher; 	/* initial pc */
	*(pcbptr+5)=(int)swstk;
	*(pcbptr+6)=(int)swstk;
	*(pcbptr+7)=(int)&swstk[STACK-1];
	*(pcbptr+8)=(int)swstk;	/* ap */
	*(pcbptr+9)=(int)swstk;	/* fp */
	*(pcbptr+19)=0;		/* no block moves */
};
