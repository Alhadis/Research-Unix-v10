#include	<CC/jerq.h>
#include	"frame.pri"
#include	"clock.pri"

Clock.Clock()
{
}

void
Clock.set()
{
	int step;

	step = m->period*60;
	if(m->maxframe > 1) step /= m->maxframe-1;
	next = realtime() + speed->val;
}

void
Clock.synch()
{
	for(; realtime() < next; sleep(1))
		if((own()&MOUSE) && button123()) break;
}
