#include <stdio.h>
#include <string.h>

	static char	SCCSID[] = "@(#)maphost.c	2.1 DKHOST 84/08/10";

	char		mh_hostname[64];
	static char	miscplace[128];
	extern char *	miscfield() ;

/*
 * Map DATAKIT VCS destination name to a dialstring
 *
 * The host name is mapped into a full DATAKIT address using
 * the file /etc/dkhosts unless it already looks like a full address
 * by having '/'s in it.  The host name must match the first field
 * of the /etc/dkhosts file and the service class specified (a single
 * ASCII character) must be in the listed classes.  The dialstring is
 * then constructed from the dialstring (from dkhosts), service
 * (defsufx or from miscfield, below), protocol (defprot or miscfield),
 * and parm.
 *
 * Another way of calling this routine is to provide a null host name,
 * in which case the routine will successively return all hosts that
 * match the service class.  When the end of the dkhosts file is reached,
 * a 0 pointer is returned.
 *
 *	host	= host name to match
 *		| 0 -- return all class matches
 *	service	= service class to match
 *	defsufx	= service to supply if none in host name or miscellany field
 *	defprot	= protocol string to supply if none in miscellany field
 *	parm	= parameter string to append
 *	return	= dialstring pointer
 *		| 0 -- if host == "" and EOF reached on /etc/dkhosts
 *	mh_hostname	: set to the matched host name if host == ""
 *	miscplace	: set for subsequent calls to miscfield()
 */

	char *
maphost(host, service, defsufx, defprot, parm)
	char		service;
	register char	*host, *defsufx, *defprot, *parm;
{
	register int	matched;
	char		*dotted, *wasdotted;
	char		hostentry[256], hostaddr[128], classes[64];
	static FILE	*hostfile;
	static char	dialstring[128];

	strcpy(hostaddr, host);
	if(dotted = strchr(hostaddr, '.'))
		*dotted++ = '\0';

	matched = 0;

	if(!strchr(hostaddr, '/')
	  && (hostfile || (hostfile = fopen("/etc/dkhosts", "r")) != NULL))

		while(fgets(hostentry, sizeof(hostentry), hostfile) != NULL)

			if(hostentry[0] != '#' &&
			  sscanf(hostentry, "%s %s %s %s",
			    mh_hostname, classes, dialstring, miscplace) == 4)

				if(strchr(classes, service)
				  && (!*host || strcmp(hostaddr, mh_hostname) == 0)){
					matched = 1;
					break;
				}

	if(hostfile && (*host || !matched)){
		fclose(hostfile);
		hostfile = NULL;
	}

	if(!matched){
		if(!*host)
			return((char *) 0);
		strcpy(dialstring, hostaddr);
		strcpy(miscplace, "-");
	}

	if(!strchr(dialstring, '.')){
		wasdotted = dotted;

		if(dotted || (dotted = miscfield(service, 's')))
			defsufx = dotted;

		strcat(dialstring, ".");
		strcat(dialstring, defsufx);

		if(!wasdotted){
			if(dotted = miscfield(service, 'p'))
				defprot = dotted;

			strcat(dialstring, ".");
			strcat(dialstring, defprot);
			strcat(dialstring, ".");
			strcat(dialstring, parm);
		}
	}

	dotted = &dialstring[strlen(dialstring)];

	while(*--dotted == '.')
		*dotted = '\0';

	return(dialstring);
}

/*
 * Parse Miscellany Field of /etc/dkhosts
 *
 * This routine is only called after a successful call to maphost()
 * and pokes around in the miscellany field saved at that time.
 * The subfields are separated by commas and have two-character names
 * constructed from the service class character and another character
 * appropriate to the subfield value.
 *
 *	class	= service class as matched by maphost()
 *	field	= character representing field name
 *	return	= field value string
 *		| 0 -- field not listed
 */

	char *
miscfield(class, field)
	char		class, field;
{
	register char	*next, *ptr;
	int		n;
	char		cs[16];
	static char	place[64];

	ptr = miscplace;

	sprintf(cs, "%c%c=", class, field);
	n = strlen(cs);

	do{
		if(next = strchr(ptr, ','))
			*next = '\0';

		if(strncmp(ptr, cs, n) == 0){
			strcpy(place, ptr+n);
			if(next)
				*next++ = ',';
			return(place);
		}

		if(next)
			*next++ = ',';
	}while(ptr = next);

	return((char *) 0);
}
