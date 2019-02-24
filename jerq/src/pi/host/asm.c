#include "asm.pri"
#include "core.pub"
#include "format.pub"
#include "parse.h"
#include "expr.pub"
#include "frame.pub"
#include "process.pub"
#include "symtab.pub"
#include "symbol.h"
#include "bpts.pub"
SRCFILE("asm.c")

char *Instr.arg(int)			{ return "<arg>";	}
char *Instr.mnemonic()			{ return sf("0x%X?", opcode&0xFF); }
int Instr.argtype(int)			{ return 0;		}
int Instr.nargs()			{ return 0;		}
char *Asm.literaldelimiter()		{ return "<literal>"; }
Instr *Asm.newInstr(long)		{ return 0; }

Asm.Asm(Core *c)
{
	trace( "%d.Asm(%d)", this, c );		VOK;
	core = c;
	fmt = F_SYMBOLIC|F_HEX;
}

void Asm.userclose()
{
	trace( "%d.userclose()", this );	VOK;
	delete pad;
	pad = 0;
	Instr *instrsetsib;
	for( ; instrset; instrset = instrsetsib ){
		instrsetsib = instrset->sib;		// new malloc
		delete instrset;
	}
}

void Asm.banner()
{
	trace( "%d.banner()", this );	VOK;
	if( pad ){
		pad->banner("Assembler: %s", core->procpath());
		pad->name("Asm %s", basename(core->procpath()));
	}
}

void Asm.open(long a)
{
	trace("%d.open(%d)", this, a);	VOK;
	if( !pad ){
		Menu m;
		pad = new Pad( (PadRcv*) this );
		banner();
		m.last( "display pc",     (Action)&Asm::displaypc	 );
		if( core->online() ){
			m.first( "run",		  (Action)&Asm::go		 );
			m.last( "step  1 instr ", (Action)&Asm::instrstep,  1 );
			m.last( "step  4 instrs", (Action)&Asm::instrstep,  4 );
			m.last( "step 16 instrs", (Action)&Asm::instrstep, 16 );
			m.last( "step 64 instrs", (Action)&Asm::instrstep, 64 );
			m.last( "step over call", (Action)&Asm::stepover	 );
		}
		pad->menu(m);
	}
	pad->makecurrent();
	if( a ) newInstr(a);
}

void Asm.go()
{
	trace( "%d.go()", this ); VOK;
	core->process()->go();
}

void Asm.displaypc()
{
	trace("%d.displaypc()", this);	VOK;
	open(core->pc());
}

char *Asm.help()
{
	trace( "%d.help()", this );
	return ".=<expr> {display instruction at address}";
}

char *Asm.kbd(char *s)
{
	Parse y(G_DOTEQ_CONEX,0);
	Expr *e;
	Bls error;

	trace( "%d.kbd(%s)", this, s );		OK("kbd");
	if( !*s ) return help();		// SWMR173 (cannot do .+1)
	if( !(e = (Expr*)y.parse(s)) )
		return sf("%s: %s", y.error, s);
	e->evaltext(core->process()->globals, error);
	if( e->evalerr )
		return sf("%s: %s", s, error.text);
	newInstr(e->val.lng);
	return 0;
}

char *Instr.literal(long f)			/* is Format right? */
{
	static char t[128];

	trace( "%d.literal(0x%X) 0x%X %g", this, f, m.lng, m.flt ); OK("literal");
	sprintf( t, "%s%s", _asm->literaldelimiter(),
			Format(f&~F_SYMBOLIC).f(m.lng,m.dbl) );
	return t;
}

char *Instr.symbolic(char *prefix)		/* is Format right? */
{
	static char t[128];

	trace( "%d.symbolic(%s) 0x%X", this, prefix, m.lng ); OK("symbolic");
	strcpy(t, prefix);
	strcat(t, Format(fmt, _asm->core->symtab()).f(m.lng));
	return t;
}

Var *Instr.local(UDisc d, long a)		/* VAX */
{
	Func *f;
	Block *b;

	trace("%d.local(0x%X,%d)", this, d, a); OK(0);
	if( !(f = (Func*)_asm->core->symtab()->loctosym(U_FUNC, addr)) ) return 0;
	if( !(b = f->blk(addr)) ) return 0;
	BlkVars bv(b);
	Var *v;
	while( v = bv.gen() )
		if( v->disc() == d && v->range.lo == a )
			break;
	return v;
}

Var *Instr.field(Var *v, long a)
{
	trace( "%d.field(%d,%d)", this, v, a ); OK(0);
	if( !v->type.isptr() ) return 0;
	DType *d = v->type.decref();
	if( !d->isstrun() ) return 0;
	TypMems tm(d->utype());
	while( v = tm.gen() )
		if( v->range.lo == a ) break;
	return v;
}

char *Instr.regarg(char *lay, long f)		/* is Format right? */
{
	static char t[128];
	char *o, *r;
	Var *v;

	trace("%d.regarg(%s,0x%X) %d 0x%X", this, lay, f, reg, m.lng); OK("regarg");
	r = _asm->core->regname(reg);
	o = Format(f&~F_SYMBOLIC).f(m.lng);
	if( f&F_SYMBOLIC ){
		if( reg == _asm->core->REG_AP() ){
			if( v = local(U_ARG, m.lng) )
				o = v->text();
		} else if( reg == _asm->core->REG_FP() ){
			if( v = local(U_AUT, m.lng) )
				o = v->text();
		} else {
			if( v = local(U_REG, reg) ){
				r = sf("%s=%s", v->text(), r);
				if( v = field(v, m.lng) )
					o = v->text();
			}
		}
	}
	sprintf(t, lay, o, r);
	return t;
}

void Instr.dobpt(int setorclr)
{
	trace( "%d.dobpt(%d)", this, setorclr ); VOK;
	Stmt *stmt = new Stmt(0,0,0);
	stmt->range.lo = addr;
	stmt->process = _asm->core->process();
	stmt->dobpt(setorclr);
}

Instr.Instr(Asm *a, long l)
{
	trace( "%d.Instr(%d,%d)", this, a, l );	VOK;
	if( !l ) return;
	addr = l;
	_asm = a;
	sib = _asm->instrset;
	_asm->instrset = this;
	_asm->banner();					/* why here? */
	fmt = _asm->fmt;
	bpt = _asm->core->process()->bpts()->isasmbpt(addr);
}

void Instr.display()
{
	int	i;
	Bls	t;

	trace( "%d.display()", this );	VOK;
	if( !addr ) return;
	t.af("%s%s", bpt?">>>":"", Format(fmt,_asm->core->symtab()).f(addr));
	opcode = _asm->core->peekcode(addr)->chr;
	next = addr+1;
	char *mnem = mnemonic();
	if( mnem ){
		t.af(": %s ", mnem);
		int n = nargs();
		for( i = 0; i < n; ++i )
			t.af("%s%s", i?",":"", arg(i));
	}
	_asm->pad->insert(addr, SELECTLINE, (PadRcv*)this, carte(), "%s", t.text);
}

void Instr.showsrc()
{
	trace( "%d.showsrc()", this );	ok();
	Stmt *stmt = (Stmt*) _asm->core->symtab()->loctosym(U_STMT, addr);
	if( stmt ) stmt->select();
}

long AF[] = { F_OCTAL, F_SIGNED, F_HEX, F_SYMBOLIC, 0 };

Index Instr.carte()
{
	Menu m, f;

	trace( "%d.carte()", this );	ok();
	if( _asm->core->online() ){
		if( bpt )
			m.last( "clr bpt", (Action)&Instr::dobpt, 0 );
		else
			m.last( "set bpt", (Action)&Instr::dobpt, 1 );
	}
	if( _asm->core->symtab()->loctosym(U_STMT, addr) )
		m.last( "src text", (Action)&Instr::showsrc, 0 );
	m.last( "open frame",	(Action)&Instr::openframe );
	m.last( "next  1",	(Action)&Instr::succ,		1 );
	m.last( "next  5",	(Action)&Instr::succ,		5 );
	m.last( "next 10",	(Action)&Instr::succ,		10 );
	for( int i = 0; AF[i]; ++i ){
		long b = AF[i];
		if( fmt&b ) b |= F_TURNOFF;
		f.last(FmtName(b), (Action)&Instr::reformat, b);
	}
	m.last(f.index("format"));
	m.last( "refresh",	(Action)&Instr::succ,		-1 );
	m.last( "raw mem",	(Action)&Instr::memory,	0 );
	return m.index();
}

void Instr.openframe()
{
	trace( "%d.openframe()", this ); VOK;
	_asm->core->process()->openframe(addr);
}

void Instr.reformat(int f)
{
	trace( "%d.reformat(0x%X) 0x%X", this, f, fmt ); VOK;
	if( f&F_TURNOFF)
		fmt &= ~f;
	else
		fmt |= f;
	if( !fmt ) fmt = F_HEX;
	_asm->fmt = fmt;
	_asm->newInstr(addr);
}

void Instr.succ(int n)		/* think about it! */
{
	trace( "%d.succ(%d)", this, n );	VOK;
	_asm->fmt = fmt;
	if( n>0 ) _asm->newInstr(next)->succ(n-1);
	else if( n<0 ) _asm->newInstr(addr);
}

void Instr.memory()
{
	trace( "%d.memory()" ); VOK;
	_asm->core->process()->openmemory(addr);
}

void Asm.instrstep(long i)
{
	trace( "%d.instrstep(%d)", this, i );	VOK;
	core->process()->instrstep(i);
}

void Asm.stepover()
{
	trace( "%d.stepover()", this );	VOK;
	core->process()->
		stepover( core->pc(), newInstr(core->pc())->next );
}
