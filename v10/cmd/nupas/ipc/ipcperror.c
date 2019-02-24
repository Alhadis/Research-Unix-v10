#include <stdio.h>
#include <string.h>

int ipcerrno;
char ipcerrstr[256];

extern int syserrno;
extern char syserrstr[128];
extern int ipcdebug;

extern char *ipclookup();
extern ipcgetsyserr();

/*
 * this routine records errors that the ipc routines themselves
 * have generated.  There is no need to check system errno's.
 */
ipcseterror(en, em, mess)
	int en;		/* the error number - should be from sysexits.h*/
	char *em;	/* the error msg associated with en */
	char *mess;	/* the error label */
{
	syserrno = en;
	ipcerrno = en;
	strncpy(syserrstr, em, sizeof(syserrstr));
	sprintf(ipcerrstr, "%s: %s", mess, syserrstr);
	if (ipcdebug)
		fprintf(stderr, "ipcseterror %d %s %s\n",
			en, em, mess);
}

/*
 * ipcsyserr determines what system error just occurred, and
 * translates it to the sysexits.h error class.  The text is
 * saved for possible posterity.  NB: the translation is highly
 * machine-dependent, so each OS needs its own ipclookup routine.
 *
 */
ipcsyserr(mess)
	char *mess;
{
	extern char syserrstr[];

	ipcgetsyserr();
	ipcerrno = ipcmaperror();
	sprintf(ipcerrstr, "%s: %s", mess, syserrstr);
}


ipcperror(mess)
char *mess;
{
	fprintf(stderr, "%s: %s\n", mess, ipcerrstr);
}
