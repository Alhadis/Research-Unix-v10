#include <pads.pub>
SRCFILE("stats.c")

class PadStats : public PadRcv {
	Pad	*pad;
		PadStats();
	void	allocfree();
	void	cachestats();
	void	sbrk0();
	void	sf0();
	void	traffic();
};

void NewPadStats() { new PadStats; }

PadStats.PadStats()
{
	void abort(), exit(int);
	Menu m;

	pad = new Pad( (PadRcv*) this );
	pad->banner( "PadStats:" );
	pad->name( "stats" );
	m.sort( "abort()?",	(Action)&abort		);
	m.sort( "allocfree()",	(Action)&PadStats::allocfree	);
	m.sort( "cachestats()",	(Action)&PadStats::cachestats	);
	m.sort( "exit(0)?",	(Action)&exit,	    0	);
	m.sort( "sbrk(0)",	(Action)&PadStats::sbrk0		);
	m.sort( "sf(0)",	(Action)&PadStats::sf0		);
	m.sort( "traffic()",	(Action)&PadStats::traffic	);
	pad->menu(m);
}

void PadStats.cachestats()
{
	char *CacheStats();
	pad->insert(1, SELECTLINE, CacheStats());
}

void PadStats.sf0()
{
	pad->insert(2, SELECTLINE, sf(0));
}

void PadStats.sbrk0()
{
	pad->insert(3, SELECTLINE, "sbrk(0)=%d=0x%X", sbrk(0), sbrk(0));
}

void PadStats.traffic()
{
	extern long BytesToTerm, BytesFromTerm;
	long k = time(0);
	pad->insert(k, SELECTLINE,
		"%0.9s h->t=%d t->h=%d", ctime(&k)+11, BytesToTerm, BytesFromTerm);
}

class Allocator {
public:
	char	*profile(long);

};
//extern Allocator NewDel;

void PadStats.allocfree()
{
//	char *p; int i;
//	for( i = 0; p = NewDel.profile(i); ++i )
//		if( *p )
//			pad->insert( i+10, SELECTLINE, p );
}
