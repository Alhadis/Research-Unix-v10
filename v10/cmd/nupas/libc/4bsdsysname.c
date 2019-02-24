/* get the system's name -- 4bsd version*/

extern char *
sysname_read()
{
	static char sysname[128];

	gethostname(sysname, sizeof(sysname)-1);
	sysname[sizeof(sysname)-1] = '\0';
	if (sysname[0] == '\0')
		strcpy(sysname, "kremvax");

	return sysname;
}
