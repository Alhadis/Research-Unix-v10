/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:usg.local.c	1.2"
#

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Local routines that are installation dependent.
 */

#include "rcv.h"


/*
 * Locate the user's mailbox file (ie, the place where new, unread
 * mail is queued).  In Standard UNIX, it is in /usr/mail/name.
 */

void
findmail()
{
	register char *cp;

	cp = copy(MAILROOT, mailname);
	copy(myname, cp);
	if (isdir(mailname)) {
		stradd(mailname, '/');
		strcat(mailname, myname);
	}
}

/*
 * Discover user login name.
 */

username(uid, namebuf)
	char namebuf[];
{
	register char *np;

	if (uid == getuid() && (np = getenv("LOGNAME")) != NOSTR) {
		strncpy(namebuf, np, PATHSIZE);
		return(0);
	}
	if( getname(uid, namebuf) == -1) {
		printf("Failure finding password file entry for uid %d\n",uid);
		return(-1);
	}
	else
		return(1);
}

char *
maildomain()
{
	return(MYDOMAIN);
}

#ifndef USG
long
ulimit(x,y)
{
	return(_NFILE);	/* hack */
}
#endif
