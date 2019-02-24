#include "task.h"

const	NASS = NSIG;

static Interrupt_handler*	hnd[NASS];	// hnd[i] is used for signal i
// static SIG_TYP	old_handlers[NASS];	// for old compilers
static SIG_FUNC_TYP*	old_handlers[NASS];

Interrupt_alerter::Interrupt_alerter()
: ("Interrupt_alerter")
{
	for(;;) {
		sleep();
// fprintf(stderr, "Interrupt_alerter loop\n");
		for (register Interrupt_handler** p = &hnd[0]; p < &hnd[NASS]; p++)
			if (*p && (*p)->got_interrupt)
				(*p)->alert();
	}
}

void
sigFunc(int sigNo
#ifdef BSD
, int, sigcontext*
#endif
)
{
// fprintf(stderr, "signal #%d\n", sigNo);
	if ((unsigned)sigNo >= (unsigned)NASS)
		((object*)0)->task_error (E_BADSIG);
	Interrupt_handler*	as = hnd[sigNo];
	if (as == 0)
		((object*)0)->task_error(E_NO_HNDLR);
	as->interrupt();
	as->got_interrupt = 1;
	sched::priority_sched = &interrupt_alerter;
	signal(sigNo, sigFunc);
// fprintf(stderr, "return from interrupt #%d\n", sigNo);
	return;
}

Interrupt_handler::Interrupt_handler(int vecid)
: id(vecid), got_interrupt(0)
{
// fprintf(stderr, "Interrupt_handler::Interrupt_handler #%d\n", vecid);
	if ((unsigned)id >= (unsigned)NASS)
		task_error(E_BADSIG);
	if((old = hnd[id]) == 0)
		old_handlers[id] = signal(id, sigFunc);
	else
		if (old->got_interrupt)
			old->alert();
	hnd[id] = this;
	this_task()->keep_waiting();
}

Interrupt_handler::~Interrupt_handler()
{
// fprintf(stderr, "Interrupt_handler::~Interrupt_handler #%d\n", id);
	this_task()->dont_wait();
	for(register Interrupt_handler** as = &hnd[id];; as = &(*as)->old) {
		if (*as == this) break;
		if (*as == 0)
			task_error(E_LOSTHNDLR);
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
