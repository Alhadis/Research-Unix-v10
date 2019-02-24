/* get the system's name -- System V */

#include <utsname.h>
#include <string.h>

extern char *
sysname_read()
{
	static struct utsname s;

	if (uname(&s)<0 || strcmp(s.nodename, "") == 0)
		return "kremvax";
	return s.nodename;
}
