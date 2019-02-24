#ifndef	defont
#include	<CC/jerq.h>
#endif
#ifndef	P_TICKS
#include	<pot.pub>
#endif

#define	SPEED	100

class Clock
{
>pub
	char pub_filler[4];
>pri
	long next;
>
public:
	Rulepot *speed;
	Clock();
	void set();
	void synch();
};
extern Clock *clock;
