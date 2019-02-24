#include "univ.h"
#include "audit.h"
SRCFILE("audit.c")

void Audit.mon(long on)
{
	extern monitor(long,long=0,short* =0,int=0,int=0);
	if( on )
		monitor(2, 150000, new short[50000], 50000, 0);
	else
		monitor(0);
}

void StartAudit() { new Audit; NewPadStats(); }

void Audit.setperiod(long p) { if( period = p ) cycle(); }

void Audit.clone() { new Audit; }

Audit.Audit(){
	pad = 0;
	lastclock = 0;
	setperiod(0);
	cycle();
}

void Audit.lookup()
{
	extern long IdToSymCalls, StrCmpCalls, LoctosymHit, Loctosym;
	int i = IdToSymCalls, s = StrCmpCalls, h = LoctosymHit, l = Loctosym;
	pad->error("strcmp/idtosym=%d/%d; hit/loctosym=%d/%d", s, i, h, l);
}

void Audit.lazy()
{
	extern int FunctionStubs, FunctionGathered, UTypeStubs, UTypeGathered;
	int fs=FunctionStubs, fg=FunctionGathered, us=UTypeStubs, ug=UTypeGathered;
	pad->error( "functions: %d/%d; types: %d/%d", fg, fs, ug, us );
}

void Audit.cycle()
{
	char *ctime(long*);
	long clock, time(long*);
	Menu m;
	void abort(), exit();

	trace( "%d.cycle()", this );
	if( !pad ){
		pad = new Pad( (PadRcv*) this );
		pad->banner( "Audit %d:", this );
		pad->name( "Audit" );
		m.sort( "abort()?",	(Action)&abort		);
		m.sort( "clone",	(Action)&Audit::clone		);
		m.sort( "exit(0)?",	(Action)&exit,	    0	);
		m.sort( "lazy symbol",	(Action)&Audit::lazy,		);
		m.sort( "lookup",	(Action)&Audit::lookup		);
		m.sort( "setperiod(0)",	(Action)&Audit::setperiod, 0	);
		m.sort( "setperiod(1)",	(Action)&Audit::setperiod, 1	);
		m.sort( "setperiod(5)",	(Action)&Audit::setperiod, 5	);
		m.sort( "setperiod(60)",(Action)&Audit::setperiod, 60	);
		m.sort( "monitor(...)", (Action)&Audit::mon,   1	);
		m.sort( "monitor(0)",   (Action)&Audit::mon,   0	);
		pad->menu(m);
	}
	if( period ) pad->alarm(period);
	clock = time(0);
	if( clock < lastclock+period ) return;
	lastclock = clock;
	pad->insert( 1, "%24s", ctime( &clock) );
}
