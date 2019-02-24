/* usage: dispatch <ipc args> */
/* TODO: connect input to /dev/null if peer wants no output,
   connect output to logfile if peer wants no input.  */

#include <ipc.h>
#include <string.h>
#include "../paths.h"

int
main(int argc, char *argv[])
{
	char c, d;
	char *args[4];
	int fd;

	if (chdir(LDIR) < 0)
		return 1;

	(args[1] = strchr(argv[1], '=')) && ++args[1];
	(args[2] = strchr(argv[2], '=')) && ++args[2];
	args[3] = 0;

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
