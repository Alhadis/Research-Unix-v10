#include "core.pri"
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/psl.h>
#include <sys/pte.h>
/*#include <sys/user.h>*/
#include "./user.h"
#include <sys/proc.h>
#include <sys/pioctl.h>

#define SYSADR		0x80000000		/* address of system seg. */
#define USRADR		(SYSADR-ctob(UPAGES))	/* last+1 address in stack seg. */

char *SignalName(int);
void Wait3();

class HostCore : public Core { friend SigMask; friend  HostProcess; friend HostContext; friend KernCore;
	proc	pr;
	union {
		user	struct_u;
		char	char_u[ctob(UPAGES)];
	} union_u;
	int	kmemfd;
	char	*uarea()		{ return union_u.char_u; }	
	user	*u()			{ return &union_u.struct_u; }
	char	*ioctl(int);
	char	*signalmask(long);
	char	*exechang(long e) 	{ return ioctl(e?PIOCSEXEC:PIOCREXEC); }
	char	*clrcurrsig();
	char	*sendsig(long);
	short	ppid()			{ return pr.p_ppid; }
	char	*stepoverVAXJSB();
	char	*pswT(long,int);
	char	*dostep(long,long,Trap*);
	char	*waitstop(long=0);

	char	*readwrite(long,char*,int,int);
	char	*read(long,char*,int);
	char	*write(long,char*,int);
virtual char	*seekto(int&,long&,int&);
virtual int	instack(long,long);
public:

		HostCore(Process *p, Master *m):(p, m) { kmemfd = -1; }
	Asm	*newAsm();
	Context	*newContext();
virtual	Behavs	behavs();
	Cslfd	*peek(long,Cslfd* =0);
	CallStk	*callstack();
	Frame	frameabove(long);
virtual	char	*eventname();
	char	*destroy();
	char	*laybpt(Trap*);
	char	*liftbpt(Trap*);
virtual	char	*open();
	char	*peekstring(long,char* =0);
	char	*poke(long,long,int);
	char	*pokedbl(long,double,int);
	char	*problem();
virtual	char	*readcontrol();
	char	*regname(int);
	char	*reopen(char*,char*);
	char	*resources();
	char	*run();
	char	*step(long=0,long=0);
	char	*popcallstack();
	char	*stop();
	int	REG_AP();
	int	REG_FP();
	int	REG_PC();
	int	REG_SP();
virtual	int	event();
virtual	long	regloc(int,int=0);
	long	saved(Frame*,int,int=0);
	long	apforcall(int);
	char	*docall(long,int);
	long	returnregloc();
};

class HostContext : public Context { friend HostCore;
	long	regs[17];				/* call psw r16 */
	HostCore
		*core;
	int	pending;
public:
		HostContext()		{}
	void	restore();
};

class KernCore : public HostCore {
	long	sbr;
	long	slr;
	long	intstack;
	pcb	pcb_copy;
	long	pcb_loc;
	char	*seekto(int&,long&,int&);
	int	instack(long,long);
public:
		KernCore(Process *p, Master *m):(p,m) {}
	long	cs_fp;
	long	regloc(int,int=0);
	long	pc();
	long	fp();
	char	*open();
	char	*readcontrol();
	Behavs	behavs();
	char	*eventname();
	int	event();
	char	*getpcb(long);
	char	*specialop(char*);
	char	*special(char*,long);
};

void WaitForExecHang(char*);
