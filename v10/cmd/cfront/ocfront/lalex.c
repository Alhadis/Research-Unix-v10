/*ident	"@(#)ctrans:src/lalex.c	1.6.3.37" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

lalex.c:

	lookahead 

*****************************************************************************/
#include <stdio.h>

#ifdef c_plusplus
overload is_empty;
#endif

#include "cfront.h"
#include "yystype.h"
#include "tqueue.h"

#ifdef DBG
#define LDB(val,a) { if(Ldebug>=val) {a;} }
#else
#define LDB(val,a) /**/
#endif

#ifdef DBG
static char*
image( int t )
{
	if(keys[t]) return keys[t];
	else { static char b[20];
		sprintf(b,"token(%d)",t);
		return b;
	}
}
static void
showQ( char* where )
/*
	display token Q
*/
{
	fprintf(stderr,"TOKEN Q (%s):\n",where);
	for (register toknode* t = front; t; t = t->next) {
		fprintf(stderr,"\t%s",image(t->tok));
		switch(t->tok) {
		case ID: case ICON: case CCON: case FCON: case STRING:
			fprintf(stderr," '%s'",t->retval.s);
			break;
		case TNAME:
			fprintf(stderr," '%s'",t->retval.pn->string);
			break;
		}
		putc(' ',stderr);
		t->place.put(stderr);
		putc('\n',stderr);
	}
	putc('\n',stderr);
}
#endif

int bl_level;

static int  laexpr( TOK );
static int  latype( TOK );
static int  la_decl();
static TOK  lookahead();

/* make this a toknode! */
static int lasttk = 0;		// one token history
static YYSTYPE lastval; 	// yylval lasttk value 

int must_be_expr = 0;		// handle redundant parentheses
int must_be_id = 0;		// !0, TNAME => ID, i.e., int X

loc curloc;
int curr_file;

toknode* latok;			// current lookahead token
toknode* front = 0;
toknode* rear  = 0;

static toknode* free_toks = 0;
const  TQCHUNK = 16;

toknode::toknode(TOK t, YYSTYPE r, loc tloc)
{
	if (free_toks == 0) {
		register toknode* q;
		free_toks = q = (toknode*)malloc( TQCHUNK * sizeof(toknode) );
		for (; q != &free_toks[TQCHUNK-1]; q->next = q+1, ++q);
		q->next = 0;
	}
	this = free_toks; free_toks = free_toks->next;
	tok = t;
	used = 0;
	retval = r;
	place = tloc;
	next = last = 0;
}

toknode::~toknode()
{
	next = free_toks;
	free_toks = this;
	this = 0;
}

#define USE_TOKEN(T,W) \
	LDB(2,error('d',&(T)->place,"use_token('%k','%s')",(T)->tok,W);); \
	if ( !(T)->used ) use_token(T);

Ptype
return_nstd_local_type( Pname n, TOK &sw )
{ 
    	Ptype tt;
	switch ( n->tp->base ) {
		case EOBJ:
		case COBJ:
			tt = Pbase(n->tp)->b_name->tp;
			sw = n->tp->base;
			break;
		default:
			tt = n->tpdef;
			sw = NESTED; // in repr.c, prints ``typedef''
			break;
	}
	return tt;
}

static Pname
local_nested_kludge( Pname n, Pname tn )
/*
 * for backward compatibility with 2.0
 * in transitional model of nested class types
 * 
 * struct T { ... };
 * foobar() {
 *	class X {
 *		typedef int T;
 *		// ...
 *	};
 *	T t;
 * }
 *
 * pure nested classes, choose global struct T{};
 * no nested classes, choose typedef int T
 * transitional model: choose typedef, and generate warning
 * BUG: local typedefs and enums do not have lex_level set
 */
{
// error( 'd', "local_nested_kludge: n: %n", n );
	for ( Pname nn = n; nn; nn = nn->n_tbl_list )
	{
		Pname local_class;
		TOK ntd;
		if ( nn->n_key != NESTED ) continue;
		Ptype tt = return_nstd_local_type(nn,ntd);
		Pclass cl = tt->in_class;
		while ( cl->in_class ) cl = cl->in_class;
		if (cl->lex_level && 
			(local_class = ktbl->look(cl->string,LOCAL)))
		{
			// same typedef at nested and non-nested scope
			if (ntd == NESTED && tn && tn->tp == nn->tp )
				; 
			else
				error('w',"%s occurs at outer and nested localC scope; using %k %t::%s", n->string,ntd,cl,n->string);

/*
			if ( ntd == NESTED && nn->n_dcl_printed != 2 ) {
				nn->dcl_print(0);
				nn->n_dcl_printed = 2;
			}
*/
			return nn;
		}
	}
	return 0;
}

enum { one_back, two_back };
	
static void
use_token( toknode* T )
/*
	lookup TNAMEs here instead of in tlex()
	maintain block level
*/
{
	static TOK last_tokens[2];  // TSCOPE not reduced at this point
	static Pname last_tname;    // tname :: id, where id is nested class
	static Pname tdef_name;     // typedef tname tdef_name

	T->used = 1;

// error('d', &T->place, "use_token: %k, lasttk: %k",T->tok,lasttk );
// error('d', &T->place, "last_tokens: %k %k",last_tokens[two_back],last_tokens[one_back]);
// error('d', &T->place, "use_token: last_tname: %n", last_tname);

	switch ( T->tok ) {
	case ID:
		Pname n;
// error('d', &T->place, "use_token: %s", T->retval.s );
		if ( last_tokens[one_back] == MEM &&
			last_tokens[two_back] == TNAME &&
			(n=ktbl->look(T->retval.s,NESTED))) 
		{ // TYPEDEF :: ID, nested class ctor 
			if (tdef_name && tdef_name->n_key==NESTED &&
				strcmp(tdef_name->string,n->string)==0)
			{
				T->tok = TNAME;
				T->retval.pn = n;
				break;
			}	
			else { // TNAME :: ID, where ID may be nested class
			for ( Pname nn = n; nn; nn = nn->n_tbl_list )
			{
				TOK sw;
				if ( nn->n_key != NESTED ) continue;
				Ptype tt = return_nstd_local_type(nn,sw);
				Pclass cl = tt->in_class;
				if (strcmp(last_tname->string,cl->string)==0)
				{
					T->tok = TNAME;
					T->retval.pn = nn;
					break;
				}	
			}
			}
		}
		else
		if ( bl_level && 
			// TNAME:: and :: cannot refer to ``local'' TNAME
			last_tokens[one_back] != MEM  && 
			(n=ktbl->look(T->retval.s,LOCAL)) ) 
		{
			DB( if(Ldebug>=1)error( 'd', &T->place, "use_token: local class instance: %n", n ) );
			T->tok = TNAME;
			T->retval.pn = n;
		} 
		else if ( n=ktbl->look(T->retval.s,0) ) {
			DB( if(Ldebug>=1)error( 'd', &T->place, "use_token: global class instance: %n", n ) );
// error( 'd', &T->place, "use_token: global class instance: %n %t", n, n->tp );

			// X:: ?, then n cannot be a global TNAME
			// 	except in the case of a constructor
			if (last_tokens[one_back] == MEM &&
				last_tokens[two_back] == TNAME &&
			 	strcmp(T->retval.s,last_tname->string))
					; // do nothing; i.e., return ID
			else 
			if ( bl_level && n->tp->base != COBJ &&
				last_tokens[one_back] == MEM &&
				last_tokens[two_back] != TNAME &&
				gtbl->look(T->retval.s,0))
					; // do nothing: local typedefs & enums not implemented
			else {
				Pname nn = 0;
				TOK ntd;
				if (bl_level && (nn=ktbl->look(T->retval.s,NESTED)))
				{
					(void) return_nstd_local_type(n,ntd);
					nn = local_nested_kludge(nn,ntd==NESTED?n:0);
				}
				T->tok = TNAME;
				T->retval.pn = nn?nn:n;
			}
		}
#ifdef DBG
		else if(Ldebug>=1)
			error('d',&T->place,"use_token: id %s",T->retval.s);
#endif
		break;
	case LC: ++bl_level; break;
	case RC: --bl_level; break;
	}
	
	if (T->tok != COMPL || last_tokens[one_back] != MEM) {
		last_tokens[two_back] = last_tokens[one_back];
 		last_tokens[one_back] = T->tok;	
		if (T->tok == TNAME) last_tname = T->retval.pn;
		if ( last_tname && 
			last_tname->tp->base == TYPE )
		{
			tdef_name = last_tname;
			do
				tdef_name = Pbase(tdef_name->tp)->b_name;
			while ( tdef_name->tp->base == TYPE );
		}
	}
}

void
addtok(TOK t, YYSTYPE r, loc tloc)
{
	toknode* T = new toknode(t,r,tloc);
	if (front == 0)
		front = rear = T;
	else {
		rear->next = T;
		T->last = rear;
		rear = T;
	}
//error('d',&tloc,"addtok: %k '%s'",t,t==ID?r.s:"");
//showQ("addtok");
}

extern TOK
deltok( int noset = 0 )
{
	register toknode* T = front;
	USE_TOKEN(T,"deltok");
	register TOK tk = T->tok;
	if ( !noset ) { yylval = T->retval; curloc = T->place; }
	curr_file = curloc.file;
	if (front = front->next)
		front->last = 0;
	else
		latok = rear = 0;
	delete T;
	return tk;
}

static void
add_tokens()
/*
    extend lookahead token queue when depleted
*/
{
    TOK tk = tlex();
    if ( tk != ID )
	return;

    while (tk == ID || tk == MEM || tk == DOT )  
	tk = tlex(); 
}

extern TOK
la_look()
/*
	peek at head of token queue
*/
{
    LDB(1,fprintf(stderr,"\n*** la_look()"));
    if ( front == 0 )
	add_tokens();

    latok = front;
    USE_TOKEN(latok,"la_look");
    LDB(1,fprintf(stderr," -- %s\n",image(latok->tok)));
    return latok->tok;
}

#define NEXTTOK() ( (yychar==-1) ? (yychar=lalex(),yychar) : yychar )
void
check_decl()
/*
	Lookahead to direct parsing of local/arg type declarations
	la_decl() returns 1 if lookahead sees a declaration.
*/
{
	TOK tk2;
	switch( NEXTTOK() ) {
	default:
	break;
	case TSCOPE: //XXX
		tk2 = la_look();
		while ( tk2 == TSCOPE ) tk2 = lookahead();
		if ( tk2 == TNAME ) {
		    toknode* t = latok;
		    if(t->tok!=TNAME)
			error('i',"check_decl() token scan");
		    tk2 = lookahead();
		    if ( tk2 == LP && la_decl() ) {
			t->tok = DECL_MARKER; //TNAME
		    }
		}
		break;
	case TYPE: case TNAME:
	    if ( la_look() == LP && la_decl() ) {
		must_be_id = 0;
		DECL_TYPE=yychar;
		yychar = DECL_MARKER;
	    }
	}
}

void
check_cast()
/*
	Lookahead to direct parsing of cast
	la_cast() returns 1 if lookahead sees an ambiguous old-style C cast.
*/
{
	TOK tk2;
	switch( NEXTTOK() ) {
	case TSCOPE: //XXX
		tk2 = la_look();
		while ( tk2 == TSCOPE ) tk2 = lookahead();
		if ( tk2 == TNAME ) {
		    toknode* t = latok;
		    if(t->tok!=TNAME)
			error('i',"check_cast() token scan");
		    tk2 = lookahead();
		    if ( tk2 == LP && la_decl() ) {
			t->tok = DECL_MARKER;//TNAME
		    }
		}
		break;
	case TYPE: case TNAME:
	    if ( la_look() == LP && la_cast() ) {
		must_be_id = 0;
		DECL_TYPE = yychar;
		yychar = DECL_MARKER;
	    }
	}
}


static int
latype( TOK t )
{
	switch ( t ) {
	default: // includes friend, typedef, storage classes, etc.
		return 0;
	case CHAR: case SHORT: case INT: case LONG:
	case FLOAT: case DOUBLE:
        case UNSIGNED:
		return 1;
	}
}

static int
laexpr( TOK t )
{
	switch ( t ) {
	default: 
		return 0;
	case RETURN: case NEW: case AND: case ANDAND: case OR: case OROR:
	case SIZEOF: case NOT: case COMPL: case MUL: case PLUS: case MINUS: 
	case ER: case ASSIGN: case ASOP: case RELOP: case EQUOP: case DIVOP: 
	case SHIFTOP: case ICOP:
		return 1;
	}
}


static TOK
lookahead()
/*
    advance lookahead pointer, lexing at end of Q
    handle occurrences of TNAME and TSCOPE
    (should be kept up to date with lalex())
*/
{
    TOK tk;
    TOK tk2;
    TOK prev_tk = 0;
    YYSTYPE lastval;

    if ( latok == rear ) {
	add_tokens();
	if ( latok )
	    latok = latok->next;
	else
	    latok = front;
    }
    else
	latok = latok->next;

    if ( latok->last ) {
	prev_tk = latok->last->tok;
	lastval = latok->last->retval;
    }

nexttok:
    USE_TOKEN(latok,"lookahead1");
    tk = latok->tok;
    if ( tk == ID || tk == TNAME ) 
    {
	if (latok->next == 0) add_tokens();
	USE_TOKEN(latok->next,"lookahead2");
	/* TOK */ tk2 = latok->next->tok;
	if ( tk == TNAME ) {
	    if ( tk2 == MEM || tk2 == DOT ) {
	tscope:	tk = TSCOPE;
// error('d',"lookahead: tk: %k tk2: %k", tk, tk2 );
	//XXX -- should be modified to loop and do lookup
		latok = latok->next;
		if (latok->next == 0) add_tokens();
		USE_TOKEN(latok->next,"lookahead3");
		tk2 = latok->next->tok;
		if ( tk2 == MUL ) {
			tk = MEMPTR;
			latok = latok->next;
		}
	    }
	    else if (( prev_tk == MUL && tk2 != RP )
			|| prev_tk == AND )
		{
		tk = ID;
		latok->retval.pn->hide();
		latok->tok = ID;
		latok->retval.s = latok->retval.pn->string;
		}
	}
	else if ( tk2 == MEM ) {
	    	// ID ::
	//XXX	latok = latok->next->next;
	//XXX	goto nexttok;
		goto tscope; // treat as tscope
	}

	if ( tk == ID &&
   		( tk2 == ID ||
		( prev_tk == ID && ( tk2 == COLON || tk2 == LC )))) {
	    		// ID ID
	    		latok = latok->next;
	    		goto nexttok;
			}
    }

	if ( tk == ID ) {
		Pname nstd = ktbl->look(latok->retval.s,NESTED);
		if (nstd && (must_be_id == 0 ||
			must_be_id && prev_tk == LP)) {
extern Pname check_for_nested(Pname,TOK,YYSTYPE,TOK); // use this in lalex, too
				Pname n = check_for_nested(nstd,prev_tk,lastval,tk2);
				if ( n ) {
					tk = latok->tok = TNAME;
					latok->retval.pn = n;
				}
		}
	}

    return tk;
}

static Pname mem_sel = 0;
char *class_typedef = 0;

static Pname
do_local_class( Pname n, int lex_level ) 
{ /*
   * modify to ``do_local_type:
   *   do local types: enums and typedefs
   */
	Pname nn = n;
	if ( n->tp ) { // already a TNAME
    		Pclass cl = n->tp->base==COBJ ? Pclass(Pbase(n->tp)->b_name->tp) : 0;
    		if ( n->lex_level != lex_level || (cl && cl->lcl) ) {
			local_hide( n );
			nn = new name( n->string );
			nn->lex_level = lex_level>=0?lex_level:0;
		}
    		else 
		if ( lex_level == n->lex_level && cl->defined ) {
			error( "localC %n redefined", n );
			return n;
		}
	}

	nn = nn->tname( lastval.t );
	modified_tn = modified_tn->l;
	nn->n_key = LOCAL;
	local_class = new name_list( nn, local_class );
	local_blk = new name_list( nn, local_blk );

// error('d', "do_local_class: nn %n tp %t", nn, Pclass(Pbase(nn->tp)->b_name->tp));
	return nn;
}

static char*
make_nested_name( char *s, Pclass cl )
{ // Q<cnt>_<class_names><space><null>
	const nested_depth = 9;
    	char *str_arr[nested_depth];
    	int  size_arr[nested_depth];
    	int cnt = 2;
    	int size = 4; // Q,<cnt>,<_>,<null>

	str_arr[0] = s; str_arr[1] = cl->string; 

	size += size_arr[0] = strlen(s); 
	size += size_arr[1] = cl->strlen?cl->strlen:strlen(cl->string);

	for (Pclass nc = cl->in_class; nc; nc = nc->in_class ) {
		if (cnt > nested_depth-1) error('s',"nested depth class beyond %d unsupported",nested_depth);
		size += size_arr[cnt] = nc->strlen?nc->strlen:strlen(nc->string);
		str_arr[cnt++] = nc->string; 
	}

	for ( int i=0; i<cnt; i++ ) // <nnn><string>
		size += size_arr[i]>99?3:size_arr[i]<10?1:2;

//error('d', "make_nested_name( %s, %t ) cnt: %d size: %d", s, cl, cnt, size );
	char *result = new char[size];
	sprintf(result, "Q%d_", cnt );
	size = 3;
	for ( i=cnt; i; i-- ) {
		sprintf(result+size,"%d%s", size_arr[i-1], str_arr[i-1]);
		size += size_arr[i-1] + (size_arr[i-1]>99?3:size_arr[i-1]<10?1:2);
	}

//error('d', "size: %d ", size );
	result[size] = '\0';
//error('d', "make_nested_name result: %s", result );
    	return result;
}

int is_empty( Pclass cl, bit const_chk )
{ /* 	for nested class check, empty means *no* members
   *    for const object check, means no *data* members
   */

// error('d',"%t->is_empty: max: %d real_size: %d", cl, cl->memtbl->max(),cl->real_size );

	int mbr_cnt = cl->memtbl->max();
	if ( mbr_cnt == 0 ) return 1;

	if ( cl->baselist == 0 && cl->real_size!=1 )
		return 0;

	// empty class to turn on transitional nested class scope
	if ( const_chk == 0 && 
		( cl->baselist != 0 || mbr_cnt > 1 )) return 0;

	int i = 1;
    	for (Pname nn=cl->memtbl->get_mem(i); nn; nn=cl->memtbl->get_mem(++i)) {
    	  	if (nn->base==NAME && 
			nn->n_union==0 && 
			nn->tp->base!=FCT && 
			nn->tp->base!=OVERLOAD && 
			nn->tp->base!=CLASS && 
			nn->tp->base!=ENUM && 
			nn->tp->base!=EOBJ && 
			nn->n_stclass != STATIC) 
			{
				if ( nn->string[0]=='_' &&
					nn->string[1]=='_' &&
					nn->string[2]=='W' )
						return 1;
				else return 0;
			}
	}

	return 1; // if here, no data members encountered
}

static int
is_empty( Penum en )
{ // is this an empty enum ??

// error('d', "%t no_of_enumerators: %d", en, en->no_of_enumerators);
	if ( en->no_of_enumerators != 0 )
		return 0;

	return 1;
}

static Pname
check_nested_type( Pname nm )
{
// error('d', "check nested type: %n ccl: %t", nm, ccl );

	Pname nx, n = ktbl->look(nm->string, NESTED);
	if ( n == 0 || n == nm ) return nm;

	int cnt = 1;
	for (nx = n; n; n=n->n_tbl_list ) 
		if (n->n_key == NESTED) ++cnt;

	if ( cnt > 1 ) {
		error( "ambiguous nested type %s (%d instances), use x::y syntax", nm->string, cnt );
		error( 'i', "cannot recover from previous errors" );
	}
	else { 
		TOK ntk;
		Ptype tt = return_nstd_local_type(nx,ntk);
		error('w', "use %t:: to access nested %k %s (anachronism)", tt->in_class, ntk, nx->string);
	}
	return nx;
}

static int
in_local_class( Pclass cl )
{
	if ( cl->lex_level )
		return 1;
	if ( cl->in_class )
		return in_local_class( cl->in_class );
	return 0;
}

static Pname
do_nested_type( Pname n ) 
{
	Pname nn = n;
	char *str = 0;

// error('d', "do_nested_type: %s in_typedef: %d ccl: %t", n->string, in_typedef, ccl );
// error('d', "lasttk: %k class_typedef: %s", lasttk, class_typedef);
	if ( in_typedef && ccl->string[0]=='_'
	&&  ccl->string[1]=='_'
	&&  ccl->string[2]=='C') return n;

	if ( n->tp ) 
        { // already a TNAME :
	  // hide existing instance, encode new instance
	  /*
	   * need handle the anomaly:
	   * class x;
	   * x *p;
	   * class y {
	   *	class x{ ... }; // oops
	   * };
	   */
		if (n->tp->base==COBJ) {
			Pclass cl = Pclass(Pbase(n->tp)->b_name->tp);
			if (cl->defined == 0 && lasttk == AGGR) {
				error('w',"forward declaration ofC%n resolved to nested%t::%s",n,ccl,n->string);
				cl->lcl = new char[9];
				strcpy(cl->lcl,"FUDGE007"); // license to hack
				n->lex_level=Pbase(n->tp)->b_name->lex_level=0;
				return n;
			}
		} 
		else if (n->tp->base==EOBJ) { // watch out for enum x;
			Penum en = Penum(Pbase(n->tp)->b_name->tp);
			if (en->defined == 0 && lasttk == ENUM) return n;
		}

		nested_hide( n );
		nn = new name( n->string );
		str = make_nested_name( n->string, ccl );
	}
	else
	// make sure we haven't already seen a nested instance
	// if so, for transition, this needs to be an error
	if (ktbl->look( n->string, NESTED ))
		error("multiple type %s nestings (to do this define an empty class/enum %s {};)",n->string,n->string); 
	if ( in_typedef && 
		( class_typedef == 0 ||
		  strcmp(nn->string,class_typedef)==0 )) 
	{
		if (strcmp(ccl->string, nn->string)==0) { // class x { typedef T x;
			error( "nested Tdef %s redefines immediately enclosing class", nn->string );
			error( 'i', "cannot recover from previous errors" );
		}

		// make sure there isn't an identifier at global scope being defined
		// by a nested typedef -- previously an error; keep it so for transition
		Pname tn;
		if ( n->tp == 0 && in_local_class(ccl)==0 &&
                     (tn=gtbl->look(n->string,0))) {
			error( "nested Tdef %s redefines global %n", n->string, tn );
			error( 'i', "cannot recover from previous errors" );
		}

		nn->tpdef = new type;
		nn->tpdef->nested_sig = str;
		nn->tpdef->in_class = ccl;
		nn->tpdef->lex_level = NESTED;
		PERM(nn->tpdef);
	}
	else {
		nn = nn->tname( lastval.t );
		Pname tn = Pbase(nn->tp)->b_name;
		Ptype tt = tn->tp;
    		if ( tt->defined && tt->in_class == ccl) {
			error( "nested %t redefines immediately enclosing class", nn->string );
			error( 'i', "cannot recover from previous errors" );
		}
		tt->nested_sig = str;
		modified_tn = modified_tn->l;
		nn->lex_level = tn->lex_level = 0;
		nested_type = new name_list( nn, nested_type );
	}
	nn->n_key = NESTED;
	return nn;
}

static Pname dtor_seen;
static int in_expr;
extern int in_sizeof;

extern TOK
lalex()
/*  return next token to grammar  */
{
    register TOK tk;
    if ( front == 0 )
	add_tokens();		// extend lookahead queue
    LDB(1,fprintf(stderr,"\n*** lalex()\n");showQ("before"));

gettok:
    tk = deltok();
// error('d',&curloc,"lalex: just got %k '%s' in_typedef: %d",tk,tk==ID?yylval.s:tk==TNAME?yylval.pn->string:"", in_typedef);

    if ( tk == ID || tk == TNAME ) 
    {
	TOK tk2 = la_look();
	int lex_level = bl_level - in_class_decl - (tk2 == LC );

	if ( tk == TNAME )
	{
//error('d', "lalex tname %n;   lasttk: %k tk2: %k", yylval.pn, lasttk, tk2);
//error('d', "    must_be_id: %d must_be_expr %d  decl_type %d",must_be_id,must_be_expr,DECL_TYPE);
//error('d', "    bl_level: %d parsing_members %d",bl_level,parsing_class_members);
	    if ( tk2 == LP 
		&& (bl_level == 0 || parsing_class_members) 
		&& ( laexpr(lasttk) == 0 ) 
		&& must_be_expr == 0
		&& DECL_TYPE == 0 ) {
			if (la_decl()) {
				must_be_id = 0;
				DECL_TYPE = tk;
				tk = DECL_MARKER;
				goto ret;
			}
	    }

	    // note: *** can handle local typedefs here, too!
	    if ( in_typedef && 
			in_typedef->base != 0 && 
			ccl && in_mem_fct == 0 &&
			(tk2 == SM || tk2 == RP || tk2 == LB))
				yylval.pn = do_nested_type(yylval.pn);
 	
	    if ( lasttk == AGGR || lasttk == ENUM ) {
		if ( tk2 == LC || tk2 == COLON ) { // class definition	
			if ( in_typedef && 
				in_typedef->base == 0 &&
				class_typedef == 0 )
					class_typedef = yylval.pn->string;
			if ( lex_level 
				&& (in_class_decl==0 || in_mem_fct)
				&& lasttk != ENUM )  // temporary
					yylval.pn = do_local_class( yylval.pn, lex_level );
			else 
			if ( in_class_decl && ccl )
				yylval.pn = do_nested_type( yylval.pn );
		}
	    } 

	    if ( tk2 == MEM || (tk2 == DOT && mem_sel == 0 )) {  
		if (tk2==DOT)
			error(strict_opt?0:'w',"``.'' used for qualification; please use ``::'' (anachronism)");
	    crunch:
		tk = TSCOPE;
		{//XXX -- should be modified to do lookup and del at each ::
		  while ( (tk2 = lookahead()) == TSCOPE ) ;
		  if ( tk2 == TNAME ) {
			tk2 = lookahead();
			if ( tk2 == LP 
			&& (bl_level == 0 || parsing_class_members) 
			&& ( laexpr(lasttk) == 0 ) 
			&& must_be_expr == 0
			&& DECL_TYPE == 0 ) {
				if (la_decl()) {
					must_be_id = 0;
					//DECL_TYPE = tk;//???
					DECL_TYPE = TNAME;
					//front should be ::
					front->tok = TSCOPE;
					front->retval.pn = yylval.pn;
					yylval.pn = 0;
					tk = DECL_MARKER;
					goto ret;
				}
			}
		  }
		}
		tk2 = deltok(1);
		tk2 = la_look();
		if ( tk2 == MUL ) {
			tk = MEMPTR;
			tk2 = deltok(1);
		}
	    }
	    // Have a TNAME.  Check to be sure.
	    else if ( must_be_id ){
//error('d',"lalex: must_be_id: <tname %n> <%k>",yylval.pn,tk2);
		if ( in_class_decl 
			&& lasttk == TYPE 
			&& tk2 == LP
			&& strcmp(yylval.pn->string,ccl->string) == 0 )
			error("%nK with returnT", yylval.pn);

		else if ( lasttk == TYPE && lastval.t == OVERLOAD
			  && ( tk2 == SM || tk2 == LP ) )
		        { 
			tk = ID;
			yylval.pn->hide();
			yylval.pn = new name( yylval.pn->string );
			yylval.pn->n_oper = TNAME;
			}
		else if ( lasttk == OPERATOR ||
			in_typedef && yylval.pn->n_key == NESTED)
				must_be_id = 0;
		else if ( lasttk != TSCOPE // watch out for X::X
		     || lastval.pn != yylval.pn 
		     || (in_typedef && 
				in_typedef->check( yylval.pn->tp,0) == 0 )) 
		{
			tk = ID;
			if ( in_typedef && (lasttk == MUL || lasttk == REF)) {
     				defer_check = 1;
     				in_tag = yylval.pn;
			}

			if ( lasttk == MEM && yylval.pn->lex_level ) {
				Pname nn = gtbl->look( yylval.pn->string, 0 );
				if (nn == 0 )
    					error( "%k%s undeclared", lasttk, yylval.pn->string);
				else
    					yylval.pn = nn;
			}
			else {
// error('d',"lalex: else: lasttk: %k", lasttk );
				if (lasttk!=DOT && lasttk!=REF
				&&  lasttk!=TSCOPE && lasttk != GOTO
				// handle typedefs in basetype::check
				//    when type is available
				&&  !in_typedef) {
// error('d',"\"%s\" line %d: hiding%n",__FILE__,__LINE__,yylval.pn);
                                       yylval.pn->hide();
				}

				yylval.pn = new name (yylval.pn->string);
				if (lasttk!=DOT && lasttk!=REF && 
					lasttk!=TSCOPE && lasttk !=GOTO) 
						yylval.pn->n_oper = TNAME;
			}
			if ( defer_check ) defer_check = 0;
		}
	    } // must_be_id

	    if ( in_class_decl &&
		ccl->lex_level &&
		yylval.pn->lex_level != 0 && 
		yylval.pn->tp && 
		(yylval.pn->tp->base != COBJ && yylval.pn->tp->base != EOBJ)) 
	    {
		Pname n = gtbl->look( yylval.pn->string,0);
		if ( in_mem_fct ) {
		     if (n && n->base == TNAME ) {
                       	   error('w', "local typedef %n(%t) is not in scope of local class %s members; using global (%t)", yylval.pn, yylval.pn->tp, ccl->string, n->tp );
			   yylval.pn = n;
		    } else
                	error( "local typedef %sis not in scope of inline member function of local class %s", yylval.pn->string, ccl->string);
		}
	    }

	    // if we still have a TNAME, make sure have the right TNAME
	    // possibility of ananchronistic reference to nested type
	    Ptype nbt = yylval.pn->tp;
	    if (tk == TNAME && curr_scope == 0 && nbt && // Y y; not X::Y y;
		(nbt->base == EOBJ || nbt->base == COBJ))
	    {
		Ptype t = Pbase(nbt)->b_name->tp;
		if ( ccl && t->in_class &&
			strcmp(t->in_class->string, ccl->string)) 
		{
			switch( nbt->base ) {
				case COBJ:
					if (is_empty(Pclass(t)))
						yylval.pn = check_nested_type( yylval.pn );
					break;
				case EOBJ:
					if (is_empty(Penum(t)))
						yylval.pn = check_nested_type( yylval.pn );
					break;
			};
		}
	    }
        }
	else 
        { // tk == ID 
    		char *s = yylval.s;
		Pname n = ktbl->look( s, HIDDEN );
		Pname nstd = ktbl->look( s, NESTED );

		// inside a class definition, ccl, that is nested 
		// s is a nested class name, and is the name of ccl
		if (ccl && ccl->in_class && nstd && 
			strcmp(s, ccl->string)==0)
		{
			for (Pname nn=nstd; nn; nn=nn->n_tbl_list) {
				Ptype tt = (nn->tp->base==COBJ || nn->tp->base==EOBJ)
						? Pbase(nn->tp)->b_name->tp : nn->tpdef;
				Pclass cl = tt->in_class; 
				if ( nn->n_key != NESTED ) continue;
				if (strcmp(ccl->in_class->string,cl->string) == 0) {
					tk = TNAME;
					yylval.pn = nn;
					n = nstd = nn;
					}
			}
		}

		if (tk2 == MEM) {
	            // ID ::
		    if (n) {
			yylval.pn = n;
		/*XXX*/	goto crunch;
		    } 
		    else
		    if (nstd &&
			nstd->n_tbl_list==0)
		    {
			yylval.pn = nstd;
			tk = TSCOPE;
			tk2 = deltok(1);
			tk2 = la_look();
			if (tk2 == MUL ) {
				tk = MEMPTR;
				tk2 = deltok(1);
			}
		    }
		    else {
			error( "%s:: %sis not aTN", s, s ); 
		    	tk2 = deltok(1);
	    	    	goto gettok;
		    }
		} 
		else  // transitional kludge
		if ( n && nstd && n == nstd )  
			; // null statement

		// Have an ID. Check last token to be sure.
		else if (lasttk==ENUM || lasttk==AGGR) {
			int fd = tk2!=LC && tk2!=COLON;
			if ( in_typedef && fd == 0 &&
				in_typedef->base == 0 &&
				class_typedef == 0 )
					class_typedef = yylval.s;
			tk = TNAME;
			if ( nstd ) { 
				// in transitional model, need flag this as error
				if ( fd == 0 ) { // real definition
					if ( ccl == 0 )
						error("nested and global%k %s(to do this place global%k %s {}; first)",lasttk==ENUM?lasttk:CLASS, s, lasttk==ENUM?lasttk:CLASS, s); 
					else
						error("multiple nested%k %s(to do this place global%k %s {}; first)",lasttk==ENUM?lasttk:CLASS, s, lasttk==ENUM?lasttk:CLASS,s); 
					error( 'i', "cannot recover from previous errors" );
				}
			}
			else
			if (n==0) { // new tag, define it
// error('d', "ccl: %t fd: %d, in_mem_fct: %d, in_class_decl: %d", ccl, fd, in_mem_fct, in_class_decl);
				n = new name( s );
				if ( fd )  // struct X*, etc.
					n->lex_level=0; 
				else
					n->lex_level=lex_level>=0?lex_level:0;

				if ( ccl && fd == 0 && 
					in_class_decl && 
					in_mem_fct == 0 )
						n = do_nested_type( n );
				else
				// note: ***** modify to handle local typedef
				// note: ***** add local enums
				if ( n->lex_level &&
					lasttk != ENUM ) // temporary
						n = do_local_class( n, n->lex_level );
				else {
					n = n->tname( lastval.t );
					modified_tn = modified_tn->l;
					if (fd && gtbl->look(n->string,0)) statStat = n;
				}
			}
			else {
				if (n->tp->base!=COBJ && n->tp->base!=EOBJ) {
					error( 'i', "hidden%n:%t",n,n->tp );
					goto gettok;
				}

				if ( tk2 == LC || tk2 == COLON ) { 
					// class declared and hidden but not yet defined
					// may have ctor defined which invalidates hiding
					statStat = n;
					n->n_key = 0; // inside class definition it cannot be hidden
				}
			}
			yylval.pn = nstd?nstd:n; 
		}
		else {
	    		tk = ID;
			yylval.pn = new name( s );
		}

		if ( tk == ID ) 
		{
		    switch ( tk2 ) {
		    case ID: case TNAME: case AGGR: case ENUM:
		    {
			Pname n = 0; 

			if ((curr_scope||ccl) && nstd) {
			// within class scope in which nested class is visible
			// curr_scope == set by TSCOPE, X::foo() { ... }
			// ccl == parsing class definition ``ccl''

				char *str = curr_scope?curr_scope->string:ccl->string;
				for (Pname nn=nstd; nn; nn=nn->n_tbl_list) {
					Ptype tt = (nn->tp->base==COBJ || nn->tp->base==EOBJ)
							? Pbase(nn->tp)->b_name->tp : nn->tpdef;
					Pclass cl = tt->in_class; 
					if ( nn->n_key != NESTED ) continue;
					if ( strcmp(str,cl->string) == 0){	
						tk = TNAME;
						yylval.pn = nn;
						if (lasttk == TYPE && 
							lastval.t == TYPEDEF )
								in_typedef = yylval.pn->tp;
						break;
					}
				}
			}
			if (tk == TNAME) break; // found nested class

			n = ktbl->look( s, HIDDEN ); 
			if ( n ) {
				Pname nn = n;
				switch ( n->tp->base ) {
					default:
						error("typedef %sis not visible in this scope", s );
						break;
					case COBJ:
						if (is_empty(Pclass(Pbase(n->tp)->b_name->tp)))
 							n = check_nested_type( nn );
						if (nn == n)
							error("%sis hidden: use struct %s%s", s,s,front->retval.s);
						break;
					case EOBJ:
						if (is_empty(Penum(Pbase(n->tp)->b_name->tp)))
 							n = check_nested_type( nn );
						if (nn == n)
							error("%sis hidden: use enum %s%s", s,s,front->retval.s);
						break;
				};
				tk = TNAME;
				yylval.pn = n;
			}
			else  
			if (n=ktbl->look(s,NESTED)) 
			{
				TOK ntk;
				bit ok = 0;
				Ptype tt = return_nstd_local_type(n,ntk);
				Pclass cl = tt->in_class; 
				if (ccl) {
					// x::y unncessary with in_class,
					// a derived class of in_class
					// or classes enclosing in_class
					if (ccl==cl || ccl->has_base(cl))
						ok++;
					else {
					for (Pclass eccl=ccl->in_class;eccl; eccl=eccl->in_class)
						if ( eccl == cl ) { ok++; break; }
					}
				}
				if (!ok)
					error('w', "use %t:: to access nested %k %s (anachronism)", cl, ntk, n->string );
				tk = TNAME;
				yylval.pn = n;
			}
			else { // probably a typo
			    if ( tk2 == ID )
  				error("%s%s: %sis not aTN", s,front->retval.s,s);
			    else if ( tk2 == TNAME )
  				error("%s%s: %sis not aTN", s,front->retval.pn->string,s);
			    else 
  				error("%s%k: %sis not aTN", s,front->retval.t,s);
			    goto gettok;
			}
    			 break;
		    }
		    case DOT: case REF:
    			 mem_sel = yylval.pn;
    			 break;
		    default:
			if ( lasttk == TNAME && tk2 == LC )  
			{
	    			error("T%s %k: %s is unexpected", s, tk2, s );
	    			goto gettok;
			}
			
			// have an ID.  lets just make sure it should not be a TNAME
			if (curr_scope||ccl||nstd) {
				if (ccl && in_typedef && 
					in_typedef->base != 0 &&  
					in_mem_fct == 0 &&
					(tk2 == SM || tk2 == RP || tk2 == LB)) 
					{
						yylval.pn = do_nested_type( yylval.pn );
						tk = TNAME;
					}
				else
				if (nstd && must_be_id == 0 && in_expr == 0) {
// error('d',"nstd: %n must_be_id 0 have id tk2: %k lasttk: %k",nstd,tk2,lasttk);
// error('d',"nstd: in_expr %d lex_level %d",in_expr,lex_level);
					Pclass xcl = curr_scope?Pclass(Pbase(curr_scope->tp)->b_name->tp):(ccl?ccl:0);
					for (Pname nn=nstd; nn; nn=nn->n_tbl_list) {
						TOK ntk;
						bit ok = 0;
						Ptype tt = return_nstd_local_type(nn,ntk);
						Pclass cl = tt->in_class; 
// error('d',"xcl: %t ccl: %t", xcl, ccl );
						if ( xcl ) {
							if (xcl==cl || xcl->has_base(cl) || ccl == cl)
								ok++;
							else {
							for (Pclass eccl=xcl->in_class;eccl;eccl=eccl->in_class)
								if ( eccl == cl ) { ok++; break; }
							}
						}
							
						if (nn == nstd)
						{
						if (
						    // special case: foo(X,
						    // in_arg_list not set until **after** X is handled
						    ((in_arg_list || lasttk==LP) &&  // foo(nestedX
							(tk2==CM || tk2==ASSIGN || 
								(tk2==RP && lasttk!=MUL && lasttk!=REF))) 
			     			    || // class x : public nestedX
			    				(tk2==LC && (lasttk==PR || lasttk==VIRTUAL))
						    || // nestedX [*&]
							(tk2 == MUL || tk2==AND)
						    || (lasttk==LP && tk2==RP)
						    || (lasttk==TSCOPE && lastval.pn == nn) 
						    || (lasttk==COMPL && dtor_seen == nn) 
						    || (lasttk==TYPE && lastval.t == TYPEDEF)
                                                    || lasttk == OPERATOR
						    || lasttk == NEW || in_sizeof )
						{  // must be type name, and it must be nested: 
    							if ( nstd->n_tbl_list == 0 ) { // only one: ok
								if (lasttk != TSCOPE && !ok )
									error('w', "use %t:: to access nested %k %s (anachronism)", cl, ntk, nn->string);
								break;
    							}
    							else {
								if (lasttk != TSCOPE && !ok){
									error("ambiguous nested type %s, use X::%s",nn->string,nn->string);
									error( 'i', "cannot recover from previous errors" );
								}
							}
						}

					    }
						if ( nn->n_key != NESTED ) continue;
						if (xcl && 
							strcmp(xcl->string,cl->string) == 0) break;
					} // end: for nn = nstd

					if (nn) {
						tk = TNAME;
						yylval.pn = nn;
						if (lasttk == TYPE && 
							lastval.t == TYPEDEF )
								in_typedef = yylval.pn->tp;
					}
				}  // end: if (nstd)
			} // end: if (curr_scope||ccl)
    			 break;
		    } // end: switch tk2
    		} // end: if (tk == ID)
	    }

// error('d',"testing for in_expr: in_expr: %d tk: %k", in_expr, tk );
// error('d',"testing for in_expr: tk2: %k lasttk: %k", tk2, lasttk );
	    if (lex_level && tk==ID && tk2==LP &&
			(lasttk==LC || lasttk==RC || lasttk==RP ||
			lasttk == ASSIGN || lasttk == SM))
				in_expr = 1;
	    else in_expr = 0;
	
	}
        if ( tk == TNAME || ( tk == TYPE && latype(yylval.t) )
// XXX     || tk == TSCOPE || tk == MEM 
	   || tk == REF || tk == DOT || tk == GOTO 
	   || tk == MEMPTR )
	   // TNAME cannot immediately follow a type name,
	   // scope operator, right curly, selection, or goto
		must_be_id = 1;
    	else
		must_be_id = 0;

        switch ( tk ) {
	    case SM:
		mem_sel = 0; // no break
		in_expr = 0;
	    case RP: case RC: must_be_expr = 0; break;
	    case COLON: 
		if  (lasttk == RP || 
		    (lasttk == TYPE && lastval.t == CONST)) 
 		        must_be_expr = 1;
		break;
	    case SIZEOF:
		in_sizeof = 1;
		break;
	};
ret:
    if ( tk == COMPL && lasttk == TSCOPE )
	 dtor_seen = lastval.pn;
    else dtor_seen = 0;
    lasttk = tk;
    lastval = yylval;
    LDB(1,showQ("after");
	fprintf(stderr,"returning '%s'",image(tk));
	if ( tk==ID || tk==TNAME ) fprintf(stderr," -- '%s'",yylval.pn->string);
	fprintf(stderr,"\n");
    );
// error('d',"returning tk: %k dtor_seen: %n", tk,dtor_seen );
    return tk;
}

extern void
la_backup( TOK t, YYSTYPE r )
/*
    called by parser to push token back on front of queue
*/
{
    LDB(1,fprintf(stderr,"\n*** la_backup( '%s', ...)\n",image(t)));
    if ( t == ID ) { Pname n = r.pn; r.s = n->string; DEL(n); }
    register toknode* T = new toknode(t,r,curloc);
    if (front) {
	front->last = T;
	T->next = front;
	T->last = 0;
	front = T;
    } else
	front = rear = T;
    lasttk = 0;
}

extern int
la_sctype( TOK t )
{
//error('d',&latok->place,"la_sctype(%k ) -- latok ==%k",t,latok->tok);
	if ( t != latok->tok && t != TSCOPE && t != MEMPTR )
		error( 'i', &latok->place, "la_sctype, lalex.c" );

	switch( latok->retval.t ) {
		case TYPEDEF:
		case EXTERN:
		case STATIC:
		case AUTO:
		case REGISTER:
		case OVERLOAD:
		case INLINE:
		case FRIEND:
		case CONST:
		case VOLATILE:
			return 1; 
		default:
			return 0; 
	}
}

extern int
la_cast()
/*
	called in reduction of term_lp to check for ambiguous prefix-style cast
	if result is 1, caller inserts DECL_MARKER to force reduction of cast
*/
{
	// yychar already designates TYPE or TNAME
	// LP must start the lookahead queue!
	LDB(1,fprintf(stderr,"\n*** la_cast()\n"););
	int tk, tk2 = latok->tok;

	for ( ; ; ) {
	    tk = tk2;
	    tk2 = lookahead();

	    switch( tk ) {
	    case LP:
		if ( tk2 == MUL || tk2 == AND ||
     			tk2 == TSCOPE || tk2 == MEMPTR )
		    // T ( * ...
		    // T ( C ::* ...
		    continue;
		else
		    // T ( exp )
		    return 0;
	    case MUL: case AND:
		//if ( tk2 == SCTYPE )
		if ( la_sctype( tk2 ) )
		    // T ( * const ...
		    // T ( * volatile ...
		    tk2 = lookahead();
		continue;
	    case TSCOPE:
		if ( tk2 == MUL )
		    // T ( C :: * ...
		    continue;
		else
		    // T ( exp )
		    return 0;
	    case RP: case LB:
		// T (*)()
		// T (*[])()
		return 1;
	    case MEMPTR:
		if ( tk2 == RP )
    			continue;
	    }

	    return 0;
	}
}

static int
la_decl()
/*
	handles ambiguities
		type (*x) ()
		type (*x) []
	at start of arg list / statement
	return val == 1 if lookahead finds a declaration
		(used for error messages only)
	if declaration is "ambiguous" (i.e., can't be recognized with
		1-symbol lookahead), insert DECL_MARKER to force reduction
		of "type"
*/
{
	
	// LP must start the lookahead queue!
	LDB(1,fprintf(stderr,"\n*** la_decl()\n"););
	int tk, tk2 = latok->tok;
	int paren = 0;
	int ptr = 0;

	for ( ; ; ) {

	    tk = tk2;
	    tk2 = lookahead();

// fprintf(stderr,"\nla_decl:tk:%d %s tk2: %d %s", tk, keys[tk], tk2, keys[tk2]);
	    switch( tk ) {
	    case LP:
		if ( tk2 == MUL || tk2 == AND || tk2 == TSCOPE ) {
		    // T ( * ...
		    ++paren;
		    ptr = 1;
		    continue;
		} else
		if ( tk2 == MEMPTR ) {
		// T ( C ::*  ...
			return 1;
		} else 
		// possible redundant parens
		if ( tk2 == ID && lookahead() == RP ) {
			TOK tp = lookahead();
// error( 'd', "tp %k tk: %k tk2: %k", tp, tk, tk2 );
// error( 'd', "bl_level: %d, in_class_decl: %d", bl_level,in_class_decl );
			if ( tp == SM || tp == CM || tp == ASSIGN )
			{
				// member initialization list
				if ( tp != SM && in_arg_list == 0 ) return 1;
			}
			else 
			if ( tp == RP && (bl_level-in_class_decl==0))
				return 1;
			if ( tp != LP ) 
				return 0;
			latok=latok->last; // restore lookahead
			++paren;
			continue;
			}
		else
		    // T ( exp )
		    return 0;
	    case MUL: case AND:
		//if ( tk2 == SCTYPE )
		if ( la_sctype( tk2 ))
		    // T ( * const ...
		    // T ( * volatile ...
		    return 1;
		else {
		    ptr = 0;
		    continue;
		}
	    case MEMPTR:
		// T ( C :: * ...
		return 1;
	    case TSCOPE:
		if ( tk2 == MUL ) //??tk SHOULD HAVE TRANSLATED TO MEMPTR!!
		    // T ( C :: * ...
		    return 1;
		else if ( ptr )
		    // T ( exp )
		    return 0;
		else if ( tk2 == ID || tk2 == OPERATOR )
		    // T ( * C :: id ...
		    continue;
		else
		    // error
		    return 0;
	    }

	    break;
	}

	if ( tk == RP || tk == LB )
	    // T (*)()
	    // T (*[])()
	    return 1;

	if ( tk != ID && tk != OPERATOR )
	    // T ( exp )
	    return 0;

	if ( tk == OPERATOR )
	    switch ( tk2 ) {
	    case PLUS: case MINUS: case MUL: case REFMUL:
            case AND: case OR: case ER: case SHIFTOP: case EQUOP: 
	    case DIVOP: case RELOP: case ANDAND: case OROR: 
	    case NOT: case COMPL: case ICOP: case ASSIGN: 
            case ASOP: case NEW: case GNEW: case DELETE:
		// OPERATOR oper
		tk2 = lookahead();
		break;
	    case LP:
		// OPERATOR ()
		tk2 = lookahead();
		if ( tk2 == RP ) {
		    tk2 = lookahead();
		    break;
		} else
		    return 0;
	    case LB:
		// OPERATOR []
		tk2 = lookahead();
		if ( tk2 == LB ) {
		    tk2 = lookahead();
		    break;
		} else
		    return 0;
	    default:	// illegal operator
		return 0;
	    }

	int allow_lp = 1;
	int allow_rp = 1;
	for ( ; ; ) {
	    tk = tk2;
	    tk2 = lookahead();

// fprintf(stderr,"\nla_decl2:tk:%d %s tk2: %d %s", tk, keys[tk], tk2, keys[tk2]);
	    switch( tk ) {
	    case LP:
		if ( !allow_lp )
		    // T ( * id [ exp ] ( ...
		    return 0;

		// Current lookahead will be a decl if
		// the next lookahead is an arg decl
		if ( tk2 == RP || tk2 == ENUM || tk2==AGGR 
 			|| tk2==ELLIPSIS || la_sctype( tk2 )) 
		    // T ( * id ()
		    // T ( * id ) ()
		    return 1;

		if ( tk2 == TYPE || tk2 == TNAME ) {
		    // T ( * id ) ( T2 ...
		    if ( lookahead() == LP && !la_decl() )
			return 0;
		    return 1;
		}

		return 0;
	    case LB:
                if ( tk2 == RB || lookahead() == RB )
		    // T ( * id [] ...
		    return 1;
		else {
		    // T ( * id [ exp ] ...
		    allow_lp = 0;
		    allow_rp = 1;
		    while ( lookahead() != RB );
		    tk2 = lookahead();
		    continue;
		}
	    case RP:
// error ('d', "rp: allow_rp: %d paren: %d", allow_rp, paren );
		if ( !allow_rp || !paren )
		    // T ( * id ) )
		    return 0;
// permit redundant parentheses
		else 
		if ( tk2 == SM || tk2 == CM || tk2 == ASSIGN )
			return 1;
		else
		if ( tk2 == RP && (bl_level-in_class_decl == 0))
			return 1;
		else
		{
		    // T ( * id ) ...
		    allow_lp = 1;
		    allow_rp = 0;
		    --paren;
		    continue;
		}
	    default:
		return 0;
	    }
	}
}



/*
**	PROCESSING OF INLINE MEMBER FUNCTIONS
*/
static int la_snarf();

extern toknode*
save_text()
/*
	save text of inline def on q of class
*/
{
	// Q should contain at least the tokens < FDEF, X ... >
	// where X is either LC or COLON (start of ftn)
	LDB(2,fprintf(stderr,"save_text()"));
	LDB(3,fprintf(stderr,"front: %s",image(front->tok)));
	LDB(3,fprintf(stderr,"front->next: %s",image(front->next->tok)));
	latok = front->next;
	if ( la_snarf() ) {
		// append this set of tokens to
		// inline tokenq for class
		toknode* t = front; // FDEF
		if ( ccl->c_funqf == 0 )
			ccl->c_funqf = front;
		else {
			ccl->c_funqr->next = front;
			front->last = ccl->c_funqr;
		}
		ccl->c_funqr = latok;
		front = latok->next;
		latok->next = 0;
		if (front)  front->last = 0;
		return t;
	}
	return 0;
}


extern void
restore_text()
/*
	restore tokens for member inlines onto token q
*/
{
	LDB(2,fprintf(stderr,"restore_text()"));
	if (ccl->c_funqf == 0)	// no inlines on Q
		return;
	LDB(3,fprintf(stderr,"	Q present: %d,%d",ccl->c_funqf,ccl->c_funqr));
	LDB(3,fprintf(stderr,"	front==%s",image(ccl->c_funqf->tok)));
	LDB(3,fprintf(stderr,"	rear ==%s",image(ccl->c_funqr->tok)));
	ccl->c_funqr->next = front;
	if (front)  front->last = ccl->c_funqr;
	front = ccl->c_funqf;
	ccl->c_funqf = ccl->c_funqr = 0;
}


static void
del_tokens( toknode* marker )
/*
    delete tokens from marker to latok, not inclusive
*/
{
    if ( marker == 0 || marker == latok || marker->next == 0 )
	error('i', "bad token queue");

    LDB(2,fprintf(stderr,"del_tokens: %s..%s\n",image(marker->tok),image(latok->tok)));

    register toknode* tt = marker->next;
    if ( tt == latok ) return;
    marker->next = latok;
    latok->last->next = 0;
    latok->last = marker;
    register toknode* tx = tt;
    do {
	LDB(3,fprintf(stderr,"	deleting %s\n",image(tt->tok)));
	tx = tx->next;
	delete tt;
	tt = tx;
    } while ( tx );
}


static int
la_snarf()
/*
	scan function def without processing declarations
*/
{
	LDB(2,fprintf(stderr,"la_snarf()"));
	loc *L = &latok->place;
	//DBPLACE(1,L.l,L.f);
	int parens = 0;
	int paren_error = 0;
	toknode* marker = latok;
	switch ( latok->tok ) {
	default:
		error('i', L, "bad token Q snarfing function: %d", latok->tok);
	case COLON:
		break;
	case LC:
		--bl_level;
		goto eatf;
	}
	LDB(2,fprintf(stderr,"\"eat\" member initializers"));
	for (;;) {
		if (latok->next == 0) add_tokens();
		switch ( (latok=latok->next)->tok ) {
		case LP:
			++parens;
		default:
			LDB(3,fprintf(stderr,"...%s",image(latok->tok)));
			continue;
		case RP:
			if ( (--parens < 0) && (paren_error++ == 0) )
				error(0,&latok->place,"unbalanced ()");
			continue;
		case LC:
		case RC:
			if ( parens <= 0 )
				goto eatf;
			continue;
		case SM:
			if ( parens <= 0 ) {
				error(0, L, "illegal bit field");
				del_tokens( front );
				delete front;
				front = latok;
				front->last = 0;
				return 0;
			}
			continue;
		case EOFTOK:
			error('i',&latok->place,"unexpected end of file");
		} // switch
	} // for

	eatf:
	int level = 1;
	for (;;) {
		if (latok->next == 0) add_tokens();
		switch ( (latok=latok->next)->tok ) {
		case LC:
			++level;
		default:
			LDB(3,fprintf(stderr,"...%s",image(latok->tok)));
			continue;
		case RC:
			LDB(3,fprintf(stderr,"...RC"));
			if (--level <= 0) {
				if (level < 0) {
					error(0,&latok->place,"unexpected '}'");
					goto bad;
				}
				return 1;
			}
			break;
		case EOFTOK:
			error('e', &latok->place, "unbalanced {}");
			goto bad;
		} // switch
	} // for
	bad:
		del_tokens( marker );
		marker->tok = SM;
		return 0;
}

Pname check_for_nested( Pname nstd, TOK lasttk, YYSTYPE lastval, TOK tk2)
{
// error('d',"nstd: %n must_be_id 0 tk2: %k lasttk: %d",nstd,tk2,lasttk);
	TOK tk = ID;
	Pclass xcl = curr_scope?Pclass(Pbase(curr_scope->tp)->b_name->tp):(ccl?ccl:0);

	for (Pname nn=nstd; nn; nn=nn->n_tbl_list) {
		TOK ntk;
		bit ok = 0;
		Ptype tt = return_nstd_local_type(nn,ntk);
		Pclass cl = tt->in_class; 
		if ( xcl ) {
			if (xcl==cl || xcl->has_base(cl))
				ok++;
			else {
				for (Pclass eccl=xcl->in_class;eccl;eccl=eccl->in_class)
				if ( eccl == cl ) { ok++; break; }
			}
		}
							
		if (nn == nstd)
		{
			if ( ((in_arg_list || lasttk==LP) &&  // foo(nestedX
				(tk2==CM || tk2==ASSIGN || tk2==RP)) 
			    || (tk2==LC && (lasttk==PR || lasttk==VIRTUAL))
			    || (tk2 == MUL || tk2==AND)
			    || (lasttk==LP && tk2==RP)
			    || (lasttk==TSCOPE && lastval.pn == nn) 
			    || (lasttk==COMPL && dtor_seen == nn) 
			    || (lasttk==TYPE && lastval.t == TYPEDEF)
			    || lasttk == NEW || in_sizeof )
			{  
				if ( nstd->n_tbl_list == 0 ) { // only one: ok
					break;
    				} else {
					if (lasttk != TSCOPE && !ok){
						error("ambiguous nested type %s, use X::%s",nn->string,nn->string);
						error( 'i', "cannot recover from previous errors" );
					}
				}
			}
	        }
		if ( nn->n_key != NESTED ) continue;
		if (xcl && strcmp(xcl->string,cl->string) == 0) break;
	} // end: for nn = nstd
	return nn;
}
