/*
 * libpath - calculate path to prefer library
 *
 * Alan Hastings 4/13/87
 *
 * Added for EXPTOOLS support
 */

char *libpath(deflib)
char *deflib;
{
	register char *retval;
	char *malloc(), *getenv(), *strcpy(), *strcat();

	if ((retval = getenv("PHOME")) != (char *)0)
		return retval;

#ifdef EXPTOOLS
# define PREFLIB "/lib/prefer"
	if ((retval = getenv("TOOLS")) != (char *)0)
	{
		char *libuf;

		libuf = malloc((unsigned)(strlen(retval)+sizeof(PREFLIB)+1));
		(void) strcpy(libuf, retval);
		(void) strcat(libuf, PREFLIB);
		return libuf;
	}
#endif /* EXPTOOLS */

	return deflib;
}
