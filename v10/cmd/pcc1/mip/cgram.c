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

# line 108 "../mip/cgram.y"
# include "mfile1"
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;

# line 127 "../mip/cgram.y"
	static int fake = 0;
#ifndef FLEXNAMES
	static char fakename[NCHNAM+1];
#else
	static char fakename[24];
#endif
# define YYERRCODE 256

# line 840 "../mip/cgram.y"


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
	flostat = 0;
	}

resetbc(mask){

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

	genswitch( swbeg-1, swp-swbeg );
	swp = swbeg-1;
	}
short yyexca[] ={
-1, 1,
	0, -1,
	2, 23,
	11, 23,
	50, 23,
	57, 23,
	-2, 0,
-1, 19,
	56, 82,
	57, 82,
	-2, 7,
-1, 24,
	56, 81,
	57, 81,
	-2, 79,
-1, 26,
	56, 85,
	57, 85,
	-2, 80,
-1, 32,
	52, 46,
	-2, 44,
-1, 34,
	52, 38,
	-2, 36,
-1, 109,
	33, 18,
	34, 18,
	35, 18,
	49, 18,
	-2, 13,
-1, 280,
	33, 16,
	34, 16,
	35, 16,
	49, 16,
	-2, 14,
-1, 297,
	33, 17,
	34, 17,
	35, 17,
	49, 17,
	-2, 15,
	};
# define YYNPROD 187
# define YYLAST 1228
short yyact[]={

 211,  18, 262,  93, 231,  89,  87,  88,  27, 209,
  80, 200, 132,  78,   6, 147,  79,  21,  10,   9,
  14,  20, 100, 166,  51, 131,  27, 311,  96,  92,
 310,  46,  82,  81,  16,  21,  98, 110, 221, 222,
 226,  59, 230, 219, 220, 227, 228, 229, 233, 232,
  83,  75,  84, 304, 110, 256,  22,  66,  56, 223,
 231,  89,  87,  88,  27, 291,  80, 266, 112,  78,
  36, 297,  79,  21,  22, 271, 265, 133, 107, 270,
  45, 280, 212, 101,  27, 148, 152, 204,  82,  81,
 164,  24, 203,  21, 221, 222, 226,  91, 230, 219,
 220, 227, 228, 229, 233, 232,  83,  74,  84, 193,
 110, 210,  22,  40,  42, 223, 168, 169, 170, 172,
 174, 176, 178, 180, 181, 183, 185, 187, 188, 189,
 190, 191,  22,  27,  94, 133, 156, 162, 195,  17,
  19, 160,  21,  10,   9,  14, 103, 148,  99,  95,
 155, 156, 207,  96, 158, 298,  41,  43, 161,  16,
 194, 284, 197,  36,  35,  70,  77, 114, 192, 157,
  71, 136, 235, 138, 236,  38, 237, 111, 238,  39,
 239,  22, 234, 240,  73, 241, 250, 242, 163, 208,
 286, 139,  65, 250, 133, 137, 102,  48,  48,  95,
 202,  49,  49, 283, 254, 244, 245,  38,  69, 255,
  33,  39, 159, 252, 253, 206,  48,  68, 263,  31,
  49,   8, 268, 258, 259, 260, 261, 154, 264,  28,
 306, 246, 289, 278, 202, 272, 281, 277, 248, 201,
 251, 108, 198,  27, 282, 140, 141, 142, 143, 144,
 145,  53,  21,  57,  10,   9,  14,  72, 224,  64,
 300, 299,  97, 276, 275, 153, 274, 273, 153,  52,
  16, 288, 287, 201, 292, 293, 263, 295, 294,   9,
  26, 231,  89,  87,  88,  47,  10,  80,  14,  54,
  78,  22,  30,  79, 121, 279, 269,  95, 196, 301,
  60, 307,  16,   7, 113, 263, 149, 308, 285,  82,
  81, 117,  29,  57, 224, 221, 222, 226,  26, 230,
 219, 220, 227, 228, 229, 233, 232,  83,  67,  84,
  34, 110, 118,  32, 119, 225, 223, 121,  26, 118,
 123, 119, 109, 124, 121, 125, 106, 128, 105, 126,
 127, 129, 115, 122, 117, 120, 134,  50,  25, 115,
 122, 117, 120,  61,  44, 249,   4, 205, 149, 102,
 118,  90, 119,  55,  58, 121, 167, 309, 118, 165,
 119, 104, 116, 121, 130,  63, 123,  97,  62, 124,
  37, 125, 117, 128,   3, 126, 127, 129, 115, 122,
 117, 120,   2, 151,  85,  11,  12,   5,  23,  13,
  15, 218, 216, 217, 118, 249, 119, 215, 213, 121,
 214,   1, 123, 305,   0, 124,   0, 125, 116, 128,
 130, 126, 127, 129, 115, 122, 117, 120,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 118,   0, 119,   0,   0, 121,   0,   0, 123, 303,
   0, 124,   0, 125, 116, 128, 130, 126, 127, 129,
 115, 122, 117, 120,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 118,   0,
 119,   0, 118, 121, 119, 302, 123, 121,   0, 124,
 116, 125, 130, 128, 296, 126, 127, 129, 115, 122,
 117, 120, 115,   0, 117, 120,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 118,   0, 119,   0,
   0, 121,   0,   0, 123, 224,   0, 124, 116, 125,
 130, 128,   0, 126, 127, 129, 115, 122, 117, 120,
 118,   0, 119,   0,   0, 121,   0, 118, 123, 119,
   0, 124, 121, 125,   0, 128,   0, 126, 127, 129,
 115, 122, 117, 120,   0,   0, 116, 290, 130, 117,
 120,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 116, 257, 130, 118,   0, 119,   0,   0, 121,   0,
   0, 123,   0,   0, 124,   0, 125,   0, 128,   0,
 126, 127, 129, 115, 122, 117, 120, 118,   0, 119,
   0,   0, 121,   0,   0, 123,   0,   0, 124,   0,
 125,   0, 128, 243, 126, 127, 129, 115, 122, 117,
 120,   0, 247, 116,   0, 130,   0,   0,   0,   0,
   0,   0,   0, 118,   0, 119,   0, 118, 121, 119,
   0, 123, 121,   0, 124, 123, 125, 116, 128, 130,
 126, 127, 129, 115, 122, 117, 120, 115, 122, 117,
 120,   0,   0,   0,   0,   0,   0,   0,   0, 118,
   0, 119,   0,   0, 121,   0,   0, 123, 199,   0,
 124,   0, 125, 116, 128, 130, 126, 127, 129, 115,
 122, 117, 120, 118,   0, 119,   0,   0, 121,   0,
   0, 123,   0,   0, 124,   0, 125,   0, 128,   0,
 126, 127, 129, 115, 122, 117, 120,   0,   0, 116,
   0, 130,  86,  89,  87,  88,   0,   0,  80,   0,
   0,  78,   0,   0,  79,  86,  89,  87,  88,   0,
   0,  80,   0,   0,  78, 130,   0,  79,   0,   0,
  82,  81,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  82,  81,   0,   0,   0,  83,   0,
  84,   0,   0,  86,  89,  87,  88,   0, 186,  80,
   0,  83,  78,  84,   0,  79,  86,  89,  87,  88,
   0, 184,  80,   0,   0,  78,   0,   0,  79,   0,
   0,  82,  81,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  82,  81,   0,   0,   0,  83,
   0,  84,   0,   0,  86,  89,  87,  88,   0, 182,
  80,   0,  83,  78,  84,   0,  79,  86,  89,  87,
  88,   0, 179,  80,   0,   0,  78,   0,   0,  79,
   0,   0,  82,  81,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  82,  81,   0,   0,   0,
  83,   0,  84,   0,   0,  86,  89,  87,  88,   0,
 177,  80,   0,  83,  78,  84,   0,  79,  86,  89,
  87,  88,   0, 175,  80,   0,   0,  78,   0,   0,
  79,   0,   0,  82,  81,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  82,  81,   0,   0,
   0,  83,   0,  84,   0,   0,  86,  89,  87,  88,
   0, 173,  80,   0,  83,  78,  84,   0,  79,  86,
  89,  87,  88,   0, 171,  80,   0,   0,  78,   0,
   0,  79,   0,   0,  82,  81, 118,   0, 119,   0,
   0, 121,   0,   0, 123,   0,   0,  82,  81, 125,
   0,   0,  83,   0,  84,   0, 115, 122, 117, 120,
   0, 267,   0,   0,   0,  83,   0,  84,   0, 135,
  86,  89,  87,  88,   0,   0,  80,   0,   0,  78,
   0,   0,  79,  86,  89,  87,  88,   0,   0,  80,
   0,   0,  78,   0,   0,  79,   0,   0,  82,  81,
   0,  10,   0,  14,   0,   0,   0,   0,   0,   0,
   0,  82,  81,   0,   0,   0,  83,  16,  84,   0,
   0,   0,   0,  86,  89,  87,  88,   0,   0,  83,
   0,  84,   0,  76,  86,  89,  87,  88,   0,   0,
  80,   0,   0,  78,   0,   0,  79,  86,  89,  87,
  88,  82,  81,  80,   0,   0,  78,   0,   0,  79,
   0,   0,  82,  81,   0,   0,   0,   0,   0,  83,
   0,  84,   0,   0,   0,  82,  81,   0,   0,   0,
  83,   0,  84, 150,   0,  86,  89,  87,  88,   0,
   0,  80,   0,  83,  78,  84,   0,  79,   0,   0,
   0,   0,   0,   0, 118,   0, 119,   0,   0, 121,
   0,   0, 123,  82,  81, 124,   0, 125,   0, 128,
   0, 126, 127,   0, 115, 122, 117, 120,   0,   0,
 118,  83, 119, 146,   0, 121,   0,   0, 123,   0,
   0, 124,   0, 125,   0,   0,   0, 126,   0,   0,
 115, 122, 117, 120, 118,   0, 119,   0,   0, 121,
   0,   0, 123,   0,   0, 124,   0, 125,   0,   0,
   0,   0,   0,   0, 115, 122, 117, 120 };
short yypact[]={

-1000, 110,-1000,-1000,-1000,  82,-1000, 253, 245,-1000,
 259,-1000,-1000, 167, 331, 158, 328,-1000, 107, 125,
-1000, 241, 241,  29, 148, -34,-1000, 219,-1000, 253,
 256, 253,-1000, 298,-1000,-1000,-1000,-1000, 208, 137,
 148, 125, 166, 157, 114,-1000,-1000,-1000, 206, 129,
1031,-1000,-1000,-1000,-1000,  40,-1000,   6,  92,-1000,
 -36,  62,-1000, -15,-1000,-1000, 122,1095,-1000,-1000,
-1000, 302,-1000,-1000, 112, 717, 967, 141,1095,1095,
1095,1095,1095,1133,1018,1082, 218,-1000,-1000,-1000,
 174, 253,  80,-1000, 125, 147,-1000,-1000, 159, 298,
-1000,-1000, 125,-1000,-1000,-1000,-1000, 131,  33,-1000,
-1000,-1000, 717,-1000,-1000,1095,1095, 916, 903, 865,
 852, 814,1095, 801, 763, 750,1095,1095,1095,1095,
1095,  53,-1000, 717, 967,-1000,-1000,1095, 296,-1000,
 141, 141, 141, 141, 141, 141,1018, 191, 657, 223,
-1000,  36, 717,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,  95,-1000,-1000,-1000,  58, 221, 551, 717,
-1000,1095, 283,1095, 283,1095, 364,1095,-1000,1095,
 486, 333,1095, 980,1095, 661,1095,1198,1174, 621,
 717, 717, 152, 967,  53, 597,-1000, 187,1095,-1000,
 132, 189, 223,1095,-1000,   6,-1000,-1000,   2,-1000,
-1000, 544,-1000, 279, 279, 279, 279,1095, 279,  19,
  10, 954, 294,-1000,  22, 279, 217, 216,-1000, 214,
 213, 215,1095, 273,  24, 717, 717, 717, 717, 717,
 717, 717, 717,1095,-1000,-1000, 150,-1000,1071, 141,
 106,-1000, 139, 132, 717,-1000,-1000,-1000, 233,-1000,
-1000, 228, 181, 693,-1000,-1000,-1000,-1000, 520,   8,
-1000,-1000,-1000,1095,1095,1095,1095,-1000, 482,-1000,
-1000,  14,1148,-1000,-1000, 100, 211,-1000, 210, 279,
-1000,-1000, 444, 408,  -4, 372,-1000,-1000,-1000, 179,
1095,-1000,-1000,-1000,1095,-1000,-1000, 326, -27, -30,
-1000,-1000 };
short yypgo[]={

   0, 421,  57, 420, 418, 417, 413, 412, 411, 410,
 409, 408,   0,   2, 166,  14, 407, 221, 406, 405,
  15,  11, 404,   3, 134,  91, 403, 402, 394,   1,
 390, 388, 385,  82, 381,  23,   9, 379, 376,  29,
 303, 374,  36,  41, 373, 371,  58, 367, 364,  21,
 363, 358, 357,  25,  12, 356, 348, 346, 342, 335,
 328 };
short yyr1[]={

   0,   1,   1,  27,  27,  28,  28,  30,  28,  31,
  32,  32,  35,  35,  37,  37,  38,  38,  38,  34,
  34,  34,  16,  16,  15,  15,  15,  15,  15,  40,
  17,  17,  17,  17,  17,  18,  18,   9,   9,  41,
  41,  43,  43,  19,  19,  10,  10,  44,  44,  46,
  46,  39,  47,  39,  23,  23,  23,  23,  23,  25,
  25,  25,  25,  25,  25,  24,  24,  24,  24,  24,
  24,  24,  11,  48,  48,  48,  29,  50,  29,  51,
  51,  49,  49,  49,  49,  49,  53,  53,  54,  54,
  42,  42,  45,  45,  52,  52,  55,  33,  33,  56,
  57,  58,  36,  36,  36,  36,  36,  36,  36,  36,
  36,  36,  36,  36,  36,  36,  36,  36,  36,  59,
  59,  59,   7,   4,   3,   5,   6,   8,  60,   2,
  13,  13,  26,  26,  12,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  12,  12,  12,  12,  12,  14,
  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
  14,  14,  14,  14,  14,  14,  14,  20,  21,  21,
  21,  21,  21,  21,  21,  22,  22 };
short yyr2[]={

   0,   2,   0,   1,   1,   2,   3,   0,   4,   2,
   2,   0,   2,   0,   3,   4,   3,   4,   0,   3,
   2,   2,   1,   0,   2,   2,   1,   1,   3,   1,
   1,   2,   3,   1,   1,   5,   2,   1,   2,   1,
   3,   1,   3,   5,   2,   1,   2,   1,   3,   2,
   1,   1,   0,   4,   1,   1,   3,   2,   1,   2,
   3,   3,   4,   1,   3,   2,   3,   3,   4,   3,
   3,   2,   2,   1,   3,   1,   1,   0,   4,   1,
   1,   1,   1,   3,   6,   1,   1,   3,   1,   4,
   0,   1,   0,   1,   0,   1,   1,   1,   1,   4,
   3,   1,   2,   1,   2,   2,   2,   7,   4,   2,
   2,   2,   2,   3,   3,   1,   2,   2,   2,   2,
   3,   2,   1,   4,   3,   4,   6,   4,   0,   2,
   1,   0,   1,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,
   4,   4,   4,   4,   4,   5,   3,   3,   1,   2,
   2,   2,   2,   2,   2,   2,   4,   4,   4,   2,
   3,   3,   1,   1,   1,   1,   3,   2,   0,   2,
   5,   2,   3,   4,   3,   2,   2 };
short yychk[]={

-1000,  -1, -27, -28, 256, -16, -15, -40, -17,  34,
  33, -19, -18, -10,  35,  -9,  49,  57, -29, -24,
 -49,  11,  50, -11, -25, -51, 256,   2, -17, -40,
  33,  52,   2,  52,   2,  57,  56, -30,  50,  54,
 -25, -24, -25, -24, -48,  51,   2, 256,  50,  54,
 -52,  58,  50, -17,  33, -44, -46, -17, -41, -43,
   2, -50, -31, -32,  51,  55,  -2, -60,  51,  51,
  51,  56,  51,  55,  -2, -12,  52, -14,  11,  14,
   8,  31,  30,  48,  50, -22,   2,   4,   5,   3,
 -45,  57, -39, -23, -24, -25,  22, 256, -42,  56,
  58, -49, -24, -33, -34, -56, -57, -15, 256, -58,
  52,  55, -12,   2,  55,  26,  56,  28,   6,   8,
  29,  11,  27,  14,  17,  19,  23,  24,  21,  25,
  58, -53, -54, -12, -55,  52,  30,  54,  32,  50,
 -14, -14, -14, -14, -14, -14,  50, -20, -12, -17,
  51, -26, -12,  50,  53, -46,  56,  22,  -2,  53,
 -43,  -2, -39,  57,  57, -37, -35, -38, -12, -12,
 -12,  58, -12,  58, -12,  58, -12,  58, -12,  58,
 -12, -12,  58, -12,  58, -12,  58, -12, -12, -12,
 -12, -12, -42,  56, -53, -12,   2, -20,  51,  51,
 -21,  50,  11,  56,  51, -47,  -2,  57, -35, -36,
  53, -12, -33,  -4,  -3,  -5,  -7,  -6,  -8,  41,
  42,  36,  37,  57, 256, -59,  38,  43,  44,  45,
  40,   2,  47,  46, -15, -12, -12, -12, -12, -12,
 -12, -12, -12,  22,  53, -54, -42,  55,  51, -14,
  54,  51, -21, -21, -12, -23,  53,  57, -36, -36,
 -36, -36, -13, -12, -36,  57,  57,  57, -12,   2,
  57,  53, -36,  50,  50,  50,  50,  22, -12,  22,
  57, -29, -12,  53,  55,  -2,  51,  39,  43,  51,
  57,  57, -12, -12, -13, -12,  22,  57,  55,  50,
  50, -36,  51,  51,  57,  51,  51, -12, -13,  51,
  57,  57 };
short yydef[]={

   2,  -2,   1,   3,   4,   0,  22,  26,  27,  29,
  30,  33,  34,   0,  45,   0,  37,   5,   0,  -2,
  76,   0,   0,   0,  -2,  94,  -2,  63,  24,  25,
  31,   0,  -2,   0,  -2,   6,  77,  11,   0, 128,
  59,  65,   0,   0,   0,  71,  73,  75,   0, 128,
   0,  95,  72,  28,  32,  92,  47,  50,  90,  39,
  41,   0,   8,   0,  66,  67,   0,   0,  64,  69,
  70,   0,  60,  61,   0,  83,   0, 158,   0,   0,
   0,   0,   0,   0,   0,   0, 172, 173, 174, 175,
   0,  93,  49,  51,  54,  55, 128,  58,   0,  91,
 128,  78,  82,   9,  10,  97,  98,   0,   0,  -2,
 101,  68, 129,  74,  62,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  90,  86,  88,   0,  96, 159,   0,   0, 186,
 160, 161, 162, 163, 164, 165,   0,   0,   0, 178,
 169,   0, 132, 185,  43,  48,  52, 128,  57,  35,
  40,  42,   0,  20,  21,  13,   0,   0, 134, 135,
 136,   0, 137,   0, 138,   0, 139,   0, 140,   0,
 141, 142,   0, 143,   0, 144,   0, 145, 146,   0,
 156, 157,   0,  91,  90,   0, 171,   0,   0, 176,
 177, 178, 178,   0, 170,   0,  56,  19,   0,  12,
 100,   0, 103,   0,   0,   0,   0, 131,   0,   0,
   0,   0,   0, 115,   0,   0,   0,   0, 122,   0,
   0, 172,   0,   0,   0, 148, 149, 150, 151, 147,
 152, 153, 154,   0,  84,  87,   0, 168, 167, 166,
 128, 179,   0, 181, 133,  53,  99, 102, 104, 105,
 106,   0,   0, 130, 109, 110, 111, 112,   0,   0,
 116, 117, 118,   0,   0, 131,   0, 119,   0, 121,
  -2,   0, 155,  89, 182,   0, 184, 124,   0,   0,
 113, 114,   0,   0,   0,   0, 120,  -2, 183,   0,
   0, 108, 123, 125, 131, 127, 180,   0,   0,   0,
 126, 107 };
#
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

yyparse() {

	short yys[YYMAXDEPTH];
	short yyj, yym;
	register YYSTYPE *yypvt;
	register short yystate, *yyps, yyn;
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
	if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
		if( ++yyps> &yys[YYMAXDEPTH] ) { yyerror( "yacc stack overflow" ); return(1); }
		*yyps = yystate;
		++yypv;
		*yypv = yyval;

 yynewstate:

	yyn = yypact[yystate];

	if( yyn<= YYFLAG ) goto yydefault; /* simple state */

	if( yychar<0 ) if( (yychar=yylex())<0 ) yychar=0;
	if( (yyn += yychar)<0 || yyn >= YYLAST ) goto yydefault;

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
		if( yychar<0 ) if( (yychar=yylex())<0 ) yychar = 0;
		/* look through exception table */

		for( yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate) ; yyxi += 2 ) ; /* VOID */

		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
			}
		if( (yyn = yyxi[1]) < 0 ) return(0);   /* accept */
		}

	if( yyn == 0 ){ /* error */
		/* error ... attempt to resume parsing */

		switch( yyerrflag ){

		case 0:   /* brand new error */

			yyerror( "syntax error" );
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
			if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif

			if( yychar == 0 ) goto yyabort; /* don't discard EOF, quit */
			yychar = -1;
			goto yynewstate;   /* try again in the same state */

			}

		}

	/* reduction by production yyn */

#ifdef YYDEBUG
		if( yydebug ) printf("reduce %d\n",yyn);
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
# line 137 "../mip/cgram.y"
ftnend(); break;
case 3:
# line 140 "../mip/cgram.y"
{ curclass = SNULL;  blevel = 0; } break;
case 4:
# line 142 "../mip/cgram.y"
{ curclass = SNULL;  blevel = 0; } break;
case 5:
# line 146 "../mip/cgram.y"
{  yypvt[-1].nodep->in.op = FREE; } break;
case 6:
# line 148 "../mip/cgram.y"
{  yypvt[-2].nodep->in.op = FREE; } break;
case 7:
# line 149 "../mip/cgram.y"
{
				defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass==STATIC?STATIC:EXTDEF );
#ifndef LINT
				pfstab(stab[yypvt[-0].nodep->tn.rval].sname);
#endif
				} break;
case 8:
# line 155 "../mip/cgram.y"
{  
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    yypvt[-3].nodep->in.op = FREE;
			    ftnend();
			    } break;
case 11:
# line 166 "../mip/cgram.y"
{  blevel = 1; } break;
case 13:
# line 171 "../mip/cgram.y"
{  bccode();
			    locctr(PROG);
			    } break;
case 14:
# line 177 "../mip/cgram.y"
{  yypvt[-1].nodep->in.op = FREE; 
#ifndef LINT
			    plcstab(blevel);
#endif
			    } break;
case 15:
# line 183 "../mip/cgram.y"
{  yypvt[-2].nodep->in.op = FREE; 
#ifndef LINT
			    plcstab(blevel);
#endif
			    } break;
case 16:
# line 191 "../mip/cgram.y"
{  yypvt[-1].nodep->in.op = FREE; } break;
case 17:
# line 193 "../mip/cgram.y"
{  yypvt[-2].nodep->in.op = FREE; } break;
case 19:
# line 197 "../mip/cgram.y"
{ curclass = SNULL;  yypvt[-2].nodep->in.op = FREE; } break;
case 20:
# line 199 "../mip/cgram.y"
{ curclass = SNULL;  yypvt[-1].nodep->in.op = FREE; } break;
case 21:
# line 201 "../mip/cgram.y"
{  curclass = SNULL; } break;
case 23:
# line 205 "../mip/cgram.y"
{  yyval.nodep = mkty(INT,0,INT);  curclass = SNULL; } break;
case 24:
# line 208 "../mip/cgram.y"
{  yyval.nodep = yypvt[-0].nodep; } break;
case 26:
# line 211 "../mip/cgram.y"
{  yyval.nodep = mkty(INT,0,INT); } break;
case 27:
# line 213 "../mip/cgram.y"
{ curclass = SNULL ; } break;
case 28:
# line 215 "../mip/cgram.y"
{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-0].nodep->in.type, UNDEF );
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 29:
# line 222 "../mip/cgram.y"
{  curclass = yypvt[-0].intval; } break;
case 31:
# line 227 "../mip/cgram.y"
{  yypvt[-1].nodep->in.type = types( yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type, UNDEF );
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 32:
# line 231 "../mip/cgram.y"
{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type );
			    yypvt[-1].nodep->in.op = yypvt[-0].nodep->in.op = FREE;
			    } break;
case 35:
# line 239 "../mip/cgram.y"
{ yyval.nodep = dclstruct(yypvt[-4].intval); } break;
case 36:
# line 241 "../mip/cgram.y"
{  yyval.nodep = rstruct(yypvt[-0].intval,0);  stwart = instruct; } break;
case 37:
# line 245 "../mip/cgram.y"
{  yyval.intval = bstruct(-1,0); stwart = SEENAME; } break;
case 38:
# line 247 "../mip/cgram.y"
{  yyval.intval = bstruct(yypvt[-0].intval,0); stwart = SEENAME; } break;
case 41:
# line 255 "../mip/cgram.y"
{  moedef( yypvt[-0].intval ); } break;
case 42:
# line 257 "../mip/cgram.y"
{  strucoff = yypvt[-0].intval;  moedef( yypvt[-2].intval ); } break;
case 43:
# line 261 "../mip/cgram.y"
{ yyval.nodep = dclstruct(yypvt[-4].intval);  } break;
case 44:
# line 263 "../mip/cgram.y"
{  yyval.nodep = rstruct(yypvt[-0].intval,yypvt[-1].intval); } break;
case 45:
# line 267 "../mip/cgram.y"
{  yyval.intval = bstruct(-1,yypvt[-0].intval);  stwart=0; } break;
case 46:
# line 269 "../mip/cgram.y"
{  yyval.intval = bstruct(yypvt[-0].intval,yypvt[-1].intval);  stwart=0;  } break;
case 49:
# line 277 "../mip/cgram.y"
{ curclass = SNULL;  stwart=0; yypvt[-1].nodep->in.op = FREE; } break;
case 50:
# line 279 "../mip/cgram.y"
{  if( curclass != MOU ){
				curclass = SNULL;
				}
			    else {
				sprintf( fakename, "$%dFAKE", fake++ );
#ifdef FLEXSTRINGS
				/* No need to hash this, we won't look it up */
				defid( tymerge(yypvt[-0].nodep, bdty(NAME,NIL,lookup( savestr(fakename), SMOS ))), curclass );
#else
				defid( tymerge(yypvt[-0].nodep, bdty(NAME,NIL,lookup( fakename, SMOS ))), curclass );
#endif
				werror("structure typed union member must be named");
				}
			    stwart = 0;
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 51:
# line 299 "../mip/cgram.y"
{ defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);  stwart = instruct; } break;
case 52:
# line 300 "../mip/cgram.y"
{yyval.nodep=yypvt[-2].nodep;} break;
case 53:
# line 301 "../mip/cgram.y"
{ defid( tymerge(yypvt[-4].nodep,yypvt[-0].nodep), curclass);  stwart = instruct; } break;
case 56:
# line 307 "../mip/cgram.y"
{  if( !(instruct&INSTRUCT) ) uerror( "field outside of structure" );
			    if( yypvt[-0].intval<0 || yypvt[-0].intval >= FIELD ){
				uerror( "illegal field size" );
				yypvt[-0].intval = 1;
				}
			    defid( tymerge(yypvt[-3].nodep,yypvt[-2].nodep), FIELD|yypvt[-0].intval );
			    yyval.nodep = NIL;
			    } break;
case 57:
# line 317 "../mip/cgram.y"
{  if( !(instruct&INSTRUCT) ) uerror( "field outside of structure" );
			    falloc( stab, yypvt[-0].intval, -1, yypvt[-2].nodep );  /* alignment or hole */
			    yyval.nodep = NIL;
			    } break;
case 58:
# line 322 "../mip/cgram.y"
{  yyval.nodep = NIL; } break;
case 59:
# line 327 "../mip/cgram.y"
{  umul:
				yyval.nodep = bdty( UNARY MUL, yypvt[-0].nodep, 0 ); } break;
case 60:
# line 330 "../mip/cgram.y"
{  uftn:
				yyval.nodep = bdty( UNARY CALL, yypvt[-2].nodep, 0 );  } break;
case 61:
# line 333 "../mip/cgram.y"
{  uary:
				yyval.nodep = bdty( LB, yypvt[-2].nodep, 0 );  } break;
case 62:
# line 336 "../mip/cgram.y"
{  bary:
				if( (int)yypvt[-1].intval <= 0 ) werror( "zero or negative subscript" );
				yyval.nodep = bdty( LB, yypvt[-3].nodep, yypvt[-1].intval );  } break;
case 63:
# line 340 "../mip/cgram.y"
{  yyval.nodep = bdty( NAME, NIL, yypvt[-0].intval );  } break;
case 64:
# line 342 "../mip/cgram.y"
{ yyval.nodep=yypvt[-1].nodep; } break;
case 65:
# line 345 "../mip/cgram.y"
{  goto umul; } break;
case 66:
# line 347 "../mip/cgram.y"
{  goto uftn; } break;
case 67:
# line 349 "../mip/cgram.y"
{  goto uary; } break;
case 68:
# line 351 "../mip/cgram.y"
{  goto bary; } break;
case 69:
# line 353 "../mip/cgram.y"
{ yyval.nodep = yypvt[-1].nodep; } break;
case 70:
# line 355 "../mip/cgram.y"
{
				if( blevel!=0 ) uerror("function declaration in bad context");
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-2].intval), 0 );
				stwart = 0;
				} break;
case 71:
# line 361 "../mip/cgram.y"
{
				yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,yypvt[-1].intval), 0 );
				stwart = 0;
				} break;
case 72:
# line 368 "../mip/cgram.y"
{
				/* turn off typedefs for argument names */
				stwart = SEENAME;
				if( stab[yypvt[-1].intval].sclass == SNULL )
				    stab[yypvt[-1].intval].stype = FTN;
				} break;
case 73:
# line 377 "../mip/cgram.y"
{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 74:
# line 379 "../mip/cgram.y"
{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 77:
# line 385 "../mip/cgram.y"
{yyval.nodep=yypvt[-2].nodep;} break;
case 79:
# line 389 "../mip/cgram.y"
{  defid( yypvt[-0].nodep = tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
			    beginit(yypvt[-0].nodep->tn.rval);
			    } break;
case 81:
# line 396 "../mip/cgram.y"
{  nidcl( tymerge(yypvt[-1].nodep,yypvt[-0].nodep) ); } break;
case 82:
# line 398 "../mip/cgram.y"
{  defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), uclass(curclass) );
			} break;
case 83:
# line 402 "../mip/cgram.y"
{  doinit( yypvt[-0].nodep );
			    endinit(); } break;
case 84:
# line 405 "../mip/cgram.y"
{  endinit(); } break;
case 88:
# line 415 "../mip/cgram.y"
{  doinit( yypvt[-0].nodep ); } break;
case 89:
# line 417 "../mip/cgram.y"
{  irbrace(); } break;
case 94:
# line 429 "../mip/cgram.y"
{  werror( "old-fashioned initialization: use =" ); } break;
case 96:
# line 434 "../mip/cgram.y"
{  ilbrace(); } break;
case 99:
# line 444 "../mip/cgram.y"
{  
#ifndef LINT
			    prcstab(blevel);
#endif
			    --blevel;
			    if( blevel == 1 ) blevel = 0;
			    clearst( blevel );
			    checkst( blevel );
			    autooff = *--psavbc;
			    regvar = *--psavbc;
			    } break;
case 100:
# line 458 "../mip/cgram.y"
{  --blevel;
			    if( blevel == 1 ) blevel = 0;
			    clearst( blevel );
			    checkst( blevel );
			    autooff = *--psavbc;
			    regvar = *--psavbc;
			    } break;
case 101:
# line 468 "../mip/cgram.y"
{  if( blevel == 1 ) dclargs();
			    ++blevel;
			    if( psavbc > &asavbc[BCSZ-2] ) cerror( "nesting too deep" );
			    *psavbc++ = regvar;
			    *psavbc++ = autooff;
			    } break;
case 102:
# line 477 "../mip/cgram.y"
{ ecomp( yypvt[-1].nodep ); } break;
case 104:
# line 480 "../mip/cgram.y"
{ deflab(yypvt[-1].intval);
			   reached = 1;
			   } break;
case 105:
# line 484 "../mip/cgram.y"
{  if( yypvt[-1].intval != NOLAB ){
				deflab( yypvt[-1].intval );
				reached = 1;
				}
			    } break;
case 106:
# line 490 "../mip/cgram.y"
{  branch(  contlab );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP)) reached = 1;
			    else reached = 0;
			    resetbc(0);
			    } break;
case 107:
# line 497 "../mip/cgram.y"
{  deflab( contlab );
			    if( flostat & FCONT ) reached = 1;
			    ecomp( buildtree( CBRANCH, buildtree( NOT, yypvt[-2].nodep, NIL ), bcon( yypvt[-6].intval ) ) );
			    deflab( brklab );
			    reached = 1;
			    resetbc(0);
			    } break;
case 108:
# line 505 "../mip/cgram.y"
{  deflab( contlab );
			    if( flostat&FCONT ) reached = 1;
			    if( yypvt[-2].nodep ) ecomp( yypvt[-2].nodep );
			    branch( yypvt[-3].intval );
			    deflab( brklab );
			    if( (flostat&FBRK) || !(flostat&FLOOP) ) reached = 1;
			    else reached = 0;
			    resetbc(0);
			    } break;
case 109:
# line 515 "../mip/cgram.y"
{  if( reached ) branch( brklab );
			    deflab( yypvt[-1].intval );
			   swend();
			    deflab(brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached = 1;
			    resetbc(FCONT);
			    } break;
case 110:
# line 523 "../mip/cgram.y"
{  if( brklab == NOLAB ) uerror( "illegal break");
			    else if(reached) branch( brklab );
			    flostat |= FBRK;
			    if( brkflag ) goto rch;
			    reached = 0;
			    } break;
case 111:
# line 530 "../mip/cgram.y"
{  if( contlab == NOLAB ) uerror( "illegal continue");
			    else branch( contlab );
			    flostat |= FCONT;
			    goto rch;
			    } break;
case 112:
# line 536 "../mip/cgram.y"
{  retstat |= NRETVAL;
			    branch( retlab );
			rch:
			    if( !reached ) werror( "statement not reached");
			    reached = 0;
			    } break;
case 113:
# line 543 "../mip/cgram.y"
{  register NODE *temp;
			    idname = curftn;
			    temp = buildtree( NAME, NIL, NIL );
			    if(temp->in.type == TVOID)
				uerror("void function %s cannot return value",
					stab[idname].sname);
			    temp->in.type = DECREF( temp->in.type );
			    temp = buildtree( RETURN, temp, yypvt[-1].nodep );
			    /* now, we have the type of the RHS correct */
			    temp->in.left->in.op = FREE;
			    temp->in.op = FREE;
			    ecomp( buildtree( FORCE, temp->in.right, NIL ) );
			    retstat |= RETVAL;
			    branch( retlab );
			    reached = 0;
			    } break;
case 114:
# line 560 "../mip/cgram.y"
{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, INT );
			    q->tn.rval = idname = yypvt[-1].intval;
			    defid( q, ULABEL );
			    stab[idname].suse = -lineno;
			    branch( stab[idname].offset );
			    goto rch;
			    } break;
case 119:
# line 574 "../mip/cgram.y"
{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, LABEL );
			    q->tn.rval = yypvt[-1].intval;
			    defid( q, LABEL );
			    reached = 1;
			    } break;
case 120:
# line 581 "../mip/cgram.y"
{  addcase(yypvt[-1].nodep);
			    reached = 1;
			    } break;
case 121:
# line 585 "../mip/cgram.y"
{  reached = 1;
			    adddef();
			    flostat |= FDEF;
			    } break;
case 122:
# line 591 "../mip/cgram.y"
{  savebc();
			    if( !reached ) werror( "loop not entered at top");
			    brklab = getlab();
			    contlab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;
			    } break;
case 123:
# line 600 "../mip/cgram.y"
{  ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( yyval.intval=getlab()) ) ) ;
			    reached = 1;
			    } break;
case 124:
# line 605 "../mip/cgram.y"
{  if( reached ) branch( yyval.intval = getlab() );
			    else yyval.intval = NOLAB;
			    deflab( yypvt[-2].intval );
			    reached = 1;
			    } break;
case 125:
# line 613 "../mip/cgram.y"
{  savebc();
			    if( !reached ) werror( "loop not entered at top");
			    if( yypvt[-1].nodep->in.op == ICON && yypvt[-1].nodep->tn.lval != 0 ) flostat = FLOOP;
			    deflab( contlab = getlab() );
			    reached = 1;
			    brklab = getlab();
			    if( flostat == FLOOP ) tfree( yypvt[-1].nodep );
			    else ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( brklab) ) );
			    } break;
case 126:
# line 624 "../mip/cgram.y"
{  if( yypvt[-3].nodep ) ecomp( yypvt[-3].nodep );
			    else if( !reached ) werror( "loop not entered at top");
			    savebc();
			    contlab = getlab();
			    brklab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;
			    if( yypvt[-1].nodep ) ecomp( buildtree( CBRANCH, yypvt[-1].nodep, bcon( brklab) ) );
			    else flostat |= FLOOP;
			    } break;
case 127:
# line 636 "../mip/cgram.y"
{  savebc();
			    brklab = getlab();
			    ecomp( buildtree( FORCE, yypvt[-1].nodep, NIL ) );
			    branch( yyval.intval = getlab() );
			    swstart();
			    reached = 0;
			    } break;
case 128:
# line 645 "../mip/cgram.y"
{ yyval.intval=instruct; stwart=instruct=0; } break;
case 129:
# line 647 "../mip/cgram.y"
{  yyval.intval = icons( yypvt[-0].nodep );  instruct=yypvt[-1].intval; } break;
case 131:
# line 651 "../mip/cgram.y"
{ yyval.nodep=0; } break;
case 133:
# line 656 "../mip/cgram.y"
{  goto bop; } break;
case 134:
# line 660 "../mip/cgram.y"
{
			preconf:
			    if( yychar==RELOP||yychar==EQUOP||yychar==AND||yychar==OR||yychar==ER ){
			    precplaint:
				if( hflag ) werror( "precedence confusion possible: parenthesize!" );
				}
			bop:
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep );
			    } break;
case 135:
# line 670 "../mip/cgram.y"
{  yypvt[-1].intval = COMOP;
			    goto bop;
			    } break;
case 136:
# line 674 "../mip/cgram.y"
{  goto bop; } break;
case 137:
# line 676 "../mip/cgram.y"
{  if(yychar==SHIFTOP) goto precplaint; else goto bop; } break;
case 138:
# line 678 "../mip/cgram.y"
{  if(yychar==SHIFTOP ) goto precplaint; else goto bop; } break;
case 139:
# line 680 "../mip/cgram.y"
{  if(yychar==PLUS||yychar==MINUS) goto precplaint; else goto bop; } break;
case 140:
# line 682 "../mip/cgram.y"
{  goto bop; } break;
case 141:
# line 684 "../mip/cgram.y"
{  goto preconf; } break;
case 142:
# line 686 "../mip/cgram.y"
{  if( yychar==RELOP||yychar==EQUOP ) goto preconf;  else goto bop; } break;
case 143:
# line 688 "../mip/cgram.y"
{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 144:
# line 690 "../mip/cgram.y"
{  if(yychar==RELOP||yychar==EQUOP) goto preconf; else goto bop; } break;
case 145:
# line 692 "../mip/cgram.y"
{  goto bop; } break;
case 146:
# line 694 "../mip/cgram.y"
{  goto bop; } break;
case 147:
# line 696 "../mip/cgram.y"
{  abop:
				yyval.nodep = buildtree( ASG yypvt[-2].intval, yypvt[-3].nodep, yypvt[-0].nodep );
				} break;
case 148:
# line 700 "../mip/cgram.y"
{  goto abop; } break;
case 149:
# line 702 "../mip/cgram.y"
{  goto abop; } break;
case 150:
# line 704 "../mip/cgram.y"
{  goto abop; } break;
case 151:
# line 706 "../mip/cgram.y"
{  goto abop; } break;
case 152:
# line 708 "../mip/cgram.y"
{  goto abop; } break;
case 153:
# line 710 "../mip/cgram.y"
{  goto abop; } break;
case 154:
# line 712 "../mip/cgram.y"
{  goto abop; } break;
case 155:
# line 714 "../mip/cgram.y"
{  yyval.nodep=buildtree(QUEST, yypvt[-4].nodep, buildtree( COLON, yypvt[-2].nodep, yypvt[-0].nodep ) );
			    } break;
case 156:
# line 717 "../mip/cgram.y"
{  werror( "old-fashioned assignment operator" );  goto bop; } break;
case 157:
# line 719 "../mip/cgram.y"
{  goto bop; } break;
case 159:
# line 723 "../mip/cgram.y"
{  yyval.nodep = buildtree( yypvt[-0].intval, yypvt[-1].nodep, bcon(1) ); } break;
case 160:
# line 725 "../mip/cgram.y"
{ ubop:
			    yyval.nodep = buildtree( UNARY yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 161:
# line 729 "../mip/cgram.y"
{  if( ISFTN(yypvt[-0].nodep->in.type) || ISARY(yypvt[-0].nodep->in.type) ){
				werror( "& before array or function: ignored" );
				yyval.nodep = yypvt[-0].nodep;
				}
			    else goto ubop;
			    } break;
case 162:
# line 736 "../mip/cgram.y"
{  goto ubop; } break;
case 163:
# line 738 "../mip/cgram.y"
{
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 164:
# line 742 "../mip/cgram.y"
{  yyval.nodep = buildtree( yypvt[-1].intval==INCR ? ASG PLUS : ASG MINUS,
						yypvt[-0].nodep,
						bcon(1)  );
			    } break;
case 165:
# line 747 "../mip/cgram.y"
{  yyval.nodep = doszof( yypvt[-0].nodep ); } break;
case 166:
# line 749 "../mip/cgram.y"
{  yyval.nodep = buildtree( CAST, yypvt[-2].nodep, yypvt[-0].nodep );
			    yyval.nodep->in.left->in.op = FREE;
			    yyval.nodep->in.op = FREE;
			    yyval.nodep = yyval.nodep->in.right;
			    } break;
case 167:
# line 755 "../mip/cgram.y"
{  yyval.nodep = doszof( yypvt[-1].nodep ); } break;
case 168:
# line 757 "../mip/cgram.y"
{  yyval.nodep = buildtree( UNARY MUL, buildtree( PLUS, yypvt[-3].nodep, yypvt[-1].nodep ), NIL ); } break;
case 169:
# line 759 "../mip/cgram.y"
{  yyval.nodep=buildtree(UNARY CALL,yypvt[-1].nodep,NIL); } break;
case 170:
# line 761 "../mip/cgram.y"
{  yyval.nodep=buildtree(CALL,yypvt[-2].nodep,yypvt[-1].nodep); } break;
case 171:
# line 763 "../mip/cgram.y"
{  if( yypvt[-1].intval == DOT ){
				if( notlval( yypvt[-2].nodep ) )uerror("structure reference must be addressable");
				yypvt[-2].nodep = buildtree( UNARY AND, yypvt[-2].nodep, NIL );
				}
			    idname = yypvt[-0].intval;
			    yyval.nodep = buildtree( STREF, yypvt[-2].nodep, buildtree( NAME, NIL, NIL ) );
			    } break;
case 172:
# line 771 "../mip/cgram.y"
{  idname = yypvt[-0].intval;
			    /* recognize identifiers in initializations */
			    if( blevel==0 && stab[idname].stype == UNDEF ) {
				register NODE *q;
#ifndef FLEXNAMES
				werror( "undeclared initializer name %.8s", stab[idname].sname );
#else
				werror( "undeclared initializer name %s", stab[idname].sname );
#endif
				q = block( FREE, NIL, NIL, INT, 0, INT );
				q->tn.rval = idname;
				defid( q, EXTERN );
				}
			    yyval.nodep=buildtree(NAME,NIL,NIL);
			    stab[yypvt[-0].intval].suse = -lineno;
			} break;
case 173:
# line 788 "../mip/cgram.y"
{  yyval.nodep=bcon(0);
			    yyval.nodep->tn.lval = lastcon;
			    yyval.nodep->tn.rval = NONAME;
			    if( yypvt[-0].intval ) yyval.nodep->fn.csiz = yyval.nodep->in.type = ctype(LONG);
			    } break;
case 174:
# line 794 "../mip/cgram.y"
{  yyval.nodep=buildtree(FCON,NIL,NIL);
			    yyval.nodep->fpn.dval = dcon;
			    } break;
case 175:
# line 798 "../mip/cgram.y"
{  yyval.nodep = getstr(); /* get string contents */ } break;
case 176:
# line 800 "../mip/cgram.y"
{ yyval.nodep=yypvt[-1].nodep; } break;
case 177:
# line 804 "../mip/cgram.y"
{
			yyval.nodep = tymerge( yypvt[-1].nodep, yypvt[-0].nodep );
			yyval.nodep->in.op = NAME;
			yypvt[-1].nodep->in.op = FREE;
			} break;
case 178:
# line 812 "../mip/cgram.y"
{ yyval.nodep = bdty( NAME, NIL, -1 ); } break;
case 179:
# line 814 "../mip/cgram.y"
{ yyval.nodep = bdty( UNARY CALL, bdty(NAME,NIL,-1),0); } break;
case 180:
# line 816 "../mip/cgram.y"
{  yyval.nodep = bdty( UNARY CALL, yypvt[-3].nodep, 0 ); } break;
case 181:
# line 818 "../mip/cgram.y"
{  goto umul; } break;
case 182:
# line 820 "../mip/cgram.y"
{  goto uary; } break;
case 183:
# line 822 "../mip/cgram.y"
{  goto bary;  } break;
case 184:
# line 824 "../mip/cgram.y"
{ yyval.nodep = yypvt[-1].nodep; } break;
case 185:
# line 828 "../mip/cgram.y"
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
