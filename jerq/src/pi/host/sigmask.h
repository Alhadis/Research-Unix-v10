#ifndef SIGMASK_H
#define SIGMASK_H
#ifndef UNIV_H
#include "univ.h"
#endif

class SigMask : public PadRcv { friend SigBit; friend HostProcess;
	HostCore
		*core;
	long	mask;
	Pad	*pad;
	long	exechang;

	long	bit(long s)		{ return 1<<(s-1); }
	void	signalmask(long);
	void	setsig(long);
	void	clrsig(long);
	void	clrcurrsig(long);
	void	sendsig(long);
	void	open();
	void	execline(long);
	void	updatecore(char* =0);
PUBLIC(SigMask,U_SIGMASK)
		SigMask(HostCore*);
	void	linereq(long,Attrib=0);
	void	hostclose();
	void	banner();
};

class SigBit : public PadRcv { friend SigMask;
	int	bit;
	void	set(SigMask*);
	void	clr(SigMask*);
	void	send(SigMask*);
		SigBit() {}
};
