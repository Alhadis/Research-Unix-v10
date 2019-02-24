
typedef union  {
	char		cc;
	long		ll;
	char		ss[32];
	struct Expr	*ee;
	double		dd;
} YYSTYPE;
extern YYSTYPE yylval;
# define G_EXPR 257
# define G_DOTEQ_CONEX 258
# define G_DOLEQ_CONEX 259
# define G_CONEX 260
# define G_DOTDOT 261
# define ICONST 262
# define ID 263
# define PCENT 264
# define EQUAL 265
# define SLASH 266
# define DOLLAR 267
# define SIZEOF 268
# define TYPEOF 269
# define QMARK 270
# define SEMI 271
# define UNOP 272
# define STAR 273
# define PLUS 274
# define MINUS 275
# define AMPER 276
# define ARROW 277
# define DOT 278
# define LB 279
# define LP 280
# define COMMA 281
# define ERROR 282
# define RB 283
# define RP 284
# define PLUSPLUS 285
# define MINUSMINUS 286
# define EQUALEQUAL 287
# define GREATER 288
# define LESS 289
# define BAR 290
# define BARBAR 291
# define AMPERAMPER 292
# define HAT 293
# define TILDE 294
# define GREATEREQUAL 295
# define LESSEQUAL 296
# define FABS 297
# define GREATERGREATER 298
# define LESSLESS 299
# define BANG 300
# define BANGEQUAL 301
# define DCONST 302
# define LC 303
# define RC 304
# define DOTDOT 305
