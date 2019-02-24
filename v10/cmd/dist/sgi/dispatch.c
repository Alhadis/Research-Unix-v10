/* usage: dispatch <ipc args> */
/* TODO: connect input to /dev/null if peer wants no output,
   connect output to logfile if peer wants no input.  */

#include <string.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../paths.h"

int
main(int argc, char *argv[])
{
	char c, d;
	char *args[4];
	int fd, i, sz;
	struct sockaddr_in sin;
	struct hostent *h;
	char locname[16], remname[16];
	struct passwd *pw;

	if (chdir(LDIR) < 0)
		exit(1);

	sz = sizeof sin;
	if (getpeername(0, (struct sockaddr *) &sin, &sz) < 0)
		exit(1);
	if (ntohs(sin.sin_port) >= 1024)
		exit(1);
	if (!(h = gethostbyaddr(&sin.sin_addr, sizeof sin.sin_addr, sin.sin_family)))
		exit(1);	/* another silent exit */
	/* execute the rsh authentication protocol */
	if (read(0, locname, 1) != 1 || locname[0] != '\0')
		exit(1);
	for (i = 0; i < sizeof remname; ++i)
		if (read(0, remname + i, 1) == 1 && remname[i] == '\0')
			break;
	if (i == sizeof remname)
		exit(1);
	for (i = 0; i < sizeof locname; ++i)
		if (read(0, locname + i, 1) == 1 && locname[i] == '\0')
			break;
	if (i == sizeof locname)
		exit(1);
	if (strcmp(locname, remname) != 0)
		exit(1);
	if (!(pw = getpwnam(locname)))
		exit(1);
	if (ruserok(h->h_name, pw->pw_uid == 0, remname, locname) != 0)
		exit(1);
	write(0, "", 1);	/* a NUL byte */

	setgid(pw->pw_gid);
	setuid(pw->pw_uid);

	args[1] = h->h_name;
	args[2] = pw->pw_name;
	args[3] = 0;

	if (read(0, &c, 1) != 1)
		return 1;
	do
		if (read(0, &d, 1) != 1)
			return 1;
	while (d);

	dup2(0, 1);
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
