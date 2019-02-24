#include "termcore.h"
#include "master.pri"
#include "process.pri"
#include "symtab.pri"
SRCFILE("termmaster.c")

Process	*TermMaster.domakeproc(char *proc, char *stab, char* comment)
{
	return (Process*) new TermProcess(child, proc, stab, comment);
}

extern char *MUXTERM, *COREHOST;

TermMaster.TermMaster(Remote *r )
{
	Menu m;

	trace( "%d.TermMaster()", this );	VOK;
	pad = new Pad( (PadRcv*) this );		/* this code cannot */
	pad->options(TRUNCATE|SORTED);			/* be in base ctor */
	pad->name( "3pi" );
	pad->banner( "3pi = 9.424776" );
	m.last( "jps", (Action)&TermMaster::jps );
	if( !COREHOST ){
		m.last("layer", (Action)&TermMaster::layer);
		m.last("host pi", (Action)&TermMaster::pi);
	}
	pad->menu(m);
	pad->makecurrent();
	if( MUXTERM ){
		Process *p = (Process*) new TermProcess(0, 0, MUXTERM);
		core = new TermCore(p, this);
		pad->insert(1, FLUSHLINE, "Checking %s symbols ...", MUXTERM );
		char *error = core->open();
		if( error )
			pad->insert(1, "%s: %s", MUXTERM, error);
		else
			pad->insert(1, "%s", core->symtab()->warn() );
	}
	remote = r;
	CoreVersion(remote);
	remote->pktstart(C_SPEE);
	remote->sendlong(getpid());
	remote->pktflush();
	jps();
}

TermProcess *TermMaster.makeP(long P, char *stab)
{
	trace( "%d.makeP(0x%X,%s)", this, P, stab );	OK(0);
	return (TermProcess*) makeproc( sf("P=0x%X",P), stab, 0 );
}

void TermMaster.layer()
{
	char dotm[256];
	long P;

	trace( "%d.layer()", this );	VOK;
	remote->pktstart(C_DEBUG);
	remote->pktflush();
	P = remote->rcvlong();
	remote->rcvstring(dotm);
	if( P ){
		TermProcess *p = makeP(P, dotm);
		if( p ) p->open();			// how can p==0 ?
	}
}	

void TermMaster.jps()
{
	char dotm[256];
	long NPROC, i, P;

	trace( "%d.jps()", this );	VOK;
	remote->pktstart(C_JPS);
	remote->pktflush();
	NPROC = remote->rcvuchar();
	if( !NPROC ){
		pad->insert(1, "jps failed");
		return;
	}
	pad->clear();
	for( i = 0; i < NPROC; ++i ){
		P = remote->rcvlong();
		remote->rcvstring(dotm);
		if( COREHOST && dotm[0]=='/' )
			makeP(P, sf("/n/%s%s", COREHOST, dotm));
		else
			makeP(P, dotm);
	}
}

char *TermMaster.kbd(char *s)
{
	char syms[64], star = 0;
	TermProcess *p = 0;
	long P = 0;

	trace( "%d.kbd(%s)", this, s );		OK("kbd");
	while( *s == ' ' ) ++s;
	switch( *s ){
	case '*':
		star = 1;
		for( ++s; *s==' '; ++s ) {}
	default:
		switch( sscanf(s, "%*[ Pp=0xX]%x %s", &P, syms) ){
		case 1: p = makeP(P);
			break;
		case 2: p = makeP(P, syms);
			if( star ) p->open();
			break;
		default:
			return help();
		}
		if( star && p ) p->open();
	}
	return 0;
}

char *TermMaster.help()
{
	trace( "%d.help()", this );
	return "[*] P=0X<addr> [<path>] {[open] mux process [tables]}";
}

void TermMaster.pi()
{
	trace( "%d.pi()", this );	VOK;
	if( !hostmaster ) hostmaster = new HostMaster;
	hostmaster->pad->makecurrent();
}
