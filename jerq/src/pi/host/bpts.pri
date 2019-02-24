#ifndef BPTS_H
#define BPTS_H
#ifndef UNIV_H
#include "univ.h"
#endif

class Core;

enum LiftLay { LIFT, LAY };

class Trap : PadRcv { friend Bpts; friend TermCore; friend HostCore;
	long	key;
	char	saved;
	Stmt	*stmt;
	char	*error;
	Trap	*sib;
	char	*liftorlay(LiftLay,Core*);
PUBLIC(Trap,U_TRAP)
		Trap(Stmt*, Trap *);
};
class Bpts : public PadRcv {
	friend	HostProcess; friend Process;
	Pad	*pad;
	Core	*core;
	Trap	*trap;
	int	layed;
	Trap	*istrap(Stmt*);
	void	select(Trap*);
	void	clearall();
	void	refresh();
	void	liftparents(Bpts*);
PUBLIC(Bpts,U_BPTS)
		Bpts(Core*);
	void	lift();
	void	lay();
	void	set(Stmt*);
	void	clr(Stmt*);
	int	isbpt(Stmt*);
	int	isasmbpt(long);
	Stmt	*bptstmt(long);
	void	hostclose();
	void	banner();
};

enum BegEnd { BEGIN = 0x1, END = 0x2 };

class BptReq : public PadRcv {
	char	*file;
	char	*func;
	BegEnd	be;
	long	line;
	Expr	*expr;
	char	*error;
	char	*setfunc(Process*);
	char	*setline(Process*);
	void	parse(char*);
PUBLIC(BptReq,U_BPTREQ)
	char	*set(Process*);
		BptReq(char*, long,  char* =0);
		BptReq(char*, char*, char* =0);
};
#endif
