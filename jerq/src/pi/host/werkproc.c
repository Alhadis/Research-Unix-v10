#include "m68kcore.h"
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
SRCFILE("snetproc.c")

Index WerkProcess.carte()
{
	Menu m("open process", (Action)&WerkProcess::open);
	return m.index();
}

void WerkProcess.open()
{
	Menu m;
	char *error;

	trace( "%d.open(%d)", this );	VOK;
	Process::openpad();
	if( core ) return;
	insert(ERRORKEY, "Checking process and symbol table...");
	core = (Core*) new WerkCore(this, (WerkMaster*)master);
	if( error = core->open() ){
		delete core;
		core = 0;
		m.last( "open process", (Action)&WerkProcess::open, 0 );
		pad->menu( m );
		insert(ERRORKEY, error);
		return;
	}
	insert(ERRORKEY, core->symtab()->warn());
	globals = new Globals(core);
	_asm = core->newAsm();
	m.last( "RawMemory", (Action)&Process::openmemory  );
	m.last( "Assembler", (Action)&Process::openasm     );
	pad->menu(m);
	_bpts = new Bpts(core);
	_bpts->lay();
	pad->makecurrent();
	docycle();
}
