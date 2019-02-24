/*ident	"%W%" */
/**************************************************************************
			Copyright (c) 1984 AT&T
	  		  All Rights Reserved  	

	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
	
	The copyright notice above does not evidence any   	
	actual or intended publication of such source code.

*****************************************************************************/
#include <task.h>

const	NASS = NSIG;

static Interrupt_handler*	hnd[NASS];	// hnd[i] is used for signal i
static SIG_PF	old_handlers[NASS];

Interrupt_alerter::Interrupt_alerter()
: ("Interrupt_alerter")
{
	for(;;) {
		sleep();
		for (register Interrupt_handler** p = &hnd[0]; p < &hnd[NASS]; p++)
			if (*p && (*p)->got_interrupt) {
				(*p)->alert();
			}
	}
}

/* Note:  the type of this function must match the definition of SIG_FUNC_TYP
   in signal.h. */
void
sigFunc(int sigNo)
{
	if ((unsigned)sigNo >= (unsigned)NASS)
		object::task_error(E_BADSIG, (object*)0);
	Interrupt_handler*	as = hnd[sigNo];
	if (as == 0)
		object::task_error(E_NO_HNDLR, (object*)0);
	as->interrupt();
	as->got_interrupt = 1;
	sched::priority_sched = &interrupt_alerter;
	signal(sigNo, sigFunc);		//reset
					//MORE:  ifdef SVR3 use sigset
}

Interrupt_handler::Interrupt_handler(int vecid)
: id(vecid), got_interrupt(0)
{
	if ((unsigned)id >= (unsigned)NASS)
		task_error(E_BADSIG, this);
	if((old = hnd[id]) == 0)
		old_handlers[id] = signal(id, sigFunc);
	else
		if (old->got_interrupt) {
			old->alert();
		}
	hnd[id] = this;
	this_task()->keep_waiting();
}

Interrupt_handler::~Interrupt_handler()
{
	this_task()->dont_wait();
	for(register Interrupt_handler** as = &hnd[id];; as = &(*as)->old) {
		if (*as == this) break;
		if (*as == 0)
			task_error(E_LOSTHNDLR, this);
	}
	if ((*as = old) == 0)
		 signal(id, old_handlers[id]);
}

void
Interrupt_handler::interrupt()
{
}

int
Interrupt_handler::pending()
{
	if (!got_interrupt)
		return 1;
	got_interrupt = 0;
	return 0;
}
