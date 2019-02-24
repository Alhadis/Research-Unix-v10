#include "symtab.pri"
#include "dtype.pri"
#include "symbol.h"
#include "coff.h"
SRCFILE("coffsymtab.c")

DType CoffSymTab.chain(int t, SymEnt *s)
{
	DType d;

	trace( "%d.chain(0%o,%d) %s", this, t, s, PccName(t) );
	if( t&TMASK ){
		d.pcc = t&TMASK;
		d.univ = new DType;
		*(d.ref()) = chain(DECREF(t), s);
	} else {
		d.pcc = t;
		if( (t==STRTY||t==ENUMTY||t==UNIONTY) )
			d.univ = s->numaux()
			       ? idtosym(U_UTYPE, entry(s->tagndx())->id())
			       : 0;
	}
	return d;
}

DType CoffSymTab.gatherdtype(SymEnt *s)
{
	DType d, *dp;
	int i = 0;

	trace( "%d.gatherdtype(%d)", this, s );
	d = chain(s->desc(),s);
	for( dp = &d; dp; dp = dp->ref() )
		if( dp->isary() )
			dp->dim = s->dimen(i++);
	trace( "%s", d.text() );
	return d;
}

CoffSymTab.CoffSymTab(class Core* c,int fd,SymTab *s,long r):(c,fd,s,r)
{
	fhdr = new filehdr;
	ahdr = new aouthdr;
	int i; for( i = 0; i < SCNHDRS; ++i ) shdr[i] = new scnhdr;
}

CoffSymTab.~CoffSymTab()
{
	delete fhdr;
	delete ahdr;
	int i; for( i = 0; i < SCNHDRS; ++i ) delete shdr[i];
}

char *CoffSymTab.gethdr()
{
	int i;
	trace( "%d.getheader()", this );	OK( "CoffSymTab.gethdr" );
	if( lseek( fd, 0L, 0 ) == -1
	 || !ReadOK(fd, (char*)fhdr, sizeof *fhdr)
	 || !ReadOK(fd, (char*)ahdr, sizeof *ahdr ) )
		return SysErr( "symbol table: " );
	_magic = fhdr->f_magic;
	if( fhdr->f_magic != FBOMAGIC )
		return "symbol table: not compiled for 32000 mux";
	entries = fhdr->f_nsyms;
	if( fhdr->f_nscns > SCNHDRS )
		return sf("too many sections: %d", fhdr->f_nscns);
	for( i = 0; i<fhdr->f_nscns; ++i )
		if( !ReadOK(fd, (char*)shdr[i], sizeof(scnhdr) ) )
			return "section header error";
	return 0;
}

char *CoffSymTab.gettbl()
{
	trace( "%d.gettbl()" ); OK("CoffSymTab.gettbl");
	symoff = (SymEnt*)fhdr->f_symptr;
	if( COFFENTRYSIZE*2 != sizeof(SymEnt) ) abort();
	base = new SymEnt[(entries+1)/2];
	if( lseek(fd,(long)symoff,0) == -1
	 || !ReadOK(fd, (char*)base, entries*COFFENTRYSIZE)
	 || !ReadOK(fd, (char*)&strsize, 4) ){
		delete base; base = 0;
		return SysErr( "symbol table: " );
	}
	strings = new char[strsize];
	if( lseek(fd, -4, 1) == -1 || !ReadOK(fd, strings, strsize) ){
		delete strings; strings = 0;
		delete base; base = 0;
		return SysErr( "strings table: " );
	}
	strcpy( strings, "???" );	/* zero string index */
	return 0;
}

/* inline eventually */
SymEnt *CoffSymTab.entry(long i)
	{ return (SymEnt*) ((long)base + i*COFFENTRYSIZE); }

Source *CoffSymTab.tree()
{
	SymEnt	*s, *fsym = 0;
	Source	*src = 0;
	Func	*func;
	Block	*fake = fakeblk();
	Var	*glb, *fst = 0;
	long	inasrc = 0, i = 0, skip = 0;
	UType	*u;
	
	trace( "%d.tree()", this ); OK(0);

	if( _warn = gettbl() ) return 0;
	
	func = new Func(this, 0, 0, 0, "start" );
	func->range.lo = relocation;
	func->_blk = fake;
	func->type.pcc = CHAR;		// why is this CHAR? cf ed8
	glb = globregs(_blk, 13);	/* 0 .. 8, 9=fp, 10=ap, 11=psw, 12=sp */
	while( i < entries ){
		s = entry(i);
		s->mkid(strings);
		switch( s->sclass() ){
		case C_STRTAG:
		case C_UNTAG:
		case C_ENTAG:
			skip = s->endndx();
			if( u = (UType*) idtosym(U_UTYPE,s->id()) ){
				if( u->range.lo == s->size() )
					break;
			}
			++UTypeStubs;
			u = new UType(this, i, skip-1, s->id());
			u->range.lo = s->size();
			u->type.pcc = s->desc();
			u->type.univ = u;
			u->rsib = utype;
			utype = u;
			break;
		case C_EXT:
		case C_STAT:
			if( ISFTN(s->desc()) )
				fsym = s;
			else
				if( s->sclass()== C_EXT )
					gathervar( s, &glb, _blk, U_GLB );
				else {
					if( !inasrc || !src || !src->blk )
						break;
					gathervar( s, &fst, src->blk, U_STA );
				}
			break;
		case C_FILE:
			inasrc = 1;
			char *id = s->filename();		/* un-cfront */
			int l = strlen(id);
			if( id[l-1]=='i' && id[l-2]=='.' )
				id = sf("%0.*sc", l-1, id );
			src = new Source(this, src, id, 0);
			func = 0;
			fst = 0;
			break;
		case C_FCN:
			if( !inasrc || !src ) break;
			if( strcmp(s->id(), ".bf") || !fsym )
				break;
			++FunctionStubs;
			func = new Func(this, src, func, i, fsym->id());
			if( !src->child ) src->child = src->linefunc = func;
			skip = fsym->endndx();
			func->range.lo = fsym->value()+relocation;
			func->range.hi = func->range.lo+fsym->fsize();
			func->lines.lo = s->lnno();
			func->lnnoptr = fsym->lnnoptr();
			if( func->lines.lo > 30000 )	/* C++/coff bug */
				func->lnnoptr = 0;
			func->regsave = -1;
			func->lines.hi = func->lines.lo+entry(skip-2)->lnno()-1;
			func->size = skip-i;
			func->type = gatherdtype(fsym);
			break;
		default:
			break;
		}
		i = skip ? skip : i+s->numaux()+1;
		skip = 0;
	}
	while( src && src->lsib ) src = (Source*) src->lsib;
	return src;
}

void CoffSymTab.gathervar( SymEnt *s, Var **v, Block *b, UDisc d )
{
	trace( "%d.gathervar(%d,%d,%d,%d)", s, v, b, d ); VOK;
	IF_LIVE( !v ) return;
	if( s->id()[0] == '.' ) return;			/* .text, .bss, .data */
	*v = new Var( this, b, *v, d, s->id() );
	if( b && !b->var ) b->var = *v;
	(*v)->type = gatherdtype(s);
	(*v)->range.lo = s->value();
	if( d == U_GLB || d == U_STA )
		(*v)->range.lo += relocation;
}

Block *CoffSymTab.gatherfunc(Func *func)
{
	Block *ablk, *lblk;
	Var *arg = 0, *lcl = 0;
	Stmt *stmt = 0;
	long i, bf = func->begin, ef = bf+func->size, argvalue = 0;
	SymEnt *s;

	++FunctionGathered;
	trace( "%d.gatherfunc(%d,%d)", this, bf, size );	OK(0);
	ablk = new Block( this, 0, 0, sf("{%s.args}",func->text()) );
	lblk = new Block( this, ablk, 0, sf("{%s.lcls}",func->text()) );
	ablk->child = lblk;
	for( i = bf; i < ef; i += s->numaux()+1 ){
		s = entry(i);
		s->mkid(strings);
		switch( s->sclass() ){
		case C_STAT:
			gathervar( s, &lcl, lblk, U_STA );
			break;
		case C_REGPARM:
		case C_REG:
			gathervar( s, &lcl, lblk, U_REG );
			if( s->sclass() == C_REG ) break;
			gathervar( s, &arg, ablk, U_ARG );
			arg->range.lo = (argvalue += 4) - arg->type.size_of();
			break;
		case C_ARG:
			gathervar( s, &arg, ablk, U_ARG );
			argvalue += arg->type.size_of()+3;
			argvalue &= ~3;
			break;
		case C_AUTO:
			gathervar( s, &lcl, lblk, U_AUT );
			break;
		}
	}
	LineNo lines[MAXLINENO];
	register struct LineNo *l = lines;
	if( func->lnnoptr && lseek(fd, func->lnnoptr, 0) != -1
	 && ::read(fd, (char*)lines, MAXLINENO*LINENOSIZE) >= 2*LINENOSIZE ){
		for( i = 1; i < MAXLINENO; ++i ){
			l = (LineNo*) ((long)lines + i*LINENOSIZE);
			if( l->line == 0 ) break;
			l->line += func->lines.lo-1;
			l->addr += relocation;
			while( stmt && stmt->lineno > l->line ){
				stmt = (Stmt*) stmt->lsib;
				if( stmt ) stmt->rsib = 0;
				else ablk->stmt = 0;
			}
			if( stmt && stmt->lineno == l->line )
				continue;
			if( stmt ) stmt->range.hi = l->addr;
			stmt = new Stmt(this,lblk,stmt);
			if( !ablk->stmt ) ablk->stmt = stmt;
			stmt->lineno = l->line;
			stmt->range.lo = l->addr;
			stmt->range.hi = func->range.hi;
			if( !ablk->range.lo ) ablk->range.lo = stmt->range.lo;
			ablk->range.hi = stmt->range.lo;
		}
	}
	uncfront( ablk->var, "_auto_" );
	uncfront( lblk->var, "_auto_" );
	return ablk;
}

Var *CoffSymTab.gatherutype(UType *u)
{
	SymEnt	*s;
	Var	*first = 0, *v = 0;
	long	i, strtag = u->begin, eos = strtag+u->size;

	++UTypeGathered;
	trace( "%d.gatherutype(%d,%d)", this, strtag, u->size );
	for( i = strtag; i < eos; i += s->numaux()+1 ){
		s = entry(i);
		s->mkid(strings);
		switch( s->sclass() ){
		case C_FIELD:
		case C_MOS:
		case C_MOU:
		case C_MOE:
			gathervar(s, &v, 0, U_MOT);
			if( !first ) first = v;
			if( s->sclass()==C_FIELD ){
				v->range.lo += (v->type.dim = s->fldsize());
				int lo5 = v->range.lo&31;
				v->range.lo &= ~31;
				v->range.lo |= 32-lo5;
				switch( s->desc() ){
				case INT:	v->type.pcc = BITS; break;
				case UNSIGNED:	v->type.pcc = UBITS;
				}
			}
			break;
		case C_EOS:
			goto Out;
		}
	}
Out:
	uncfront(first, sf("_%s_", u->_text));
	return first;
}
