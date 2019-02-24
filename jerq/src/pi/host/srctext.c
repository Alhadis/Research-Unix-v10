#include "symbol.h"
#include "srctext.pri"
#include "core.pub"
#include "symtab.pub"
#include "process.pub"
#include "bpts.pub"
#include "expr.pub"
#include "format.pub"
#include "frame.pri"
#include <CC/sys/types.h>
#include <CC/sys/stat.h>
#include <sys/dir.h>
SRCFILE("srctext.c")

SrcText.SrcText(Source *s, long c)
{
	trace( "%d.SrcText(%d,%d)", this, s, c ); VOK;
	source = s;
	compiletime = c;
	path = source->_text;
	prevpat = "<no pattern>";
}

Core *SrcText.core() { return source->symtab->core(); }

Process *SrcText.process() { return core()->process(); }

void DoNothing(){}

void SrcText.promote()
{
	Menu m;
	trace( "%d.promote()", this ); VOK;
	if( promoted ){
		process()->openglobals();
		return;
	}
	Var *v;
	BlkVars bv(source->blk);
	int limit = 25;
	while( limit-->0 && (v = bv.gen()) )
		m.sort(sf("%s\240Sta",v->_text), (Action)&Frame::pickvar, (long)v);
	char *t = sf("\276 %s", basename(source->text()));
	m.first( t, (Action)&DoNothing );
	process()->openglobals(&m);
	promoted = 1;
}
	
char *SrcText.read(Menu &m)
{
	Func *fun;
	register int limit = 40, l;
	struct stat fdstat;
	register char *p;
	Menu sub;

	trace("%d.read(%d) path=%s", this, m, path);	OK("SrcText.read");
	if( edge ) return 0;
	if( fstat(fd, &fdstat) )
		return SysErr("cannot stat");
	if( fdstat.st_size == 0 )
		return "null file";
	if( !compiletime ) compiletime = source->symtab->modtime();
	if( modified(fd) > compiletime && !warned ){
		++warned;
		trace( "%d>%d", modified(fd), compiletime );
		return sf("modified since compilation at %24s",ctime(&compiletime));
	}
	warned = 0;
	fun = (Func*)source->child;
	long n = 0;
	while( fun  ){
		Bls toc;
		if( lastline < fun->lines.hi ) lastline = fun->lines.hi;
		if( strlen(fun->text())<=32 && fun->lines.lo>0 && --limit>0 ){
			toc.af( "%s\256%d", fun->text(), fun->lines.lo );
			sub.sort( toc.text, (Action) &SrcText::select, fun->lines.lo );
			++n;
		}
		fun = (Func*)fun->rsib;
	}
	if( lastline <= 0 )
		return "not referenced by symbol table (cc -g?)";
	m.last(sub.index(n>4 ? "index by fcn " : 0));
	edge = new char*[lastline+1];
	body = new char[fdstat.st_size];
	if( !ReadOK( fd, body, fdstat.st_size ) )
		return SysErr( "read error" );
	l = 1;
	p = body;
	while( l <= lastline && *p ){		/* should check p stays in body */
		edge[l++] = p;
		while( *p && *p != '\n' ) ++p;
		*p++ = '\0';
		trace( "%d %s", l-1, edge[l-1] );
	}
	pad->lines(l-1);
	return 0;
}

void SrcText.open()
{
	Menu m;
	char *error = 0;
	struct stat stbuf;

	trace( "%d.open() path=%s", this, path );	VOK;
	if( pad && edge ){
		pad->makecurrent();
		return;
	}
	if( !pad ) pad = new Pad( (PadRcv*) this );
	pad->options( TRUNCATE );
	pad->banner( "Source Text: %s", process()->prefix(path) );
	pad->name( basename(path) );
	pad->tabs(4);
	pad->error(0);
	if( !edge ){
		pad->menu( ZIndex );
		pad->lines(0);
		if( (fd = ::open(process()->prefix(path),0)) < 0 )
			error=  SysErr( "cannot open:" );
		if( !error ){
			if( ::fstat( fd, &stbuf ) )
				error = "cannot stat"; 
			else if( (stbuf.st_mode&S_IFMT) == S_IFDIR )
				error = "is a directory";
		}
		if( !error ) error = read(m);
		if( fd >= 0 ) close(fd);
		if( !error ){
			if( core()->online() ){
				Action a = (Action)&SrcText::stmtstep;
				Menu sub;
				for( long i = 5; i <= 50; i += 5 )
					sub.last(sf("step %3d stmts",i), a, i);
				sub.last("step 100 stmts", a, 100);
				sub.last("step 500 stmts", a, 500);
				m.first("step into fcn", (Action)&SrcText::stepinto   );
				m.first(sub.index("step >4 stmts"));
				m.first("step 4 stmts ",  a, 4);
				m.first("step 3 stmts ",  a, 3);
				m.first("step 2 stmts ",  a, 2);
				m.first("step 1 stmt  ",  a, 1);
			}
			m.first( "current stmt",  (Action)&SrcText::currentstmt	);
			if( core()->online() )
				m.first( "run",		  (Action)&SrcText::go		);
		}
	}
	m.first( sf( "reopen %s",basename(path)) ,(Action)&SrcText::reopen );
	pad->menu(m);
	pad->error( error );
	pad->makecurrent();
}

void SrcText.reopen()
{
	trace( "%d.reopen()", this );	VOK;
	free();
	open();
}

void SrcText.free()
{
	trace( "%d.free()", this );	VOK;
	if( edge ) { delete edge; edge = 0; }
	if( body ) { delete body; body = 0; }
}

void SrcText.userclose()
{
	trace( "%d.userclose()", this );	VOK;
	free();
	if( pad ){
		delete pad;
		pad = 0;
	}
}

#define NONBPT 0
#define SETBPT 1
#define SETCLR 2
#define CLRBPT 3

void SrcText.linereq(long i, Attrib a)	/* profile hot spot ... */
{
	Stmt *stmt = 0;
	Bls t;
	static Index ix[4];
	int bpty;

	trace( "%d.linereq(%d,0x%X)", this, i, a ); VOK;
	if( ix[0].null() ){		/* ... that's better */
		Menu m[4];
		for( bpty = NONBPT; bpty <= CLRBPT; ++bpty ){
		      m[bpty].last( "assembler",   (Action) &Stmt::asmblr    );
		      m[bpty].last( "open frame",  (Action) &Stmt::openframe );
		}
		m[SETBPT].first( "cond bpt",	(Action) &Stmt::conditional, (long)Q_BPT);
		m[SETBPT].first( "trace on",	(Action) &Stmt::settrace );
		m[SETBPT].first( "set  bpt",	(Action) &Stmt::dobpt, 1 );

		m[SETCLR].first( "trace on",	(Action) &Stmt::settrace );
		m[SETCLR].first( "set   bpt",	(Action) &Stmt::dobpt, 1 );
		m[SETCLR].first( "clear bpt",	(Action) &Stmt::dobpt, 0 );

		m[CLRBPT].first( "clear bpt",	(Action) &Stmt::dobpt, 0 );
		for( bpty = NONBPT; bpty <= CLRBPT; ++bpty )
			ix[bpty] = m[bpty].index();
	}
	if( !edge ) open();
	if( !edge ) return;
	if( i<1 || i>lastline ) return;
	bpty = NONBPT;
	stmt = source->stmtafter(i);
	if( stmt && core()->online() ){
	    	if( !process()->bpts()->isbpt(stmt) ){
			bpty = SETBPT;
			if( stmt->lineno==i && stmt->condition==Q_BPT ){
				bpty = SETCLR;
				t.af( "if( ? ) >>>" );
			}
		} else {
			bpty = CLRBPT;
			if( stmt->lineno == i ){
				if( stmt->condition )
					t.af("if(%s)", stmt->condtext->text);
				t.af( ">>>" );
			}
		}
	}
	t.af("%s", edge[i]);
	pad->insert(i, (Attrib)a|ACCEPT_KBD, (PadRcv*)stmt, ix[bpty], "%s", t.text);
}

char *SrcText.srcline(long i)
{
	trace( "%d.srcline(%d)", this, i ); OK("SrcText.srcline");
	if( !edge || i<1 || i>lastline ) return "";
	return edge[i];
}

void SrcText.select(long i, long svp)
{
	trace( "%d.select(%d,%d)", this, i, svp ); VOK;
	if( svp!=SVP || edge ) linereq(i, SELECTLINE);
}

void SrcText.go()
{
	trace( "%d.go()", this ); VOK;
	process()->go();
}

void SrcText.currentstmt()
{
	trace( "%d.currentstmt()", this ); VOK;
	core()->process()->currentstmt();
}

void SrcText.stmtstep(long i)
{
	trace( "%d.stmtstep()", this ); VOK;
	process()->stmtstep(i);
}

void SrcText.stepinto()
{
	trace( "%d.stepinto()", this ); VOK;
	process()->stepinto();
}

char *SrcText.kbd(char *s)
{
	trace( "%d.kbd(%s)", this, s );		OK("kbd");
	if( edge ){
		if( *s == '/' )
			return contextsearch( lastline, s+1, 1 );
		else if( *s == '?' )
			return contextsearch( 1, s+1, -1 );
		else if( alldigits(s) )
			select( atoi(s) );
		else
			return help();
	} else {
		path = sf("%s",s);
		reopen();
	}
	return 0;
}

char *SrcText.help()
{
	trace( "%d.help()", this );		OK("SrcText.help");
	return edge ? "<line number> {display line} | [/?]<string> {search}"
		    : "<path> {change source file name}";
}

char *SrcText.contextsearch(int from, char *pat, int dir)
{
	trace( "%d.contextsearch(%d,%s,%d)", this, from, pat?pat:"", dir );
	OK("contextsearch");
	if( !pat || !*pat || !strcmp(pat,"?") || !strcmp(pat,"/") )
		pat = prevpat;
	prevpat = sf("%s", pat);
	int patlen = strlen(pat);
	register char pat0 = *pat;
	for( int probe = from+dir; probe != from; probe += dir ){
		if( probe == 0 ) probe = lastline;
		if( probe == lastline+1 ) probe = 1;
		register char *p;
		for( p = edge[probe]; *p; ++p ){
			if( *p == pat0 && !strncmp(p, pat, patlen) ){
				linereq(probe, SELECTLINE);
				return 0;
			}
		}
	}
	return sf("%s: not found", pat);
}
