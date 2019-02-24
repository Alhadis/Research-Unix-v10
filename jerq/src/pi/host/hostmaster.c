#include "hostcore.h"
#include "master.pri"
#include "process.pri"
#include <CC/stdio.h>
#include "symtab.pri"
SRCFILE("hostmaster.c")

Process	*HostMaster.domakeproc(char *p, char *s, char* c)
{
	return (Process*) new HostProcess(child, p, s, c);
}

extern char *UNIX;

HostMaster.HostMaster()
{
	static char *ps[] = {
		"/bin/ps  ",
		"/bin/ps a",
		"/bin/ps x",
	0 };
	Menu m;
	int i;

	trace( "%d.HostMaster()", this );	VOK;
	pad = new Pad( (PadRcv*) this );		/* this code cannot */
	pad->options(TRUNCATE|SORTED);			/* be in base ctor */
	pad->name( "pi" );
	pad->banner( "pi = 3.141592653" ); // 3.141592653 589793
	for( i = 0; ps[i]; ++i )
		m.last( ps[i], (Action)&HostMaster::refresh, (long)ps[i] );
	m.last("kernel pi", (Action)&HostMaster::kpi);
	pad->menu(m);
	pad->makecurrent();
	if( UNIX ){
		Process *p = (Process*) new HostProcess(0, 0, UNIX);
		core = new HostCore(p, this);
		pad->insert(1, FLUSHLINE, "Checking %s symbols ...", UNIX );
		char *error = core->open();
		if( error )
			pad->insert(1, "%s: %s", UNIX, error);
		else
			pad->insert(1, "%s", core->symtab()->warn() );
	}
	refresh(0);
}

void HostMaster.kpi()
{
	trace( "%d.kpi()", this );	VOK;
	if( !kernmaster ) kernmaster = new KernMaster(core->_symtab);
	kernmaster->pad->makecurrent();
}

#define PSOUT 100
#define PROCS 100
char *HostMaster.dopscmd(char *ps)
{
	char psout[PROCS][PSOUT];
	FILE *f, *Popen(char*,char*);
	int pid, i, j, e;

	trace( "%d.dops(%s)", this, ps?ps:"0" );	OK("Master.dops");
	if( !ps ) return 0;
	if( !(f = Popen( ps, "r")) )
		return SysErr( "cannot read from popen(): " );
	for( i = 0; i<PROCS && fgets(psout[i],PSOUT,f); ++i ) {}
	if( e = Pclose(f) )
		return sf( "exit(%d): %s", e, ps );
	qsort( psout, i, PSOUT, strcmp );
	for( j = 0; j <= i; ++j )
		if( 2 == sscanf( psout[j], " %d %[^\n]", &pid, psout[0] ) )
			makeproc( sf("/proc/%d",pid), 0, psout[0] );
	return 0;
}

void HostMaster.refresh(char *ps)
{
	char *error;
	Process *p;

	trace( "%d.refresh()", this );	VOK;
	pad->clear();
	makeproc( "core", "a.out", "" );
	makeproc( "!", "a.out", "" );
	if( error = dopscmd(ps) ) pad->error(error);
	for( p = child; p; p = p->sibling )
		if( p->core || !strcmp(p->stabpath,"!") ) insert(p);
}

char *HostMaster.kbd(char *s)
{
	char *corep, core[64], syms[64], star = 0;
	HostProcess *p;

	trace("%d.kbd(%s)", this, s);		OK("kbd");
	while( *s == ' ' ) ++s;
	if( !strcmp(s, "new Audit") ){
		StartAudit();
		return 0;
	}
	while( *s==' ' ) ++s;
	switch( *s ){
	case '!':
		for( ++s; *s==' '; ++s ) {}
		makeproc("!", s, "");
		break;
	case '*':
		star = 1;
		for( ++s; *s==' '; ++s ) {}
	default:
		switch( sscanf(s, "%s %s \n", corep = core, syms) ){
		case 1: syms[0] = '\0';
		case 2:	if( alldigits(corep) )
				corep = sf( "/proc/%s", core );
			p = (HostProcess*) makeproc(corep, syms[0]?syms:0, 0);
			if( star && p ) p->open();
			break;
		default:
		return help();
		}
	}
	return 0;
}

char *HostMaster.help()
{
	trace( "%d.help()", this );
	return "[*] <path> [<tables>] {[open] process/dump} | !<cmd> {program}";
}
