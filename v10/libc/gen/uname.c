/*
 *	uname(s)
 *	struct utsname *s;
 *
 *	tell something about which system version we're on
 */

#include <utsname.h>

#define WHOFILE "/etc/whoami"

int
uname(s)
	struct utsname *s;
{
	static struct utsname clear;

	register int fd, i;

	*s = clear;

	/* read the system identity */
	fd = open (WHOFILE, 0);
	if (fd < 0)
		return -1;
	read (fd, s->nodename, sizeof (s->nodename) - 1);
	close(fd);

	/* find the trailing \n if any and make it \0 */
	for (i = 0; i < sizeof (s->nodename); i++)
		if (s->nodename[i] == '\n')
			s->nodename[i] = '\0';
	
	/* provide additional identifying information */
	strcpy (s->sysname, s->nodename);
	strcpy (s->release, "2");
	strcpy (s->version, "9th edition");

	return 0;
}
