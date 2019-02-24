/* Copyright 1989 by AT&T Bell Laboratories */
/* MIPS.dep.c
 *
 *    MIPS dependent code for SML/NJ runtime kernel.
 */

#include <signal.h>
#include "tags.h"


/* ghandle:
 *    Handler for GC signals.
 */
ghandle(sig,code,scp,addr) int sig, code, addr; struct sigcontext *scp;
{
    saved_pc = scp->sc_pc;
    scp->sc_pc = (int)saveregs;
    if(sig == SIGSEGV)
        cause = CAUSE_GC;
    else {
        fault_code = exnCode(sig,code);
        cause = CAUSE_FAULT;
    }
}


extern int handleprof();

setupsignals ()
{
  struct sigvec a;

  a.sv_handler = (void (*)()) ghandle;
  a.sv_mask	= sigmask(SIGINT);
  a.sv_onstack	= 0;
  a.sv_mask	= 0;
  sigvec(SIGINT,&a,0);
  sigvec(SIGFPE,&a,0);
  a.sv_handler = SIG_IGN;
  sigvec(SIGPIPE,&a,0);
  a.sv_handler = (int (*)()) handleprof;
  sigvec(SIGVTALRM,&a,0);
}
