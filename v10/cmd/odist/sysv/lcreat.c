#include <fcntl.h>

int
lcreat(char *name, int mode)
{
	return open(name, O_EXCL | O_CREAT | O_TRUNC | O_WRONLY, mode);
}
