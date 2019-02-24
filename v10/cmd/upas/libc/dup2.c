#include <fcntl.h>

int
dup2(a,b)
	int a;
	int b;
{
	if (a==b)
		return 0;
	close(b);
	return fcntl(a, F_DUPFD, b);
}
