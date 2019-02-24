#include "mcc.h"
#include "symtab.pri"
#include "dtype.pri"
#include "symbol.h"
SRCFILE("mccsymtab.c")

DType MccSymTab.chain(int t, mlist *n)
{
	DType d;

	trace("%d.chain(0%o,%d) %s", this, t, n, PccName(t));
	if( t&TMASK ){
		d.pcc = t&TMASK;
		d.univ = new DType;
		*(d.ref()) = chain(DECREF(t), n);
	} else {
		d.pcc = t==INT ? SHORT : t;
		if((t==STRTY || t==ENUMTY || t==UNIONTY) && (++n)->other==S_TYID) 
			d.univ = idtosym(U_UTYPE,n->name,0);
	}
	return d;
}

DType MccSymTab.gatherdtype(mlist *n)
{
	DType d, *dp;

	trace("%d.gatherdtype(%d) %s", this, n);
	d = chain(n->desc,n);
	if( (++n)->other==S_TYID ) ++n;
	for( dp = &d; dp; dp = dp->ref() )
		if( dp->isary() && n->other==S_DIM )
			dp->dim = n++->desc;
	return d;
}

MccSymTab.MccSymTab(Core* c, int fd, SymTab *i, long r):(c, fd, i, r)
{
	hdr = new exec;
}

MccSymTab.~MccSymTab()
{
	delete hdr;
	delete base;
}

#define MAGABS 0407
char *MccSymTab.gethdr()
{
	trace("%d.gethdr()", this);	OK("MccSymTab.gethdr");
	if( lseek(fd, 0L, 0) == -1 || !ReadOK(fd, (char*)hdr, sizeof *hdr) )
		return SysErr("symbol table: ");
	_magic = hdr->a_magic;
	if( _magic != MAGABS )
		return "symbol table: not 68000 absolute text";
	entries = hdr->a_syms/sizeof(mlist);
	return 0;
}

Block *MccSymTab.gatherfunc(Func *func)
{
	register mlist *n;
	register Block *ablk, *lblk;
	Var *arg = 0, *lcl = 0;
	register Stmt *stmt = 0;
	long bfun = func->begin, size = func->size, i, inasrc = 1;
	char *so = func->source()->_text;

	++FunctionGathered;
	trace("%d.gatherfunc(%d,%d,%s)", this, bfun, size, so);	OK(0);
	IF_LIVE( bfun < 0 || bfun+size > entries ) return 0;
	n = base+bfun;
	IF_LIVE( n->other != S_BFUN ) return 0;
	ablk = new Block(this, 0, 0, sf("%s().arg_blk",n->name));
	lblk = new Block(this, ablk, 0, sf("%s().lcl_blk",n->name));
	ablk->child = lblk;
	for( i = 0; i < size; ++i, ++n ){
		if( n->other == S_SOL ){
			inasrc = eqstr(n->name, so);
			n->other = S_SLINE;			// ???
		}
		if( !inasrc ) continue;
		switch( n->other ){
		case S_PSYM:
			gathervar(n, &arg, ablk, U_ARG);
			break;
		case S_LSYM:
			gathervar(n, &lcl, lblk, U_AUT);
			lcl->range.lo = - lcl->range.lo;	// mc68000
			break;
		case S_STSYM:
		case S_LCSYM:
			gathervar(n, &lcl, lblk, U_STA);
			break;
		case S_RSYM:
			gathervar(n, &lcl, lblk, U_REG);
			break;
		case S_BFUN:
			n->value += 2;				// ????? vax
//		case S_RFUN:
		case S_SLINE:
			while( stmt && stmt->lineno > n->desc ){
				stmt = (Stmt*) stmt->lsib;
				if( stmt ) stmt->rsib = 0;
				else ablk->stmt = 0;
			}
			if( stmt && stmt->lineno == n->desc ) break;
			if( stmt ) stmt->range.hi = n->value;
			stmt = new Stmt(this, lblk, stmt);
			if( !ablk->stmt ) ablk->stmt = stmt;
			stmt->lineno = n->desc;
			stmt->range.lo = n->value;
			if( !ablk->range.lo )
				ablk->range.lo = n->value;
			ablk->range.hi = n->value;
			break;
		case S_EFUN:
			if( stmt ) stmt->range.hi = n->value;
			goto BreakOut;
		}
	}
BreakOut:
	uncfront(ablk->var, "_auto_");
	uncfront(lblk->var, "_auto_");
	return ablk;
}

void MccSymTab.gathervar(mlist *n, Var **v, Block *b, UDisc d)
{
	trace("%d.gathervar(%d,%d,%d,%d)", n, v, b, d); VOK;
	IF_LIVE( !v ) return;
	*v = new Var(this, b, *v, d, n->name);
	if( b && !b->var ) b->var = *v;
	(*v)->range.lo = n->value;
	(*v)->type = gatherdtype(n);
}

char *MccSymTab.gettbl()
{
	trace( "%d.gettbl()" ); OK("MccSymTab.gettbl");
	base = new mlist[entries];
	symoff = (mlist*) (sizeof *hdr + hdr->a_text + hdr->a_data);
	if( lseek(fd, (long)symoff, 0) == 1
	 || !ReadOK(fd, (char*)base, hdr->a_syms) ){
		delete base; base = 0;
		return SysErr( "symbol table: " );
	}
	return 0;
}

#define IGNORE_(s) ( ((s) && *(s)=='_') ? (s)+1 : (s) )
Source *MccSymTab.tree()
{
	register mlist	*n, *base_entries, *r;
	Source		*src = 0;
	Func		*func = 0;
	Block		*fake = fakeblk();
	Var		*glb = 0, *fst = 0, *resolve;
	UType		*u;
	register	inafunc = 0, inasrc = 0;
	
	trace("%d.tree()", this); OK(0);
	if( _warn = gettbl() ) return 0;
	base_entries = base+entries;
	glb = globregs(_blk, 16);
	for( n = base; n < base_entries; ++n ){		// lazy mkid for SSYMs?
		n->mkid();
		if( n->type==N_ABS && n->other==S_BSTR ){
			for( r = n+1; r->other != S_ESTR; ++r ) {}
			if( u = (UType*) idtosym(U_UTYPE,n->name,0) ){ 
				if( u->size<r+1-n ){	/* use the biggest */
					u->size = r+1-n;
					u->begin = n-base;
					u->range.lo = r->value;
					u->type.pcc = n->desc;
				}
				continue;
			}
			++UTypeStubs;
			u = new UType(this, n-base, r+1-n, n->name); 
			u->range.lo = r->value;
			u->type.pcc = n->desc;
			u->type.univ = u;
			u->rsib = utype;
			utype = u;
		}
	}
	for( n = base; n < base_entries; ++n ){
		switch( n->other ){
		case 0: switch( n->type ){
			case N_TEXT|N_EXT:	// ????
				if( idtosym(U_FUNC, IGNORE_(n->name), 0) ) break; 
				func = new Func(this,0,0,0,n->name);
				func->range.lo = n->value;
				func->_blk = fake;
				DType *t = new DType;
				t->pcc = LONG;
				func->type = t->incref();
				func->type.pcc = FTN;
				break;
			case N_ABS|N_EXT:
			case N_BSS|N_EXT:
			case N_DATA|N_EXT:
				resolve = (Var*) idtosym(U_GLB, n->name, 0);
				if( resolve ){
					if( !resolve->range.lo )
						resolve->range.lo = n->value;
				} else {
					n->desc = LONG;
					gathervar(n, &glb, _blk, U_GLB);
				}
				break;
			}
			break;
		case S_GSYM:
			if( !inasrc || inafunc ) break;
			resolve = (Var*) idtosym(U_GLB, n->name, 0);
			if( !resolve )
				gathervar( n, &glb, _blk, U_GLB );
			else
				resolve->type = gatherdtype(n);
			break;
		case S_LCSYM:
		case S_STSYM:
			if( !inasrc || inafunc || !src || !src->blk ) break;
			gathervar(n, &fst, src->blk, U_FST);
			break;
		case S_SO:
			char *srcname = n->name;
			while( n[1].other==S_SO )
				srcname = sf("%s%s", (++n)->name);
			inasrc = 1;
			src = new Source(this,src,srcname,0);
			func = 0;
			inafunc = 0;
			fst = 0;
			break;
		case S_SOL:
			inasrc = src && eqstr(src->_text,n->name); 
			break;
		case S_BFUN:
			if( !inasrc || !src ) break;
			inafunc = 1;
			++FunctionStubs;
			func = new Func(this,src,func,n-base,n->name); 
			if( !src->child ) src->child = src->linefunc = func;
			func->range.lo = n->value;
			func->lines.lo = n->desc;
			if( (n+1)->other == S_GSYM )
				func->type = gatherdtype(++n);
			break;
//		case N_ESO:
//			inasrc = inafunc = 0;
//			break;
		case S_EFUN:
			/* if( !inasrc ) break; screws yyparse() */
			if( !inafunc || !func ) break;
			inafunc = 0;
			func->lines.hi = n->desc;
			func->size = n-base - func->begin + 1;
		}
	}
	while( src && src->lsib ) src = (Source*) src->lsib;
	return src;
}

Var *MccSymTab.gatherutype(UType *u)
{
	register mlist	*n = base+u->begin;
	Var	*first = 0, *v = 0;

	++UTypeGathered;
	trace("%d.gatherutype(%s,%d,%d)", this, u->_text, u->begin, u->size);
	for( int i = 0; i < u->size; ++i, ++n ){
		switch( n->other ){
		case S_SSYM:
			gathervar(n, &v, 0, U_MOT);
			if( !first ) first = v;
			break;
		case S_ESTR:
			goto Out;
		}
	}
Out:
	uncfront(first, sf("_%s_", u->_text));
	return first;
}
