

static	char	SCCSID[] = "@(#) cgram.y:	2.1 83/08/02";





















































extern int yychar;
extern short yyerrflag;



YYSTYPE yylval, yyval;


	extern int wloop_level;	
	extern int floop_level;	
	static int fake = 0;
	static char fakename[NCHNAM+1];





NODE *
mkty( t, d, s ) unsigned t; {
	return( block( 33, NIL, NIL, t, d, s ) );
	}

NODE *
bdty( op, p, v ) NODE *p; {
	register NODE *q;

	q = block( op, p, NIL, INT, 0, INT );

	switch( op ){

	case UNARY 11:
	case UNARY CALL:
		break;

	case 54:
		q->in.right = bcon(v);
		break;

	case 2:
		q->tn.rval = v;
		break;

	default:
		cerror( "bad bdty" );
		}

	return( q );
	}

dstash( n ){ 
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

addcase(p) NODE *p; { 

	p = optim( p );  
	if( p->in.op != 4 || p->tn.rval != NONAME){
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

adddef(){ 
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
	
	if( swp >= &swtab[SWITSZ] ){
		cerror( "switch table overflow");
		}
	swx = swp - swtab;
	swp->slab = -1;
	++swp;
	}

swend(){ 

	register struct sw *swbeg, *p, *q, *r, *r1;
	CONSZ temp;
	int tempi;

	swbeg = &swtab[swx+1];

	

	r1 = swbeg;
	r = swp-1;

	while( swbeg < r ){
		
		for( q=swbeg; q<r; ++q ){
			if( q->sval > (q+1)->sval ){
				
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










YYSTYPE yyv[150]; 
int yychar = -1; 
int yynerrs = 0;  
short yyerrflag = 0;  

yyparse()
{	short yys[150];
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

yystack:    

	if( ++yyps >= &yys[150] ) { 
		yyerror( "yacc stack overflow" ); 
		return(1); 
	}
	*yyps = yystate;
	++yypv;
	*yypv = yyval;
yynewstate:
	yyn = yypact[yystate];
	if(yyn <= -1000) goto yydefault; 
	if(yychar<0) {
		yychar = yylex();

		if(yychar < 0)
			yychar = 0;
	}
	if((yyn += yychar) < 0 || yyn >= 1176)
		goto yydefault;
	if( yychk[ yyn=yyact[ yyn ] ] == yychar ){ 
		yychar = -1;
		yyval = yylval;
		yystate = yyn;
		if( yyerrflag > 0 ) --yyerrflag;
		goto yystack;
	}
yydefault:
	
	if( (yyn=yydef[yystate]) == -2 ) {
		if(yychar < 0) {
			yychar = yylex();

			if(yychar < 0)
				yychar = 0;
		}
		
		for(yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=yystate);
			yyxi += 2 ) ; 
		while( *(yyxi+=2) >= 0 ){
			if( *yyxi == yychar ) break;
		}
		if( (yyn = yyxi[1]) < 0 ) return(0);   
	}
	if( yyn == 0 ){ 
		
		switch( yyerrflag ){
		case 0:   

			yyerror( "syntax error" );

yyerrlab:
			++yynerrs;
		case 1:
		case 2: 
			yyerrflag = 3;
			
			while ( yyps >= yys ) {
				yyn = yypact[*yyps] + 256;
				if( yyn>= 0 && yyn < 1176 && yychk[yyact[yyn]] == 256 ){
					yystate = yyact[yyn];  
					goto yystack;
				}
				yyn = yypact[*yyps];
				

				--yyps;
				--yypv;
			}
			
yyabort:
			return(1);
		case 3:  

			if( yychar == 0 ) goto yyabort; 
			yychar = -1;
			goto yynewstate;   
		}
	}
	

	yyps -= yyr2[yyn];
	yypvt = yypv;
	yypv -= yyr2[yyn];
	yyval = yypv[1];
	yym=yyn;
	
	yyn = yyr1[yyn];
	yyj = yypgo[yyn] + *yyps + 1;
	if( yyj>=1176 || yychk[ yystate = yyact[yyj] ] != -yyn ) yystate = yyact[yypgo[yyn]];
	switch(yym){
		
case 2:

{

			beg_file();

			 ftnend();
			 } break;
case 3:

{ curclass = SNULL;  blevel = 0; } break;
case 4:

{ curclass = SNULL;  blevel = 0; } break;
case 5:

{ asmout();  curclass = SNULL; blevel = 0; } break;
case 6:

{  yypvt[-1].nodep->in.op = FREE; } break;
case 7:

{  yypvt[-2].nodep->in.op = FREE; } break;
case 8:

{   defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep),
					curclass==STATIC?STATIC:EXTDEF ); } break;
case 9:

{  
			    if( blevel ) cerror( "function level error" );
			    if( reached ) retstat |= NRETVAL; 
			    yypvt[-3].nodep->in.op = FREE;
			    ftnend();
			    } break;
case 10:

{	regvar = 0; } break;
case 11:

{ curclass = SNULL;  yypvt[-2].nodep->in.op = FREE; } break;
case 12:

{ curclass = SNULL;  yypvt[-1].nodep->in.op = FREE; } break;
case 13:

{  blevel = 1; } break;
case 15:

{  bccode();
			    locctr(PROG);
			    } break;
case 16:

{  yypvt[-1].nodep->in.op = FREE; } break;
case 17:

{  yypvt[-2].nodep->in.op = FREE; } break;
case 20:

{  yyval.nodep = mkty(INT,0,INT);  curclass = SNULL; } break;
case 21:

{  yyval.nodep = yypvt[-0].nodep; } break;
case 23:

{  yyval.nodep = mkty(INT,0,INT); } break;
case 24:

{ curclass = SNULL ; } break;
case 25:

{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-0].nodep->in.type,
					UNDEF );
 			    yypvt[-0].nodep->in.op = FREE;
 			    } break;
case 26:

{  curclass = yypvt[-0].intval; } break;
case 28:

{  yypvt[-1].nodep->in.type = types( yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type, UNDEF );
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 29:

{  yypvt[-2].nodep->in.type = types( yypvt[-2].nodep->in.type, yypvt[-1].nodep->in.type, yypvt[-0].nodep->in.type );
			    yypvt[-1].nodep->in.op = yypvt[-0].nodep->in.op = FREE;
			    } break;
case 32:

{ yyval.nodep = dclstruct(yypvt[-4].intval); } break;
case 33:

{  yyval.nodep = rstruct(yypvt[-0].intval,0);  stwart = instruct; } break;
case 34:

{  yyval.intval = bstruct(-1,0); stwart = SEENAME; } break;
case 35:

{  yyval.intval = bstruct(yypvt[-0].intval,0); stwart = SEENAME; } break;
case 38:

{  moedef( yypvt[-0].intval ); } break;
case 39:

{  strucoff = yypvt[-0].intval;  moedef( yypvt[-2].intval ); } break;
case 40:

{ yyval.nodep = dclstruct(yypvt[-4].intval);  } break;
case 41:

{  yyval.nodep = rstruct(yypvt[-0].intval,yypvt[-1].intval); } break;
case 42:

{  yyval.intval = bstruct(-1,yypvt[-0].intval);  stwart=0; } break;
case 43:

{  yyval.intval = bstruct(yypvt[-0].intval,yypvt[-1].intval);  stwart=0;  } break;
case 46:

{ curclass = SNULL;  stwart=0; yypvt[-1].nodep->in.op = FREE; } break;
case 47:

{  if( curclass != MOU ){
				curclass = SNULL;
				}
			    else {
				sprintf( fakename, "$%dFAKE", fake++ );
				defid( tymerge(yypvt[-0].nodep, bdty(2,NIL,
					lookup( fakename, SMOS ))), curclass );
				werror("union member must be named");
				}
			    stwart = 0;
			    yypvt[-0].nodep->in.op = FREE;
			    } break;
case 48:

{ defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 49:

{yyval.nodep=yypvt[-2].nodep;} break;
case 50:

{ defid( tymerge(yypvt[-4].nodep,yypvt[-0].nodep), curclass);
				stwart = instruct; } break;
case 52:

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

{   if( !(instruct&INSTRUCT) )
				uerror( "field outside of structure" );
			    
			    falloc( stab, yypvt[-0].intval, -1, yypvt[-2].nodep );
			    yyval.nodep = NIL;
			    } break;
case 54:

{  yyval.nodep = NIL; } break;
case 55:

{  umul:
				yyval.nodep = bdty( UNARY 11, yypvt[-0].nodep, 0 ); } break;
case 56:

{  uftn:
				yyval.nodep = bdty( UNARY CALL, yypvt[-2].nodep, 0 );  } break;
case 57:

{  uary:
				yyval.nodep = bdty( 54, yypvt[-2].nodep, 0 );  } break;
case 58:

{  bary:
				if( (int)yypvt[-1].intval <= 0 )
					werror( "zero or negative subscript" );
				yyval.nodep = bdty( 54, yypvt[-3].nodep, yypvt[-1].intval );  } break;
case 59:

{  yyval.nodep = bdty( 2, NIL, yypvt[-0].intval );  } break;
case 60:

{ yyval.nodep=yypvt[-1].nodep; } break;
case 61:

{  goto umul; } break;
case 62:

{  goto uftn; } break;
case 63:

{  goto uary; } break;
case 64:

{  goto bary; } break;
case 65:

{ yyval.nodep = yypvt[-1].nodep; } break;
case 66:

{ if (paramno)
				uerror("arg list in declaration"); } break;
case 67:

{
				if( blevel!=0 )
					uerror(
					"function declaration in bad context");
				yyval.nodep = bdty( UNARY CALL, bdty(2,NIL,yypvt[-3].intval), 0 );
				stwart = 0;
				} break;
case 68:

{ if (paramno)
				uerror("arg list in declaration"); } break;
case 69:

{
				yyval.nodep = bdty( UNARY CALL, bdty(2,NIL,yypvt[-2].intval), 0 );
				stwart = 0;
				} break;
case 70:

{
				
				stwart = SEENAME;
				if( stab[yypvt[-1].intval].sclass == SNULL )
				    stab[yypvt[-1].intval].stype = FTN;
				} break;
case 71:

{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 72:

{ ftnarg( yypvt[-0].intval );  stwart = SEENAME; } break;
case 75:

{yyval.nodep=yypvt[-2].nodep;} break;
case 77:

{  defid( yypvt[-0].nodep = tymerge(yypvt[-1].nodep,yypvt[-0].nodep), curclass);
			    beginit(yypvt[-0].nodep->tn.rval);
			    } break;
case 79:

{  nidcl( tymerge(yypvt[-1].nodep,yypvt[-0].nodep) ); } break;
case 80:

{  defid( tymerge(yypvt[-1].nodep,yypvt[-0].nodep), uclass(curclass) );
			} break;
case 81:

{  doinit( yypvt[-0].nodep );
			    endinit(); } break;
case 82:

{  endinit(); } break;
case 86:

{  doinit( yypvt[-0].nodep ); } break;
case 87:

{  irbrace(); } break;
case 92:

{  ilbrace(); } break;
case 93:

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

{  if( blevel == 1 ) dclargs();
			    uplevel();
			    if( psavbc > &asavbc[BCSZ-2] )
					cerror( "nesting too deep" );
			    *psavbc++ = regvar;
			    *psavbc++ = autooff;
			    } break;
case 95:

{ ecomp(yypvt[-1].nodep); } break;
case 96:

{ asmout(); } break;
case 98:

{ deflab(yypvt[-1].intval);
			   reached = 1;

			   } break;
case 99:

{  if( yypvt[-1].intval != NOLAB ){
				deflab( yypvt[-1].intval );
				reached = 1;
				}

			    } break;
case 100:

{

			} break;
case 101:

{
				savebc();
				if (!reached)
					werror("loop not entered at top");
				reached = 1;
				brklab = getlab();
				contlab = getlab();
				switch (wloop_level) {
				default:
					cerror("bad while loop code gen value");
					
				case LL_TOP:	
					deflab(contlab);
					if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
					} else {
						yypvt[-1].nodep->ln.lineno = yypvt[-4].lineno;
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
						    bcon(brklab)));
					}
					break;
				case LL_BOT:	
					if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
						deflab(contlab);
					} else {
						branch(contlab);
						deflab(yyval.intval = getlab());
					}
					break;
				case LL_DUP:	
					if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat = FLOOP;
						tfree(yypvt[-1].nodep);
						deflab(yyval.intval = contlab);
					} else {
						register NODE *sav;
						extern NODE *treecpy();
						sav = treecpy(yypvt[-1].nodep);
						ecomp(buildtree(CBRANCH,yypvt[-1].nodep,
						    bcon(brklab)));
						yypvt[-1].nodep = sav;
						deflab(yyval.intval = getlab());
					}
					break;
				}
			} break;
case 102:

{
			switch (wloop_level) {
			default:
				cerror("bad while loop code gen. value");
				
			case LL_TOP:	
				branch(contlab);
				break;
			case LL_BOT:	
				if (flostat & FLOOP)
					branch(contlab);
				else {
					reached = 1;
					deflab(contlab);
					ecomp(buildtree(CBRANCH,
						buildtree(NOT, yypvt[-3].nodep, NIL),
						bcon(yypvt[-1].intval)));
				}
				break;
			case LL_DUP:	
				if (flostat & FLOOP)
					branch(contlab);
				else {
					if (flostat & FCONT) {
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

		} break;
case 103:

{   deflab( contlab );
			    if( flostat & FCONT ) reached = 1;
			    yypvt[-2].nodep->ln.lineno = yypvt[-4].lineno;
			    ecomp( buildtree(CBRANCH,
				buildtree(NOT, yypvt[-2].nodep, NIL), bcon(yypvt[-6].intval)));
			    deflab( brklab );
			    reached = 1;
			    resetbc(0);

			    } break;
case 104:

{

			} break;
case 105:

{
				if (yypvt[-4].nodep) {
					yypvt[-4].nodep->ln.lineno = yypvt[-6].lineno;
					ecomp(yypvt[-4].nodep);
				} else if (!reached)
					werror("loop not entered at top");
				savebc();
				contlab = getlab();
				brklab = getlab();
				reached = 1;
				switch (floop_level) {
				default:
					cerror("bad for loop code gen. value");
					
				case LL_TOP:	
					deflab(yyval.intval = getlab());
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					} else {
						if (!yypvt[-4].nodep)
							yypvt[-1].nodep->ln.lineno = yypvt[-6].lineno;
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
						    bcon(brklab)));
					}
					break;
				case LL_BOT:	
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					} else
						branch(yypvt[-6].intval = getlab());
					deflab(yyval.intval = getlab());
					break;
				case LL_DUP:	
					if (!yypvt[-1].nodep)
						flostat |= FLOOP;
					else if (yypvt[-1].nodep->in.op == 4 && yypvt[-1].nodep->tn.lval) {
						flostat |= FLOOP;
						tfree(yypvt[-1].nodep);
						yypvt[-1].nodep = (NODE *)0;
					} else {
						register NODE *sav;
						extern NODE *treecpy();
						sav = treecpy(yypvt[-1].nodep);
						ecomp(buildtree(CBRANCH, yypvt[-1].nodep,
							bcon(brklab)));
						yypvt[-1].nodep = sav;
					}
					deflab(yyval.intval = getlab());
					break;
				}
			} break;
case 106:

{
			if (flostat & FCONT) {
				deflab(contlab);
				reached = 1;
			}
			if (yypvt[-2].nodep)
				yypvt[-2].nodep->ln.lineno = lineno, ecomp(yypvt[-2].nodep);
			switch (floop_level) {
			default:
				cerror("bad for loop code gen. value");
				
			case LL_TOP:	
				branch(yypvt[-3].intval);
				break;
			case LL_BOT:	
				if (yypvt[-5].nodep)
					deflab(yypvt[-10].intval);
				
			case LL_DUP:	
				if (yypvt[-5].nodep) {
					ecomp(buildtree(CBRANCH,
						buildtree(NOT, yypvt[-5].nodep, NIL),
						bcon(yypvt[-3].intval)));
				} else
					branch(yypvt[-3].intval);
				break;
			}
			deflab(brklab);
			if ((flostat & FBRK) || !(flostat & FLOOP))
				reached = 1;
			else
				reached = 0;
			resetbc(0);

		} break;
case 107:

{   if( reached ) branch( brklab );
			    deflab( yypvt[-1].intval );
			    swend();
			    deflab(brklab);
			    if( (flostat&FBRK) || !(flostat&FDEF) ) reached=1;
			    resetbc(FCONT);

			    } break;
case 108:

{  if( brklab == NOLAB ) uerror( "illegal break");
			    else if(reached) {
				slineno = yypvt[-1].lineno; dbline();
				branch( brklab );
			    }
			    flostat |= FBRK;
			    if( brkflag ) goto rch;
			    reached = 0;
			    } break;
case 109:

{   if( contlab == NOLAB ) uerror( "illegal continue");
			    else {
				slineno = yypvt[-1].lineno; dbline();
				branch( contlab );
			    }
			    flostat |= FCONT;
			    goto rch;
			    } break;
case 110:

{   retstat |= NRETVAL;
			    slineno = yypvt[-1].lineno; dbline();
			    branch( retlab );
			rch:
			    if( !reached ) werror( "statement not reached");
			    reached = 0;
			    } break;
case 111:

{   register NODE *temp;
			    TWORD indtype();
			    idname = curftn;
			    temp = buildtree( 2, NIL, NIL );
			    temp->in.type = DECREF(temp->in.type);
			    if(temp->in.type == (FTN|VOID))
				uerror(
				"void function %s cannot return value",
					stab[idname].sname);
 			    temp->tn.op = RNODE;
 			    yypvt[-1].nodep = makety( yypvt[-1].nodep, temp->fn.type,
					temp->fn.cdim, temp->fn.csiz );
			    temp->in.type = indtype( temp->in.type );
			    temp = buildtree( 58, temp, yypvt[-1].nodep );
			    temp->ln.lineno = yypvt[-2].lineno;
			    ecomp( temp );
			    retstat |= RETVAL;
			    branch( retlab );
			    reached = 0;
			    } break;
case 112:

{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, INT );
			    q->tn.rval = idname = yypvt[-1].intval;
			    defid( q, ULABEL );
			    stab[idname].suse = -lineno;
			    slineno = yypvt[-2].lineno; dbline();
			    branch( stab[idname].offset );
			    goto rch;
			    } break;
case 117:

{  register NODE *q;
			    q = block( FREE, NIL, NIL, INT|ARY, 0, LABEL );
			    q->tn.rval = yypvt[-1].intval;
			    defid( q, LABEL );
			    reached = 1;
			    } break;
case 118:

{  addcase(yypvt[-1].nodep);
			    reached = 1;
			    } break;
case 119:

{  reached = 1;
			    adddef();
			    flostat |= FDEF;
			    } break;
case 120:

{  savebc();
			    if( !reached ) werror( "loop not entered at top");
			    brklab = getlab();
			    contlab = getlab();
			    deflab( yyval.intval = getlab() );
			    reached = 1;

			    } break;
case 121:

{   yypvt[-1].nodep->ln.lineno = yypvt[-3].lineno;
			    ecomp(buildtree(CBRANCH, yypvt[-1].nodep, bcon(yyval.intval=getlab()))) ;
			    reached = 1;

			    } break;
case 122:

{  if( reached ) branch( yyval.intval = getlab() );
			    else yyval.intval = NOLAB;
			    deflab( yypvt[-2].intval );
			    reached = 1;
			    } break;
case 123:

{   register NODE *temp;
			    savebc();
			    temp = block( SNODE, NIL, NIL, INT, 0, INT );
			    temp = buildtree( 58, temp, yypvt[-1].nodep );

			    brklab = getlab();
			    temp->ln.lineno = yypvt[-3].lineno;
			    ecomp( temp );
			    branch( yyval.intval = getlab() );
			    swstart();
			    reached = 0;

			    } break;
case 124:

{ yyval.intval=instruct; stwart=instruct=0; } break;
case 125:

{  yyval.intval = icons( yypvt[-0].nodep ); instruct=yypvt[-1].intval; } break;
case 127:

{ yyval.nodep = 0; } break;
case 129:

{  goto bop; } break;
case 130:

{
			preconf:
			    if( yychar==26 || yychar==27 || yychar==14
					|| yychar==17 || yychar==19 ){
			precplaint:
				if( hflag ) werror(
				"precedence confusion possible: parenthesize!"
					);
				}
			bop:
			    yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-2].nodep, yypvt[-0].nodep );
			    } break;
case 131:

{  yypvt[-1].intval = COMOP;
			    goto bop;
			    } break;
case 132:

{  goto bop; } break;
case 133:

{  if(yychar==29) goto precplaint; else goto bop; } break;
case 134:

{  if(yychar==29 ) goto precplaint; else goto bop; } break;
case 135:

{  if(yychar==6||yychar==8) goto precplaint; else goto bop; } break;
case 136:

{  goto bop; } break;
case 137:

{  goto preconf; } break;
case 138:

{  if( yychar==26||yychar==27 ) goto preconf;  else goto bop; } break;
case 139:

{  if(yychar==26||yychar==27) goto preconf; else goto bop; } break;
case 140:

{  if(yychar==26||yychar==27) goto preconf; else goto bop; } break;
case 141:

{  goto bop; } break;
case 142:

{  goto bop; } break;
case 143:

{  abop:
				yyval.nodep = buildtree( ASG yypvt[-2].intval, yypvt[-3].nodep, yypvt[-0].nodep );
				} break;
case 144:

{  goto abop; } break;
case 145:

{  goto abop; } break;
case 146:

{  goto abop; } break;
case 147:

{  goto abop; } break;
case 148:

{  goto abop; } break;
case 149:

{  goto abop; } break;
case 150:

{  goto abop; } break;
case 151:

{  yyval.nodep=buildtree(21, yypvt[-4].nodep, buildtree( 22, yypvt[-2].nodep, yypvt[-0].nodep ) );
			    } break;
case 152:

{  werror( "old-fashioned assignment operator" );
				goto bop; } break;
case 153:

{  goto bop; } break;
case 155:

{  yyval.nodep = buildtree( yypvt[-0].intval, yypvt[-1].nodep, bcon(1) ); } break;
case 156:

{ ubop:
			   yyval.nodep = buildtree( UNARY yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 157:

{

			    if( ISFTN(yypvt[-0].nodep->in.type) || ISARY(yypvt[-0].nodep->in.type) ){
				werror( "& before array or function: ignored" );
				yyval.nodep = yypvt[-0].nodep;
				}
			    else goto ubop;
			    } break;
case 158:

{  goto ubop; } break;
case 159:

{
			   yyval.nodep = buildtree( yypvt[-1].intval, yypvt[-0].nodep, NIL );
			    } break;
case 160:

{  yyval.nodep = buildtree( yypvt[-1].intval==INCR ? ASG 6 : ASG 8,
						yypvt[-0].nodep,
						bcon(1)  );
			    } break;
case 161:

{  yyval.nodep = doszof( yypvt[-0].nodep ); } break;
case 162:

{  yyval.nodep = buildtree( CAST, yypvt[-2].nodep, yypvt[-0].nodep );
			   yyval.nodep->in.left->in.op = FREE;
			   yyval.nodep->in.op = FREE;
			   yyval.nodep = yyval.nodep->in.right;
			    } break;
case 163:

{  yyval.nodep = doszof( yypvt[-1].nodep ); } break;
case 164:

{  yyval.nodep = buildtree( 54, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 165:

{  yyval.nodep = xicolon( yypvt[-5].nodep, yypvt[-3].nodep, yypvt[-1].nodep ); } break;
case 166:

{  yyval.nodep=buildtree(UNARY CALL,yypvt[-1].nodep,NIL);
			} break;
case 167:

{  yyval.nodep=buildtree(CALL,yypvt[-2].nodep,yypvt[-1].nodep); } break;
case 168:

{  if( yypvt[-1].intval == DOT ){
				if( notlval( yypvt[-2].nodep ) )werror(
				       "structure reference must be addressable"
					);
				yypvt[-2].nodep = buildtree( UNARY 14, yypvt[-2].nodep, NIL );
				}
			    idname = yypvt[-0].intval;
			    yyval.nodep = buildtree( STREF, yypvt[-2].nodep,
					buildtree( 2, NIL, NIL ) );
			    } break;
case 169:

{  idname = yypvt[-0].intval;
			    
			    if( blevel==0 && stab[idname].stype == UNDEF ) {
				register NODE *q;
				werror( "undeclared initializer name %s",
					stab[idname].sname );
				q = block( FREE, NIL, NIL, INT, 0, INT );
				q->tn.rval = idname;
				defid( q, EXTERN );
				}
			    yyval.nodep=buildtree(2,NIL,NIL);
			    stab[yypvt[-0].intval].suse = -lineno;
			} break;
case 170:

{   yyval.nodep=bcon(0);
			    yyval.nodep->tn.lval = lastcon;
			    yyval.nodep->tn.rval = NONAME;
			    if( yypvt[-0].intval ) yyval.nodep->fn.csiz = yyval.nodep->in.type = ctype(LONG);
			    } break;
case 171:

{   yyval.nodep=buildtree(5,NIL,NIL);
			    yyval.nodep->fpn.dval = dcon;
			    } break;
case 172:

{  yyval.nodep = getstr(); } break;
case 173:

{  yyval.nodep=yypvt[-1].nodep; } break;
case 174:

{
			yyva