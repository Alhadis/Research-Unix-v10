#ident "@(#)sigretro.h	1.3 'attmail mail(1) command'"
#ident	"@(#)mailx:hdr/sigretro.h	1.5.1.1"
#ident	"@(#)mailx:hdr/sigretro.h	1.5"

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Define extra stuff not found in pre-SVr4 signal.h
 */

#ifndef SIG_HOLD

#define	SIG_HOLD	(int (*)()) 3		/* Phony action to hold sig */
#endif /* SIG_HOLD */
