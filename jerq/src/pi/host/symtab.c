#include "symtab.pri"
#include "dtype.pri"
#include "symbol.h"
#include "srctext.pub"
#include "phrase.pub"
#include "format.pub"
#include "core.pub"
SRCFILE("symtab.c")

int FunctionGathered, UTypeGathered, FunctionStubs, UTypeStubs;
int IdToSymCalls, StrCmpCalls;

UType *SymTab.utypelist() { return utype; }

int HASHF(register char *s)
{
	register unsigned h = 0;
	while( *s ) h = (h<<1)^*(s++);
	return h % HASH;
}

void SymTab.uncfront(Var *v, char *prefix )
{
	trace( "%d.uncfront(%d,%s)", this );	VOK;
	int l = strlen(prefix);
	for( ; v; v = (Var*)v->rsib )
		if( !strncmp(v->_text, prefix, l) )
				v->_text += l;
}

SSet.SSet(char a)
{
	v[0] = a;
	v[1] = 0;
}

SSet.SSet(char a, char b, char c, char d, char e, char f, char g )
{
	v[0] = a; v[1] = b; v[2] = c; v[3] = d;
	v[4] = e; v[5] = f; v[6] = g; v[7] = 0;
}

char *DiscName(UDisc d)
{
	switch( d ){
	case U_ARG:	return "arg";
	case U_AUT:	return "aut";
	case U_REG:	return "reg";
	case U_STA:	return "sta";
	case U_FST:	return "Sta";
	case U_GLB:	return "Glb";
	default:	return Name("U_%d",d);
	}
}

char *SymTab.stabpath()	{ OK("SymTab.stabpath"); return _core->stabpath(); }
char *SymTab.warn()	{ OK("SymTab.warn"); return _warn; }
Pad *SymTab.pad()	{ OK(0); return _pad; }

void SymTab.showutype(UType *u)		// cf Var.showutype(UTpe*)
{
	trace("%d.showutype(%d))", this, u); VOK;
	u->show(LEAVE, SELECTLINE);
}

void SymTab.banner()
{
	trace("%d.banner()", this);	VOK;
	if( !_pad ) return;
	_pad->banner("User Defined Types: %s", _core->procpath());
	_pad->name("Types %s", basename(_core->procpath()));
	_pad->options(TRUNCATE);
	Menu m;
	Action a = (Action)&SymTab::showutype;
	char *index(char*,char);
	for( UType *u = utype; u; u = (UType *)u->rsib ){
		if( u->type.isstrun() )
			m.sort(sf("%s\240",u->text()), a, (long) u);
	}
	_pad->menu(m);
}

SymTab.SymTab(Core* c,int stabfd, SymTab *i, long r)
{
	trace("%d.SymTab(%d,%d)", this, c, stabfd); VOK;
	_core = c;
	fd = stabfd;
	inherit = i;
	relocation = r;
	_blk = new Block(this, 0, 0, sf("%s.glb_blk",stabpath()));
}

void SymTab.opentypes()
{
	if( !_pad ){
		_pad = new Pad((PadRcv*) this);
		banner();
	}
	_pad->makecurrent();
}

SymTab.~SymTab()
{
	trace( "%d.~SymTab()", this ); VOK;
	if( _pad ) delete _pad;
	if( strings ) delete strings;
	Source *_rootrsib;
	for( ; _root; _root = _rootrsib){
		_rootrsib = (Source*) _root->rsib;	// new malloc
		_root->srctext->userclose();
		delete _root;
	}
	for( int i = 0; i <= TOSYM; ++i )
	  for( int j = 0; j < HASH; ++j )
	    for( Symbol *s = hashtable[i][j]; s; s = s->hashlink )
		switch( s->disc() ){
		case U_UTYPE:	delete (UType*)s; break;
		case U_GLB:
		case U_STA:	((Var*)s)->type.free();
				delete (Var*)  s;
				break;
		case U_FUNC:	delete (Func*) s; break;
		case U_STMT:	delete (Stmt*) s; break;
		default:	abort();
		}				
}

Core *SymTab.core()		{ OK(0); return _core; }
Source *SymTab.root()		{ OK(0); return _root; }
long SymTab.modtime()		{ OK(0); return modified(fd); }
Block *SymTab.blk()		{ OK(0); return _blk; }
long SymTab.magic()		{ OK(0); return _magic; }

Var *SymTab.globregs(Block *b, int r)
{
	Var *g = 0;
	int i;

	trace("%d.globregs(%d,%d)", this, b, r);	OK(0);
	for( i = 0; i<r; ++i ){
		g = new Var(this, b, g, U_REG, sf("$%s",_core->regname(i)) );
		g->range.lo = i;
		g->type.pcc = LONG;
		if( !b->var ) b->var = g;
	}
	return g;
}

char *SymTab.dump()
{
	int i, j;
	Symbol *s;

	OK("SymTab.dump");
	trace( "stabpath()=%s entries=%d", stabpath(), strings );
	trace( "strings=%d strsize=%d", strings, strsize );
	for( i = 0; i <= TOSYM; ++i )
		for( j = 0; j < HASH; ++j )
			for( s = hashtable[i][j]; s; s = s->hashlink )
				trace( "%s", s->dump() );
	return "SymTab.dump";
}

void SymTab.read()
{
	char *error;

	trace( "%d.read()", this );	VOK;
	trace( "symtab modified %d", modtime() );
	_root = 0;
	if( error = gethdr() )
		_warn = sf( "symbol table header: %s; go on", error );
	else if( !entries )
		_warn = "symbol table missing; go on";
	else if( !(_root = tree()) )
		_warn = sf( "%s; go on", _warn ? _warn : "symbol table error" );
	trace( "%s", dump() );
}

void SymTab.enter( Symbol *s )
{
	int i, h;

	trace( "%d.enter(%d) %s %d", this, s, s->dump(), HASHF(s->_text) ); VOK;
	s->hashlink = hashtable[i=s->disc()&TOSYM][h=HASHF(s->_text)];
	hashtable[i][h] = s;
}


Block *SymTab.fakeblk()		/* VAX  ap offsets */
{
	Block *b;
	int i;
	Var *a = 0;

	b = new Block( this, 0, 0, "?().arg_blk" );
	new Block( this, b, 0, "?().lcl_blk" );
	for( i = 1; i <= 3; ++i ){
		a = new Var( this, b, a, U_ARG, sf( "$arg%d", i ) );
		if( i==1 ) b->var = a;
		a->range.lo = i*4;		// (i-1)*4 for mac32
		a->type.pcc = INT;
	}
	return b;
}

Symbol *SymTab.idtosym(SSet set, register char *id, int lev)
{
	register int i, h;
	register Symbol *s;

	trace("%d.idtosym(%d,%s) %d", this, set.v[0], id?id:"0", HASHF(id)); OK(0);
	++IdToSymCalls;
	if( !id ) return 0;
	h = HASHF(id);
	for( i = 0; set.v[i]; ++i ){
		trace( "%s", DiscName(set.v[i]) );
		for( s = hashtable[set.v[i]&TOSYM][h]; s; s = s->hashlink ){
			trace( "%s", s->dump() );
			++StrCmpCalls;
			if( eqstr(id,s->_text) ) return s;
		}
	}
	return (lev>0 && inherit) ? inherit->idtosym(set,id,lev-1) : 0;
}

inline Symbol *LookupCache.match(SSet _set, long _loc)
{
	return !strcmp(_set.v, set.v) && loc == _loc ? sym : 0;
}

void LookupCache.save(SSet _set, long _loc, Symbol *_sym)
{
	set = _set;
	loc = _loc;
	sym = _sym;
}

int LoctosymHit, Loctosym;
Symbol *SymTab.loctosym(SSet set, register long loc, int lev)
{
	register h, best_lo, s_lo;
	register Symbol *s, *best = 0;
	register i, setvi;
	Symbol   *ibest;

	trace( "%d.loctosym(%d,%d)", this, set.v[0], loc ); OK(0);
	++Loctosym;
	if( s = loctosymcache.match(set, loc) ){
		++LoctosymHit;
		return s;
	}
	for( i = 0; setvi = set.v[i]; ++i ){
		setvi &= TOSYM;
		for( h = 0; h < HASH; ++h ){
			for( s = hashtable[setvi][h]; s; s = s->hashlink ){
				s_lo = s->range.lo;
				if( loc < s_lo
				||( best && best_lo >= s_lo )
				||( s->range.hi && loc > s->range.hi ) )
					continue;
				best = s;
				best_lo = best->range.lo;
				if( loc == best_lo ) goto returnbest;
			}
		}
	}
	if( lev>0 && inherit && (ibest = inherit->loctosym(set,loc,lev-1)) ){
		if( !best || ibest->range.lo > best->range.lo ){
			best = ibest;
			goto returnbest;
		}
	}
returnbest:
	loctosymcache.save(set, loc, best);
	return best;
}

char *SymTab.symaddr(long a)
{
	trace( "%d.symaddr(%d)", this, a ); OK("SymTab.symaddr");
	return Format(F_SYMBOLIC, this).f(a);
}

Index SymTab.utypecarte(short sorety)
{
	Menu m;
	UType *u;
	Action a = (Action)&Phrase::applycast;
	char *index(char*,char);

	trace( "%d.utypecarte(%d)", this, sorety );	OK(ZIndex);
	if( !castix[sorety].null() ) return castix[sorety];
	for( u = utype; u; u = (UType *)u->rsib ){
		if( ::index(u->text(),'$') ) continue;		/* $ ? */
		if( u->type.pcc == sorety )
			m.sort( sf("%s\240",u->text()), a, (long) &u->type );
	}
	if( inherit && !inherit->utypecarte(sorety).null() ){
		Menu s;
		s.first(inherit->utypecarte(sorety));
		m.last(s.index("inherit"));
	}
	return castix[sorety] = m.index();
}

Block *SymTab.gatherfunc(Func*)		{ IF_LIVE(1) return 0; }
Var *SymTab.gatherutype(UType*)		{ IF_LIVE(1) return 0; }

void SymbolStats()
{
	void WireTap(...);

	int fpc = FunctionStubs ? FunctionGathered*100/FunctionStubs : 0;
	int upc = UTypeStubs ? UTypeGathered*100/UTypeStubs : 0;
	static int prevfpc = -1, prevupc = -1;
	if( fpc>prevfpc || upc>prevupc )
		WireTap("FG=%d FS=%d UG=%d US=%d\n", FunctionGathered,
			FunctionStubs, UTypeGathered, UTypeStubs);
	prevfpc = fpc;
	prevupc = upc;
}
