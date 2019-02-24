/*ident	"@(#)ctrans:src/gram.y	1.12" */
/*************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All Rights Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

gram.y:
	
	This is the C++ syntax analyser.

	Syntax extensions for error handling:
		nested functions
		any expression can be empty
		any expression can be a constant_expression

	A call to error() does not change the parser's state

***************************************************************************/

%{
#include "cfront.h"
#include "size.h"
#include "template.h"
#include <string.h>
// include tqueue.h after YYSTYPE is defined ...


struct parstate {
	Ptype intypedef;
	int   defercheck;
	Pname intag;
} pstate[BLMAX];
static int px;
static void
SAVE_STATE()
{
//error('d',"save_state: in_typedef%t in_tag%n defer_check %d",in_typedef,in_tag,defer_check);
	if ( px++ >= BLMAX ) error('i',"maximum scope depth exceeded");
	pstate[px].intypedef = in_typedef;
	in_typedef = 0;
	pstate[px].defercheck = defer_check;
	defer_check = 0;
	pstate[px].intag = in_tag;
	in_tag = 0;
}
static void
RESTORE_STATE()
{
//error('d',"restore_state: in_typedef%t in_tag%n defer_check %d",in_typedef,in_tag,defer_check);
	if ( --px < 0 ) error('i',"scope stack underflow");
	in_typedef = pstate[px].intypedef;
	defer_check = pstate[px].defercheck;
	in_tag = pstate[px].intag;
//error('d',"            -> in_typedef%t in_tag%n defer_check %d",in_typedef,in_tag,defer_check);
}

#define copy_if_need_be(s)  ((templp->in_progress || templp->parameters_in_progress) ? strdup(s) : s) 
#define YYMAXDEPTH 600

#ifdef DBG
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#endif

static init_seen = 0;
static cdi = 0;
static Pnlist cd = 0, cd_vec[BLMAX];
static char stmt_seen = 0, stmt_vec[BLMAX];

//local class
static Plist tn_vec[BLMAX], lcl_tn_vec[BLMAX], lcl_blk_vec[BLMAX];
extern void local_restore();
extern void local_name();

//nested class
static Plist nested_tn_vec[BLMAX], nested_type_vec[BLMAX];
extern void nested_restore();

static Pname err_name = 0;

// fcts put into norm2.c just to get them out of gram.y
void sig_name(Pname);	
Ptype tok_to_type(TOK);
void memptrdcl(Pname, Pname, Ptype, Pname);

static Pptr doptr(TOK p, TOK t)
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

static Pbcl dobase(TOK pr, Pname n, TOK v = 0)
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


#define Ndata(a,b)	b->normalize(Pbase(a),0,0)
#define Ncast(a,b)	b->normalize(Pbase(a),0,1)
#define Nfct(a,b,c)	b->normalize(Pbase(a),Pblock(c),0)
#define Ncopy(n)	(n->base==TNAME)?new name(n->string):n

#define Finit(p)	Pfct(p)->f_init
#define Fargdcl(p,q,r)	Pfct(p)->argdcl(q,r)
#define Freturns(p)	Pfct(p)->returns
#define Vtype(v)	Pvec(v)->typ
#define Ptyp(p)		Pptr(p)->typ

		/* avoid redefinitions */
#undef EOFTOK
#undef ASM
#undef BREAK
#undef CASE
#undef CONTINUE
#undef DEFAULT
#undef DELETE
#undef DO
#undef ELSE
#undef ENUM
#undef FOR
#undef FORTRAN
#undef FRIEND
#undef GOTO
#undef IF
#undef NEW
#undef OPERATOR
#undef RETURN
#undef SIZEOF
#undef SWITCH
#undef THIS
#undef WHILE
#undef LP
#undef RP
#undef LB
#undef RB
#undef REF
#undef DOT
#undef NOT
#undef COMPL
#undef MUL
#undef AND
#undef PLUS
#undef MINUS
#undef ER
#undef OR
#undef ANDAND
#undef OROR
#undef QUEST
#undef COLON
#undef ASSIGN
#undef CM
#undef SM
#undef LC
#undef RC
#undef ID
#undef STRING
#undef ICON
#undef FCON
#undef CCON
#undef ZERO
#undef ASOP
#undef RELOP
#undef EQUOP
#undef DIVOP
#undef SHIFTOP
#undef ICOP
#undef TYPE
#undef TNAME
#undef EMPTY
#undef NO_ID
#undef NO_EXPR
#undef FDEF
#undef ELLIPSIS
#undef AGGR
#undef MEM
#undef MEMPTR
#undef PR
#undef TSCOPE
#undef DECL_MARKER
#undef REFMUL
#undef LDOUBLE
#undef LINKAGE
#undef LOCAL
#undef TEMPLATE

#undef XVIRT
#undef XNLIST
#undef XILINE
#undef XIA
#undef STATEMENT
#undef EXPRESSION
#undef SM_PARAM
#undef TEMPLATE_TEST
#undef PTNAME
#undef NEW_INIT_KLUDGE
%}

%union {
	char*	s;
	TOK	t;
	int	i;
	loc	l;
	Pname	pn;
	Ptype	pt;
	Pexpr	pe;
	Pstmt	ps;
	Pbase	pb;
	Pnlist	nl;
	Pslist	sl;
	Pelist	el;
	Pbcl	pbc;
	Pptr	pp;
	PP	p;	// fudge: pointer to all class node objects
	Plist	pl;
	toknode* q;	// token queue
}
%{
#include "tqueue.h"
extern YYSTYPE yylval, yyval;
extern int yyparse();

// in_typedef should allow for nested in_typedef
extern int	declTag;	 // !1: inline, virtual mod permitted
int		in_sizeof = 0;
Ptype 		in_typedef = 0;  // catch redefinition of TNAME
Pname		in_tag = 0;      // handle complex typedefs: int (*)()
extern int	defer_check;	 // redefinition typedef check delay
Pname		curr_scope;

extern int 	must_be_id;	 // !0, TNAME => ID, i.e., int X
int	DECL_TYPE = 0; 	 // lalex() wants this set for global x(*fp)()
int	in_arg_list=0; 	 // !0 when parsing argument list
static int in_binit_list=0;
int	in_class_decl=0; // !0 when processing class definition
int	parsing_class_members=0; // !0 when parsing class def but not member function body
int	in_mem_fct=0;    // !0 when parsing member function definition

#define yylex lalex
#define NEXTTOK() ( (yychar==-1) ? (yychar=yylex(),yychar) : yychar )
#define EXPECT_ID() must_be_id = 1
#define NOT_EXPECT_ID() must_be_id = 0

Pname syn()
{
ll:
	switch (yyparse()) {
	case 0:		return 0;	// EOF
	case 1:		goto ll;	// no action needed
	default:	return yyval.pn;
	}
}

%}
/*
	the token definitions are copied from token.h,
	and all %token replaced by %token
*/
			/* keywords in alphabetical order */
%token EOFTOK		0
%token ASM		1
%token AUTO		2
%token BREAK		3
%token CASE		4
%token CONTINUE		7
%token DEFAULT		8
%token DELETE		9
%token DO		10
%token ELSE		12
%token ENUM		13
%token FOR		16
%token FORTRAN		17 
%token FRIEND		18 
%token GOTO		19
%token IF		20
%token NEW		23
%token OPERATOR		24
%token RETURN		28
%token SIZEOF		30
%token SWITCH		33
%token THIS		34
%token WHILE		39

			/* operators in priority order (sort of) */
%token LP		40
%token RP		41
%token LB		42
%token RB		43
%token REF		44
%token DOT		45
%token NOT		46
%token COMPL		47
%token MUL		50
%token AND		52
%token PLUS		54
%token MINUS		55
%token LT		58
%token GT		60
%token ER		64
%token OR		65
%token ANDAND		66
%token OROR		67
%token QUEST		68
%token COLON		69
%token ASSIGN		70
%token CM		71
%token SM		72
%token LC		73
%token RC		74

    /* = constants etc. */
%token ID	   80
%token STRING	   81
%token ICON	   82
%token FCON	   83
%token CCON	   84
%token NAME	   85
%token ZERO	   86
    /* groups of tokens */
%token ASOP   90	/* op= */
%token RELOP  91	/* LE GE LT GT */
%token EQUOP  92	/* EQ NE */
%token DIVOP  93	/* DIV MOD */
%token SHIFTOP		94	/* LS RS */
%token ICOP		95	/* INCR DECR */
%token TYPE		97
    /* TYPE =	INT FLOAT CHAR DOUBLE REGISTER STATIC EXTERN AUTO
	LONG SHORT UNSIGNED INLINE FRIEND VIRTUAL */

%token TNAME	  123
%token EMPTY	  124
%token NO_ID	  125
%token NO_EXPR	  126
%token FDEF	  127

%token ELLIPSIS	155
%token AGGR	156
%token MEM	160
%token MEMPTR	173
%token PR		175	/* PUBLIC PRIVATE PROTECTED */
%token TSCOPE		178	/* TNAME :: */
%token DECL_MARKER	179
%token REFMUL		180	/* ->*, .* */
%token LDOUBLE		181
%token LINKAGE		182	/* extern "asdf" */
%token LOCAL		183	/* local class */
%token TEMPLATE		185	/* local class */

/* "tokens" for aux data structures */

%token XVIRT	        200 /* class virt */
%token XNLIST		201 /* struct name_list */
%token XILINE		202
%token XIA		203
%token STATEMENT        205
%token EXPRESSION       206
%token SM_PARAM         207 
%token TEMPLATE_TEST    208
%token PTNAME           209
%token NEW_INIT_KLUDGE  210
%token XDELETED_NODE    211
%token DUMMY_LAST_NODE  212

%type <p>	external_def fct_dcl fct_def att_fct_def arg_dcl_list 
		base_init init_list binit
		data_dcl ext_def vec ptr
		type tp enum_dcl moe_list moe 
		tag ttag enumtag class_head class_dcl cl_mem_list 
		cl_mem dl decl_list 
		fname decl initializer stmt_list
		caselab_stmt caselablist
		block statement simple ex_list elist e ee term prim
		term_elist
		cast_decl cast_type c_decl c_type c_tp
		arg_decl formal_decl at arg_type arg_list arg_type_list
		new_decl new_type
		condition
		TSCOPE tscope TNAME tn_list MEMPTR
		qualified_tname
		PTNAME tname ptname template_def
%type <l>	LC RC SWITCH CASE DEFAULT FOR IF DO WHILE GOTO RETURN DELETE
		BREAK CONTINUE
%type <t>	oper ellipsis_opt
		EQUOP DIVOP SHIFTOP ICOP RELOP GT LT ASOP
		ANDAND OROR PLUS MINUS MUL ASSIGN OR ER AND 
		LP LB NOT COMPL AGGR
		TYPE PR REFMUL
		STATEMENT EXPRESSION stmt_or_expr
%type <s>	CCON ZERO ICON FCON STRING LINKAGE
%type <pn>	ID FDEF inline_fct_def identifier
%type <pbc>	base_list base_unit_list base_unit
%type <q>	EMPTY
%type <i>	fct_attributes
%type <pl>	arg_lp
%type <pe>      temp_inst_parm
%type <el>      temp_inst_parms

%left	EMPTY
%left	NO_ID
%left	RC LC ID BREAK CONTINUE RETURN GOTO DELETE DO IF WHILE FOR CASE DEFAULT
	AGGR ENUM TYPE TNAME TSCOPE
%left	NO_EXPR

%left	CM
%right	ASOP ASSIGN
%right	QUEST COLON
%left	OROR
%left	ANDAND
%left	OR
%left	ER
%left	AND
%left	EQUOP
%left	RELOP GT LT
%left	SHIFTOP
%left	PLUS MINUS
%left	MUL DIVOP MEMPTR
%left   REFMUL
%right	NOT COMPL NEW
%right	ICOP SIZEOF
%left	LB LP DOT REF MEM

%start ext_def

%%
/*
	this parser handles declarations one by one,
	NOT a complete .c file
*/


/************** DECLARATIONS in the outermost scope: returns Pname (in yylval) ***/

ext_def		:  external_def		{	return 2; }
		|  SM			{	return 1; }
		|  EOFTOK		{	return 0; }
		|  LINKAGE LC
			{
				set_linkage($<s>1);
				bl_level--;
				return 1;
			}
		|  RC
			{
				set_linkage(0);
				bl_level++;
				return 1;
			}
 		|  template              {  return 1; } 
                |  template_test         {  return 1 ;}
		;


template_test   : TEMPLATE_TEST identifier LT temp_inst_parms GT SM
	          { Ptreet t = tree_template::get($<pn>2->string) ;
                    Pexpr  e = 0 ;
	            if (t)
		      e = t->expand(expr_unlist($<el>4)) ;
	            else error ("%s wasn't an expression template",
		                	$<pn>2->string)  ;
	           } ;

template    :    TEMPLATE
                 { templp->start() ; }
                 LT template_parm_list GT
                 {templp->enter_parameters() ; }
                 template_def
                 {templp->end($<pn>7);
                  templp->in_progress = false ;
                  goto mod;}
                 ;

template_def   : att_fct_def
                 { goto mod; }
                | fct_def
                  { goto mod; }
                | class_dcl SM
                  { Pname pn = $<pb>1->aggr();
                    /* basetype:aggr() does not return the name for a forward */
		    /* declaration, so extract it directly */
                    $$ = (pn ? pn : $<pb>1->b_name) ;
                    DECL_TYPE = 0;  }
                /* internal template specification productions*/
                | STATEMENT 
                  {templp->curr_tree_template = $1 ; }
                   identifier COLON statement
                  {$<pn>3->n_initializer = $<pe>5 ; /* actually a stmt */
                    $<pn>$ = $<pn>3; }
                | EXPRESSION 
                  {templp->curr_tree_template = $1 ; }
		   identifier COLON ee SM
                  {$<pn>3->n_initializer = $<pe>5 ; /* actually a stmt */
                    $<pn>$ = $<pn>3 ; }
                 ;

identifier	: ID 
		|  qualified_tname
			{ $<pn>$ = Ncopy($<pn>1) ;} ;
		;

external_def	:  data_dcl
			{	
			/* if function declartion with arguments
			 * need to make sure modified_tn is traversed */
			if ( $<pn>1 != 0 
				&& $<pn>1->tp->base == FCT 
				&& Pfct($<pn>1->tp)->nargs !=0 ) 
					goto mod;
			else {
				modified_tn = 0;
				curr_scope = 0;
				if ($<pn>1==0) $<i>$ = 1; 
			     }
			}
		|  att_fct_def
			{	goto mod; }
		|  fct_def
			{	goto mod; }
		|  fct_dcl
			{ mod:	if (modified_tn) {
					restore();
					modified_tn = 0;
				}
				local_blk = 0;
				curr_scope = 0;
				if (local_tn) {
					local_restore();
					local_tn = 0;
				}
				
				if (nested_tn) { // x::f(){}
					nested_restore();
					nested_tn = 0;
					nested_type = 0;
				}
			}
		|  ASM LP STRING RP SM
			{	Pname n = new name(make_name('A'));
				n->tp = new basetype(ASM,0);
				Pbase(n->tp)->b_name = Pname($<s>3);
				$$ = n;
			}
		;

fct_dcl		: decl ASSIGN initializer SM
			{
				err_name = $<pn>1;
				if(err_name) err_name->n_initializer = $<pe>3;
				goto fix;
			}
		| decl SM
			{
				Ptype t;
				err_name = $<pn>1;
			fix:
				if (err_name == 0) {
					error("syntax error:TX");
					$$ = Ndata(defa_type,err_name);
				}
				else if ((t=err_name->tp) == 0) {
					error("TX for%n",err_name);
					$$ = Ndata(defa_type,err_name);
				}
				else if (t->base==FCT) {
					if (Pfct(t)->returns==0)
						$$ = Nfct(defa_type,err_name,0);
					else
						$$ = Ndata(0,err_name);
				}
				else {
					error("syntax error:TX for%k%n",t->base,err_name);
					$$ = Ndata(defa_type,err_name);
				}
			}
		;

att_fct_def	:  type decl arg_dcl_list check_inline base_init block
			{	Pname n = Nfct($1,$<pn>2,$6);
				Fargdcl(n->tp,name_unlist($<nl>3),n);
				Finit(n->tp) = $<pn>5;
				$$ = n;
				NOT_EXPECT_ID();
                        //???POP_SCOPE(); // undef arg names
			}
		|  type decl arg_dcl_list check_inline EMPTY
			{
				Pname n = Nfct($1,$<pn>2,dummy);
				Fargdcl(n->tp,name_unlist($<nl>3),n);
				$<q>5->retval.pn = n;
				$$ = n;
				NOT_EXPECT_ID();
			}
		|  type decl arg_dcl_list check_inline NO_ID /*syntax error*/
			{
				error(&$<pn>2->where,"syntax error -- did you forget a ';'?");
				Pname n = Nfct($1,$<pn>2,0);
				$$ = n;
				NOT_EXPECT_ID();
                   	//???POP_SCOPE(); // undef arg names
			}
		;

fct_def		:  decl arg_dcl_list check_inline base_init block
			{	Pname n = Nfct(defa_type,$<pn>1,$5);
				Fargdcl(n->tp,name_unlist($<nl>2),n);
				if ( $<pn>4 && $<pn>4->n_list && 
     					ccl && ccl->csu == UNION )  
						error( "multiple initializers in unionK %s::%n", $<pn>1->string, $<pn>1 );
				Finit(n->tp) = $<pn>4;
				$$ = n;
				NOT_EXPECT_ID();
                        //???POP_SCOPE(); // undef arg names 
			}
		|  decl arg_dcl_list check_inline EMPTY
			{
				Pname n = Nfct(defa_type,$<pn>1,dummy);
				Fargdcl(n->tp,name_unlist($<nl>2),n);
				$<q>4->retval.pn = n;
				$$ = n;
				NOT_EXPECT_ID();
			}
		|  decl arg_dcl_list check_inline NO_ID /*syntax error*/
			{
				error(&$<pn>1->where,"badD of%n -- did you forget a ';'?",$<pn>1);
				Pname n = Nfct(defa_type,$<pn>1,0);
				$$ = n;
				NOT_EXPECT_ID();
                         //???POP_SCOPE(); // undef arg names
			}
		;

inline_fct_def	:  FDEF 
			{//PUSH_ARG_SCOPE
                        	arg_redec($<pn>1);
                   	}
		   base_init block
			{
				Finit($1->tp) = $<pn>3;
				Pfct($1->tp)->body = Pblock($4);
				$$ = $1;
				NOT_EXPECT_ID();
                        //???POP_SCOPE(); // undef arg names
			}
		;


check_inline	:  /* empty */
		   {
			// if parsing implicit inline def, save body
			//   of function for parsing after class def
			switch ( NEXTTOK() ) {
			case LC: case COLON:
				if ( in_class_decl ) {
					// mem or friend inline def
					// save text of mem_init & ftn
					la_backup(yychar,yylval);
					// yylval used as dummy...
					la_backup(FDEF, yylval);
					if ( yylval.q = save_text() )
						yychar = EMPTY;
					else { // syntax error
						// just parse in place
						yylex(); // FDEF
						yychar = yylex();
					}
				} // if in_class_decl
				break;
			default:
				la_backup(yychar,yylval);
				yychar = NO_ID; // 'graceful' recovery
				break;
			}
		   }
		;

base_init	:  COLON { ++in_binit_list; } init_list
			{	
				$$ = $3; 
				in_arg_list = 0;
				--in_binit_list;
			}
		|  %prec EMPTY
			{	$$ = 0; }
		;

init_list	:  binit
			{ $$ = $1; }
		|  init_list CM binit
			{ $<pn>$ = $<pn>3;  $<pn>$->n_list = $<pn>1; }
		;

binit		:  LP elist RP
			{
				$<pn>$ = new name;
				$<pn>$->n_initializer = $<pe>2;
			}
		|  ttag LP elist RP
			{
				Pname n = Ncopy($<pn>1);
				n->base = $<pn>1->base;
				n->tp = $<pn>1->tp;
				n->n_initializer = $<pe>3;
				$<pn>$ = n;
			}

/*
		|  NEW LP elist RP
			{	Pname n = new name;
				n->base = NEW;
				n->n_initializer = $<pe>3;
				$<pn>$ = n;
			}
*/
		;




/*************** declarations: returns Pname ********************/

arg_dcl_list	:  arg_dcl_list data_dcl
			{	if ($<pn>2 == 0)
					error("badAD");
				else if ($<pn>2->tp->base == FCT)
					error("FD inAL (%n)",$<pn>2);
				else if ($1)
                                        $<nl>1->add_list($<pn>2);
				else
					$<nl>$ = new nlist($<pn>2);
			}
		|  %prec EMPTY
			{
                                $$ = 0; 
			}
		;

dl		:  decl
		|  ID COLON 
                        {
                                if ( in_typedef ) {
                                        error("Tdef field");
                                        in_typedef = 0;
                                }
                        //      ENTER_NAME($<pn>1);
                        }
		    e		%prec CM
			{	$$ = $<pn>1;
				$<pn>$->tp = new basetype(FIELD,$<pn>4);
		 	}
		|  COLON e		%prec CM
			{	$$ = new name;
				$<pn>$->tp = new basetype(FIELD,$<pn>2);
                                if ( in_typedef ) {
                                        error("Tdef field");
                                        in_typedef = 0;
                                }
			}
                |  decl ASSIGN
                        {
                        //      ENTER_NAME($<pn>1);
                        }
                   initializer
                        {       Pexpr e = $<pe>4;
                                if (e == dummy) error("emptyIr");
                                $<pn>1->n_initializer = e;
                                init_seen = 0;
                        }
                ;

decl_list	:  dl
			{	
				if ($1) $<nl>$ = new nlist($<pn>1); 
				if ( NEXTTOK() == CM && la_look() == TNAME ) 
					EXPECT_ID();
			}
		|  decl_list CM dl
			{	if ($1)
					if ($3)
						$<nl>1->add($<pn>3);
					else
						error("DL syntax");
				else {
					if ($3) $<nl>$ = new nlist($<pn>3);
					error("DL syntax");
				}
				if ( NEXTTOK() == CM && la_look() == TNAME ) 
					EXPECT_ID();
			}
		;

data_dcl	:  type decl_list SM	
			{ 
				extern int co_hack;
				co_hack = 1;
				/*$$ = Ndata($1,name_unlist($<nl>2));*/
				Pname n = Ndata($1,name_unlist($<nl>2)); 
//error('d',"data_dcl:type decl_list sm: %n%t in_typedef%t in_tag%n",n,n->tp,in_typedef,in_tag);
				if ( in_typedef && in_tag ) { 
					if ( n->tp->check( in_tag->tp, 0 ))
    						error("%nredefined: previous: %t now: %t", in_tag, in_tag->tp, n->tp );
				}
				in_typedef = 0;
				in_tag = 0;
				co_hack = 0;
				DECL_TYPE = 0; 
				$$ = n;
			}
		|  type SM		
			{
				$$ = $<pb>1->aggr(); 
				in_typedef = 0;
				in_tag = 0;
				DECL_TYPE = 0; 
			}
		
		;

/* This is where parametrized types, and regular types come together. */

lt 		: LT { templp->parameters_in_progress++; };
gt 		: GT { templp->parameters_in_progress--; };

tname           : qualified_tname { $<pn>$ = templp->check_tname($<pn>1) ; }
                | qualified_tname lt temp_inst_parms gt
                  { 
		    $<pn>$ = parametrized_typename($<pn>1,
						  (expr_unlist($<el>3))) ; 
		  }
                | NAME LT temp_inst_parms GT
                  { extern Pbase any_type;
		    error("%n was not a parametrized type.", $<pn>$) ;
                    $<pn>$= $<pn>1->tdef() ;
                    $<pn>$->tp = any_type ; } ;



tp		:  TYPE			
			{ 
				$$ = new basetype($<t>1,0); 
				if ( $<t>1 == TYPEDEF ) in_typedef = $<pt>$;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			}
		|  LINKAGE		
			{	$$ = new basetype(EXTERN,0);
				$<pb>$->b_linkage = $<s>1;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			}
		|  qualified_tname
			{ 
				templp->check_tname($<pn>1);
				$$ = new basetype(TYPE,$<pn>1); 
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			}
	/*XXX*/	|  tn_list DECL_MARKER
			{ // modified tn_list TNAME
				$$ = new basetype(TYPE,$<pn>2);
				//xxx qualifier currently ignored...
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			}
		| qualified_tname lt temp_inst_parms gt
			{
				$<pb>$ = parametrized_basetype($<pn>1,(expr_unlist($<el>3)));
			}
		|  class_dcl 
		|  enum_dcl
		|  DECL_MARKER		
			{ 
				if (DECL_TYPE == TNAME)
					$$ = new basetype(TYPE,$<pn>1); 
		//	else if (DECL_TYPE == TSCOPE)
		//	$$ = 0;
				else
				if (DECL_TYPE == 0 &&
					$<p>1->base == TNAME)
						$$ = new basetype(TYPE,$<pn>1); 
				else
					$$ = new basetype($<t>1,0); 
				DECL_TYPE = -1;
			}
		;

type		:  tp
		|  type TYPE		
			{ 
				if ( DECL_TYPE != -1 ) {
					switch ($<pb>1->base) { Pbase bt;
					case COBJ: case EOBJ:
						bt = new basetype(0,0);
						*bt = *$<pb>1;
						DEL($<pb>1);
						$<pb>1 = bt;
					}
					$$ = $<pb>1->type_adj($<t>2); 
				}
				DECL_TYPE = 0;
			}
		|  type tname
			{
//error('d',"decl_type: %d  $1: %t  $2: %n",DECL_TYPE,$<pb>1,$<pn>2);
				if ( DECL_TYPE != -1 ) 
			 		$$ = $<pb>1->name_adj($<pn>2);
			/*XXX*/	else if($<pb>1==0) $$=new basetype(TYPE,$<pn>2);
				DECL_TYPE = 0;
			}
		|  type class_dcl	{ $$ = $<pb>1->base_adj($<pb>2); }
		|  type enum_dcl	{ $$ = $<pb>1->base_adj($<pb>2); }
		|  type DECL_MARKER		
			{ 
				if (DECL_TYPE == TYPE) {
					switch ($<pb>1->base) { Pbase bt;
					case COBJ: case EOBJ:
						bt = new basetype(0,0);
						*bt = *$<pb>1;
						DEL($<pb>1);
						$<pb>1 = bt;
					}
					$$ = $<pb>1->type_adj($<t>2);  
				}
			/*XXX*/	else if (DECL_TYPE == TSCOPE) {
			/*XXX*/		error('i',"type decl_marker(tscope)");
			/*XXX*/	//	$$ = $1;//ignore(?)
			/*XXX*/	}
				else
					$$ = $<pb>1->name_adj($<pn>2); 
				DECL_TYPE = -1;
			}
		;

temp_inst_parms : temp_inst_parms CM temp_inst_parm
                  {$<el>1->add(new expr(ELIST,$<pe>3,NULL)) ; }
                | temp_inst_parm { $<el>$ =
				     new elist(new expr(ELIST,$<pe>1,NULL)); } ;

temp_inst_parm  : new_type
                  {$<pn>1->n_template_arg = template_actual_arg_dummy ;
		   $<pe>$ = $<pn>1; /* keep yacc happy */ }
                | e %prec GT
                  { $<pe>$ = $<pe>1 ; } ; 

/***************** aggregate: returns Pname *****************/

enumtag	:  tag
		{ enumcheck:
			Ptype tx = $<pn>1->tp;
			$$ = $1;
			if ( tx->base == TYPE ) {
				$$ = Pbase(tx)->b_name;
				tx = $<pn>$->tp;
				if ( tx->base != EOBJ
				||   strcmp($<pn>$->string,$<pn>1->string)
				)
					error("%n of type%t redeclared as enum.",$<pn>1,tx);
			} else if ( tx->base != EOBJ )
				error("%n of type%t redeclared as enum",$<pn>1,tx);
		}
	|  DECL_MARKER { goto enumcheck; }
	;

enum_dcl	:  ENUM LC moe_list RC		{ $$ = end_enum(0,$<nl>3); }
		|  ENUM enumtag LC moe_list RC { $$ = end_enum($<pn>2,$<nl>4); }
		|  ENUM enumtag		{ $<pb>$ = (Pbase)$<pn>2->tp; }
		;

moe_list	:  moe
			{	if ($1) $<nl>$ = new nlist($<pn>1); }
		|  moe_list CM moe
			{	if( $3)
					if ($1)
						$<nl>1->add($<pn>3);
					else
						$<nl>$ = new nlist($<pn>3);
			}
		;

template_parm_list : template_parm_list CM template_parm
                   | template_parm
                   | { $<pn>$ = NULL ;} ;


stmt_or_expr    : STATEMENT | EXPRESSION ;

template_parm   : AGGR identifier
                  /* Build the name for the parameter
		  /* Check that AGGR is indeed CLASS */
                 { templp->collect($<t>1, $<pn>2) ; }
                 | stmt_or_expr identifier
                 { templp->collect($<t>1, $<pn>2) ; }	
                 |   type formal_decl
			{templp->collect(Ndata($1,$<pn>2)); } ;	


/* Sam: these productions are a variant of the ones for arg_decl,
 verify them against arg_decl for each release. */
formal_decl	:  ID
			{	$$ = $<pn>1; }
		|  ptr formal_decl		%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$1;
				$$ = $2;
			}
		|  formal_decl vec		%prec LB
			{	Vtype($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			}
		|  formal_decl arg_list
			{	Freturns($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			} ;


moe		:  ID
			{	$$ = $<pn>1; $<pn>$->tp = moe_type; }
		|  ID ASSIGN e
			{	$$ = $<pn>1;
				$<pn>$->tp = moe_type;
				$<pn>$->n_initializer = $<pe>3;
			}
		|  /* empty: handle trailing CM: enum e { a,b, }; */
			{	$$ = 0; }
		;

class_dcl	:  class_head cl_mem_list RC
			{	parsing_class_members = 0;
				RESTORE_STATE();
				switch ( NEXTTOK() ) {
				case TYPE: case AGGR: case ENUM: case EOFTOK:
					error("`;' or declaratorX afterCD");
					la_backup(yychar,yylval);
					yychar = SM;
					break;
				}
				la_backup(yychar,yylval);
				yychar = -1;
				restore_text();
				++bl_level; // scope weirdness!
				++in_mem_fct;
			}
			inline_mem_defs
			{
				--in_mem_fct;
				--bl_level; // scope weirdness!
				if ( yychar == ID ) {
					// (yuk!) adjust lex level
					--yylval.pn->lex_level;
				}
				ccl->mem_list = name_unlist($<nl>2);
				if ( --in_class_decl )  // nested class
					// continue to parse enclosing class
					parsing_class_members = 1;
				ccl->nest_list = nested_type;
				if ( nested_tn ) nested_restore();
				nested_type = nested_type_vec[in_class_decl];
				nested_tn = nested_tn_vec[in_class_decl];
				end_cl();
				declTag = 1;
                        //POP_SCOPE();
			}
        	|  AGGR tag 
			{ aggrcheck:
				$<pb>$ = (Pbase)$<pn>2->tp; 
				if ( $$->base == TYPE ) {
					Pname nx = $<pb>$->b_name;
					$<pb>$ = (Pbase)nx->tp;
					if ( $$->base != COBJ
					||   strcmp(nx->string,$<pn>2->string)
					)
						error("%n of type%t redeclared as%k.",$<pn>2,$<pb>$,$<t>1);
				} else if ( $$->base != COBJ )
					error("%n of type%t redeclared as%k",$<pn>2,$<pb>$,$<t>1);
				check_tag();
			}
                | AGGR qualified_tname lt temp_inst_parms gt
                  { 
		    Pname p = parametrized_typename($<pn>2, (expr_unlist($<el>4))) ;
                    $<pb>$ = (Pbase)p->tp; 
                    check_tag(); 
		  }                   
        	|  AGGR DECL_MARKER 
			{
				goto aggrcheck;
			}
		;


inline_mem_defs	:  /* empty */
		|  inline_mem_defs inline_fct_def
		;

base_list	:  COLON base_unit_list		{ $$ = $2; }
		|  %prec EMPTY			{ $$ = 0; }
		;

base_unit_list	:  base_unit
		|  base_unit_list CM base_unit
			{	if ($3) { $$ = $3; $<pbc>$->next = $1; } }
		;

base_unit	:  ttag			{ $$ = dobase(0,$<pn>1); }
		|  PR ttag		{ $$ = dobase($<t>1,$<pn>2); }
		|  TYPE ttag		{ $$ = dobase(0,$<pn>2,$<t>1); }
		|  PR TYPE ttag		{ $$ = dobase($<t>1,$<pn>3,$<t>2); }
		|  TYPE PR ttag		{ $$ = dobase($<t>2,$<pn>3,$<t>1); }
		;

class_head	:  AGGR LC	
			{//PUSH_CLASS_SCOPE(0);
				parsing_class_members = 1;
				$$ = start_cl($<t>1,0,0); 
				nested_tn_vec[in_class_decl] = nested_tn;
				nested_type_vec[in_class_decl++] = nested_type;
				nested_tn = nested_type = 0;
				SAVE_STATE();
			}

		|  AGGR tag base_list LC
			{ //PUSH_CLASS_SCOPE($<pn>2->string);
				parsing_class_members = 1;
				$$ = start_cl($<t>1,$<pn>2,$<pbc>3);
				nested_tn_vec[in_class_decl] = nested_tn;
				nested_type_vec[in_class_decl++] = nested_type;
				nested_tn = nested_type = 0;
				SAVE_STATE();
			}
		;

tag		:  ID	{ $$ = $1; }
		|  qualified_tname { $$=$1; }
		;

ttag		:  ID	{ $$ = $1; }
		|  tname { $$=$1; }
		;

cl_mem_list	:  cl_mem_list cl_mem
			{
				if ($2) {
					if ($1)
						$<nl>1->add_list($<pn>2);
					else
						$<nl>$ = new nlist($<pn>2);
				}
			}
		|  %prec EMPTY { $$ = 0; }
		|  cl_mem_list TEMPLATE
			{
				error( "ZizedTD must be atG, notC scope" );
				error('i', "cannot recover from previous error" );
			}
		;

cl_mem		:  data_dcl
		|  att_fct_def SM
		|  fct_def SM
		|  fct_def
		|  att_fct_def
		|  fct_dcl
		|  PR COLON
			{	$$ = new name;
				$<pn>$->base = $<t>1;
			}
	/*XXX	|  tn_list TNAME SM
	 *		{	Pname n = Ncopy($<pn>2);
	 *			n->n_qualifier = $<pn>1;
	 *			n->base = PR;
	 *			$$ = n;
	 *		}
	 */	|  tn_list fname SM
			{	Pname n = Ncopy($<pn>2);
				if (n->n_oper == TYPE) {
					error('s',"visibilityD for conversion operator");
					// n->tp = Ptype(n->n_initializer);
					n->tp = Ptype(n->cond);
					n->cond = 0;
					// n->n_initializer = 0;
					n->n_oper = 0;
					sig_name(n);
				}
				n->n_qualifier = $<pn>1;
				n->base = PR;
				$$ = n;
			}
		;

/************* declarators:	returns Pname **********************/
/*	a ``decl'' is used for function and data declarations,
		and for member declarations
		(it has a name)
	an ``arg_decl'' is used for argument declarations
		(it may or may not have a name)
	an ``cast_decl'' is used for casts
		(it does not have a name)
	a ``new_decl'' is used for type specifiers for the NEW operator
		(it does not have a name, and PtoF and PtoV cannot be expressed)
*/

fname		:  ID
			{	$$ = $<pn>1; }
		|  COMPL TNAME  /* qualified_tname? */
			{	$$ = Ncopy($<pn>2);
				$<pn>$->n_oper = DTOR;
			}
		|  OPERATOR oper
			{	$$ = new name(oper_name($2));
				$<pn>$->n_oper = $<t>2;
			}
		|  OPERATOR c_type
			{	Pname n = $<pn>2;
				n->string = "_type";
				n->n_oper = TYPE;
				n->cond = Pexpr(n->tp);
				// n->n_initializer = Pexpr(n->tp);
				n->tp = 0;
				$$ = n;
			}
		;

oper		:  PLUS
		|  MINUS
		|  MUL
		|  AND
		|  OR
		|  ER
		|  SHIFTOP
		|  EQUOP
		|  DIVOP
		|  RELOP
		|  LT
		|  GT
		|  ANDAND
		|  OROR
		|  LP RP	{	$$ = CALL; }
		|  LB RB	{	$$ = DEREF; }
		|  NOT
		|  COMPL
		|  ICOP
		|  ASOP
		|  ASSIGN
		|  NEW		{	$$ = NEW; }
		|  DELETE	{	$$ = DELETE; }
		|  REF		{	$$ = REF; }
		|  CM		{	$$ = CM; }
		|  REFMUL	{	$$ = REFMUL;
					if ($<t>1 == DOT) error(".* cannot be overloaded");
				}
		;

tn_list		:  tscope
		/*XXX*/	{   if ( $<pn>1 != sta_name ) {
// error('d',"tn_list: tscope: pn1: %s", $<pn>1->string);
				Ptype t = $<pn>1->tp;
				while ( t->base == TYPE )
					t = Pbase(t)->b_name->tp;
				Pname n = Pbase(t)->b_name;
				if (NEXTTOK() == TNAME
				&& strcmp(n->string,yylval.pn->string)==0){
					// ctor -- change to ID to avoid
					//    parsing as type spec
					yychar = ID;
					yylval.pn = Ncopy(yylval.pn);
					yylval.pn->n_oper = TNAME;
				}
			    }
			    $<pn>$ = $<pn>1;
			}
/* YYY		|  tn_list tscope	{ $<pn>$ = $<pn>1; } */
 		|  tn_list tscope	{ error('s', "CNs do not nest, use typedef x::y y_in_x"); };
	/*XXX	|  tn_list ID DOT	{ error("CNs do not nest"); } */
		;

qualified_tname	: tn_list TNAME
			{	$<pn>$ = $<pn>2;
				//xxx qualifier currently ignored...
				// $<pn>$ = Ncopy( $<pn>2 );
				// $<pn>$->n_oper = TNAME;
				// $<pn>$->n_qualifier = $<pn>1;
			}
		| TNAME
			{	$<pn>$ = $<pn>1;
			//	$<pn>$ = Ncopy( $<pn>1 );
			//	$<pn>$->n_oper = TNAME;
			}
		;

fct_attributes	: /* empty */
			{ $$ = 0; }
		| fct_attributes TYPE
 			{ /* const/volatile function */
				switch ( $<t>2 ) {
				case VOLATILE:
					error('s',"volatile functions");
					break;
				case CONST:
					$$ = ($1 | 1);
					break;
				default:
					if ( NEXTTOK() != SM
					&&   yychar != COLON
					&&   yychar != LC ) {
						la_backup(yychar,yylval);
						yylval.t = $<t>2;
						la_backup(TYPE,yylval);
						yylval.t = SM;
						yychar = SM;
						error("syntax error: unexpected%k (did you forget a `;'?)",$<t>2);
					} else error("FD syntax: unexpected%k",$<t>2);
					break;
				}
 			}
		;

decl		:  decl arg_list
			{	Freturns($2) = $<pn>1->tp;
				$<pn>1->tp = $<pt>2;
			}
 		|  decl LP RP fct_attributes
 			{ /* function with no argument */
 				$<pn>1->tp = new fct($<pn>1->tp,0,1);
 				Pfct($<pn>1->tp)->f_const = ($<i>4 & 1);
 			}
		|  tname arg_list
			{	Pname n = $<pn>1;
				$$ = Ncopy(n);
                                //??? what if tname is qualified ???
				if (ccl && strcmp(n->string,ccl->string)) n->hide();
				$<pn>$->n_oper = TNAME;
				Freturns($2) = $<pn>$->tp;
				$<pn>$->tp = $<pt>2;
			}
		|  decl arg_lp elist RP
			/*	may be class object initializer,
				class object vector initializer,
				if not elist will be a CM or an ID
			*/
			{	
				$<pn>1->tp = new fct($<pn>1->tp,$<pn>3,1); 
 				in_arg_list = 0;
 				end_al($2,0);
				//RESTORE_STATE();
                        //POP_SCOPE(); // similar to end_al()
			}
		|  tname LP MUL ID RP arg_list
			{
				Pptr p = new ptr( PTR, 0 );
				Ptyp(p) = new basetype(TYPE,$<pn>1);
				Freturns( $6 ) = Ptype(p);
				$<pn>4->tp = $<pt>6;
				$$ = $4;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			}
		|  tname LP elist RP
			{	$$ = Ncopy($<pn>1);
				$<pn>$->n_oper = TNAME;
				$<pn>$->tp = new fct(0,$<pn>3,1);
			} 
 		|  tname LP RP fct_attributes
 			{ /* function with no argument */
				$$ = Ncopy($<pn>1);
 				$<pn>$->n_oper = TNAME;
 				$<pn>$->tp = new fct(0,0,1);
 				Pfct($<pn>1->tp)->f_const = ($<i>4 & 1);
			}
		|  tname LP MEMPTR decl RP arg_list
			{	memptrdcl($<pn>3,$<pn>1,$<pt>6,$<pn>4);
				$$ = $4;
			}
		|  fname
		|  ID DOT fname
			{	$$ = Ncopy($<pn>3);
				$<pn>$->n_qualifier = $1;
				error(strict_opt?0:'w',"`.' used for qualification; please use `::' (anachronism)");
			}
		|  tn_list fname
			{	$$ = $2;
				if ( $<pn>1 != sta_name ) {
    					set_scope($<pn>1); 
    					$<pn>$->n_qualifier = $<pn>1;
				}
			}
	/*XXX*/	|  tn_list ID DOT fname
			{	$$ = Ncopy($<pn>4);
				$<pn>$->n_qualifier = $2;
				error(ansi_opt?0:'w',"anachronism `.' used for qualification; please use `::'");
				if ( $<pn>1 != sta_name ) {
    					set_scope($<pn>1); 
    					$<pn>2->n_qualifier = $<pn>1;
				}
			}
	/*XXX	|  tn_list TNAME
	 *		{
	 *			if ( $<pn>1 == sta_name ) 
     	 *				error( ":: applied to CN%n", $<pn>2 );
	 *			$$ = Ncopy($<pn>2);
	 *			set_scope($<pn>1);
	 *			$<pn>$->n_oper = TNAME;
	 *			$<pn>$->n_qualifier = $<pn>1;
	 *		}
	 */	|  ptr decl	%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = $<pt>1;
				$$ = $2;
			}
		|  ptr tname	%prec MUL
			{	$$ = Ncopy($<pn>2);
				$<pn>$->n_oper = TNAME;
				// cannot evaluate at this point: defer until data_dcl
				if ( in_typedef ) {
					defer_check = 1;
					in_tag = $<pn>2;
				}
				$<pn>2->hide();
				defer_check = 0;
				$<pn>$->tp = $<pt>1;
			}
		|  tname vec	%prec LB
			{	$$ = Ncopy($<pn>1);
				$<pn>$->n_oper = TNAME;
				if ( in_typedef ) {
					defer_check = 1;
					in_tag = $<pn>1;
				}
				$<pn>1->hide();
				defer_check = 0;
				$<pn>$->tp = $<pt>2;
			}
		|  decl vec	%prec LB	
			{	Vtype($2) = $<pn>1->tp;
				$<pn>1->tp = $<pt>2;
			}
/*
		|  LP decl RP arg_list
			{
				Freturns($4) = $<pn>2->tp;
				$<pn>2->tp = $<pt>4;
				$$ = $2;
			}
		|  LP decl RP vec
			{	Vtype($4) = $<pn>2->tp;
				$<pn>2->tp = $<pt>4;
				$$ = $2;
			}
*/
		|  arg_lp decl RP 
			{
				$$ = $2; 
				in_arg_list = 0;
				end_al($1,0);
				//RESTORE_STATE();
                        //POP_SCOPE(); // similar to end_al()
			}
		;

arg_decl	:  ID
			{	$$ = $<pn>1; }
		|  ptr qualified_tname	%prec MUL
			{	$$ = Ncopy($<pn>2);
				$<pn>$->n_oper = TNAME;
				$<pn>2->hide();
				$<pn>$->tp = $<pt>1;
			}
		|  %prec NO_ID
			{	
				$$ = new name; 
				NOT_EXPECT_ID();
			}
		|  ptr arg_decl		%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$1;
				$$ = $2;
			}
		|  arg_decl vec		%prec LB
			{	Vtype($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			}
		|  arg_decl arg_list
			{	Freturns($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			}
/*
		|  LP arg_decl RP arg_list
			{	Freturns($4) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$4;
				$$ = $2;
			}
		|  LP arg_decl RP vec
			{	Vtype($4) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$4;
				$$ = $2;
			}
*/
		|  arg_lp arg_decl RP	
			{ 
// error('d', "arg_lp arg_decl rp in_arg_list: %d", in_arg_list );
				$$ = $2; 
				in_arg_list = 0;
				end_al($1,0);
				//RESTORE_STATE();
                        //POP_SCOPE(); // similar to end_al()
			}
		;

new_decl	:  %prec NO_ID
			{	$$ = new name; }
		|  ptr new_decl		%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$1;
				$$ = $2;
                                NOT_EXPECT_ID();
			}
		|  new_decl vec		%prec LB
			{	Vtype($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			}
		;

cast_decl	:  %prec NO_ID { $$ = new name; }
		|  ptr cast_decl			%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$1;
				$$ = $2;
				NOT_EXPECT_ID();
			}
		|  cast_decl vec			%prec LB
			{	Vtype($2) = $<pn>1->tp;
				$<pn>1->tp = (Ptype)$2;
			}
		|  LP cast_decl RP arg_list
			{	Freturns($4) = $<pn>2->tp;
				$<pn>2->tp = $<pt>4;
				$$ = $2;
			}
		|  LP cast_decl RP vec
			{	Vtype($4) = $<pn>2->tp;
				$<pn>2->tp = $<pt>4;
				$$ = $2;
			}
		;

c_decl		:  %prec NO_ID
			{	$$ = new name; }
		|  ptr c_decl				%prec MUL
			{	Ptyp($1) = $<pn>2->tp;
				$<pn>2->tp = (Ptype)$1;
				$$ = $2;
			}
		;



/***************** statements: returns Pstmt *****************/
stmt_list	:  /* empty */
			{
				$$ = 0; 
			}
		|  stmt_list TEMPLATE
			{
				error( "ZizedTD must be atG, not local scope" );
				error('i', "cannot recover from previous error" );
			}
         	|  stmt_list caselab_stmt
			{	
				if ($2)
					if ($1)
						$<sl>1->add($<ps>2);
					else {
						$<sl>$ =  new slist($<ps>2);
						stmt_seen = 1;
					}
			}
		;
caselab_stmt	:  caselablist statement
			{
				$$ = $2;
				if ($2)	stmt_seen = 1;
			}
		;

caselablist	: /* empty */
			{
				$$ = 0;
				check_decl();
			}
		;

condition	:  LP e RP
			{	$$ = $2;
			/*	if ($<pe>$ == dummy) error("empty condition");*/
				stmt_seen = 1;
			}
		;

block		:  LC
			{//PUSH_BLOCK_SCOPE	
				cd_vec[cdi] = cd;
				stmt_vec[cdi] = stmt_seen;
				tn_vec[cdi] = modified_tn;
				lcl_blk_vec[cdi++] = local_blk;
				lcl_tn_vec[cdi] = local_tn;
				local_blk = 0;
				local_tn = 0;
				cd = 0;
				stmt_seen = 0;
				modified_tn = 0;
			}
			stmt_list RC
			{	Pname n = name_unlist(cd);
				Pstmt ss = stmt_unlist($<sl>3);
				$$ = new block($<l>1,n,ss,$<l>4);
				if ( local_tn ) local_restore();
				if ( local_blk ) local_name();
				if (modified_tn) restore();
				cd = cd_vec[--cdi];
				stmt_seen = stmt_vec[cdi];
				modified_tn = tn_vec[cdi];
				local_tn = lcl_tn_vec[cdi];
				local_blk = lcl_blk_vec[cdi];
				if (cdi < 0) error('i',"block level(%d)",cdi);
				NOT_EXPECT_ID();
                        //POP_SCOPE(); // similar to end_al()
			}
		|  LC RC
			{	$$ = new block($<l>1,0,0,$<l>2); NOT_EXPECT_ID();}
		|  LC error RC
			{	$$ = new block($<l>1,0,0,$<l>3); NOT_EXPECT_ID();}
		;

simple		:  ee
			{	$$ = new estmt(SM,curloc,$<pe>1,0);	}
		|  BREAK
			{	$$ = new stmt(BREAK,$<l>1,0); }
		|  CONTINUE
			{	$$ = new stmt(CONTINUE,$<l>1,0); }
		|  GOTO ID
			{	$$ = new lstmt(GOTO,$<l>1,$<pn>2,0); }
		|  DO { stmt_seen=1; } caselab_stmt WHILE condition
			{	$$ = new estmt(DO,$<l>1,$<pe>5,$<ps>3); }
		|  ASM LP STRING RP
			{	
				if (stmt_seen)
					$$ = new estmt(ASM,curloc,(Pexpr)$<s>3,0);
				else {
					Pname n = new name(make_name('A'));
					n->tp = new basetype(ASM,(Pname)$<s>3);
					if (cd)
						cd->add_list(n);
					else
						cd = new nlist(n);
					$$ = 0;
				}
			}
		;

sm		: {
			if ( NEXTTOK() != SM ) {
				error("`;' missing afterS");
				la_backup(yychar,yylval);
				yychar = SM;
			}
		  } SM
		;

statement	:  simple sm
		|  SM
			{	$$ = new estmt(SM,$<l>1,dummy,0); }
		|  RETURN e SM
			{	$$ = new estmt(RETURN,$<l>1,$<pe>2,0); }
		|  TYPE STRING block
			{
				error("local linkage specification");
				$$ = $<pn>3;
			}
		|  data_dcl
			{	Pname n = $<pn>1;
				if (n) {
//error('d',"adding local dcl of%n%t ll %d in_typedef%t",n,n->tp,n->lex_level,in_typedef);
					if (stmt_seen) {
						$$ = new block(n->where,n,0);
						$<ps>$->base = DCL;
					}
					else {
						if (cd)
							cd->add_list(n);
						else
							cd = new nlist(n);
						$$ = 0;
					}
				} // if n
			}
		|  att_fct_def
			{
				Pname n = $<pn>1;
				error(&n->where,"%n's definition is nested (did you forget a ``}''?)",n);
				if (cd)
					cd->add_list(n);
				else
					cd = new nlist(n);
				$$ = 0;
			}
		|  block
		|  IF condition caselab_stmt
			{	$$ = new ifstmt($<l>1,$<pe>2,$<ps>3,0); }
		|  IF condition caselab_stmt ELSE caselab_stmt
			{	$$ = new ifstmt($<l>1,$<pe>2,$<ps>3,$<ps>5); }
		|  WHILE condition caselab_stmt
			{	$$ = new estmt(WHILE,$<l>1,$<pe>2,$<ps>3); }
		|  FOR LP { stmt_seen=1; } caselab_stmt e SM e RP caselab_stmt
			{	$$ = new forstmt($<l>1,$<ps>4,$<pe>5,$<pe>7,$<ps>9); }
		|  SWITCH condition caselab_stmt
			{	$$ = new estmt(SWITCH,$<l>1,$<pe>2,$<ps>3); }
		|  ID COLON { $$ = $1; stmt_seen=1; } caselab_stmt
			{	Pname n = $<pn>3;
				$$ = new lstmt(LABEL,n->where,n,$<ps>4);
			}
		|  TNAME COLON { $$ = new name($<pn>1->string); stmt_seen=1; } caselab_stmt
			{	Pname n = $<pn>3;
				$$ = new lstmt(LABEL,n->where,n,$<ps>4);
			}
		|  CASE { stmt_seen=1; } e COLON caselab_stmt
			{	if ($<pe>3 == dummy) error("empty case label");
				$$ = new estmt(CASE,$<l>1,$<pe>3,$<ps>5);
			}
		|  DEFAULT COLON { stmt_seen=1; } caselab_stmt
			{	$$ = new stmt(DEFAULT,$<l>1,$<ps>4); }
		;



/********************* expressions: returns Pexpr **************/
elist		: ex_list
			{	Pexpr e = expr_unlist($<el>1);
				while (e && e->e1==dummy) {
					register Pexpr ee2 = e->e2;
					if (ee2) error("EX inEL");
					delete e;
					e = ee2;
				}
				$$ = e;
			}
		;

ex_list		:  initializer		%prec CM
			{	$<el>$ = new elist(new expr(ELIST,$<pe>1,0)); }
		|  ex_list CM initializer
			{	$<el>1->add(new expr(ELIST,$<pe>3,0)); }
		;

initializer	:  e				%prec CM
		|  LC elist RC
			{
  			  	if ( in_arg_list ) 
       			  		error( "syntax error: IrL not permitted in AL" );
				else if ( in_binit_list )
					error( "syntax error: IrL not permitted inMIr" );
  			  	else 
					init_seen = 1;
				Pexpr e;
				if ($2)
					e = $<pe>2;
				else
					e = new expr(ELIST,dummy,0);
				$$ = new expr(ILIST,e,0);
			}
		;

ee		:  ee ASSIGN ee
			{	bbinop:	$$ = new expr($<t>2,$<pe>1,$<pe>3); }
		|  ee PLUS ee	{	goto bbinop; }
		|  ee MINUS ee	{	goto bbinop; }
		|  ee MUL ee	{	goto bbinop; }
		|  ee AND ee	{	goto bbinop; }
		|  ee OR ee	{	goto bbinop; }
		|  ee ER ee	{	goto bbinop; }
		|  ee SHIFTOP ee	{ 	goto bbinop; }
		|  ee EQUOP ee	{	goto bbinop; }
		|  ee DIVOP ee	{	goto bbinop; }
		|  ee RELOP ee	{	goto bbinop; }
		|  ee GT ee	{	goto bbinop; }
		|  ee LT ee	{	goto bbinop; }
		|  ee ANDAND ee	{	goto bbinop; }
		|  ee OROR ee	{	goto bbinop; }
		|  ee ASOP ee	{	goto bbinop; }
		|  ee CM ee	{	goto bbinop; }
		|  ee QUEST ee COLON ee
			{	$$ = new qexpr($<pe>1,$<pe>3,$<pe>5); }
		|  DELETE term 
			{ $$ = new expr(DELETE,$<pe>2,0); }
		|  DELETE LB e RB term
			{
				if($<pe>3 != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `delete[v]' is redundant; use `delete[] instead (anachronism)");
				}
				$$ = new expr(DELETE,$<pe>5,$<pe>3);
			}
		|  MEM DELETE term 
			{	$$ = new expr(GDELETE,$<pe>3,0); }
		|  MEM DELETE LB e RB term
			{
				if($<pe>4 != dummy) { 
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `::delete[v]' is redundant; use `::delete[] instead (anachronism)");
				}
				$$ = new expr(DELETE,$<pe>6,$<pe>4);
			}
		|  term
		; 

e		:  e ASSIGN e
			{	binop:	$$ = new expr($<t>2,$<pe>1,$<pe>3); }
		|  e PLUS e	{	goto binop; }
		|  e MINUS e	{	goto binop; }
		|  e MUL e	{	goto binop; }
		|  e AND e	{	goto binop; }
		|  e OR e	{	goto binop; }
		|  e ER e	{	goto binop; }
		|  e SHIFTOP e	{ 	goto binop; }
		|  e EQUOP e	{	goto binop; }
		|  e DIVOP e	{	goto binop; }
		|  e RELOP e	{	goto binop; }
		|  e LT e	{	goto binop; }
		|  e GT e	{	goto binop; }
		|  e ANDAND e	{	goto binop; }
		|  e OROR e	{	goto binop; }
		|  e ASOP e	{	goto binop; }
		|  e CM e	{	goto binop; }
		|  e QUEST e COLON e
			{	$$ = new qexpr($<pe>1,$<pe>3,$<pe>5); }
		|  DELETE term 
			{	$$ = new expr(DELETE,$<pe>2,0); }
		|  DELETE LB e RB term
			{
				if($<pe>3 != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `delete[v]' is redundant; use `delete[] instead (anachronism)");
				}
				$$ = new expr(DELETE,$<pe>5,$<pe>3);
			}
		|  MEM DELETE term 
			{	$$ = new expr(GDELETE,$<pe>3,0); }
		|  MEM DELETE LB e RB term
			{
				if($<pe>4 != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `::delete[v]' is redundant; use `::delete[] instead (anachronism)");
				}
				$$ = new expr(DELETE,$<pe>6,$<pe>4);
			}
		|  term { 
			init_seen = 0; 
			} 
		|  %prec NO_EXPR
			{	$$ = dummy; }
		; 

term		:  NEW cast_type	{ goto new1; }
		|  NEW new_type
			{	new1:
				Ptype t = $<pn>2->tp;
				$$ = new texpr(NEW,t,0);
			}
 		|  MEM NEW cast_type	{ goto new3; }
		|  MEM NEW new_type
 			{	new3:
				Ptype t = $<pn>3->tp;
 				$$ = new texpr(GNEW,t,0);
 			}
		|  term ICOP
			{	$$ = new expr($<t>2,$<pe>1,0); }
		|  cast_type term %prec ICOP
			{	$$ = new texpr(CAST,$<pn>1->tp,$<pe>2); }
		|  MUL term
			{	$$ = new expr(DEREF,$<pe>2,0); }
		|  AND term
			{	$$ = new expr(ADDROF,0,$<pe>2); }
		|  MINUS term
			{	$$ = new expr(UMINUS,0,$<pe>2); }
		|  PLUS term
			{	$$ = new expr(UPLUS,0,$<pe>2); }
		|  NOT term
			{	$$ = new expr(NOT,0,$<pe>2); }
		|  COMPL term
			{	$$ = new expr(COMPL,0,$<pe>2); }
		|  ICOP term
			{	$$ = new expr($<t>1,0,$<pe>2); }
		|  SIZEOF term
			{	
				$$ = new texpr(SIZEOF,0,$<pe>2); 
				in_sizeof = 0;
			}
		|  SIZEOF cast_type %prec SIZEOF
			{	
				$$ = new texpr(SIZEOF,$<pn>2->tp,0); 
				in_sizeof = 0;
			}
		|  term LB e RB
			{	$$ = new expr(DEREF,$<pe>1,$<pe>3); }
		|  term REF prim
			{	$$ = new ref(REF,$<pe>1,$<pn>3); }
		|  term REFMUL term
			{	$$ = new expr($<t>2,$<pe>1,$<pe>3); }
		|  term REF qualified_tname
			{	$$ = new ref(REF,$<pe>1,Ncopy($<pn>3)); }
		|  term DOT prim
			{	$$ = new ref(DOT,$<pe>1,$<pn>3); }
		|  term DOT qualified_tname
			{	$$ = new ref(DOT,$<pe>1,Ncopy($<pn>3)); }
		|  prim
		|  term_elist 
			{
			if ( init_seen )
     				error( "syntax error:IrL illegal within ()");
			}

		|  term_lp e RP
			{
				if ( $2 == dummy )
					error("syntax error: nullE");
				$$ = $2;
			}
		|  ZERO
			{	$$ = zero; }
		|  ICON
			{	$$ = new expr(ICON,0,0);
				$<pe>$->string = copy_if_need_be($<s>1);
			}
		|  FCON
			{	$$ = new expr(FCON,0,0);
				$<pe>$->string = copy_if_need_be($<s>1);
			}
		|  STRING
			{	$$ = new expr(STRING,0,0);
				$<pe>$->string = copy_if_need_be($<s>1);
			}
		|  CCON
			{	$$ = new expr(CCON,0,0);
				$<pe>$->string = copy_if_need_be($<s>1);
			}
		|  THIS
			{	$$ = new expr(THIS,0,0); }
		;

term_elist	: TYPE LP elist RP
			{ 	$$ = new texpr(VALUE,tok_to_type($<t>1),$<pe>3); }
/*
		|  qualified_tname LP elist RP
*/
		|  tname LP elist RP
			{	$$ = new texpr(VALUE,$<pn>1->tp,$<pe>3); }
                |  NEW term_lp elist RP cast_type    { goto new2; }
                |  NEW term_lp elist RP new_type     /* allow separate allocation */
			{	new2:
				Ptype t = $<pn>5->tp;
				$$=new texpr(NEW,t,0);
				$<pe>$->e2 = $<pe>3;
			}
		|  MEM NEW term_lp elist RP cast_type	{ goto new4; }
		|  MEM NEW term_lp elist RP new_type	/* allow separate allocation */
			{	new4:
				Ptype t = $<pn>6->tp;
				$$ = new texpr(GNEW,t,0);
				$<pe>$->e2 = $<pe>4;
			}
		|  term LP elist RP
			{	
				Pexpr ee = $<pe>3;
				Pexpr e = $<pe>1;
				if (e->base==NEW || e->base==GNEW)
					e->e1 = ee;
				else
					$$ = new call(e,ee);
			}

		;

ptname         : PTNAME lt temp_inst_parms  gt
                 {
		    $<pn>$ =parametrized_typename($<pn>1,(expr_unlist($<el>3))); 
	 	 }
               ;

tscope         :  TSCOPE
                       {       
				$<pn>$ = $<pn>1; 
				curr_scope = $<pn>1;
			}
               	|  MEM { $<pn>$ = sta_name; }
		|  ptname TSCOPE { $<pn>$ = $<pn>1; }
               ;


prim		:  ID
			{	$$ = $<pn>1; }
	/*XXX*/	|  tn_list ID
			{	$$ = Ncopy($<pn>2);
				$<pn>$->n_qualifier = $<pn>1;
			}
		|  OPERATOR oper
			{	$$ = new name(oper_name($2));
				$<pn>$->n_oper = $<t>2;
			}
		|  tn_list OPERATOR oper
			{	$$ = new name(oper_name($3));
				$<pn>$->n_oper = $<t>3;
				$<pn>$->n_qualifier = $<pn>1;
			}
		|  OPERATOR c_type
			{	$$ = $2;
				sig_name($<pn>$);
			}
		|  tn_list OPERATOR c_type
			{	$$ = $3;
				sig_name($<pn>$);
				$<pn>$->n_qualifier = $<pn>1;
			}
		|  tn_list COMPL tag	/* allow explicit call of destructor */
			{	
				if (strcmp($<pn>1->string,$<pn>3->string)) error("syntax error: inconsistent destructor notation");
				$$ = new name(oper_name(DTOR));
				$<pn>$->n_oper = DTOR;
				$<pn>$->n_qualifier = $<pn>1;
			}
		;



/****************** abstract types (return type Pname) *************/
cast_type	:  term_lp type cast_decl RP 
			{ $$ = Ncast($2,$<pn>3); }
		;

term_lp		:  LP { check_cast(); }
		;

c_tp		:  TYPE	
			{
				TOK t = $<t>1;

				switch (t) {
				case FRIEND:
				case OVERLOAD:
				case REGISTER:
				case STATIC:
				case EXTERN:
				case AUTO:
				case VIRTUAL:
					error("%k in operatorT",t);
					t = INT;
					
				}

				$$ = new basetype(t,0);

			}
		|  tname	{ $$ = new basetype(TYPE,$<pn>1); }
		|  c_tp TYPE		
			{ 
				if ( DECL_TYPE != -1 ) {
					switch ($<pb>1->base) { Pbase bt;
					case COBJ: case EOBJ:
						bt = new basetype(0,0);
						*bt = *$<pb>1;
						DEL($<pb>1);
						$<pb>1 = bt;
					}
					$$ = $<pb>1->type_adj($<t>2); 
				}
				DECL_TYPE = 0;
			}
		|  c_tp tname
			{ 
				if ( DECL_TYPE != -1 ) 
			 		$$ = $<pb>1->name_adj($<pn>2);
				DECL_TYPE = 0;
			}
		;

c_type		:  c_tp c_decl	{ $$ = Ncast($1,$<pn>2); }
		;

new_type	:  type new_decl	{ $$ = Ncast($1,$<pn>2); };

arg_type	:  type arg_decl
			{	
                         //      ENTER_NAME($<pn>2);
				$$ = Ndata($1,$<pn>2); 
			}
                |  type arg_decl ASSIGN
                        {
                        //      ENTER_NAME($<pn>2);
                        }
                   initializer
                        {       $$ = Ndata($1,$<pn>2);
                                $<pn>$->n_initializer = $<pe>5;
                        }
                ;

arg_lp		:  LP 
			{//PUSH_ARG_SCOPE 
				//SAVE_STATE();
				check_decl(); 
				in_arg_list=1; 
				$$ = modified_tn;
				modified_tn = 0;
			}
		;

arg_list	:  arg_lp arg_type_list ellipsis_opt RP fct_attributes
		    {
			$$ = new fct(0,name_unlist($<nl>2),$<t>3); 
			if ( NEXTTOK() != COLON ) in_arg_list=0;
			//in_arg_list=0;
			Pfct($<pt>$)->f_const = ($<i>5 & 1);
			if ( parsing_class_members
			||   (NEXTTOK()!=LC && yychar!=COLON)) {
				end_al($1,1);
                        //POP_SCOPE(); // similar to end_al()
			} else
				end_al($1,0);
			//RESTORE_STATE();
		    }
		;

arg_type_list	:  arg_type_list CM at
			{
				if ($3)
					if ($1)
						$<nl>1->add($<pn>3);
					else {
						error("AD syntax");
						$<nl>$ = new nlist($<pn>3); 
					}
				else
					error("AD syntax");
			}
		|  at	%prec CM
			{
				if ($1) $<nl>$ = new nlist($<pn>1); 
			}
		;

at		:  arg_type
		|  %prec EMPTY	{	$$ = 0; }
		;

ellipsis_opt	:  /* empty */
		    {	$$ = 1; }
		|  ELLIPSIS
		    {	$$ = ELLIPSIS; }
		|  CM ELLIPSIS
		    {	$$ = ELLIPSIS; }
		;

ptr		:  MUL %prec NO_ID
			{
			$$ = new ptr(PTR,0); 
			EXPECT_ID();
			}
		|  AND %prec NO_ID
			{
			$$ = new ptr(RPTR,0); 
			EXPECT_ID();
			}
		|  MUL TYPE %prec NO_ID
			{	$$ = doptr(PTR,$<t>2); }
		|  ptr TYPE %prec NO_ID
			{	
				switch ( $<t>2 ) {
				case CONST:
                                     $<pp>1->rdo = 1; break;
				case VOLATILE:
				     error('w',"\"volatile\" not implemented (ignored)");
				     break;
				default:
				    error( "syntax error: *%k", $<t>2 );
				}
				$$ = $<pp>1; 
			}
		|  AND TYPE %prec NO_ID
			{	$$ = doptr(RPTR,$<t>2); }
		|  ptname MEMPTR %prec NO_ID
			{	goto memptr1;  }
		|  MEMPTR %prec NO_ID
			{
			memptr1:
			$$ = new ptr(PTR,0);
			$<pp>$->memof = Pclass(Pbase($<pn>1->tp)->b_name->tp);
			EXPECT_ID();
			}
		|  ptname MEMPTR TYPE %prec NO_ID
			{	
				$<t>2 = $<t>3;
				goto memptr2;  
			}
		|  MEMPTR TYPE %prec NO_ID
			{	
				memptr2:
				$$ = doptr(PTR,$<t>2);
				$<pp>$->memof = Pclass(Pbase($<pn>1->tp)->b_name->tp);
			}
		;

vec		:  LB e RB		{ $$ = new vec(0,$<pe>2!=dummy?$<pe>2:0 ); }
		|  NOT %prec LB		{ $$ = new vec(0,0); }
		;

%%

static void
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

static void
end_al( Plist mtn, int rst )
// unhide type names hidden by arg names
// mtn == saved modified_tn
{
	if ( rst == 0 ) {
		// not really an arg list, or we are entering a function def
		// merge modified_tn and don't restore
		if ( modified_tn == 0 ) modified_tn = mtn;
		else {
			for ( Plist l = modified_tn; l->l; l = l->l ) ;
			l->l = mtn;
		}
	} else {
		restore();
		modified_tn = mtn;
	}
}
static void
arg_redec( Pname fn )
{
	if ( fn==0 || fn->tp->base != FCT )
		error('i',"bad inline rewrite!");
	Pname al = Pfct(fn->tp)->argtype;
	Pname n = 0;
	for ( ;  al;  al = al->n_list ) {
		DB( if(Ydebug>=1)error('d',"arg_redec:  %n  %d",al,al->lex_level); );
		// nested function args should have lex_level >= 1
		if ( al->lex_level==1 && (n=ktbl->look(al->string,0)) )
			n->hide();
		else if ( al->lex_level>1 && (n=ktbl->look(al->string,LOCAL)) )
			n->hide();
		DB( if(Ydebug>=1)error('d',"   %n",n); );
	}
}


