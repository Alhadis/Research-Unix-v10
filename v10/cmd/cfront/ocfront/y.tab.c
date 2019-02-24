
# line 24 "gram.y"
#include "cfront.h"
#include "size.h"
// include tqueue.h after YYSTYPE is defined ...

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
extern char *class_typedef;

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
#undef NESTED

# line 201 "gram.y"
typedef union  {
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
} YYSTYPE;

# line 221 "gram.y"
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

# define EOFTOK 0
# define ASM 1
# define BREAK 3
# define CASE 4
# define CONTINUE 7
# define DEFAULT 8
# define DELETE 9
# define DO 10
# define ELSE 12
# define ENUM 13
# define FOR 16
# define FORTRAN 17
# define GOTO 19
# define IF 20
# define NEW 23
# define OPERATOR 24
# define RETURN 28
# define SIZEOF 30
# define SWITCH 33
# define THIS 34
# define WHILE 39
# define LP 40
# define RP 41
# define LB 42
# define RB 43
# define REF 44
# define DOT 45
# define NOT 46
# define COMPL 47
# define MUL 50
# define AND 52
# define PLUS 54
# define MINUS 55
# define ER 64
# define OR 65
# define ANDAND 66
# define OROR 67
# define QUEST 68
# define COLON 69
# define ASSIGN 70
# define CM 71
# define SM 72
# define LC 73
# define RC 74
# define MEMPTR 173
# define ID 80
# define STRING 81
# define ICON 82
# define FCON 83
# define CCON 84
# define ZERO 86
# define ASOP 90
# define RELOP 91
# define EQUOP 92
# define DIVOP 93
# define SHIFTOP 94
# define ICOP 95
# define TYPE 97
# define TNAME 123
# define EMPTY 124
# define NO_ID 125
# define NO_EXPR 126
# define FDEF 127
# define ELLIPSIS 155
# define AGGR 156
# define MEM 160
# define PR 175
# define TSCOPE 178
# define DECL_MARKER 179
# define REFMUL 180
# define LDOUBLE 181
# define LINKAGE 182
# define LOCAL 183
# define NESTED 190
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1947 "gram.y"


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


short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 40,
	71, 31,
	72, 31,
	-2, 30,
-1, 57,
	155, 307,
	-2, 245,
-1, 62,
	13, 302,
	97, 302,
	123, 302,
	155, 302,
	156, 302,
	160, 302,
	178, 302,
	179, 302,
	182, 302,
	-2, 245,
-1, 111,
	73, 69,
	-2, 64,
-1, 346,
	1, 164,
	3, 164,
	4, 164,
	7, 164,
	8, 164,
	9, 164,
	10, 164,
	13, 164,
	16, 164,
	19, 164,
	20, 164,
	23, 164,
	24, 164,
	28, 164,
	30, 164,
	33, 164,
	34, 164,
	39, 164,
	40, 164,
	46, 164,
	47, 164,
	50, 164,
	52, 164,
	54, 164,
	55, 164,
	72, 164,
	73, 164,
	80, 164,
	81, 164,
	82, 164,
	83, 164,
	84, 164,
	86, 164,
	95, 164,
	97, 164,
	123, 164,
	156, 164,
	160, 164,
	178, 164,
	179, 164,
	182, 164,
	-2, 0,
	};
# define YYNPROD 320
# define YYLAST 2463
short yyact[]={

 129, 423,   8, 461, 165, 345, 159,   7, 238, 391,
 266,  58, 262, 291, 333, 336, 273, 201, 168, 169,
 388,  55, 140, 200, 188, 292, 131,  63, 400, 142,
 236, 114, 232,  16, 233, 235,  28,  61,  35,  77,
  68,  78,  76, 240,  12,  22, 269,  27, 294,  23,
 158,  50,  69,   9, 156,  67,  63, 348,  45, 254,
 111, 118,  46,  32,  19, 154,  61, 177, 109, 108,
  26,  10, 155,  29,  25,  30,  29,  37,  30, 446,
 107, 114, 255,  69,  63, 231,  67,  63, 473,  27,
 338, 521,  51,  27,  61,  36,  66,  61, 160, 180,
  29, 171,  30,  48,  68,  29, 126,  30, 171,  32,
 120,  37,  26, 193, 175, 253,  26, 192, 265,  29,
 213,  30,  21,  73,  25, 486, 293, 202, 226,  36,
 270, 479, 241, 242, 243, 244, 245, 246, 247, 248,
 249,  37, 206,  67, 346,  70,  25,  73,  25,  17,
 424, 473, 186,  27,  12, 471, 472, 237,  68,  36,
 239,  37, 261, 219, 194, 160, 250, 258, 189, 413,
 234, 344, 205, 301, 302, 207,  26, 242,  25,  36,
 209,  34, 195,  37, 289,  37, 252,  67,  68, 280,
 265, 256,  25, 257, 479,  37,  31, 251, 203,  31,
  67,  36, 389,  36,  24,  38,  45, 161, 460,  70,
  46,  67, 492,  36, 283,  37, 282,  45, 300, 296,
  35,  46, 370,  31, 362,  37, 299, 298,  31,  37,
 320, 288, 286,  36,  67, 328, 275, 296, 338, 323,
 295, 331,  31,  36, 332, 263, 264,  36,  29, 285,
  30, 241,  68, 260, 189, 414, 326, 322, 329, 363,
 324, 123, 124, 342, 343, 303, 304, 305, 306, 307,
 308, 309, 310, 311, 312, 313, 314, 315, 316, 317,
 318, 341, 319, 252, 271, 353,  45, 325, 350,  37,
  46, 352, 357, 358, 339, 354, 340, 297,  45, 469,
 467,  27,  46, 466,  21, 334, 371,  36,  66, 265,
  27,  65, 287, 171, 368,  32, 199, 337, 213, 364,
 366, 182, 211, 212,  26,  29,  32,  30,  65, 375,
  25, 185, 392,  26, 534, 125,  29,  32,  30,  59,
 275, 275, 455,  60,  62, 379,  59, 427, 381, 380,
  60, 116, 369, 383, 384, 274, 407, 181, 114, 386,
 394, 219, 351,  34, 385, 378,  17,  37, 252, 353,
 353,  31, 114, 393, 405, 395, 374, 401, 377, 354,
 354, 397, 398,  73, 359,  36,  24, 409, 254, 161,
 349, 402, 379, 403, 151, 381, 381, 415,  56, 334,
  59,  25, 337, 337,  60, 408, 279, 418, 410,  25,
  59, 255, 411,  59,  60,  25, 420,  60, 114, 267,
 204, 428, 184, 422, 426, 164, 454, 404, 436, 462,
 429, 437, 435, 421, 419, 456, 488, 410,  37, 464,
 393, 416,  32, 463, 253, 465,  37, 259,  31,  54,
 149, 258,  37, 489, 252,  39,  36, 119, 533,  31,
 230,  25, 525, 495,  36, 497, 494, 499, 503,  12,
  36, 454, 454, 454, 454, 454, 454, 454, 454, 454,
 454, 454, 454, 454, 454, 454, 454,  68, 459, 520,
 121,  35, 523,  25,  64,  20, 382,  59,  37, 468,
 527,  60, 528, 529, 491, 531, 501,  20,  57, 500,
 498, 458, 229, 367,  20,  20,  36, 496, 230,  32,
 396,  59, 457, 502,  37,  60, 230, 537, 487, 387,
  37, 361, 539, 454, 473,  57, 542, 541, 471, 472,
 113, 284,  36,  66, 545, 522, 347, 114,  36, 548,
 504, 505, 506, 507, 508, 509, 510, 511, 512, 513,
 514, 515, 516, 517, 518, 519,  57, 153,  15,  57,
  32, 417,  59, 290, 198,  21,  60, 479, 477, 536,
  44, 114, 538,  56, 360,  59, 399,  72,  75,  60,
  25, 268, 473, 150,  18, 105, 471, 472, 365, 546,
 355,  25, 115, 115, 166,  20,  49, 170, 103, 141,
 167, 431, 540,  49,  49,  35, 430,  56,  20,  59,
 425, 106, 162,  60,  25, 173,  27,  37, 106, 106,
 412,  41, 162, 480,  34, 479, 477, 191,  37, 236,
 110, 232,  32, 233, 235,  36, 112, 122, 163,  26,
  33, 174,  29,  35,  30,  20,  36,  24, 163, 117,
 161,  37,  14, 213,  27,   1, 276, 211, 212, 390,
   2, 187,   0,   0,  20,  13, 190,   0,  15,  36,
  32,   0,  17,   0,   0,   0,   0,  26,  40, 162,
  29,  75,  30,   0, 231,  71,  74, 106, 162,  21,
   0,   0,   0,  57, 197,   0, 219, 217,   0,  51,
 173,  35,  42,   0,   0, 163,   0,  49,   0, 105,
  48,  56,   0,  59, 163,  25,   0,  60,  44,  56,
 183,  59,  32,   0,   0,  60, 174,  43,  32, 272,
   0,   0,  29,   0,  30, 106,   0,  75,  29,   0,
  30,  52,   0,  53,  49,   0,   0,   0,  34,   0,
   0,   0,  37,  25,   0, 106,   0, 115,   0,   0,
 276, 276, 274,  49,   0,  31,  57, 196, 274,   0,
  36,  24,   0,   0, 161,   0, 473,   0, 474,  73,
 471, 472,   0, 106,   0,  43,  34,   0, 162, 208,
  37, 327,   0, 330,   0,   0,   0, 173, 272,   0,
   0,   0,   0,  31,   0,  25,   0,   0,  36,  47,
 272,  25, 105, 115, 163,   0,   0, 480, 478, 479,
 477,   0,   0, 174, 106,   0, 208, 162,   0,   0,
   0,   0,   0, 356,   0,   0, 106,   0, 106, 106,
   0,   0,  37,   0,  34, 281,   0,   0,  37,   0,
   0, 115, 115, 163,   0,  31,   0,  99,   0, 106,
  36,  31,   0,   0,   0,   0,  36,  47,   0,   0,
   0,  98,   0,   0,   0,   0,   0, 106, 106,   0,
   0,   0, 173,   0,   0,   0,   0,   0,  91,   0,
  92,   0, 100,   0,  93,  94,   0,   0,  81,   0,
  82,   0,  79,  80,   0, 115,   0,   0, 174,   0,
   0,   0,  84,  83,  89,  90,   0,   0,  97, 101,
   0, 115,   0, 115,   0, 115,   0,   0,   0,   0,
   0, 106,   0,   0,   0,   0, 162,   0,  96,  88,
  86,  87,  85,  95,   0, 104,   0, 106,   0, 106,
   0, 106,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 163,   0,   0, 162,   0,   0, 162,   0,
   0,  25, 115,   0,   0,   0,   0,   0, 451,   0,
 447, 444,   0, 173, 448, 445, 452, 450,   0,   0,
  35, 163,   0, 440, 163,   0, 449, 438, 106,   0,
 130, 151,   0,   0,   0, 433,   0, 139,  37, 174,
 441, 148,   0,   0,   0,   0, 439, 154,   0,   0,
   0,   0,   0, 136, 137,   0,  36, 132, 102, 133,
   0, 135, 134,   0,   0,   0,   0, 473,   0, 474,
   0, 471, 472, 213,   0, 214,   0, 211, 212, 432,
 346, 476, 127,   0,   0,   0,  35, 442, 146, 144,
 145, 147,   0, 143,   0,   0, 130, 151,   0,   0,
   0,   0, 138, 139, 434,   0,   0, 148, 480, 478,
 479, 477,   0, 154, 220, 218, 219, 217,   0, 136,
 137,   0,   0, 132,   0, 133,   0, 135, 134,   0,
 443, 213,   0,   0,   0, 211, 212, 473,   0, 474,
   0, 471, 472,   0,   0,   0, 157,   0,   0,   0,
   0, 476, 475, 149, 146, 144, 145, 147,   0, 143,
   0,   0,   0,  34,   0,   0,   0, 453, 138,   0,
 172,   0, 220, 127, 219, 217,   0,  35, 480, 478,
 479, 477,   0,   0,   0,  36,  24, 130, 151, 161,
   0,   0,   0,   0, 139,   0,  25,   0, 148,   0,
   0,   0,   0,   0, 154,   0,   0,   0,   0,   0,
 136, 137,   0,   0, 132,   0, 133,   0, 135, 134,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  34,
 473,   0,   0, 128, 471, 472,   0,   0,   0, 127,
   0,   0,   0,   0, 149, 146, 144, 145, 147,   0,
 143,  36,  24, 130, 151, 161,   0,   0, 213, 138,
 139, 172, 211, 212, 148,   0,   0,   0,   0,   0,
 154, 480, 478, 479, 477,   0, 136, 137,  35,   0,
 132,   0, 133,   0, 135, 134,   0,  25,   0,   0,
   0,   0, 473,   0, 474,   0, 471, 472,   0, 220,
 218, 219, 217, 157,   0, 338, 476, 475, 481,   0,
 149, 146, 144, 145, 147,  29, 143,  30,   0,   0,
  34,   0,   0, 127, 128, 138,   0, 152,   0,   0,
   0,   0,   0, 480, 478, 479, 477, 130, 151,   0,
   0,   0,  36,  24, 139,   0, 161,   0, 148,   0,
   0,   0,   0,  25, 154,   0,   0,   0,   0,   0,
 136, 137,  43,   0, 132,   0, 133,   0, 135, 134,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  25, 452,
 128,   0,   0,   0, 149, 146, 144, 145, 147,   0,
 143,   0,   0, 130, 151,   0,   0,   0,  36, 138,
 139, 152,   0,   0, 148,   0,   0,   0,  35,   0,
 154,  34, 130, 151,   0,  37, 136, 137,   0, 139,
 132,   0, 133, 148, 135, 134,   0,  25,  31, 154,
   0, 524,   0,  36,  47, 136, 137,   0,   0, 132,
   0, 133,   0, 135, 134,  29,   0,  30,   0,   0,
 149, 146, 144, 145, 147,   0, 143,   0,   0,   0,
   0,   0,   0,   0, 128, 138,   0, 152,   0, 149,
 146, 144, 145, 147,   0, 143,   0,   0, 130, 151,
   0,   0,  36,   0, 138, 139, 152,   0,   0, 148,
   0,   0,  43,  25,   0, 154,   0, 490,   0,   0,
   0, 136, 137,   0,   0, 132,   0, 133,   0, 135,
 134,   0,  25,   0,   0, 213,   0, 214,  25, 211,
 212,   0,   0,   0,   0,   0,   0,   0,   0, 216,
 453,   0,   0,   0,   0, 149, 146, 144, 145, 147,
   0, 143,   0,   0,   0,   0,   0,   0,  36, 228,
 138,  34, 152, 130, 151,  37, 220, 218, 219, 217,
 139,   0,   0,   0, 148,   0,   0,  36,  31,   0,
 154,   0, 321,  36,  47,   0, 136, 137,  25,   0,
 132,   0, 133,   0, 135, 134,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   4,  11,   0,
 149, 146, 144, 145, 147, 228, 143, 130, 151,   0,
  35,   0,   0,   0, 139, 138,   0, 152, 148,   0,
   0,  27,   0,  36, 154,   0, 227,   0,   0,   0,
 136, 137,   0,   0, 132,   0, 133,  32, 135, 134,
   0,   0,   0,  25,  26,   0,   0,  29,   0,  30,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 149, 146, 144, 145, 147,   3,
 143,   6,   0,   0,   0,   0,   0,  17,   0, 138,
 228, 152, 130, 151,   0,   0, 473,   0, 474, 139,
 471, 472,   0, 148,  21,   0,   0,   0,  36, 154,
 476, 475, 481, 482, 485, 136, 137,  25,   0, 132,
   0, 133,   0, 135, 134,   0,   0, 213,   0, 214,
  25, 211, 212,   0,   0,   0,   0, 480, 478, 479,
 477, 216, 215, 221, 222, 225,   0,   0,   0, 149,
 146, 144, 145, 147, 228, 143,   0,   0,   0,   0,
   0,   0,   0,  34, 138,   0, 152,  37, 220, 218,
 219, 217,  36,   0,   0,   0, 130, 151,   0,   0,
  31,   0,   0, 139,   0,  36,  24, 148,   0,   5,
   0,   0,  25, 154,   0, 130, 151,   0,   0, 136,
 137,   0, 139, 132,   0, 133, 148, 135, 134,   0,
   0,   0, 154,   0,   0,   0,   0,   0, 136, 137,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 228,
   0,   0,   0, 278, 146, 144, 145, 147,   0, 143,
   0,   0,   0,   0,   0,   0,   0,  36, 138,   0,
 152,   0, 149, 146, 144, 145, 147, 213, 143, 214,
   0, 211, 212,   0,   0, 127,   0, 138,   0, 152,
   0, 216, 215, 221,   0,   0,  25,   0,   0, 130,
 151,   0,   0,   0,   0,   0, 139,   0,   0,   0,
 148,   0,   0,   0,   0,  25, 154, 178, 220, 218,
 219, 217, 136, 137,   0,   0, 176,   0, 133,   0,
 135, 134, 213, 228, 214,   0, 211, 212,   0,   0,
   0,   0,   0,   0,   0,   0, 216, 215,   0, 157,
   0,  36, 228,   0, 547,   0, 149, 146, 144, 145,
 147,   0, 143, 213,   0, 214,   0, 211, 212,   0,
  36, 138,   0, 220, 218, 219, 217, 216, 215, 221,
 222, 225,   0, 210, 224,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 213,   0, 214,   0, 211, 212,
   0,   0,   0, 223, 220, 218, 219, 217, 216, 215,
 221, 222, 225,   0, 210, 224, 544,   0, 543,   0,
   0,   0,   0,   0,   0, 213,   0, 214,   0, 211,
 212,   0,   0,   0, 223, 220, 218, 219, 217, 216,
 215, 221, 222, 225,   0, 210, 224,   0,   0, 179,
 535,   0,   0,   0,   0,   0,   0, 213,   0, 214,
   0, 211, 212,   0,   0, 223, 220, 218, 219, 217,
   0, 216, 215, 221, 222, 225,   0, 210, 224,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 473,   0,
 474,   0, 471, 472,   0,   0,   0, 223, 220, 218,
 219, 217, 476, 475, 481, 482, 485, 532, 470, 484,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 213,
   0, 214,   0, 211, 212,   0,   0,   0, 483, 480,
 478, 479, 477, 216, 215, 221, 222, 225, 530, 210,
 224, 526,   0,   0,   0,   0,   0,   0,   0,   0,
 213,   0, 214,   0, 211, 212,   0,   0,   0, 223,
 220, 218, 219, 217, 216, 215, 221, 222, 225,   0,
 210, 224,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 213,   0, 214,   0, 211, 212,   0,   0,   0,
 223, 220, 218, 219, 217, 216, 215, 221, 222, 225,
   0, 210, 224, 493,   0, 406,   0,   0,   0,   0,
   0,   0, 213,   0, 214,   0, 211, 212,   0,   0,
   0, 223, 220, 218, 219, 217, 216, 215, 221, 222,
 225,   0, 210, 224,   0,   0, 376,   0,   0,   0,
   0,   0,   0, 213,   0, 214,   0, 211, 212,   0,
   0,   0, 223, 220, 218, 219, 217, 216, 215, 221,
 222, 225,   0, 210, 224,   0,   0, 373,   0,   0,
   0,   0,   0,   0, 213,   0, 214,   0, 211, 212,
   0,   0,   0, 223, 220, 218, 219, 217, 216, 215,
 221, 222, 225,   0, 210, 224,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 213,   0, 214,   0, 211,
 212,   0,   0,   0, 223, 220, 218, 219, 217, 216,
 215, 221, 222, 225, 372, 210, 224, 335,   0,   0,
   0,   0,   0,   0,   0,   0, 213,   0, 214,   0,
 211, 212,   0,   0,   0, 223, 220, 218, 219, 217,
 216, 215, 221, 222, 225,   0, 210, 224,   0,   0,
 277,   0,   0,   0,   0,   0,   0, 213,   0, 214,
   0, 211, 212,   0,   0,   0, 223, 220, 218, 219,
 217, 216, 215, 221, 222, 225,   0, 210, 224,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 473,   0,
 474,   0, 471, 472,   0,   0,   0, 223, 220, 218,
 219, 217, 476, 475, 481, 482, 485,   0, 470, 484,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 473,
   0, 474,   0, 471, 472,   0,   0,   0, 483, 480,
 478, 479, 477, 476, 475, 481, 482, 485, 213, 470,
 214,   0, 211, 212,   0,   0,   0,   0,   0,   0,
   0,   0, 216, 215, 221, 222, 225,   0, 210, 483,
 480, 478, 479, 477,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 223, 220,
 218, 219, 217 };
short yypact[]={

1597,-1000,-1000,-1000,-1000, 132,-1000,-1000,-1000,-1000,
-1000, 415, 640, 681,-1000, 304,-1000, 283, 129, 286,
  69,-1000,-1000,-1000,-1000,-1000, -81, 858,-1000, -17,
 -28, -29,-1000,-1000, 467, 278,-1000,-1000,-1000,  29,
 577, 190,-1000,-1000, 304,-1000,-1000,-1000, 266,  65,
-1000,1294,1210,-1000, 478,-1000, 384,1053,-1000,1294,
-1000,-1000,1846,-1000, 478, 277,-1000,-1000,-1000,-1000,
 276, 358, 304,-1000, 689, 304,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000, 381, 288,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,  55,-1000,-1000, -19,-1000,-1000,-1000,
 602, 247,-1000,-1000,-1000,-1000,  47, 125,-1000,-1000,
 379, 478,1210,  23,-1000,1294,2368,1584, 503, -10,
  25,1659,1659,1659,1659,1659,1659,1659,1659,1659,
-1000,-1000,1144,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
  35, 858, 411, 407,-1000, 181,2368,1210, 121,-1000,
 640,-1000,-1000, -83,-1000, 378, -25, 213,-1000,-1000,
-1000, 698, 411, 407, 364,2287,1743, 365,-1000,  69,
-1000,-1000, 277,-1000,-1000,-1000,-1000,-1000,-1000,  26,
-1000,-1000,-1000, 177, 160,-1000, 243, 129, 111, -49,
 166,-1000, 227,  47, 154,  49,-1000,-1000, 577,2368,
1294,1294,1294,1294,1294,1294,1294,1294,1294,1294,
1294,1294,1294,1294,1294,1294, -10,1294, 437,1520,
  25,-1000,1294, 370,1659, 370,1210,-1000,-1000,1053,
1385, 599, -10, -10, -10, -10, 599, 599, 599, 599,
1762,2256,1245,-1000, 858,   1,-1000,-1000,1210,1210,
-1000,  97,  71,-1000,-1000,-1000, -40,-1000, 349, 207,
-1000,1210,-1000, 530,-1000, 692, 275,-1000, 343,-1000,
 -40, 543,-1000,-1000,-1000,-1000,-1000,-1000, 152,-1000,
 188,-1000,-1000, 501, 338,-1000,  47,1294, 148,-1000,
  71,-1000,-1000,2368,  70,  70,-1000,1188,1455,1003,
 268,1061,-1000, 613,1852,1797,2368,2368,2225,2194,
 -10,1294,-1000,-1000,1053,2163,-1000,-1000, 599,-1000,
-1000, 337, 324, 368,  26,-1000, 455,  50, 198,-1000,
-1000,-1000, 323, 318,-1000,-1000, -54, 292,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,1210,-1000, 297, 479, 402,
 402, -99,-1000, -49,-1000,   1,-1000,   1,-1000,2368,
-1000,-1000,1294,1659,2132, 315,-1000,-1000,  25,-1000,
 368,-1000,-1000, 368, 371,-1000,-1000,-1000,-1000,  95,
 184,-1000,1210, 401, -40,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,1667, -10,1659,  25,-1000,-1000,
 478, 297,  76,-1000, 292, 306,1210, 240, -10,-1000,
-1000,-1000,-1000,-1000,-1000, 987,-1000,-1000, 301,  71,
-1000,-1000,-1000,1294, 127,-1000,-1000,-1000, 389, 389,
 399, 389, 234, 231,-1000, 230,2318,-1000,-1000,  45,
-1000, 396,1445, 495, -10,-1000,-1000,-1000, 140,2101,
  71,-1000,1294,-1000,-1000,-1000,-1000,-1000,1294,-1000,
1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,
1360,1360,1360,1360,1360,1360,-1000,-1000,  10, -10,
1294,1379,-1000,-1000,-1000, 450,2070,-1000,-1000,-1000,
-1000,-1000,2039,-1000,2349,  38,  38,-1000,1160, 997,
 736, 101, 542,-1000, 484,1067,1222,2349,2349,2008,
 419, 293,1977, -10,1294,-1000,-1000,1294,-1000,-1000,
-1000,-1000,1360, 389,-1000,1659,1945,-1000,1914,-1000,
1636,-1000, -10,1659,1294, -10,1883,-1000,-1000 };
short yypgo[]={

   0, 670,  71,  53,   2, 449,  12, 669,   9,   6,
 665,  11,  64,  43, 662,  49,  23,  17,  25, 659,
 650,  45, 640, 637,  51, 631,  33, 675,  19, 630,
   1, 620,   5, 616, 611, 610,   4,  54,  79,   0,
  22, 609,  15,  26,  24,  41, 608,  16,  18, 607,
  21, 604,  14,   8,   3,  36, 593, 567,  39, 591,
 586, 574, 573,  13,  10, 494,  50, 571, 546, 541,
 531, 529, 528, 522, 511, 510, 509, 506, 499, 468,
  29 };
short yyr1[]={

   0,  10,  10,  10,  10,  10,   1,   1,   1,   1,
   1,   2,   2,   4,   4,   4,   3,   3,   3,  67,
  60,  66,  68,   6,   6,   7,   7,   8,   8,   5,
   5,  24,  24,  24,  24,  25,  25,   9,   9,  14,
  14,  14,  14,  14,  14,  14,  13,  13,  13,  13,
  13,  13,  19,  19,  15,  15,  15,  16,  16,  17,
  17,  17,  69,  21,  21,  21,  70,  70,  61,  61,
  62,  62,  63,  63,  63,  63,  63,  20,  20,  18,
  18,  22,  22,  23,  23,  23,  23,  23,  23,  23,
  23,  26,  26,  26,  26,  58,  58,  58,  58,  58,
  58,  58,  58,  58,  58,  58,  58,  58,  58,  58,
  58,  58,  58,  58,  58,  58,  58,  58,  58,  56,
  56,  57,  57,  64,  64,  27,  27,  27,  27,  27,
  27,  27,  27,  27,  27,  27,  27,  27,  27,  27,
  27,  27,  47,  47,  47,  47,  47,  47,  47,  52,
  52,  52,  42,  42,  42,  42,  42,  44,  44,  29,
  29,  30,  31,  54,  71,  32,  32,  32,  34,  34,
  34,  34,  72,  34,  34,  74,  73,  33,  33,  33,
  33,  33,  33,  33,  33,  33,  33,  75,  33,  33,
  76,  33,  77,  33,  78,  33,  79,  33,  36,  35,
  35,  28,  28,  38,  38,  38,  38,  38,  38,  38,
  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
  38,  38,  38,  38,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  37,  39,  39,  39,  39,
  39,  39,  39,  39,  39,  39,  39,  39,  39,  39,
  39,  39,  39,  39,  39,  39,  39,  39,  39,  39,
  39,  39,  39,  39,  39,  39,  41,  41,  41,  41,
  41,  41,  41,  55,  55,  40,  40,  40,  40,  40,
  40,  40,  43,  80,  46,  46,  46,  46,  45,  53,
  49,  49,  65,  50,  51,  51,  48,  48,  59,  59,
  59,  12,  12,  12,  12,  12,  12,  12,  11,  11 };
short yyr2[]={

   0,   1,   1,   1,   2,   1,   1,   1,   1,   1,
   5,   4,   2,   6,   5,   5,   5,   4,   4,   0,
   4,   0,   0,   3,   0,   1,   3,   3,   4,   2,
   0,   1,   3,   2,   3,   1,   3,   3,   2,   1,
   1,   1,   2,   1,   1,   1,   1,   2,   2,   2,
   2,   2,   1,   1,   4,   5,   2,   1,   3,   1,
   3,   0,   0,   5,   2,   2,   0,   2,   2,   0,
   1,   3,   1,   2,   2,   3,   3,   2,   4,   1,
   1,   2,   0,   1,   2,   2,   1,   1,   1,   2,
   3,   1,   2,   2,   2,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   2,   2,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   2,   2,   1,   0,   2,   2,   4,   2,   4,   6,
   4,   4,   6,   1,   3,   2,   4,   2,   2,   2,
   2,   3,   1,   2,   0,   2,   2,   2,   3,   0,
   2,   2,   0,   2,   2,   4,   4,   0,   2,   0,
   2,   2,   0,   3,   0,   4,   2,   3,   1,   1,
   1,   2,   0,   5,   4,   0,   2,   2,   1,   3,
   3,   1,   1,   1,   3,   5,   3,   0,   9,   3,
   0,   4,   0,   4,   0,   5,   0,   4,   1,   1,
   3,   1,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   5,   2,
   5,   3,   6,   1,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   5,
   2,   5,   3,   6,   1,   0,   2,   2,   3,   3,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   4,   3,   3,   3,   3,   3,   1,   1,   3,
   1,   1,   1,   1,   1,   1,   4,   4,   5,   5,
   6,   6,   4,   1,   1,   1,   2,   2,   3,   2,
   3,   3,   4,   1,   1,   1,   2,   2,   2,   2,
   2,   4,   1,   5,   3,   1,   1,   0,   0,   1,
   2,   1,   1,   2,   2,   2,   1,   2,   3,   1 };
short yychk[]={

-1000, -10,  -1,  72,   0, 182,  74,  -9,  -4,  -3,
  -2,   1, -13, -27, -14, -57, -26,  80, -56, -12,
 -65,  97, -21, -15, 179, 123,  47,  24, -55,  50,
  52, 173,  40, -20, 156,  13, 178, 160,  73,  40,
 -27, -25,  72,  97, -57, -21, -15, 179,  80, -56,
 -24,  69,  70,  72,  -5, -50,  40, -65, -11,  42,
  46, -50,  40, -11, -65,  45, 179, -55, 123, -26,
  80, -27, -57,  97, -27, -57, 123, -58, -45,  54,
  55,  50,  52,  65,  64,  94,  92,  93,  91,  66,
  67,  40,  42,  46,  47,  95,  90,  70,  23,   9,
  44,  71, 180, -46,  97, -57, -56,  97,  97,  97,
 -22, -18, 179,  73,  80, -57,  73, -19, -18, 179,
  81,  -5,  70,  71,  72,  69, -37,   9, 160, -39,
  23, -43,  50,  52,  55,  54,  46,  47,  95,  30,
 -40, -41, -80,  86,  82,  83,  81,  84,  34,  80,
 -56,  24,  97, -57,  40, -28, -37,  73, -66,  -9,
 -13, 182, -57, -56,  41, -36, -51, -35, -48, -28,
 -49, -13,  97, -57, -56, -37,  50, -36,  41, 173,
 -26,  80,  45,  41,  41,  43,  97, -57, -44, -12,
  74, -23,  -9,  -4,  -3,  -2, 175, -56, -61,  69,
 -16, -17,  80,  73,  41, -66, -28, -24, -27, -37,
  70,  54,  55,  50,  52,  65,  64,  94,  92,  93,
  91,  66,  67,  90,  71,  68, -39,  42, 160,   9,
  23,  95,  42,  44, 180,  45,  40, -43, -53, -80,
 -13, -39, -39, -39, -39, -39, -39, -39, -39, -39,
 -43, -37, -13,  80,  24,  47, -58, -45,  40,  40,
  72, -36,  -6, 124, 125,  69, -64,  41, -59,  71,
 155,  71, -57, -47,  80, -12, -65,  43,  80,  41,
 -64, -27, -26, -44, -69,  72,  72,  69, -26,  73,
 -62, -63, -18, 175,  97,  74,  71,  70, -16,  72,
  -6, 124, 125, -37, -37, -37, -37, -37, -37, -37,
 -37, -37, -37, -37, -37, -37, -37, -37, -37, -37,
 -39,  42, -43, -53, -80, -37, -40, -57, -39, -40,
 -57, -36, -36, -52, -12,  41, -42, -12,  40, -58,
 -45, -18, -36, -36,  74, -32,  73, -68,  97,  41,
 -48, 155, -28, -11, -50,  70, -57, -47, -47,  41,
  41, -70,  72,  71, -18,  97, -18, 175, -17, -37,
  74, -32,  69,  43, -37, -36,  43,  41,  41, -11,
 -52, -11,  41, -42, -42,  41,  41, -71,  74, 256,
  -7,  -8,  40, -18, -64, -28,  41, -50, -50, -60,
 127, -63, -18, -18, -37, -39,  43,  41, -43, -53,
 -80,  41, -29,  74,  71, -36,  40, -67, -39, -43,
 -53, -50, -11, -30,  74, -31,  -8,  41, -36,  -6,
 -33, -34,  72,  28,  97,  -9,  -4, -32,  20,  39,
  16,  33,  80, 123,   4,   8, -38,   3,   7,  19,
  10,   1,   9, 160, -39,  41, -32, -73, -74, -37,
  81, -54,  40, -54,  40, -54,  69,  69, -78,  69,
  70,  54,  55,  50,  52,  65,  64,  94,  92,  93,
  91,  66,  67,  90,  71,  68,  80, -72,  40, -39,
  42,   9,  72,  72, -32, -30, -37, -30, -75, -30,
 -76, -77, -37, -79, -38, -38, -38, -38, -38, -38,
 -38, -38, -38, -38, -38, -38, -38, -38, -38, -38,
 -30,  81, -37, -39,  42,  12,  41, -30, -30, -30,
  69, -30,  69,  39,  41,  43, -37, -30, -37, -30,
 -38, -54, -39,  43,  72, -39, -37,  41, -30 };
short yydef[]={

   0,  -2,   1,   2,   3,  40,   5,   6,   7,   8,
   9,   0,   0,  30,  46,  41, 133,  91,   0,   0,
   0,  39,  43,  44,  45, 122,   0,   0, 119, 311,
 312, 316, 302,  82,   0,   0, 283, 284,   4,   0,
  -2,   0,  38,  47,  48,  49,  50,  51,  91,   0,
  35, 245, 245,  12,  21, 125, 302,  -2, 140, 245,
 319, 127,  -2, 139, 307,   0,  42, 120, 121, 135,
  91, 137, 138, 314,   0,   0,  92,  93,  94,  95,
  96,  97,  98,  99, 100, 101, 102, 103, 104, 105,
 106,   0,   0, 109, 110, 111, 112, 113, 114, 115,
 116, 117, 118, 157, 294, 295,   0, 313, 315, 317,
   0,  -2,  65,  77,  79,  80,  61,  56,  52,  53,
   0,  21, 245,   0,  37, 245,  33,   0, 284, 244,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 267, 268, 245, 270, 271, 272, 273, 274, 275, 285,
   0,   0,   0,   0, 293,   0, 201, 245,  24,  29,
   0,  40,  41,   0, 123,   0, 308, 198, 305, 199,
 306, 144,  39,  41,   0,   0,   0,   0, 123,   0,
 134,  91,   0, 141, 107, 108, 296, 297, 298, 157,
  62,  81,  83,  87,  86,  88,   0,   0,   0,   0,
   0,  57,  59,  61,   0,  24,  34,  36,  31,  32,
 245, 245, 245, 245, 245, 245, 245, 245, 245, 245,
 245, 245, 245, 245, 245, 245, 240, 245, 284,   0,
   0, 250, 245,   0,   0,   0, 245, 246, 247, 245,
 149, 251, 252, 253, 254, 255, 256, 257, 258, 259,
 260,   0, 152, 286,   0,   0, 287, 289, 245, 245,
  11,   0,   0,  17,  18,  22, 126, 128,   0, 307,
 309, 245,  48, 300, 142, 144, 144, 318, 285, 130,
 131,   0, 136, 158,  66,  84,  85,  89, 135,  78,
  68,  70,  72,   0,   0,  54,  61, 245,   0,  10,
   0,  14,  15, 224, 225, 226, 227, 228, 229, 230,
 231, 232, 233, 234, 235, 236, 237, 238,   0,   0,
 242, 245, 248, 249, 245,   0, 262, 264, 263, 265,
 266,   0,   0, 299, 149, 269,   0, 152, 152, 288,
 290, 291,   0,   0, 202,  16,  -2,   0, 124, 123,
 304, 310, 200, 146, 147, 245, 143, 145,   0,   0,
   0,  63,  90,   0,  73,   0,  74,   0,  58,  60,
  55,  13, 245,   0,   0,   0, 261, 282,   0, 151,
 150, 154, 292, 153,   0, 276, 277, 159, 166,   0,
  23,  25, 245,   0, 303, 301, 148, 129, 132,  67,
  19,  71,  75,  76, 239, 241,   0,   0, 278, 279,
   0,   0, 162, 167,   0,   0, 245,  24, 243, 280,
 281, 155, 156, 160, 165,   0,  26,  27,   0,   0,
 161, 175, 178, 245,  39, 181, 182, 183,   0,   0,
   0,   0, 285, 122, 194,   0, 168, 169, 170,   0,
 172,   0,   0, 284, 223,  28,  20, 177,   0,   0,
   0, 162, 245, 162, 187, 162, 190, 192, 245, 196,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 171, 162,   0, 219,
 245,   0, 176, 179, 180, 184,   0, 186, 162, 189,
 162, 162,   0, 162, 203, 204, 205, 206, 207, 208,
 209, 210, 211, 212, 213, 214, 215, 216, 217,   0,
   0,   0,   0, 221, 245, 162, 163, 245, 191, 193,
 162, 197,   0,   0, 174,   0,   0, 185,   0, 195,
 218, 173, 220,   0, 245, 222,   0, 162, 188 };
# ifdef YYDEBUG
# include "y.debug"
# endif

# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT return(0)
# define YYABORT return(1)

/*	parser for yacc output	*/

#ifdef YYDEBUG
int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH]; /* where the values are stored */
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse()
{	short yys[YYMAXDEPTH];
	int yyj, yym;
	register YYSTYPE *yypvt;
	register int yystate, yyn;
	register short *yyps;
	register YYSTYPE *yypv;
	register short *yyxi;

	yystate = 0;
	yychar = -1;
	yynerrs = 0;
	yyerrflag = 0;
	yyps= &yys[-1];
	yypv= &yyv[-1];

yystack:    /* put a state and value onto the stack */
#ifdef YYDEBUG
	if(yydebug >= 3)
		if(yychar < 0 || yytoknames[yychar] == 0)
			printf("char %d in %s", yychar, yystates[yystate]);
		else
			printf("%s in %s", yytoknames[yychar], yystates[yystate]);
#endif
	if( ++yyps >= &yys[YYMAXDEPTH] ) { 
		yyerror( "yacc stack overflow" ); 
		return(1); 
	}
	*yyps = yystate;
	++yypv;
	*yypv = yyval;
yynewstate:
	yyn = yypact[yystate];
	if(yyn <= YYFLAG) goto yydefault; /* simple state */
	if(yychar<0) {
		yychar = yylex();
#ifdef YYDEBUG
		if(yydebug >= 2) {
			if(yychar <= 0)
				printf("lex EOF\n");
			else if(yytoknames[yychar])
				printf("lex %s\n", yytoknames[yychar]);
			else
				printf("lex (%c)\n", yychar);
		}
#endif
		if(yychar < 0)
			yychar = 0;
	}
	if((yyn += yychar) < 0 || yyn >= YYLAST)
		goto yydefault;
	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ /* valid shift */
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
	}
yydefault:
	/* default state action */
	if( (yyn=yydef[yystate]) == -2 ) {
		if(yychar < 0) {
			yychar = yylex();
#ifdef YYDEBUG
			if(yydebug >= 2)
				if(yychar < 0)
					printf("lex EOF\n");
				else
					printf("lex %s\n", yytoknames[yychar]);
#endif
			if(yychar < 0)
				yychar = 0;
		}
		/* look through exception table */
		for(yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate);
			yyxi += 2 ) ; /* VOID */
		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
		}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
	}
	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */
		switch( yyerrflag ){
		case 0:   /* brand new error */
#ifdef YYDEBUG
			yyerror("syntax error\n%s", yystates[yystate]);
			if(yytoknames[yychar])
				yyerror("saw %s\n", yytoknames[yychar]);
			else if(yychar >= ' ' && yychar < '\177')
				yyerror("saw `%c'\n", yychar);
			else if(yychar == 0)
				yyerror("saw EOF\n");
			else
				yyerror("saw char 0%o\n", yychar);
#else
			yyerror( "syntax error" );
#endif
yyerrlab:
			++yynerrs;
		case 1:
		case 2: /* incompletely recovered error ... try again */
			yyerrflag = 3;
			/* find a state where "error" is a legal shift action */
			while ( yyps >= yys ) {
				yyn = yypact[*yyps] + YYERRCODE;
				if( yyn>= 0 && yyn < YYLAST && yychk[yyact[yyn]] == YYERRCODE ){
					yystate = yyact[yyn];  /* simulate a shift of "error" */
					goto yystack;
				}
				yyn = yypact[*yyps];
				/* the current yyps has no shift onn "error", pop stack */
#ifdef YYDEBUG
				if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
				--yyps;
				--yypv;
			}
			/* there is no state on the stack with an error shift ... abort */
yyabort:
			return(1);
		case 3:  /* no shift yet; clobber input char */
#ifdef YYDEBUG
			if( yydebug ) {
				printf("error recovery discards ");
				if(yytoknames[yychar])
					printf("%s\n", yytoknames[yychar]);
				else if(yychar >= ' ' && yychar < '\177')
					printf("`%c'\n", yychar);
				else if(yychar == 0)
					printf("EOF\n");
				else
					printf("char 0%o\n", yychar);
			}
#endif
			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */
		}
	}
	/* reduction by production yyn */
#ifdef YYDEBUG
	if(yydebug) {	char *s;
		printf("reduce %d in:\n\t", yyn);
		for(s = yystates[yystate]; *s; s++) {
			putchar(*s);
			if(*s == '\n' && *(s+1))
				putchar('\t');
		}
	}
#endif
	yyps -= yyr2[yyn];
	yypvt = yypv;
	yypv -= yyr2[yyn];
	yyval = yypv[1];
	yym=yyn;
	/* consult goto table to find next state */
	yyn = yyr1[yyn];
	yyj = yypgo[yyn] + *yyps + 1;
	if( yyj>=YYLAST || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
	switch(yym){
		
case 1:
# line 415 "gram.y"
{	return 2; } break;
case 2:
# line 416 "gram.y"
{	return 1; } break;
case 3:
# line 417 "gram.y"
{	return 0; } break;
case 4:
# line 419 "gram.y"
{
				set_linkage(yypvt[-1].s);
				bl_level--;
				return 1;
			} break;
case 5:
# line 425 "gram.y"
{
				set_linkage(0);
				bl_level++;
				return 1;
			} break;
case 6:
# line 433 "gram.y"
{	
			/* if function declartion with arguments
			 * need to make sure modified_tn is traversed */
			if ( yypvt[-0].pn != 0 
				&& yypvt[-0].pn->tp->base == FCT 
				&& Pfct(yypvt[-0].pn->tp)->nargs !=0 ) 
					goto mod;
			else {
				modified_tn = 0;
				curr_scope = 0;
				if (yypvt[-0].pn==0) yyval.i = 1; 
			     }
			} break;
case 7:
# line 447 "gram.y"
{	goto mod; } break;
case 8:
# line 449 "gram.y"
{	goto mod; } break;
case 9:
# line 451 "gram.y"
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
			} break;
case 10:
# line 469 "gram.y"
{	Pname n = new name(make_name('A'));
				n->tp = new basetype(ASM,0);
				Pbase(n->tp)->b_name = Pname(yypvt[-2].s);
				yyval.p = n;
			} break;
case 11:
# line 477 "gram.y"
{
				err_name = yypvt[-3].pn;
				if(err_name) err_name->n_initializer = yypvt[-1].pe;
				goto fix;
			} break;
case 12:
# line 483 "gram.y"
{
				Ptype t;
				err_name = yypvt[-1].pn;
			fix:
				if (err_name == 0) {
					error("syntax error:TX");
					yyval.p = Ndata(defa_type,err_name);
				}
				else if ((t=err_name->tp) == 0) {
					error("TX for%n",err_name);
					yyval.p = Ndata(defa_type,err_name);
				}
				else if (t->base==FCT) {
					if (Pfct(t)->returns==0)
						yyval.p = Nfct(defa_type,err_name,0);
					else
						yyval.p = Ndata(0,err_name);
				}
				else {
					error("syntax error:TX for%k%n",t->base,err_name);
					yyval.p = Ndata(defa_type,err_name);
				}
			} break;
case 13:
# line 509 "gram.y"
{	Pname n = Nfct(yypvt[-5].p,yypvt[-4].pn,yypvt[-0].p);
				Fargdcl(n->tp,name_unlist(yypvt[-3].nl),n);
				Finit(n->tp) = yypvt[-1].pn;
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 14:
# line 516 "gram.y"
{
				Pname n = Nfct(yypvt[-4].p,yypvt[-3].pn,dummy);
				Fargdcl(n->tp,name_unlist(yypvt[-2].nl),n);
				yypvt[-0].q->retval.pn = n;
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 15:
# line 524 "gram.y"
{
				error(&yypvt[-3].pn->where,"syntax error -- did you forget a ';'?");
				Pname n = Nfct(yypvt[-4].p,yypvt[-3].pn,0);
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 16:
# line 533 "gram.y"
{	Pname n = Nfct(defa_type,yypvt[-4].pn,yypvt[-0].p);
				Fargdcl(n->tp,name_unlist(yypvt[-3].nl),n);
				if ( yypvt[-1].pn && yypvt[-1].pn->n_list && 
     					ccl && ccl->csu == UNION )  
						error( "multiple initializers in unionK %s::%n", yypvt[-4].pn->string, yypvt[-4].pn );
				Finit(n->tp) = yypvt[-1].pn;
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 17:
# line 543 "gram.y"
{
				Pname n = Nfct(defa_type,yypvt[-3].pn,dummy);
				Fargdcl(n->tp,name_unlist(yypvt[-2].nl),n);
				yypvt[-0].q->retval.pn = n;
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 18:
# line 551 "gram.y"
{
				error(&yypvt[-3].pn->where,"badD of%n -- did you forget a ';'?",yypvt[-3].pn);
				Pname n = Nfct(defa_type,yypvt[-3].pn,0);
				yyval.p = n;
				NOT_EXPECT_ID();
			} break;
case 19:
# line 559 "gram.y"
{ arg_redec(yypvt[-0].pn); } break;
case 20:
# line 561 "gram.y"
{
				Finit(yypvt[-3].pn->tp) = yypvt[-1].pn;
				Pfct(yypvt[-3].pn->tp)->body = Pblock(yypvt[-0].p);
				yyval.pn = yypvt[-3].pn;
				NOT_EXPECT_ID();
			} break;
case 21:
# line 571 "gram.y"
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
		   } break;
case 22:
# line 599 "gram.y"
{ ++in_binit_list; } break;
case 23:
# line 600 "gram.y"
{	
				yyval.p = yypvt[-0].p; 
				in_arg_list = 0;
				--in_binit_list;
			} break;
case 24:
# line 606 "gram.y"
{	yyval.p = 0; } break;
case 25:
# line 610 "gram.y"
{ yyval.p = yypvt[-0].p; } break;
case 26:
# line 612 "gram.y"
{ yyval.pn = yypvt[-0].pn;  yyval.pn->n_list = yypvt[-2].pn; } break;
case 27:
# line 616 "gram.y"
{
				yyval.pn = new name;
				yyval.pn->n_initializer = yypvt[-1].pe;
			} break;
case 28:
# line 621 "gram.y"
{
				Pname n = Ncopy(yypvt[-3].pn);
				n->base = yypvt[-3].pn->base;
				n->tp = yypvt[-3].pn->tp;
				n->n_initializer = yypvt[-1].pe;
				yyval.pn = n;
			} break;
case 29:
# line 645 "gram.y"
{	if (yypvt[-0].pn == 0)
					error("badAD");
				else if (yypvt[-0].pn->tp->base == FCT)
					error("FD inAL (%n)",yypvt[-0].pn);
				else if (yypvt[-1].p)
                                        yypvt[-1].nl->add_list(yypvt[-0].pn);
				else
					yyval.nl = new nlist(yypvt[-0].pn);
			} break;
case 30:
# line 655 "gram.y"
{
                                yyval.p = 0; 
			} break;
case 32:
# line 662 "gram.y"
{	yyval.p = yypvt[-2].pn;
				yyval.pn->tp = new basetype(FIELD,yypvt[-0].pn);
		 	} break;
case 33:
# line 666 "gram.y"
{	yyval.p = new name;
				yyval.pn->tp = new basetype(FIELD,yypvt[-0].pn);
			} break;
case 34:
# line 670 "gram.y"
{	Pexpr e = yypvt[-0].pe;
				if (e == dummy) error("emptyIr");
				yypvt[-2].pn->n_initializer = e;
				init_seen = 0;
			} break;
case 35:
# line 678 "gram.y"
{	
				if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); 
				if ( NEXTTOK() == CM && la_look() == TNAME ) 
					EXPECT_ID();
			} break;
case 36:
# line 684 "gram.y"
{	if (yypvt[-2].p)
					if (yypvt[-0].p)
						yypvt[-2].nl->add(yypvt[-0].pn);
					else
						error("DL syntax");
				else {
					if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn);
					error("DL syntax");
				}
				if ( NEXTTOK() == CM && la_look() == TNAME ) 
					EXPECT_ID();
			} break;
case 37:
# line 699 "gram.y"
{ 
				extern int co_hack;
				co_hack = 1;
				/*$$ = Ndata($1,name_unlist($<nl>2));*/
				Pname n = Ndata(yypvt[-2].p,name_unlist(yypvt[-1].nl)); 
				if ( in_typedef && in_tag ) { 
					if ( n->tp->check( in_tag->tp, 0 ))
    						error("%nredefined: previous: %t now: %t", in_tag, in_tag->tp, n->tp );
				}
				in_typedef = 0;
				class_typedef = 0;
				in_tag = 0;
				co_hack = 0;
				DECL_TYPE = 0; 
				yyval.p = n;
			} break;
case 38:
# line 716 "gram.y"
{
				yyval.p = yypvt[-1].pb->aggr(); 
				in_typedef = 0;
				class_typedef = 0;
				in_tag = 0;
				DECL_TYPE = 0; 
			} break;
case 39:
# line 727 "gram.y"
{ 
				yyval.p = new basetype(yypvt[-0].t,0); 
				if ( yypvt[-0].t == TYPEDEF ) in_typedef = yyval.pt;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 40:
# line 733 "gram.y"
{	yyval.p = new basetype(EXTERN,0);
				yyval.pb->b_linkage = yypvt[-0].s;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 41:
# line 738 "gram.y"
{ 
				yyval.p = new basetype(TYPE,yypvt[-0].pn); 
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 42:
# line 743 "gram.y"
{ // modified tn_list TNAME
				yyval.p = new basetype(TYPE,yypvt[-0].pn);
				//xxx qualifier currently ignored...
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 45:
# line 751 "gram.y"
{ 
				if (DECL_TYPE == TNAME)
					yyval.p = new basetype(TYPE,yypvt[-0].pn); 
		//	else if (DECL_TYPE == TSCOPE)
		//	yyval.p = 0;
				else
				if (DECL_TYPE == 0 &&
					yypvt[-0].p->base == TNAME)
						yyval.p = new basetype(TYPE,yypvt[-0].pn); 
				else
					yyval.p = new basetype(yypvt[-0].t,0); 
				DECL_TYPE = -1;
			} break;
case 47:
# line 768 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t); 
				DECL_TYPE = 0;
			} break;
case 48:
# line 774 "gram.y"
{
//error('d',"decl_type: %d  $1: %t  $2: %n",DECL_TYPE,yypvt[-1].pb,yypvt[-0].pn);
				if ( DECL_TYPE != -1 ) 
			 		yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn);
			/*XXX*/	else if(yypvt[-1].pb==0) yyval.p=new basetype(TYPE,yypvt[-0].pn);
				DECL_TYPE = 0;
			} break;
case 49:
# line 781 "gram.y"
{ yyval.p = yypvt[-1].pb->base_adj(yypvt[-0].pb); } break;
case 50:
# line 782 "gram.y"
{ yyval.p = yypvt[-1].pb->base_adj(yypvt[-0].pb); } break;
case 51:
# line 784 "gram.y"
{ 
				if (DECL_TYPE == TYPE)
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t);  
			/*XXX*/	else if (DECL_TYPE == TSCOPE) {
			/*XXX*/		error('i',"type decl_marker(tscope)");
			/*XXX*/	//	yyval.p = yypvt[-1].p;//ignore(?)
			/*XXX*/	}
				else
					yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn); 
				DECL_TYPE = -1;
			} break;
case 52:
# line 800 "gram.y"
{ enumcheck:
			Ptype tx = yypvt[-0].pn->tp;
			yyval.p = yypvt[-0].p;
			if ( tx->base == TYPE ) {
				yyval.p = Pbase(tx)->b_name;
				tx = yyval.pn->tp;
				if ( tx->base != EOBJ
				||   strcmp(yyval.pn->string,yypvt[-0].pn->string)
				)
					error("%n of type%t redeclared as enum.",yypvt[-0].pn,tx);
			} else if ( tx->base != EOBJ )
				error("%n of type%t redeclared as enum",yypvt[-0].pn,tx);
		} break;
case 53:
# line 813 "gram.y"
{ goto enumcheck; } break;
case 54:
# line 816 "gram.y"
{ yyval.p = end_enum(0,yypvt[-1].pn); } break;
case 55:
# line 817 "gram.y"
{ yyval.p = end_enum(yypvt[-3].pn,yypvt[-1].pn); } break;
case 56:
# line 818 "gram.y"
{ yyval.pb = (Pbase)yypvt[-0].pn->tp; } break;
case 57:
# line 822 "gram.y"
{	if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); } break;
case 58:
# line 824 "gram.y"
{	if( yypvt[-0].p)
					if (yypvt[-2].p)
						yypvt[-2].nl->add(yypvt[-0].pn);
					else
						yyval.nl = new nlist(yypvt[-0].pn);
			} break;
case 59:
# line 833 "gram.y"
{	yyval.p = yypvt[-0].pn; yyval.pn->tp = moe_type; } break;
case 60:
# line 835 "gram.y"
{	yyval.p = yypvt[-2].pn;
				yyval.pn->tp = moe_type;
				yyval.pn->n_initializer = yypvt[-0].pe;
			} break;
case 61:
# line 840 "gram.y"
{	yyval.p = 0; } break;
case 62:
# line 844 "gram.y"
{	parsing_class_members = 0;
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
			} break;
case 63:
# line 859 "gram.y"
{
				--in_mem_fct;
				--bl_level; // scope weirdness!
				if ( yychar == ID ) {
					// (yuk!) adjust lex level
					--yylval.pn->lex_level;
				}
				ccl->mem_list = name_unlist(yypvt[-3].nl);
				if ( --in_class_decl )  // nested class
					// continue to parse enclosing class
					parsing_class_members = 1;
				ccl->nest_list = nested_type;
				if ( nested_tn ) nested_restore();
				nested_type = nested_type_vec[in_class_decl];
				nested_tn = nested_tn_vec[in_class_decl];
				end_cl();
				declTag = 1;
			} break;
case 64:
# line 878 "gram.y"
{ aggrcheck:
				yyval.pb = (Pbase)yypvt[-0].pn->tp; 
				if ( yyval.p->base == TYPE ) {
					Pname nx = yyval.pb->b_name;
					yyval.pb = (Pbase)nx->tp;
					if ( yyval.p->base != COBJ
					||   strcmp(nx->string,yypvt[-0].pn->string)
					)
						error("%n of type%t redeclared as%k.",yypvt[-0].pn,yyval.pb,yypvt[-1].t);
				} else if ( yyval.p->base != COBJ )
					error("%n of type%t redeclared as%k",yypvt[-0].pn,yyval.pb,yypvt[-1].t);
				check_tag();
			} break;
case 65:
# line 893 "gram.y"
{
				goto aggrcheck;
			} break;
case 68:
# line 903 "gram.y"
{ yyval.pbc = yypvt[-0].pbc; } break;
case 69:
# line 904 "gram.y"
{ yyval.pbc = 0; } break;
case 71:
# line 909 "gram.y"
{	if (yypvt[-0].pbc) { yyval.pbc = yypvt[-0].pbc; yyval.pbc->next = yypvt[-2].pbc; } } break;
case 72:
# line 912 "gram.y"
{ yyval.pbc = dobase(0,yypvt[-0].pn); } break;
case 73:
# line 913 "gram.y"
{ yyval.pbc = dobase(yypvt[-1].t,yypvt[-0].pn); } break;
case 74:
# line 914 "gram.y"
{ yyval.pbc = dobase(0,yypvt[-0].pn,yypvt[-1].t); } break;
case 75:
# line 915 "gram.y"
{ yyval.pbc = dobase(yypvt[-2].t,yypvt[-0].pn,yypvt[-1].t); } break;
case 76:
# line 916 "gram.y"
{ yyval.pbc = dobase(yypvt[-1].t,yypvt[-0].pn,yypvt[-2].t); } break;
case 77:
# line 920 "gram.y"
{	parsing_class_members = 1;
				yyval.p = start_cl(yypvt[-1].t,0,0); 
				nested_tn_vec[in_class_decl] = nested_tn;
				nested_type_vec[in_class_decl++] = nested_type;
				nested_tn = nested_type = 0;
			} break;
case 78:
# line 928 "gram.y"
{	parsing_class_members = 1;
				yyval.p = start_cl(yypvt[-3].t,yypvt[-2].pn,yypvt[-1].pbc);
				nested_tn_vec[in_class_decl] = nested_tn;
				nested_type_vec[in_class_decl++] = nested_type;
				nested_tn = nested_type = 0;
			} break;
case 79:
# line 936 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 80:
# line 937 "gram.y"
{ yyval.p=yypvt[-0].p; } break;
case 81:
# line 941 "gram.y"
{
				if (yypvt[-0].p) {
					if (yypvt[-1].p)
						yypvt[-1].nl->add_list(yypvt[-0].pn);
					else
						yyval.nl = new nlist(yypvt[-0].pn);
				}
			} break;
case 82:
# line 950 "gram.y"
{	yyval.p = 0; } break;
case 89:
# line 960 "gram.y"
{	yyval.p = new name;
				yyval.pn->base = yypvt[-1].t;
			} break;
case 90:
# line 970 "gram.y"
{	Pname n = Ncopy(yypvt[-1].pn);
				if (n->n_oper == TYPE) {
					error('s',"visibilityD for conversion operator");
					// n->tp = Ptype(n->n_initializer);
					n->tp = Ptype(n->cond);
					n->cond = 0;
					// n->n_initializer = 0;
					n->n_oper = 0;
					sig_name(n);
				}
				n->n_qualifier = yypvt[-2].pn;
				n->base = PR;
				yyval.p = n;
			} break;
case 91:
# line 999 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 92:
# line 1001 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_oper = DTOR;
			} break;
case 93:
# line 1005 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
			} break;
case 94:
# line 1009 "gram.y"
{	Pname n = yypvt[-0].pn;
				n->string = "_type";
				n->n_oper = TYPE;
				n->cond = Pexpr(n->tp);
				// n->n_initializer = Pexpr(n->tp);
				n->tp = 0;
				yyval.p = n;
			} break;
case 107:
# line 1031 "gram.y"
{	yyval.t = CALL; } break;
case 108:
# line 1032 "gram.y"
{	yyval.t = DEREF; } break;
case 114:
# line 1038 "gram.y"
{	yyval.t = NEW; } break;
case 115:
# line 1039 "gram.y"
{	yyval.t = DELETE; } break;
case 116:
# line 1040 "gram.y"
{	yyval.t = REF; } break;
case 117:
# line 1041 "gram.y"
{	yyval.t = CM; } break;
case 118:
# line 1042 "gram.y"
{	yyval.t = REFMUL;
					if (yypvt[-0].t == DOT) error(".* cannot be overloaded");
				} break;
case 119:
# line 1048 "gram.y"
{   if ( yypvt[-0].pn != sta_name ) {
// error('d',"tn_list: tscope: pn1: %s", yypvt[-0].pn->string);
				Ptype t = yypvt[-0].pn->tp;
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
			    yyval.pn = yypvt[-0].pn;
			} break;
case 120:
# line 1066 "gram.y"
{ error('s', "CNs do not nest, use typedef x::y y_in_x"); } break;
case 121:
# line 1071 "gram.y"
{	yyval.pn = yypvt[-0].pn;
				//xxx qualifier currently ignored...
				// yyval.pn = Ncopy( yypvt[-0].pn );
				// yyval.pn->n_oper = TNAME;
				// yyval.pn->n_qualifier = yypvt[-1].pn;
			} break;
case 122:
# line 1078 "gram.y"
{	yyval.pn = yypvt[-0].pn;
			//	yyval.pn = Ncopy( yypvt[-0].pn );
			//	yyval.pn->n_oper = TNAME;
			} break;
case 123:
# line 1085 "gram.y"
{ yyval.i = 0; } break;
case 124:
# line 1087 "gram.y"
{ /* const/volatile function */
				switch ( yypvt[-0].t ) {
				case VOLATILE:
					error('s',"volatile functions");
					break;
				case CONST:
					yyval.i = (yypvt[-1].i | 1);
					break;
				default:
					if ( NEXTTOK() != SM
					&&   yychar != COLON
					&&   yychar != LC ) {
						la_backup(yychar,yylval);
						yylval.t = yypvt[-0].t;
						la_backup(TYPE,yylval);
						yylval.t = SM;
						yychar = SM;
						error("syntax error: unexpected%k (did you forget a `;'?)",yypvt[-0].t);
					} else error("FD syntax: unexpected%k",yypvt[-0].t);
					break;
				}
 			} break;
case 125:
# line 1112 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = yypvt[-0].pt;
			} break;
case 126:
# line 1116 "gram.y"
{ /* function with no argument */
 				yypvt[-3].pn->tp = new fct(yypvt[-3].pn->tp,0,1);
 				Pfct(yypvt[-3].pn->tp)->f_const = (yypvt[-0].i & 1);
 			} break;
case 127:
# line 1121 "gram.y"
{	Pname n = yypvt[-1].pn;
				yyval.p = Ncopy(n);
				if (ccl && strcmp(n->string,ccl->string)) n->hide();
				yyval.pn->n_oper = TNAME;
				Freturns(yypvt[-0].p) = yyval.pn->tp;
				yyval.pn->tp = yypvt[-0].pt;
			} break;
case 128:
# line 1133 "gram.y"
{	
				yypvt[-3].pn->tp = new fct(yypvt[-3].pn->tp,yypvt[-1].pn,1); 
 				in_arg_list = 0;
 				end_al(yypvt[-2].pl,0);
			} break;
case 129:
# line 1139 "gram.y"
{
				Pptr p = new ptr( PTR, 0 );
				Ptyp(p) = new basetype(TYPE,yypvt[-5].pn);
				Freturns( yypvt[-0].p ) = Ptype(p);
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].pn;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 130:
# line 1148 "gram.y"
{	yyval.p = Ncopy(yypvt[-3].pn);
				yyval.pn->n_oper = TNAME;
				yyval.pn->tp = new fct(0,yypvt[-1].pn,1);
			} break;
case 131:
# line 1153 "gram.y"
{ /* function with no argument */
				yyval.p = Ncopy(yypvt[-3].pn);
 				yyval.pn->n_oper = TNAME;
 				yyval.pn->tp = new fct(0,0,1);
 				Pfct(yypvt[-3].pn->tp)->f_const = (yypvt[-0].i & 1);
			} break;
case 132:
# line 1160 "gram.y"
{	memptrdcl(yypvt[-3].pn,yypvt[-5].pn,yypvt[-0].pt,yypvt[-2].pn);
				yyval.p = yypvt[-2].p;
			} break;
case 134:
# line 1165 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_qualifier = yypvt[-2].pn;
				error(strict_opt?0:'w',"`.' used for qualification; please use `::' (anachronism)");
			} break;
case 135:
# line 1170 "gram.y"
{	yyval.p = yypvt[-0].p;
				if ( yypvt[-1].pn != sta_name ) {
    					set_scope(yypvt[-1].pn); 
    					yyval.pn->n_qualifier = yypvt[-1].pn;
				}
			} break;
case 136:
# line 1177 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_qualifier = yypvt[-2].pn;
				error(ansi_opt?0:'w',"anachronism `.' used for qualification; please use `::'");
				if ( yypvt[-3].pn != sta_name ) {
    					set_scope(yypvt[-3].pn); 
    					yypvt[-2].pn->n_qualifier = yypvt[-3].pn;
				}
			} break;
case 137:
# line 1195 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = yypvt[-1].pt;
				yyval.p = yypvt[-0].p;
			} break;
case 138:
# line 1200 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_oper = TNAME;
				// cannot evaluate at this point: defer until data_dcl
				if ( in_typedef ) {
					defer_check = 1;
					in_tag = yypvt[-0].pn;
				}
				yypvt[-0].pn->hide();
				defer_check = 0;
				yyval.pn->tp = yypvt[-1].pt;
			} break;
case 139:
# line 1212 "gram.y"
{	yyval.p = Ncopy(yypvt[-1].pn);
				yyval.pn->n_oper = TNAME;
				if ( in_typedef ) {
     					defer_check = 1;
     					in_tag = yypvt[-0].pn;
				}
				yypvt[-1].pn->hide();
				defer_check = 0;
				yyval.pn->tp = yypvt[-0].pt;
			} break;
case 140:
# line 1223 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = yypvt[-0].pt;
			} break;
case 141:
# line 1240 "gram.y"
{ 
				yyval.p = yypvt[-1].p; 
				in_arg_list = 0;
				end_al(yypvt[-2].pl,0);
			} break;
case 142:
# line 1248 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 143:
# line 1250 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_oper = TNAME;
				yypvt[-0].pn->hide();
				yyval.pn->tp = yypvt[-1].pt;
			} break;
case 144:
# line 1256 "gram.y"
{	
				yyval.p = new name; 
				NOT_EXPECT_ID();
			} break;
case 145:
# line 1261 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
			} break;
case 146:
# line 1266 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 147:
# line 1270 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 148:
# line 1286 "gram.y"
{ 
// error('d', "arg_lp arg_decl rp in_arg_list: %d", in_arg_list );
				yyval.p = yypvt[-1].p; 
				in_arg_list = 0;
				end_al(yypvt[-2].pl,0);
			} break;
case 149:
# line 1295 "gram.y"
{	yyval.p = new name; } break;
case 150:
# line 1297 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
                                NOT_EXPECT_ID();
			} break;
case 151:
# line 1303 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 152:
# line 1308 "gram.y"
{ yyval.p = new name; } break;
case 153:
# line 1310 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
				NOT_EXPECT_ID();
			} break;
case 154:
# line 1316 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 155:
# line 1320 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-2].pn->tp;
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].p;
			} break;
case 156:
# line 1325 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-2].pn->tp;
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].p;
			} break;
case 157:
# line 1332 "gram.y"
{	yyval.p = new name; } break;
case 158:
# line 1334 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
			} break;
case 159:
# line 1344 "gram.y"
{
				yyval.p = 0; 
			} break;
case 160:
# line 1348 "gram.y"
{	
				if (yypvt[-0].p)
					if (yypvt[-1].p)
						yypvt[-1].sl->add(yypvt[-0].ps);
					else {
						yyval.sl =  new slist(yypvt[-0].ps);
						stmt_seen = 1;
					}
			} break;
case 161:
# line 1359 "gram.y"
{
				yyval.p = yypvt[-0].p;
				if (yypvt[-0].p)	stmt_seen = 1;
			} break;
case 162:
# line 1366 "gram.y"
{
				yyval.p = 0;
				check_decl();
			} break;
case 163:
# line 1373 "gram.y"
{	yyval.p = yypvt[-1].p;
			/*	if ($<pe>$ == dummy) error("empty condition");*/
				stmt_seen = 1;
			} break;
case 164:
# line 1380 "gram.y"
{	cd_vec[cdi] = cd;
				stmt_vec[cdi] = stmt_seen;
				tn_vec[cdi] = modified_tn;
				lcl_blk_vec[cdi++] = local_blk;
				lcl_tn_vec[cdi] = local_tn;
				local_blk = 0;
				local_tn = 0;
				cd = 0;
				stmt_seen = 0;
				modified_tn = 0;
			} break;
case 165:
# line 1392 "gram.y"
{	Pname n = name_unlist(cd);
				Pstmt ss = stmt_unlist(yypvt[-1].sl);
				yyval.p = new block(yypvt[-3].l,n,ss,yypvt[-0].l);
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
			} break;
case 166:
# line 1407 "gram.y"
{	yyval.p = new block(yypvt[-1].l,0,0,yypvt[-0].l); NOT_EXPECT_ID();} break;
case 167:
# line 1409 "gram.y"
{	yyval.p = new block(yypvt[-2].l,0,0,yypvt[-0].l); NOT_EXPECT_ID();} break;
case 168:
# line 1413 "gram.y"
{	yyval.p = new estmt(SM,curloc,yypvt[-0].pe,0);	} break;
case 169:
# line 1415 "gram.y"
{	yyval.p = new stmt(BREAK,yypvt[-0].l,0); } break;
case 170:
# line 1417 "gram.y"
{	yyval.p = new stmt(CONTINUE,yypvt[-0].l,0); } break;
case 171:
# line 1419 "gram.y"
{	yyval.p = new lstmt(GOTO,yypvt[-1].l,yypvt[-0].pn,0); } break;
case 172:
# line 1420 "gram.y"
{ stmt_seen=1; } break;
case 173:
# line 1421 "gram.y"
{	yyval.p = new estmt(DO,yypvt[-4].l,yypvt[-0].pe,yypvt[-2].ps); } break;
case 174:
# line 1423 "gram.y"
{	
				if (stmt_seen)
					yyval.p = new estmt(ASM,curloc,(Pexpr)yypvt[-1].s,0);
				else {
					Pname n = new name(make_name('A'));
					n->tp = new basetype(ASM,(Pname)yypvt[-1].s);
					if (cd)
						cd->add_list(n);
					else
						cd = new nlist(n);
					yyval.p = 0;
				}
			} break;
case 175:
# line 1438 "gram.y"
{
			if ( NEXTTOK() != SM ) {
				error("`;' missing afterS");
				la_backup(yychar,yylval);
				yychar = SM;
			}
		  } break;
case 178:
# line 1449 "gram.y"
{	yyval.p = new estmt(SM,yypvt[-0].l,dummy,0); } break;
case 179:
# line 1451 "gram.y"
{	yyval.p = new estmt(RETURN,yypvt[-2].l,yypvt[-1].pe,0); } break;
case 180:
# line 1453 "gram.y"
{
				error("local linkage specification");
				yyval.p = yypvt[-0].pn;
			} break;
case 181:
# line 1458 "gram.y"
{	Pname n = yypvt[-0].pn;
				if (n)
					if (stmt_seen) {
						yyval.p = new block(n->where,n,0);
						yyval.ps->base = DCL;
					}
					else {
						if (cd)
							cd->add_list(n);
						else
							cd = new nlist(n);
						yyval.p = 0;
					}
			} break;
case 182:
# line 1473 "gram.y"
{
				Pname n = yypvt[-0].pn;
				error(&n->where,"%n's definition is nested (did you forget a ``}''?)",n);
				if (cd)
					cd->add_list(n);
				else
					cd = new nlist(n);
				yyval.p = 0;
			} break;
case 184:
# line 1484 "gram.y"
{	yyval.p = new ifstmt(yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps,0); } break;
case 185:
# line 1486 "gram.y"
{	yyval.p = new ifstmt(yypvt[-4].l,yypvt[-3].pe,yypvt[-2].ps,yypvt[-0].ps); } break;
case 186:
# line 1488 "gram.y"
{	yyval.p = new estmt(WHILE,yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps); } break;
case 187:
# line 1489 "gram.y"
{ stmt_seen=1; } break;
case 188:
# line 1490 "gram.y"
{	yyval.p = new forstmt(yypvt[-8].l,yypvt[-5].ps,yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].ps); } break;
case 189:
# line 1492 "gram.y"
{	yyval.p = new estmt(SWITCH,yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps); } break;
case 190:
# line 1493 "gram.y"
{ yyval.p = yypvt[-1].pn; stmt_seen=1; } break;
case 191:
# line 1494 "gram.y"
{	Pname n = yypvt[-1].pn;
				yyval.p = new lstmt(LABEL,n->where,n,yypvt[-0].ps);
			} break;
case 192:
# line 1497 "gram.y"
{ yyval.p = new name(yypvt[-1].pn->string); stmt_seen=1; } break;
case 193:
# line 1498 "gram.y"
{	Pname n = yypvt[-1].pn;
				yyval.p = new lstmt(LABEL,n->where,n,yypvt[-0].ps);
			} break;
case 194:
# line 1501 "gram.y"
{ stmt_seen=1; } break;
case 195:
# line 1502 "gram.y"
{	if (yypvt[-2].pe == dummy) error("empty case label");
				yyval.p = new estmt(CASE,yypvt[-4].l,yypvt[-2].pe,yypvt[-0].ps);
			} break;
case 196:
# line 1505 "gram.y"
{ stmt_seen=1; } break;
case 197:
# line 1506 "gram.y"
{	yyval.p = new stmt(DEFAULT,yypvt[-3].l,yypvt[-0].ps); } break;
case 198:
# line 1513 "gram.y"
{	Pexpr e = expr_unlist(yypvt[-0].el);
				while (e && e->e1==dummy) {
					register Pexpr ee2 = e->e2;
					if (ee2) error("EX inEL");
					delete e;
					e = ee2;
				}
				yyval.p = e;
			} break;
case 199:
# line 1525 "gram.y"
{	yyval.el = new elist(new expr(ELIST,yypvt[-0].pe,0)); } break;
case 200:
# line 1527 "gram.y"
{	yypvt[-2].el->add(new expr(ELIST,yypvt[-0].pe,0)); } break;
case 202:
# line 1532 "gram.y"
{
  			  	if ( in_arg_list ) 
       			  		error( "syntax error: IrL not permitted in AL" );
				else if ( in_binit_list )
					error( "syntax error: IrL not permitted inMIr" );
  			  	else 
					init_seen = 1;
				Pexpr e;
				if (yypvt[-1].p)
					e = yypvt[-1].pe;
				else
					e = new expr(ELIST,dummy,0);
				yyval.p = new expr(ILIST,e,0);
			} break;
case 203:
# line 1549 "gram.y"
{	bbinop:	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 204:
# line 1550 "gram.y"
{	goto bbinop; } break;
case 205:
# line 1551 "gram.y"
{	goto bbinop; } break;
case 206:
# line 1552 "gram.y"
{	goto bbinop; } break;
case 207:
# line 1553 "gram.y"
{	goto bbinop; } break;
case 208:
# line 1554 "gram.y"
{	goto bbinop; } break;
case 209:
# line 1555 "gram.y"
{	goto bbinop; } break;
case 210:
# line 1556 "gram.y"
{ 	goto bbinop; } break;
case 211:
# line 1557 "gram.y"
{	goto bbinop; } break;
case 212:
# line 1558 "gram.y"
{	goto bbinop; } break;
case 213:
# line 1559 "gram.y"
{	goto bbinop; } break;
case 214:
# line 1560 "gram.y"
{	goto bbinop; } break;
case 215:
# line 1561 "gram.y"
{	goto bbinop; } break;
case 216:
# line 1562 "gram.y"
{	goto bbinop; } break;
case 217:
# line 1563 "gram.y"
{	goto bbinop; } break;
case 218:
# line 1565 "gram.y"
{	yyval.p = new qexpr(yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].pe); } break;
case 219:
# line 1567 "gram.y"
{ yyval.p = new expr(DELETE,yypvt[-0].pe,0); } break;
case 220:
# line 1569 "gram.y"
{
				if(yypvt[-2].pe != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `delete[v]' is redundant; use `delete[] instead (anachronism)");
				}
				yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe);
			} break;
case 221:
# line 1577 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,0); } break;
case 222:
# line 1579 "gram.y"
{
				if(yypvt[-2].pe != dummy) { 
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `::delete[v]' is redundant; use `::delete[] instead (anachronism)");
				}
				yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe);
			} break;
case 224:
# line 1590 "gram.y"
{	binop:	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 225:
# line 1591 "gram.y"
{	goto binop; } break;
case 226:
# line 1592 "gram.y"
{	goto binop; } break;
case 227:
# line 1593 "gram.y"
{	goto binop; } break;
case 228:
# line 1594 "gram.y"
{	goto binop; } break;
case 229:
# line 1595 "gram.y"
{	goto binop; } break;
case 230:
# line 1596 "gram.y"
{	goto binop; } break;
case 231:
# line 1597 "gram.y"
{ 	goto binop; } break;
case 232:
# line 1598 "gram.y"
{	goto binop; } break;
case 233:
# line 1599 "gram.y"
{	goto binop; } break;
case 234:
# line 1600 "gram.y"
{	goto binop; } break;
case 235:
# line 1601 "gram.y"
{	goto binop; } break;
case 236:
# line 1602 "gram.y"
{	goto binop; } break;
case 237:
# line 1603 "gram.y"
{	goto binop; } break;
case 238:
# line 1604 "gram.y"
{	goto binop; } break;
case 239:
# line 1606 "gram.y"
{	yyval.p = new qexpr(yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].pe); } break;
case 240:
# line 1608 "gram.y"
{	yyval.p = new expr(DELETE,yypvt[-0].pe,0); } break;
case 241:
# line 1610 "gram.y"
{
				if(yypvt[-2].pe != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `delete[v]' is redundant; use `delete[] instead (anachronism)");
				}
				yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe);
			} break;
case 242:
# line 1618 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,0); } break;
case 243:
# line 1620 "gram.y"
{
				if(yypvt[-2].pe != dummy) {
					if ( warning_opt || strict_opt )
						error(strict_opt?0:'w',"v in `::delete[v]' is redundant; use `::delete[] instead (anachronism)");
				}
				yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe);
			} break;
case 244:
# line 1627 "gram.y"
{ 
			init_seen = 0; 
			} break;
case 245:
# line 1631 "gram.y"
{	yyval.p = dummy; } break;
case 246:
# line 1634 "gram.y"
{ goto new1; } break;
case 247:
# line 1636 "gram.y"
{	new1:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p = new texpr(NEW,t,0);
			} break;
case 248:
# line 1640 "gram.y"
{ goto new3; } break;
case 249:
# line 1642 "gram.y"
{	new3:
				Ptype t = yypvt[-0].pn->tp;
 				yyval.p = new texpr(GNEW,t,0);
 			} break;
case 250:
# line 1647 "gram.y"
{	yyval.p = new expr(yypvt[-0].t,yypvt[-1].pe,0); } break;
case 251:
# line 1649 "gram.y"
{	yyval.p = new texpr(CAST,yypvt[-1].pn->tp,yypvt[-0].pe); } break;
case 252:
# line 1651 "gram.y"
{	yyval.p = new expr(DEREF,yypvt[-0].pe,0); } break;
case 253:
# line 1653 "gram.y"
{	yyval.p = new expr(ADDROF,0,yypvt[-0].pe); } break;
case 254:
# line 1655 "gram.y"
{	yyval.p = new expr(UMINUS,0,yypvt[-0].pe); } break;
case 255:
# line 1657 "gram.y"
{	yyval.p = new expr(UPLUS,0,yypvt[-0].pe); } break;
case 256:
# line 1659 "gram.y"
{	yyval.p = new expr(NOT,0,yypvt[-0].pe); } break;
case 257:
# line 1661 "gram.y"
{	yyval.p = new expr(COMPL,0,yypvt[-0].pe); } break;
case 258:
# line 1663 "gram.y"
{	yyval.p = new expr(yypvt[-1].t,0,yypvt[-0].pe); } break;
case 259:
# line 1665 "gram.y"
{	
				yyval.p = new texpr(SIZEOF,0,yypvt[-0].pe); 
				in_sizeof = 0;
			} break;
case 260:
# line 1670 "gram.y"
{	
				yyval.p = new texpr(SIZEOF,yypvt[-0].pn->tp,0); 
				in_sizeof = 0;
			} break;
case 261:
# line 1675 "gram.y"
{	yyval.p = new expr(DEREF,yypvt[-3].pe,yypvt[-1].pe); } break;
case 262:
# line 1677 "gram.y"
{	yyval.p = new ref(REF,yypvt[-2].pe,yypvt[-0].pn); } break;
case 263:
# line 1679 "gram.y"
{	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 264:
# line 1681 "gram.y"
{	yyval.p = new ref(REF,yypvt[-2].pe,Ncopy(yypvt[-0].pn)); } break;
case 265:
# line 1683 "gram.y"
{	yyval.p = new ref(DOT,yypvt[-2].pe,yypvt[-0].pn); } break;
case 266:
# line 1685 "gram.y"
{	yyval.p = new ref(DOT,yypvt[-2].pe,Ncopy(yypvt[-0].pn)); } break;
case 268:
# line 1688 "gram.y"
{
			if ( init_seen )
     				error( "syntax error:IrL illegal within ()");
			} break;
case 269:
# line 1694 "gram.y"
{
				if ( yypvt[-1].p == dummy )
					error("syntax error: nullE");
				yyval.p = yypvt[-1].p;
			} break;
case 270:
# line 1700 "gram.y"
{	yyval.p = zero; } break;
case 271:
# line 1702 "gram.y"
{	yyval.p = new expr(ICON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 272:
# line 1706 "gram.y"
{	yyval.p = new expr(FCON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 273:
# line 1710 "gram.y"
{	yyval.p = new expr(STRING,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 274:
# line 1714 "gram.y"
{	yyval.p = new expr(CCON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 275:
# line 1718 "gram.y"
{	yyval.p = new expr(THIS,0,0); } break;
case 276:
# line 1722 "gram.y"
{ 	yyval.p = new texpr(VALUE,tok_to_type(yypvt[-3].t),yypvt[-1].pe); } break;
case 277:
# line 1724 "gram.y"
{	yyval.p = new texpr(VALUE,yypvt[-3].pn->tp,yypvt[-1].pe); } break;
case 278:
# line 1725 "gram.y"
{ goto new2; } break;
case 279:
# line 1727 "gram.y"
{	new2:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p=new texpr(NEW,t,0);
				yyval.pe->e2 = yypvt[-2].pe;
			} break;
case 280:
# line 1732 "gram.y"
{ goto new4; } break;
case 281:
# line 1734 "gram.y"
{	new4:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p = new texpr(GNEW,t,0);
				yyval.pe->e2 = yypvt[-2].pe;
			} break;
case 282:
# line 1740 "gram.y"
{	
				Pexpr ee = yypvt[-1].pe;
				Pexpr e = yypvt[-3].pe;
				if (e->base==NEW || e->base==GNEW)
					e->e1 = ee;
				else
					yyval.p = new call(e,ee);
			} break;
case 283:
# line 1751 "gram.y"
{       
				yyval.pn = yypvt[-0].pn; 
				curr_scope = yypvt[-0].pn;
			} break;
case 284:
# line 1756 "gram.y"
{       yyval.pn = sta_name; } break;
case 285:
# line 1761 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 286:
# line 1763 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_qualifier = yypvt[-1].pn;
			} break;
case 287:
# line 1767 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
			} break;
case 288:
# line 1771 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 289:
# line 1776 "gram.y"
{	yyval.p = yypvt[-0].p;
				sig_name(yyval.pn);
			} break;
case 290:
# line 1780 "gram.y"
{	yyval.p = yypvt[-0].p;
				sig_name(yyval.pn);
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 291:
# line 1785 "gram.y"
{	
				if (strcmp(yypvt[-2].pn->string,yypvt[-0].pn->string)) error("syntax error: inconsistent destructor notation");
				yyval.p = new name(oper_name(DTOR));
				yyval.pn->n_oper = DTOR;
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 292:
# line 1797 "gram.y"
{ yyval.p = Ncast(yypvt[-2].p,yypvt[-1].pn); } break;
case 293:
# line 1800 "gram.y"
{ check_cast(); } break;
case 294:
# line 1804 "gram.y"
{
				TOK t = yypvt[-0].t;

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

				yyval.p = new basetype(t,0);

			} break;
case 295:
# line 1823 "gram.y"
{ yyval.p = new basetype(TYPE,yypvt[-0].pn); } break;
case 296:
# line 1825 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t); 
				DECL_TYPE = 0;
			} break;
case 297:
# line 1831 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
			 		yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn);
				DECL_TYPE = 0;
			} break;
case 298:
# line 1838 "gram.y"
{ yyval.p = Ncast(yypvt[-1].p,yypvt[-0].pn); } break;
case 299:
# line 1841 "gram.y"
{ yyval.p = Ncast(yypvt[-1].p,yypvt[-0].pn); } break;
case 300:
# line 1844 "gram.y"
{	yyval.p = Ndata(yypvt[-1].p,yypvt[-0].pn); } break;
case 301:
# line 1846 "gram.y"
{	yyval.p = Ndata(yypvt[-3].p,yypvt[-2].pn);
				yyval.pn->n_initializer = yypvt[-0].pe;
			} break;
case 302:
# line 1852 "gram.y"
{ 
				check_decl(); 
				in_arg_list=1; 
				yyval.pl = modified_tn;
				modified_tn = 0;
			} break;
case 303:
# line 1861 "gram.y"
{
			yyval.p = new fct(0,name_unlist(yypvt[-3].nl),yypvt[-2].t); 
			if ( NEXTTOK() != COLON ) in_arg_list=0;
			//in_arg_list=0;
			Pfct(yyval.pt)->f_const = (yypvt[-0].i & 1);
			if ( parsing_class_members
			||   (NEXTTOK()!=LC && yychar!=COLON))
				end_al(yypvt[-4].pl,1);
			else
				end_al(yypvt[-4].pl,0);
		    } break;
case 304:
# line 1875 "gram.y"
{
				if (yypvt[-0].p)
					if (yypvt[-2].p)
						yypvt[-2].nl->add(yypvt[-0].pn);
					else {
						error("AD syntax");
						yyval.nl = new nlist(yypvt[-0].pn); 
					}
				else
					error("AD syntax");
			} break;
case 305:
# line 1887 "gram.y"
{
				if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); 
			} break;
case 307:
# line 1893 "gram.y"
{	yyval.p = 0; } break;
case 308:
# line 1897 "gram.y"
{	yyval.t = 1; } break;
case 309:
# line 1899 "gram.y"
{	yyval.t = ELLIPSIS; } break;
case 310:
# line 1901 "gram.y"
{	yyval.t = ELLIPSIS; } break;
case 311:
# line 1905 "gram.y"
{
			yyval.p = new ptr(PTR,0); 
			EXPECT_ID();
			} break;
case 312:
# line 1910 "gram.y"
{
			yyval.p = new ptr(RPTR,0); 
			EXPECT_ID();
			} break;
case 313:
# line 1915 "gram.y"
{	yyval.p = doptr(PTR,yypvt[-0].t); } break;
case 314:
# line 1917 "gram.y"
{	
				switch ( yypvt[-0].t ) {
				case CONST:
                                     yypvt[-1].pp->rdo = 1; break;
				case VOLATILE:
				     error('w',"\"volatile\" not implemented (ignored)");
				     break;
				default:
				    error( "syntax error: *%k", yypvt[-0].t );
				}
				yyval.p = yypvt[-1].pp; 
			} break;
case 315:
# line 1930 "gram.y"
{	yyval.p = doptr(RPTR,yypvt[-0].t); } break;
case 316:
# line 1932 "gram.y"
{
			yyval.p = new ptr(PTR,0);
			yyval.pp->memof = Pclass(Pbase(yypvt[-0].pn->tp)->b_name->tp);
			EXPECT_ID();
			} break;
case 317:
# line 1938 "gram.y"
{	yyval.p = doptr(PTR,yypvt[-0].t);
				yyval.pp->memof = Pclass(Pbase(yypvt[-1].pn->tp)->b_name->tp);
			} break;
case 318:
# line 1943 "gram.y"
{ yyval.p = new vec(0,yypvt[-1].pe!=dummy?yypvt[-1].pe:0 ); } break;
case 319:
# line 1944 "gram.y"
{ yyval.p = new vec(0,0); } break;
	}
	goto yystack;  /* stack new state and value */
}
