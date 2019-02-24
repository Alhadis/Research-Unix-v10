#include <sysexits.h>

dk_connect(dest, param)
char *dest, *param;
{
	ipcseterror(EX_SOFTWARE, "dk_connect not installed in libipc",
		"dk_connect");
	return -1;
}

dk_close(fd)
int fd;
{
	ipcseterror(EX_SOFTWARE, "dk_close not installed in libipc",
		"dk_close");
	return -1;
}
