#include "termcore.h"
#include "process.pri"
#include "srcdir.h"
#include "expr.pub"
#include "master.pub"
#include "bpts.pub"
#include "frame.pri"
#include "memory.pub"
#include "asm.pub"
#include "symtab.pub"
#include "symbol.h"
#include "srctext.pub"
#include "format.pub"
#include <ctype.h>
SRCFILE("termproc.c")

#define HostProcess DONT_USE_HostProcess

Index TermProcess.carte()
{
	Menu m( "open process", (Action)&TermProcess::open );
	trace( "%d.carte()", this ); OK(ZIndex);
	return m.index();
}

void TermProcess.open()
{
	Menu m, s;
	char *error;

	trace( "%d.open(%d)", this );	VOK;
	Process::openpad();
	if( core ) return;
	pad->insert(ERRORKEY, "Checking process and symbol table...");
	core = (Core*) new TermCore(this, (TermMaster*)master);
	if( error = core->open() ){
		delete core;
		core = 0;
		m.last( "open process", (Action)&TermProcess::open, 0 );
		pad->menu( m );
		pad->insert(ERRORKEY, error);
		return;
	}
	insert(ERRORKEY, core->symtab()->warn());
	globals = new Globals(core);
	_asm = core->newAsm();
	m.last("run",	    (Action)&TermProcess::go		);
	m.last("stop",	    (Action)&/*this->*/TermProcess::stop	);
	m.last("src text",  (Action)&TermProcess::srcfiles	);
	m.last("Globals",   (Action)&TermProcess::openglobals);
	m.last("RawMemory", (Action)&TermProcess::openmemory	);
	s.last("Assembler", (Action)&TermProcess::openasm	);
	s.last("pick slave",(Action)&TermProcess::pickslave  );
	s.last( "User Types",(Action)&Process::opentypes   );
	s.last("Journal",   (Action)&TermProcess::openjournal);
	s.last("Bpt List",  (Action)&TermProcess::openbpts);
	m.last(s.index("more"));
	pad->menu(m);
	_bpts = new Bpts(core);
	_bpts->lay();
	pad->makecurrent();
	docycle();
}

void TermProcess.pickslave()
{
	trace("%d.pickslave()"); VOK;
	Pick("slave", (Action)&TermProcess::slavedriver, (long)this);
}

int TermProcess.accept(Action a)
{
	trace("%d.accept(%d)", this, a);		OK(0);
	return a == (Action)&TermProcess::slavedriver;
}
