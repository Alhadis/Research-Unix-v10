#include <jerq.h>

#define	RUN		1	/* ready to be scheduled */
#define	ZOMBIE		1024	/* proc died horribly; waiting for debugger */
#define	setrun(p)	((p)->state|=RUN)

static Rectangle nullrect = { 32, 32, 128, 64 };
static int poked, poke(); static char *paddr;

probe(argaddr)
char *argaddr;
{
	register struct Proc *p; register s;

	poked = 0;
	paddr = argaddr;
	if (p = newproc(poke)) {
		p->rect = nullrect;
		p->layer = newlayer(p->rect);
		setrun(p);
		alarm(100);
		do {
			wait(CPU|ALARM);
		} while (poked <= 0 && (p->state&ZOMBIE) == 0);
		dellayer(p->layer);
		p->state = 0;
		return poked;
	} else
		return -1;
}

static
poke()
{
	register c;
	c = *paddr;
	++poked;
	for (;;)
		wait(CPU);
}
