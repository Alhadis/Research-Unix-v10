#ifndef UNIV_H
#define UNIV_H

enum Op;
enum EDisc;
class Asm;
class Block;
class Bls;
class Bpts;
class BptReq;
class CallStk;
class Context;
class Core;
class CoreContext;
class Cslfd;
class DType;
class Expr;
class Frame;
class Func;
class Globals;
class HostContext;
class HostCore;
class HostMaster;
class HostProcess;
class Index;
class Journal;
class KernCore;
class KernMaster;
class KernProcess;
class Master;
class Memory;
class Pad;
class Phrase;
class Process;
class Remote;
class M68kCore;
class SigBit;
class SigMask;
class SnetContext;
class SnetCore;
class SnetMaster;
class SnetProcess;
class Source;
class SrcDir;
class Stmt;
class SymTab;
class Symbol;
class TermCore;
class TermMaster;
class TermProcess;
class Trap;
class UType;
class Var;
class Var;
class WerkCore;
class WerkMaster;
class WerkProcess;
class WerkContext;
enum Behavs;

int strcmp(char*,char*);
inline int eqstr(char *a, char *b)
{
	return !a ? !b : b && *a==*b && !strcmp(a+1,b+1);
}


class Cslfd {	friend HostCore; friend TermCore; friend M68kCore;
	 void	init(long l, double d);
	 char	*flterr;
public:
	 double	dbl;
	 float	flt;
	 long	lng;
	 short	sht;
	 char	chr;
unsigned char	uch();
unsigned short	ush();
	 char	*floaterror();
	 	Cslfd(long);
	 	Cslfd(double);
		Cslfd(int);
		Cslfd();
};

enum UDisc {
	U_ERROR		= 0,

	U_FUNC		= 1,
	U_GLB		= 2,
	U_STA		= 3,
	U_STMT		= 4,
	U_UTYPE		= 5,
#				define TOSYM 0x7	/* 2^n-1 */
	U_ARG		= 10,
	U_AUT		= 11,
	U_BLOCK		= 12,
	U_FST		= 13,
	U_MOT		= 14,
	U_SOURCE	= 15,

	U_ASM		= 20,
	U_AUDIT		= 21,
	U_BLKVARS	= 22,
	U_BPTS		= 23,
	U_CELL		= 24,
	U_CORE		= 25,
	U_DTYPE		= 26,
	U_EXPR		= 27,
	U_MASTER	= 28,
	U_GLOBALS	= 29,
	U_INSTR		= 30,
	U_MEMORY	= 32,
	U_PHRASE	= 33,
	U_PROCESS	= 34,
	U_REG		= 35,
	U_REGCELL	= 36,
	U_REGFILE	= 37,
	U_SRCFUNCS	= 38,
	U_SRCTEXT	= 39,
	U_STABSRCS	= 40,
	U_SYMTAB	= 41,
	U_TRAP		= 42,
	U_TYPMEMS	= 43,
	U_WD		= 44,
	U_SIGMASK	= 45,
	U_HELP		= 46,
	U_SRCDIR	= 47,
	U_CONTEXT	= 48,
	U_HOSTMASTER	= 49,
	U_TERMMASTER	= 49,
	U_SIGBIT	= 50,
	U_BPTREQ	= 51,
	U_JOURNAL	= 52,
	U_FRAME		= 53,
};

#include "lib.h"
#include <pads.pub>

#define PUBLIC(c,d)\
	int	disc()	{ return d; }\
public:\
	int	ok()	{ return this && disc() == d; }


#define VACUOUS		{ abort(); }
#define NOVALUE		{ abort(); return x; }

#define SVP '3.14'	/* != 0 */
#endif
