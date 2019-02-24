#include <sysexits.h>

tcp_connect(dest, param)
char *dest, *param;
{
	ipcseterror(EX_SOFTWARE, "tcp_connect not installed in libipc",
		"tcp_connect");
	return -1;
}

tcp_close(fd)
int fd;
{
	ipcseterror(EX_SOFTWARE, "tcp_close not installed in libipc",
		"tcp_close");
	return -1;
}
