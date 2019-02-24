#ifndef SYMTAB_H
#define SYMTAB_H
#ifndef UNIV_H
#include "univ.h"
#endif

#include "mip.h"
extern int FunctionGathered, UTypeGathered, FunctionStubs, UTypeStubs;
extern int IdToSymCalls, StrCmpCalls;

class SSet {
	friend	SymTab;	friend LookupCache;
	char	v[8];
public:
		SSet(char=0);
		SSet(char,char,char=0,char=0,char=0,char=0,char=0);
};

class LookupCache {
	SSet	set;
	Symbol	*sym;
	long	loc;
	char	*id;
public:
		LookupCache() {}
	Symbol	*match(SSet, long);
	void	save(SSet, long, Symbol*);
};

class SymTab : public PadRcv {
	friend Ed8SymTab; friend CoffSymTab; friend MccSymTab; friend Core;
	friend	TermCore;		/* rob's system problem */
	Pad	*_pad;
	int	fd;
	char	*strings;
	long	strsize;
	long	entries;
	long	_magic;
	long	relocation;
#define HASH	 	101 /* prime */
	Symbol	*hashtable[TOSYM+1][HASH];
	Core	*_core;
	char	*stabpath();
	Source	*_root;
	Block	*fakeblk();
	char	*dump();
	Block	*_blk;
	char	*_warn;
	Index	castix[UNDEF];	// only [STRTY] [ENUMTY] used
	UType	*utype;
	SymTab	*inherit;
	Var	*globregs(Block*, int);
	void	uncfront(Var *, char*);
	void	showutype(UType*);
	LookupCache
		loctosymcache;
virtual	char	*gethdr()		{ return "SymTab.gethdr"; }
virtual	Source	*tree()			{ return 0; }
PUBLIC(SymTab,U_SYMTAB)
		SymTab(Core*,int,SymTab* =0,long=0);
virtual		~SymTab();
	void	read();
	void	enter(Symbol*);
	Symbol	*idtosym(SSet,char*,int=1);
	Symbol	*loctosym(SSet,long,int=1);
	Pad	*pad();
	Core	*core();
	char	*symaddr(long);
	Source	*root();
	long	modtime();
	Block	*blk();
	char	*warn();
	Index	utypecarte(short);
	long	magic();
	UType	*utypelist();
	void	banner();
virtual	Block	*gatherfunc(Func*);
virtual	Var	*gatherutype(UType*);
	void	opentypes();
};
struct nlist; struct exec; struct mlist;

class Ed8SymTab : public SymTab {
	exec	*hdr;
	nlist	*base;
	nlist	*symoff;
	DType	gatherdtype(nlist *);
	DType	chain(int, nlist *);
	void	gathervar(nlist*, Var**, Block*, UDisc);
	int	isastring(char*);
	char	*gettbl();
	nlist	*nlistvector(long,long);
	Source	*tree();
	char	*gethdr();
public:
		Ed8SymTab(Core*,int,SymTab* =0);
		~Ed8SymTab();
	Block	*gatherfunc(Func*);
	Var	*gatherutype(UType*);
};

class SymEnt;	struct scnhdr;	struct aouthdr;	struct filehdr;
#define SCNHDRS 8

class CoffSymTab : public SymTab {
	filehdr	*fhdr;
	aouthdr	*ahdr;
	scnhdr	*shdr[SCNHDRS];
	SymEnt	*base;
	SymEnt	*symoff;
	SymEnt	*entry(long);
	DType	gatherdtype(SymEnt*);
	DType	chain(int, SymEnt*);
	void	gathervar(SymEnt*, Var**, Block*, UDisc);
	char	*gettbl();
	Source	*tree();
	char	*gethdr();
public:
		CoffSymTab(Core*,int,SymTab* =0,long=0);
		~CoffSymTab();
	Block	*gatherfunc(Func*);
	Var	*gatherutype(UType*);
/*	int	specialop(char*);		what? */
};

class MccSymTab : public SymTab {
	exec	*hdr;
	mlist	*base;
	mlist	*symoff;
	DType	gatherdtype(mlist*);
	DType	chain(int, mlist*);
	void	gathervar(mlist*, Var**, Block*, UDisc);
	char	*gettbl();
	Source	*tree();
	char	*gethdr();
public:
		MccSymTab(Core*,int,SymTab* =0,long=0);
		~MccSymTab();
	Block	*gatherfunc(Func*);
	Var	*gatherutype(UType*);
};

char *DiscName(UDisc);
#endif
