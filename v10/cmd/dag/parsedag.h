
typedef union  {
	char		*s;
	int			i;
	pair_t		*p;	// for node lists
	DAG_edge_t	*e;	// for edge lists
} YYSTYPE;
extern YYSTYPE yylval;
# define AS 257
# define BACKEDGE 258
# define BACKPATH 259
# define COLOR 260
# define DASHED 261
# define DOTTED 262
# define DRAW 263
# define EDGE 264
# define EDGES 265
# define EQUALLY 266
# define EXACTLY 267
# define FROM 268
# define HEIGHT 269
# define INVIS 270
# define LABEL 271
# define MAXIMUM 272
# define MINIMUM 273
# define NODES 274
# define ORDERED 275
# define PATH 276
# define POINTSIZE 277
# define RANK 278
# define RANKS 279
# define SAME 280
# define SEPARATE 281
# define SOLID 282
# define TO 283
# define WEIGHT 284
# define WIDTH 285
# define STRING 286
# define DESC 287
