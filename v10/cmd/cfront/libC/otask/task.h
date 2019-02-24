/*	HEADER FILE FOR THE TASK SYSTEM		*/

#ifndef TASK_DOT_H
#define TASK_DOT_H
#include "signal.h"

extern "C" {
	printf(const char* ...);
	void exit(int);
	extern double log(double);
}

#define SIZE		750
#define MODE		DEDICATED

class object;
class sched;	/* : public object */
class timer;	/* : public sched  */
class task;	/* : public sched  */
class qhead;	/* : public object */
class qtail;	/* : public object */
class team;

/* object.o_type */
#define OBJECT		0
#define TIMER		1
#define TASK		2
#define QHEAD		4
#define QTAIL		5

/* sched.s_state */
#define IDLE		1
#define RUNNING		2
#define TERMINATED	4

/* type of stack */
#define DEDICATED	1
#define SHARED		2
/* loc on stack */
#define UNTOUCHED	052525

/* error codes */
#define task_error_messages							\
macro_start									\
macro(0,E_ERROR,"")								\
macro(1,E_OLINK,"object.delete(): has chain")					\
macro(2,E_ONEXT,"object.delete(): on chain")					\
macro(3,E_GETEMPTY,"qhead.get(): empty")					\
macro(4,E_PUTOBJ,"qhead.putback(): object on other queue")			\
macro(5,E_PUTFULL,"qhead.putback(): full")					\
macro(6,E_BACKOBJ,"qtail.put(): object on other queue")				\
macro(7,E_BACKFULL,"qtail.put(): full")						\
macro(8,E_SETCLOCK,"set_clock(): clock!=0")					\
macro(9,E_CLOCKIDLE,"schedule(): clock_task not idle")				\
macro(10,E_RESTERM,"schedule: terminated")					\
macro(11,E_RESRUN,"schedule: running")						\
macro(12,E_NEGTIME,"schedule: clock<0")						\
macro(13,E_RESOBJ,"schedule: task or timer on other queue")			\
macro(14,E_HISTO,"histogram.new(): bad arguments")				\
macro(15,E_STACK,"task.save(): stack overflow")					\
macro(16,E_STORE,"new: free store exhausted")					\
macro(17,E_TASKMODE,"task.new(): bad mode")					\
macro(18,E_TASKDEL,"task.delete(): not terminated")				\
macro(19,E_TASKPRE,"task.preempt(): not running")				\
macro(20,E_TIMERDEL,"timer.delete(): not terminated")				\
macro(21,E_SCHTIME,"schedule: bad time")					\
macro(22,E_SCHOBJ,"schedule: bad object")					\
macro(23,E_QDEL,"queue.delete(): not empty")					\
macro(24,E_RESULT,"thistask->result()")						\
macro(25,E_WAIT,"wait for self")						\
macro(26,E_FUNCS,"FrameLayout - func start")					\
macro(27,E_FRAMES,"FrameLayout - frame size")					\
macro(28,E_REGMASK,"FrameLayout - register mask")				\
macro(29,E_FUDGE,"fudge return - no place to copy")				\
macro(30,E_NO_HNDLR,"sigFunc - no handler for signal")				\
macro(31,E_BADSIG,"illegal signal number")					\
macro(32,E_LOSTHNDLR,"signal handler not on chain")				\
macro_end(E_LOSTHNDLR)
#define macro_start
#define macro(num,name,string) const name = num ;
#define macro_end(last_name) const MAXERR = last_name;
task_error_messages
#undef macro_start
#undef macro
#undef macro_end

typedef int (*PFIO)(int,object*);
typedef void (*PFV)();

/* print flags */
#define CHAIN		1
#define VERBOSE		2
#define STACK		4


/* DATA STRUCTURES */
/*
	object --> olink --> olink ...
	   |         |         |
	  ...        V         V
	   |        task      task
	   V
	object --> ...
*/

class olink
/*	the building block for chains of task pointers */
{
friend object;
	olink*	l_next;
	task*	l_task;
		olink(task* t, olink* l) { l_task=t; l_next=l; };
};

class object
{
friend sched;
friend task;
	olink*	o_link;
	static task*	thxstxsk;
public:
	object*	o_next;
	virtual int	o_type();	// OBJECT

		object()	{ o_link=0; o_next=0; }
		~object();

	void	remember(task* t) { o_link = new olink(t,o_link); }
	void	forget(task*);	// remove all occurrences of task from chain
	void	alert();	// prepare IDLE tasks for scheduling
	virtual int	pending();	// TRUE if this object should be waited for
	virtual void	print(int, int =0);  // first arg VERBOSE, CHAIN, or STACK
	int	task_error(int);	// the central error function
	task*	this_task() { return thxstxsk; }
	static PFIO	error_fct;	// user-supplied error function
};

// fake compatibility with previous version
#define thistask (((object*)0)->this_task())
#define run_chain (((sched*)0)->get_run_chain())

static void print_error(int);
class sched : public object {  // only instances of subclasses are used
friend timer;
friend task;
friend object;
friend void print_error(int n);
friend SIG_FUNC_TYP sigFunc;
// friend void sigFunc(int);
	static int	keep_waiting_count;
	static sched* runchain;	// list of ready-to-run scheds (by s_time)
	static sched* priority_sched;	// if non-zero, sched to run next
	static task*	clock_task;	// awoken at each clock tick
	static long clxck;
	void	schedule();	/* sched clock_task or front of runchain */
	virtual void	resume();
	void	insert(int,object*); /* sched for d time units, ?t_alert=obj */
	void	remove();	/* remove from runchain & make IDLE */

	long	s_time;		/* time to sched; result after cancel() */
	int	s_state;	/* IDLE, RUNNING, TERMINATED */
public:
		sched() : s_time(0), s_state(IDLE) {}

	void	setclock(long);
	long	get_clock() { return clxck; }
	sched*	get_run_chain() { return runchain; }
	sched*	get_priority_sched() { return priority_sched; }
	long	rdtime()	{ return s_time; }
	int	rdstate()	{ return s_state; }
	int	pending();	// s_state != TERMINATED
	int	keep_waiting() { return keep_waiting_count++; }
	int	dont_wait() { return keep_waiting_count--; }

	void	cancel(int);
	int	result();
	virtual void	setwho(object* t);  // who alerted me
	void	print(int, int =0);
	static PFV	exit_fct;	// user-supplied exit function
};
// for compatibility
#define clock (((sched*)0)->get_clock())
inline void	setclock(long i) { ((sched*)0)->setclock(i); }

class timer : public sched {
	void	resume();
public:
		timer(int);
		~timer();
	void	reset(int);
	int	o_type();	// TIMER
	void	setwho(object* t);  // do nothing
	void	print(int, int =0);
};

extern _hwm;
class task : public sched {
friend sched;
	static task*	txsk_chxin;	// list of all tasks
	static team*	current_stack;	// stack we're currently running on
	void	save();
	void	restore();	/* swap in new task */
	int	swap_stack(int*,int*); // copy parent's stack for this child
	int	curr_hwm();	/* "high water mark" */
				/*     (how high stack has risen) */
	void	fudge_return(int*, int, task* =0);	// used in starting new tasks
	int	copy_share();	// used in starting shared tasks
	void	get_size();	// ditto -- saves size of active stack
	void	resume();
	int*	t_framep;	/* WARNING: t_framep
				   is manipulated as an offset
				   by restore()
				*/
	void*	th; /* fudge return from swap */
	int*	t_ap; 		// this frame's arg pointer (only for 3B)
	int*	t_basep;	// addr of stack when running (note stack is backwards)
	int	t_size;		/* holds hwm after cancel() */
	int*	t_savearea;	// addr of stack when not running (SHARED only)
	int	t_trap;
	team*	t_team;		/* stack and info for sharing */

	int	t_mode;		/* DEDICATED/SHARED stack */
	int	t_stacksize;

	object*	t_alert;	/* object that inserted you */
public:
		task(char* name=0, int mode=0, int stacksize=0);
		~task();

	int	o_type();	// TASK
	task*	t_next;		/* insertion in "task_chain" */
	char*	t_name;

	task*	get_task_chain() { return txsk_chxin; }
	int	waitvec(object**);
	int	waitlist(object* ...);
	void	wait(object* ob);

	void	delay(int);
	int	preempt();
	void	sleep(object* t =0);	// t is remembered
	void	resultis(int);
	void	cancel(int);
	void	setwho(object* t);  // t_alert = t
	void	print(int, int =0);
};
// for compatibility
#define task_chain (((task*)0)->get_task_chain())

// an Interrupt_handler supplies an interrupt routine that runs when the
// interrupt occurs (real time).  Also the Interrupt_handler can be waited for.
class Interrupt_handler : public object {
	friend class Interrupt_alerter;
	friend SIG_FUNC_TYP sigFunc;
//	friend void sigFunc(int);
	int	id;		// signal or interrupt number
	int	got_interrupt;	// an interrupt has been received but not alerted
	Interrupt_handler	*old;	// previous handler for this signal
	virtual void	interrupt();	// runs at real time
public:
	int	pending();	// FALSE once after interrupt
		Interrupt_handler(int sig_num);
		~Interrupt_handler();
};

/* QUEUE MANIPULATION (see queue.c) */
/*
	qhead <--> oqueue <--> qtail   (qhead, qtail independent)
	oqueue ->> circular queue of objects
*/

/* qh_modes */
#define EMODE		1
#define WMODE		2
#define ZMODE		3

class oqueue
{
friend qhead;
friend qtail;
	int	q_max;
	int	q_count;
	object*	q_ptr;
	qhead*	q_head;
	qtail*	q_tail;

		oqueue(int m)	{ q_max=m; q_count=0; q_head=0; q_tail=0; };
		~oqueue()	{ (q_count)?((object*)0)->task_error(E_QDEL):0; };
	void	print(int);
};

class qhead : public object
{
friend qtail;
	int	qh_mode;	/* EMODE,WMODE,ZMODE */
	oqueue*	qh_queue;
public:
		qhead(int = WMODE, int = 10000);
		~qhead();

	int	o_type();	// QHEAD
	object*	get();
	int	putback(object*);

	int	rdcount()	{ return qh_queue->q_count; }
	int	rdmax()		{ return qh_queue->q_max; }
	int	rdmode()	{ return qh_mode; }
	qtail*	tail();

	qhead*	cut();
	void	splice(qtail*);

	void	setmode(int m)	{ qh_mode = m; }
	void	setmax(int m)	{ qh_queue->q_max = m; }
	int	pending();	// rdcount() == 0
	void	print(int, int =0);
};

class qtail : public object
{
friend qhead;
	int	qt_mode;
	oqueue*	qt_queue;
public:
		qtail(int = WMODE, int = 10000);
		~qtail();

	int	o_type();	// QTAIL
	int	put(object*);

	int	rdspace()	{ return qt_queue->q_max - qt_queue->q_count; }
	int	rdmax()		{ return qt_queue->q_max; }
	int	rdmode()	{ return qt_mode; }

	qtail*	cut();
	void 	splice(qhead*);

	qhead*	head();

	void	setmode(int m)	{ qt_mode = m; }
	void	setmax(int m)	{ qt_queue->q_max = m; }
	int	pending();	// rdspace() == 0
	void	print(int, int =0);
};


struct histogram
/*
	"nbin" bins covering the range [l:r[ uniformly
	nbin*binsize == r-l
*/
{
	int	l, r;
	int	binsize;
	int	nbin;
	int*	h;
	long	sum;
	long	sqsum;
		histogram(int=16, int=0, int=16);

	void	add(int);
	void	print();
};

/*	the result of randint() is always >= 0	*/

#define DRAW (randx = randx*1103515245 + 12345)
#define ABS(x)	(x&0x7fffffff)

#ifdef pdp11
#define MASK(x)	((x>>16)&077777)
#define MAXINT_AS_FLOAT 32768.0
#else
#define MASK(x) ABS(x)
#define MAXINT_AS_FLOAT 2147483648.0
#endif

class randint
/*	uniform distribution in the interval [0,MAXINT_AS_FLOAT] */
{
	long	randx;
public:
		randint(long s = 0)	{ randx=s; }
	void	seed(long s)	{ randx=s; }
	int	draw()		{ return MASK(DRAW); }
	float	fdraw()		{ return ABS(DRAW)/MAXINT_AS_FLOAT; };
};

class urand : public randint
/*	uniform distribution in the interval [low,high]	*/
{
public:
	int	low, high;
		urand(int l, int h)	{ low=l; high=h; }
	int	draw() { return int(low + (high-low) * (0+randint::draw()/MAXINT_AS_FLOAT)); }
};

class erand : public randint
/*	exponential distribution random number generator */
{
public:
	int	mean;
		erand(int m) { mean=m; };
	int	draw() { return (int)(-mean * log( (double)(MAXINT_AS_FLOAT-randint::draw())
						/ MAXINT_AS_FLOAT) + .5); };
};

// This task will alert Interrupt_handler objects.
class Interrupt_alerter : public task {
public:
		Interrupt_alerter();
};
extern Interrupt_alerter	interrupt_alerter;
#endif
