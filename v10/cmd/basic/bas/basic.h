/*
 * global variables:
 */

int	trace,		/* execution trace flag */
	tflg,		/* debugging flag */
	attnflg,	/* keyboard interrupt flag */
	tty,		/* non-zero if input is from terminal */
	col,		/* current column number for stdin */
	typelens[];	/* defined in sym.c */
char	*prompt,	/* pointer to current prompt string */
	*tokens[],	/* defined in token.c */
	*tempfile,	/* temporary file name for editing (cmds.c) */
	curfile[MAXFILENAME];	/* name of current input file */
FILE	*infile;	/* file pointer of current input file */

char	*lines, 	/* pointer to beginning of line buffer */
	*endlines,	/* pointer to end of line buffer */
	*lastline;	/* line buffer free pointer */
int	linecnt;	/* number of lines in the buffer */

char	*symspace,	/* pointer to beginning of symbol table space */
	*symend,	/* pointer to end of symbol table space */
	*symlast;	/* symbol table free pointer */
Symptr	chains[3][MAXTYPES];	/* symbol table list headers:
				 * scalar, vector, fn for each type
				 */

char	stkspace[MAXSTACK],	/* execution stack space */
	*stkptr;		/* pointer to current top of stack */

char	strspace[MAXSTRSPACE],	/* string table space */
	*strptr;		/* string table free pointer */

char	askline[MAXLINELEN],	/* data line for INPUT/ASK statement */
	*askptr,		/* current position in askline */
	askdelims[];		/* defined in io.c */

char	lnrdelims[];	/* defined in main.c */

int	argcnt; 	/* count for ARG$ */
char	**argvec;	/* argument strings for ARG$ */

Stkfr	gosub,		/* for use in GOSUB statements */
	data;		/* indicates position of DATA statements */

int	nsubs,		/* used in variable declarations */
	subsc[MAXSUBS];

Linep	curline;	/* pointer to line being interpreted */
char	*inptr; 	/* position in said line */
struct textline immed;	/* intermediate line structure */

Linep	nextline();	/* required by ALL_LINES macro */
