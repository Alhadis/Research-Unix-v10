#include "m68kcore.h"
#include "master.pri"
#include "process.pri"
#include "symtab.pri"

Process	*WerkMaster.domakeproc(char *proc, char *stab, char* comment)
{
	return (Process*) new WerkProcess(child, proc, stab, comment);
}

WerkMaster.WerkMaster()
{
	pad = new Pad((PadRcv*) this);			// this code cannot
	pad->options(TRUNCATE|SORTED);			// be in base ctor
	pad->name("Wpi");
	pad->banner("Wpi:");
	pad->makecurrent();
}

char *WerkMaster.kbd(char *s)
{
	while( *s==' ' ) ++s;
	char core[64];

	if( sscanf(s, "%s\n", core) != 1)
		return help();
	makeproc(core, core, 0);
	return 0;
}

char *WerkMaster.help()
{
	return "<andrew.out> {identify goo}";
}
