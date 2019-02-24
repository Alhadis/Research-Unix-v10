/* Copyright 1989 by AT&T Bell Laboratories */
/* VAX.dep.c
 *
 *    VAX dependent code for the SML/NJ runtime kernel.
 */

#include <signal.h>
#include "tags.h"

extern int cause, fault_code;
extern int saved_pc;
extern int saveregs[];


int ghandle(sig,code,scp,addr) int sig, code, addr; struct sigcontext *scp;
{
#ifdef V9
#define FPE_INTOVF_TRAP K_INTOVF
    saved_pc = addr;  addr = (int)saveregs; 
#else
    saved_pc = scp->sc_pc; scp->sc_pc = (int)saveregs; 
#endif

 if (*(int*)(saved_pc-4) == 0x59585cc1 && code==FPE_INTOVF_TRAP)
       {cause=CAUSE_GC;
        saved_pc -= 4;	   
       }
 else if ((*(int*)saved_pc & 0xffffff) == 0x5958c0 && code==FPE_INTOVF_TRAP)
   {cause=CAUSE_GC;
/* the next line is temporary */
if ( (*(int*)(saved_pc-8) & 0xffff) == 0x5cc1 && code==FPE_INTOVF_TRAP) saved_pc-=8; else
    saved_pc -= 12;
   }
 else {fault_code=exnCode(sig,code); cause=CAUSE_FAULT;}
}

/* This next define is needed by ultrix systems */
#ifndef sigmask
#define sigmask(X) (1<<((X)-1))
#endif

extern int handleprof();

setupsignals()
{
#ifdef BSD
  struct sigvec a;
  int (**handlerp)() = (int (**)()) &a.sv_handler;
  *handlerp = ghandle;
  a.sv_onstack	= 0;
  a.sv_mask	= 0;
  sigvec(SIGINT,&a,0);
  sigvec(SIGFPE,&a,0);
  a.sv_handler = SIG_IGN;
  sigvec(SIGPIPE,&a,0);
  a.sv_handler = (int (*)()) handleprof;
  sigvec(SIGVTALRM,&a,0);
#endif
#ifdef V9
  signal(SIGINT,ghandle);
  signal(SIGFPE,ghandle);
  signal(SIGPIPE,SIG_IGN);
#endif
}
