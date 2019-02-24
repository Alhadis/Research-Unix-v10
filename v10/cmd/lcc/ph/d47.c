/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */


#include "flags.h"
#define LIB_TEST 1
#include "defs.h"  /* 92/05/04 moved here from line 18 */
#ifndef SKIP47
/*
 * 4.7 - Signal handling. Both signal() and raise() are tested here.
 * This file is not relevant for pre ANSI C.
 */

#if !ANSI
void d4_7() { }
#else

#include <signal.h>
void d4_7x(int);
void d4_7y(void);
static volatile sig_atomic_t flag = 0;
volatile sig_atomic_t flag_sig = 0;
void (*prev_fn)(int) = SIG_DFL;
static int num_sig[] = {SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM};
typedef void (*PIF)(int);
PIF d4_7b(int);
void d4_7b_2(int);

void d4_7()
	{
	int i, j;

	Filename = "d47.c";

	/* make sure that these are actually macros */
	#if !defined(SIG_DFL) || !defined(SIG_ERR) || !defined(SIG_IGN)
		complain(__LINE__);
	#elif !defined(SIGABRT) || !defined(SIGFPE) || !defined(SIGILL)
		complain(__LINE__);
	#elif !defined(SIGINT) || !defined(SIGSEGV) || !defined(SIGTERM)
		complain(__LINE__);
	#endif




	/* test that SIG modes are distinct and of type void (*)(int) */
	checkthat(__LINE__, SIG_DFL != SIG_ERR && SIG_DFL != SIG_IGN && SIG_ERR != SIG_IGN);
	{void (*p)(int) = SIG_DFL; checkthat(__LINE__, sizeof(void (*)(int)) == sizeof(SIG_DFL));}
	{void (*p)(int) = SIG_ERR; checkthat(__LINE__, sizeof(void (*)(int)) == sizeof(SIG_ERR));}
	{void (*p)(int) = SIG_IGN; checkthat(__LINE__, sizeof(void (*)(int)) == sizeof(SIG_IGN));}

	/* test that SIG numbers are positive and distinct */
	for (i = 0; i < 6; ++i)
		{
		checkthat(__LINE__, num_sig[i] > 0);
		for (j = 0; j < i; ++j)
			inotequals(__LINE__, num_sig[i], num_sig[j]);
		}



	/* 4.7.1.1 signal
	 * it can be set to a user function ... raise should change the flag
	 */
	signal(SIGINT, d4_7x);
	/* 4.7.2.1 raise
	 */
 	iequals(__LINE__, raise(SIGINT), 0);
 	iequals(__LINE__, flag, 2);

	/* after the raise, the signal handler MAY have set SIG_DFL */
	checkthat( - __LINE__, signal(SIGINT, d4_7x) == SIG_DFL);

	/* the signal can be set to ignore the raise */
	flag = 0;
	signal(SIGINT, SIG_IGN);
 	iequals(__LINE__, raise(SIGINT), 0);
 	iequals(__LINE__, flag, 0);
	
	/* the return from signal should be the "previous" value */
	signal(SIGINT, d4_7x);
	checkthat(__LINE__, d4_7x == signal(SIGINT, SIG_DFL));

	/* more extensive raise tests, with externally-linked subfunctions */
	checkthat(__LINE__, d4_7b(SIGABRT) != SIG_ERR);
	flag_sig = 0;
	raise(SIGABRT);
	checkthat(__LINE__, flag_sig == (sig_atomic_t)SIGABRT);
	prev_fn = signal(SIGABRT, SIG_DFL);
	checkthat( - __LINE__, prev_fn == SIG_DFL);





	checkthat(__LINE__, d4_7b(SIGTERM) != SIG_ERR);
	flag_sig = 0;
	raise(SIGTERM);
	checkthat(__LINE__, flag_sig == (sig_atomic_t)SIGTERM);
	prev_fn = signal(SIGTERM, SIG_DFL);
	checkthat( - __LINE__, prev_fn == SIG_DFL);

	/* test that signal returns the previous handler */
	prev_fn = signal(SIGTERM, SIG_IGN);
	prev_fn = signal(SIGTERM, SIG_DFL);
	checkthat(__LINE__, prev_fn == SIG_IGN);

	/* test that we can longjmp out of the signal handler */
	d4_7y();

	/* test that bad SIG numbers are diagnosed */
	/* (But test is only a warning, because signal numbers are implem-def'ed.) */
	checkthat( - __LINE__, SIG_ERR == signal(100+SIGABRT+SIGFPE+SIGILL+SIGINT+SIGSEGV+SIGTERM, SIG_IGN));
	}




void d4_7x(signo)
	int signo;
	{
	iequals(__LINE__, signo, SIGINT);
	flag = 2;
	}

/* make sure that a signal handler can do a longjmp */
#include <setjmp.h>
jmp_buf d7jbuf = {0};
void d4_7y()
	{
	signal(SIGINT, d4_7b_2);
	if (setjmp(d7jbuf) == 0)
		{
		raise(SIGINT);
		/* this line should never be reached */
		complain(__LINE__);
		}
	}
		
#endif

#else /* if SKIP47 */
void d4_7() { pr_skip("d4_7: SKIPPED ENTIRELY\n"); }
#endif
