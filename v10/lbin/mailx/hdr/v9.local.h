/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:hdr/usg.local.h	1.4"
/*
 * Declarations and constants specific to an installation.
 */
 
/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 */

#define	USR_SPOOL_MAIL
#define	LOCAL		EMPTYID		/* Dynamically determined local host */
#define	GATEWAY		"/usr/lib/upas/gateway"	/* where to find gateway host */
#define WHOAMI		"/etc/whoami"	/* file w/ name of local host, if it exists */
#define MYDOMAIN	".att.com"	/* Appended to local host name */
#define	MAIL		"/bin/mail"	/* Mail delivery agent */
#define	EDITOR		"ed"		/* Name of text editor */
#define	VISUAL		"vi"		/* Name of display editor */
#define PG		(value("PAGER") ? value("PAGER") : "p")
					/* Standard output pager */
#define	LS		(value("LISTER") ? value("LISTER") : "ls")
					/* Name of directory listing prog*/
#define	SHELL		"/bin/sh"	/* Standard shell */
#define	HELPFILE	libpath("mailx.help")
					/* Name of casual help file */
#define	THELPFILE	libpath("mailx.help.~")
					/* Name of casual tilde help */
#define	MASTER		libpath("mailx.rc")
#define MAILROOT	"/usr/spool/mail/"
					/* directory where mail log file lives */
#define	APPEND				/* New mail goes to end of mailbox */
#define CANLOCK				/* Locking protocol actually works */
#define	UTIME				/* System implements utime(2) */
