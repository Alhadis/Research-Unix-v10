#ident "@(#)usg.local.c	1.4 'attmail mail(1) command'"
#ident	"@(#)mailx:usg.local.c	1.2.1.1"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:usg.local.c	1.2"

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Local routines that are installation dependent.
 */

#include "rcv.h"

/*
 * Locate the user's mailbox file (ie, the place where new, unread
 * mail is queued).  In SVr4 UNIX, it is in /var/mail/name.
 * In preSVr4 UNIX, it is in either /usr/mail/name or /usr/spool/mail/name.
 */
void
findmail()
{
	register char *cp;

	cp = copy(maildir, mailname);
	copy(myname, cp);
	if (isdir(mailname)) {
		strcat(mailname, "/");
		strcat(mailname, myname);
	}
}

/*
 * Discover user login name.
 */

username(uid, namebuf)
	uid_t uid;
	char namebuf[];
{
	register char *np;

	if (uid == myruid && (np = getenv("LOGNAME")) != NOSTR) {
		strncpy(namebuf, np, PATHSIZE);
		return(0);
	}
	return(getname(uid, namebuf));
}
