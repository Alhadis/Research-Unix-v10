# define SEOS 1
# define SCOMMENT 2
# define SLABEL 3
# define SUNKNOWN 4
# define SHOLLERITH 5
# define SICON 6
# define SRCON 7
# define SDCON 8
# define SBITCON 9
# define SOCTCON 10
# define SHEXCON 11
# define STRUE 12
# define SFALSE 13
# define SNAME 14
# define SNAMEEQ 15
# define SFIELD 16
# define SSCALE 17
# define SINCLUDE 18
# define SLET 19
# define SASSIGN 20
# define SAUTOMATIC 21
# define SBACKSPACE 22
# define SBLOCK 23
# define SCALL 24
# define SCHARACTER 25
# define SCLOSE 26
# define SCOMMON 27
# define SCOMPLEX 28
# define SCONTINUE 29
# define SDATA 30
# define SDCOMPLEX 31
# define SDIMENSION 32
# define SDO 33
# define SDOUBLE 34
# define SELSE 35
# define SELSEIF 36
# define SEND 37
# define SENDFILE 38
# define SENDIF 39
# define SENTRY 40
# define SEQUIV 41
# define SEXTERNAL 42
# define SFORMAT 43
# define SFUNCTION 44
# define SGOTO 45
# define SASGOTO 46
# define SCOMPGOTO 47
# define SARITHIF 48
# define SLOGIF 49
# define SIMPLICIT 50
# define SINQUIRE 51
# define SINTEGER 52
# define SINTRINSIC 53
# define SLOGICAL 54
# define SNAMELIST 55
# define SOPEN 56
# define SPARAM 57
# define SPAUSE 58
# define SPRINT 59
# define SPROGRAM 60
# define SPUNCH 61
# define SREAD 62
# define SREAL 63
# define SRETURN 64
# define SREWIND 65
# define SSAVE 66
# define SSTATIC 67
# define SSTOP 68
# define SSUBROUTINE 69
# define STHEN 70
# define STO 71
# define SUNDEFINED 72
# define SWRITE 73
# define SLPAR 74
# define SRPAR 75
# define SEQUALS 76
# define SCOLON 77
# define SCOMMA 78
# define SCURRENCY 79
# define SPLUS 80
# define SMINUS 81
# define SSTAR 82
# define SSLASH 83
# define SPOWER 84
# define SCONCAT 85
# define SAND 86
# define SOR 87
# define SNEQV 88
# define SEQV 89
# define SNOT 90
# define SEQ 91
# define SLT 92
# define SGT 93
# define SLE 94
# define SGE 95
# define SNE 96

# line 98 "gram.in"
#	include "defs"

#ifdef SDB
#	include <a.out.h>
char *stabline();
#	ifdef UCBVAXASM
		char *stabdline();
#	endif

#	ifndef N_SO
#		include <stab.h>
#	endif
#endif

static int nstars;
static int ndim;
static int vartype;
static ftnint varleng;
static struct { expptr lb, ub; } dims[MAXDIM+1];
static struct Labelblock *labarray[MAXLABLIST];
static int lastwasbranch = NO;
static int thiswasbranch = NO;
static int hadcomma;
extern ftnint yystno;
extern flag intonly;
static chainp datastack;
extern long laststfcn, thisstno;

ftnint convci();
double convcd();
Addrp nextdata();
expptr mklogcon(), mkaddcon(), mkrealcon(), mkstrcon(), mkbitcon();
expptr mkcxcon();
struct Listblock *mklist();
struct Listblock *mklist();
struct Impldoblock *mkiodo();
struct Extsym *comblock();

 static void
pop_datastack() {
	chainp d0 = datastack;
	if (d0->datap && !(bugwarn & 2)) {
		if (bugwarn)
			warnb("old f77 incorrectly parsed this data statement");
		curdtp = (chainp)d0->datap;
		}
	datastack = d0->nextp;
	d0->nextp = 0;
	frchain(&d0);
	}




# line 155 "gram.in"
typedef union 	{
	int ival;
	char *charpval;
	chainp chval;
	tagptr tagval;
	expptr expval;
	struct Labelblock *labval;
	struct Nameblock *namval;
	struct Eqvchain *eqvval;
	struct Extsym *extval;
	} YYSTYPE;
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 20,
	1, 31,
	-2, 214,
-1, 24,
	1, 35,
	-2, 214,
-1, 148,
	1, 230,
	-2, 178,
-1, 168,
	1, 251,
	78, 251,
	-2, 178,
-1, 215,
	77, 164,
	-2, 130,
-1, 237,
	74, 214,
	-2, 211,
-1, 263,
	1, 272,
	-2, 134,
-1, 267,
	1, 281,
	78, 281,
	-2, 136,
-1, 312,
	77, 165,
	-2, 132,
-1, 342,
	1, 253,
	14, 253,
	74, 253,
	78, 253,
	-2, 179,
-1, 403,
	91, 0,
	92, 0,
	93, 0,
	94, 0,
	95, 0,
	96, 0,
	-2, 144,
-1, 420,
	1, 275,
	78, 275,
	-2, 134,
-1, 422,
	1, 277,
	78, 277,
	-2, 134,
-1, 424,
	1, 279,
	78, 279,
	-2, 134,
-1, 461,
	78, 275,
	-2, 134,
	};
# define YYNPROD 286
# define YYLAST 1245
short yyact[]={

 229, 266, 492, 491, 224, 490, 389, 340, 292, 434,
 282, 382, 244, 126, 341, 381, 221, 226, 310, 291,
  98, 285, 214, 301, 265, 114, 433, 270, 228, 256,
   5, 184, 262, 196, 116,  17, 178, 459, 194, 177,
 119, 104, 258, 320, 154, 109, 192, 318, 319, 320,
 252, 203, 188, 466, 299, 100, 162, 163, 318, 319,
 320, 326, 325, 324, 323, 322, 102, 327, 329, 328,
 331, 330, 332, 297, 118, 155, 117, 155, 128, 129,
 130, 131, 112, 133, 162, 163, 318, 319, 320, 326,
 325, 324, 323, 322, 507, 327, 329, 328, 331, 330,
 332, 411, 102, 153, 102, 153, 162, 163, 318, 319,
 320, 326, 325, 324, 323, 322, 127, 327, 329, 328,
 331, 330, 332, 162, 163, 250, 251, 252, 253, 443,
  93, 489, 180, 181, 179, 118, 488, 117, 510, 187,
  97, 222, 232, 232, 394,  94,  95,  96, 250, 251,
 252, 479, 155, 509, 413, 249, 155, 378, 235, 231,
 233, 441, 377, 207, 442, 280, 155, 212, 482, 477,
 284, 211, 284,  99,  99, 263, 263, 155, 210, 205,
 153, 206, 102, 431, 153, 444, 432, 414, 267, 267,
 413, 440, 386, 428, 153, 387, 240, 427, 264, 426,
 254, 271, 272, 273, 269, 153, 304, 305, 287, 355,
 255, 288, 276, 277, 419, 308, 410, 312, 303, 278,
 314, 274, 375, 317, 279, 376, 298, 334, 182, 395,
 335, 336, 337, 317, 296, 361, 338, 369, 333, 360,
 370, 249, 260, 359, 155, 343, 257, 261, 344, 346,
 155, 155, 155, 155, 155, 249, 249, 200, 339, 242,
 307, 162, 163, 318, 319, 320, 326, 317, 315, 171,
 334, 316, 153, 362, 110, 249, 108, 107, 153, 153,
 153, 153, 153,   4, 106, 105, 267, 356, 345, 103,
 357, 358, 388, 364, 349, 350, 351, 352, 353, 371,
 213, 368, 365, 366, 502, 317, 317, 147, 334, 317,
 367, 335, 391, 317, 379, 317, 485, 484, 222, 399,
 400, 401, 402, 403, 404, 405, 406, 407, 408, 215,
 451, 482, 483, 398, 148, 317, 168, 317, 317, 317,
 374, 478, 481, 476, 135, 155, 249, 317, 449, 393,
 249, 249, 249, 249, 249, 412, 286, 415, 162, 163,
 318, 319, 320, 101, 420, 422, 424, 162, 163, 250,
 251, 252, 115, 153, 185, 435, 295, 267, 267, 267,
 238, 430, 429, 164, 166, 170, 234, 439, 218, 416,
 446, 215, 193, 204, 202, 165, 232, 453, 418, 139,
 317, 317, 317, 317, 317, 317, 317, 317, 317, 317,
 447, 445, 454, 452, 313, 455, 450, 249, 448, 102,
 456, 102, 102, 383, 201, 227, 236, 217, 457,  91,
  30, 461, 422, 424, 435, 458, 317,   6, 101, 101,
 101, 101, 241, 183, 267, 267, 267, 317, 186, 439,
 189, 190, 191, 232, 317, 471, 475, 199, 470, 465,
 480,  80, 418, 121, 462, 463, 464, 363, 487, 493,
 134,  79,  78, 189, 208, 209, 486,  77, 469, 294,
 167, 317, 260,  76, 505, 473, 494,  75, 317, 237,
 493, 239, 503,  74,  59, 506, 508, 497,  48,  47,
 152,  44, 152, 335,  32, 111, 317, 494, 493, 198,
 512, 511, 514, 380, 197, 317, 373, 372, 497, 468,
 467, 438, 497, 195, 513, 494, 132, 101, 300, 113,
 283, 158, 159, 160, 161,  26, 497,  25, 156, 157,
 102, 158, 159, 160, 161, 115,  24, 302, 156, 157,
 102,  23, 189,  22, 158, 159, 160, 161, 421, 423,
 425, 156, 157, 102,  21, 158, 159, 160, 161, 501,
 500, 499, 156, 157, 102,   9,   8, 152,   7,   2,
 289, 152, 162, 163, 318, 319, 320, 326, 325, 324,
  20, 152, 259, 327, 329, 328, 331, 330, 332, 495,
 225,  50, 152, 504, 281, 220, 162, 163, 347, 311,
 348, 309, 384,  90, 247,  52, 230, 321,  19, 290,
  54, 293,  36, 225, 219, 421, 423, 425, 216, 162,
 163, 223, 189,   3, 498,   1,   0,   0,   0, 230,
 162, 163, 385, 409,   0,   0,   0,   0, 162, 163,
 318, 319, 320, 326, 325, 324, 323, 322, 396, 327,
 329, 328, 331, 330, 332,   0,   0,   0,   0, 152,
   0,   0,   0,   0,   0, 152, 152, 152, 152, 152,
   0,   0,   0, 259, 397,   0, 259, 259,   0, 162,
 163, 318, 319, 320, 326, 325, 324, 323, 322,   0,
 327, 329, 328, 331, 330, 332,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 283,   0, 390,   0,   0,
 293,   0, 162, 163, 318, 319, 320, 326, 325, 324,
 323, 322, 302, 327, 329, 328, 331, 330, 332, 162,
 163, 318, 319, 320, 326, 325,   0,   0,   0,   0,
 327, 329, 328, 331, 330, 332,   0,   0,   0,   0,
 158, 159, 160, 161, 460,   0,   0, 156, 157, 102,
 152, 460, 460, 460,   0, 158, 159, 160, 161,   0,
   0, 259, 156, 157, 102, 460,   0,   0,   0, 472,
   0,   0, 474,   0, 162, 163, 318, 319, 320, 326,
   0,   0, 437,   0, 293, 327, 329, 328, 331, 330,
 332, 417,   0, 496,   0,   0, 162, 163, 250, 251,
 252, 253,   0,   0,   0,   0,   0,   0,   0, 151,
   0,   0,   0,   0, 496, 162, 163, 149, 496, 150,
   0,   0,   0,   0, 225, 259,   0,   0,   0,   0,
 162, 163, 496,  12,   0, 158, 159, 160, 161,   0,
 230,   0, 156, 157, 102,   0, 293,  10,  55,  45,
  72,  84,  14,  60,  69,  89,  37,  65,  46,  41,
  67,  71,  31,  66,  34,  33,  11,  86,  35,  18,
  40,  38,  28,  16,  56,  57,  58,  49,  53,  42,
  87,  63,  39,  68,  43,  88,  29,  61,  83,  13,
  92,  81,  64,  51,  85,  27,  73,  62,  15,   0,
   0,  70,  82,   0, 225,   0, 158, 159, 160, 161,
 162, 163, 436, 156, 157, 102,   0,   0,   0,   0,
 230,   0, 120, 354, 123, 124, 125,   0, 162, 163,
 250, 251, 252, 253,   0,   0, 136, 137,   0,   0,
 138,   0, 140, 141, 142,   0,   0, 143, 144, 145,
   0, 146, 158, 159, 160, 161,   0, 342,   0, 156,
 157, 102, 162, 163, 250, 251, 252, 253,   0,   0,
 172, 173, 174, 175, 176, 225,   0, 158, 159, 160,
 161, 162, 163, 223, 156, 157, 102,   0,   0,   0,
   0, 230, 158, 159, 160, 161,   0,   0,   0, 156,
 157, 102,   0, 158, 159, 160, 161,   0,   0,   0,
 156, 157, 102, 248,   0,   0,   0,   0,   0,   0,
   0, 225,   0, 158, 159, 160, 161, 162, 163, 392,
 156, 157, 102,   0,   0,   0,   0, 230,   0,   0,
   0,   0,   0,   0,   0,   0, 268,   0,   0,   0,
   0,   0, 162, 163,   0,   0,   0,   0,   0,   0,
   0, 306, 230,   0,   0,   0,   0, 162, 163,   0,
   0,   0, 243,   0,   0,   0,   0, 230, 162, 163,
 245,   0, 246,  55,  45,   0,  84,   0,  60,   0,
  89,   0, 243,  46,   0,   0,   0,   0, 162, 163,
 275,   0,  86,   0,   0,   0,   0,   0,   0,  56,
  57,  58,  49,   0,   0,  87,   0,   0,   0,   0,
  88,   0,  61,  83,   0,   0,  81,   0,  51,  85,
   0,   0,  62,   0, 122,   0,   0,  82, 158, 159,
 160, 161,   0,   0,   0, 156, 157, 102, 158, 159,
 160, 161,   0,   0,   0, 156, 157, 102,  72,   0,
   0,   0,  69,   0,   0,  65,   0,   0,  67,  71,
   0,  66,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  63,
   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,
  64,   0,   0,   0,  73,   0,   0, 151,   0,  70,
   0,   0,   0, 162, 163, 169,   0, 243,   0,   0,
   0,   0,   0, 162, 163 };
short yypact[]={

-1000,  27, 436, 849,-1000,-1000,-1000,-1000,-1000,-1000,
 424,-1000,-1000,-1000,-1000,-1000,-1000,  96, 407, 211,
 207, 206, 199, 198,  95, 196,  -1,  52,-1000,-1000,
-1000,-1000,1084,-1000,-1000,-1000,  34,-1000,-1000,-1000,
-1000,-1000,-1000,-1000, 407,-1000,-1000,-1000,-1000,-1000,
 325,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000, 755, 321,1153, 321, 191,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000, 407, 407, 407, 407,-1000, 407,-1000,
 300,-1000,-1000, 407,  -9, 407, 407, 407, 318,-1000,
-1000,-1000, 407, 179,-1000,-1000,-1000,-1000, 410, 320,
-1000,-1000,-1000, 319,-1000,-1000,-1000,-1000,  52, 407,
 407, 318,-1000,-1000, 224, 317, 421,-1000, 314, 549,
 770, 770, 312, 420, 407, 306, 407,-1000,-1000,-1000,
-1000,1018,-1000,-1000,  43,1163,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,1018, 168, 169,-1000,-1000,
 992, 992,-1000,-1000,-1000,-1000,1038,-1000,-1000,-1000,
 300, 300, 407,-1000,-1000,  90, 282,  95,-1000, 282,
-1000,-1000,-1000, 407,-1000, 405,-1000, 302,1157, -10,
  52, -29, 407, 420, 770,1007,-1000, 407,-1000,-1000,
-1000,-1000,-1000, 770,-1000, 770, 343,-1000, 770,-1000,
 193,-1000,   4, 420,-1000, 770,-1000,-1000,-1000, 770,
 770,-1000,   4,-1000, 770,-1000,-1000,  95, 420,-1000,
 902, 170,-1000,1163,-1000,-1000,-1000, 526,-1000,1163,
1163,1163,1163,1163, -34, 868, 131, 408,-1000,-1000,
 408, 408,-1000, 165, 161, 157,   4,-1000, 992,-1000,
-1000,-1000,-1000,-1000,  43,-1000,-1000,-1000, 300, 282,
-1000, 162,-1000,-1000,-1000,  34,-1000, 407,-1000, 147,
-1000,  79,-1000,-1000, 405, 409,-1000, 407,-1000,-1000,
 117,-1000, 216,-1000, 642,   4, 967,-1000,   4, 274,
  67, 151,   4, 407, 609,-1000, 921, 770, 770, 770,
 770, 770, 770, 770, 770, 770, 770,-1000,-1000,-1000,
-1000,-1000,-1000,-1000, 568, 138, -41, 714,  26, 281,
 112,-1000,-1000,-1000,1018, 736,   4,-1000,-1000,  66,
 -34, -34, -34, 287,-1000, 408, 131, 136, 131, 992,
 992, 992, 121, 119, 115,-1000,-1000,-1000,  34,-1000,
  88,-1000, 108, 850,-1000,-1000, 407,-1000, 405, 113,
  86,-1000,  48,-1000, 107,-1000,-1000, 407, 770,  95,
 348,-1000, 273, 256, 770, 770,-1000, 420,-1000, -35,
 -41, -41, -41, 181, 502, 502, 659, 714, 278,-1000,
 770,-1000, 420, 420,  95,-1000,  43,-1000,-1000, 408,
-1000,-1000,-1000,-1000,-1000,-1000, 992, 992, 992,-1000,
-1000,-1000, 850,-1000,-1000, -24,-1000,-1000,-1000,-1000,
 405,-1000, 409, 409, 407,-1000,   4, 407,-1000,-1000,
-1000, 770, 268,   4,  91, 266,  76,-1000, 770, 267,
 255, 266, 257, 242, 241,-1000, 850,  53, 560, 229,
-1000,-1000,-1000,-1000,  92,-1000,-1000, 420,-1000,-1000,
   4,-1000, 770,-1000,-1000,-1000,-1000,   4,-1000, 560,
-1000,-1000,  12, 536,-1000,-1000,-1000,-1000, 770,-1000,
-1000,-1000,-1000,  75,  60,   4,-1000, 560,-1000, 420,
 770,-1000,-1000,-1000,   4 };
short yypgo[]={

   0, 635, 633,  14, 628, 198,  42,  13,  35, 622,
 620, 618,  11,   0, 617, 615, 614, 613,   8,  19,
 612, 611, 609,  37, 605,  31, 604, 603,  29,  32,
  24,  22, 344,  10, 601,  52, 425,   1,   4,  18,
  44, 259,   9,   2,   3,  16,  17,  28, 599, 590,
  34,  36,  39, 580, 579, 578, 576, 575, 910, 130,
 564, 553, 551, 546, 537, 535, 529,  40, 528,  20,
  21,  51,   6,  46, 526,  38, 523, 521, 520, 519,
   5,  25,  23, 517, 516,  26,   7,  33, 514, 513,
 509,  15, 505, 430, 504, 501, 499, 498, 494, 493,
 307, 487, 483, 480, 477, 472, 471,  27, 461, 442,
  12 };
short yyr1[]={

   0,   1,   1,  54,  54,  54,  54,  54,  54,  54,
   2,  55,  55,  55,  55,  55,  55,  59,  51,  32,
  52,  52,  25,  25,  25,  26,  26,  33,  33,  17,
  56,  56,  56,  56,  56,  56,  56,  56,  56,  56,
  56,  56,  11,  11,   8,   9,  10,  10,  10,  10,
  10,  10,  10,  10,  10,  10,  10,   7,   7,   7,
  49,  49,  49,  49,  50,  50,  60,  60,  61,  61,
  62,  62,  73,  53,  53,  63,  63,  74,  75,  76,
  77,  79,  78,  78,  80,  80,  44,  44,  44,  44,
  66,  66,  81,  81,  68,  68,  82,  35,  18,  18,
  19,  19,  70,  70,  84,  83,  83,  85,  85,  42,
  42,  86,  86,   3,  64,  64,  87,  90,  88,  89,
  89,  91,  91,  12,  65,  65,  92,  20,  20,  67,
  21,  21,  22,  22,  37,  37,  37,  38,  38,  38,
  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,
  38,  13,  13,  14,  14,  14,  14,  14,  14,  36,
  36,  36,  36,  31,  39,  39,  43,  43,  47,  47,
  47,  47,  47,  47,  46,  48,  48,  48,  40,  40,
  41,  41,  41,  41,  41,  41,  41,  41,  57,  57,
  57,  57,  57,  57,  57,  94,  23,  93,  93,  93,
  93,  93,  93,  93,  93,  93,  93,  93,   4,  95,
  96,  96,  96,  96,  69,  69,  34,  24,  24,  45,
  45,  15,  15,  27,  27,  58,  71,  72,  97,  98,
  98,  98,  98,  98,  98,  98,  98,  98,  98,  98,
  98,  98,  98,  99, 106, 106, 106, 101, 108, 108,
 108, 103, 103, 100, 100, 109, 109, 110, 110, 110,
 110, 110, 110,  16, 102, 104, 105, 105,  28,  28,
   6,   6,  29,  29,  29,  30,  30,  30,  30,  30,
  30,   5,   5,   5,   5, 107 };
short yyr2[]={

   0,   0,   3,   2,   2,   2,   3,   3,   2,   1,
   1,   3,   3,   4,   4,   5,   3,   0,   1,   1,
   0,   1,   0,   2,   3,   1,   3,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,
   1,   5,   6,   5,   2,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   1,   1,   1,   0,   4,   6,
   3,   4,   5,   3,   1,   3,   3,   3,   3,   3,
   3,   3,   3,   1,   3,   3,   3,   0,   6,   0,
   0,   0,   2,   3,   1,   3,   1,   2,   1,   1,
   1,   3,   1,   1,   1,   3,   3,   2,   1,   5,
   1,   3,   0,   3,   0,   2,   3,   1,   3,   1,
   1,   1,   3,   1,   3,   3,   4,   0,   2,   1,
   3,   1,   3,   1,   1,   2,   4,   1,   3,   0,
   0,   1,   1,   3,   1,   3,   1,   1,   1,   3,
   3,   3,   3,   2,   3,   3,   3,   3,   3,   2,
   3,   1,   1,   1,   1,   1,   1,   1,   1,   1,
   2,   4,   5,   5,   0,   1,   1,   1,   1,   1,
   1,   1,   1,   1,   5,   1,   1,   1,   1,   3,
   1,   1,   3,   3,   3,   3,   2,   3,   1,   7,
   2,   2,   6,   2,   2,   5,   3,   4,   5,   2,
   1,   1,  10,   1,   3,   4,   3,   3,   1,   1,
   3,   3,   7,   7,   0,   1,   3,   1,   3,   1,
   2,   1,   1,   1,   3,   0,   0,   0,   1,   2,
   2,   2,   2,   2,   2,   2,   3,   4,   4,   2,
   3,   1,   3,   3,   1,   1,   1,   3,   1,   1,
   1,   1,   1,   3,   3,   1,   3,   1,   1,   1,
   2,   2,   2,   1,   3,   3,   4,   4,   1,   3,
   1,   5,   1,   1,   1,   3,   3,   3,   3,   3,
   3,   1,   5,   5,   5,   0 };
short yychk[]={

-1000,  -1, -54,  -2, 256,   3,   1, -55, -56, -57,
  18,  37,   4,  60,  23,  69,  44,  -8,  40, -11,
 -49, -60, -61, -62, -63, -64, -65,  66,  43,  57,
 -93,  33, -94,  36,  35,  39,  -9,  27,  42,  53,
  41,  30,  50,  55, -95,  20,  29, -96, -97,  48,
 -34,  64, -15,  49, -10,  19,  45,  46,  47, -98,
  24,  58,  68,  52,  63,  28,  34,  31,  54,  25,
  72,  32,  21,  67, -99,-101,-102,-104,-105,-106,
-108,  62,  73,  59,  22,  65,  38,  51,  56,  26,
 -17,   5, -58, -59, -59, -59, -59,  44, -69,  78,
 -51, -32,  14,  78, -69,  78,  78,  78,  78, -69,
  78, -92,  83, -66, -81, -32, -50,  85,  83, -67,
 -58, -93,  70, -58, -58, -58,  -7,  82, -67, -67,
 -67, -67, -74, -67, -36, -32, -58, -58, -58,  74,
 -58, -58, -58, -58, -58, -58, -58,-100, -41,  82,
  84,  74, -36, -47, -40, -13,  12,  13,   5,   6,
   7,   8,  80,  81,-100,  74,-100,-103, -41,  82,
-100,  78, -58, -58, -58, -58, -58, -52, -51, -52,
 -51, -51, -59, -32, -25,  74, -32, -50, -35, -32,
 -32, -32, -73,  74, -75, -76, -87, -88, -90, -32,
  78,  14,  74, -71,  74, -71, -35, -50, -32, -32,
 -73, -75, -87,  76, -31,  74,  -4,   6,  74,  75,
 -24, -45, -37,  82, -38,  74, -46, -36, -47, -13,
  90, -39, -37, -39,  74,  -3,   6, -32,  74, -32,
 -40,-109, -41,  74,-110,  82,  84, -16,  15, -13,
  82,  83,  84,  85, -40, -40, -28,  78,  -6, -36,
  74,  78, -29, -38,  -5, -30, -37, -46,  74, -29,
-107,-107,-107,-107, -40,  82, -25, -25, -51, -67,
  75, -26, -33, -32,  82, -70,  74, -69, -70, -53,
 -36, -19, -18, -36,  74,  74,  -8,  83, -81,  83,
 -68, -82, -32,  -3, -37, -37,  74, -35, -37, -21,
 -39, -22, -37,  71, -37,  75,  78, -13,  82,  83,
  84, -14,  89,  88,  87,  86,  85,  91,  93,  92,
  95,  94,  96,  -3, -37, -38, -37, -37, -37, -69,
 -86,  -3,  75,  75,  78, -40, -37,  82,  84, -40,
 -40, -40, -40, -40,  75,  78, -28, -28, -28,  78,
  78,  78, -38,  -5, -30,-107,-107, -25, -70,  75,
  78,  -7, -83, -84, -35,  75,  78,  83,  78, -19,
 -89, -91, -12,  14, -20, -32,  75,  78,  76, -72,
  75, -72,  82,  75,  77,  78, -32,  75, -45, -37,
 -37, -37, -37, -37, -37, -37, -37, -37, -37,  75,
  78,  75,  74,  78,  75,-110, -40,  75,  -6,  78,
 -38,  -5, -38,  -5, -38,  -5,  78,  78,  78,  -7,
 -33,  75,  78, -85, -42, -37,  82, -36, -77, -18,
  78,  75,  78,  81,  78, -82, -37, -69,  70,  75,
 -31,  74, -39, -37,  -3, -38, -86,  -3, -69, -23,
 -32, -38, -23, -23, -23, -85,  77, -78, -79, -23,
 -91, -12, -32, -23, -32, -72,  75,  78,  75,  75,
 -37,  75,  76,  75,  75,  75, -42, -37,  83,  78,
 -80, -44, -43, -13, -46, -48, -32, -47,  74,  11,
  10,   9,  75,  -3, -27, -37, -80,  82, -43,  78,
  78, -44, -43,  -3, -37 };
short yydef[]={

   1,  -2,   0,   0,   9,  10,   2,   3,   4,   5,
   0, 225,   8,  17,  17,  17,  17, 214,   0,  30,
  -2,  32,  33,  34,  -2,  36,  37,  38,  40, 129,
 188, 225,   0, 225, 225, 225,  57, 129, 129, 129,
 129,  77, 129, 124,   0, 225, 225, 200, 201, 225,
 203, 225, 225, 225,  45, 209, 225, 225, 225, 228,
 225, 221, 222,  46,  47,  48,  49,  50,  51,  52,
  53,  54,  55,  56,   0,   0,   0,   0, 241, 225,
 225, 225, 225, 225, 244, 245, 246, 248, 249, 250,
   6,  29,   7,  20,  20,   0,   0,  17,   0, 215,
  22,  18,  19,   0,   0, 215,   0,   0,   0,  79,
 117, 125,   0,  39,  90,  92,  93,  64,   0,   0,
 226, 190, 191,   0, 193, 194,  44, 226,   0,   0,
   0,   0,  79, 117,   0, 159,   0, 199,   0,   0,
 164, 164,   0,   0,   0,   0,   0, 229,  -2, 231,
 232,   0, 180, 181,   0,   0, 168, 169, 170, 171,
 172, 173, 151, 152, 233,   0, 234, 235,  -2, 252,
 239,   0, 285, 285, 285, 285,   0,  11,  21,  12,
  22,  22,   0, 129,  16,   0, 102, 214,  63, 102,
  67,  69,  71,   0,  76,   0, 115,   0,   0,   0,
   0,   0,   0,   0,   0,   0,  60,   0,  66,  68,
  70,  75, 114,   0, 160,  -2,   0, 208,   0, 204,
   0, 217, 219,   0, 134,   0, 136, 137, 138,   0,
   0, 206, 165, 207,   0, 210, 113,  -2,   0, 216,
 257,   0, 178,   0, 255, 258, 259,   0, 263,   0,
   0,   0,   0,   0, 186, 257, 236,   0, 268, 270,
   0,   0, 240,  -2, 273, 274,   0,  -2,   0, 242,
 243, 247, 264, 265, 285, 285,  13,  14,  22, 102,
  23,   0,  25,  27,  28,  57, 104,   0,  97,   0,
  73,   0, 100,  98,   0,   0, 118,   0,  91,  65,
   0,  94,   0, 227,   0, 227,   0,  61, 197,   0,
   0, 131,  -2,   0,   0, 205,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 153, 154, 155,
 156, 157, 158, 220,   0, 134, 143, 149,   0,   0,
   0, 111,  -2, 254,   0,   0, 260, 261, 262, 182,
 183, 184, 185, 187, 253,   0, 238,   0, 237,   0,
   0,   0, 134,   0,   0, 266, 267,  15,  57,  24,
   0,  43,   0,   0,  62,  72,   0,  80,   0,   0,
   0, 119, 121, 123, 126, 127,  41,   0,   0, 214,
   0,  58,   0, 161, 164,   0, 198,   0, 218, 139,
 140, 141, 142,  -2, 145, 146, 147, 148, 150, 135,
   0, 195,   0,   0, 214, 256, 257, 179, 269,   0,
  -2, 276,  -2, 278,  -2, 280,   0,   0,   0,  42,
  26, 103,   0, 105, 107, 110, 109,  74,  81, 101,
   0, 116,   0,   0,   0,  95,  96,   0, 192, 227,
 162, 164,   0, 133,   0, 134,   0, 112,   0,   0,
 159,  -2,   0,   0,   0, 106,   0,   0,   0,   0,
 120, 122, 128, 189,   0,  59, 163,   0, 174, 212,
 213, 271,   0, 282, 283, 284, 108, 110,  78,   0,
  82,  84,  86,   0,  88,  89, 166, 167,   0, 175,
 176, 177,  99,   0, 196, 223,  83,   0,  87,   0,
   0,  85,  86, 202, 224 };
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
		
case 3:
# line 200 "gram.in"
{ lastwasbranch = NO; } break;
case 5:
# line 203 "gram.in"
{ /* forbid further statement function definitions... */
		  if (parstate == INDATA
				&& laststfcn != thisstno
				&& !(bugwarn & 2))
			parstate = INEXEC;
		  thisstno++;
		  if(yypvt[-1].labval && (yypvt[-1].labval->labelno==dorange))
			enddo(yypvt[-1].labval->labelno);
		  if(lastwasbranch && thislabel==NULL)
			warn("statement cannot be reached");
		  lastwasbranch = thiswasbranch;
		  thiswasbranch = NO;
		  if(yypvt[-1].labval)
			{
			if(yypvt[-1].labval->labtype == LABFORMAT)
				err("label already that of a format");
			else
				yypvt[-1].labval->labtype = LABEXEC;
			}
		} break;
case 6:
# line 224 "gram.in"
{ doinclude( yypvt[-0].charpval ); } break;
case 7:
# line 226 "gram.in"
{ lastwasbranch = NO;  endproc(); } break;
case 8:
# line 228 "gram.in"
{ execerr("unclassifiable statement", CNULL);  flline(); } break;
case 9:
# line 230 "gram.in"
{ flline();  needkwd = NO;  inioctl = NO; 
		  yyerrok; yyclearin; } break;
case 10:
# line 235 "gram.in"
{
#ifdef SDB
		if( sdbflag && parstate >= INDATA )
			{
#	ifdef UCBVAXASM
			p2pass( stabdline(N_SLINE, lineno) );
#	else
			char buff[10];
			sprintf(buff,"LL%d", ++dbglabel);
			p2pass( stabline(0, N_SLINE, lineno, buff) );
			p2pi("LL%d:\n", dbglabel);
#	endif
			}
#endif

		if(yystno != 0)
			{
			yyval.labval = thislabel =  mklabel(yystno);
			if( ! headerdone )
				puthead(CNULL, procclass);
			if(thislabel->labdefined)
				execerr("label %s already defined",
					convic(thislabel->stateno) );
			else	{
				if(thislabel->blklevel!=0 && thislabel->blklevel<blklevel
				    && thislabel->labtype!=LABFORMAT)
					warn1("there is a branch to label %s from outside block",
					      convic( (ftnint) (thislabel->stateno) ) );
				thislabel->blklevel = blklevel;
				thislabel->labdefined = YES;
				if(thislabel->labtype != LABFORMAT)
					putlabel(thislabel->labelno);
				}
			}
		else    yyval.labval = thislabel = NULL;
		} break;
case 11:
# line 274 "gram.in"
{startproc(yypvt[-0].extval, CLMAIN); } break;
case 12:
# line 276 "gram.in"
{ if(yypvt[-0].extval) NO66("named BLOCKDATA");
		  startproc(yypvt[-0].extval, CLBLOCK); } break;
case 13:
# line 279 "gram.in"
{ entrypt(CLPROC, TYSUBR, (ftnint) 0,  yypvt[-1].extval, yypvt[-0].chval); } break;
case 14:
# line 281 "gram.in"
{ entrypt(CLPROC, TYUNKNOWN, (ftnint) 0, yypvt[-1].extval, yypvt[-0].chval); } break;
case 15:
# line 283 "gram.in"
{ entrypt(CLPROC, yypvt[-4].ival, varleng, yypvt[-1].extval, yypvt[-0].chval); } break;
case 16:
# line 285 "gram.in"
{ if(parstate==OUTSIDE || procclass==CLMAIN
			|| procclass==CLBLOCK)
				execerr("misplaced entry statement", CNULL);
		  entrypt(CLENTRY, 0, (ftnint) 0, yypvt[-1].extval, yypvt[-0].chval);
		} break;
case 17:
# line 293 "gram.in"
{ newproc(); } break;
case 18:
# line 297 "gram.in"
{ yyval.extval = newentry(yypvt[-0].namval); } break;
case 19:
# line 301 "gram.in"
{ yyval.namval = mkname(toklen, token); } break;
case 20:
# line 304 "gram.in"
{ yyval.extval = NULL; } break;
case 22:
# line 309 "gram.in"
{ yyval.chval = 0; } break;
case 23:
# line 311 "gram.in"
{ NO66(" () argument list");
		  yyval.chval = 0; } break;
case 24:
# line 314 "gram.in"
{yyval.chval = yypvt[-1].chval; } break;
case 25:
# line 318 "gram.in"
{ yyval.chval = (yypvt[-0].namval ? mkchain(yypvt[-0].namval,CHNULL) : CHNULL ); } break;
case 26:
# line 320 "gram.in"
{ if(yypvt[-0].namval) yypvt[-2].chval = yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].namval,CHNULL)); } break;
case 27:
# line 324 "gram.in"
{ if(yypvt[-0].namval->vstg!=STGUNKNOWN && yypvt[-0].namval->vstg!=STGARG)
			dclerr("name declared as argument after use", yypvt[-0].namval);
		  yypvt[-0].namval->vstg = STGARG;
		} break;
case 28:
# line 329 "gram.in"
{ NO66("altenate return argument");
		  yyval.namval = 0;  substars = YES; } break;
case 29:
# line 336 "gram.in"
{
		char *s;
		s = copyn(toklen+1, token);
		s[toklen] = '\0';
		yyval.charpval = s;
		} break;
case 38:
# line 352 "gram.in"
{ NO66("SAVE statement");
		  saveall = YES; } break;
case 39:
# line 355 "gram.in"
{ NO66("SAVE statement"); } break;
case 40:
# line 357 "gram.in"
{ fmtstmt(thislabel); setfmt(thislabel); } break;
case 41:
# line 359 "gram.in"
{ NO66("PARAMETER statement"); } break;
case 42:
# line 363 "gram.in"
{ settype(yypvt[-3].namval, yypvt[-5].ival, yypvt[-0].ival);
		  if(ndim>0) setbound(yypvt[-3].namval,ndim,dims);
		} break;
case 43:
# line 367 "gram.in"
{ settype(yypvt[-2].namval, yypvt[-4].ival, yypvt[-0].ival);
		  if(ndim>0) setbound(yypvt[-2].namval,ndim,dims);
		} break;
case 44:
# line 373 "gram.in"
{ varleng = yypvt[-0].ival; } break;
case 45:
# line 377 "gram.in"
{ varleng = (yypvt[-0].ival<0 || yypvt[-0].ival==TYLONG ? 0 : typesize[yypvt[-0].ival]); } break;
case 46:
# line 380 "gram.in"
{ yyval.ival = TYLONG; } break;
case 47:
# line 381 "gram.in"
{ yyval.ival = TYREAL; } break;
case 48:
# line 382 "gram.in"
{ ++complex_seen; yyval.ival = TYCOMPLEX; } break;
case 49:
# line 383 "gram.in"
{ yyval.ival = TYDREAL; } break;
case 50:
# line 384 "gram.in"
{ ++dcomplex_seen; NOEXT("DOUBLE COMPLEX statement"); yyval.ival = TYDCOMPLEX; } break;
case 51:
# line 385 "gram.in"
{ yyval.ival = TYLOGICAL; } break;
case 52:
# line 386 "gram.in"
{ NO66("CHARACTER statement"); yyval.ival = TYCHAR; } break;
case 53:
# line 387 "gram.in"
{ yyval.ival = TYUNKNOWN; } break;
case 54:
# line 388 "gram.in"
{ yyval.ival = TYUNKNOWN; } break;
case 55:
# line 389 "gram.in"
{ NOEXT("AUTOMATIC statement"); yyval.ival = - STGAUTO; } break;
case 56:
# line 390 "gram.in"
{ NOEXT("STATIC statement"); yyval.ival = - STGBSS; } break;
case 57:
# line 394 "gram.in"
{ yyval.ival = varleng; } break;
case 58:
# line 396 "gram.in"
{
		expptr p;
		p = yypvt[-1].expval;
		NO66("length specification *n");
		if( ! ISICON(p) || p->constblock.Const.ci<0 )
			{
			yyval.ival = 0;
			dclerr("length must be a positive integer constant",
				PNULL);
			}
		else yyval.ival = p->constblock.Const.ci;
		} break;
case 59:
# line 409 "gram.in"
{ NO66("length specification *(*)"); yyval.ival = -1; } break;
case 60:
# line 413 "gram.in"
{ incomm( yyval.extval = comblock(0, CNULL) , yypvt[-0].namval ); } break;
case 61:
# line 415 "gram.in"
{ yyval.extval = yypvt[-1].extval;  incomm(yypvt[-1].extval, yypvt[-0].namval); } break;
case 62:
# line 417 "gram.in"
{ yyval.extval = yypvt[-2].extval;  incomm(yypvt[-2].extval, yypvt[-0].namval); } break;
case 63:
# line 419 "gram.in"
{ incomm(yypvt[-2].extval, yypvt[-0].namval); } break;
case 64:
# line 423 "gram.in"
{ yyval.extval = comblock(0, CNULL); } break;
case 65:
# line 425 "gram.in"
{ yyval.extval = comblock(toklen, token); } break;
case 66:
# line 429 "gram.in"
{ setext(yypvt[-0].namval); } break;
case 67:
# line 431 "gram.in"
{ setext(yypvt[-0].namval); } break;
case 68:
# line 435 "gram.in"
{ NO66("INTRINSIC statement"); setintr(yypvt[-0].namval); } break;
case 69:
# line 437 "gram.in"
{ setintr(yypvt[-0].namval); } break;
case 72:
# line 445 "gram.in"
{
		struct Equivblock *p;
		if(nequiv >= maxequiv)
			many("equivalences", 'q', maxequiv);
		p  =  & eqvclass[nequiv++];
		p->eqvinit = NO;
		p->eqvbottom = 0;
		p->eqvtop = 0;
		p->equivs = yypvt[-1].eqvval;
		} break;
case 73:
# line 458 "gram.in"
{ yyval.eqvval=ALLOC(Eqvchain);
		  yyval.eqvval->eqvitem.eqvlhs = (struct Primblock *)yypvt[-0].expval;
		} break;
case 74:
# line 462 "gram.in"
{ yyval.eqvval=ALLOC(Eqvchain);
		  yyval.eqvval->eqvitem.eqvlhs = (struct Primblock *) yypvt[-0].expval;
		  yyval.eqvval->eqvnextp = yypvt[-2].eqvval;
		} break;
case 77:
# line 473 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc(PNULL, CLMAIN);
			}
		  if(parstate < INDATA)
			{
			enddcl();
			parstate = INDATA;
			}
		} break;
case 78:
# line 487 "gram.in"
{ ftnint junk;
		  if(nextdata(&junk,&junk) != NULL)
			err("too few initializers");
		  frdata(yypvt[-4].chval);
		  frrpl();
		} break;
case 79:
# line 495 "gram.in"
{ frchain(&datastack); curdtp = 0; } break;
case 80:
# line 497 "gram.in"
{ pop_datastack(); } break;
case 81:
# line 499 "gram.in"
{ toomanyinit = NO; } break;
case 84:
# line 504 "gram.in"
{ dataval(PNULL, yypvt[-0].expval); } break;
case 85:
# line 506 "gram.in"
{ dataval(yypvt[-2].expval, yypvt[-0].expval); } break;
case 87:
# line 511 "gram.in"
{ if( yypvt[-1].ival==OPMINUS && ISCONST(yypvt[-0].expval) )
			consnegop(yypvt[-0].expval);
		  yyval.expval = yypvt[-0].expval;
		} break;
case 92:
# line 524 "gram.in"
{ int k;
		  yypvt[-0].namval->vsave = YES;
		  k = yypvt[-0].namval->vstg;
		if( ! ONEOF(k, M(STGUNKNOWN)|M(STGBSS)|M(STGINIT)) )
			dclerr("can only save static variables", yypvt[-0].namval);
		} break;
case 93:
# line 531 "gram.in"
{ yypvt[-0].extval->extsave = 1; } break;
case 96:
# line 539 "gram.in"
{ if(yypvt[-2].namval->vclass == CLUNKNOWN)
			make_param(yypvt[-2].namval, yypvt[-0].expval);
/* was...
			{ $1->vclass = CLPARAM;
			  ( (struct Paramblock *) ($1) )->paramval = $3;
			}
 */
		  else dclerr("cannot make %s parameter", yypvt[-2].namval);
		} break;
case 97:
# line 551 "gram.in"
{ if(ndim>0) setbound(yypvt[-1].namval, ndim, dims); } break;
case 98:
# line 555 "gram.in"
{ Namep np;
		  np = ( (struct Primblock *) yypvt[-0].expval) -> namep;
		  vardcl(np);
		  if(np->vstg == STGCOMMON)
			extsymtab[np->vardesc.varno].extinit = YES;
		  else if(np->vstg==STGEQUIV)
			eqvclass[np->vardesc.varno].eqvinit = YES;
		  else if(np->vstg!=STGINIT && np->vstg!=STGBSS)
			dclerr("inconsistent storage classes", np);
		  yyval.chval = mkchain(yypvt[-0].expval, CHNULL);
		} break;
case 99:
# line 567 "gram.in"
{ chainp p; struct Impldoblock *q;
		pop_datastack();
		q = ALLOC(Impldoblock);
		q->tag = TIMPLDO;
		(q->varnp = (Namep) (yypvt[-1].chval->datap))->vimpldovar = !(bugwarn & 2);
		p = yypvt[-1].chval->nextp;
		if(p)  { q->implb = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impub = (expptr)(p->datap); p = p->nextp; }
		if(p)  { q->impstep = (expptr)(p->datap); p = p->nextp; }
		frchain( & (yypvt[-1].chval) );
		yyval.chval = mkchain(q, CHNULL);
		q->datalist = hookup(yypvt[-3].chval, yyval.chval);
		} break;
case 100:
# line 583 "gram.in"
{ if (!datastack)
			curdtp = 0;
		  datastack = mkchain((tagptr)curdtp, datastack);
		  curdtp = yypvt[-0].chval; curdtelt = 0;
		  } break;
case 101:
# line 589 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, yypvt[-0].chval); } break;
case 102:
# line 593 "gram.in"
{ ndim = 0; } break;
case 104:
# line 597 "gram.in"
{ ndim = 0; } break;
case 107:
# line 602 "gram.in"
{ if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ dims[ndim].lb = 0;
			  dims[ndim].ub = yypvt[-0].expval;
			}
		  ++ndim;
		} break;
case 108:
# line 611 "gram.in"
{ if(ndim == maxdim)
			err("too many dimensions");
		  else if(ndim < maxdim)
			{ dims[ndim].lb = yypvt[-2].expval;
			  dims[ndim].ub = yypvt[-0].expval;
			}
		  ++ndim;
		} break;
case 109:
# line 622 "gram.in"
{ yyval.expval = 0; } break;
case 111:
# line 627 "gram.in"
{ nstars = 1; labarray[0] = yypvt[-0].labval; } break;
case 112:
# line 629 "gram.in"
{ if(nstars < MAXLABLIST)  labarray[nstars++] = yypvt[-0].labval; } break;
case 113:
# line 633 "gram.in"
{ yyval.labval = execlab( convci(toklen, token) ); } break;
case 114:
# line 637 "gram.in"
{ NO66("IMPLICIT statement"); } break;
case 117:
# line 644 "gram.in"
{ needkwd = 1; } break;
case 118:
# line 645 "gram.in"
{ vartype = yypvt[-0].ival; } break;
case 121:
# line 653 "gram.in"
{ setimpl(vartype, varleng, yypvt[-0].ival, yypvt[-0].ival); } break;
case 122:
# line 655 "gram.in"
{ setimpl(vartype, varleng, yypvt[-2].ival, yypvt[-0].ival); } break;
case 123:
# line 659 "gram.in"
{ if(toklen!=1 || token[0]<'a' || token[0]>'z')
			{
			dclerr("implicit item must be single letter", PNULL);
			yyval.ival = 0;
			}
		  else yyval.ival = token[0];
		} break;
case 126:
# line 673 "gram.in"
{
		if(yypvt[-2].namval->vclass == CLUNKNOWN)
			{
			yypvt[-2].namval->vclass = CLNAMELIST;
			yypvt[-2].namval->vtype = TYINT;
			yypvt[-2].namval->vstg = STGINIT;
			yypvt[-2].namval->varxptr.namelist = yypvt[-0].chval;
			yypvt[-2].namval->vardesc.varno = ++lastvarno;
			}
		else dclerr("cannot be a namelist name", yypvt[-2].namval);
		} break;
case 127:
# line 687 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].namval, CHNULL); } break;
case 128:
# line 689 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].namval, CHNULL)); } break;
case 129:
# line 693 "gram.in"
{ switch(parstate)	
			{
			case OUTSIDE:	newproc();
					startproc(PNULL, CLMAIN);
			case INSIDE:	parstate = INDCL;
			case INDCL:	break;

			default:
				dclerr("declaration among executables", PNULL);
			}
		} break;
case 130:
# line 706 "gram.in"
{ yyval.chval = 0; } break;
case 132:
# line 711 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 133:
# line 713 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL) ); } break;
case 135:
# line 718 "gram.in"
{ yyval.expval = yypvt[-1].expval; } break;
case 139:
# line 725 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 140:
# line 727 "gram.in"
{ yyval.expval = mkexpr(OPSTAR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 141:
# line 729 "gram.in"
{ yyval.expval = mkexpr(OPSLASH, yypvt[-2].expval, yypvt[-0].expval); } break;
case 142:
# line 731 "gram.in"
{ yyval.expval = mkexpr(OPPOWER, yypvt[-2].expval, yypvt[-0].expval); } break;
case 143:
# line 733 "gram.in"
{ if(yypvt[-1].ival == OPMINUS)
			yyval.expval = mkexpr(OPNEG, yypvt[-0].expval, ENULL);
		  else 	yyval.expval = yypvt[-0].expval;
		} break;
case 144:
# line 738 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 145:
# line 740 "gram.in"
{ NO66(".EQV. operator");
		  yyval.expval = mkexpr(OPEQV, yypvt[-2].expval,yypvt[-0].expval); } break;
case 146:
# line 743 "gram.in"
{ NO66(".NEQV. operator");
		  yyval.expval = mkexpr(OPNEQV, yypvt[-2].expval, yypvt[-0].expval); } break;
case 147:
# line 746 "gram.in"
{ yyval.expval = mkexpr(OPOR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 148:
# line 748 "gram.in"
{ yyval.expval = mkexpr(OPAND, yypvt[-2].expval, yypvt[-0].expval); } break;
case 149:
# line 750 "gram.in"
{ yyval.expval = mkexpr(OPNOT, yypvt[-0].expval, ENULL); } break;
case 150:
# line 752 "gram.in"
{ NO66("concatenation operator //");
		  yyval.expval = mkexpr(OPCONCAT, yypvt[-2].expval, yypvt[-0].expval); } break;
case 151:
# line 756 "gram.in"
{ yyval.ival = OPPLUS; } break;
case 152:
# line 757 "gram.in"
{ yyval.ival = OPMINUS; } break;
case 153:
# line 760 "gram.in"
{ yyval.ival = OPEQ; } break;
case 154:
# line 761 "gram.in"
{ yyval.ival = OPGT; } break;
case 155:
# line 762 "gram.in"
{ yyval.ival = OPLT; } break;
case 156:
# line 763 "gram.in"
{ yyval.ival = OPGE; } break;
case 157:
# line 764 "gram.in"
{ yyval.ival = OPLE; } break;
case 158:
# line 765 "gram.in"
{ yyval.ival = OPNE; } break;
case 159:
# line 769 "gram.in"
{ yyval.expval = mkprim(yypvt[-0].namval, PNULL, CHNULL); } break;
case 160:
# line 771 "gram.in"
{ NO66("substring operator :");
		  yyval.expval = mkprim(yypvt[-1].namval, PNULL, yypvt[-0].chval); } break;
case 161:
# line 774 "gram.in"
{ yyval.expval = mkprim(yypvt[-3].namval, mklist(yypvt[-1].chval), CHNULL); } break;
case 162:
# line 776 "gram.in"
{ NO66("substring operator :");
		  yyval.expval = mkprim(yypvt[-4].namval, mklist(yypvt[-2].chval), yypvt[-0].chval); } break;
case 163:
# line 781 "gram.in"
{ yyval.chval = mkchain(yypvt[-3].expval, mkchain(yypvt[-1].expval,CHNULL)); } break;
case 164:
# line 785 "gram.in"
{ yyval.expval = 0; } break;
case 166:
# line 790 "gram.in"
{ if(yypvt[-0].namval->vclass == CLPARAM)
			yyval.expval = (expptr) cpexpr(
				( (struct Paramblock *) (yypvt[-0].namval) ) -> paramval);
		} break;
case 168:
# line 797 "gram.in"
{ yyval.expval = mklogcon(1); } break;
case 169:
# line 798 "gram.in"
{ yyval.expval = mklogcon(0); } break;
case 170:
# line 799 "gram.in"
{ yyval.expval = mkstrcon(toklen, token); } break;
case 171:
# line 800 "gram.in"
 { yyval.expval = mkintcon( convci(toklen, token) ); } break;
case 172:
# line 801 "gram.in"
 { yyval.expval = mkrealcon(TYREAL, convcd(toklen, token)); } break;
case 173:
# line 802 "gram.in"
 { yyval.expval = mkrealcon(TYDREAL, convcd(toklen, token)); } break;
case 174:
# line 806 "gram.in"
{ yyval.expval = mkcxcon(yypvt[-3].expval,yypvt[-1].expval); } break;
case 175:
# line 810 "gram.in"
{ NOEXT("hex constant");
		  yyval.expval = mkbitcon(4, toklen, token); } break;
case 176:
# line 813 "gram.in"
{ NOEXT("octal constant");
		  yyval.expval = mkbitcon(3, toklen, token); } break;
case 177:
# line 816 "gram.in"
{ NOEXT("binary constant");
		  yyval.expval = mkbitcon(1, toklen, token); } break;
case 179:
# line 822 "gram.in"
{ yyval.expval = yypvt[-1].expval; } break;
case 182:
# line 828 "gram.in"
{ yyval.expval = mkexpr(yypvt[-1].ival, yypvt[-2].expval, yypvt[-0].expval); } break;
case 183:
# line 830 "gram.in"
{ yyval.expval = mkexpr(OPSTAR, yypvt[-2].expval, yypvt[-0].expval); } break;
case 184:
# line 832 "gram.in"
{ yyval.expval = mkexpr(OPSLASH, yypvt[-2].expval, yypvt[-0].expval); } break;
case 185:
# line 834 "gram.in"
{ yyval.expval = mkexpr(OPPOWER, yypvt[-2].expval, yypvt[-0].expval); } break;
case 186:
# line 836 "gram.in"
{ if(yypvt[-1].ival == OPMINUS)
			yyval.expval = mkexpr(OPNEG, yypvt[-0].expval, ENULL);
		  else	yyval.expval = yypvt[-0].expval;
		} break;
case 187:
# line 841 "gram.in"
{ NO66("concatenation operator //");
		  yyval.expval = mkexpr(OPCONCAT, yypvt[-2].expval, yypvt[-0].expval); } break;
case 189:
# line 846 "gram.in"
{
		if(yypvt[-3].labval->labdefined)
			execerr("no backward DO loops", CNULL);
		yypvt[-3].labval->blklevel = blklevel+1;
		exdo(yypvt[-3].labval->labelno, yypvt[-0].chval);
		if (hadcomma) {
			if (bugwarn & 2)
				err("syntax error");
			else if (bugwarn & 1)
				warnb("old f77 didn't allow comma after DO label");
			}
		} break;
case 190:
# line 859 "gram.in"
{ exendif();  thiswasbranch = NO; } break;
case 192:
# line 862 "gram.in"
{ exelif(yypvt[-2].expval); lastwasbranch = NO; } break;
case 193:
# line 864 "gram.in"
{ exelse(); lastwasbranch = NO; } break;
case 194:
# line 866 "gram.in"
{ exendif(); lastwasbranch = NO; } break;
case 195:
# line 870 "gram.in"
{ exif(yypvt[-1].expval); } break;
case 196:
# line 874 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].namval, yypvt[-0].chval); } break;
case 197:
# line 878 "gram.in"
{ exequals(yypvt[-2].expval, yypvt[-0].expval); } break;
case 198:
# line 880 "gram.in"
{ exassign(yypvt[-0].namval, yypvt[-2].labval); } break;
case 201:
# line 884 "gram.in"
{ inioctl = NO; } break;
case 202:
# line 886 "gram.in"
{ exarif(yypvt[-6].expval, yypvt[-4].labval, yypvt[-2].labval, yypvt[-0].labval);  thiswasbranch = YES; } break;
case 203:
# line 888 "gram.in"
{ excall(yypvt[-0].namval, PNULL, 0, labarray); } break;
case 204:
# line 890 "gram.in"
{ excall(yypvt[-2].namval, PNULL, 0, labarray); } break;
case 205:
# line 892 "gram.in"
{ if(nstars < MAXLABLIST)
			excall(yypvt[-3].namval, mklist(yypvt[-1].chval), nstars, labarray);
		  else
			err("too many alternate returns");
		} break;
case 206:
# line 898 "gram.in"
{ exreturn(yypvt[-0].expval);  thiswasbranch = YES; } break;
case 207:
# line 900 "gram.in"
{ exstop(yypvt[-2].ival, yypvt[-0].expval);  thiswasbranch = yypvt[-2].ival; } break;
case 208:
# line 904 "gram.in"
{ yyval.labval = mklabel( convci(toklen, token) ); } break;
case 209:
# line 908 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc(PNULL, CLMAIN);
			}
		} break;
case 210:
# line 917 "gram.in"
{ exgoto(yypvt[-0].labval);  thiswasbranch = YES; } break;
case 211:
# line 919 "gram.in"
{ exasgoto(yypvt[-0].namval);  thiswasbranch = YES; } break;
case 212:
# line 921 "gram.in"
{ exasgoto(yypvt[-4].namval);  thiswasbranch = YES; } break;
case 213:
# line 923 "gram.in"
{ if(nstars < MAXLABLIST)
			putcmgo(fixtype(yypvt[-0].expval), nstars, labarray);
		  else
			err("computed GOTO list too long");
		} break;
case 214:
# line 930 "gram.in"
{ hadcomma = 0; } break;
case 215:
# line 931 "gram.in"
{ hadcomma = 1; } break;
case 216:
# line 935 "gram.in"
{ nstars = 0; yyval.namval = yypvt[-0].namval; } break;
case 217:
# line 939 "gram.in"
{ yyval.chval = (yypvt[-0].expval ? mkchain(yypvt[-0].expval,CHNULL) : CHNULL); } break;
case 218:
# line 941 "gram.in"
{ if(yypvt[-0].expval)
			if(yypvt[-2].chval) yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL));
			else yyval.chval = mkchain(yypvt[-0].expval,CHNULL);
		  else
			yyval.chval = yypvt[-2].chval;
		} break;
case 220:
# line 951 "gram.in"
{ if(nstars<MAXLABLIST) labarray[nstars++] = yypvt[-0].labval; yyval.expval = 0; } break;
case 221:
# line 955 "gram.in"
{ yyval.ival = 0; } break;
case 222:
# line 957 "gram.in"
{ yyval.ival = 1; } break;
case 223:
# line 961 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 224:
# line 963 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval,CHNULL) ); } break;
case 225:
# line 967 "gram.in"
{ if(parstate == OUTSIDE)
			{
			newproc();
			startproc(PNULL, CLMAIN);
			}
		  if(parstate < INDATA) enddcl();
		} break;
case 226:
# line 977 "gram.in"
{ intonly = YES; } break;
case 227:
# line 981 "gram.in"
{ intonly = NO; } break;
case 228:
# line 986 "gram.in"
{ endio(); } break;
case 230:
# line 991 "gram.in"
{ ioclause(IOSUNIT, yypvt[-0].expval); endioctl(); } break;
case 231:
# line 993 "gram.in"
{ ioclause(IOSUNIT, PNULL); endioctl(); } break;
case 232:
# line 995 "gram.in"
{ ioclause(IOSUNIT, IOSTDERR); endioctl(); } break;
case 234:
# line 998 "gram.in"
{ doio(PNULL); } break;
case 235:
# line 1000 "gram.in"
{ doio(PNULL); } break;
case 236:
# line 1002 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 237:
# line 1004 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 238:
# line 1006 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 239:
# line 1008 "gram.in"
{ doio(PNULL); } break;
case 240:
# line 1010 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 241:
# line 1012 "gram.in"
{ doio(PNULL); } break;
case 242:
# line 1014 "gram.in"
{ doio(yypvt[-0].chval); } break;
case 244:
# line 1021 "gram.in"
{ iostmt = IOBACKSPACE; } break;
case 245:
# line 1023 "gram.in"
{ iostmt = IOREWIND; } break;
case 246:
# line 1025 "gram.in"
{ iostmt = IOENDFILE; } break;
case 248:
# line 1032 "gram.in"
{ iostmt = IOINQUIRE; } break;
case 249:
# line 1034 "gram.in"
{ iostmt = IOOPEN; } break;
case 250:
# line 1036 "gram.in"
{ iostmt = IOCLOSE; } break;
case 251:
# line 1040 "gram.in"
{
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, yypvt[-0].expval);
		endioctl();
		} break;
case 252:
# line 1046 "gram.in"
{
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, PNULL);
		endioctl();
		} break;
case 253:
# line 1054 "gram.in"
{
		ioclause(IOSUNIT, yypvt[-1].expval);
		endioctl();
		} break;
case 254:
# line 1059 "gram.in"
{ endioctl(); } break;
case 257:
# line 1067 "gram.in"
{ ioclause(IOSPOSITIONAL, yypvt[-0].expval); } break;
case 258:
# line 1069 "gram.in"
{ ioclause(IOSPOSITIONAL, PNULL); } break;
case 259:
# line 1071 "gram.in"
{ ioclause(IOSPOSITIONAL, IOSTDERR); } break;
case 260:
# line 1073 "gram.in"
{ ioclause(yypvt[-1].ival, yypvt[-0].expval); } break;
case 261:
# line 1075 "gram.in"
{ ioclause(yypvt[-1].ival, PNULL); } break;
case 262:
# line 1077 "gram.in"
{ ioclause(yypvt[-1].ival, IOSTDERR); } break;
case 263:
# line 1081 "gram.in"
{ yyval.ival = iocname(); } break;
case 264:
# line 1085 "gram.in"
{ iostmt = IOREAD; } break;
case 265:
# line 1089 "gram.in"
{ iostmt = IOWRITE; } break;
case 266:
# line 1093 "gram.in"
{
		iostmt = IOWRITE;
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, yypvt[-1].expval);
		endioctl();
		} break;
case 267:
# line 1100 "gram.in"
{
		iostmt = IOWRITE;
		ioclause(IOSUNIT, PNULL);
		ioclause(IOSFMT, PNULL);
		endioctl();
		} break;
case 268:
# line 1109 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].tagval, CHNULL); } break;
case 269:
# line 1111 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].tagval, CHNULL)); } break;
case 270:
# line 1115 "gram.in"
{ yyval.tagval = (tagptr) yypvt[-0].expval; } break;
case 271:
# line 1117 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval,yypvt[-3].chval); } break;
case 272:
# line 1121 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].expval, CHNULL); } break;
case 273:
# line 1123 "gram.in"
{ yyval.chval = mkchain(yypvt[-0].tagval, CHNULL); } break;
case 275:
# line 1128 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].expval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 276:
# line 1130 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].expval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 277:
# line 1132 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].tagval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 278:
# line 1134 "gram.in"
{ yyval.chval = mkchain(yypvt[-2].tagval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 279:
# line 1136 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].expval, CHNULL) ); } break;
case 280:
# line 1138 "gram.in"
{ yyval.chval = hookup(yypvt[-2].chval, mkchain(yypvt[-0].tagval, CHNULL) ); } break;
case 281:
# line 1142 "gram.in"
{ yyval.tagval = (tagptr) yypvt[-0].expval; } break;
case 282:
# line 1144 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, mkchain(yypvt[-3].expval, CHNULL) ); } break;
case 283:
# line 1146 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, mkchain(yypvt[-3].tagval, CHNULL) ); } break;
case 284:
# line 1148 "gram.in"
{ yyval.tagval = (tagptr) mkiodo(yypvt[-1].chval, yypvt[-3].chval); } break;
case 285:
# line 1152 "gram.in"
{ startioctl(); } break;
	}
	goto yystack;  /* stack new state and value */
}
