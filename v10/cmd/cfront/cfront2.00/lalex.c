/*ident	"@(#)ctrans:src/lalex.c	1.6.3.21" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

lalex.c:

	lookahead 

*****************************************************************************/
#include <stdio.h>
#include "cfront.h"
#include "yystype.h"
#include "tqueue.h"

// external functions
extern void la_backup( TOK, YYSTYPE );
extern int  la_decl();
extern int  la_cast();
extern TOK  lalex();
extern TOK  la_look();

static int  laexpr( TOK );
static int  latype( TOK );

/* make this a toknode! */
static int lasttk = 0;		// one token history
static YYSTYPE lastval; 	// yylval lasttk value 

int must_be_expr = 0;		// handle redundant parentheses
int must_be_id = 0;		// !0, TNAME => ID, i.e., int X

extern int in_mem_fct;
extern int in_class_decl;	
extern int in_arg_list;
extern int DECL_TYPE;

toknode* latok;			// current lookahead token
toknode* front = 0;
toknode* rear  = 0;

static toknode* free_toks = 0;
const  TQCHUNK = 16;

toknode::toknode(TOK t, YYSTYPE r) 
{
	if (free_toks == 0) {
//		extern char* calloc(unsigned,unsigned);
		register toknode* q;
		int sz = sizeof( toknode );
		free_toks = q = (toknode*)calloc( TQCHUNK, sz );
		for (; q != &free_toks[TQCHUNK-1]; q->next = q+1, ++q);
	}
	this = free_toks; free_toks = free_toks->next;
	tok = t;
	retval = r;
	next = last = 0;
}

toknode::~toknode()
{
	next = free_toks;
	free_toks = this;
	this = 0;
}

void addtok(TOK t, YYSTYPE r)
{
	toknode* T = new toknode(t,r);
	if (front == 0)
		front = rear = T;
	else {
		rear->next = T;
		T->last = rear;
		rear = T;
	}
}

extern TOK
deltok( int noset = 0 )
{
	register toknode* T = front;
	register TOK tk = T->tok;
	if ( !noset ) yylval = T->retval;
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
    if ( front == 0 )
	add_tokens();

    latok = front;
    return latok->tok;
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
    tk = latok->tok;
    if ( tk == ID || tk == TNAME ) 
    {
	if (latok->next == 0) add_tokens();
	TOK tk2 = latok->next->tok;
	if ( tk == TNAME ) {
	    if ( tk2 == MEM || tk2 == DOT ) {
		tk = TSCOPE;
		latok = latok->next;
		if (latok->next == 0) add_tokens();
		tk2 = latok->next->tok;
		if ( tk2 == MUL )
			{
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
	    	latok = latok->next->next;
	    	goto nexttok;
		}

	if ( tk == ID &&
   		( tk2 == ID ||
		( prev_tk == ID && ( tk2 == COLON || tk2 == LC )))) {
	    		// ID ID
	    		latok = latok->next;
	    		goto nexttok;
			}
    }

    return tk;
}

extern Ptype in_typedef;
extern Pname statStat;
static Pname mem_sel = 0;

extern TOK
lalex()
/*  return next token to grammar  */
{
    register TOK tk;
    if ( front == 0 )
	add_tokens();		// extend lookahead queue

gettok:
    tk = deltok();

    if ( tk == ID || tk == TNAME ) 
    {
	TOK tk2 = la_look();
	extern int bl_level;
	int lex_level = bl_level - in_class_decl - (tk2 == LC );

	if ( tk == TNAME )
	{
// error('d', "lalex %n lasttk: %k tk2: %k must_be_id: %d", yylval.pn, lasttk, tk2, must_be_id );
	    if ( tk2 == LP 
		&& (bl_level == 0 || in_class_decl ) 
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

	    if ( lasttk == AGGR || lasttk == ENUM ) {
		if ( in_typedef == 0 ) {
    			TOK type_tk = yylval.pn->tp->base==COBJ
       				? AGGR 
       				: (yylval.pn->tp->base==EOBJ?ENUM:TYPEDEF);
			if ( lasttk != type_tk )
         			error( "%nredefined: %k and %k", yylval.pn, type_tk, lasttk );
		}


		if ( lex_level && (tk2 == LC || tk2 == COLON) 
			// temporary
			&& lasttk != ENUM ) {
			Pname bn = Pbase(yylval.pn->tp)->b_name;
			Pclass cl = Pclass(bn->tp);
			if ( yylval.pn->lex_level != lex_level || cl->lcl ) {
				extern void local_hide( Pname ); // place in cfront.h
				extern Plist local_blk; // place in cfront.h
				local_hide( yylval.pn );
				Pname n = new name( yylval.pn->string );
				n->lex_level = lex_level>=0?lex_level:0;
				n = n->tname( lastval.t );
				modified_tn = modified_tn->l;
				n->n_key = LOCAL;
				local_class = new name_list( n, local_class );
				local_blk = new name_list( n, local_blk );
				yylval.pn = n;
			}
// catch redefinition of local classes
 			else if ( lex_level == yylval.pn->lex_level && cl->defined )
 				error( "localC%nredefined", yylval.pn );
		}
	    } // end: if (lasttk == AGGR or ENUM )

	    if ( tk2 == MEM || (tk2 == DOT && mem_sel == 0 )) {  
		if (tk2==DOT)
			error('w',"``.'' used for qualification, please use ``::''");
		tk = TSCOPE;
		tk2 = deltok(1);
		tk2 = la_look();
		if ( tk2 == MUL ) {
			tk = MEMPTR;
			tk2 = deltok(1);
			}
	        }
	    
	    // Have a TNAME.  Check to be sure.
	    else if ( must_be_id ){

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
		else if ( lasttk == OPERATOR )
    			must_be_id = 0;
		else
		// watch out for X::X
		if ( lasttk != TSCOPE 
		     || lastval.pn != yylval.pn 
		     || ( in_typedef && in_typedef->check( yylval.pn->tp,0) == 0 )) 
		{
			tk = ID;
			extern int defer_check;
			extern Pname in_tag;
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
				if (lasttk!=DOT && lasttk!=REF && lasttk!=TSCOPE && lasttk !=GOTO) yylval.pn->hide();
				yylval.pn = new name (yylval.pn->string);
				if (lasttk!=DOT && lasttk!=REF && lasttk!=TSCOPE && lasttk !=GOTO) yylval.pn->n_oper = TNAME;
			}
			if ( defer_check ) defer_check = 0;
                }
	} // must_be_id

        if ((lasttk == SM || lasttk == RC || lasttk == LC) 
           && tk2 == COLON ) { // label
		tk = ID;
		yylval.pn = new name (yylval.pn->string);
        }

        if ( in_class_decl &&
                ccl->lex_level &&
                yylval.pn->lex_level != 0 && 
                yylval.pn->tp && 
                (yylval.pn->tp->base != COBJ && yylval.pn->tp->base != EOBJ)) 
		{
			Pname n = gtbl->look( yylval.pn->string,0);
// error( 'd', "bl_level: %d in_mem_fct: %d", bl_level, in_mem_fct );
			if ( in_mem_fct ) {
			     if (n && n->base == TNAME ) {
                        	   error('w', "local typedef %n(%t) is not in scope of local class %s members; using global (%t)", yylval.pn, yylval.pn->tp, ccl->string, n->tp );
				   yylval.pn = n;
			   	    }
			else
                        	error( "local typedef %sis not in scope of inline member function of local class %s", yylval.pn->string, ccl->string);
			}
		}
        }
	else 
        { // tk == ID 
    		char *s = yylval.s;
		Pname n = ktbl->look( s, HIDDEN );
//error( 'd', "n: %n, beenhid: %n, in_arg_list: %d", n, beenHid,in_ arg_list );
		if (tk2 == MEM) {
	            // ID ::
		    if (n) {
			tk = TSCOPE;
			tk2 = deltok(1);
			yylval.pn = n;
			tk2 = la_look();
			if ( tk2 == MUL ) {
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
		// Have an ID. Check last token to be sure.
		else if (lasttk==ENUM || lasttk==AGGR) {
			tk = TNAME;
			if (n==0) { // new tag, define it
				n = new name( s );
				n->lex_level = lex_level>=0?lex_level:0;
				n = n->tname( lastval.t );
				modified_tn = modified_tn->l;
				if (lex_level == 0 ) {
     					if (gtbl->look(n->string,0))  
          					// don't know yet if n has ctor(): defer
          					statStat = n;
				}
				else if ( lex_level > 0 
						// temporary
						&& lasttk != ENUM ) { 
					n->n_key = LOCAL; 
					local_class = new name_list( n, local_class );
					extern Plist local_blk; // place in cfront.h
					local_blk = new name_list( n, local_blk );
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
			yylval.pn = n; 
		}
		else {
	    		tk = ID;
			yylval.pn = new name( s );
		}

		if ( tk == ID ) 
		{
		    if ( tk2 == ID ) {
			Pname n = ktbl->look( s, HIDDEN );
			if ( n ) {
// add handling of enum
				if ( n->tp->base == COBJ )
					error("%sis hidden: use struct %s%s", s,s,front->retval.s);
				else if ( n->tp->base == EOBJ )
					error("%sis hidden: use enum %s%s", s,s,front->retval.s);
				else error("typedef %sis not visible in this scope", s );
			}
			else
  				error("%s%s: %sis not aTN", s,front->retval.s,s);
		    }
		    else
		    if ( tk2 == DOT || tk2 == REF )
    			 mem_sel = yylval.pn;
		    else
		    if ( lasttk == TNAME && tk2 == LC )  
		    {
	    		error("T%s %k: %s is unexpected", s, tk2, s );
	    		goto gettok;
		    }
    		}

	    }
	}
        if ( tk == TNAME || ( tk == TYPE && latype(yylval.t) )
       	   || tk == TSCOPE || tk == MEM 
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
	    case RP: case RC: must_be_expr = 0; break;
	    case COLON: 
//if (lasttk == TYPE )
//error( 'd', "lasttk: %k, lastval.t: %k", lasttk, lastval.t);
		if  (lasttk == RP || 
		    (lasttk == TYPE && lastval.t == CONST)) 
 		        must_be_expr = 1;
		break;
	};
ret:
    lasttk = tk;
    lastval = yylval;
    return tk;
}

extern void
la_backup( TOK t, YYSTYPE r )
/*
    called by parser to push token back on front of queue
*/
{
    if ( t == ID )  r.s = r.pn->string;
    register toknode* T = new toknode(t,r);
    if (front) {
	front->last = T;
	T->next = front;
	T->last = 0;
	front = T;
    } else
	front = rear = T;
}

extern int
la_sctype( TOK t ) {

	if ( t != latok->tok && t != TSCOPE ) error( 'i', "la_sctype, lalex.c" );

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

extern int
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
	    case TSCOPE:
		if ( tk2 == MUL )
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
