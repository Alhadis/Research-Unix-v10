#include <signal.h>
#include <setjmp.h>

static jmp_buf jmp;

sleep(n)
unsigned n;
{
	int sleepx();
	unsigned altime;
	unsigned sltime;
	int (*alsig)() = SIG_DFL;

	if (n==0)
		return;
	altime = alarm(1000);	/* time to maneuver */
	if (setjmp(jmp)) {
		signal(SIGALRM, alsig);
		if (sltime) {
			kill(getpid(), SIGALRM);
			sleep(sltime);
			return;
		}
		alarm(altime);
		return;
	}
	sltime = 0;
	if (altime) {
		if (altime < n) {
			sltime = n - altime;
			n -= altime;
		} else {
			altime -= n;
			if (altime == 0)
				altime = 1;
		}
	}
	alsig = signal(SIGALRM, sleepx);
	alarm(n);
	for(;;)
		pause();
	/*NOTREACHED*/
}

static
sleepx()
{
	longjmp(jmp, 1);
}
