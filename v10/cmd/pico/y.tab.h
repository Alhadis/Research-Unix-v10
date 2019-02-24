
typedef union {
	Node	*node;
	Symbol	*sym;
	int	resu;
	struct SNode *snode;
} YYSTYPE;
extern YYSTYPE yylval;
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
