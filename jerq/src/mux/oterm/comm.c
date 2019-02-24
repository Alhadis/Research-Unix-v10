#include <jerq.h>
#include <layer.h>
#include <queue.h>
#include <jerqproc.h>
#include <setup.h>
#include <kbd.h>

#ifdef	BLITKBD
#define	REBOOTKEY	0xEC	/* ctrl-shift-BREAK */
#else
#define	REBOOTKEY	0x81	/* discon */
#endif
extern int doubleclickOK;

/*
 * clockroutine() called at video interrupt time.
 *	reads chars off the keyboard, sends things to host
 */

short		second;
short		ticks;
extern long	ticks0;
extern char	*patchedspot;
extern char	patch;
extern int	kbdrepeat;
#define	CONTROL	1

clockroutine()
{
	register c;
	extern struct Proc *kbdproc;
	register struct Proc *p;
	register char *upc;	/*SFBOTCH*/

	ticks0++;
	if(--ticks<=0){
		ticks=60;	/* really HZ */
		second=1;
	}

	for(p= &proctab[CONTROL+1]; p<&proctab[NPROC]; p++)
		if(p->nticks>0 && --p->nticks==0)
			p->state|=WAKEUP|RUN;
	setrun(&proctab[CONTROL]);
	if((p=kbdproc)==0)
		return;
	while(KBDQUEUE.c_cc>0){
		doubleclickOK=0;
		c=qgetc(&KBDQUEUE);
		if(c==0x8E){	/* SHIFT-SETUP; show what's up */
			rectf(&display, P->rect, F_XOR);
			do auto4(); while(button123()==0);
			if(button2()){
				/*SFBOTCHchar **/upc=(char *)P->pcb.pc;
				patchedspot=upc;
				patch= *upc;
				*upc=0x14;	/* EXTOP trap */
			}
			rectf(&display, P->rect, F_XOR);
			do auto4(); while(button123());
		}else if(c==REBOOTKEY)
			reboot();
		else
			qputc(&p->kbdqueue, c);
		if(p->state&KBDLOCAL)
			p->state|=WAKEUP;
	}
#ifndef	BLITKBD
	if((kbdrepeat&(RPTHAVECHR|RPTON)) && (ticks&1)==0)
		kbdrpt();
#endif
	givemouse(p);
}
givemouse(p)
	register struct Proc *p;
{
		register struct Mouse *m;	/*SFBOTCH*/
	if((p->state&(MOUSELOCAL|USER)) == (MOUSELOCAL|USER)){
		/*SFBOTCHregister struct Mouse * */m= &((struct udata *)p->data)->mouse;
		*m=mouse;
	}
}
sleep(s){
	register struct Proc *p=P;
	register alarmed=p->state&ALARMREQD;
	register long nticks;
	extern long ticks0;
	nticks=ticks0+p->nticks;
	alarm(s);
	Uwait(ALARM);
	/* a little dance because sleep calls alarm */
	if(alarmed){
		spl1();
		if(nticks>ticks0)
			p->nticks=nticks-ticks0;
		else	/* we missed his wakeup! */
			p->state|=WAKEUP;
		spl0();
		p->state|=ALARMREQD;
	}else
		p->state&=~ALARMREQD;
}
alarm(s){
	P->state|=ALARMREQD;
	if(s>0)
		P->nticks=s;
}
