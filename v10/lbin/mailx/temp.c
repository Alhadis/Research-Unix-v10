#ident "@(#)temp.c	1.4 'attmail mail(1) command'"
#ident	"@(#)mailx:temp.c	1.5.1.1"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:temp.c	1.5"

#include "rcv.h"
#include <pwd.h>
#ifdef preSVr4
extern struct passwd *getpwnam();
extern struct passwd *getpwuid();
#endif

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Give names to all the temporary files that we will need.
 */

void
tinit()
{
/*	char usrname[PATHSIZE];			why is this here??  adb */
	register int err = 0;
	register pid_t pid = mypid;

	sprintf(tempMail, "/tmp/Rs%-ld", pid);
	sprintf(tempResid, "/tmp/Rq%-ld", pid);			/* adb */
	sprintf(tempQuit, "/tmp/Rm%-ld", pid);
	sprintf(tempEdit, "/tmp/Re%-ld", pid);
	sprintf(tempSet, "/tmp/Rx%-ld", pid);
	sprintf(tempMesg, "/tmp/Rx%-ld", pid);
	sprintf(tempZedit, "/tmp/Rz%-ld", pid);

	/* check the uid if myname was set by -u */
	if (strlen(myname) != 0) {
		struct passwd *pwd = getpwnam(myname);
		if (!pwd) {
			printf("\"%s\" is not a user of this system\n",
			    myname);
			exit(1);
		}
		else
			uid = pwd->pw_uid;
		endpwent();
	}
	/* get the name associated with this uid */
	else {
		struct passwd *pwd = getpwuid(uid = myruid);
		if (!pwd) {
			copy("ubluit", myname);
			err++;
			if (rcvmode) {
				printf("Who are you!?\n");
				exit(1);
			}
		}
		else
			copy(pwd->pw_name, myname);
		endpwent();
	}
	strcpy(homedir, Getf("HOME"));
	findmail();
	assign("MBOX", Getf("MBOX"));
	assign("MAILRC", Getf("MAILRC"));
	assign("DEAD", Getf("DEAD"));
	assign("save", "");
	assign("asksub", "");
	assign("header", "");
}
