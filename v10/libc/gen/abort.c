#include <signal.h>

extern int kill(), getpid();

int
abort()
{
	return(kill(getpid(), SIGIOT));
}
