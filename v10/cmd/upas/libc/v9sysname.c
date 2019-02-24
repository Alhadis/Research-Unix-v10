/* get the system's name -- v10 version*/

#define WHOFILE "/etc/whoami"

extern char *
sysname_read()
{
	register int fd, i, n;
	static char sysname[128];

	/* read the system identity */
	fd = open(WHOFILE, 0);
	if (fd < 0)
		return "kremvax";
	n = read(fd, sysname, sizeof(sysname)-1);
	close(fd);
	if (n < 0)
		return "kremvax";

	sysname[n] = '\0';

	/* find the trailing \n if any and make it \0 */
	for (i = 0; i < n; i++)
		if (sysname[i] == '\n')
			sysname[i] = '\0';

	return sysname;
}
