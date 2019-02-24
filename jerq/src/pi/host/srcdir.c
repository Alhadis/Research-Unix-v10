#include "srcdir.h"
#include "process.pri"
#include "symbol.h"
#include "srctext.pri"
#include "symtab.pub"
SRCFILE("srcdir.c")

SrcDir.SrcDir(Process *p)
{
	trace( "%d.SrcDir(%d)", this, p );	VOK;
	process = p;
}

void SrcDir.banner()
{
	trace( "%d.banner()", this );	VOK;
	if( pad ){
		pad->name( "Src Files" );
		pad->banner( "Source Files: %s", process->procpath );
	}
}

void SrcDir.hostclose()
{
	trace( "%d.hostclose()", this );	VOK;
	if( pad ) delete pad;
	pad = 0;
}

char *SrcDir.help()
{
	trace( "%d.help()", this );	OK("SrcDir.help");
	return "<path> {set source path prefix}";
}

char *SrcDir.kbd(char *s)
{
	trace( "%d.kbd(%s)", this, s );	OK("kbd");
	process->_prefix = sf("%s",s);
	pad->insert( 1, SELECTLINE|DONT_CUT, "    prefix: %s", s );
	return 0;
}

void SrcDir.open()		// host/srcdir.c SWMR 170
{
	Menu	m;
	long	k = 2;
	Source	*r;

	trace( "%d.open()", this );	VOK;
	if( !pad ){
		r = process->symtab()->root();
		if( !r ){
			PadsWarn("no source files");
			return;
		}
		if( !(Symbol*)r->rsib ){
			r->srctext->open();
			return;
		}
		pad = new Pad( this );
		banner();
		pad->options(SORTED);
		pad->makecurrent();
		m.last( "open source file", (Action)&SrcText::open);
// wait for complaints...
//		m.last( "file statics", (Action)&SrcText::promote);
		for( ; r; r = (Source*)((Symbol*)r->rsib) )
		    pad->insert( k++, DONT_CUT, (PadRcv*)r->srctext, m, r->_text );
	}
	pad->makecurrent();
}
