#include	"mk.h"
#include	<signal.h>

static
sigint()
{
	extern errno;

	signal(SIGINT, SIG_IGN);
	kflag = 1;	/* to make sure waitup doesn't exit */
	jobs = 0;	/* make sure no more get scheduled */
	while(waitup(1, (int *)0) == 0)
		;
	Fprint(1, "mk: interrupted!\n");
	Exit();
}

sigcatch()
{
	if(signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, sigint);
}
