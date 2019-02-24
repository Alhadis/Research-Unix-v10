/* usage: dispatch <ipc args> */
/* TODO: connect input to /dev/null if peer wants no output,
   connect output to logfile if peer wants no input.  */

#include <sys/types.h>
#include <sys/sockets.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "../paths.h"

int
main(int argc, char *argv[])
{
	char c, d;
	char *args[3];
	int fd;

	if (chdir(LDIR) < 0)
		return 1;

	/* TODO: at this point, perform rshd style authentication check,
	   set the appropriate user id, and all that.  See what kind of
	   args we can get from inetd.  */

	if (read(0, &c, 1) != 1)
		return 1;
	do
		if (read(0, &d, 1) != 1)
			return 1;
	while (d);

	fd = open("/dev/null", 1);
	dup2(fd, 2);
	if (fd > 2)
		close(fd);

	switch (c) {
	case 's':
		args[0] = "showq";
		execv("showq", args);
		break;
	case 't':
		args[0] = "transmit";
		execv("transmit", args);
		break;
	case 'n':
		args[0] = "notice";
		execv("notice", args);
		break;
	}

	return 1;
}
