#include "event.h"
Event	*event, *efree;
#define	NEVENT	32
Event	eventpool[NEVENT];	/* for allocation */

/*
 * Advance the clock
 */
tick()
{
	register Event *e=event;
	if(e==0)
		return;
	--e->dt;
	while(e->dt==0){
		event=e->next;
		if((*e->fn)(e->type))
			(void)addevent(e, e->time, e->fn, e->type);	/* it's gotta work! */
		else{
			e->next=efree;
			efree=e;
		}
		e=event;
	}
}
/*
 * Add an event at time t from now
 */
addevent(e, t, fn, type)
	register Event *e;
	register t;
	int	 (*fn)();
{
	register Event *ep, *last=0;
	if(e==0)
		return 0;
	e->fn=fn;
	e->next=0;
	e->type=type;
	e->time=t;
	for(ep=event; ep; ep=ep->next){
		if(ep->dt < t)
			t -= ep->dt;
		else{	/* gotta insert it here */
			if(last)
				last->next = e;
			ep->dt-=t;
			e->next=ep;
			break;
		}
		last=ep;
	}
	if(last==0)
		event=e;
	else if(last->next==0)
		last->next=e;
	e->dt=t;
	return 1;
}
/*
 * Initially link all the events together
 */
initevent()
{
	register Event *e;
	for(e=eventpool; e<&eventpool[NEVENT-1]; e++)
		e->next=e+1;
	e->next=0;
	efree=eventpool;
	event=0;
}
/*
 * Allocate a new guy out of the array
 */
Event *
newevent()
{
	register Event *e;
	if(e=efree)	/* assignment = */
		efree=efree->next;
	return e;
}
