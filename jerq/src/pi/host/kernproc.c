#include "univ.h"
#include "process.pri"
#include "hostcore.h"
#include "master.pub"
#include "frame.pri"
#include "memory.pub"
#include "symtab.pub"
#include "symbol.h"
#include "srcdir.h"
#include "asm.pub"
#include "parse.h"
#include "expr.pub"
SRCFILE("kernproc.c")

char *KernProcess.kbd(char *s)
{
	Parse y(G_EXPR,0);
	Expr *e;
	Bls error;

	trace( "%d.kbd(%s)", this, s );	OK(0);
	if( !(e = (Expr*)y.parse(s)) )
		return sf("%s: %s", y.error, s);
	e->evaltext(globals, error);
	if( e->evalerr )
		return sf("%s: %s", s, error.text);
	((KernCore*)core)->cs_fp = e->val.lng;
	habeascorpus(PENDING,0);
	return 0;
}

char *KernProcess.help()
{
	return "<expr> {start fp for callstack}";
}

void KernProcess.open()
{
	Menu m;
	char *error;

	trace("%d.open(%d)", this);	VOK;
	Process::openpad();
	if( core ) return;
	insert(ERRORKEY, "Checking dump and symbol table...");
	core = (Core*) new KernCore(this, master);
	if( error = core->open() ){
		delete core;
		core = 0;
		m.last("open system dump", (Action)&KernProcess::open, 0);
		pad->menu( m );
		insert(ERRORKEY, error);
		return;
	}
	insert(ERRORKEY, core->symtab()->warn());
	globals = new Globals(core);
	_asm = core->newAsm();
	m.last( "src text",  (Action)&KernProcess::srcfiles );	/* should check */
	m.last( "Globals",   (Action)&KernProcess::openglobals );
	m.last( "RawMemory", (Action)&KernProcess::openmemory  );
	m.last( "Assembler", (Action)&KernProcess::openasm     );
	pad->menu(m);
	pad->makecurrent();
	docycle();
}

Index KernProcess.carte()
{
	Menu m;
	trace( "%d.carte(%d)", this ); OK(ZIndex);
	m.last( "open system dump",  (Action)&KernProcess::open, 0 );
	return m.index();
}

void KernProcess.userclose()
{
	trace( "%d.userclose()", this );	VOK;
	Process::userclose();
}
