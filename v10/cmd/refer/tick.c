/* time programs */
# include "stdio.h"
# include "sys/types.h"
# ifndef SVR2
# include "sys/timeb.h"
# endif
struct tbuffer {
	long	proc_user_time;
	long	proc_system_time;
	long	child_user_time;
	long	child_system_time;
};
static long start, user, system;
tick()
{
	struct tbuffer tx;
# ifndef SVR2
	struct timeb tp;
	ftime (&tp);
	times (&tx);
	start = tp.time*1000+tp.millitm;
# else
	start = times(&tx);
# endif
	user =  tx.proc_user_time;
	system = tx.proc_system_time;
}
tock()
{
	struct tbuffer tx;
	float lap, use, sys;
# ifndef SVR2
	struct timeb tp;
	ftime (&tp);
	times (&tx);
	lap = (tp.time*1000+tp.millitm-start)/1000.;
# else
	lap = (times(&tx)-start)/60.;
# endif
	if (start==0) return;
	use = (tx.proc_user_time - user)/60.;
	sys = (tx.proc_system_time - system)/60.;
	printf("Elapsed %.2f CPU %.2f (user %.2f, sys %.2f)\n",
		lap, use+sys, use, sys);
}
