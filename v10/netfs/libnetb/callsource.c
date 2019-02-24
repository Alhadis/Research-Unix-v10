/*
 * figure out who called us
 * research version:
 * network services have a known environment
 * either
 *	CSOURCE="source=callingmachine ..."	(new service manager)
 * or
 *	CSOURCE="callingmachine"	(old manager, and some imitations)
 * or
 *	DKSOURCE="callingmachine.userid"	(very old manager)
 *
 * it may be pointless to support them all in one place
 */

extern char *getenv();
extern char *strchr(), *strrchr();

#define	NULL	0

char *
_rfcallsource()
{
	char *v;
	register char *p;

	if ((v = getenv("CSOURCE")) != NULL) {
		if (strncmp("source=", v, 7) == 0) {	/* new style */
			v += 7;
			if ((p = strchr(v, ' ')) != NULL)
				*p = 0;
			return (v);
		}
		/* old style */
		if ((p = strrchr(v, '!')) != NULL)
			*p = 0;
		return (v);
	}
	if ((v = getenv("DKSOURCE")) != NULL) {		/* even older style */
		if ((p = strchr(v, '.')) != NULL)
			*p = 0;
		return (v);
	}
	/* give up */
	return (0);
}
