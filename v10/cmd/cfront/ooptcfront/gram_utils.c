/* utilities used by the grammar. We never change them, so why
   compile them constantly ? */

#include "cfront.h"

/* macros filtched from gram.skeleton */

#define yylex lalex
extern TOK	la_look();
extern void	la_backup( TOK, YYSTYPE );
extern int	la_cast();
extern int	la_decl();
extern TOK	lalex();
extern int yychar, must_be_id, DECL_TYPE, declTag;
FILE * yydebugfile;

char * gram_print_node (int tok, YYSTYPE* val)
{
    static char buffer[1000];

    switch(tok) {
      case ID:
      case TNAME:
	Pname n = val->pn;
	return n->string;
      case STRING:
	return val->s;
      default:
	sprintf(buffer, "0x%x", val->p);
	return buffer;
    }
}

#define NEXTTOK() ( (yychar==-1) ? (yychar=yylex(),yychar) : yychar )
#define EXPECT_ID() must_be_id = 1
#define NOT_EXPECT_ID() must_be_id = 0
    
Pptr doptr(TOK p, TOK t)
{
	Pptr r = new ptr(p,0);
	switch (t) {
	case CONST:
		r->rdo = 1;
               // if (p == RPTR) error('w',"redundant `const' after &");
		break;
	case VOLATILE:
		error('w',"\"volatile\" not implemented (ignored)");
		break;
	default:
		error("syntax error: *%k",t);
	}
	return r;
}

Pbcl dobase(TOK pr, Pname n, TOK v = 0)
{
	Pbcl b = new basecl(0,0);

	if (pr == PROTECTED) {
		pr = PUBLIC;
		error("protectedBC");
	}
	b->ppp = pr;	// save protection indicator

	if (n) {
		if (n->base != TNAME) {
			error("BN%n not aTN",n);
			return 0;
		}

		Pbase bt = Pbase(n->tp);
		while (bt->base == TYPE) bt = Pbase(bt->b_name->tp);

		if (bt->base != COBJ) {
			error("BN%n not aCN",n);
			return 0;
		}

		if (v) {
			if (v != VIRTUAL) error("syntax error:%k inBCD",v);
			b->base = VIRTUAL;
		}
		else
			b->base = NAME;

		b->bclass = Pclass(bt->b_name->tp);
	}

	return b;
}

extern int yyparse ();
extern YYSTYPE yylval, yyval;

Pname syn()
{
ll:
	switch (yyparse()) {
	case 0:		return 0;	// EOF
	case 1:		goto ll;	// no action needed
	default:	return yyval.pn;
	}
}

void
check_cast()
/*
	Lookahead to direct parsing of cast
	la_cast() returns 1 if lookahead sees an ambiguous old-style C cast.
*/
{
	switch( NEXTTOK() ) {
	case TYPE: case TNAME:
	    if ( la_look() == LP && la_cast() ) {
		must_be_id = 0;
		yychar = DECL_MARKER;
	    }
	}
}

void
check_decl()
/*
	Lookahead to direct parsing of local/arg type declarations
	la_decl() returns 1 if lookahead sees a declaration.
*/
{

	switch( NEXTTOK() ) {
	case TYPE: case TNAME:
	    if ( la_look() == LP && la_decl() ) {
		must_be_id = 0;
		DECL_TYPE=yychar;
		yychar = DECL_MARKER;
	    }
	}
}

void
check_tag()
/*
        Allow the case of inline/virtual/overload as 
        modifiers of return type of form struct/class/union x foo() 
        SM, COLON, LC ==> real class declaration, not return type
*/
{
	switch ( NEXTTOK() ) {
	case SM: case COLON: case LC:
    		declTag = 1;
    		break;
	default:
    		declTag = 0;
    		break;
        }
}

Pname find_fct_dcl (Pname fname)
{
    Pname function_name;

    if(!(fname && fname->base == NAME && fname->tp && fname->tp->base == FCT))
	return 0;
    /* In an ideal world, we would defer this to something in semantics.
       oh well, here we go. */

    if (fname->n_qualifier) {	
	Pname cn = fname->n_qualifier;

	switch (cn->base) {
	  case TNAME:
	    break;
	  case NAME:
	    cn = gtbl->look(cn->string,0);
	    if (cn && cn->base==TNAME) break;
	  default:
	    error("badQr%n for%n",fname->n_qualifier,fname);
	    return 0;
	}

	if (cn->tp->base != COBJ) {
	    return 0;
	}

	cn = Pbase(cn->tp)->b_name;

	Pclass cl = Pclass(cn->tp);

	if ((cl->defined&(DEFINED|SIMPLIFIED)) == 0) {
	    error("C%nU",cn);
	    return 0;
	}

	Ptable etbl = cl->memtbl;
	Pname x = etbl->look(fname->string,0);
	if (x==0 || x->n_table!=etbl) {
	    error("%n is not aM of%n",fname,cn);
	    return 0;
	}

	if(x->base != NAME || x->tp == 0) {
	    return 0;
	}

	function_name = x;
    } else {
	function_name = gtbl->look (fname->string, 0);
	if(!function_name) {
	  err::print ("No function %s.", err::name_string (fname));
	    return 0;
	}
    }

    switch(function_name->tp->base) {
      case FCT:
	break;
      case OVERLOAD:
	function_name = Pgen(function_name->tp)->find(Pfct(fname->tp),0);
	if(!function_name) {
	  err::print ("Pragma specifies undefined function.");
	    return 0;
	}
    }

    return function_name;
}


void mark_abnormal_query_function (Pname fname)
{
    Pname function_name =  find_fct_dcl(fname);
    if(!function_name) {
      err::print("#pragma ObjectStore query_abnormal_operator must be followed by a function declaration.");
    }
    else Pfct(function_name->tp)->f_query_abnormal = 1;
}

void declare_query_zvec_compare_function (Pname fn)
{
    Pname function_name = find_fct_dcl(fn);
    int found = 0;
    if(!function_name)
      err::print("#pragma ObjectStore query_zvec_compare_function must be followed by a function declaration.");
    else Pfct(function_name->tp)->f_query_compare = 1;
}

void declare_query_zvec_copy_function (Pname fn)
{
    Pname function_name = find_fct_dcl(fn);
    int found = 0;
    if(!function_name)
      err::print("#pragma ObjectStore query_zvec_copy_function must be followed by a function declaration.");
    else Pfct(function_name->tp)->f_query_copy = 1;
}

void register_zvec_type (Ptype t);

void declare_query_zvec_type (Pname tn)
{
    register_zvec_type (tn->tp);
}
