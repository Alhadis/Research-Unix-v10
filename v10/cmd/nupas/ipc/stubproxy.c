#include <sysexits.h>

proxy_connect(dest, param, prhost)
char *dest, *param, *prhost;
{
	ipcseterror(EX_SOFTWARE, "proxy_connect not installed in libipc",
		"proxy_connect");
	return -1;
}

proxy_close(fd)
int fd;
{
	ipcseterror(EX_SOFTWARE, "proxy_close not installed in libipc",
		"proxy_close");
	return -1;
}
