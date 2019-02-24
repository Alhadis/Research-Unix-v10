#ident "@(#)uparm.h	1.3 'attmail mail(1) command'"
#ident	"@(#)mailx:hdr/uparm.h	1.3.1.1"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:hdr/uparm.h	1.3"
/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 */

extern char *libpath();

#ifdef preSVr4
# define LIBPATH          "/usr/lib/mailx"
#else
# define LIBPATH          "/usr/share/lib/mailx"
#endif
