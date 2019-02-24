# include "r.h"

char *keyword [] = {
	"do",
	"if",
	"else",
	"for",
	"repeat",
	"until",
	"while",
	"break",
	"next",
	"define",
	"include",
	"return",
	"switch",
	"case",
	"default",
	0};

int keytran[] = {
	DO,
	IF,
	ELSE,
	FOR,
	REPEAT,
	UNTIL,
	WHILE,
	BREAK,
	NEXT,
	DEFINE,
	INCLUDE,
	RETURN,
	SWITCH,
	CASE,
	DEFAULT,
	0};

char	*fcnloc;	/* spot for "function" */
char	*FCN1loc;	/* spot for "FUNCTION"; kludge */

int	svargc;
char	**svargv;
char	*curfile[10]	= { "" };
int	infptr	= 0;
FILE	*outfil	= { stdout };
FILE	*infile[10]	= { stdin };
int	linect[10];

int	contfld	= CONTFLD;	/* place to put continuation char */
int	printcom	= 0;	/* print comments if on */
int	hollerith	= 0;	/* convert "..." to 27H... if on */
int	uppercase	= 0;	/* produce output in upper case (except for "...") */
int	f77		= 0;	/* output in fortran 77 (if-then-else-endif) */

main(argc,argv) int argc; char **argv; {
	int i;
	while(argc>1 && argv[1][0]=='-' && (i = argv[1][1]) != '\0') {
		if (isdigit(i)) {
			contfld = i - '0';
			if (argv[1][2]!='\0')
				contchar = argv[1][2];
		} else if (i == 'C')
			printcom++;
		else if (i == 'h')
			hollerith++;
		else if (i == 'u' && (argv[1][2] == 'c' || argv[1][2] == 'C'))
			uppercase++;
		else if (strcmp(argv[1], "-f77") == 0)
			f77 = 1;
		else if (strcmp(argv[1], "-f66") == 0)
			f77 = 0;
		argc--;
		argv++;
	}


	svargc = argc;
	svargv = argv;
	if (svargc > 1)
		putbak('\0');
	for (i=0; keyword[i]; i++)
		install(keyword[i], "", keytran[i]);
	fcnloc = install("function", "", 0);
	FCN1loc = install("FUNCTION", "", 0);
	yyparse();
	exit(errorflag);
}

cant(s) char *s; {
	linect[infptr] = 0;
	curfile[infptr] = s;
	error("can't open");
	exit(1);
}

inclstat() {
	int c;
	char *ps;
	char fname[100];
	while ((c = getchr()) == ' ' || c == '\t');
	if (c == '(') {
		for (ps=fname; (*ps=getchr()) != ')'; ps++);
		*ps = '\0';
	} else if (c == '"' || c == '\'') {
		for (ps=fname; (*ps=getchr()) != c; ps++);
		*ps = '\0';
	} else {
		putbak(c);
		for (ps=fname; (*ps=getchr()) != ' ' &&*ps!='\t' && *ps!='\n' && *ps!=';'; ps++);
		*ps = '\0';
	}
	if ((infile[++infptr] = fopen(fname,"r")) == NULL) {
		cant(fname);
		exit(1);
	}
	linect[infptr] = 0;
	curfile[infptr] = fname;
}

char	str[5000];
int	nstr;

yylex() {
	int c, t;
	for (;;) {
		while ((c=gtok(str))==' ' || c=='\n' || c=='\t')
			;
		yylval = c;
		if (c==';' || c=='{' || c=='}')
			return(c);
		if (c==EOF)
			return(0);
		yylval = (int) str;
		if (c == DIG)
			return(DIGITS);
		t = lookup(str)->ydef;
		if (t==DEFINE)
			defstat();
		else if (t==INCLUDE)
			inclstat();
		else if (t > 0)
			return(t);
		else
			return(GOK);
	}
}

int	dbg	= 0;

yyerror(p) char *p; {;}


defstat() {
	int c,i,val,t,nlp;
	extern int nstr;
	extern char str[];
	while ((c=getchr())==' ' || c=='\t');
	if (c == '(') {
		t = '(';
		while ((c=getchr())==' ' || c=='\t');
		putbak(c);
	}
	else {
		t = ' ';
		putbak(c);
	}
	for (nstr=0; c=getchr(); nstr++) {
		if (!isalpha(c) && !isdigit(c))
			break;
		str[nstr] = c;
	}
	putbak(c);
	str[nstr] = '\0';
	if (c != ' ' && c != '\t' && c != '\n' && c != ',') {
		error("illegal define statement");
		return;
	}
	val = nstr+1;
	if (t == ' ') {
		while ((c=getchr())==' ' || c=='\t');
		putbak(c);
		for (i=val; (c=getchr())!='\n' && c!='#' && c!='\0'; i++)
			str[i] = c;
		putbak(c);
	} else {
		while ((c=getchr())==' ' || c=='\t' || c==',' || c=='\n');
		putbak(c);
		nlp  = 0;
		for (i=val; nlp>=0 && (c=str[i]=getchr()); i++)
			if (c == '(')
				nlp++;
			else if (c == ')')
				nlp--;
		i--;
	}
	for ( ; i>0; i--)
		if (str[i-1] != ' ' && str[i-1] != '\t')
			break;
	str[i] = '\0';
	install(str, &str[val], 0);
}

