/*#include "../protocol.h"*/
#include "core.pri"
#include "mcc.h"

class M68kCore : public Core { friend SnetCore; friend WerkCore;
virtual	char	*readwrite(long,char*,int,int);
	char	*read(long,char*,int);
	char	*write(long,char*,int);
virtual	long	savearea();
virtual char	*seekto(int&,long&);
	long	stack_lo;
	long	stack_hi;
	exec	hdr;
public:
		M68kCore(Process*,Master*);
	Asm	*newAsm();
	Context	*newContext();
virtual	Behavs	behavs();
	Cslfd	*peek(long,Cslfd* = 0);
	CallStk	*callstack();
	Frame	frameabove(long);
	char	*eventname();
	char	*laybpt(Trap*);
	char	*liftbpt(Trap*);
	char	*open();
	char	*peekstring(long,char* =0);
	char	*poke(long,long,int);
	char	*problem();
	char	*readcontrol();
	char	*regname(int);
	char	*resources();
	char	*run();
	char	*step(long=0,long=0);
	char	*stop();
	int	REG_FP();
	int	REG_SP();
	int	REG_AP();
	long	regloc(int,int=0);
	long	pc();
	long	saved(Frame*,int,int=0);
	long	apforcall(int);
	char	*docall(long,int);
	long	returnregloc();
};

class SnetCore : public M68kCore {
virtual	long	savearea();
virtual char	*seekto(int&,long&);
public:
		SnetCore(Process*,SnetMaster*);
};

class WerkCore : public M68kCore {
virtual	long	savearea();
virtual char	*seekto(int&,long&);
public:
		WerkCore(Process*,WerkMaster*);
};

class SnetContext : public Context { friend M68kCore;
	M68kCore	*core;
public:
			SnetContext()		{}
};
