
# line 2 "x.y"
#include <stdio.h>
#include "pico.h"
#include "pico2.h"

#define usedboth	usedold=usednew=1

#define YYDEBUG	1

extern short CURSCRATCH, CUROLD;
extern char *progr;
extern struct SRC src[MANY];
extern int nsrc, nrparams, LO, linenumber, whichdevice;
extern char usednew, usedold, faster, frameb, metheus;
extern char isglobal, seetree, optim, touchedcmap;
extern FILE *INPUT;

# line 19 "x.y"
typedef union {
	Node	*node;
	Symbol	*sym;
	int	resu;
	struct SNode *snode;
} YYSTYPE;
# define VARIABLE 257
# define AUTO 258
# define ARRAY 259
# define FCT 260
# define BLTN 261
# define NAME 262
# define VAL 263
# define STRING 264
# define A 265
# define WINDOW 266
# define C 267
# define DEF 268
# define DEL 269
# define SCOM 270
# define ECOM 271
# define UP 272
# define DOWN 273
# define RUN 274
# define F 275
# define H 276
# define Q 277
# define R 278
# define SET 279
# define SETF 280
# define GET 281
# define W 282
# define PCONST 283
# define RETURN 284
# define OPEN 285
# define CLOSE 286
# define DECL 287
# define INT 288
# define GLOBAL 289
# define X 290
# define Y 291
# define I 292
# define OLD 293
# define TMP 294
# define CMAP 295
# define FOR 296
# define DO 297
# define WHILE 298
# define JUMP 299
# define IF 300
# define ELSE 301
# define COLOR 302
# define NOCOLOR 303
# define ZAP 304
# define FASTER 305
# define SLOWER 306
# define HELP 307
# define FRAMEBUFFER 308
# define METHEUS 309
# define BLIND 310
# define SEETREE 311
# define OPTIM 312
# define ASSIGN 313
# define OPER 314
# define OROR 315
# define ANDAND 316
# define OR 317
# define AND 318
# define EQ 319
# define NE 320
# define GT 321
# define GE 322
# define LT 323
# define LE 324
# define LSH 325
# define RSH 326
# define POST 327
# define UNARYMINUS 328
# define NOT 329
# define RCHAN 330
# define GCHAN 331
# define BCHAN 332
# define BW 333
# define RGB 334
# define POW 335
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 316 "x.y"

short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 108,
	58, 79,
	-2, 135,
-1, 109,
	58, 80,
	-2, 136,
-1, 112,
	58, 83,
	-2, 137,
-1, 113,
	58, 84,
	-2, 125,
-1, 114,
	58, 67,
	-2, 131,
-1, 115,
	58, 68,
	-2, 132,
-1, 116,
	58, 69,
	-2, 133,
-1, 134,
	286, 50,
	-2, 55,
	};
# define YYNPROD 180
# define YYLAST 1448
short yyact[]={

  62,  66, 178, 270, 176, 269, 138, 137, 338, 174,
 172, 265, 173, 264, 175, 263, 262,  62, 163, 176,
 136,  92,  63,  64, 174, 172, 238, 173, 176, 175,
 171, 140, 139, 174, 104, 266, 104, 176, 175, 156,
 104, 214, 174, 172, 213, 173, 212, 175, 333, 334,
 335, 332, 331, 221, 222, 223, 220, 219, 119,  73,
 133, 177, 320, 207, 152, 232, 199, 200, 201, 130,
 198, 102,  97,  39,  96, 301,   7,  21, 303,  23,
   8,  41,  42,  82, 302,   3,   9,  11,   6,   5,
  19,  20,  10,  22,  68,  71,  72,  69,  70,  67,
 330,  74,  39, 329,  39, 230, 118, 166,  39, 328,
 135, 327, 326,  17,  16, 141,  12,  13,  18, 210,
 325, 104,  14,  15, 209, 324, 284,  75,  76,  77,
  62, 282,  62,  39, 123, 215, 104, 283, 260, 127,
 281,  82, 280,  68,  71,  72,  69,  70,  67, 151,
  74, 279,  75,  76,  77, 146, 278, 202, 143, 274,
 273, 148,  64, 339, 236, 176,  25, 104, 107, 272,
 174, 172,  27, 173, 167, 175,  75,  76,  77, 150,
 149, 164, 268,  62, 271, 124, 234,  58, 154,  39,
 101, 169, 193, 100,  99,  98,  84, 267,  89, 208,
 147,  78,  24, 103,  39, 106, 203,  28, 165,  26,
 316, 289, 290, 288, 275, 293, 142, 162, 161, 145,
 128, 109, 112, 113, 110, 111, 108, 143,  74, 120,
 153, 144,  27, 117, 211,  39, 104, 235, 109, 112,
 113, 110, 111, 108, 206,  74,  40,  30,  36,  38,
 231,  90,   2,  52, 114, 115, 116,  61,  60,  37,
  33,  35,  44, 105,  43,  36,  38,  28, 233,  26,
  52, 114, 115, 116,  61,  60,  37,  33,  35,  44,
 105,  43, 186, 185, 187, 188, 183, 184, 179, 180,
 181, 182, 189, 190,  55,  56,  57,  53,  54, 309,
 310,  81, 178,   1,  39,   4, 291, 189, 190,  86,
 168,  55,  56,  57,  53,  54, 191, 178,  48, 306,
 194, 195, 196,  34,  32, 197, 178, 132, 208, 121,
 204, 205, 300, 134,   0, 178,   0, 217, 218,  62,
   0,   0, 104, 123,   0, 228, 305,   0, 127, 307,
 308,  50,  51,  59,  41,  42,  49, 125, 126,  81,
   0, 340,   0,   0, 239, 240, 241, 242, 243, 244,
 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
 255, 256, 257, 258,  45,  46,  47,  61,  60,  61,
  60, 176,   0,   0, 124, 337, 174, 172,   0, 173,
   0, 175,   0,   0,  50,  51,  59,  41,  42,  49,
  39,   0, 341,   0,   0,   0,   0, 171,   0,   0,
   0,   0,   0, 129,  55,  56,  57,  53,  54, 128,
 292,  36,  38,   0,   0,   0,  52,  45,  46,  47,
  61,  60,  37,  33,  35,  44,   0,  43, 177, 179,
 180, 181, 182, 189, 190, 296, 297, 298, 299,   0,
   0,   0, 176, 178,   0, 122,   0, 174, 172,  29,
 173,   0, 175,   0, 319,   0,   0,  55,  56,  57,
  53,  54,   0,   0, 321, 176, 322, 323, 171,   0,
 174, 172,   0, 173,   0, 175,   0,   0,  29,  62,
  29,   0,   0,   0,  29,   0, 294,   0,  82,   0,
   0, 171, 176,   0, 336,   0, 277, 174, 172, 177,
 173,   0, 175,   0,   0,   0,   0,   0,   0,  29,
   0,   0,   0,   0,   0,   0,   0,   0, 171,   0,
   0,   0, 177,   0, 176,   0,   0,   0, 276, 174,
 172,   0, 173,   0, 175,   0,   0,   0,   0,   0,
  50,  51,  59,  41,  42,  49, 125, 126,   0, 177,
 171,   0, 176,   0,   0,   0,   0, 174, 172, 261,
 173,   0, 175,   0,   0,  29,   0,   0,   0,   0,
   0,   0,   0,  45,  46,  47,  61,  60, 171, 176,
  29, 177,   0, 259, 174, 172,   0, 173,   0, 175,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 171,   0,   0,   0, 177,
   0,  29, 129,  55,  56,  57,  53,  54, 176,   0,
   0,   0,   0, 174, 172,   0, 173,   0, 175,   0,
   0,   0,   0,   0,   0,   0, 177,   0,   0,   0,
 237,   0,   0,   0, 171,   0,   0,   0,   0, 186,
 185, 187, 188, 183, 184, 179, 180, 181, 182, 189,
 190,   0, 176,   0,   0,   0,   0, 174, 172, 178,
 173,   0, 175,   0,   0, 177,   0,   0,   0,   0,
  29,   0,   0,   0, 170, 176,   0,   0, 171,   0,
 174, 172,   0, 173,   0, 175,   0,   0,   0,   0,
  68,  71,  72,  69,  70,  67,  81,  74,   0,   0,
   0, 171,  95,   0,   0,   0,   0,   0,   0, 177,
 186, 185, 187, 188, 183, 184, 179, 180, 181, 182,
 189, 190,   0,  75,  76,  77,  61,  60,   0,   0,
 178,   0, 177, 186, 185, 187, 188, 183, 184, 179,
 180, 181, 182, 189, 190,   0, 131,   0,   0, 176,
  31,   0,   0, 178, 174, 172,   0, 173,   0, 175,
 186, 185, 187, 188, 183, 184, 179, 180, 181, 182,
 189, 190,   0,   0,   0,   0,  29,  82, 176,  31,
 178,  31,   0, 174, 172,  31, 173,  62, 175,   0,
   0,   0, 186, 185, 187, 188, 183, 184, 179, 180,
 181, 182, 189, 190,   0,   0, 177,   0, 176,   0,
  31,   0, 178, 174, 172,   0, 173,   0, 175,   0,
 186, 185, 187, 188, 183, 184, 179, 180, 181, 182,
 189, 190,   0, 176,   0, 177,   0,   0, 174, 172,
 178, 173,   0, 175,   0,   0,   0, 186, 185, 187,
 188, 183, 184, 179, 180, 181, 182, 189, 190,   0,
   0,   0,   0, 176,   0, 177,  31, 178, 174, 172,
   0, 173,   0, 175,   0,   0,   0,   0,   0,  62,
   0,  31,   0,   0,   0,   0, 186, 185, 187, 188,
 183, 184, 179, 180, 181, 182, 189, 190,   0, 176,
   0,   0,   0,   0, 174, 172, 178, 173,   0, 175,
   0,   0,  31,   0,  68,  71,  72,  69,  70,  67,
 177,  74,   0,   0,   0,   0,   0,   0,   0, 192,
 186, 185, 187, 188, 183, 184, 179, 180, 181, 182,
 189, 190,   0,   0,   0,   0,   0,  75,  76,  77,
 178,   0,   0, 186, 185, 187, 188, 183, 184, 179,
 180, 181, 182, 189, 190,   0,   0,   0,   0,   0,
   0,   0,   0, 178,   0,   0,   0,   0,   0,   0,
   0,  31,   0,   0,   0,   0,   0,   0,   0,  68,
  71,  72,  69,  70,  67,  81,  74,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,  68,  71,
  72,  69,  70,  67,  85,  74, 304, 304,   0,   0,
 304, 304,  75,  76,  77,   0,   0, 186, 185, 187,
 188, 183, 184, 179, 180, 181, 182, 189, 190,   0,
   0,  75,  76,  77,  61,  60,   0, 178,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 185, 187, 188,
 183, 184, 179, 180, 181, 182, 189, 190,   0,   0,
   0,   0, 216,   0,   0,   0, 178, 224, 225,   0,
   0,   0,   0,   0,   0,   0,   0,  31, 187, 188,
 183, 184, 179, 180, 181, 182, 189, 190,   0,   0,
  68,  71,  72,  69,  70,  67, 178,  74,   0,   0,
   0,   0,   0,   0, 188, 183, 184, 179, 180, 181,
 182, 189, 190,   0,   0,   0,   0,   0,   0,   0,
  65, 178,   0,  75,  76,  77,  61,  60,  79,  83,
   0,  87,  91,  80, 188, 183, 184, 179, 180, 181,
 182, 189, 190,  94,  88,   0,   0,   0,   0,   0,
   0, 178,   0,   0,   0,  93,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0, 183, 184, 179, 180, 181, 182, 189, 190,   0,
   0, 295,   0,   0,   0,   0,   0, 178,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0, 311, 312,
 313, 314, 315,   0,   0,   0,   0,   0,   0,   0,
   0, 158,   0,   0, 155,   0, 160,   0,   0,   0,
   0,   0, 157,   0,   0,   0,   0, 159,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 226,
   0, 227,   0, 229,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 285, 286,   0, 287,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0, 317, 318 };
short yypact[]={

-189,-1000, 143, 147, 103,-163, 142, 762, 781,-1000,
 463,-163,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-163,
-1000,  38, 687,-188,-1000,-1000,-1000, 136, 135, 134,
-1000, 131, -19, 193, -19, 103, 303,  14, -19,-307,
-256, 176, 176, 191, 179,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 109,  70,
-1000,-1000,-114, 147,-1000, 129,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
  38,-1000,-224,-1000,-1000,-1000,-1000,-1000,  38,-1000,
-163,-1000,-1000,  38,-1000,-163, 178, 177,-1000,-1000,
-1000,-1000,-283, 122, 150,-163,-1000, 115,-1000,-1000,
 176, 176,-1000,  70,-1000,-1000,-1000, 303,-1000, -19,
 645,-1000,-1000, 303, 303,-1000,-1000, 303, 303, 303,
-307,-1000, 303,-264, -36,-1000,-1000, 303, 303,-138,
-218,-1000,  94,-1000, 303, 303,-277, 303, 303,-1000,
-1000,-1000,-1000,-1000,-1000,  38,-1000,  96,-1000,  38,
-1000,-197,-197, 103,-1000, -19, 105,-1000, 601,-272,
-1000, 303, 303, 303, 303, 303, 303, 303, 303, 303,
 303, 303, 303, 303, 303, 303, 303, 303, 303, 303,
 303, 562,  45, 535,-333,-333,-333, 668,-297,-298,
-300,-302,-251,-1000, 742, 668, 138,-1000,-1000,-308,
-310, 125,-1000,  69,  68,-1000, 173, 507, 475,  65,
  60,  51,  49,  40,  44,  33,  38,  38,-1000,  38,
 172, 167,-1000, 171, -19,-1000,-1000, 303, 175, 448,
  -9,  -9,-333,-333,-333, 826,-333, -18, -18, -18,
 -18, 128, 128, 801, 771, 856, 892,   0,   0,-1000,
-1000, 303, 303, 303, 303, 303,-1000,-1000,-138,-179,
-179,-1000,-218,-179,-179,-1000, 103, 103, 303, 303,
 303, 303, 303, 164,-1000,-163, 873,-1000,-1000,-200,
-1000,-1000, 425, 303, 303,-1000, 668, 668, 668, 668,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,  32,  27,-1000,
-1000,  19,  18,  16,  10,   7,-282,-1000,-1000,-1000,
-1000, 303, 354, 668,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000, -33, 104, 103,-1000,
 -19,-1000 };
short yypgo[]={

   0,  71, 205, 203, 333, 329, 465, 324, 323, 115,
 776, 959, 192, 168, 318,  69, 187, 166,  75,1173,
 309,   1,  59,1160, 305, 303, 252,  22, 105, 251,
 250, 247, 246, 244, 234,  63,  46 };
short yyr1[]={

   0,  25,  25,  25,  25,  25,  24,  24,  26,  26,
  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
  26,  26,  26,  26,  26,  26,  26,  26,  26,  26,
  26,  26,  26,  20,  20,  20,  29,  29,  28,  28,
  30,  30,  17,  17,  17,  13,  13,  13,  13,  13,
  27,  27,  31,  31,  32,  32,  33,  33,  34,  34,
  35,  35,  35,  35,  35,  36,  36,  22,  22,  22,
   3,   4,   4,   9,   9,  11,  11,   7,   8,  21,
  21,  21,  21,  21,  21,  21,  21,  23,   1,   1,
   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,
   2,   2,  14,  14,  14,  14,  14,  14,  14,  14,
  14,  14,  14,  14,  14,  14,  14,  14,  14,  14,
  14,  14,  14,  14,  14,  14,   6,   6,   6,   5,
   5,  15,  15,  15,  15,  15,  15,  15,  10,  10,
  19,  19,  18,  18,  18,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,
  12,  12,  12,  12,  16,  16,  16,  16,  16,  16 };
short yyr2[]={

   0,   2,   2,   3,   3,   2,   5,   5,   0,   2,
   6,   2,   2,   2,   1,   1,   2,   2,   6,   2,
   1,   1,   1,   1,   1,   1,   1,   1,   2,   1,
   5,   2,   3,   1,   3,   5,   1,   2,   0,   1,
   1,   3,   1,   2,   2,   3,   5,   5,   5,   5,
   0,   1,   4,   4,   1,   0,   1,   3,   1,   3,
   1,   1,   1,   3,   3,   4,   4,   1,   1,   1,
   4,   1,   2,   2,   3,   1,   3,   5,   5,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   2,   3,
   3,   1,   2,   2,   1,   2,   2,   5,  10,   2,
   8,   3,   1,   1,   1,   1,   1,   1,   3,   3,
   3,   3,   3,   4,   6,   6,   6,   6,   6,   6,
   6,   6,   6,   6,   4,   1,   2,   3,   3,   1,
   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,
   1,   2,   1,   1,   1,   1,   1,   3,   3,   1,
   1,   5,   2,   2,   2,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   1,   1,   2,   2,   2,   2 };
short yychk[]={

-1000, -25, -26, 274, -24, 278, 277, 265, 269, 275,
 281, 276, 305, 306, 311, 312, 303, 302, 307, 279,
 280, 266, 282, 268,  59, -17,  -2, -13,  -3,  -6,
 -31, -10,  -7, 296,  -8, 297, 284, 295, 285, -15,
 -32, 260, 261, 300, 298, 290, 291, 292, -14, 262,
 257, 258, 289, 333, 334, 330, 331, 332, -16, 259,
 294, 293,  36, -27,  59, -23, -21, 262, 257, 260,
 261, 258, 259, -22, 264, 290, 291, 292,  59, -23,
 -19, 263,  45, -23, -16, 263, -20, -23, -19, -16,
 -29, -23, -21, -19, -23,  45, 262, 260,  59,  59,
  59,  59,  -1,  -3, -21, 299,  -2, -13, 262, 257,
 260, 261, 258, 259, 290, 291, 292,  40,  -1, -27,
 -12,  -5,  -6,  40,  91, 263, 264,  45, 126, 329,
 -15, -10, 313,  46,  -4,  -1, 327, 314, 313, 288,
 287,  -9,  40,  -9,  40,  40,  46,  91,  91, 294,
 293, 263, -21, -17,  59, -19, 263, -19, -23, -19,
 -23,  40,  40, 301,  59,  58, -21,  59, -12,  -1,
  59,  63,  43,  45,  42,  47,  37,  94, 335, 321,
 322, 323, 324, 319, 320, 316, 315, 317, 318, 325,
 326, -12, -11, -12, -12, -12, -12, -12, 334, 330,
 331, 332, -27,  -1, -12, -12, -33, -35, -22, 262,
 257, -34, -36, 262, 259,  41, -11, -12, -12, 334,
 333, 330, 331, 332, -11, -11, -19, -19, -16, -19,
 -28, -30, 262, -28, -27,  -1,  59,  59, 298, -12,
 -12, -12, -12, -12, -12, -12, -12, -12, -12, -12,
 -12, -12, -12, -12, -12, -12, -12, -12, -12,  41,
  93,  44, 313, 313, 313, 313, 286,  59,  44, 313,
 313,  59,  44,  91,  91,  41,  41,  41,  91,  91,
  91,  91,  91,  93,  93, -19, -19, -19,  41,  44,
  41,  -1, -12,  40,  58, -11, -12, -12, -12, -12,
 -35, -18, 263, 257, -10, -18, -36, -18, -18, -27,
 -27, -11, -11, -11, -11, -11,  46, -23, -23, -16,
 262,  59, -12, -12,  93,  93,  93,  93,  93,  93,
  93, 334, 333, 330, 331, 332, -12,  41,  41,  59,
 -27,  -1 };
short yydef[]={

   8,  -2,   0,  55,  50,   0,   0,   0,   0,  14,
  15,   0,  20,  21,  22,  23,  24,  25,  26,  27,
  29,   0,   0,   0,   1,   2,  42,   0,   0,   0,
  94,   0,  55,   0,  55,  50,   0,   0,  55,   0,
   0,   0,   0,   0,   0, 131, 132, 133, 134, 135,
 136, 137,  54, 102, 103, 104, 105, 106, 107, 125,
 174, 175,   0,  55,  51,   0,  87,  79,  80,  81,
  82,  83,  84,  85,  86,  67,  68,  69,   5,   9,
   0, 140,   0,  11,  12,  13,  16,  17,   0,  33,
  19,  36,  28,   0,  31,   0,   0,   0,  43,  44,
  93,  95,  96,   0,   0,   0,  91,   0,  -2,  -2,
  81,  82,  -2,  -2,  -2,  -2,  -2,   0,  99,  55,
   0, 145, 146,   0,   0, 149, 150,   0,   0,   0,
 129, 130,   0,   0,  -2,  71, 126,   0,   0,   0,
   0, 138,   0, 139,   0,   0,   0,   0,   0, 176,
 177, 178, 179,   3,   4,   0, 141,   0,  37,   0,
  32,  38,  38,  50,  88,  55,   0,  92,   0,   0,
 101,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  75, 152, 153, 154,  45,   0,   0,
   0,   0,   0,  72, 127, 128,   0,  56,  60,  61,
  62,   0,  58,   0,   0,  73,   0,   0,   0, 108,
 109, 110, 111, 112,   0,   0,   0,   0,  34,   0,
   0,  39,  40,   0,  55,  89,  90,   0,   0,   0,
 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
 165, 166, 167, 168, 169, 170, 171, 172, 173, 147,
 148,   0,   0,   0,   0,   0,  70,  52,   0,   0,
   0,  53,   0,   0,   0,  74,  50,  50,   0,   0,
   0,   0,   0, 113, 124,   0,   0,  30,   6,   0,
   7,  97,   0,   0,   0,  76,  46,  47,  48,  49,
  57,  63, 142, 143, 144,  64,  59,   0,   0,  77,
  78,   0,   0,   0,   0,   0,   0,  10,  18,  35,
  41,   0,   0, 151,  65,  66, 119, 120, 121, 122,
 123, 114, 115, 116, 117, 118,   0,   0,  50, 100,
  55,  98 };
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
# line 64 "x.y"
{ return 1; } break;
case 2:
# line 65 "x.y"
{ munge(yypvt[-0].resu); return 1; } break;
case 3:
# line 66 "x.y"
{ postfunct(yypvt[-2].sym); return 1; } break;
case 4:
# line 67 "x.y"
{ redirect(yypvt[-1].sym->name);
				  checkit();
				  return 1;
				} break;
case 5:
# line 71 "x.y"
{ return 0; } break;
case 6:
# line 73 "x.y"
{ prefunct(yypvt[-3].sym, nrparams); yyval.sym = yypvt[-3].sym; } break;
case 7:
# line 74 "x.y"
{ prefunct(yypvt[-3].sym, nrparams); yyval.sym = yypvt[-3].sym; } break;
case 8:
# line 76 "x.y"
{ /* if (INPUT == stdin) linenumber--; */ } break;
case 9:
# line 77 "x.y"
{ append( 0,  0,  0,  0, yypvt[-0].sym->name); } break;
case 10:
# line 78 "x.y"
{ append(yypvt[-4].resu, yypvt[-3].resu, yypvt[-2].resu, yypvt[-1].resu, yypvt[-0].sym->name); } break;
case 11:
# line 79 "x.y"
{ discard(yypvt[-0].sym->name); } break;
case 12:
# line 80 "x.y"
{ if (yypvt[-0].resu > 1 && yypvt[-0].resu <= nsrc-1)
				  	discard(src[yypvt[-0].resu].fname);
				  else
					yyerror("unknown file number %d", yypvt[-0].resu);
				} break;
case 13:
# line 85 "x.y"
{ if (yypvt[-0].resu > 1 && yypvt[-0].resu <= nsrc-1)
				  	discard(src[yypvt[-0].resu].fname);
				  else
					yyerror("unknown file number %d", yypvt[-0].resu);
				} break;
case 14:
# line 90 "x.y"
{ files(); } break;
case 15:
# line 91 "x.y"
{ getscreen(Old); } break;
case 16:
# line 92 "x.y"
{ getter(yypvt[-0].resu); } break;
case 17:
# line 93 "x.y"
{ getter(append( 0,  0,  0,  0, yypvt[-0].sym->name)); } break;
case 18:
# line 94 "x.y"
{ getter(append(yypvt[-4].resu, yypvt[-3].resu, yypvt[-2].resu, yypvt[-1].resu, yypvt[-0].sym->name)); } break;
case 20:
# line 96 "x.y"
{ faster = 1; } break;
case 21:
# line 97 "x.y"
{ faster = 0; } break;
case 22:
# line 98 "x.y"
{ seetree = 1 - seetree; } break;
case 23:
# line 99 "x.y"
{ optim = 1 - optim; } break;
case 24:
# line 100 "x.y"
{ Old->nchan = Scratch->nchan = 1; } break;
case 25:
# line 101 "x.y"
{ Old->nchan = Scratch->nchan = 3;
				  checkpix(Old); checkpix(Scratch);
				} break;
case 26:
# line 104 "x.y"
{ dohelp(); } break;
case 27:
# line 105 "x.y"
{ showvars(); } break;
case 28:
# line 106 "x.y"
{ showsym(yypvt[-0].sym); } break;
case 29:
# line 107 "x.y"
{ showfcts(); } break;
case 30:
# line 108 "x.y"
{ setwindow(yypvt[-3].resu, yypvt[-2].resu, yypvt[-1].resu, yypvt[-0].resu); } break;
case 31:
# line 109 "x.y"
{ putdpix(yypvt[-0].sym->name, PICO); } break;
case 32:
# line 110 "x.y"
{ putdpix(yypvt[-0].sym->name, 0); } break;
case 33:
# line 112 "x.y"
{ yyval.resu = yypvt[-0].resu; } break;
case 34:
# line 113 "x.y"
{ yyval.resu = append(yypvt[-2].resu, yypvt[-1].resu,  0,  0, src[yypvt[-0].resu].fname); } break;
case 35:
# line 114 "x.y"
{ yyval.resu = append(yypvt[-4].resu, yypvt[-3].resu, yypvt[-2].resu, yypvt[-1].resu, src[yypvt[-0].resu].fname); } break;
case 36:
# line 116 "x.y"
{ interpret(yypvt[-0].sym->name); } break;
case 37:
# line 117 "x.y"
{ interpret(yypvt[-0].sym->name); } break;
case 40:
# line 122 "x.y"
{ automa(yypvt[-0].sym, nrparams++); } break;
case 41:
# line 123 "x.y"
{ automa(yypvt[-0].sym, nrparams++); } break;
case 42:
# line 125 "x.y"
{ compile(yypvt[-0].node, progr); yyval.resu = BLOOP; } break;
case 43:
# line 126 "x.y"
{ compile(yypvt[-1].node, progr); yyval.resu = SLOOP; } break;
case 44:
# line 127 "x.y"
{ compile(yypvt[-1].node, progr); yyval.resu = NLOOP; } break;
case 45:
# line 129 "x.y"
{ yyval.node = mapall(cast(yypvt[-0].node)); } break;
case 46:
# line 130 "x.y"
{ yyval.node = mapall(yypvt[-0].node); } break;
case 47:
# line 131 "x.y"
{ yyval.node = mapred(cast(yypvt[-0].node)); } break;
case 48:
# line 132 "x.y"
{ yyval.node = mapgrn(cast(yypvt[-0].node)); } break;
case 49:
# line 133 "x.y"
{ yyval.node = mapblu(cast(yypvt[-0].node)); } break;
case 54:
# line 141 "x.y"
{ isglobal = 1; } break;
case 55:
# line 142 "x.y"
{ isglobal = 0; } break;
case 61:
# line 151 "x.y"
{ decvar(yypvt[-0].sym, 0); } break;
case 62:
# line 152 "x.y"
{ decvar(yypvt[-0].sym, 0); } break;
case 63:
# line 153 "x.y"
{ decvar(yypvt[-2].sym, yypvt[-0].resu); } break;
case 64:
# line 154 "x.y"
{ decvar(yypvt[-2].sym, yypvt[-0].resu); } break;
case 65:
# line 156 "x.y"
{ decarray(yypvt[-3].sym, yypvt[-1].resu); } break;
case 66:
# line 157 "x.y"
{ decarray(yypvt[-3].sym, yypvt[-1].resu); } break;
case 67:
# line 159 "x.y"
{ yyval.sym = newsym("x"); } break;
case 68:
# line 160 "x.y"
{ yyval.sym = newsym("y"); } break;
case 69:
# line 161 "x.y"
{ yyval.sym = newsym("i"); } break;
case 70:
# line 163 "x.y"
{ yyval.node = yypvt[-2].node; } break;
case 71:
# line 165 "x.y"
{ yyval.node = yypvt[-0].node; } break;
case 72:
# line 166 "x.y"
{ yyval.node = new(OCOMMA, yypvt[-1].node, yypvt[-0].node, Z); } break;
case 73:
# line 168 "x.y"
{ yyval.node =  Z; } break;
case 74:
# line 169 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 75:
# line 171 "x.y"
{ yyval.node = cast(yypvt[-0].node); } break;
case 76:
# line 172 "x.y"
{ yyval.node = new(ACOMMA, cast(yypvt[-2].node), yypvt[-0].node, Z); } break;
case 77:
# line 174 "x.y"
{ yyval.node = yypvt[-2].node; } break;
case 78:
# line 175 "x.y"
{ yyval.node = yypvt[-2].node; } break;
case 79:
# line 177 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 80:
# line 178 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 81:
# line 179 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 82:
# line 180 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 83:
# line 181 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 84:
# line 182 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 85:
# line 183 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 86:
# line 184 "x.y"
{ yyval.sym = lookup(yypvt[-0].resu); } break;
case 87:
# line 186 "x.y"
{ yyval.sym = yypvt[-0].sym; } break;
case 88:
# line 188 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 89:
# line 189 "x.y"
{ yyval.node = new(LABL,yypvt[-0].node, Z, yypvt[-2].sym); } break;
case 90:
# line 190 "x.y"
{ yyval.node = new(GOTO, Z, Z, yypvt[-1].sym); } break;
case 91:
# line 191 "x.y"
{ yyval.node = yypvt[-0].node; } break;
case 92:
# line 192 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 93:
# line 194 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 94:
# line 195 "x.y"
{ yyval.node = Z; } break;
case 95:
# line 196 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 96:
# line 197 "x.y"
{ yyval.node = new(CONDI, yypvt[-0].node, Z, yypvt[-1].node); } break;
case 97:
# line 198 "x.y"
{ yyval.node = new(CONDI, yypvt[-3].node, yypvt[-0].node, yypvt[-4].node); } break;
case 98:
# line 199 "x.y"
{
				  yyval.node = newloop(yypvt[-7].node, yypvt[-5].node, yypvt[-3].node, yypvt[-0].node); } break;
case 99:
# line 201 "x.y"
{ yyval.node = newhile(yypvt[-1].node, yypvt[-0].node); } break;
case 100:
# line 202 "x.y"
{ yyval.node = newuntil(yypvt[-5].node, yypvt[-2].node); } break;
case 101:
# line 203 "x.y"
{ yyval.node = new(ORETURN, yypvt[-1].node, Z, Z); } break;
case 102:
# line 205 "x.y"
{ usedboth; yyval.snode = super(CURSCRATCH, Z, BW); } break;
case 103:
# line 206 "x.y"
{ usedboth; yyval.snode = super(CURSCRATCH, Z, RGB); } break;
case 104:
# line 207 "x.y"
{ usedboth; yyval.snode = super(CURSCRATCH, Z, RCHAN); } break;
case 105:
# line 208 "x.y"
{ usedboth; yyval.snode = super(CURSCRATCH, Z, GCHAN); } break;
case 106:
# line 209 "x.y"
{ usedboth; yyval.snode = super(CURSCRATCH, Z, BCHAN); } break;
case 107:
# line 210 "x.y"
{ yyval.snode = super(yypvt[-0].resu, Z, (src[yypvt[-0].resu].nchan == 1)?BW:RGB); } break;
case 108:
# line 211 "x.y"
{ yyval.snode = super(yypvt[-2].resu, Z, RGB); } break;
case 109:
# line 212 "x.y"
{ yyval.snode = super(yypvt[-2].resu, Z, BW);  } break;
case 110:
# line 213 "x.y"
{ yyval.snode = super(yypvt[-2].resu, Z, RCHAN); } break;
case 111:
# line 214 "x.y"
{ yyval.snode = super(yypvt[-2].resu, Z, GCHAN); } break;
case 112:
# line 215 "x.y"
{ yyval.snode = super(yypvt[-2].resu, Z, BCHAN); } break;
case 113:
# line 216 "x.y"
{ yyval.snode = super(yypvt[-3].resu, yypvt[-1].node,(src[yypvt[-3].resu].nchan == 1)?BW:RGB); } break;
case 114:
# line 217 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-3].node, RGB); } break;
case 115:
# line 218 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-3].node, BW);  } break;
case 116:
# line 219 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-3].node, RCHAN); } break;
case 117:
# line 220 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-3].node, GCHAN); } break;
case 118:
# line 221 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-3].node, BCHAN); } break;
case 119:
# line 222 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-1].node, RGB); } break;
case 120:
# line 223 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-1].node, BW);  } break;
case 121:
# line 224 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-1].node, RCHAN); } break;
case 122:
# line 225 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-1].node, GCHAN); } break;
case 123:
# line 226 "x.y"
{ yyval.snode = super(yypvt[-5].resu, yypvt[-1].node, BCHAN); } break;
case 124:
# line 227 "x.y"
{ yyval.snode=nsup(RL((yypvt[-3].sym->u.ar->base),Index(yypvt[-1].node)),0,0,128);} break;
case 125:
# line 228 "x.y"
{ yyval.snode=nsup(RL((yypvt[-0].sym->u.ar->base),DII), 0,0,128); } break;
case 126:
# line 230 "x.y"
{ yyval.node = weird(yypvt[-1].snode, notnew(LO, yypvt[-1].snode->n, NR(1))); } break;
case 127:
# line 231 "x.y"
{ yyval.node = weird(yypvt[-2].snode, notnew(LO, yypvt[-2].snode->n, yypvt[-0].node)); } break;
case 128:
# line 232 "x.y"
{ yyval.node = weird(yypvt[-2].snode, yypvt[-0].node); } break;
case 129:
# line 234 "x.y"
{ yyval.node = yypvt[-0].snode->n; } break;
case 130:
# line 235 "x.y"
{ yyval.node = yypvt[-0].node;    } break;
case 131:
# line 237 "x.y"
{ yyval.snode = nsup(new(REG, Z, Z, XREG), 0, 0, 128); } break;
case 132:
# line 238 "x.y"
{ yyval.snode = nsup(new(REG, Z, Z, YREG), 0, 0, 128); } break;
case 133:
# line 239 "x.y"
{ yyval.snode = nsup(new(REG, Z, Z, IREG), 0, 0, 128); } break;
case 134:
# line 240 "x.y"
{ yyval.snode = yypvt[-0].snode; } break;
case 135:
# line 241 "x.y"
{ yyerror("undeclared variable %s", yypvt[-0].sym->name); } break;
case 136:
# line 242 "x.y"
{ yyval.snode = nsup(new(VAR,  Z, Z, &yypvt[-0].sym->u.val), 0,0,128);} break;
case 137:
# line 243 "x.y"
{ yyval.snode = nsup(new(OARG, Z, Z,  yypvt[-0].sym->u.val), 0,0,128);} break;
case 138:
# line 245 "x.y"
{ if (yypvt[-1].sym->u.fu->usednew == -1 && notrecurcall(yypvt[-1].sym))
					yyerror("undefined function %s", yypvt[-1].sym->name);
				  argcount(yypvt[-1].sym->u.fu->nrparams, yypvt[-0].node, yypvt[-1].sym->name);
				  usednew += yypvt[-1].sym->u.fu->usednew;
				  usedold += yypvt[-1].sym->u.fu->usedold;
				  yyval.node = new(OCALL, yypvt[-0].node, Z, yypvt[-1].sym->u.fu->fct);
				} break;
case 139:
# line 252 "x.y"
{ argcount(yypvt[-1].sym->u.bl->nrparams, yypvt[-0].node, yypvt[-1].sym->name);
				  usednew += yypvt[-1].sym->u.bl->usednew;
				  yyval.node = new(CCALL, yypvt[-0].node, Z, yypvt[-1].sym->u.bl->faddr);
				} break;
case 140:
# line 257 "x.y"
{ yyval.resu = yypvt[-0].resu; } break;
case 141:
# line 258 "x.y"
{ yyval.resu = -yypvt[-0].resu; } break;
case 142:
# line 260 "x.y"
{ yyval.resu = yypvt[-0].resu; } break;
case 143:
# line 261 "x.y"
{ yyval.resu = yypvt[-0].sym->u.val; } break;
case 144:
# line 262 "x.y"
{ compile(yypvt[-0].node, progr); yyval.resu = callit(); } break;
case 145:
# line 264 "x.y"
{ yyval.node = yypvt[-0].node; } break;
case 146:
# line 265 "x.y"
{ yyval.node = yypvt[-0].node; } break;
case 147:
# line 266 "x.y"
{ yyval.node = yypvt[-1].node; } break;
case 148:
# line 267 "x.y"
{ yyval.node = new(COMP, yypvt[-1].node, Z, Z); } break;
case 149:
# line 268 "x.y"
{ yyval.node = NR(yypvt[-0].resu); } break;
case 150:
# line 269 "x.y"
{ yyval.node = NR(yypvt[-0].resu); } break;
case 151:
# line 270 "x.y"
{ yyval.node = new(CONDI, yypvt[-2].node, yypvt[-0].node, yypvt[-4].node); } break;
case 152:
# line 271 "x.y"
{ yyval.node = new(OMINUS, yypvt[-0].node, Z, Z); } break;
case 153:
# line 272 "x.y"
{ yyval.node = new(ONEG, yypvt[-0].node, Z, Z); } break;
case 154:
# line 273 "x.y"
{ yyval.node = new(ONOT, yypvt[-0].node, Z, Z); } break;
case 155:
# line 274 "x.y"
{ yyval.node = notnew(OADD, yypvt[-2].node, yypvt[-0].node); } break;
case 156:
# line 275 "x.y"
{ yyval.node = notnew(OSUB, yypvt[-2].node, yypvt[-0].node); } break;
case 157:
# line 276 "x.y"
{ yyval.node = notnew(OMUL, yypvt[-2].node, yypvt[-0].node); } break;
case 158:
# line 277 "x.y"
{ yyval.node = notnew(DIVV, yypvt[-2].node, yypvt[-0].node); } break;
case 159:
# line 278 "x.y"
{ yyval.node = notnew(MODU, yypvt[-2].node, yypvt[-0].node); } break;
case 160:
# line 279 "x.y"
{ yyval.node = notnew(OXOR, yypvt[-2].node, yypvt[-0].node); } break;
case 161:
# line 280 "x.y"
{ yyval.node = notnew(OPOW, yypvt[-2].node, yypvt[-0].node); } break;
case 162:
# line 281 "x.y"
{ yyval.node = notnew(OGT,  yypvt[-2].node, yypvt[-0].node); } break;
case 163:
# line 282 "x.y"
{ yyval.node = notnew(OGE,  yypvt[-2].node, yypvt[-0].node); } break;
case 164:
# line 283 "x.y"
{ yyval.node = notnew(OLT,  yypvt[-2].node, yypvt[-0].node); } break;
case 165:
# line 284 "x.y"
{ yyval.node = notnew(OLE,  yypvt[-2].node, yypvt[-0].node); } break;
case 166:
# line 285 "x.y"
{ yyval.node = notnew(OEQ,  yypvt[-2].node, yypvt[-0].node); } break;
case 167:
# line 286 "x.y"
{ yyval.node = notnew(ONE,  yypvt[-2].node, yypvt[-0].node); } break;
case 168:
# line 287 "x.y"
{ yyval.node = notnew(OANDAND, yypvt[-2].node, yypvt[-0].node); } break;
case 169:
# line 288 "x.y"
{ yyval.node = notnew(OOROR, yypvt[-2].node, yypvt[-0].node); } break;
case 170:
# line 289 "x.y"
{ yyval.node = notnew(OOR,  yypvt[-2].node, yypvt[-0].node); } break;
case 171:
# line 290 "x.y"
{ yyval.node = notnew(OAND, yypvt[-2].node, yypvt[-0].node); } break;
case 172:
# line 291 "x.y"
{ yyval.node = notnew(OLSH, yypvt[-2].node, yypvt[-0].node); } break;
case 173:
# line 292 "x.y"
{ yyval.node = notnew(OLSH, yypvt[-2].node, notnew(OMINUS, yypvt[-0].node, Z)); } break;
case 174:
# line 294 "x.y"
{ usednew=usedold=1; yyval.resu = CURSCRATCH; } break;
case 175:
# line 295 "x.y"
{ usedold=1; yyval.resu = CUROLD; } break;
case 176:
# line 296 "x.y"
{ usednew=usedold=1; yyval.resu = CURSCRATCH; } break;
case 177:
# line 297 "x.y"
{ usedold=1; yyval.resu = CUROLD; } break;
case 178:
# line 298 "x.y"
{ int x = yypvt[-0].resu;
				  if (x < 0 || x > nsrc-2)
					yyerror("unknown file number %d", x);
				  x = (x == 0) ? CUROLD : x+1;
				  if (x == CUROLD)
					usedold=1;
				  else
					src[x].used = 1;
				  yyval.resu = x;
				} break;
case 179:
# line 308 "x.y"
{ int x = whatarray(yypvt[-0].sym->name);
				  if (x <= 0)
					x = prepare(yypvt[-0].sym->name);
				  if (x <= 0)
					yyerror("unknown file %s", yypvt[-0].sym->name);
				  src[x].used = 1;
				  yyval.resu = x;
				} break;
	}
	goto yystack;  /* stack new state and value */
}
