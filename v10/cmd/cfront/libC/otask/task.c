#include "task.h"

/*	macros giving the addresses of the stack frame pointer
	and the program counter of the caller of the current function
	given the first local variable
	The AP macro (used only by 3B) takes the first function
	argument (after the implied this) which must not be a
	register.
*/

#ifdef pdp11
/* of historical interest only */
#define FP()		(&_that+4)
#define OLD_FP(fp)	(*fp)
#endif

#ifdef vax
#define STACK_GROWS_DOWN	1
#define FP(p)		((int*)(&p+1))
#define AP(arg1)	((int*)0)       /* unnecessary on vax */
#define OLD_AP(fp)	(*(fp+2))
#define OLD_FP(fp)	(*(fp+3))
#define FIX_FRAME(x)\
	OLD_AP(t_framep) = int(de_ap-x);\
	OLD_FP(t_framep) = int(de_fp-x)

#endif

#ifdef mc68000
/* careful -- stack frame not self-describing */
#define STACK_GROWS_DOWN	1
#define FP(p)		( (int*)(&p+1) )
#define AP(arg1)            ((int*)0)       /* unnecessary on mc68000 */
#define OLD_AP(fp)	(int*)0	/* unnecessary on mc68000 */
#define OLD_FP(fp)	(*fp)
#define FIX_FRAME(x)\
	OLD_FP(t_framep) = int(de_fp - x)
#endif

#if u3b | u3b5 | u3b2 | u3b15
#define STACK_GROWS_UP	1
#define FP(p)		( (int*)&p )
#define AP(arg1)            ((int*)&arg1-1)       /* AP of current frame */
#ifdef u3b
#define OLD_FP(fp)	(*( (int*)fp - 11 ) )
#define OLD_AP(fp)	(*( (int*)fp - 12 ) )
#else
#define OLD_FP(fp)	(*( (int*)fp - 7 ) )
#define OLD_AP(fp)	(*( (int*)fp - 8 ) )
#endif

#define FIX_FRAME(x)\
	OLD_AP(t_framep) = int(de_ap - x);\
	OLD_FP(t_framep) = int(de_fp - x)

#endif

#ifdef STACK_GROWS_UP  /* stack grows toward higher memory */
#define COPY_STACK(f,c,t)   while (c--)  *t++ = *f++
#define ACTIVE_STK_SZ(b,t)  (t - b + 1)	/* size of active stack */
#define STACK_BASE(b,s)     b
#define SAVED_AREA(b,s)     b
#define STACK_TOP(b,s)      (b + s + 1)
#define STACK_DIFF(s1,s2)   ((int*)s2 - (int*)s1)
#define SETTRAP()	t_trap = *(t_basep + t_team->size - 1)
#define CHECKTRAP()\
	if (t_team->size && t_trap != *(t_basep + t_team->size - 1))\
				 task_error(E_STACK)
#else  /* stack grows toward lower memory */
#define COPY_STACK(f,c,t)   while (c--) *t-- = *f--
#define ACTIVE_STK_SZ(b,t)  (b - t + 1)	/* size of active stack */
#define STACK_BASE(b,s)     (b + s - 1)
#define SAVED_AREA(b,s)     (b - s + 1)
#define STACK_TOP(b,s)      (b - s - 1)
#define STACK_DIFF(s1,s2)   ((int*)s1 - (int*)s2)
#define SETTRAP()	t_trap = *(t_basep - t_team->size + 1)
#define CHECKTRAP()\
	if (t_team->size && t_trap != *(t_basep - t_team->size + 1))\
				 task_error(E_STACK)
#endif

int _hwm;

static void copy_in(task* th, int* savearea, int sz, int* basep);

// a team is a collection of tasks that share one stack
class team
{
friend task;
	int	no_of_tasks;
	task*	got_stack;
	int*	stack;
	int	size;	// of the stack
	team(task*, int =0);	// stacksize == zero ==> using the main stack
	~team() { delete stack; }
};
team::team(task* t, int stacksize) {
	no_of_tasks = 1;
	got_stack = t;
	if (size = stacksize) {
		stack = new int[stacksize];
		if (_hwm) for (int x=0; x<stacksize; x++) stack[x] = UNTOUCHED;
	}
}


static void
usemainstack()
/* fudge to allow simple stack overflow check */
{
	register v[SIZE+100];

	if (_hwm)
		for (register i=0;i<SIZE+100;i++) v[i] = UNTOUCHED;
	else
		v[0] = 0;
}

static void
copy_stack(register* f, register c, register* t)
/*
	copy c words down from f to t
	do NOT attempt to copy "copy_stack"'s own stackframe
*/
{
	COPY_STACK(f,c,t);
}

// copy the current task out when starting a new SHARED task
int	// return offset to save area
task::copy_share()
{
	int*	p;
	p = new int[t_size];
	t_savearea = STACK_BASE(p,t_size);
	copy_stack(t_basep,t_size,t_savearea);
	return t_savearea-t_basep;
}

void
task::get_size()	// save size of active stack in preparation for copy out
{
	int x = ACTIVE_STK_SZ(t_basep,FP(x));
	t_size = x;
}

int	// return distance from original to new stack
task::swap_stack(int* p, int* pa)	// copy parent's stack for child
{
	int x = ACTIVE_STK_SZ(pa,FP(x));	// a little extra for this frame
	copy_stack(pa,x,p);
	return pa-p;
}
#include <stdio.h>
task::task(char* name, int mode, int stacksize)
/*
	executed in the task creating a new task - thistask.
	1:	put thistask at head of scheduler queue,
	2:	create new task
	3:	transfer execution to new task
	derived::derived can never return - its return link is destroyed

	if thistask==0 then we are executing on main()'s stack and
	should turn it into the "main" task

	for the mc68000 implementation, the function prologue of this function
	must save all registers of interest to any function, and it must match the
	prologue of sched::schedule().  The assembly language function swap()
	must have an epilogue that matches the epilogue of this function exactly!
	The current prologue with the -O switch is

	link	a6,#-44
	moveml	#15612,sp@

	and without the -O switch

	link	a6,#0
	addl	#-LF195,sp
	moveml	#LS195,sp@
	LF195 = 44
	LS195 = 0x3cfc

	this allocates a stack frame of 11 longwords and saves registers
	a5, a4, a3, a2, d7, d6, d5, d4, d3, and d2 in it.
*/
{
	int* de_ap;
	register int* ta_fp = (int*)FP(de_ap);		// task::task() frame pointer
	register int* de_fp = (int*)OLD_FP(ta_fp);	// My_task::My_task() frame pointer
	de_ap = (int*)OLD_AP(ta_fp);
	register int* pa_fp = (int*)OLD_FP(de_fp);	// parent task frame pointer
	// we have to trick the compiler into thinking we really use all these registers
	register int	x, y, z, u, v, w;
	w = 0; v = w; u = v; z = u; y = z; x = y;
//fprintf(stderr,"task\n");
	t_name = name;
	t_mode = mode ? mode : DEDICATED;
	t_alert = 0;
	s_state = RUNNING;
	t_next = txsk_chxin;
	txsk_chxin = this;
	th = this;	/* fudged return value -- "returned" from swap */

//fprintf(stderr,"this %d name %s mode %d size %d\n",this,name,mode,stacksize);
	switch ((int)thxstxsk) {
	case 0:
		/* initialize task system by creating "main" task */
		thxstxsk = (task*) 1;
		thxstxsk = new task("main");
		break;
	case 1:
		/*	create "main" task	*/
		usemainstack();			/* ensure that store is allocated */
		// set base pointer assuming a static task
		t_basep = (int*)OLD_FP((int*)OLD_FP((int*)OLD_FP(pa_fp)));
		current_stack = t_team = new team(this);	/* don't allocate stack */
		t_team->no_of_tasks = 2;   	/* never deallocate */
		return;
	}
	thxstxsk->th = this;	/* return pointer to "child" */
	thxstxsk->t_framep = de_fp; // prepare to return from My_task::My_task frame
	thxstxsk->t_ap = de_ap;	// only matters to 3B
	thxstxsk->insert(0,this);

	switch (t_mode) {
	case DEDICATED:
		t_team = new team(this, (stacksize ? stacksize : SIZE));
		t_basep = STACK_BASE(t_team->stack, t_team->size);
		t_framep = ta_fp - (x = swap_stack(t_basep, pa_fp));
				/* now doctor the new frame */
		t_ap = AP(name) - x;        // only matters to 3B
		FIX_FRAME(x);
		if (thxstxsk->t_mode == SHARED)
			thxstxsk->get_size();
		thxstxsk->fudge_return(ta_fp, 0, this);
		/* NO_RETURN */
	case SHARED:
		thxstxsk->t_mode = SHARED; /* you cannot share on your own */
		t_basep = pa_fp;
		t_team = thxstxsk->t_team;
		t_team->no_of_tasks++;
		t_framep = ta_fp;
		t_ap = AP(name);    // only matters for 3B
		SETTRAP();
		thxstxsk->get_size();
		x = thxstxsk->copy_share();	// copy out
		thxstxsk->fudge_return(ta_fp, x);
		t_team->got_stack = thxstxsk = this;
		return;
	default:
		task_error(E_TASKMODE);
	}
}

void
task::save()
/*
	save task's state so that ``restore'' can resume it later.
	Works by saving the frame pointer to the scheduler's frame.
*/
{
	int* x;
	register* old_fp = (int*)OLD_FP((int*)FP(x));  // caller's frame pointer (task::resume())
	t_framep = (int*)OLD_FP(old_fp);  // sched::schedule() frame pointer
	t_ap = (int*)OLD_AP(old_fp);	// only matters for 3B

	CHECKTRAP();

	if (t_mode == SHARED)
		t_size = ACTIVE_STK_SZ(t_basep,old_fp);	// a little extra room
	// don't copy out until we have to
}

void
task::resume()
{
	if (thxstxsk && thxstxsk->s_state != TERMINATED)
		thxstxsk->save();
	restore();
}

extern "C" {
	swap(task*);
}

void
task::restore()
/*
	make "this" task run after suspension by returning from the frame
	denoted by "t_framep"

	the key function "swap" is written in assembly code,
	it returns from the function which "save"d the task
		- typically the scheduler
*/
{
	register sz;
	register int* p;
	register task*	prevOnStack;

	SETTRAP();

	thxstxsk = this;
	if (t_mode == SHARED && this != (prevOnStack = t_team->got_stack)) {
		p = new int[sz = prevOnStack->t_size];
		prevOnStack->t_savearea = p = STACK_BASE(p,sz);
		copy_stack(prevOnStack->t_basep, sz, p);	// copy out
		sz = t_size;
		t_team->got_stack = this;
		if (current_stack == t_team)	// do we have to be careful?
			copy_in(this, t_savearea, sz, t_basep);	// no return
		copy_stack(t_savearea, sz, t_basep);	// copy in carelessly
		delete SAVED_AREA(t_savearea,sz);
	}
	current_stack = t_team;
	swap(this);
}

int*	dummy_write_but_dont_read_pointer;	// defeat simple optimization

static void	// watch out for tail recursion elimination
hack_stack32(task* th, int* savearea, int sz, int* basep)
{
	int	dummy[32];
	dummy_write_but_dont_read_pointer = dummy;
	copy_in(th, savearea, sz, basep);
}

static void	// watch out for tail recursion elimination
hack_stack64(task* th, int* savearea, int sz, int* basep)
{
	int	dummy[64];
	dummy_write_but_dont_read_pointer = dummy;
	copy_in(th, savearea, sz, basep);
}

static void	// make sure we don't copy on top of our own stack frame
copy_in(task* th, int* savearea, int sz, int* basep)
{
	int	i;
	if ((i = STACK_DIFF((int*)OLD_FP(FP(i)), STACK_TOP(basep,sz)))
			<= 0) {
		copy_stack(savearea, sz, basep);	// copy in
		delete SAVED_AREA(savearea,sz);
		swap(th);
	} else if (i < 32)
		hack_stack32(th, savearea, sz, basep);
	else
		hack_stack64(th, savearea, sz, basep);
}

void
task::cancel(int val)
/*
	TERMINATE and free stack space
*/
{
	sched::cancel(val);
	if (_hwm) t_size = curr_hwm();
	if (t_team->no_of_tasks-- == 1) delete t_team;
}

task::~task()
/*
	free stack space and remove task from task chain
*/
{
	if (s_state != TERMINATED) task_error(E_TASKDEL);
	if (this == txsk_chxin)
		txsk_chxin = t_next;
	else {
		register task* t;
		register task* tt;

		for (t=txsk_chxin; tt=t->t_next; t=tt)  
			if (tt == this) {
				t->t_next = t_next;
				break;
			}
	}

	if (this == thxstxsk) {
		delete (int*) thxstxsk;	/* fudge: free(_that) */
		thxstxsk = 0;
		schedule();
	}
}

void
task::resultis(int val)
{
	cancel(val);
	if (this == thxstxsk) schedule();
}

void
task::sleep(object* t)
{
	if (t) t->remember(this);
	if (s_state == RUNNING) remove();
	if (this == thxstxsk) schedule();
}

void
task::delay(int d)
{
	insert(d,this);
	if (thxstxsk == this) schedule();
}

int
task::preempt()
{
	if (s_state == RUNNING) {
		remove();
		return s_time-clock;
	}
	else {
		task_error(E_TASKPRE);
		return 0;
	}
}

char*
state_string(int s)
{
	switch (s) {
	case IDLE:		return "IDLE";
	case TERMINATED:	return "TERMINATED";
	case RUNNING:		return "RUNNING";
	default:		return 0;
	}
}

char*
mode_string(int m)
{
	switch(m) {
	case SHARED:		return "SHARED";
	case DEDICATED:		return "DEDICATED";
	default:		return 0;
	}
}

void
task::print(int n, int baseClass)
/*
	``n'' values:	CHAIN,VERBOSE,STACK
*/
{
	if (!baseClass)
		printf("task ");

	char* ss = state_string(s_state);
	char* ns = (t_name) ? t_name : "";
	
	printf("task %s ",ns);
	if (this == thxstxsk)
		printf("(is thistask):\n");
	else if (ss)
		printf("(%s):\n",ss);
	else
		printf("(state==%d CORRUPTED):\n",s_state);

	if (n&VERBOSE) {
		char* ms = mode_string(t_mode);
		if (ms == 0) ms = "CORRUPTED";
		printf("\tmode=%s alert=%d next=%d",
			ms, t_alert, t_next);
		printf((s_state==TERMINATED) ? " result=%d\n" : " s_time=%d\n", s_time);
	}

	if (n&STACK) {
		printf("\tstack: ");
		if (s_state == TERMINATED) {
			if (_hwm) printf("hwm=%d",t_size);
			printf(" deleted\n");
		}
		else {
			int* b = t_basep;
			int x = this==thxstxsk || t_mode==DEDICATED ?
					b - t_framep : t_size;
			printf("max=%d current=%d",t_team->size,x);
			if (_hwm) printf(" hwm=%d",curr_hwm());
			printf(" t_base=%d, t_frame=%d, t_size=%d\n",b,t_framep,t_size);
		}
	}

	if (n&CHAIN) {
		if (t_next) t_next->print(n);
	}

	sched::print(n, 1);
}

int
task::curr_hwm()
{
	int* b = t_basep;
	int i;
	for (i=t_team->size-1; 0<=i && *(b-i)==UNTOUCHED; i--) ;
	return i;
}

void
task::wait(object* ob)
{
	if (ob == (object*)this) task_error(E_WAIT);
	t_alert = ob;
	while (ob->pending())
		sleep(ob);
}

int
task::waitlist(object* a ...)
{
	return waitvec(&a);
}

int
task::waitvec(object** v)
/*
	first determine if it is necessary to sleep(),
	return hint: who caused return
*/
{
	int i;
	int j;
	register object* ob;

	for(;;) {
		for (i = 0; ob = v[i]; i++) {
			if (!ob->pending()) goto ex;
			ob->remember(this);
		}
		if (i==1 && v[0]==(object*)this) task_error(E_WAIT);
		sleep();
	}
ex:
	t_alert = ob;
	for (j = 0; ob = v[j]; j++)
		ob->forget(this);
	return i;
} 

void
task::setwho(object* t)
{
	t_alert = t;
}

int
task::o_type()
{
	return TASK;
}

Interrupt_alerter	interrupt_alerter;

