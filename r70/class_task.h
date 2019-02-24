#class

#define SIZE		512
#define MODE		DEDICATED

#define UNTOUCHED	052525

extern char * calloc();
extern char * malloc();
extern free();

extern class sched * run_chain;		/* list of ready to run scheds */
extern class task * task_chain;		/* list of all tasks */

extern int task_error(int,class object *);
extern void print_error(int);

#define E_OLINK		1
#define E_ONEXT		2
#define E_GETEMPTY	3
#define E_PUTOBJ	4
#define E_PUTFULL	5
#define E_BACKOBJ	6
#define E_BACKFULL	7
#define E_SETCLOCK	8
#define E_CLOCKIDLE	9
#define E_RESTERM	10
#define E_RESRUN	11
#define E_NEGTIME	12
#define E_RESOBJ	13
#define E_HISTO		14
#define E_STACK 	15
#define E_STORE		16
#define E_TASKMODE	17
#define E_TASKDEL	18
#define E_TASKPRE	19
#define E_TIMERDEL	20
#define E_SCHTIME	21
#define E_SCHOBJ	22
#define E_QDEL		23
#define E_RESULT	24
#define E_WAIT		25
#define MAXERR	E_WAIT

extern char _hwm;
#define hwm()	(_hwm=1)

class olink
/*	the building block for chains of task pointers */
{
friend object;
	class olink *	l_next;
	class task *	l_task;
	void		new(class task * t, class olink * l) { l_task=t; l_next=l; };
};

class oqueue
{
friend qhead;
friend qtail;
	int		q_max;
	int		q_count;
	class object *	q_ptr;
	class qhead *	q_head;
	class qtail *	q_tail;

	void		new(int m)	{ q_max=m; q_count=0; q_head=0; q_tail=0; };
	void		delete()	{ (q_count)?task_error(E_QDEL,0):0; };
};

	/*	header file for the task system		*/

#define TIMER		1
#define TASK		2
#define QHEAD		4
#define QTAIL		5

#define IDLE		1
#define RUNNING		2
#define TERMINATED	4

#define DEDICATED	1
#define SHARED		2

#define CHAIN		1
#define VERBOSE		2
#define STACK		4

extern class task * clock_task;
extern class task * thistask;
extern long clock;

/*
typedef int (*PFI)();
extern PFI error_fct;
extern PFI exit_fct;
*/
extern int (*error_fct)();
extern int (*exit_fct)();

#define EMODE		1
#define WMODE		2
#define ZMODE		3

class object
{
	void		delete(void);

	class olink *	o_link;
public:
	class object *	o_next;
	int		o_type;

	void		new(int t = 0) { o_type=t; o_link=0; o_next=0; };
	void		remember(class task * t) { o_link = new class olink(t,o_link); };
	void		forget(class task *);
	void		alert(void);

	void		print(int);
};

class sched : public object
{
friend timer;
friend task;
friend object;
	void		schedule(void);
	void		insert(int,class object *);
	void		remove(void);

	long		s_time;
	int		s_state;
public:
	void		print(int);

	long		rdtime()	{ return s_time; };
	int		rdstate()	{ return s_state; };

	void		cancel(int);
	int		result(void);
};

class timer : public sched
{
	void		new(int);
	void		delete(void);
public:
	void		reset(int);
	void		print(int);
};

class task : public sched
{
friend sched;
	void		new(char* = 0, int = 0, int = 0);
	void		delete(void);

	void		save(void);
	void		restore(void);

	int*		t_framep;	/* WARNING: t_framep
					   is manipulated as an offset
					   by restore()
					*/
	int*		t_basep;
	int		t_size;
	int*		t_savearea;
	int		t_trap;
	class team *	t_team;

	int		t_mode;
	int		t_stacksize;

	class object *	t_alert;
public:
	class task *	t_next;
	char*		t_name;

	int		waitvec(class object **);
	int		waitlist();
	void		wait(class object * ob)	{ (void) waitlist(ob,0); };

	void		delay(int);
	int		preempt(void);
	void		sleep(void);
	void		resultis(int);

	void		print(int);
};



class qhead : public object
{
friend qtail;
	void            new(int=WMODE, int=10000);
	void		delete(void);

	int		qh_mode;
	class oqueue *	qh_queue;
public:
	class object *  get(void);
	int		putback(class object *);

	int	        rdcount()	{ return qh_queue->q_count; };
	int		rdmax()		{ return qh_queue->q_max; };
	int		rdmode()	{ return qh_mode; };

	class qtail *   tail(void);

	class qhead *   cut(void);
	void            splice(class qtail *);

	void		setmode(int m){ qh_mode = m; };
	void		setmax(int m)	{ qh_queue->q_max = m; };

	void		print(int);
};

class qtail : public object
{
friend qhead;
	void		new(int=WMODE, int=10000);
	void		delete(void);

	int		qt_mode;
	class oqueue *  qt_queue;
public:
	int		put(class object *);

	int		rdspace()	{ return qt_queue->q_max - qt_queue->q_count; };
	int		rdmax()		{ return qt_queue->q_max; };
	int		rdmode()	{ return qt_mode; };

	class qtail *   cut(void);
	void            splice(class qhead *);

	class qhead *   head(void);

	void		setmode(int m){ qt_mode = m; };
	void		setmax(int m)	{ qt_queue->q_max = m; };

	void		print(int);
};

class histogram
/*
	"nbin" bins covering the range [l:r[ uniformly
	nbin*binsize == r-l
*/
{
public:
	int l, r;
	int binsize;
	int nbin;
	int * h;
	long sum;
	long sqsum;
	void new(int=16, int=0, int=16);

	void add(int);
	void print(void);
};

/*	the result of randint() is always >= 0	*/

#define DRAW (randx = randx*1103515245 + 12345)
#define ABS(x)	(x&0x7fffffff)

#ifdef pdp11
#define MASK(x)	((x>>16)&077777)
#define MAX 32768.0
#endif

#ifdef vax
#define MASK(x) ABS(x)
#define MAX 2147483648.0
#endif

class randint
/*	uniform distribution in the interval [0,MAX] */
{
	long randx;
public:
	void	new(long s=0)	{ randx=s; };
	void	seed(long s)	{ randx=s; };
	int	draw()		{ return MASK(DRAW); };
	float	fdraw()		{ return ABS(DRAW)/MAX; };
};

class urand : public randint
/*	uniform distribution in the interval [low,high]	*/
{
public:
	int low, high;
	void new(int l, int h)	{ low=l; high=h; };
	int draw() { return (int) (low + (high - low) * (randint.draw()/MAX) ); };
};

extern double log();

class erand : public randint
/*	exponential distribution random number generator */
{
public:
	int mean;
	void new(int m) { mean=m; };
	int draw() { return (int) (-mean * log( (double) (MAX-randint.draw())/MAX) ); };
};
