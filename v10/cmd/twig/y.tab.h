# define ERROR 257

typedef union  {
	Node *y_nodep;
	SymbolEntry *y_symp;
	Code *y_code;
	int y_int;
} YYSTYPE;
extern YYSTYPE yylval;
# define K_NODE 258
# define K_LABEL 259
# define K_PROLOGUE 260
# define K_CONST 261
# define K_INSERT 262
# define K_COST 263
# define K_ACTION 264
# define ID 265
# define NUMBER 266
# define CBLOCK 267
