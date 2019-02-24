#include <jerq.h>

#define	RUN		1	/* ready to be scheduled */
#define	setrun(p)	((p)->state|=RUN)

static int dummy(), dead; static struct Proc *p;

Bitmap *
lalloc(r)
Rectangle r;
{
	if ((p = newproc(dummy)) && (p->layer = newlayer(r))) {
		p->rect = inset(r, 3);
		border(p->layer, r, 1, F_STORE);
		setrun(p);
		return (Bitmap *)(p->layer);
	} else
		return 0;
}

ldead()
{
	dead = 1;
	wait(CPU);
	return dead;
}

lfree()
{
	dead = 1;
	wait(CPU);
	if (!dead) {
		dellayer(p->layer);
		p->state = 0;
		p->layer=0;
	}
}

static
dummy()
{
	for (;;)
		dead = 0, wait(CPU);
}
