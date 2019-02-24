#ident "@(#)local.h	1.3 'attmail mail(1) command'"
#ident	"@(#)mailx:hdr/local.h	1.2.1.1"
/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)mailx:hdr/local.h	1.2"
/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 */

#ifdef V7
#include "v7.local.h"

#else
#ifdef V9
#include "v9.local.h"
#else
#include "usg.local.h"
#endif
#endif

