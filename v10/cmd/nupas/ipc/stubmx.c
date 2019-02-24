#include <sysexits.h>

mx_connect(dest, param)
char *dest, *param;
{
	ipcseterror(EX_SOFTWARE, "mx_connect not installed in libipc",
		"mx_connect");
	return -1;
}
