#ifndef SYMBOL_H
#define SYMBOL_H
#ifndef UNIV_H
#include "univ.h"
#endif

#include "dtype.pub"

class Stmt; class SrcText;

struct Range {
	long	lo;
	long	hi;
};

class Symbol : public PadRcv {				// Symbol
	friend	SymTab;
	friend	Ed8SymTab;
	friend	CoffSymTab;
	Symbol	*hashlink;
public:
	int	disc();
	int	ok();
		Symbol(Symbol*,Symbol*,char*);
	Symbol	*parent;
	Symbol	*child;
	Symbol	*rsib;
	Symbol	*lsib;
	char	*_text;
virtual	char	*text(long=0);
	Range	range;
	char	*dump();
	Source	*source();
};

const LEAVE = 0, SHOW = 1, HIDE = 2;

class Var : public Symbol {				// Var
	friend	Expr;
	friend	UType;
	short	_disc;
	short	showorhide;
	Index	carte();
	void	reformat(long);
	char	*fmtlist();
	void	showutype(UType*);
public:
		Var(class SymTab*,class Block*,Var*,UDisc,char*);
		~Var();
	int	disc();
	int	ok();
	DType	type;
	void	show(int=LEAVE, Attrib=0);
};

class BlkVars : PadRcv {					// BlkVars
class	Block	*b;		// next block
	Var	*v;		// prev variable
PUBLIC(BlockVars,U_BLKVARS)
	Var	*gen();
		BlkVars(Block*i);
};

class Block : public Symbol {				// Block
	friend	BlkVars;
	friend	SymTab;
	friend	Ed8SymTab;
	friend	CoffSymTab;
	friend	MccSymTab;
	Var	*var;
PUBLIC(Block,U_BLOCK)
		Block(SymTab*,Symbol*,Block*,char*);
		~Block();
class	Stmt	*stmt;
};	

class Source : public Symbol {				// Source
	friend	Ed8SymTab;
	friend	CoffSymTab;
	friend	MccSymTab;
	Func	*linefunc;
	Stmt	*linestmt;
PUBLIC(Source,U_SOURCE)
		Source(SymTab*,Source*,char*,long);
		~Source();
	SrcText	*srctext;
	SymTab	*symtab;
	Block	*blk;
	Stmt	*stmtafter(int);
	char	*text(long=0);
};
	
class Func : public Symbol {				// Func
	friend	Ed8SymTab;
	friend	CoffSymTab;
	friend	MccSymTab;
	friend	TermCore;				// mac32 - goes away
	long	begin;
	long	size;
	long	lnnoptr;				// coff
	long	regsave;				// mac32 - goes away
	Block	*_blk;
	void	gather();
PUBLIC(Func,U_FUNC)
		Func(SymTab*,Source*,Func*,long,char*);
		~Func();
	Block	*blk();
	Block	*blk(long);
	DType	type;
	Range	lines;
	Stmt	*stmt(long);	
	char	*text(long=0);
	Var	*argument(int);
	int	regused(int);
};

#define Q_BPT   ((Expr*)1)	
class Stmt : public Symbol {				// Stmt
	friend	Ed8SymTab;
	friend	CoffSymTab;
	friend	Instr;
class	Process	*process;
class	Pad	*srcpad();
	void	error(char*);
	char	*contextsearch(char*,int);
PUBLIC(Stmt,U_STMT)
		Stmt(SymTab*,Block*,Stmt*);
		~Stmt();
	void	asmblr();
	void	select(long=0);
	char	*text(long=0);
	short	lineno;
	short	hits;
class	Expr	*condition;
	Bls	*condtext;
	void	dobpt(int);
	void	settrace();
	void	openframe();
	char	*kbd(char*);
	char	*help();
	void	conditional(Expr*);
	char	*srcline();
	Func	*func();
	char	*journal(Bls&);
};

class UType : public Symbol {				// UType
	friend	Ed8SymTab;
	friend	CoffSymTab;
	friend	MccSymTab;
	friend	TypMems;
	friend	Var;		// for symtab
	SymTab	*symtab;
	long	begin;
	short	size;
	char	*canspecial;
	Var	*mem;
	void	gather();
	void	display();
PUBLIC(UType,U_UTYPE)
		UType(SymTab*,long,long,char*);
		~UType();
	DType	type;
	Index	carte(enum Op);
	void	show(int=LEAVE, Attrib=0);
};

class TypMems : public PadRcv {				// TypMems
	char	pub_filler[8];
	UType	*ut;
	Var	*v;			// prev
PUBLIC(TypMems,U_TYPMEMS)
		TypMems(UType *);
	Var	*gen();
};

#endif
