#include "ed8.h"
#include "symtab.pri"
#include "dtype.pri"
#include "symbol.h"
SRCFILE("ed8symtab.c")

//char *MagicName(long m)
//{
//	static char strange[64];
//
//	switch( m ){
//	case OMAGIC : return "old-impure-format";
//	case ZMAGIC : return "demand-load-format";
//	case NMAGIC : return "read-only-text";
//	default	    : return Name( "magic=0%o", m );
//	}
//}

char *N_Name(int n_type)
{
	char *type;
	static char name[64];

	switch( n_type&~N_EXT ){
		default:	type = Name("N_0x%X", n_type );
		case N_ABS:	type = "ABS";		break;
		case N_BCOMM:	type = "BCOMM";		break;
		case N_BSS:	type = "BSS";		break;
		case N_BSTR:	type = "BSTR";		break;
		case N_COMM:	type = "COMM";		break;
		case N_DATA:	type = "DATA";		break;
		case N_DIM:	type = "DIM";		break;
		case N_ECOML:	type = "ECOML";		break;
		case N_ECOMM:	type = "ECOMM";		break;
		case N_BFUN:	type = "BFUN";		break;
		case N_EFUN:	type = "EFUN";		break;
		case N_ENTRY:	type = "ENTRY";		break;
		case N_ESO:	type = "ESO";		break;
		case N_ESTR:	type = "ESTR";		break;
		case N_FN:	type = "FN";		break;
		case N_FNAME:	type = "FNAME";		break;
		case N_GSYM:	type = "GSYM";		break;
		case N_LBRAC:	type = "LBRAC";		break;
		case N_LCSYM:	type = "LCSYM";		break;
		case N_LENG:	type = "LENG";		break;
		case N_LSYM:	type = "LSYM";		break;
		case N_PSYM:	type = "PSYM";		break;
		case N_RBRAC:	type = "RBRAC";		break;
		case N_RFUN:	type = "RFUN";		break;
		case N_RSYM:	type = "RSYM";		break;
		case N_SFLD:	type = "SFLD";		break;
		case N_SLINE:	type = "SLINE";		break;
		case N_SO:	type = "SO";		break;
		case N_SOL:	type = "SOL";		break;
		case N_SSYM:	type = "SSYM";		break;
		case N_STSYM:	type = "STSYM";		break;
		case N_TEXT:	type = "TEXT";		break;
		case N_TYID:	type = "TYID";		break;
		case N_UNDF:	type = "UNDF";		break;
		case N_VER:	type = "VER";		break;
	}
	sprintf( name, "%s%s", type, n_type&N_EXT ? "'" : "" );
	return name;
}

//char *nlisttext(struct nlist *n)
//{
//	static char buf[128];
//
//	sprintf( buf, "%s	%s	%d	0%o=%d=%s %s",
//		n->n_un.n_name, N_Name(n->n_type), n->n_value,
//			n->n_desc,n->n_desc,PccName(n->n_desc),
//				(n+1)->n_type==N_TYID?(n+1)->n_un.n_name:"");
//	return buf;
//}

DType Ed8SymTab.chain(int t, nlist *n)
{
	DType d;

	trace( "%d.chain(0%o,%d) %s", this, t, n, PccName(t) );
	if( t&TMASK ){
		d.pcc = t&TMASK;
		d.univ = new DType;
		*(d.ref()) = chain(DECREF(t), n);
	} else {
		d.pcc = t;
		if((t==STRTY || t==ENUMTY || t==UNIONTY) && (++n)->n_type==N_TYID) 
			d.univ = idtosym(U_UTYPE,n->n_un.n_name,0);
	}
	return d;
}

DType Ed8SymTab.gatherdtype(nlist *n)
{
	DType d, *dp;

	trace( "%d.gatherdtype(%d)", this, n);
	d = chain(n->n_desc,n);
	if( (++n)->n_type==N_TYID ) ++n;
	for( dp = &d; dp; dp = dp->ref() )
		if( dp->isary() && n->n_type==N_DIM )
			dp->dim = n++->n_value;
	trace( "%s", d.text() );
	return d;
}

//void execdump(exec h)
//{
//	trace( "a_magic=%s a_text=%d a_data=%d",
//		MagicName(h.a_magic), h.a_text,	h.a_data );
//	trace( "a_bss=%d a_syms=%d a_entry=%d", h.a_bss, h.a_syms, h.a_entry );
//	trace( "a_trsize=%d a_drsize=%d", h.a_trsize, h.a_drsize );
//}

Ed8SymTab.Ed8SymTab(Core* c, int fd, SymTab *i):(c, fd, i)
{
	hdr = new exec;
}

Ed8SymTab.~Ed8SymTab() { delete hdr; }

char *Ed8SymTab.gethdr()
{
	trace( "%d.gethdr()", this );	OK( "Ed8SymTab.gethdr" );
	if( lseek( fd, 0L, 0 ) == -1 || !ReadOK(fd, (char*)hdr, sizeof *hdr) )
		return SysErr( "symbol table: " );
	_magic = hdr->a_magic;
	if( N_BADMAG(*hdr) || hdr->a_trsize || hdr->a_drsize )
		return "symbol table: not executable text";
	entries = hdr->a_syms/sizeof(nlist);
	return 0;
}

Block *Ed8SymTab.gatherfunc(Func *func)
{
	register struct nlist *f, *n;
	register Block *ablk, *lblk;
	Var *arg = 0, *lcl = 0, *common = 0;
	register Stmt *stmt = 0;
	long bfun = func->begin, size = func->size, i, inasrc = 1, equiv = 0;
	char *so = func->source()->_text;

	++FunctionGathered;
	SymbolStats();
	trace( "%d.gatherfunc(%d,%d,%s)", this, bfun, size, so );	OK(0);
	IF_LIVE( bfun < 0 || bfun+size > entries ) return 0;
	if( !(n = f = nlistvector(bfun,size)) ) return 0;
	IF_LIVE( n->n_type != N_BFUN ) return 0;
	ablk = new Block( this, 0, 0, sf("%s().arg_blk",n->n_un.n_name) );
	lblk = new Block( this, ablk, 0, sf("%s().lcl_blk",n->n_un.n_name) );
	ablk->child = lblk;
	for( i = 0; i < size; ++i, ++n ){
		if( n->n_type == N_SOL ){
			inasrc = eqstr( n->n_un.n_name, so );
			n->n_type = N_SLINE;
		}
		if( !inasrc ) continue;
		switch( n->n_type ){
		case N_BCOMM:					// f77
			if( n->n_un.n_name[0] != '?' )
				common = (Var*) idtosym(U_GLB,n->n_un.n_name,0);
			else
				if( n[1].n_type == N_STSYM 
				||  n[1].n_type == N_LCSYM )
					equiv = (++n)->n_value;
			break;
		case N_SSYM:					// f77
			if( common || equiv ){
				gathervar( n, &lcl, lblk, U_STA );
				lcl->range.lo += common ? common->range.lo
							: equiv;
			}
			break;
		case N_ECOMM:					// f77
		case N_ECOML:
			common = 0;
			equiv = 0;
			break;
		case N_PSYM:
			gathervar( n, &arg, ablk, U_ARG );
			break;
		case N_LSYM:
			gathervar( n, &lcl, lblk, U_AUT );
			lcl->range.lo = - lcl->range.lo;	/* VAX */
			break;
		case N_STSYM:
		case N_LCSYM:
			gathervar( n, &lcl, lblk, U_STA );
			break;
		case N_RSYM:
			gathervar( n, &lcl, lblk, U_REG );
			break;
		case N_BFUN:
			n->n_value += 2;			/* VAX */
		case N_RFUN:
		case N_SLINE:					// cfront screws up
			if( stmt && n->n_desc <= stmt->lineno ) break;
			if( stmt ) stmt->range.hi = n->n_value;
			stmt = new Stmt(this,lblk,stmt);
			if( !ablk->stmt ) ablk->stmt = stmt;
			func->lines.hi = stmt->lineno = n->n_desc;
			stmt->range.lo = n->n_value;
			if( !ablk->range.lo )
				ablk->range.lo = n->n_value;
			ablk->range.hi = n->n_value;
			break;
		case N_EFUN:
			if( stmt ) stmt->range.hi = n->n_value;
			goto BreakOut;
		}
	}
BreakOut:
	delete f;
	uncfront( ablk->var, "_auto_" );
	uncfront( lblk->var, "_auto_" );
	return ablk;
}

void Ed8SymTab.gathervar( nlist *n, Var **v, Block *b, UDisc d )
{
	trace( "%d.gathervar(%d,%d,%d,%d)", n, v, b, d ); VOK;
	IF_LIVE( !v ) return;
	*v = new Var( this, b, *v, d, n->n_un.n_name );
	if( b && !b->var ) b->var = *v;
	(*v)->range.lo = n->n_value;
	(*v)->type = gatherdtype(n);
}

char *Ed8SymTab.gettbl()
{
	trace( "%d.gettbl()" ); OK("SymTab.gettbl");
	base = new nlist[entries];
	symoff = (nlist*)N_SYMOFF(*hdr);
	if( lseek(fd, (long)symoff, 0) == 1
	 || !ReadOK(fd, (char*)base, hdr->a_syms)
	 || !ReadOK(fd, (char*)&strsize, 4) ){
		delete base; base = 0;
		return SysErr( "symbol table: " );
	}
	strings = new char[strsize];
	if( lseek( fd, -4, 1 ) == -1 || !ReadOK(fd, strings, strsize) ){
		delete strings; strings = 0;
		delete base; base = 0;
		return SysErr( "strings table: " );
	}
	strcpy( strings, "???" );	/* zero string index */
	return 0;
}

int VERstamp = 0x4314;	/* 8YMDD */
Source *Ed8SymTab.tree()
{
	register nlist	*n, *base_entries, *r;
	Source		*src = 0;
	Func		*func = 0;
	Block		*fake = fakeblk();
	Var		*glb = 0, *fst = 0, *resolve;
	UType		*u;
	register long	regstrings, inafunc = 0, inasrc = 0;
	
	trace( "%d.tree()", this ); OK(0);
	if( _warn = gettbl() ) return 0;
	base_entries = base+entries;
	regstrings = (long) strings;
	glb = globregs(_blk, 16 );
	for( n = base; n < base_entries; ++n ){
		n->n_un.n_strx += (long) regstrings;
		switch( n->n_type ){
		case N_BSTR:
			for( r = n+1; r->n_type != N_ESTR; ++r ) {}
			if( u = (UType*) idtosym(U_UTYPE,n->n_un.n_name,0) ){
				if( u->size<r+1-n ){	/* use the biggest */
					u->size = r+1-n;
					u->begin = n-base;
					u->range.lo = r->n_value;
					u->type.pcc = n->n_desc;
				}
				break;
			}
			++UTypeStubs;
			u = new UType(this, n-base, r+1-n, n->n_un.n_name );
			u->range.lo = r->n_value;
			u->type.pcc = n->n_desc;
			u->type.univ = u;
			u->rsib = utype;
			utype = u;
		}
	}
	for( n = base; n < base_entries; ++n ){
		switch( n->n_type ){
		case N_ABS|N_EXT:
			if( n->n_un.n_name[0] != '_' ) break;	/* ? */
		case N_BSS|N_EXT:
		case N_DATA|N_EXT:
			if( *n->n_un.n_name == '_' ) ++n->n_un.n_name;
			resolve = (Var*)idtosym(U_GLB, n->n_un.n_name, 0);
			if( resolve ){
				if( !resolve->range.lo )
					resolve->range.lo = n->n_value;
			} else {
				n->n_desc = LONG;
				gathervar( n, &glb, _blk, U_GLB );
				break;
			}
			break;
		case N_GSYM:
			if( !inasrc || inafunc ) break;
			if( !idtosym(U_GLB, n->n_un.n_name, 0) )
				gathervar( n, &glb, _blk, U_GLB );
			break;
		case N_LCSYM:
		case N_STSYM:
			if( !inasrc || inafunc || !src || !src->blk ) break;
			gathervar( n, &fst, src->blk, U_FST );
			break;

		case N_SO:
			trace( "N_VER 0x%X", n[1].n_desc );
			if( n[1].n_type!=N_VER || n[1].n_desc<VERstamp){
			    _warn = sf("%s: old cc", n->n_un.n_name);
			    break;
			}
			inasrc = 1;
			src = new Source(this,src,n->n_un.n_name,n[1].n_value);
			func = 0;
			inafunc = 0;
			fst = 0;
			break;
		case N_SOL:
			inasrc = src && eqstr(src->_text,n->n_un.n_name);
			break;
		case N_TEXT|N_EXT:
			if( *n->n_un.n_name == '_' ) ++n->n_un.n_name;
// f77			for( char *p = n->n_un.n_name; p && p[1]; ++p ) {}
// f77			if( *p == '_' ) *p = 0;
// f77__ conflicts with cfront type signatures__ (dmg: sorry)
			if( idtosym(U_FUNC, n->n_un.n_name, 0) ) break;
			func = new Func(this,0,0,0,n->n_un.n_name);
			func->range.lo = n->n_value;
			func->_blk = fake;
			DType *t = new DType;
			t->pcc = LONG;
			func->type = t->incref();
			func->type.pcc = FTN;
			break;
		case N_BFUN:
			if( !inasrc || !src ) break;
			inafunc = 1;
			++FunctionStubs;
			func = new Func(this,src,func,n-base,n->n_un.n_name);
			if( !src->child ) src->child = src->linefunc = func;
			func->range.lo = n->n_value;
			func->lines.lo = n->n_desc;
			if( !strcmp(n->n_un.n_name,(n+1)->n_un.n_name) )
				func->type = gatherdtype(++n);
			break;
		case N_ESO:
			inasrc = inafunc = 0;
			break;
		case N_EFUN:
			/* if( !inasrc ) break; screws yyparse() */
			if( !inafunc || !func ) break;
			inafunc = 0;
			func->lines.hi = n->n_desc;
			func->size = n-base - func->begin + 1;
		}
	}
	while( src && src->lsib ) src = (Source*) src->lsib;
	if( base ) { delete base; base = 0; }
	return src;
}

Var *Ed8SymTab.gatherutype(UType *u)
{
	nlist *f, *n;
	Var *first = 0, *v = 0;
	long bstr = u->begin, size = u->size;

	++UTypeGathered;
	SymbolStats();
	trace( "%d.gatherutype(%d %d)", this, bstr, size );
	if( !(f = n = nlistvector(bstr,size)) ) return 0;
	for( ++n; n->n_type != N_ESTR; ++n ){
		switch( n->n_type ){
		case N_SFLD:
			int length = n->n_desc>>BTSHIFT;
			switch( n->n_desc & BTMASK ){
			case INT:	n->n_desc = BITS; break;
			case UNSIGNED:	n->n_desc = UBITS;
			}
			gathervar(n, &v, 0, U_MOT);
			v->type.dim = length;
			break;
		case N_SSYM:
			gathervar(n, &v, 0, U_MOT);
		}
		if( !first ) first = v;
	}
	delete f;
	uncfront(first, sf("_%s_", u->_text));
	return first;
}

nlist *Ed8SymTab.nlistvector(long start, long size )
{
	struct nlist *n;
	int i;

	trace( "%d.nlistvector(%d,%d) %d", this, start, size, symoff );
	lseek(fd, (long) (symoff + start), 0);
	n = new nlist[size];
	IF_LIVE( !ReadOK(fd, (char*) n, size * sizeof *n) ){
		delete n;
		return 0;
	}
	for( i = 0; i < size; ++i )
		n[i].n_un.n_strx += (long) strings;
	return n;
}

