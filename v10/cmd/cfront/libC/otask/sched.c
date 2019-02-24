#include <osfcn.h>
#include "task.h"

void
sched::setclock(long t)
{
	if (clxck) task_error(E_SETCLOCK);
	clxck = t;
}

void
sched::cancel(int res)
{
	if (s_state==RUNNING) remove();
	s_state = TERMINATED;
	s_time = res;
	alert();
}

int
sched::result()
/* wait for termination and retrieve result */
{
	if (this == (sched*)this_task()) ((object*)0)->task_error(E_RESULT);
	while (s_state != TERMINATED) {
		this_task()->sleep(this);
	}

	return (int) s_time;
}

void
sched::schedule()
/* schedule either clock_task or front of runchain.
	This routine causes a coroutine switch by calling resume.
	When control eventually returns to this coroutine, it does so
	through task::restore and swap().
	For the 68000 implementation, the exit sequence of this function must
	match that of task::task() and swap(), so check and be sure they
	do.  Copy the exit sequence into swap if necesssary.
*/
{
	int a_dummy;
	register sched* p;
	register long tt;
	register int* p1_dummy;
	register int* p2_dummy;
	// we must trick the compiler into thinking we really use all these registers
	register int	y, z, u, v, w;
	w = 0; v = w; u = v; z = u; y = z; a_dummy = y;
	p1_dummy = p2_dummy = 0;

keep_waiting:
	if (p = priority_sched) {
		priority_sched = 0;  // no chain here
		p->remove();  // in case it's also on the runchain
		p->s_state = RUNNING;  // since remove sets it IDLE
	}
	else if (p = runchain) {
		runchain = (sched*) p->o_next;
		p->o_next = 0;
	}
	else {
		if (keep_waiting_count) {
			::wait(0);
			goto keep_waiting;
		}
		if (exit_fct) (*exit_fct)();
		for (p = task::txsk_chxin; p; p = ((task*)p)->t_next) {
			if (p->s_state == RUNNING) p->remove();
			p->s_state = TERMINATED;
			p->s_time = 0;
		}
		exit(0);
	}

	tt = p->s_time;
	if (tt != clxck) {
		if (tt < clxck) task_error(E_SCHTIME);
		clxck = tt;
		if (clock_task) {
			if (clock_task->s_state != IDLE)
				task_error(E_CLOCKIDLE);
			/* clock_task preferred -- put p back onto runchain */
			p->o_next = (object*) runchain;
			runchain = p;
			p = (sched*) clock_task;
		}
	}

	if (p != this)
		p->resume();
} /* schedule */

void
sched::insert(int d, object* who)
/*
	schedule THIS to run in ``d'' time units
	inserted by who
*/
{
	register sched * p;
	register sched * pp;
	register long tt = s_time = clxck + d;

	switch (s_state) {
	case TERMINATED:
		task_error(E_RESTERM);
		break;
	case IDLE:
		break;
	case RUNNING:
		if (this != (class sched *)this_task()) task_error(E_RESRUN);
	}

	if (d<0) task_error(E_NEGTIME);	

	if (o_next) task_error(E_RESOBJ);

	s_state = RUNNING;
	setwho(who);

	/* runchain ordered by s_time */
	if (p = runchain) {
		if (tt < p->s_time) {
			o_next = (object*) runchain;
			runchain = this;
		}
		else {
			while (pp = (sched *) p->o_next) {
				if (tt < pp->s_time) {
					o_next = pp;
					p->o_next = this;
					return;
				}
				else p = pp;
			}
			p->o_next = this;
		}
	}
	else 
		runchain = this;
}

void
sched::remove()
/* remove from runchain and make IDLE */
{
	register class sched * p;
	register class sched * pp;

	if (p = runchain)
		if (p == this)
			runchain = (sched*) o_next;
		else
			for (; pp = (sched*) p->o_next; p=pp)
				if (pp == this) {
					p->o_next = pp->o_next;
					break;
				}
	s_state = IDLE;
	o_next = 0;
}


void
sched::print(int n, int baseClass)
{
	if (!baseClass)
		printf("naked sched ");
	if (n&CHAIN) {
		if (o_next) ((sched*) o_next)->print(n);
	}

	object::print(n, 1);
	if (!baseClass)
		task_error(E_SCHOBJ);  // only derived class instances allowed
}

int
sched::pending()
{
	return s_state != TERMINATED;
}

void
sched::resume()  // this should never be called
{
	task_error(E_SCHOBJ);
}

void
sched::setwho(object*)  // this should never be called
{
	task_error(E_SCHOBJ);
}

