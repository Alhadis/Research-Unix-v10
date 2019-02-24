/* Copyright 1989 by AT&T Bell Laboratories */
/* SPARC.dep.c
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY  14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created (extracted from callgc.c and run.c)
 *
 *    SPARC dependent code for SML/NJ runtime kernel.
 */

#include <signal.h>
#include <floatingpoint.h>
#include "tags.h"

/* When handling signals we need to know the context of the signal.  We view
 * execution as being at one of two levels, and use the flag inML to distinguish
 * the levels:
 *    0 -- in C code above ML
 *    1 -- in ML code or in C/asm code called from ML.
 * This is adjusted by the assembler code that implements the C/ML interface.
 */
int		inML = 0;	   /* == 1 when execution is in ML code */
int		fault_pending = 0; /* == 1 when a fault is pending in level 0 */


extern int cause, fault_code;
extern int saved_pc;
extern int saveregs[];


/* ghandle:
 *    Handler for GC signals.  This is based on what level execution is in.
 * If a fault occurs in level 0 we note it and invoke the exn handler the next
 * time ML code is run.  If a fault occurs in level 1 we need to determine if
 * it is in ML code or in C/asm code called from ML.  If it is in ML code then
 * we transfer control directly to the exn handler.  If it is in C/asm code,
 * then we call ml_longjmp() (in SPARC.prim.s) to restore the ML register window.
 */
ghandle(sig, code, scp, addr)
    int sig, code, addr;
    struct sigcontext *scp;
{
    extern	datalist, startprim, endprim, endmo, etext, ml_longjmp;

    saved_pc    = scp->sc_pc;

    if ((sig == SIGEMT) && (code == EMT_TAG)) {
      /* GC related fault.  There are three possible code sequences that may
       * trigger GC (where n is an integer and %r is a temp register).
       *
       *   taddcctv %g6,%g4,%g0	    1000 0001 0001 0001 1000 0000 0000 0100
       *
       *   add      %g4,n,%r	    10rr rrr0 0000 0001 001n nnnn nnnn nnnn
       *   taddcctv %g6,%r,%g0	    1000 0001 0001 0001 1000 0000 000r rrrr
       *
       *   sethi    %hi(n),%r	    00rr rrr1 00nn nnnn nnnn nnnn nnnn nnnn
       *   or       %r,%lo(n),%r    10rr rrr0 0001 0rrr rr1n nnnn nnnn nnnn
       *   add      %g4,%r,%r	    10rr rrr0 0000 0001 0000 0000 000r rrrr
       *   taddcctv %g6,%r,%g0	    1000 0001 0001 0001 1000 0000 000r rrrr
       */
	register unsigned int *pc = (unsigned int *)saved_pc;
	register int pc_adjust;

	if (pc[0] == 0x81118004 /* taddcctv %g6,%r,%g0 */)
	    pc_adjust = 0;
	else if ((pc[0] & 0xffffffe0) == 0x81118000 /* taddcctv %g6,%r,%g0 */)
	    pc_adjust = (pc[-1] & 0x00002000 /* immed flag */) ? 4 : 12;
	else
	    die ("SIGEMT not related to gc\n");
	if (((int)pc)+4 != scp->sc_npc)
	    die ("SIGEMT not related to garbage collection (bogus npc)\n");
	saved_pc    = ((int)pc) - pc_adjust;
	scp->sc_pc  = (int)saveregs;
	scp->sc_npc = ((int)saveregs)+4;
        cause = CAUSE_GC;
    }
    else if (! inML) {
      /* execution above ML */
	if (sig != SIGINT)
	    die ("Unknown signal %d (%#x)\n", sig, code);
	fault_pending = 1;
	fault_code = exnCode(sig, code);
    }
    else {
	fault_code = exnCode(sig, code);
	cause = CAUSE_FAULT;

      /* Check to see if we are in ML code or in C/asm called from ML */
	if (((int)&etext <= saved_pc)
	|| (((int)&startprim <= saved_pc) && (saved_pc < (int)&endprim))
	|| (((int)&datalist != ML_NIL) && ((int)&datalist <= saved_pc)
	   && (saved_pc <= (int)&endmo)))
	{
	  /* Execution is in ML code */
	    scp->sc_pc  = (int)saveregs;
	    scp->sc_npc = ((int)saveregs)+4;
	}
	else {
	  /* Execution is in C/asm code called from ML. */
	    if (sig != SIGINT)
		die ("Unknown signal %d (%#x)\n", sig, code);
	    scp->sc_pc  = (int)&ml_longjmp;
	    scp->sc_npc = ((int)&ml_longjmp)+4;
	}
    }
}

extern int handleprof();
extern void set_fsr();

/* setupsignals:
 *   Install the signal handlers.
 */
setupsignals()
{
  struct sigvec a;

  a.sv_handler = (void (*)()) ghandle;
  a.sv_mask	= sigmask(SIGINT);
  a.sv_onstack	= 0;
  sigvec(SIGEMT,&a,0);
  a.sv_mask	= 0;
  sigvec(SIGINT,&a,0);
  set_fsr (0x0f000000); /* enable FP exceptions NV, OF, UF & DZ */
  sigvec(SIGFPE,&a,0);
  a.sv_handler = SIG_IGN;
  sigvec(SIGPIPE,&a,0);
  a.sv_handler = (void (*)()) handleprof;
  sigvec(SIGVTALRM,&a,0);
}

/**** TEMP ****/
char *malloc(){die("malloc"); return 0;}
char *realloc(){die("realloc"); return 0;}
char *calloc(){die("calloc"); return 0;}
int free(){die("free");}
int cfree(){die("cfree");}
