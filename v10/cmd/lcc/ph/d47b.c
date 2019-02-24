/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#ifndef SKIP47
#define LIB_TEST 1
/*
 * 4.7 - Signal handling (Continued)
 */
#include "defs.h"

#if ANSI
#include <signal.h>
extern volatile sig_atomic_t flag_sig;
extern void (*prev_fn)(int);
typedef void (*PIF)(int);

/* handler - record signal number in flag_sig */
static void handler(sig_no)
	int sig_no;
	{
	flag_sig = sig_no;
	}

/* d4_7b - establish handler for sig_no */
PIF d4_7b(sig_no)
	int sig_no;
	{
	flag_sig = 0;
	prev_fn = signal(sig_no, handler);
	return (prev_fn);
	}

#include <setjmp.h>
/* d4_7b_2 - a signal handler that longjmps out */
void d4_7b_2(sig_no)
	int sig_no;
	{
	extern jmp_buf d7jbuf;

	longjmp(d7jbuf, sig_no);	/* no-return is ok; see 3.6.6.4 */
	}
#endif	/* ANSI */

#endif	/* SKIP47 */
