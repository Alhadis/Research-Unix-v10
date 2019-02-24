#ifndef ASM_H
#define ASM_H
#ifndef UNIV_H
#include "univ.h"
#endif

class Instr : public PadRcv {
PUBLIC(Instr,U_INSTR)
	long	addr;
	long	next;
	Asm	*_asm;
	int	fmt;
	Cslfd	m;
	char	bpt;
	char	reg;
	char	opcode;

	Index	carte();
	void	reformat(int);
	Instr	*sib;
	char	*literal(long);
	char	*symbolic(char* ="");
	Var	*local(UDisc, long);
	Var	*field(Var*, long);
	char	*regarg(char*, long);
	void	succ(int);
	void	memory();
	void	dobpt(int);
	void	showsrc();
	void	openframe();
	void	display();

virtual	char	*arg(int);
virtual	char	*mnemonic();
virtual	int	argtype(int);
virtual	int	nargs();
	
		Instr(Asm*,long);
};

class Asm : public PadRcv { friend Instr; friend VaxAsm; friend Mac32Asm;
			    friend VaxInstr; friend Mac32Instr; friend M68kInstr;
	int	fmt;
	Core	*core;
	Pad	*pad;
	Instr	*instrset;
	void	instrstep(long);
	void	stepover();
	void	displaypc();
	void	go();
virtual char	*literaldelimiter();
virtual Instr	*newInstr(long l);
PUBLIC(Asm,U_ASM)
		Asm(Core*);
	char	*kbd(char *);
	char	*help();
	void	userclose();
	void	open(long=0);
	void	banner();
};
class VaxAsm : public Asm {
public:
	char	*literaldelimiter();
	Instr	*newInstr(long);
		VaxAsm(Core*);
};

class Mac32Asm : public Asm {
public:
	char	*literaldelimiter();
	Instr	*newInstr(long);
		Mac32Asm(Core *);
};

class M68kAsm : public Asm {
public:
	char	*literaldelimiter();
	Instr	*newInstr(long);
		M68kAsm(Core*);
};
#endif
