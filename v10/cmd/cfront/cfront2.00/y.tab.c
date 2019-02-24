
# line 24 "gram.y"
#include "cfront.h"
#include "size.h"

extern int ansi_opt;
#define YYMAXDEPTH 600

static init_seen = 0;
static cdi = 0;
static Pnlist cd = 0, cd_vec[BLMAX];
static char stmt_seen = 0, stmt_vec[BLMAX];

//local class
//static Plist tn_vec[BLMAX];
extern Plist local_blk; // put in cfront.h
static Plist tn_vec[BLMAX], lcl_tn_vec[BLMAX], lcl_blk_vec[BLMAX];
extern void local_restore();
extern void local_name();

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


#define lex_unget(x) back = x

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

# line 192 "gram.y"
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
} YYSTYPE;

# line 210 "gram.y"
extern YYSTYPE yylval, yyval;
extern int yyparse();

// interface to lalex/tlex
extern TOK	la_look();
extern void	la_backup( TOK, YYSTYPE );
extern int	la_cast();
extern int	la_decl();
extern TOK	lalex();

extern int	declTag;	 // !1: inline, virtual mod permitted
Ptype 		in_typedef = 0;  // catch redefinition of TNAME
Pname		in_tag = 0;      // handle complex typedefs: int (*)()
extern int	defer_check;	 // redefinition typedef check delay

extern int 	must_be_id;	 // !0, TNAME => ID, i.e., int X
int 		DECL_TYPE = 0; 	 // lalex() wants this set for global x(*fp)()
int 		in_arg_list=0; 	 // !0 when parsing argument list
int 		in_class_decl=0; // !0 when parsing class definition
int 		in_mem_fct=0;    // !0 when parsing member function definition

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
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 1697 "gram.y"


static void
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

static void
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

short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 39,
	71, 22,
	72, 22,
	-2, 21,
-1, 54,
	155, 309,
	-2, 246,
-1, 59,
	13, 303,
	97, 303,
	123, 303,
	155, 303,
	156, 303,
	179, 303,
	182, 303,
	-2, 246,
-1, 106,
	73, 56,
	-2, 53,
-1, 231,
	13, 294,
	97, 294,
	123, 294,
	156, 294,
	179, 294,
	182, 294,
	-2, 246,
-1, 254,
	1, 151,
	3, 151,
	4, 151,
	7, 151,
	8, 151,
	9, 151,
	10, 151,
	13, 151,
	16, 151,
	19, 151,
	20, 151,
	23, 151,
	24, 151,
	28, 151,
	30, 151,
	33, 151,
	34, 151,
	39, 151,
	40, 151,
	46, 151,
	47, 151,
	50, 151,
	52, 151,
	54, 151,
	55, 151,
	72, 151,
	73, 151,
	80, 151,
	81, 151,
	82, 151,
	83, 151,
	84, 151,
	86, 151,
	95, 151,
	97, 151,
	123, 151,
	156, 151,
	160, 151,
	178, 151,
	179, 151,
	182, 151,
	-2, 0,
-1, 316,
	13, 294,
	97, 294,
	123, 294,
	156, 294,
	179, 294,
	182, 294,
	-2, 246,
	};
# define YYNPROD 322
# define YYLAST 2323
short yyact[]={

 123, 253, 150,   7, 454,   8, 160,  55, 256, 159,
 285,  52, 258,  19, 325, 328, 193, 267, 337, 233,
  12,  73,  22,  60, 257, 162,  74,  58, 192, 134,
 125,  16, 180, 156, 400,  44,  34, 230, 136, 333,
  34, 109, 109,  23,  26, 111, 106, 112,  61,  20,
  65,  60, 109,  72, 262,  58,  45, 149, 288, 109,
  31,  20,  54, 148, 109,  18,  48,  25,  20,  20,
  28, 152,  29, 385, 164, 120,  60, 274, 259,  60,
  58, 164, 167,  58, 110, 110, 330, 104,  54,  49,
 103, 102, 247, 169, 172, 110,  28, 108,  29, 465,
  47, 109, 110, 511, 109, 478,   9, 110, 184, 424,
 114, 185,  10, 181, 194, 248,  54, 251, 359,  54,
  21, 401, 218, 198,  21,  12, 234, 235, 236, 237,
 238, 239, 240, 241, 242, 152, 287, 361, 263, 381,
 144,  27, 471,  71, 110, 201, 154, 110, 109, 205,
 154, 113, 346,  27,  20, 229, 245, 246, 453,  63,
  27,  27, 244, 232, 243, 290,  20, 249, 364, 235,
 254, 189, 250, 197, 290,  44, 356, 289, 269,  33,
  36, 283, 344,  33, 199, 195,  37,  44, 355,  34,
 293, 110, 211,  30, 279, 181,  45, 265,  35, 294,
 338,  20,  24, 107, 286, 153,  24, 278,  45, 153,
 357,  34, 186, 270, 276, 339, 316, 264, 187,  30,
  20, 282, 312, 291, 292,  26, 461, 320, 295, 296,
 297, 298, 299, 300, 301, 302, 303, 304, 305, 306,
 307, 308, 309, 310, 234, 311,  27, 326,  25,  54,
 317,  34, 245, 314,  31, 318,  44, 321,  27, 329,
 315, 232, 323,  34,  28, 324,  29, 151,  44, 331,
 177, 345, 343,  21, 332, 347, 459,  45, 231, 348,
  31, 173, 164, 269, 269, 334, 335, 351, 352,  45,
  28, 340,  29,  27, 268,  21, 117, 118,  62, 154,
 358, 360,  53, 280,  56, 191,  56, 362,  57, 465,
  57,  69,  27, 463, 464, 174,  62, 363, 270, 270,
 268, 154, 119, 228,  54, 224, 523, 225, 227, 403,
  63, 396,  33, 372,  50,  21, 374, 350, 383, 367,
 326, 373, 379, 329, 329, 376, 377,  21, 382,  34,
 368, 378, 471, 469,  33,  24, 386, 371, 153, 347,
 347, 154, 145, 348, 348, 388, 389, 394, 390, 370,
 286, 353, 391, 154, 392, 384,  31,  24, 223, 455,
 153, 372, 342, 145, 374, 374,  28,  30,  29, 273,
  28, 393,  29, 260,  33, 228, 404, 224,  26, 225,
 227, 196, 397, 432, 416, 414,  33, 408, 415, 398,
 232, 407, 176,  30,  31, 480, 268,  24, 143, 155,
 153,  25,  12, 456,  28, 458,  29, 405, 457,  24,
 330, 481, 153,  42, 406, 232,  31,  69, 452, 143,
  28, 341,  29, 252, 251,  38, 522,  28,  51,  29,
 223, 222, 205, 515,  17, 485, 203, 204, 493, 266,
 460, 322,  26, 432, 432, 432, 432, 432, 432, 432,
 432, 432, 432, 432, 432, 432, 432, 432, 432, 491,
 489, 487, 319, 434, 513,  25, 492,  69, 115, 486,
 479, 488,  33, 490, 178, 211, 209,  71,  36, 465,
  26, 205, 336, 463, 464, 203, 204, 483, 512,  30,
 284, 190, 261,  30, 510,  46,  35, 221,  64,  36,
 179, 222, 432,  25, 517, 531, 518, 530, 520, 157,
  31, 222,  56, 534,  36, 226,  57,  35, 161,  99,
 525, 135, 471, 527, 211,  13, 158,  30, 410,  31,
 526,  56,  35, 409, 528,  57,  64, 402,  39, 535,
 349, 281,  53,  30,  56,  67,  70, 380,  57,  40,
  30, 537, 494, 495, 496, 497, 498, 499, 500, 501,
 502, 503, 504, 505, 506, 507, 508, 509, 429, 183,
 425, 422, 116, 105, 426, 423, 430, 428,  36,  66,
  34,  32,  26, 419, 465,  14, 427, 417, 463, 464,
 124, 145,   1, 255,   2, 412,  35, 133,  31,   0,
 420, 142,   0,   0,   0,  25, 418, 148,  28,   0,
  29, 529,   0, 130, 131,   0,  36, 126,  59, 127,
  56, 129, 128,   0,  57, 472, 470, 471, 469,   0,
   0, 465, 399,  56,  35, 463, 464,  57,  17, 411,
 254,   0, 121, 200,   0,   0,  34, 421, 140, 138,
 139, 141,  53, 137,  56,  69, 124, 145,  57,  31,
 387,  56, 132, 133, 413,  57,   0, 142,   0,   0,
   0,  34, 472, 148, 471, 469,   0,   0, 200, 130,
 131,  68,   0, 126,   0, 127,   0, 129, 128,   0,
 166, 205,   0,   0,   0, 203, 204, 275, 330, 465,
   0, 466,   0, 463, 464,   0, 163,   0,  28,   0,
  29,   0,   0, 143, 140, 138, 139, 141,  36, 137,
  53, 354,  56,  33, 375,  56,  57, 431, 132,  57,
 165,  30, 212, 121, 211, 209,  35,  34,   0,   0,
 472, 470, 471, 469,   0,  35,  24, 124, 145, 153,
   0,  34,   0,   0, 133,  42, 166,   0, 142,  53,
 175,  56,   0,   0, 148,  57,   0,   0,   0,   0,
 130, 131,   0,   0, 126,   0, 127,   0, 129, 128,
   0, 266,   0,   0,   0,   0,   0,  95,  28,  33,
  29,   0,   0, 122,   0,   0,   0,   0,   0,   0,
   0,  94,   0,   0, 143, 140, 138, 139, 141,   0,
 137,  35,  24,   0,  33, 153,   0,   0,  87, 132,
  88, 165,  96,   0,  89,  90,   0,   0,  77,   0,
  78,  30,  75,  76,   0,  42,   0,  46,   0,   0,
   0,   0,  80,  79,  85,  86,   0, 166,  93,  97,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 266,   0,   0,   0,   0,   0, 121,  92,  84,
  82,  83,  81,  91,   0, 100,   0,   0,   0,   0,
  33, 124, 145,   0, 122, 465,   0, 466, 133, 463,
 464, 205, 142, 206,  33, 203, 204,   0, 148, 468,
 467, 101,  35,  24, 130, 131, 153,   0, 126,   0,
 127,  30, 129, 128,   0,   0,   0,  46,   0,   0,
   0,   0,   0,   0,   0,   0, 472, 470, 471, 469,
   0, 163, 212, 210, 211, 209,   0,   0, 143, 140,
 138, 139, 141,   0, 137,   0,   0,   0, 121,   0,
   0,   0,   0, 132,   0, 146,   0,   0,  98,   0,
   0,   0, 124, 145,   0,   0, 205,   0,   0, 133,
 203, 204,   0, 142,   0,   0,   0,   0,   0, 148,
 170, 147,   0,   0,   0, 130, 131,   0,   0, 168,
   0, 127,   0, 129, 128,   0,   0,   0,   0, 465,
   0, 466,   0, 463, 464,   0,   0, 212, 210, 211,
 209,   0, 163, 468, 121,   0,   0,   0, 122, 143,
 140, 138, 139, 141,   0, 137,   0,   0, 124, 145,
   0,   0,   0, 430, 132, 133,  35,   0,   0, 142,
 472, 470, 471, 469,   0, 148,   0, 124, 145,   0,
   0, 130, 131,   0, 133, 126,   0, 127, 142, 129,
 128,   0,   0,   0, 148,   0,   0,   0,   0,   0,
 130, 131,   0,   0, 126,   0, 127,   0, 129, 128,
   0,   0,   0,   0,   0, 143, 140, 138, 139, 141,
   0, 137,   0,   0,   0,   0,   0,   0,   0, 122,
 132,   0, 146,   0, 143, 140, 138, 139, 141,   0,
 137,   0, 171, 124, 145,   0,   0,  35,   0, 132,
 133, 146,   0,   0, 142,   0,   0,   0, 147,   0,
 148,   0, 514,   0,   0,   0, 130, 131,   0,   0,
 126,   0, 127,   0, 129, 128,   0, 147,   0, 205,
   0, 206,   0, 203, 204,   0,   0,   0,   0,   0,
   0,   0,   0, 208, 207, 122,   0,   0,   0,   0,
 143, 140, 138, 139, 141,   0, 137,   0,   0,   0,
   0,   0,   0,  35, 431, 132,   0, 146, 124, 145,
 212, 210, 211, 209,   0, 133,   0,   0,   0, 142,
   0,   0,  35,   0,   0, 148,   0, 482,   0,   0,
   0, 130, 131, 147,   0, 126,   0, 127,   0, 129,
 128,   0,   0, 465,   0, 466,   0, 463, 464,   0,
   0,   0,   0,   0,   0,   0,   0, 468, 467, 473,
 474, 477,   0,   0,   0, 143, 140, 138, 139, 141,
 220, 137, 124, 145,   0,   0,   0,   0,   0, 133,
 132,   0, 146, 142, 472, 470, 471, 469,  35, 148,
   0, 313,   0,   0,   0, 130, 131,   0,   0, 126,
   0, 127,   0, 129, 128,   0,   0,   0, 147,   0,
   4,  11,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  34,   0,   0,   0,   0,   0, 143,
 140, 138, 139, 141,  26, 137,   0,   0,   0,   0,
   0,   0, 124, 145, 132, 220, 146,   0,   0, 133,
  31,   0,   0, 142,   0,   0,   0,  25,   0, 148,
  28, 219,  29,  35,   0, 130, 131,   0,   0, 126,
   0, 127, 147, 129, 128,   0,   0,   0,   0,   0,
   0,   0,   3,   0,   6,   0,   0,   0,   0,   0,
  17,   0,   0,   0,   0,   0,   0,   0,   0, 143,
 140, 138, 139, 141,   0, 137, 465,  21, 466, 220,
 463, 464,   0,   0, 132,   0, 146,   0,   0,   0,
 468, 467, 473, 474, 477,   0, 462,  35,   0,   0,
   0,   0,   0,  15,   0,   0,   0,   0,   0,   0,
   0,   0, 147,   0, 124, 145, 475, 472, 470, 471,
 469, 133,   0,   0,   0, 142,   0,   0,   0,   0,
   0, 148,   0,   0,   0,   0,  33, 130, 131,   0,
  36, 126,   0, 127,   0, 129, 128,   0,   0, 220,
   0,   0,   0,  30,   0,   0,   0,   0,  35,  24,
   0,   0,   5,   0, 121,   0,   0,  35,   0,   0,
   0, 143, 140, 138, 139, 141,   0, 137, 124, 145,
   0,   0,   0,   0,   0, 133, 132,   0, 146, 142,
   0,   0,   0,   0,   0, 148,   0,   0,   0,   0,
   0, 130, 131,   0,   0, 126,  34, 127,   0, 129,
 128,   0,   0,   0, 147,   0,   0,  26, 205,   0,
 206,   0, 203, 204,   0,   0,   0,   0, 163,   0,
   0,   0, 208,  31,   0, 143, 140, 138, 139, 141,
  25, 137,   0,  28,   0,  29,   0, 124, 145,   0,
 132, 220,   0,   0, 133,   0,   0,   0, 142, 212,
 210, 211, 209,   0, 148,   0,   0, 182,   0,  35,
 130, 131,   0,  17, 126,  34, 127,   0, 129, 128,
   0,   0,   0, 205,   0, 206,  26, 203, 204,   0,
  21,   0,   0,   0,   0,   0,   0, 208, 207, 213,
 214, 217,  31, 202, 272, 140, 138, 139, 141,  25,
 137,   0,  28,   0,  29, 122,  15,   0,   0, 132,
   0, 146,   0, 215, 212, 210, 211, 209, 124, 145,
   0,  49,   0,  35,  41, 133,   0,   0,   0, 142,
   0,   0,  47,   0,   0, 148,   0, 147,   0,  33,
   0, 130, 131,  36,   0,   0,   0,   0,   0,  42,
   0,   0,   0,   0,   0,   0,  30,   0, 188,   0,
   0,  35,  24,   0,   0, 153,   0,   0,   0,   0,
   0,   0,   0,   0, 220,  43, 140, 138, 139, 141,
   0, 137,   0,   0,   0,   0,   0,   0,   0,   0,
 132, 450,  35, 440, 446,   0,   0, 441, 447,   0,
 439,   0,   0,   0,   0,   0, 437,   0,  33, 442,
 435,   0,  36,   0,   0,   0,   0,   0, 443,   0,
   0,   0,   0, 436,   0,  30,   0, 536,   0, 438,
  35,  46,   0,   0,   0,   0, 205,   0, 206,   0,
 203, 204,   0,   0,   0,   0,   0,   0,   0,   0,
 208, 207, 213, 214, 217, 220, 202, 216,   0, 451,
   0,   0, 433,   0, 445,   0,   0,   0,   0,   0,
 444,   0,   0,  35,   0,   0, 215, 212, 210, 211,
 209, 205,   0, 206,   0, 203, 204, 448,   0,   0,
   0,   0,   0,   0,   0, 208, 207, 213, 214, 217,
   0, 202, 216, 533,   0, 532,   0,   0,   0,   0,
   0,   0, 205, 449, 206,   0, 203, 204,   0,   0,
   0, 215, 212, 210, 211, 209, 208, 207, 213, 214,
 217,   0, 202, 216,   0,   0, 524,   0,   0,   0,
   0,   0,   0, 205,   0, 206,   0, 203, 204,   0,
   0,   0, 215, 212, 210, 211, 209, 208, 207, 213,
 214, 217,   0, 202, 216,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 465,   0, 466,   0, 463, 464,
   0,   0,   0, 215, 212, 210, 211, 209, 468, 467,
 473, 474, 477, 521, 462, 476,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 205,   0, 206,   0, 203,
 204,   0,   0,   0, 475, 472, 470, 471, 469, 208,
 207, 213, 214, 217, 519, 202, 216, 516,   0,   0,
   0,   0,   0,   0,   0,   0, 205,   0, 206,   0,
 203, 204,   0,   0,   0, 215, 212, 210, 211, 209,
 208, 207, 213, 214, 217,   0, 202, 216,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 205,   0, 206,
   0, 203, 204,   0,   0,   0, 215, 212, 210, 211,
 209, 208, 207, 213, 214, 217,   0, 202, 216, 484,
   0, 395,   0,   0,   0,   0,   0,   0, 205,   0,
 206,   0, 203, 204,   0,   0,   0, 215, 212, 210,
 211, 209, 208, 207, 213, 214, 217,   0, 202, 216,
   0,   0, 369,   0,   0,   0,   0,   0,   0, 205,
   0, 206,   0, 203, 204,   0,   0,   0, 215, 212,
 210, 211, 209, 208, 207, 213, 214, 217,   0, 202,
 216,   0,   0, 366,   0,   0,   0,   0,   0,   0,
 205,   0, 206,   0, 203, 204,   0,   0,   0, 215,
 212, 210, 211, 209, 208, 207, 213, 214, 217,   0,
 202, 216,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 205,   0, 206,   0, 203, 204,   0,   0,   0,
 215, 212, 210, 211, 209, 208, 207, 213, 214, 217,
 365, 202, 216, 327,   0,   0,   0,   0,   0,   0,
   0,   0, 205,   0, 206,   0, 203, 204,   0,   0,
   0, 215, 212, 210, 211, 209, 208, 207, 213, 214,
 217,   0, 202, 216,   0,   0, 271,   0,   0,   0,
   0,   0,   0, 205,   0, 206,   0, 203, 204,   0,
   0,   0, 215, 212, 210, 211, 209, 208, 207, 213,
 214, 217,   0, 202, 216,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 465,   0, 466,   0, 463, 464,
   0,   0,   0, 215, 212, 210, 211, 209, 468, 467,
 473, 474, 477,   0, 462, 476,   0, 465,   0, 466,
   0, 463, 464,   0,   0, 205,   0, 206,   0, 203,
 204, 468, 467, 473, 475, 472, 470, 471, 469, 208,
 207, 213, 214, 217,   0,   0,   0,   0, 205,   0,
 206,   0, 203, 204,   0,   0,   0,   0, 472, 470,
 471, 469, 208, 207, 213,   0, 212, 210, 211, 209,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 212,
 210, 211, 209 };
short yypact[]={

1310,-1000,-1000,-1000,-1000, 113,-1000,-1000,-1000,-1000,
-1000, 405,1592, 262,-1000, 598,-1000, 271, 476, 578,
 374,-1000,-1000,-1000,-1000, -70, 798,-1000,  -6,  -7,
 -10,-1000,-1000,  24, -28,-1000,-1000,-1000,  29, 522,
 225,-1000,-1000, 598,-1000,-1000,-1000, 253,-1000,1025,
-1000, 198,-1000, 378, 653,-1000,1025,-1000,-1000, 959,
-1000, 250, 201,-1000, 270,-1000,-1000, 632, 598,-1000,
 739, 598,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, 371, 227,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 397,
-1000,-1000,-1000,-1000,-1000,1523, 236,-1000,-1000,-1000,
-1000,  34, 112,-1000, 360, 198, 878,  20,-1000,1025,
1563,1319, 508, 355, 238,1421,1421,1421,1421,1421,
1421,1421,1421,1421,-1000,-1000, 744,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,  68, 798, 404, 403,-1000,  97,
-1000, -16,1592,-1000,-1000, -19, 352, -17, 146,-1000,
-1000,-1000,1563, 878, 336, 404, 403,2143,1554, 348,
 -20, 374,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000, 340,-1000,-1000,-1000, 135, 122,-1000, 234, 438,
 108, -39, 103,-1000, 153,  34, 118,  97,-1000,-1000,
 522,1563,1025,1025,1025,1025,1025,1025,1025,1025,
1025,1025,1025,1025,1025,1025,1025,1025, 355,1025,
 428,1249, 176,-1000,1025, 359,1421, 338, 878,-1000,
-1000,1485, 250, 758, 283, 355, 355, 355, 355, 283,
 283, 283, 283,1635,2112, 678,-1000, 798, -84,-1000,
-1000, 878, 878,-1000, -56, 144,-1000, 878, 401,-1000,
-1000, 341,  27,-1000, 878,  78,-1000, 490,-1000, 214,
 240,-1000, 330,-1000,-1000, 700,-1000,-1000,-1000,-1000,
-1000, 116, 104,-1000, 139,-1000,-1000,  21, -38,-1000,
  34,1025,  94,-1000,-1000,1563,  99,  99,-1000, 936,
1498, 861, 451, 661,-1000, 402,1119,2228,1563,1563,
2081,2050, 355,1025,-1000,-1000,1485,2019,-1000,-1000,
 283,-1000,-1000, 328, 316, 264, 340,-1000, 703, 390,
  46,-1000,-1000,-1000, 310, 301,-1000,-1000,  65, -16,
 297, 878, -24,-1000,-1000,-1000,-1000,-1000,-1000, 878,
-1000, 509, 639, 396, 396,-1000,-1000, -39,-1000, -21,
-1000, -21,-1000,1563,-1000,1025,1421,1988, 290,-1000,
-1000,  23,-1000, 264,-1000,-1000, 264, 611,-1000,-1000,
  47,-1000,-1000,-1000, 288,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,2205, 355,1421,  23,-1000,-1000, 509,
-1000,-1000, 587,-1000, 355,-1000,-1000,-1000,-1000,-1000,
1730,-1000,1025,  77,-1000,-1000,-1000, 339, 339, 388,
 339, 207,-1000, 157,2174,-1000,-1000,  25,-1000, 375,
1185, 498, 355,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,1957,  97,-1000,1025,-1000,-1000,-1000,-1000,
1025,-1000,1044,1044,1044,1044,1044,1044,1044,1044,
1044,1044,1044,1044,1044,1044,1044,1044,-1000,-1000,
  22, 355,1025,1110,-1000,-1000, 441,1926,-1000,-1000,
-1000,-1000,1895,-1000,1356,  49,  49,-1000, 554, 969,
 669, 449, 601,-1000, 259, 855,2197,1356,1356,1864,
 407, 285,1833, 355,1025,-1000,-1000,1025,-1000,-1000,
-1000,1044, 339,-1000,1421,1802,-1000,1771,-1000,1193,
-1000, 355,1421,1025, 355,1726,-1000,-1000 };
short yypgo[]={

   0, 614, 112, 106,   5, 448,  57, 613,   8,   2,
 612,   7,  13,  19, 605,  43,  28,  16,  12, 601,
  22, 593, 589,  66, 569,  31, 545,   6, 567,  34,
 557,   1, 553, 548, 546,  33,  25, 109,   0,  29,
 541,  15,  30,  32,  26, 539,  17,   9, 538,  11,
 529,  14,  37,   4, 140,  65,  21, 512, 511, 510,
  10,  48, 502, 490, 483, 480, 479, 460, 458,  38 };
short yyr1[]={

   0,  10,  10,  10,  10,  10,   1,   1,   1,   1,
   1,   2,   4,   3,   6,   6,   7,   7,   8,   8,
   5,   5,  23,  23,  23,  23,  24,  24,   9,   9,
  14,  14,  14,  14,  14,  14,  13,  13,  13,  13,
  13,  13,  15,  15,  15,  15,  16,  16,  17,  17,
  17,  20,  20,  20,  20,  58,  58,  59,  59,  60,
  60,  60,  60,  60,  19,  19,  18,  18,  21,  21,
  22,  22,  22,  22,  22,  22,  22,  22,  22,  25,
  25,  25,  25,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  56,  56,  56,  56,  56,  55,  55,  55,
  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
  26,  26,  26,  26,  26,  26,  26,  26,  26,  46,
  46,  46,  46,  46,  46,  46,  51,  51,  51,  41,
  41,  41,  41,  41,  43,  43,  28,  28,  29,  30,
  53,  62,  31,  31,  31,  33,  33,  33,  33,  63,
  33,  33,  64,  64,  64,  64,  64,  64,  64,  64,
  64,  64,  64,  64,  64,  64,  64,  64,  64,  32,
  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
  65,  32,  32,  66,  32,  67,  32,  68,  32,  35,
  34,  34,  27,  27,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  37,  37,  37,  37,  37,
  37,  37,  37,  37,  37,  36,  36,  36,  36,  36,
  36,  36,  36,  36,  36,  36,  36,  36,  36,  36,
  36,  36,  36,  36,  36,  36,  36,  38,  38,  38,
  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
  38,  38,  38,  38,  38,  38,  38,  40,  40,  40,
  40,  40,  40,  40,  54,  54,  39,  39,  39,  39,
  39,  39,  39,  42,  69,  45,  45,  45,  45,  44,
  52,  48,  48,  61,  49,  49,  50,  50,  47,  47,
  57,  57,  57,  12,  12,  12,  12,  12,  12,  12,
  11,  11 };
short yyr2[]={

   0,   1,   1,   1,   2,   1,   1,   1,   1,   1,
   5,   2,   5,   4,   2,   0,   1,   3,   3,   4,
   2,   0,   1,   3,   2,   3,   1,   3,   3,   2,
   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,
   2,   2,   4,   5,   2,   2,   1,   3,   1,   3,
   0,   3,   4,   2,   2,   2,   0,   1,   3,   1,
   2,   2,   3,   3,   2,   4,   1,   1,   2,   0,
   1,   2,   2,   1,   1,   1,   2,   3,   3,   1,
   2,   2,   2,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   2,   2,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   2,   3,
   2,   3,   4,   2,   4,   6,   4,   3,   4,   6,
   1,   3,   2,   2,   2,   2,   2,   2,   3,   1,
   2,   0,   2,   2,   2,   3,   0,   2,   2,   0,
   2,   2,   4,   4,   0,   2,   0,   2,   2,   0,
   3,   0,   4,   2,   3,   1,   1,   1,   2,   0,
   5,   4,   1,   1,   1,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
   2,   1,   3,   3,   1,   1,   1,   3,   5,   3,
   0,   9,   3,   0,   4,   0,   5,   0,   4,   1,
   1,   3,   1,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   5,
   2,   5,   3,   6,   1,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   5,   2,   5,   3,   6,   1,   0,   2,   2,   3,
   3,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   4,   3,   3,   3,   3,   3,   1,   1,
   3,   1,   1,   1,   1,   1,   1,   4,   4,   5,
   5,   6,   6,   4,   1,   1,   1,   2,   2,   3,
   2,   3,   3,   4,   1,   1,   1,   2,   2,   2,
   2,   2,   4,   1,   4,   5,   3,   1,   1,   0,
   0,   1,   2,   1,   1,   2,   2,   2,   1,   2,
   3,   1 };
short yychk[]={

-1000, -10,  -1,  72,   0, 182,  74,  -9,  -4,  -3,
  -2,   1, -13, -26, -14, 123, -25,  80, -55, -12,
 -61,  97, -20, -15, 179,  47,  24, -54,  50,  52,
 173,  40, -19, 156,  13, 178, 160,  73,  40, -26,
 -24,  72,  97, 123, -20, -15, 179,  80, -23,  69,
  72,  -5, -49,  40, -61, -11,  42,  46, -49,  40,
 -11, -61,  45, -54,  80, -25, 123, -26, 123,  97,
 -26, 123, 123, -56, -44,  54,  55,  50,  52,  65,
  64,  94,  92,  93,  91,  66,  67,  40,  42,  46,
  47,  95,  90,  70,  23,   9,  44,  71, 180, -45,
  97, 123,  97,  97,  97, -21, -18, 179,  73,  80,
 123,  73, -18, 179,  81,  -5,  70,  71,  72,  69,
 -36,   9, 160, -38,  23, -42,  50,  52,  55,  54,
  46,  47,  95,  30, -39, -40, -69,  86,  82,  83,
  81,  84,  34,  80, -54,  24,  97, 123,  40,  -6,
  -9,  69, -13, 182, 123,  41, -35, -50, -34, -47,
 -27, -48, -36,  73, -13,  97, 123, -36,  50, -35,
  41, 173, -25,  80,  45,  41,  41,  43,  97, 123,
 -43, -12,  74, -22,  -9,  -4,  -3,  -2, 175, -55,
 -58,  69, -16, -17,  80,  73,  41,  -6, -27, -23,
 -26, -36,  70,  54,  55,  50,  52,  65,  64,  94,
  92,  93,  91,  66,  67,  90,  71,  68, -38,  42,
 160,   9,  23,  95,  42,  44, 180,  45,  40, -42,
 -52,  40, -69, -13, -38, -38, -38, -38, -38, -38,
 -38, -38, -38, -42, -36, -13, -18,  24,  47, -56,
 -44,  40,  40, -31,  73,  -7,  -8,  40, -18,  97,
  41, -57,  71, 155,  71, -35, 123, -46,  80, -12,
 -61,  43,  80,  41,  97, -26, -43,  97,  72,  72,
  69, 123, -25,  73, -59, -60, -18, 175,  97,  74,
  71,  70, -16,  72, -31, -36, -36, -36, -36, -36,
 -36, -36, -36, -36, -36, -36, -36, -36, -36, -36,
 -36, -36, -38,  42, -42, -52,  40, -36, -39, 123,
 -38, -39, 123, -35, -35, -51, -12,  41, -41, -12,
  40, -56, -44, 123, -35, -35, -62,  74, 256,  71,
 -35,  40,  41, -47, 155, -27,  74, -11, -49,  70,
 123, -46, -46,  41,  41,  72,  72,  71, -18,  97,
 -18, 175, -17, -36,  74,  69,  43, -36, -35,  43,
  41,  41, -11, -51, -11,  41, -41, -41,  41,  41,
 -28,  74,  -8,  41, -35,  97, -27,  41, -49, -49,
 -60, -18, -18, -36, -38,  43,  41, -42, -52,  41,
 -29,  74, -30,  41, -38, -42, -52, -49, -11, -32,
 -33,  72,  28,  97,  -9,  -4, -31,  20,  39,  16,
  33,  80,   4,   8, -37,   3,   7,  19,  10,   1,
   9, 160, -38,  72, -64,  20,  33,  16,  39,  10,
   3,   7,  19,  28,  80,  74,   4,   8,  97, 123,
   1,  69, -36,  81, -53,  40, -53,  40, -53,  69,
 -67,  69,  70,  54,  55,  50,  52,  65,  64,  94,
  92,  93,  91,  66,  67,  90,  71,  68,  80, -63,
  40, -38,  42,   9,  72, -31, -29, -36, -29, -65,
 -29, -66, -36, -68, -37, -37, -37, -37, -37, -37,
 -37, -37, -37, -37, -37, -37, -37, -37, -37, -37,
 -29,  81, -36, -38,  42,  12,  41, -29, -29,  69,
 -29,  69,  39,  41,  43, -36, -29, -36, -29, -37,
 -53, -38,  43,  72, -38, -36,  41, -29 };
short yydef[]={

   0,  -2,   1,   2,   3,  31,   5,   6,   7,   8,
   9,   0,   0,  21,  36,  32, 120,  79,   0,   0,
   0,  30,  33,  34,  35,   0,   0, 107, 313, 314,
 318, 303,  69,   0,   0, 284, 285,   4,   0,  -2,
   0,  29,  37,  38,  39,  40,  41,  79,  26, 246,
  11,  15, 110, 303,  -2, 127, 246, 321, 113,  -2,
 126, 309,   0, 108,  79, 122, 123, 124, 125, 316,
   0,   0,  80,  81,  82,  83,  84,  85,  86,  87,
  88,  89,  90,  91,  92,  93,  94,   0,   0,  97,
  98,  99, 100, 101, 102, 103, 104, 105, 106, 144,
 295, 296, 315, 317, 319,   0,  -2,  54,  64,  66,
  67,  50,  44,  45,   0,  15, 246,   0,  28, 246,
  24,   0, 285, 245,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 268, 269, 246, 271, 272, 273,
 274, 275, 276, 286,   0,   0,   0,   0, 294,   0,
  20,   0,   0,  31,  32, 111,   0, 310, 199, 307,
 200, 308, 202, 246, 131,  30,  32,   0,   0,   0,
 117,   0, 121,  79, 109, 128,  95,  96, 297, 298,
 299, 144,  51,  68,  70,  74,  73,  75,   0,   0,
   0,   0,   0,  46,  48,  50,   0,   0,  25,  27,
  22,  23, 246, 246, 246, 246, 246, 246, 246, 246,
 246, 246, 246, 246, 246, 246, 246, 246, 241, 246,
 285,   0,   0, 251, 246,   0,   0,   0, 246, 247,
 248,  -2,   0, 136, 252, 253, 254, 255, 256, 257,
 258, 259, 260, 261,   0, 139, 287,   0,   0, 288,
 290, 246, 246,  13,  -2,  14,  16, 246,   0, 112,
 114,   0, 309, 311, 246,   0,  38, 301, 129, 131,
 131, 320, 286, 116, 118,   0, 145,  52,  71,  72,
  76, 123, 122,  65,  55,  57,  59,   0,   0,  42,
  50, 246,   0,  10,  12, 225, 226, 227, 228, 229,
 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
   0,   0, 243, 246, 249, 250,  -2,   0, 263, 265,
 264, 266, 267,   0,   0, 300, 136, 270,   0, 139,
 139, 289, 291, 292,   0,   0, 146, 153,   0,   0,
   0, 246, 304, 306, 312, 201, 203, 133, 134, 246,
 130, 132,   0,   0,   0,  77,  78,   0,  60,   0,
  61,   0,  47,  49,  43, 246,   0,   0,   0, 262,
 283,   0, 138, 137, 141, 293, 140,   0, 277, 278,
 149, 154,  17,  18,   0, 305, 302, 135, 115, 119,
  58,  62,  63, 240, 242,   0,   0, 279, 280,   0,
 147, 152,   0,  19, 244, 281, 282, 142, 143, 148,
   0, 181, 246,  30, 184, 185, 186,   0,   0,   0,
   0, 286, 195,   0, 155, 156, 157,   0, 159,   0,
   0, 285, 224, 179, 180, 162, 163, 164, 165, 166,
 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
 177, 178,   0,   0, 149, 246, 149, 190, 149, 193,
 246, 197,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 158, 149,
   0, 220, 246,   0, 182, 183, 187,   0, 189, 149,
 192, 149,   0, 149, 204, 205, 206, 207, 208, 209,
 210, 211, 212, 213, 214, 215, 216, 217, 218,   0,
   0,   0,   0, 222, 246, 149, 150, 246, 194, 149,
 198,   0,   0, 161,   0,   0, 188,   0, 196, 219,
 160, 221,   0, 246, 223,   0, 149, 191 };
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
# line 399 "gram.y"
{	return 2; } break;
case 2:
# line 400 "gram.y"
{	return 1; } break;
case 3:
# line 401 "gram.y"
{	return 0; } break;
case 4:
# line 403 "gram.y"
{
				set_linkage(yypvt[-1].s);
				bl_level--;
				return 1;
			} break;
case 5:
# line 409 "gram.y"
{
				set_linkage(0);
				bl_level++;
				return 1;
			} break;
case 6:
# line 417 "gram.y"
{	
			/* if function declartion with arguments
			 * need to make sure modified_tn is traversed */
			if ( yypvt[-0].pn != 0 
				&& yypvt[-0].pn->tp->base == FCT 
				&& Pfct(yypvt[-0].pn->tp)->nargs !=0 ) 
					goto mod;
			else {
				modified_tn = 0;
				if (yypvt[-0].pn==0) yyval.i = 1; 
			     }
			} break;
case 7:
# line 430 "gram.y"
{	goto mod; } break;
case 8:
# line 432 "gram.y"
{	goto mod; } break;
case 9:
# line 434 "gram.y"
{ mod:	if (modified_tn) {
					restore();
					modified_tn = 0;
				}
				local_blk = 0;
				if (local_tn) {
					extern void local_restore();
					local_restore();
					local_tn = 0;
				}
			} break;
case 10:
# line 446 "gram.y"
{	Pname n = new name(make_name('A'));
				n->tp = new basetype(ASM,0);
				Pbase(n->tp)->b_name = Pname(yypvt[-2].s);
				yyval.p = n;
			} break;
case 11:
# line 462 "gram.y"
{	
				Ptype t;
		/*	fix: */
				if (err_name == 0) err_name = yypvt[-1].pn;
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
				err_name = 0;
			} break;
case 12:
# line 489 "gram.y"
{	Pname n = Nfct(yypvt[-4].p,yypvt[-3].pn,yypvt[-0].p);
				Fargdcl(n->tp,name_unlist(yypvt[-2].nl),n);
				Finit(n->tp) = yypvt[-1].pn;
				yyval.p = n;
				NOT_EXPECT_ID();
				in_mem_fct = 0;
			} break;
case 13:
# line 499 "gram.y"
{	Pname n = Nfct(defa_type,yypvt[-3].pn,yypvt[-0].p);
				Fargdcl(n->tp,name_unlist(yypvt[-2].nl),n);
				Finit(n->tp) = yypvt[-1].pn;
				yyval.p = n;
				NOT_EXPECT_ID();
				in_mem_fct = 0;
			} break;
case 14:
# line 509 "gram.y"
{	
				yyval.p = yypvt[-0].p; 
				in_arg_list = 0;
			} break;
case 15:
# line 514 "gram.y"
{	yyval.p = 0; } break;
case 16:
# line 518 "gram.y"
{	yyval.p = yypvt[-0].p; } break;
case 17:
# line 520 "gram.y"
{	yyval.pn = yypvt[-0].pn;  yyval.pn->n_list = yypvt[-2].pn; } break;
case 18:
# line 524 "gram.y"
{
				yyval.pn = new name;
				yyval.pn->n_initializer = yypvt[-1].pe;
			} break;
case 19:
# line 529 "gram.y"
{
				Pname n = Ncopy(yypvt[-3].pn);
				n->base = yypvt[-3].pn->base;
				n->n_initializer = yypvt[-1].pe;
				yyval.pn = n;
			} break;
case 20:
# line 552 "gram.y"
{	if (yypvt[-0].pn == 0)
					error("badAD");
				else if (yypvt[-0].pn->tp->base == FCT)
					error("FD inAL (%n)",yypvt[-0].pn);
				else if (yypvt[-1].p)
                                        yypvt[-1].nl->add_list(yypvt[-0].pn);
				else
					yyval.nl = new nlist(yypvt[-0].pn);
			} break;
case 21:
# line 562 "gram.y"
{
                                yyval.p = 0; 
/* error( 'd', "arg_dcl_list: in_class_decl: %d", in_class_decl );
 */
                                if ( in_class_decl ) in_mem_fct = 1;
			} break;
case 23:
# line 572 "gram.y"
{	yyval.p = yypvt[-2].pn;
				yyval.pn->tp = new basetype(FIELD,yypvt[-0].pn);
		 	} break;
case 24:
# line 576 "gram.y"
{	yyval.p = new name;
				yyval.pn->tp = new basetype(FIELD,yypvt[-0].pn);
			} break;
case 25:
# line 580 "gram.y"
{	Pexpr e = yypvt[-0].pe;
				if (e == dummy) error("emptyIr");
				yypvt[-2].pn->n_initializer = e;
				init_seen = 0;
			} break;
case 26:
# line 588 "gram.y"
{	
				if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); 
				if ( NEXTTOK() == CM && 
					la_look() == TNAME ) 
					EXPECT_ID();
			} break;
case 27:
# line 595 "gram.y"
{	if (yypvt[-2].p)
					if (yypvt[-0].p)
						yypvt[-2].nl->add(yypvt[-0].pn);
					else
						error("DL syntax");
				else {
					if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn);
					error("DL syntax");
				}
			} break;
case 28:
# line 608 "gram.y"
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
				in_tag = 0;
				co_hack = 0;
				DECL_TYPE = 0; 
				yyval.p = n;
			} break;
case 29:
# line 624 "gram.y"
{ 
				yyval.p = yypvt[-1].pb->aggr(); 
				DECL_TYPE = 0; 
			
			} break;
case 30:
# line 633 "gram.y"
{ 
				yyval.p = new basetype(yypvt[-0].t,0); 
				if ( yypvt[-0].t == TYPEDEF ) in_typedef = yyval.pt;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 31:
# line 639 "gram.y"
{	yyval.p = new basetype(EXTERN,0);
				yyval.pb->b_linkage = yypvt[-0].s;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 32:
# line 644 "gram.y"
{ 
				yyval.p = new basetype(TYPE,yypvt[-0].pn); 
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 35:
# line 651 "gram.y"
{ 
				if (DECL_TYPE == TNAME)
					yyval.p = new basetype(TYPE,yypvt[-0].pn); 
				else
					yyval.p = new basetype(yypvt[-0].t,0); 
				DECL_TYPE = -1;
			} break;
case 37:
# line 662 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t); 
				DECL_TYPE = 0;
			} break;
case 38:
# line 668 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
			 		yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn);
				DECL_TYPE = 0;
			} break;
case 39:
# line 673 "gram.y"
{ yyval.p = yypvt[-1].pb->base_adj(yypvt[-0].pb); } break;
case 40:
# line 674 "gram.y"
{ yyval.p = yypvt[-1].pb->base_adj(yypvt[-0].pb); } break;
case 41:
# line 676 "gram.y"
{ 
				if (DECL_TYPE == TYPE)
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t);  
				else
					yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn); 
				DECL_TYPE = -1;
			} break;
case 42:
# line 688 "gram.y"
{ yyval.p = end_enum(0,yypvt[-1].pn); } break;
case 43:
# line 689 "gram.y"
{ yyval.p = end_enum(yypvt[-3].pn,yypvt[-1].pn); } break;
case 44:
# line 690 "gram.y"
{ yyval.pb = (Pbase)yypvt[-0].pn->tp;} break;
case 45:
# line 691 "gram.y"
{ yyval.pb = (Pbase)yypvt[-0].pn->tp;} break;
case 46:
# line 695 "gram.y"
{	if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); } break;
case 47:
# line 697 "gram.y"
{	if( yypvt[-0].p)
					if (yypvt[-2].p)
						yypvt[-2].nl->add(yypvt[-0].pn);
					else
						yyval.nl = new nlist(yypvt[-0].pn);
			} break;
case 48:
# line 706 "gram.y"
{	yyval.p = yypvt[-0].pn; yyval.pn->tp = moe_type; } break;
case 49:
# line 708 "gram.y"
{	yyval.p = yypvt[-2].pn;
				yyval.pn->tp = moe_type;
				yyval.pn->n_initializer = yypvt[-0].pe;
			} break;
case 50:
# line 713 "gram.y"
{	yyval.p = 0; } break;
case 51:
# line 717 "gram.y"
{	
				ccl->mem_list = name_unlist(yypvt[-1].nl);
				end_cl();
				--in_class_decl;
				declTag = 1;
			} break;
case 52:
# line 724 "gram.y"
{	
				ccl->mem_list = name_unlist(yypvt[-2].nl);
				end_cl();
				--in_class_decl;
				error("`;' or declaratorX afterCD");
				lex_unget(yypvt[-0].t);
				/* lex_unget($4); but only one unget, sorry */
				declTag = 1;
			} break;
case 53:
# line 734 "gram.y"
{ 
				yyval.pb = (Pbase)yypvt[-0].pn->tp; 
				check_tag();
			} break;
case 54:
# line 740 "gram.y"
{
				 yyval.pb = (Pbase)yypvt[-0].pn->tp; 
				check_tag();
			
			} break;
case 55:
# line 747 "gram.y"
{ yyval.pbc = yypvt[-0].pbc; } break;
case 56:
# line 748 "gram.y"
{ yyval.pbc = 0; } break;
case 58:
# line 753 "gram.y"
{	if (yypvt[-0].pbc) { yyval.pbc = yypvt[-0].pbc; yyval.pbc->next = yypvt[-2].pbc; } } break;
case 59:
# line 756 "gram.y"
{ yyval.pbc = dobase(0,yypvt[-0].pn); } break;
case 60:
# line 757 "gram.y"
{ yyval.pbc = dobase(yypvt[-1].t,yypvt[-0].pn); } break;
case 61:
# line 758 "gram.y"
{ yyval.pbc = dobase(0,yypvt[-0].pn,yypvt[-1].t); } break;
case 62:
# line 759 "gram.y"
{ yyval.pbc = dobase(yypvt[-2].t,yypvt[-0].pn,yypvt[-1].t); } break;
case 63:
# line 760 "gram.y"
{ yyval.pbc = dobase(yypvt[-1].t,yypvt[-0].pn,yypvt[-2].t); } break;
case 64:
# line 764 "gram.y"
{	
				yyval.p = start_cl(yypvt[-1].t,0,0); 
				++in_class_decl;
			} break;
case 65:
# line 770 "gram.y"
{	
				yyval.p = start_cl(yypvt[-3].t,yypvt[-2].pn,yypvt[-1].pbc);
				++in_class_decl;
			} break;
case 66:
# line 776 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 68:
# line 781 "gram.y"
{
				if (yypvt[-0].p) {
					if (yypvt[-1].p)
						yypvt[-1].nl->add_list(yypvt[-0].pn);
					else
						yyval.nl = new nlist(yypvt[-0].pn);
				}
			} break;
case 69:
# line 790 "gram.y"
{	yyval.p = 0; } break;
case 76:
# line 800 "gram.y"
{	yyval.p = new name;
				yyval.pn->base = yypvt[-1].t;
			} break;
case 77:
# line 804 "gram.y"
{	Pname n = Ncopy(yypvt[-1].pn);
				n->n_qualifier = yypvt[-2].pn;
				n->base = PR;
				yyval.p = n;
			} break;
case 78:
# line 810 "gram.y"
{	Pname n = Ncopy(yypvt[-1].pn);
				if (n->n_oper == TYPE) {
					error('s',"visibilityD for conversion operator");
					n->tp = Ptype(n->n_initializer);
					n->n_initializer = 0;
					n->n_oper = 0;
					sig_name(n);
				}
				n->n_qualifier = yypvt[-2].pn;
				n->base = PR;
				yyval.p = n;
			} break;
case 79:
# line 839 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 80:
# line 841 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_oper = DTOR;
			} break;
case 81:
# line 845 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
			} break;
case 82:
# line 849 "gram.y"
{	Pname n = yypvt[-0].pn;
				n->string = "_type";
				n->n_oper = TYPE;
				n->n_initializer = Pexpr(n->tp);
				n->tp = 0;
				yyval.p = n;
			} break;
case 95:
# line 870 "gram.y"
{	yyval.t = CALL; } break;
case 96:
# line 871 "gram.y"
{	yyval.t = DEREF; } break;
case 102:
# line 877 "gram.y"
{	yyval.t = NEW; } break;
case 103:
# line 878 "gram.y"
{	yyval.t = DELETE; } break;
case 104:
# line 879 "gram.y"
{	yyval.t = REF; } break;
case 105:
# line 880 "gram.y"
{	yyval.t = CM; } break;
case 106:
# line 881 "gram.y"
{	yyval.t = REFMUL;
					if (yypvt[-0].t == DOT) error(".* cannot be overloaded");
				} break;
case 107:
# line 886 "gram.y"
{ yyval.pn = yypvt[-0].pn; } break;
case 108:
# line 887 "gram.y"
{	error("CNs do not nest"); } break;
case 109:
# line 888 "gram.y"
{	error("CNs do not nest"); } break;
case 110:
# line 892 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = yypvt[-0].pt;
			} break;
case 111:
# line 896 "gram.y"
{	yypvt[-2].pn->tp = new fct(yypvt[-2].pn->tp,0,1);
 			} break;
case 112:
# line 900 "gram.y"
{	yypvt[-3].pn->tp = new fct(yypvt[-3].pn->tp,0,1);
 				Pfct(yypvt[-3].pn->tp)->f_const = 1;
 			} break;
case 113:
# line 904 "gram.y"
{	Pname n = yypvt[-1].pn;
				yyval.p = Ncopy(n);
				if (ccl && strcmp(n->string,ccl->string)) n->hide();
				yyval.pn->n_oper = TNAME;
				Freturns(yypvt[-0].p) = yyval.pn->tp;
				yyval.pn->tp = yypvt[-0].pt;
			} break;
case 114:
# line 916 "gram.y"
{	
				yypvt[-3].pn->tp = new fct(yypvt[-3].pn->tp,yypvt[-1].pn,1); 
 				in_arg_list = 0;
			} break;
case 115:
# line 921 "gram.y"
{ 
				Pptr p = new ptr( PTR, 0 );
				Ptyp(p) = yypvt[-2].pn->tp;
				Freturns( yypvt[-0].p ) = Ptype(p);
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].pn;
				if (DECL_TYPE == -1) DECL_TYPE = 0;
			} break;
case 116:
# line 930 "gram.y"
{	yyval.p = Ncopy(yypvt[-3].pn);
				yyval.pn->n_oper = TNAME;
				yyval.pn->tp = new fct(0,yypvt[-1].pn,1);
			} break;
case 117:
# line 935 "gram.y"
{	yyval.p = Ncopy(yypvt[-2].pn);
 				yyval.pn->n_oper = TNAME;
				yyval.pn->tp = new fct(0,0,1);
			} break;
case 118:
# line 941 "gram.y"
{	yyval.p = Ncopy(yypvt[-3].pn);
 				yyval.pn->n_oper = TNAME;
 				yyval.pn->tp = new fct(0,0,1);
 				Pfct(yypvt[-3].pn->tp)->f_const = 1;
			} break;
case 119:
# line 947 "gram.y"
{	memptrdcl(yypvt[-3].pn,yypvt[-5].pn,yypvt[-0].pt,yypvt[-2].pn);
				yyval.p = yypvt[-2].p;
			} break;
case 121:
# line 952 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_qualifier = yypvt[-2].pn;
				error(ansi_opt?0:'w',"anachronism `.' used for qualification; please use `::'");
			} break;
case 122:
# line 957 "gram.y"
{	yyval.p = yypvt[-0].p;
				if ( yypvt[-1].pn != sta_name ) {
    					set_scope(yypvt[-1].pn); 
    					yyval.pn->n_qualifier = yypvt[-1].pn;
				}
			} break;
case 123:
# line 964 "gram.y"
{	
				if ( yypvt[-1].pn == sta_name ) 
     					error( ":: applied to CN%n", yypvt[-0].pn );
				yyval.p = Ncopy(yypvt[-0].pn);
				set_scope(yypvt[-1].pn);
				yyval.pn->n_oper = TNAME;
				yyval.pn->n_qualifier = yypvt[-1].pn;
			} break;
case 124:
# line 973 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = yypvt[-1].pt;
				yyval.p = yypvt[-0].p;
			} break;
case 125:
# line 978 "gram.y"
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
case 126:
# line 990 "gram.y"
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
case 127:
# line 1001 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = yypvt[-0].pt;
			} break;
case 128:
# line 1018 "gram.y"
{ 
				yyval.p = yypvt[-1].p; 
				in_arg_list = 0;
			} break;
case 129:
# line 1025 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 130:
# line 1027 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_oper = TNAME;
				yypvt[-0].pn->hide();
				yyval.pn->tp = yypvt[-1].pt;
			} break;
case 131:
# line 1033 "gram.y"
{	
				yyval.p = new name; 
				NOT_EXPECT_ID();
			} break;
case 132:
# line 1038 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
			} break;
case 133:
# line 1043 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 134:
# line 1047 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 135:
# line 1063 "gram.y"
{ 
// error('d', "arg_lp arg_decl rp in_arg_list: %d", in_arg_list );
				yyval.p = yypvt[-1].p; 
				in_arg_list = 0;
			} break;
case 136:
# line 1071 "gram.y"
{	yyval.p = new name; } break;
case 137:
# line 1073 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
                                NOT_EXPECT_ID();
			} break;
case 138:
# line 1079 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 139:
# line 1084 "gram.y"
{ yyval.p = new name; } break;
case 140:
# line 1086 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
				NOT_EXPECT_ID();
			} break;
case 141:
# line 1092 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-1].pn->tp;
				yypvt[-1].pn->tp = (Ptype)yypvt[-0].p;
			} break;
case 142:
# line 1096 "gram.y"
{	Freturns(yypvt[-0].p) = yypvt[-2].pn->tp;
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].p;
			} break;
case 143:
# line 1101 "gram.y"
{	Vtype(yypvt[-0].p) = yypvt[-2].pn->tp;
				yypvt[-2].pn->tp = yypvt[-0].pt;
				yyval.p = yypvt[-2].p;
			} break;
case 144:
# line 1108 "gram.y"
{	yyval.p = new name; } break;
case 145:
# line 1110 "gram.y"
{	Ptyp(yypvt[-1].p) = yypvt[-0].pn->tp;
				yypvt[-0].pn->tp = (Ptype)yypvt[-1].p;
				yyval.p = yypvt[-0].p;
			} break;
case 146:
# line 1120 "gram.y"
{
				yyval.p = 0; 
			} break;
case 147:
# line 1124 "gram.y"
{	
				if (yypvt[-0].p)
					if (yypvt[-1].p)
						yypvt[-1].sl->add(yypvt[-0].ps);
					else {
						yyval.sl =  new slist(yypvt[-0].ps);
						stmt_seen = 1;
					}
			} break;
case 148:
# line 1135 "gram.y"
{
				yyval.p = yypvt[-0].p;
				if (yypvt[-0].p)	stmt_seen = 1;
			} break;
case 149:
# line 1142 "gram.y"
{
				yyval.p = 0;
				check_decl();
			} break;
case 150:
# line 1149 "gram.y"
{	yyval.p = yypvt[-1].p;
			/*	if ($<pe>$ == dummy) error("empty condition");*/
				stmt_seen = 1;
			} break;
case 151:
# line 1156 "gram.y"
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
case 152:
# line 1168 "gram.y"
{	Pname n = name_unlist(cd);
				Pstmt ss = stmt_unlist(yypvt[-1].sl);
				yyval.p = new block(yypvt[-3].l,n,ss);
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
case 153:
# line 1183 "gram.y"
{	yyval.p = new block(yypvt[-1].l,0,0); NOT_EXPECT_ID();} break;
case 154:
# line 1185 "gram.y"
{	yyval.p = new block(yypvt[-2].l,0,0); NOT_EXPECT_ID();} break;
case 155:
# line 1189 "gram.y"
{	yyval.p = new estmt(SM,curloc,yypvt[-0].pe,0);	} break;
case 156:
# line 1191 "gram.y"
{	yyval.p = new stmt(BREAK,yypvt[-0].l,0); } break;
case 157:
# line 1193 "gram.y"
{	yyval.p = new stmt(CONTINUE,yypvt[-0].l,0); } break;
case 158:
# line 1195 "gram.y"
{	yyval.p = new lstmt(GOTO,yypvt[-1].l,yypvt[-0].pn,0); } break;
case 159:
# line 1196 "gram.y"
{ stmt_seen=1; } break;
case 160:
# line 1197 "gram.y"
{	yyval.p = new estmt(DO,yypvt[-4].l,yypvt[-0].pe,yypvt[-2].ps); } break;
case 161:
# line 1199 "gram.y"
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
case 180:
# line 1235 "gram.y"
{ error("`;' missing afterS"); } break;
case 181:
# line 1237 "gram.y"
{	yyval.p = new estmt(SM,yypvt[-0].l,dummy,0); } break;
case 182:
# line 1239 "gram.y"
{	yyval.p = new estmt(RETURN,yypvt[-2].l,yypvt[-1].pe,0); } break;
case 183:
# line 1241 "gram.y"
{
				error("local linkage specification");
				yyval.p = yypvt[-0].pn;
			} break;
case 184:
# line 1246 "gram.y"
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
case 185:
# line 1261 "gram.y"
{
				Pname n = yypvt[-0].pn;
				lex_unget(RC);
				error(&n->where,"%n's definition is nested (did you forget a ``}''?)",n);
				if (cd)
					cd->add_list(n);
				else
					cd = new nlist(n);
				yyval.p = 0;
			} break;
case 187:
# line 1273 "gram.y"
{	yyval.p = new ifstmt(yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps,0); } break;
case 188:
# line 1275 "gram.y"
{	yyval.p = new ifstmt(yypvt[-4].l,yypvt[-3].pe,yypvt[-2].ps,yypvt[-0].ps); } break;
case 189:
# line 1277 "gram.y"
{	yyval.p = new estmt(WHILE,yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps); } break;
case 190:
# line 1278 "gram.y"
{ stmt_seen=1; } break;
case 191:
# line 1279 "gram.y"
{	yyval.p = new forstmt(yypvt[-8].l,yypvt[-5].ps,yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].ps); } break;
case 192:
# line 1281 "gram.y"
{	yyval.p = new estmt(SWITCH,yypvt[-2].l,yypvt[-1].pe,yypvt[-0].ps); } break;
case 193:
# line 1282 "gram.y"
{ yyval.p = yypvt[-1].pn; stmt_seen=1; } break;
case 194:
# line 1283 "gram.y"
{	Pname n = yypvt[-1].pn;
				yyval.p = new lstmt(LABEL,n->where,n,yypvt[-0].ps);
			} break;
case 195:
# line 1286 "gram.y"
{ stmt_seen=1; } break;
case 196:
# line 1287 "gram.y"
{	if (yypvt[-2].pe == dummy) error("empty case label");
				yyval.p = new estmt(CASE,yypvt[-4].l,yypvt[-2].pe,yypvt[-0].ps);
			} break;
case 197:
# line 1290 "gram.y"
{ stmt_seen=1; } break;
case 198:
# line 1291 "gram.y"
{	yyval.p = new stmt(DEFAULT,yypvt[-3].l,yypvt[-0].ps); } break;
case 199:
# line 1298 "gram.y"
{	Pexpr e = expr_unlist(yypvt[-0].el);
				while (e && e->e1==dummy) {
					register Pexpr ee2 = e->e2;
					if (ee2) error("EX inEL");
					delete e;
					e = ee2;
				}
				yyval.p = e;
			} break;
case 200:
# line 1309 "gram.y"
{	yyval.el = new elist(new expr(ELIST,yypvt[-0].pe,0)); } break;
case 201:
# line 1311 "gram.y"
{	yypvt[-2].el->add(new expr(ELIST,yypvt[-0].pe,0)); } break;
case 203:
# line 1316 "gram.y"
{
  			  	if ( in_arg_list ) 
       			  		error( "syntax error: IrL not permitted in AL" );
  			  	else 
					init_seen = 1;
				Pexpr e;
				if (yypvt[-1].p)
					e = yypvt[-1].pe;
				else
					e = new expr(ELIST,dummy,0);
				yyval.p = new expr(ILIST,e,0);
			} break;
case 204:
# line 1331 "gram.y"
{	bbinop:	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 205:
# line 1332 "gram.y"
{	goto bbinop; } break;
case 206:
# line 1333 "gram.y"
{	goto bbinop; } break;
case 207:
# line 1334 "gram.y"
{	goto bbinop; } break;
case 208:
# line 1335 "gram.y"
{	goto bbinop; } break;
case 209:
# line 1336 "gram.y"
{	goto bbinop; } break;
case 210:
# line 1337 "gram.y"
{	goto bbinop; } break;
case 211:
# line 1338 "gram.y"
{ 	goto bbinop; } break;
case 212:
# line 1339 "gram.y"
{	goto bbinop; } break;
case 213:
# line 1340 "gram.y"
{	goto bbinop; } break;
case 214:
# line 1341 "gram.y"
{	goto bbinop; } break;
case 215:
# line 1342 "gram.y"
{	goto bbinop; } break;
case 216:
# line 1343 "gram.y"
{	goto bbinop; } break;
case 217:
# line 1344 "gram.y"
{	goto bbinop; } break;
case 218:
# line 1345 "gram.y"
{	goto bbinop; } break;
case 219:
# line 1347 "gram.y"
{	yyval.p = new qexpr(yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].pe); } break;
case 220:
# line 1349 "gram.y"
{	yyval.p = new expr(DELETE,yypvt[-0].pe,0); } break;
case 221:
# line 1351 "gram.y"
{	yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe); } break;
case 222:
# line 1353 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,0); } break;
case 223:
# line 1355 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,yypvt[-2].pe); } break;
case 225:
# line 1360 "gram.y"
{	binop:	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 226:
# line 1361 "gram.y"
{	goto binop; } break;
case 227:
# line 1362 "gram.y"
{	goto binop; } break;
case 228:
# line 1363 "gram.y"
{	goto binop; } break;
case 229:
# line 1364 "gram.y"
{	goto binop; } break;
case 230:
# line 1365 "gram.y"
{	goto binop; } break;
case 231:
# line 1366 "gram.y"
{	goto binop; } break;
case 232:
# line 1367 "gram.y"
{ 	goto binop; } break;
case 233:
# line 1368 "gram.y"
{	goto binop; } break;
case 234:
# line 1369 "gram.y"
{	goto binop; } break;
case 235:
# line 1370 "gram.y"
{	goto binop; } break;
case 236:
# line 1371 "gram.y"
{	goto binop; } break;
case 237:
# line 1372 "gram.y"
{	goto binop; } break;
case 238:
# line 1373 "gram.y"
{	goto binop; } break;
case 239:
# line 1374 "gram.y"
{	goto binop; } break;
case 240:
# line 1376 "gram.y"
{	yyval.p = new qexpr(yypvt[-4].pe,yypvt[-2].pe,yypvt[-0].pe); } break;
case 241:
# line 1378 "gram.y"
{	yyval.p = new expr(DELETE,yypvt[-0].pe,0); } break;
case 242:
# line 1380 "gram.y"
{	yyval.p = new expr(DELETE,yypvt[-0].pe,yypvt[-2].pe); } break;
case 243:
# line 1382 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,0); } break;
case 244:
# line 1384 "gram.y"
{	yyval.p = new expr(GDELETE,yypvt[-0].pe,yypvt[-2].pe); } break;
case 245:
# line 1385 "gram.y"
{ 
			init_seen = 0; 
			} break;
case 246:
# line 1389 "gram.y"
{	yyval.p = dummy; } break;
case 247:
# line 1392 "gram.y"
{ goto new1; } break;
case 248:
# line 1394 "gram.y"
{	new1:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p = new texpr(NEW,t,0);
			} break;
case 249:
# line 1398 "gram.y"
{ goto new3; } break;
case 250:
# line 1400 "gram.y"
{	new3:
				Ptype t = yypvt[-0].pn->tp;
 				yyval.p = new texpr(GNEW,t,0);
 			} break;
case 251:
# line 1405 "gram.y"
{	yyval.p = new expr(yypvt[-0].t,yypvt[-1].pe,0); } break;
case 252:
# line 1407 "gram.y"
{	yyval.p = new texpr(CAST,yypvt[-1].pn->tp,yypvt[-0].pe); } break;
case 253:
# line 1409 "gram.y"
{	yyval.p = new expr(DEREF,yypvt[-0].pe,0); } break;
case 254:
# line 1411 "gram.y"
{	yyval.p = new expr(ADDROF,0,yypvt[-0].pe); } break;
case 255:
# line 1413 "gram.y"
{	yyval.p = new expr(UMINUS,0,yypvt[-0].pe); } break;
case 256:
# line 1415 "gram.y"
{	yyval.p = new expr(UPLUS,0,yypvt[-0].pe); } break;
case 257:
# line 1417 "gram.y"
{	yyval.p = new expr(NOT,0,yypvt[-0].pe); } break;
case 258:
# line 1419 "gram.y"
{	yyval.p = new expr(COMPL,0,yypvt[-0].pe); } break;
case 259:
# line 1421 "gram.y"
{	yyval.p = new expr(yypvt[-1].t,0,yypvt[-0].pe); } break;
case 260:
# line 1423 "gram.y"
{	yyval.p = new texpr(SIZEOF,0,yypvt[-0].pe); } break;
case 261:
# line 1425 "gram.y"
{	yyval.p = new texpr(SIZEOF,yypvt[-0].pn->tp,0); } break;
case 262:
# line 1427 "gram.y"
{	yyval.p = new expr(DEREF,yypvt[-3].pe,yypvt[-1].pe); } break;
case 263:
# line 1429 "gram.y"
{	yyval.p = new ref(REF,yypvt[-2].pe,yypvt[-0].pn); } break;
case 264:
# line 1431 "gram.y"
{	yyval.p = new expr(yypvt[-1].t,yypvt[-2].pe,yypvt[-0].pe); } break;
case 265:
# line 1433 "gram.y"
{	yyval.p = new ref(REF,yypvt[-2].pe,Ncopy(yypvt[-0].pn)); } break;
case 266:
# line 1435 "gram.y"
{	yyval.p = new ref(DOT,yypvt[-2].pe,yypvt[-0].pn); } break;
case 267:
# line 1437 "gram.y"
{	yyval.p = new ref(DOT,yypvt[-2].pe,Ncopy(yypvt[-0].pn)); } break;
case 269:
# line 1440 "gram.y"
{
			if ( init_seen )
     				error( "syntax error:IrL illegal within ()");
			} break;
case 270:
# line 1446 "gram.y"
{	yyval.p = yypvt[-1].p; } break;
case 271:
# line 1448 "gram.y"
{	yyval.p = zero; } break;
case 272:
# line 1450 "gram.y"
{	yyval.p = new expr(ICON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 273:
# line 1454 "gram.y"
{	yyval.p = new expr(FCON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 274:
# line 1458 "gram.y"
{	yyval.p = new expr(STRING,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 275:
# line 1462 "gram.y"
{	yyval.p = new expr(CCON,0,0);
				yyval.pe->string = yypvt[-0].s;
			} break;
case 276:
# line 1466 "gram.y"
{	yyval.p = new expr(THIS,0,0); } break;
case 277:
# line 1470 "gram.y"
{ 	yyval.p = new texpr(VALUE,tok_to_type(yypvt[-3].t),yypvt[-1].pe); } break;
case 278:
# line 1472 "gram.y"
{	yyval.p = new texpr(VALUE,yypvt[-3].pn->tp,yypvt[-1].pe); } break;
case 279:
# line 1473 "gram.y"
{ goto new2; } break;
case 280:
# line 1475 "gram.y"
{	new2:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p=new texpr(NEW,t,0);
				yyval.pe->e2 = yypvt[-2].pe;
			} break;
case 281:
# line 1480 "gram.y"
{ goto new4; } break;
case 282:
# line 1482 "gram.y"
{	new4:
				Ptype t = yypvt[-0].pn->tp;
				yyval.p = new texpr(GNEW,t,0);
				yyval.pe->e2 = yypvt[-2].pe;
			} break;
case 283:
# line 1488 "gram.y"
{	
				Pexpr ee = yypvt[-1].pe;
				Pexpr e = yypvt[-3].pe;
				if (e->base==NEW || e->base==GNEW)
					e->e1 = ee;
				else
					yyval.p = new call(e,ee);
			} break;
case 284:
# line 1499 "gram.y"
{       yyval.pn = yypvt[-0].pn; } break;
case 285:
# line 1501 "gram.y"
{       yyval.pn = sta_name; } break;
case 286:
# line 1506 "gram.y"
{	yyval.p = yypvt[-0].pn; } break;
case 287:
# line 1508 "gram.y"
{	yyval.p = Ncopy(yypvt[-0].pn);
				yyval.pn->n_qualifier = yypvt[-1].pn;
			} break;
case 288:
# line 1512 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
			} break;
case 289:
# line 1516 "gram.y"
{	yyval.p = new name(oper_name(yypvt[-0].t));
				yyval.pn->n_oper = yypvt[-0].t;
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 290:
# line 1521 "gram.y"
{	yyval.p = yypvt[-0].p;
				sig_name(yyval.pn);
			} break;
case 291:
# line 1525 "gram.y"
{	yyval.p = yypvt[-0].p;
				sig_name(yyval.pn);
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 292:
# line 1530 "gram.y"
{	
				if (strcmp(yypvt[-2].pn->string,yypvt[-0].pn->string)) error("syntax error: inconsistent destructor notation");
				yyval.p = new name(oper_name(DTOR));
				yyval.pn->n_oper = DTOR;
				yyval.pn->n_qualifier = yypvt[-2].pn;
			} break;
case 293:
# line 1542 "gram.y"
{ yyval.p = Ncast(yypvt[-2].p,yypvt[-1].pn); } break;
case 294:
# line 1545 "gram.y"
{ check_cast(); } break;
case 295:
# line 1549 "gram.y"
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
case 296:
# line 1568 "gram.y"
{ yyval.p = new basetype(TYPE,yypvt[-0].pn); } break;
case 297:
# line 1570 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
					yyval.p = yypvt[-1].pb->type_adj(yypvt[-0].t); 
				DECL_TYPE = 0;
			} break;
case 298:
# line 1576 "gram.y"
{ 
				if ( DECL_TYPE != -1 ) 
			 		yyval.p = yypvt[-1].pb->name_adj(yypvt[-0].pn);
				DECL_TYPE = 0;
			} break;
case 299:
# line 1583 "gram.y"
{ yyval.p = Ncast(yypvt[-1].p,yypvt[-0].pn); } break;
case 300:
# line 1586 "gram.y"
{ yyval.p = Ncast(yypvt[-1].p,yypvt[-0].pn); } break;
case 301:
# line 1589 "gram.y"
{	yyval.p = Ndata(yypvt[-1].p,yypvt[-0].pn); } break;
case 302:
# line 1591 "gram.y"
{	yyval.p = Ndata(yypvt[-3].p,yypvt[-2].pn);
				yyval.pn->n_initializer = yypvt[-0].pe;
			} break;
case 303:
# line 1597 "gram.y"
{ 
				check_decl(); 
				in_arg_list=1; 
				if ( in_class_decl ) in_mem_fct = 1; 
			} break;
case 304:
# line 1605 "gram.y"
{ 
			yyval.p = new fct(0,name_unlist(yypvt[-2].nl),yypvt[-1].t); 
			if ( NEXTTOK() != COLON ) in_arg_list=0;
		    } break;
case 305:
# line 1610 "gram.y"
{ 
			yyval.p = new fct(0,name_unlist(yypvt[-3].nl),yypvt[-2].t); 
			in_arg_list=0;
			if (yypvt[-0].t != CONST)
			if ( la_look() != SM ) {
				error("syntax error: unexpected%k (did you forget a `;'?)",yypvt[-0].t);
                        	{ YYSTYPE y; y.t = SM; la_backup(SM,y); }
                        	lex_unget(yypvt[-0].t);
 			}
				else error("syntax error: unexpected%k",yypvt[-0].t);
			Pfct(yyval.pt)->f_const = 1;
		    } break;
case 306:
# line 1625 "gram.y"
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
case 307:
# line 1637 "gram.y"
{
				if (yypvt[-0].p) yyval.nl = new nlist(yypvt[-0].pn); 
			} break;
case 309:
# line 1643 "gram.y"
{	yyval.p = 0; } break;
case 310:
# line 1647 "gram.y"
{	yyval.t = 1; } break;
case 311:
# line 1649 "gram.y"
{	yyval.t = ELLIPSIS; } break;
case 312:
# line 1651 "gram.y"
{	yyval.t = ELLIPSIS; } break;
case 313:
# line 1655 "gram.y"
{
			yyval.p = new ptr(PTR,0); 
			EXPECT_ID();
			} break;
case 314:
# line 1660 "gram.y"
{
			yyval.p = new ptr(RPTR,0); 
			EXPECT_ID();
			} break;
case 315:
# line 1665 "gram.y"
{	yyval.p = doptr(PTR,yypvt[-0].t); } break;
case 316:
# line 1667 "gram.y"
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
case 317:
# line 1680 "gram.y"
{	yyval.p = doptr(RPTR,yypvt[-0].t); } break;
case 318:
# line 1682 "gram.y"
{
			yyval.p = new ptr(PTR,0);
			yyval.pp->memof = Pclass(Pbase(yypvt[-0].pn->tp)->b_name->tp);
			EXPECT_ID();
			} break;
case 319:
# line 1688 "gram.y"
{	yyval.p = doptr(PTR,yypvt[-0].t);
				yyval.pp->memof = Pclass(Pbase(yypvt[-1].pn->tp)->b_name->tp);
			} break;
case 320:
# line 1693 "gram.y"
{ yyval.p = new vec(0,yypvt[-1].pe!=dummy?yypvt[-1].pe:0 ); } break;
case 321:
# line 1694 "gram.y"
{ yyval.p = new vec(0,0); } break;
	}
	goto yystack;  /* stack new state and value */
}
