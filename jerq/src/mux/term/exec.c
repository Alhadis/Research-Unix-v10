#include <jerq.h>
#include <queue.h>
#include <layer.h>
#include <jerqproc.h>

#define STACK 16

execsw()
{
	restart(P, P->text);
	retps();	/* return from exec's temporary pcb into P->pcb */
}
zombsw()
{
	restart(P, P->text);
asm("	subw2	&4,%isp  ");	/* adjust isp because we don't RETPS */
	sw(1);			/* suspend  the new process */
}
exec(f)
	int (*f)();
{
	struct pcb execpcb;
	int execstk[STACK];

	execpcb.psw = 0x281e100;
	execpcb.pc = (char (*)())execsw;
	execpcb.sp = (int)execstk;
	execpcb.lower = (int)execstk;
	execpcb.upper = (int)&execstk[STACK];
	execpcb.ap = (int)execstk;
	execpcb.fp = (int)execstk;
	execpcb.blkmv = 0;
	P->text = (char *)(f);
	P->inhibited = 0;
	P->cursor = 0;
	callps(&execpcb);
}
zombexec(f)
	int (*f)();
{
	struct pcb zombpcb;
	int zombstk[STACK];

	zombpcb.psw = 0x281e100;
	zombpcb.pc = (char (*)())zombsw;
	zombpcb.sp = (int)zombstk;
	zombpcb.lower = (int)zombstk;
	zombpcb.upper = (int)&zombstk[STACK];
	zombpcb.ap = (int)zombstk;
	zombpcb.fp = (int)zombstk;
	zombpcb.blkmv = 0;
	P->text = (char *)(f);
	P->inhibited = 0;
	P->cursor = 0;
	callps(&zombpcb);
}
