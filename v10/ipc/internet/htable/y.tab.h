
typedef union  {
	int	number;
	struct	addr *addrlist;
	struct	name *namelist;
} YYSTYPE;
extern YYSTYPE yylval;
# define END 257
# define NUMBER 258
# define KEYWORD 259
# define NAME 260
