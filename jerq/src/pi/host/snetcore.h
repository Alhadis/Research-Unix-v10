#include "../protocol.h"
#include "core.pri"

class SnetCore : public Core { friend SnetContext;
	char	*read(long,char*,int);
	char	*write(long,char*,int);
public:
		SnetCore(Process*,SnetMaster*);
	Asm	*newAsm();
	Context	*newContext();
	Behavs	behavs();
	Cslfd	*peek(long,Cslfd* = 0);
	CallStk	*callstack();
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
	int	REG_PC();
	int	REG_SP();
	long	regloc(int,int=0);
	long	pc();
	long	saved(Frame*,int,int=0);
	long	apforcall(int);
	char	*docall(long,int);
	long	returnregloc();
};

class SnetContext : public Context { friend SnetCore;
	SnetCore
		*core;
public:
		SnetContext()		{}
};
