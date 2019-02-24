
# line 3 "cgram.y"
static	char	SCCSID[] = "@(#) cgram.y:	2.1 83/08/02";
# define NAME 2
# define STRING 3
# define ICON 4
# define FCON 5
# define PLUS 6
# define MINUS 8
# define MUL 11
# define AND 14
# define OR 17
# define ER 19
# define QUEST 21
# define COLON 22
# define ANDAND 23
# define OROR 24
# define ASOP 25
# define RELOP 26
# define EQUOP 27
# define DIVOP 28
# define SHIFTOP 29
# define INCOP 30
# define UNOP 31
# define STROP 32
# define TYPE 33
# define CLASS 34
# define STRUCT 35
# define RETURN 36
# define GOTO 37
# define IF 38
# define ELSE 39
# define SWITCH 40
# define BREAK 41
# define CONTINUE 42
# define WHILE 43
# define DO 44
# define FOR 45
# define DEFAULT 46
# define CASE 47
# define SIZEOF 48
# define ENUM 49
# define LP 50
# define RP 51
# define LC 52
# define RC 53
# define LB 54
# define RB 55
# define CM 56
# define SM 57
# define ASSIGN 58
# define ASM 59

# line 112 "cgram.y"
# include "mfile1.h"
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;

# line 131 "cgram.y"
	extern int wloop_level;	/* specifies while loop code generation */
	extern int floop_level;	/* specifies for loop code generation */
	static int fake = 0;
	static char fakename[NCHNAM+1];
# define YYERRCODE 256

# line 1064 "cgram.y"


NODE *
mkty( t, d, s ) unsigned t; {
	return( block( TYPE, NIL, NIL, t, d, s ) );
	}

NODE *
bdty( op, p, v ) NODE *p; {
	register NODE *q;

	q = block( op, p, NIL, INT, 0, INT );

	switch( op ){

	case UNARY MUL:
	case UNARY CALL:
		break;

	case LB:
		q->in.right = bcon(v);
		break;

	case NAME:
		q->tn.rval = v;
		break;

	default:
		cerror( "bad bdty" );
		}

	return( q );
	}

dstash( n ){ /* put n into the dimension table */
	if( curdim >= DIMTABSZ-1 ){
		cerror( "dimension table overflow");
		}
	dimtab[ curdim++ ] = n;
	}

savebc() {
	if( psavbc > & asavbc[BCSZ-4 ] ){
		cerror( "whiles, fors, etc. too deeply nested");
		}
	*psavbc++ = brklab;
	*psavbc++ = contlab;
	*psavbc++ = flostat;
	*psavbc++ = swx;
#ifdef M32B
	*psavbc++ = swregno;
#endif
	flostat = 0;
	}

resetbc(mask){

#ifdef M32B
	swregno = *--psavbc;
#endif
	swx = *--psavbc;
	flostat = *--psavbc | (flostat&mask);
	contlab = *--psavbc;
	brklab = *--psavbc;

	}

addcase(p) NODE *p; { /* add case to switch */

	p = optim( p );  /* change enum to ints */
	if( p->in.op != ICON ){
		uerror( "non-constant case expression");
		return;
		}
	if( swp == swtab ){
		uerror( "case not in switch");
		return;
		}
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	swp->sval = p->tn.lval;
	deflab( swp->slab = getlab() );
	++swp;
	tfree(p);
	}

adddef(){ /* add default case to switch */
	if( swtab[swx].slab >= 0 ){
		uerror( "duplicate default in switch");
		return;
		}
	if( swp == swtab ){
		uerror( "default not inside switch");
		return;
		}
	deflab( swtab[swx].slab = getlab() );
	}

swstart(){
	/* begin a switch block */
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	swx = swp - swtab;
	swp->slab = -1;
	++swp;
	}

swend(){ /* end a switch block */

	register struct sw *swbeg, *p, *q, *r, *r1;
	CONSZ temp;
	int tempi;

	swbeg = &swtab[swx+1];

	/* sort */

	r1 = swbeg;
	r = swp-1;

	while( swbeg < r ){
		/* bubble largest to end */
		for( q=swbeg; q<r; ++q ){
			if( q->sval > (q+1)->sval ){
				/* swap */
				r1 = q+1;
				temp = q->sval;
				q->sval = r1->sval;
				r1->sval = temp;
				tempi = q->slab;
				q->slab = r1->slab;
				r1->slab = tempi;
				}
			}
		r = r1;
		r1 = swbeg;
		}

	/* it is now sorted */

	for( p = swbeg+1; p<swp; ++p ){
		if( p->sval == (p-1)->sval ){
			uerror( "duplicate case in switch, %d", tempi=p->sval );
			return;
			}
		}

	reached = 1;
	genswitch( swbeg-1, (int)(swp-swbeg) );
	swp = swbeg-1;
	}
short yyexca[] ={
-1, 1,
	0, -1,
	2, 20,
	11, 20,
	50, 20,
	57, 20,
	-2, 0,
-1, 21,
	56, 80,
	57, 80,
	-2, 8,
-1, 25,
	51, 68,
	-2, 66,
-1, 26,
	58, 77,
	-2, 79,
-1, 28,
	58, 78,
	-2, 83,
-1, 34,
	52, 43,
	-2, 41,
-1, 36,
	52, 35,
	-2, 33,
-1, 56,
	53, 47,
	57, 47,
	-2, 0,
	};
# define YYNPROD 184
# define YYLAST 1176
short yyact[]={

 234, 232,  70,  20, 290,  11,  10,  15,  78,  94,
 101, 198,   7,  99, 253,  90,  88,  89,  29, 147,
  81,  17,  98,  79, 132, 131,  80,  23,  29,  29,
 311,   5,  93,  96, 236, 103,  58,  23,  23,  55,
  22, 277,  83,  82,  50, 276,  65, 310, 245, 246,
 250,  76, 252, 243, 244, 239, 251, 241, 255, 254,
  84, 100,  85, 300, 109, 233,  24, 111, 163, 247,
 293, 235,   9, 230, 272, 107,  24,  24, 133,  38,
 262,  30, 271,  19, 191,  99, 148, 152, 140, 141,
 142, 143, 144, 145,  98, 264,  75, 106, 156, 205,
  92, 104,  38,  37,  52,  96,  56, 202, 112,  18,
 156, 102, 201, 113, 285, 259, 166, 167, 168, 170,
 172, 174, 176, 178, 179, 181, 183, 185, 186, 187,
 188, 189, 155, 100, 114, 133, 110,  26, 193, 160,
 162, 157, 190, 158, 136, 261, 138, 148, 224, 224,
 161,  87,  90,  88,  89,  40,  68,  74, 149,  41,
 192,  42,  44,  40, 139,  56, 195,  41, 137,  48,
 208,  48, 209,  49, 210,  49, 211, 206, 212,  83,
  82, 213,  64, 214, 257, 215,  48,  67, 217,  99,
  49, 159, 133, 200,  95, 154,  35,  84,  98,  85,
  33, 314, 228, 219, 204, 223,  29,  11,  10,  15,
 231, 226, 227, 229, 297,  23, 218, 256, 222, 149,
 196,  73, 258,  17, 281,  11, 109,  15,   4,  72,
  63, 223, 199, 225, 200, 299,  42, 100,  44, 265,
 266,  17, 268, 288, 270,  95, 274, 286,  11,  10,
  15, 278, 153, 280,  24, 282,  71,  87,  90,  88,
  89, 279, 269,  81,  17, 289,  79,  97, 248,  80,
 291, 260,  28, 199, 153,  51, 287,  10,  53,  32,
 294, 295,  28,  28, 121,  83,  82, 283,   8, 298,
 253,  90,  88,  89, 275, 194,  81, 105,  31,  79,
 304, 117,  80,  84,  21,  85, 291, 165, 309,  59,
 308,  36, 273, 291,  34,  66, 315, 313,  83,  82,
 249,  43,  45, 312, 245, 246, 250, 305, 252, 243,
 244, 239, 251, 241, 255, 254,  84, 306,  85,  97,
 109,  95, 267, 108, 118, 247, 119, 235, 134, 121,
  27,  60, 123,  47,  69, 124,  46, 125, 203, 128,
 221, 126, 127, 129, 115, 122, 117, 120, 118,  91,
 119,  54,  57, 121, 164, 118, 123, 119, 207, 124,
 121, 125,  62, 128,  61, 126, 127, 129, 115, 122,
 117, 120,  39, 220, 116, 115, 130, 117, 120,   3,
   2, 151,  86,  12, 118,  13, 119,   6,  25, 121,
  14,  16, 123, 307, 242, 124, 240, 125, 116, 128,
 130, 126, 127, 129, 115, 122, 117, 120, 118, 237,
 119, 238,   1, 121,   0,   0,   0,   0,   0,   0,
 118,   0, 119,   0,   0, 121,   0,   0, 123, 303,
 117, 124,   0, 125, 116, 128, 130, 126, 127, 129,
 115, 122, 117, 120,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 118,   0, 119,   0,
   0, 121,   0,   0, 123, 302,   0, 124,   0, 125,
 116, 128, 130, 126, 127, 129, 115, 122, 117, 120,
   0, 118,   0, 119,   0,   0, 121,   0,   0,   0,
   0,   0,   0,   0, 118,   0, 119,   0,   0, 121,
   0, 301, 123, 117, 120, 124, 116, 125, 130, 128,
 296, 126, 127, 129, 115, 122, 117, 120, 118,   0,
 119,   0,   0, 121, 248,   0, 123,   0,   0, 124,
   0, 125,   0, 128,   0, 126, 127, 129, 115, 122,
 117, 120,   0, 118, 116, 119, 130,   0, 121,   0,
   0, 123,   0,   0, 124,   0, 125,   0, 128,   0,
 126, 127, 129, 115, 122, 117, 120,   0, 116, 292,
 130,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 118,   0, 119,   0,   0, 121,   0,
   0, 123, 284, 116, 124, 130, 125,   0, 128,   0,
 126, 127, 129, 115, 122, 117, 120, 118,   0, 119,
   0,   0, 121,   0,   0, 123,   0,   0, 124,   0,
 125,   0, 128, 216, 126, 127, 129, 115, 122, 117,
 120,   0,   0, 116, 263, 130,   0,   0,   0,   0,
   0,   0,   0, 118,   0, 119,   0, 118, 121, 119,
   0, 123, 121,   0, 124, 123, 125, 116, 128, 130,
 126, 127, 129, 115, 122, 117, 120, 115, 122, 117,
 120,   0,   0,   0,   0,   0,   0,   0,   0, 118,
   0, 119,   0,   0, 121,   0,   0, 123, 197,   0,
 124,   0, 125, 116, 128, 130, 126, 127, 129, 115,
 122, 117, 120, 118,   0, 119,   0,   0, 121,   0,
   0, 123,   0,   0, 124,   0, 125,   0, 128,   0,
 126, 127, 129, 115, 122, 117, 120,   0,   0, 116,
   0, 130,  87,  90,  88,  89,   0,   0,  81,   0,
   0,  79,   0,   0,  80,  87,  90,  88,  89,   0,
   0,  81,   0,   0,  79, 130,   0,  80,   0,   0,
  83,  82,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  83,  82,   0,   0,   0,  84,   0,
  85,   0,   0,  87,  90,  88,  89,   0, 184,  81,
   0,  84,  79,  85,   0,  80,  87,  90,  88,  89,
   0, 182,  81,   0,   0,  79,   0,   0,  80,   0,
   0,  83,  82,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  83,  82,   0,   0,   0,  84,
   0,  85,   0,   0,  87,  90,  88,  89,   0, 180,
  81,   0,  84,  79,  85,   0,  80,  87,  90,  88,
  89,   0, 177,  81,   0,   0,  79,   0,   0,  80,
   0,   0,  83,  82,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  83,  82,   0,   0,   0,
  84,   0,  85,   0,   0,  87,  90,  88,  89,   0,
 175,  81,   0,  84,  79,  85,   0,  80,  87,  90,
  88,  89,   0, 173,  81,   0,   0,  79,   0,   0,
  80,   0,   0,  83,  82,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  83,  82,   0,   0,
   0,  84,   0,  85,   0,   0,  87,  90,  88,  89,
   0, 171,  81,   0,  84,  79,  85,   0,  80,   0,
  87,  90,  88,  89, 169,   0,  81,   0,   0,  79,
   0,   0,  80,   0,  83,  82,   0, 118,   0, 119,
   0, 118, 121, 119,   0, 123, 121,   0,  83,  82,
 125,  11,  84,  15,  85,   0, 135, 115, 122, 117,
 120, 115, 122, 117, 120,   0,  84,  17,  85,  87,
  90,  88,  89,   0,   0,  81,   0,   0,  79,   0,
   0,  80,  87,  90,  88,  89,   0,   0,  81,   0,
   0,  79,   0,   0,  80,   0,   0,  83,  82,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  83,  82,   0,   0,   0,  84,   0,  85,   0,  77,
  87,  90,  88,  89,   0,   0,  81,   0,  84,  79,
  85, 150,  80,  87,  90,  88,  89,   0,   0,  81,
   0,   0,  79,   0,   0,  80,   0,   0,  83,  82,
   0,   0, 118,   0, 119,   0,   0, 121,   0,   0,
 123,  83,  82, 124,   0, 125,  84, 128,  85, 126,
 127,   0, 115, 122, 117, 120,   0,   0, 118,  84,
 119, 146,   0, 121,   0,   0, 123,   0,   0, 124,
   0, 125,   0,   0,   0, 126,   0,   0, 115, 122,
 117, 120, 118,   0, 119,   0,   0, 121,   0,   0,
 123,   0,   0, 124,   0, 125,   0,   0,   0,   0,
   0,   0, 115, 122, 117, 120 };
short yypact[]={

-1000, -28,-1000,-1000,-1000,  52,  26,-1000, 192, 243,
-1000, 246,-1000,-1000, 148, 312, 144, 309,-1000,-1000,
  46, 113,-1000, 204, 204,-1000, 121, -14,-1000, 225,
-1000, 192, 245, 192,-1000, 307,-1000,-1000,-1000,-1000,
 179, 127, 121, 113, 136, 105,   0, 178, 170, 102,
1017,-1000,-1000,-1000,  43,-1000,  83,  55,-1000, -23,
  27,-1000, 174,-1000,-1000,  81,1068,-1000,-1000,  57,
-1000,-1000,-1000,-1000,-1000,  79, 717, 954, 114,1068,
1068,1068,1068,1068,1081, 968,1030, 224,-1000,-1000,
-1000, 142, 192,  54,-1000, 119,-1000,-1000, 187,-1000,
 187, 138, 307,-1000,-1000, 113,-1000,  11,-1000,-1000,
-1000, 717,-1000, 305,-1000,1068,1068, 916, 903, 865,
 852, 814,1068, 801, 763, 750,1068,1068,1068,1068,
1068,  28,-1000, 717, 954,-1000,-1000,1068, 293,-1000,
 114, 114, 114, 114, 114, 114, 968, 169, 657, 223,
-1000,  56, 717,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,  42,-1000, 215,-1000, 495, 717,-1000,1068,
 273,1068, 273,1068, 422,1068,-1000,1068, 369, 985,
1068, 981,1068, 661,1068,1146,1122, 621, 717, 717,
 135, 954,  28, 338,-1000, 167,1068,-1000,  95, 182,
 223,1068,-1000,  83,-1000,-1000,  16,  12, 717, 717,
 717, 717, 717, 717, 717, 717,1068,-1000,-1000, 131,
-1000,1068, 149, 114,  60,-1000,  94,  95, 717,-1000,
-1000,  23,-1000,-1000, 597,  38,-1000, 288, 288,-1000,
 288, 212, 288,  25,  17, 255, 292,-1000, -12, 288,
 211,-1000, 203, 202,1068, 265,1096,-1000, 557,-1000,
  59, 197,-1000,-1000,-1000, 237,-1000, 193, 222,1068,
-1000,-1000,-1000,-1000, 532,  13,-1000,-1000,-1000,1068,
1068,-1000, 508,-1000,-1000,-1000, 163,-1000,1068, 185,
   6, 693,-1000,-1000, 470, 434,-1000,-1000, 398,1068,
-1000,-1000,-1000,-1000, 362,1068, 288, -10, -27,-1000,
-1000,-1000,1068, 150, 288,-1000 };
short yypgo[]={

   0, 432,  46, 431, 429, 416, 414, 411, 410, 408,
   0,   4,   8,  12, 407,  72, 405, 403,  19,  11,
 402,   9, 297, 137, 401, 400, 399,   3, 392, 384,
 382,  34,  32, 378,   1, 374, 288, 372,  10,  36,
 371, 369,  39, 358, 356, 354, 353,  40, 351, 350,
  25,  24, 348, 343, 342, 337, 327, 323, 320, 315 };
short yyr1[]={

   0,   1,   1,  25,  25,  25,  26,  26,  28,  26,
  29,  30,  30,  30,  33,  33,  35,  35,  35,  14,
  14,  13,  13,  13,  13,  13,  36,  15,  15,  15,
  15,  15,  16,  16,   7,   7,  37,  37,  39,  39,
  17,  17,   8,   8,  40,  40,  42,  42,  32,  43,
  32,  21,  21,  21,  21,  23,  23,  23,  23,  23,
  23,  22,  22,  22,  22,  22,  44,  22,  46,  22,
   9,  45,  45,  45,  27,  48,  27,  49,  49,  47,
  47,  47,  47,  47,  50,  50,  51,  51,  38,  38,
  41,  41,  52,  31,  53,  34,  34,  34,  34,  34,
  54,  55,  34,  34,  56,  57,  34,  34,  34,  34,
  34,  34,  34,  34,  34,  34,  34,  58,  58,  58,
   5,   4,   3,   6,  59,   2,  11,  11,  24,  24,
  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  18,  19,  19,  19,  19,  19,
  19,  19,  20,  20 };
short yyr2[]={

   0,   2,   0,   1,   1,   2,   2,   3,   0,   4,
   2,   4,   3,   0,   2,   0,   3,   4,   0,   1,
   0,   2,   2,   1,   1,   3,   1,   1,   2,   3,
   1,   1,   5,   2,   1,   2,   1,   3,   1,   3,
   5,   2,   1,   2,   1,   3,   2,   1,   1,   0,
   4,   1,   3,   2,   1,   2,   3,   3,   4,   1,
   3,   2,   3,   3,   4,   3,   0,   4,   0,   3,
   2,   1,   3,   1,   1,   0,   4,   1,   1,   1,
   1,   3,   6,   1,   1,   3,   1,   4,   0,   1,
   0,   1,   1,   4,   1,   2,   2,   1,   2,   2,
   0,   0,   7,   7,   0,   0,  11,   2,   2,   2,
   2,   3,   3,   1,   2,   2,   2,   2,   3,   2,
   1,   4,   3,   4,   0,   2,   1,   0,   1,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,
   4,   5,   3,   3,   1,   2,   2,   2,   2,   2,
   2,   2,   4,   4,   4,   6,   2,   3,   3,   1,
   1,   1,   1,   3,   2,   0,   2,   5,   2,   3,
   4,   3,   2,   2 };
short yychk[]={

-1000,  -1, -25, -26, 256,  59, -14, -13, -36, -15,
  34,  33, -17, -16,  -8,  35,  -7,  49,  57,  57,
 -27, -22, -47,  11,  50,  -9, -23, -49, 256,   2,
 -15, -36,  33,  52,   2,  52,   2,  57,  56, -28,
  50,  54, -23, -22, -23, -22, -44, -46,  50,  54,
  58,  50, -15,  33, -40, -42, -15, -37, -39,   2,
 -48, -29, -30,  51,  55,  -2, -59,  51,  51, -45,
   2, 256,  51,  51,  55,  -2, -10,  52, -12,  11,
  14,   8,  31,  30,  48,  50, -20,   2,   4,   5,
   3, -41,  57, -32, -21, -23,  22, 256,  11,   2,
  50, -38,  56,  58, -47, -22, -31, -13, -53,  52,
  55, -10,  51,  56,  55,  26,  56,  28,   6,   8,
  29,  11,  27,  14,  17,  19,  23,  24,  21,  25,
  58, -50, -51, -10, -52,  52,  30,  54,  32,  50,
 -12, -12, -12, -12, -12, -12,  50, -18, -10, -15,
  51, -24, -10,  50,  53, -42,  56,  22,  -2,  53,
 -39,  -2, -32,  57, -35,   2, -10, -10, -10,  58,
 -10,  58, -10,  58, -10,  58, -10,  58, -10, -10,
  58, -10,  58, -10,  58, -10, -10, -10, -10, -10,
 -38,  56, -50, -10,   2, -18,  51,  51, -19,  50,
  11,  56,  51, -43,  -2,  57, -13, -33, -10, -10,
 -10, -10, -10, -10, -10, -10,  22,  53, -51, -38,
  55,  22,  51, -12,  54,  51, -19, -19, -10, -21,
  57, -27, -34,  53, -10,  59, -31,  -4,  -3,  43,
  -5,  45,  -6,  41,  42,  36,  37,  57, 256, -58,
  38,  44,  40,   2,  47,  46, -10,  53, -10,  55,
  -2,  51,  57,  57,  57, -34, -34, -54, -34,  50,
 -34,  57,  57,  57, -10,   2,  57,  53, -34,  50,
  50,  22, -10,  22,  55,  55,  50,  39,  50,  43,
 -11, -10,  57,  57, -10, -10,  22,  51, -10,  50,
  57,  51,  51,  51, -10, -56, -55,  51, -11, -34,
  57,  57, -57, -11,  51, -34 };
short yydef[]={

   2,  -2,   1,   3,   4,   0,   0,  19,  23,  24,
  26,  27,  30,  31,   0,  42,   0,  34,   5,   6,
   0,  -2,  74,   0,   0,  -2,  -2,   0,  -2,  59,
  21,  22,  28,   0,  -2,   0,  -2,   7,  75,  13,
   0, 124,  55,  61,   0,   0,   0,   0,   0, 124,
   0,  70,  25,  29,  90,  44,  -2,  88,  36,  38,
   0,   9,   0,  62,  63,   0,   0,  60,  65,   0,
  71,  73,  69,  56,  57,   0,  81,   0, 154,   0,
   0,   0,   0,   0,   0,   0,   0, 169, 170, 171,
 172,   0,  91,  46,  48,  51, 124,  54,   0,  59,
   0,   0,  89, 124,  76,  80,  10,   0,  18,  94,
  64, 125,  67,   0,  58,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  88,  84,  86,   0,  92, 155,   0,   0, 183,
 156, 157, 158, 159, 160, 161,   0,   0,   0, 175,
 166,   0, 128, 182,  40,  45,  49, 124,  53,  32,
  37,  39,   0,  12,  15,  72, 130, 131, 132,   0,
 133,   0, 134,   0, 135,   0, 136,   0, 137, 138,
   0, 139,   0, 140,   0, 141, 142,   0, 152, 153,
   0,  89,  88,   0, 168,   0,   0, 173, 174, 175,
 175,   0, 167,   0,  52,  11,   0,   0, 144, 145,
 146, 147, 143, 148, 149, 150,   0,  82,  85,   0,
 164,   0, 163, 162, 124, 176,   0, 178, 129,  50,
  16,   0,  14,  93,   0,   0,  97,   0,   0, 100,
   0,   0,   0,   0,   0,   0,   0, 113,   0,   0,
   0, 120,   0, 169,   0,   0, 151,  87,   0, 179,
   0, 181,  17,  95,  96,  98,  99,   0,   0, 127,
 107, 108, 109, 110,   0,   0, 114, 115, 116,   0,
   0, 117,   0, 119, 165, 180,   0, 122,   0,   0,
   0, 126, 111, 112,   0,   0, 118, 177,   0,   0,
 104, 121, 123, 101,   0, 127,   0,   0,   0, 102,
 103, 105, 127,   0,   0, 106 };
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
		
case 2:
# line 139 "cgram.y"
{
#ifndef LINT
			beg_file();
#endif
			 ftnend();
			 } break;
case 3:
# line 147 "cgram.y"
{ curclass = SNULL;  blevel = 0; } break;
case 4:
# line 149 "cgram.y"
{ curclass = SNULL;  blevel = 0; } break;
case 5:
# line 151 "cgram.y"
{ asmout();  curclass = SNULL; blevel = 0; } break;
case 6:
# line 154 "cgram.y"
{  yypvt[-1].nodep->in.op = FREE; } break;
case 7:
# line 156 "cgram.y"
{  yypvt[-2].nodep->in.op = FREE; } break;
case 8:
# line 158 "cgram.y"
{   defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep),
					curclass==STATIC?STATIC:EXTDEF ); } break;
case 9:
# line 161 "cgram.y"
{  
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    yypvt[-3].nodep->in.op = FREE;
			    ftnend();
			    } break;
case 10:
# line 170 "cgram.y"
{	regvar = 0; } break;
case 11:
# line 174 "cgram.y"
{ curclass = SNULL;  yypvt[-2].nodep->in.op = FREE; } break;
case 12:
# line 177 "cgram.y"
{ curclass = SNULL;  yypvt[-1].nodep->in.op = FREE; } break;
case 13:
# line 178 "cgram.y"
{  blevel = 1; } break;
case 15:
# line 183 "cgram.y"
{  bccode();
			    locctr(PROG);
			    } break;
case 16:
# line 189 "cgram.y"
{  yypvt[-1].nodep->in.op = FREE; } break;
case 17:
# line 191 "cgram.y"
{  yypvt[-2].nodep->in.op = FREE; } break;
case 20:
# line 197 "cgram.y"
{  yyval.nodep = mkty(INT,0,INT);  curclass = SNULL; } break;
case 21:
# line 200 "cgram.y"
{  yyval.nodep = yypvt[-0].nodep; } break;
case 23:
# line 203 "cgram.y"
{  yyval.nodep = mkty(INT,0,INT); } break;
case 24:
# line 205 "cgram.y"
{ curclass = SNULL ; } break;
case 25:
# line 207 "cgram.y"
{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-0].nodep->in.type,
					UNDEF );
 			    yypvt[-0].nodep->in.op = FREE;
 			    } break;
case 26:
# line 214 "cgram.y"
{  curclass = yypvt[-0].intval; } break;
case 28:
# line 219 "cgram.y"
{  yypvt[-1].nodep->in.type = types( yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type, UNDEF );
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 29:
# line 223 "cgram.y"
{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type );
			    yypvt[-1].nodep->in.op = yypvt[-0].nodep->in.op = FREE;
			    } break;
case 32:
# line 231 "cgram.y"
{ yyval.nodep = dclstruct(yypvt[-4].intval); } break;
case 33:
# line 233 "cgram.y"
{  yyval.nodep = rstruct(yypvt[-0].intval,0);  stwart = instruct; } break;
case 34:
# line 237 "cgram.y"
{  yyval.intval = bstruct(-1,0); stwart = SEENAME; } break;
case 35:
# line 239 "cgram.y"
{  yyval.intval = bstruct(yypvt[-0].intval,0); stwart = SEENAME; } break;
case 38:
# line 247 "cgram.y"
{  moedef( yypvt[-0].intval ); } break;
case 39:
# line 249 "cgram.y"
{  strucoff = yypvt[-0].intval;  moedef( yypvt[-2].intval ); } break;
case 40:
# line 253 "cgram.y"
{ yyval.nodep = dclstruct(yypvt[-4].intval);  } break;
case 41:
# line 255 "cgram.y"
{  yyval.nodep = rstruct(yypvt[-0].intval,yypvt[-1].intval); } break;
case 42:
# line 259 "cgram.y"
{  yyval.intval = bstruct(-1,yypvt[-0].intval);  stwart=0; } break;
case 43:
# line 261 "cgram.y"
{  yyval.intval = bstruct(yypvt[-0].intval,yypvt[-1].intval);  stwart=0;  } break;
case 46:
# line 269 "cgram.y"
{ curclass = SNULL;  stwart=0; yypvt[-1].nodep->in.op = FREE; } break;
case 47:
# line 271 "cgram.y"
{  if( curclass != MOU ){
				curclass = SNULL;
				}
			    else {
				sprintf( fakename, "$%dFAKE", fake++ );
				defid( tymerge(yypvt[-0].nodep, bdty(NAME,NIL,
					lookup( fakename, SMOS ))), curclass );
				werror("union member must be named");
				}
			    stwart = 0;
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 48:
# line 287 "cgram.y"
{ defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 49:
# line 289 "cgram.y"
{yyval.nodep=yypvt[-2].nodep;} break;
case 50:
# line 290 "cgram.y"
{ defid( tymerge(yypvt[-4].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 52:
# line 296 "cgram.y"
{  if( !(instruct&INSTRUCT) )
				uerror( "field outside of structure" );
			    if( yypvt[-0].intval<0 || yypvt[-0].intval >= FIELD ){
				uerror( "illegal field size" );
				yypvt[-0].intval = 1;
				}
			    defid( tymerge(yypvt[-3].nodep,yypvt[-2].nodep), FIELD|yypvt[-0].intval );
			    yyval.nodep = NIL;
			    } break;
case 53:
# line 307 "cgram.y"
{   if( !(instruct&INSTRUCT) )
				uerror( "field outside of structure" );
			    /* alignment or hole */
			    falloc( stab, yypvt[-0].intval, -1, yypvt[-2].nodep );
			    yyval.nodep = NIL;
			    } break;
case 54:
# line 314 "cgram.y"
{  yyval.nodep = NIL; } break;
case 55:
# line 319 "cgram.y"
{  umul:
				yyval.nodep = bdty( UNARY MUL, yypvt[-0].nodep, 0 ); } break;
case 56:
# line 322 "cgram.y"
{  uftn:
				yyval.nodep = bdty( UNARY CALL, yypvt[-2].nodep, 0 );  } break;
case 57:
# line 325 "cgram.y"
{  uary:
				yyval.nodep = bdty( LB, yypvt[-2].nodep, 0 );  } break;
case 58:
# line 328 "cgram.y"
{  bary:
				if( (int)yypvt[-1].intval <= 0 )
					werror( "zero or negative subscript" );
				yyval.nodep = bdty( LB, yypvt[-3].nodep, yypvt[-1].intval );  } break;
case 59:
# line 333 "cgram.y"
{  yyval.nodep = bdty( NAME, NIL, yypvt[-0].intval );  } break;
case 60:
# line 335 "cgram.y"
{ yyval.nodep=yypvt[-1].nodep; } break;
case 61:
# line 338 "cgram.y"
{  goto umul; } break;
case 62:
# line 340 "cgram.y"
{  goto uftn; } break;
case 63:
# line 342 "cgram.y"
{  goto uary; } break;
case 64:
# line 344 "cgram.y"
{  goto bary; } break;
case 65:
# line 346 "cgram.y"
{ yyval.nodep = yypvt[-1].nodep; } break;
case 66:
# line 348 "cgram.y"
{ if (paramno)
				uerror("arg list in declaration"); } break;
case 67:
# line 351 "cgram.y"
{
				if( blevel!=0 )
					uerror(
					"function declaration in bad context");
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-3].intval), 0 );
				stwart = 0;
				} break;
case 68:
# line 359 "cgram.y"
{ if (paramno)
				uerror("arg list in declaration"); } break;
case 69:
# line 362 "cgram.y"
{
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-2].intval), 0 );
				stwart = 0;
				} break;
case 70:
# line 369 "cgram.y"
{
				/* turn off typedefs for argument names */
				stwart = SEENAME;
				if( stab[yypvt[-1].intval].sclass == SNULL )
				    stab[yypvt[-1].intval].stype = FTN;
				} break;
case 71:
# line 378 "cgram.y"
{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 72:
# line 380 "cgram.y"
{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 75:
# line 386 "cgram.y"
{yyval.nodep=yypvt[-2].nodep;} break;
case 77:
# line 390 "cgram.y"
{  defid( yypvt[-0].nodep = tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
			    beginit(yypvt[-0].nodep->tn.rval);
			    } break;
case 79:
# line 397 "cgram.y"
{  nidcl( tymerge(yypvt[-1].nodep,yypvt[-0].nodep) ); } break;
case 80:
# line 399 "cgram.y"
{  defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), uclass(curclass) );
			} break;
case 81:
# line 403 "cgram.y"
{  doinit( yypvt[-0].nodep );
			    endinit(); } break;
case 82:
# line 406 "cgram.y"
{  endinit(); } break;
case 86:
# line 416 "cgram.y"
{  doinit( yypvt[-0].nodep ); } break;
case 87:
# line 418 "cgram.y"
{  irbrace(); } break;
case 92:
# line 430 "cgram.y"
{  ilbrace(); } break;
case 93:
# line 436 "cgram.y"
{
			    clearst(blevel);
			    if (--blevel == 1)
			    {
				clearst(blevel);
				blevel = 0;
			    }
			    checkst(blevel);
			    autooff = *--psavbc;
			    regvar = *--psavbc;
			} break;
case 94:
# line 450 "cgram.y"
{  if( blevel == 1 ) dclargs();
			    ++blevel;
			    if( psavbc > &asavbc[BCSZ-2] )
					cerror( "nesting too deep" );
			    *psavbc++ = regvar;
			    *psavbc++ = autooff;
			    } break;
case 95:
# line 460 "cgram.y"
{ ecomp( yypvt[-1].nodep ); } break;
case 96:
# line 462 "cgram.y"
{ asmout(); } break;
case 98:
# line 465 "cgram.y"
{ deflab(yypvt[-1].intval);
			   reached = 1;
#ifdef M32B
			   brdepth--;
#endif
			   } break;
case 99:
# line 472 "cgram.y"
{  if( yypvt[-1].intval != NOLAB ){
				deflab( yypvt[-1].intval );
				reached = 1;
				}
#ifdef M32B
			    brdepth--;
#endif
			    } break;
case 100:
# line 481 "cgram.y"
{
#ifdef M32B
				whdepth++;
#endif
			} break;
case 101:
# line 487 "cgram.y"
{
				savebc();
				if (!reached)
					werror("loop not entered at top");
				reached = 1;
				brklab = getlab();
				contlab = getlab();
				switch (wloop_level)
				{
				default:
					cerror("bad while loop code gen value");
					/*NOTREACHED*/
				case LL_TOP:	/* test at loop top */
					deflab(contlab);
					if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
					}
					else
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
							bcon(brklab)));
					break;
				case LL_BOT:	/* test at loop bottom */
					if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
						deflab(contlab);
					}
					else
					{
						branch(contlab);
						deflab(yyval.intval = getlab());
					}
					break;
				case LL_DUP:	/* dup. test at top & bottom */
					if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
						deflab(yyval.intval = contlab);
					}
					else
					{
#ifndef LINT
/* don't rotate loops for lint */
						register NODE *sav;
						extern NODE *treecpy();

						sav = treecpy(yypvt[-1].nodep);
						ecomp(buildtree(CBRANCH,yypvt[-1].nodep,
							bcon(brklab)));
						yypvt[-1].nodep = sav;
#endif
						deflab(yyval.intval = getlab());
					}
					break;
				}
			} break;
case 102:
# line 551 "cgram.y"
{
			switch (wloop_level)
			{
			default:
				cerror("bad while loop code gen. value");
				/*NOTREACHED*/
			case LL_TOP:	/* test at loop top */
				branch(contlab);
				break;
			case LL_BOT:	/* test at loop bottom */
				if (flostat & FLOOP)
					branch(contlab);
				else
				{
					reached = 1;
					deflab(contlab);
					ecomp(buildtree(CBRANCH,
						buildtree(NOT, yypvt[-3].nodep, NIL),
						bcon(yypvt[-1].intval)));
				}
				break;
			case LL_DUP:	/* dup. test at top & bottom */
				if (flostat & FLOOP)
					branch(contlab);
				else
				{
					if (flostat & FCONT)
					{
						reached = 1;
						deflab(contlab);
					}
					ecomp(buildtree(CBRANCH,
						buildtree(NOT, yypvt[-3].nodep, NIL),
						bcon(yypvt[-1].intval)));
				}
				break;
			}
			if ((flostat & FBRK) || !(flostat & FLOOP))
				reached = 1;
			else
				reached = 0;
			deflab(brklab);
			resetbc(0);
#ifdef M32B
			whdepth--;
#endif
		} break;
case 103:
# line 599 "cgram.y"
{   deflab( contlab );
			    if( flostat & FCONT ) reached = 1;
			    ecomp( buildtree( CBRANCH,
				buildtree( NOT, yypvt[-2].nodep, NIL ), bcon( yypvt[-6].intval ) ) );
			    deflab( brklab );
			    reached = 1;
			    resetbc(0);
#ifdef M32B
			    whdepth--;
#endif
			    } break;
case 104:
# line 611 "cgram.y"
{
#ifdef M32B
				fordepth++;
#endif
			} break;
case 105:
# line 617 "cgram.y"
{
				if (yypvt[-4].nodep)
					ecomp(yypvt[-4].nodep);
				else if (!reached)
					werror("loop not entered at top");
				savebc();
				contlab = getlab();
				brklab = getlab();
				reached = 1;
				switch (floop_level)
				{
				default:
					cerror("bad for loop code gen. value");
					/*NOTREACHED*/
				case LL_TOP:	/* test at loop top */
					deflab(yyval.intval = getlab());
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					}
					else
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
							bcon(brklab)));
					break;
				case LL_BOT:	/* test at loop bottom */
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					}
					else
						branch(yypvt[-6].intval = getlab());
					deflab(yyval.intval = getlab());
					break;
				case LL_DUP:	/* dup. test at top & bottom */
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == ICON &&
						yypvt[-1].nodep->tn.lval != 0)
					{
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					}
					else
					{
#ifndef LINT
/* don't rotate loops for lint */
						register NODE *sav;
						extern NODE *treecpy();

						sav = treecpy(yypvt[-1].nodep);
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
							bcon(brklab)));
						yypvt[-1].nodep = sav;
#endif
					}
					deflab(yyval.intval = getlab());
					break;
				}
			} break;
case 106:
# line 688 "cgram.y"
{
			if (flostat & FCONT)
			{
				deflab(contlab);
				reached = 1;
			}
			if (yypvt[-2].nodep)
				ecomp(yypvt[-2].nodep);
			switch (floop_level)
			{
			default:
				cerror("bad for loop code gen. value");
				/*NOTREACHED*/
			case LL_TOP:	/* test at loop top */
				branch(yypvt[-3].intval);
				break;
			case LL_BOT:	/* test at loop bottom */
				if (yypvt[-5].nodep)
					deflab(yypvt[-10].intval);
				/*FALLTHROUGH*/
			case LL_DUP:	/* dup. test at top & bottom */
				if (yypvt[-5].nodep)
				{
					ecomp(buildtree(CBRANCH,
						buildtree(NOT, yypvt[-5].nodep, NIL),
						bcon(yypvt[-3].intval)));
				}
				else
					branch(yypvt[-3].intval);
				break;
			}
			deflab(brklab);
			if ((flostat & FBRK) || !(flostat & FLOOP))
				reached = 1;
			else
				reached = 0;
			resetbc(0);
#ifdef M32B
			fordepth--;
#endif
		} break;
case 107:
# line 730 "cgram.y"
{   if( reached ) branch( brklab );
			    deflab( yypvt[-1].intval );
			    swend();
			    deflab(brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached=1;
			    resetbc(FCONT);
#ifdef M32B
			    brdepth--;
#endif
			    } break;
case 108:
# line 741 "cgram.y"
{  if( brklab == NOLAB ) uerror( "illegal break");
			    else if(reached) branch( brklab );
			    flostat |= FBRK;
			    if( brkflag ) goto rch;
			    reached = 0;
			    } break;
case 109:
# line 748 "cgram.y"
{   if( contlab == NOLAB ) uerror( "illegal continue");
			    else branch( contlab );
			    flostat |= FCONT;
			    goto rch;
			    } break;
case 110:
# line 754 "cgram.y"
{   retstat |= NRETVAL;
			    branch( retlab );
			rch:
			    if( !reached ) werror( "statement not reached");
			    reached = 0;
			    } break;
case 111:
# line 761 "cgram.y"
{   register NODE *temp;
			    TWORD indtype();
			    idname = curftn;
			    temp = buildtree( NAME, NIL, NIL );
			    temp->in.type = DECREF(temp->in.type);
			    if(temp->in.type == (FTN|VOID))
				uerror(
				"void function %s cannot return value",
					stab[idname].sname);
 			    temp->tn.op = RNODE;
 			    yypvt[-1].nodep = makety( yypvt[-1].nodep, temp->fn.type,
					temp->fn.cdim, temp->fn.csiz );
			    temp->in.type = indtype( temp->in.type );
			    temp = buildtree( ASSIGN, temp, yypvt[-1].nodep );
			    ecomp( temp );
			    retstat |= RETVAL;
			    branch( retlab );
			    reached = 0;
			    } break;
case 112:
# line 781 "cgram.y"
{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, INT );
			    q->tn.rval = idname = yypvt[-1].intval;
			    defid( q, ULABEL );
			    stab[idname].suse = -lineno;
			    branch( stab[idname].offset );
			    goto rch;
			    } break;
case 117:
# line 795 "cgram.y"
{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, LABEL );
			    q->tn.rval = yypvt[-1].intval;
			    defid( q, LABEL );
			    reached = 1;
			    } break;
case 118:
# line 802 "cgram.y"
{  addcase(yypvt[-1].nodep);
			    reached = 1;
			    } break;
case 119:
# line 806 "cgram.y"
{  reached = 1;
			    adddef();
			    flostat |= FDEF;
			    } break;
case 120:
# line 812 "cgram.y"
{  savebc();
			    if( !reached ) werror( "loop not entered at top");
			    brklab = getlab();
			    contlab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;
#ifdef M32B
			    whdepth++;
#endif
			    } break;
case 121:
# line 824 "cgram.y"
{  ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( yyval.intval=getlab()) ) ) ;
			    reached = 1;
#ifdef M32B
			    brdepth++;
#endif
			    } break;
case 122:
# line 832 "cgram.y"
{  if( reached ) branch( yyval.intval = getlab() );
			    else yyval.intval = NOLAB;
			    deflab( yypvt[-2].intval );
			    reached = 1;
			    } break;
case 123:
# line 840 "cgram.y"
{   register NODE *temp;
			    savebc();
			    temp = block( SNODE, NIL, NIL, INT, 0, INT );
			    temp = buildtree( ASSIGN, temp, yypvt[-1].nodep );
#ifdef M32B
			    temp = setswreg( temp );
#endif
			    brklab = getlab();
			    ecomp( temp );
			    branch( yyval.intval = getlab() );
			    swstart();
			    reached = 0;
#ifdef M32B
			    brdepth++;
#endif
			    } break;
case 124:
# line 858 "cgram.y"
{ yyval.intval=instruct; stwart=instruct=0; } break;
case 125:
# line 860 "cgram.y"
{  yyval.intval = icons( yypvt[-0].nodep );  instruct=yypvt[-1].intval; } break;
case 127:
# line 864 "cgram.y"
{ yyval.nodep=0; } break;
case 129:
# line 869 "cgram.y"
{  goto bop; } break;
case 130:
# line 873 "cgram.y"
{
			preconf:
			    if( yychar==RELOP || yychar==EQUOP || yychar==AND
					|| yychar==OR || yychar==ER ){
			precplaint:
				if( hflag ) werror(
				"precedence confusion possible: parenthesize!"
					);
				}
			bop:
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep );
			    } break;
case 131:
# line 886 "cgram.y"
{  yypvt[-1].intval = COMOP;
			    goto bop;
			    } break;
case 132:
# line 890 "cgram.y"
{  goto bop; } break;
case 133:
# line 892 "cgram.y"
{  if(yychar==SHIFTOP) goto precplaint; else goto bop; } break;
case 134:
# line 894 "cgram.y"
{  if(yychar==SHIFTOP ) goto precplaint; else goto bop; } break;
case 135:
# line 896 "cgram.y"
{  if(yychar==PLUS||yychar==MINUS) goto precplaint; else goto bop; } break;
case 136:
# line 898 "cgram.y"
{  goto bop; } break;
case 137:
# line 900 "cgram.y"
{  goto preconf; } break;
case 138:
# line 902 "cgram.y"
{  if( yychar==RELOP||yychar==EQUOP ) goto preconf;  else goto bop; } break;
case 139:
# line 904 "cgram.y"
{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 140:
# line 906 "cgram.y"
{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 141:
# line 908 "cgram.y"
{  goto bop; } break;
case 142:
# line 910 "cgram.y"
{  goto bop; } break;
case 143:
# line 912 "cgram.y"
{  abop:
				yyval.nodep = buildtree( ASG yypvt[-2].intval, yypvt[-3].nodep, yypvt[-0].nodep );
				} break;
case 144:
# line 916 "cgram.y"
{  goto abop; } break;
case 145:
# line 918 "cgram.y"
{  goto abop; } break;
case 146:
# line 920 "cgram.y"
{  goto abop; } break;
case 147:
# line 922 "cgram.y"
{  goto abop; } break;
case 148:
# line 924 "cgram.y"
{  goto abop; } break;
case 149:
# line 926 "cgram.y"
{  goto abop; } break;
case 150:
# line 928 "cgram.y"
{  goto abop; } break;
case 151:
# line 930 "cgram.y"
{  yyval.nodep=buildtree(QUEST, yypvt[-4].nodep, buildtree( COLON, yypvt[-2].nodep, yypvt[-0].nodep ) );
			    } break;
case 152:
# line 933 "cgram.y"
{  werror( "old-fashioned assignment operator" );
				goto bop; } break;
case 153:
# line 936 "cgram.y"
{  goto bop; } break;
case 155:
# line 941 "cgram.y"
{  yyval.nodep = buildtree( yypvt[-0].intval, yypvt[-1].nodep, bcon(1) ); } break;
case 156:
# line 943 "cgram.y"
{ ubop:
			    yyval.nodep = buildtree( UNARY yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 157:
# line 947 "cgram.y"
{
#ifdef M32B
			    myand(yypvt[-0].nodep);
#endif
			    if( ISFTN(yypvt[-0].nodep->in.type) || ISARY(yypvt[-0].nodep->in.type) ){
				werror( "& before array or function: ignored" );
				yyval.nodep = yypvt[-0].nodep;
				}
			    else goto ubop;
			    } break;
case 158:
# line 958 "cgram.y"
{  goto ubop; } break;
case 159:
# line 960 "cgram.y"
{
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 160:
# line 964 "cgram.y"
{  yyval.nodep = buildtree( yypvt[-1].intval==INCR ? ASG PLUS : ASG MINUS,
						yypvt[-0].nodep,
						bcon(1)  );
			    } break;
case 161:
# line 969 "cgram.y"
{  yyval.nodep = doszof( yypvt[-0].nodep ); } break;
case 162:
# line 971 "cgram.y"
{  yyval.nodep = buildtree( CAST, yypvt[-2].nodep, yypvt[-0].nodep );
			    yyval.nodep->in.left->in.op = FREE;
			    yyval.nodep->in.op = FREE;
			    yyval.nodep = yyval.nodep->in.right;
			    } break;
case 163:
# line 977 "cgram.y"
{  yyval.nodep = doszof( yypvt[-1].nodep ); } break;
case 164:
# line 979 "cgram.y"
{  yyval.nodep = buildtree( LB, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 165:
# line 981 "cgram.y"
{  yyval.nodep = xicolon( yypvt[-5].nodep, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 166:
# line 983 "cgram.y"
{  yyval.nodep=buildtree(UNARY CALL,yypvt[-1].nodep,NIL); } break;
case 167:
# line 985 "cgram.y"
{  yyval.nodep=buildtree(CALL,yypvt[-2].nodep,yypvt[-1].nodep); } break;
case 168:
# line 987 "cgram.y"
{  if( yypvt[-1].intval == DOT ){
				if( notlval( yypvt[-2].nodep ) )uerror(
				       "structure reference must be addressable"
					);
				yypvt[-2].nodep = buildtree( UNARY AND, yypvt[-2].nodep, NIL );
				}
			    idname = yypvt[-0].intval;
			    yyval.nodep = buildtree( STREF, yypvt[-2].nodep,
					buildtree( NAME, NIL, NIL ) );
			    } break;
case 169:
# line 998 "cgram.y"
{  idname = yypvt[-0].intval;
			    /* recognize identifiers in initializations */
			    if( blevel==0 && stab[idname].stype == UNDEF ) {
				register NODE *q;
				werror( "undeclared initializer name %s",
					stab[idname].sname );
				q = block( FREE, NIL, NIL, INT, 0, INT );
				q->tn.rval = idname;
				defid( q, EXTERN );
				}
			    yyval.nodep=buildtree(NAME,NIL,NIL);
			    stab[yypvt[-0].intval].suse = -lineno;
			} break;
case 170:
# line 1012 "cgram.y"
{  yyval.nodep=bcon(0);
			    yyval.nodep->tn.lval = lastcon;
			    yyval.nodep->tn.rval = NONAME;
			    if( yypvt[-0].intval ) yyval.nodep->fn.csiz = yyval.nodep->in.type = ctype(LONG);
			    } break;
case 171:
# line 1018 "cgram.y"
{  yyval.nodep=buildtree(FCON,NIL,NIL);
			    yyval.nodep->fpn.dval = dcon;
			    } break;
case 172:
# line 1022 "cgram.y"
{  yyval.nodep = getstr(); /* get string contents */ } break;
case 173:
# line 1024 "cgram.y"
{ yyval.nodep=yypvt[-1].nodep; } break;
case 174:
# line 1028 "cgram.y"
{
			yyval.nodep = tymerge( yypvt[-1].nodep, yypvt[-0].nodep );
			yyval.nodep->in.op = NAME;
			yypvt[-1].nodep->in.op = FREE;
			} break;
case 175:
# line 1036 "cgram.y"
{ yyval.nodep = bdty( NAME, NIL, -1 ); } break;
case 176:
# line 1038 "cgram.y"
{ yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,-1),0); } break;
case 177:
# line 1040 "cgram.y"
{  yyval.nodep = bdty( UNARY CALL, yypvt[-3].nodep, 0 ); } break;
case 178:
# line 1042 "cgram.y"
{  goto umul; } break;
case 179:
# line 1044 "cgram.y"
{  goto uary; } break;
case 180:
# line 1046 "cgram.y"
{  goto bary;  } break;
case 181:
# line 1048 "cgram.y"
{ yyval.nodep = yypvt[-1].nodep; } break;
case 182:
# line 1052 "cgram.y"
{  if( stab[yypvt[-1].intval].stype == UNDEF ){
				register NODE *q;
				q = block( FREE, NIL, NIL, FTN|INT, 0, INT );
				q->tn.rval = yypvt[-1].intval;
				defid( q, EXTERN );
				}
			    idname = yypvt[-1].intval;
			    yyval.nodep=buildtree(NAME,NIL,NIL);
			    stab[idname].suse = -lineno;
			} break;
	}
	goto yystack;  /* stack new state and value */
}
