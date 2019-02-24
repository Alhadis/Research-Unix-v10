#include "m68kcore.h"
#include "master.pri"
#include "process.pri"
#include "symtab.pri"

Process	*SnetMaster.domakeproc(char *proc, char *stab, char* comment)
{
	return (Process*) new SnetProcess(child, proc, stab, comment);
}

SnetMaster.SnetMaster()
{
	extern char *SNETSYMS;

	pad = new Pad( (PadRcv*) this );		/* this code cannot */
	pad->options(TRUNCATE|SORTED);			/* be in base ctor */
	pad->name("Spi");
	pad->banner("Spi:");
	pad->makecurrent();
	makeproc("core", "a.out", "");
	if( SNETSYMS ){
		Process *p = (Process*) new SnetProcess(0, 0, SNETSYMS);
		core = new SnetCore(p, this);
		pad->insert(1, FLUSHLINE, "Checking %s symbols ...", SNETSYMS);
		char *error = core->open();
		if( error )
			pad->insert(1, "%s: %s", SNETSYMS, error);
		else
			pad->insert(1, "%s", core->symtab()->warn() );
	}
}

char *SnetMaster.kbd(char *s)
{
	char core[64], syms[64];

	while( *s==' ' ) ++s;
	if( sscanf(s, "%s %s \n", core, syms) != 2)
		return help();
	makeproc(core, syms, 0);
	return 0;
}

char *SnetMaster.help()
{
	return "<dump_file_from_snet> <symbol_tables> {identify dump}";
}
