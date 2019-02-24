/*
 * prefer - bibligraphy formatter
 *
 *	Marcia Derr
 *	February 26, 1988
 *
 *	input:	(stdin) text containing prefer commands;
 *	output:	(stdout) text with formatted citations and reference list
 */



#include	<stdio.h>
#include	<ctype.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/stat.h>

#ifdef	SVR2			/* for UNIX System V */ 
#include	<fcntl.h>
#define	dup2(fildes, fildes2) close(fildes2);fcntl(fildes, F_DUPFD, fildes2)
#endif

#include	"streams.h"
#include	"bib.h"


/* prefer commands */
#define		REF_CITE	"reference"
#define		REF_STYLE	"reference_style"
#define		REF_PLACE	"reference_placement"
#define		REF_LIST	"reference_list"
#define		REF_DB		"reference_database"
#define		REF_INCLUDE	"reference_include"

/* control words to awk */
#define		LETS_GO		"%LETS_GO"
#define		BEGIN_CITE	"%BEGIN_CITE"
#define		END_CITE	"%END_CITE"
#define		END_LIST	"%END_LIST"
#define		BIBLIO		"%BIBLIO"
#define		WHOLEBIB	"%WHOLEBIB"
#define		DBINCLUDE	"%DBINCLUDE"
#define		TYPE		"%type"
#define		TWOPASS		"%TWOPASS\n"
#define		ALSO		"%also"
#define		ALSOBEGIN	"%also_begin"
#define		ALSOEND		"%also_end"

#define		DSTYLE		"att"		/* default style */
#define		STYLEDIR	"/styles/"	/* style directory */
#define		AWK		"prefawk"	/* special awk version */

#define		PREFERTMP	"/tmp/preferXXXXXX"
#define		MAXTOK		2500
#define		BAR		'|'
#define		SLASH		"/"


char	stylename[maxstr] = "",		/* set by style(), options */
	awkline[MAXTOK];		/* for passing to/from awk */

FILE	*readtext,		/* input text */
	*writetext,		/* output text */
	*readbib,		/* from awk */
	*writebib,		/* to awk */
	*savefile;		/* save original file when diverting to temp */

int	nroff = 0;		/* nroff/troff flag */
int	prmult = 0;		/* print multiple references */
int	rp = 0;			/* released_paper/tm flag */
int	sort = 0;		/* order flag for reference_list */
char	sortkey[maxstr] = "";	/* sort keys (eg adt) */
int	haveeof = 0;		/* flag eof in input file */

char    INDEX[maxstr];		/* name of index file */
char	*DINPUT = BIBFILE;	/* default input file */
char 	*divertname;		/* two pass output is diverted here */
int	pass = 0;		/* indicates how many text passes are required */

int     argc;
char    **argv;

typedef enum {T_OPEN, T_CLOSE, T_COMMAND, T_REFER, T_WORD, T_EOF} TOKEN;
typedef enum {NORMAL, END, BIBLIST} bibflag;
	enum {OK, NONE}	awkstate = NONE;


char	tok[MAXTOK];			/* for get_token */
TOKEN	type;				/* type returned by get_token */
char	right;				/* expected right delimeter */

int	cleanup();
char	*strchr();
char	*libpath();
TOKEN	get_token();
char	*mktemp(),
	*locate(),
	*gulpnl(),
	*realloc(),
	*strtok();

#define	leftdelim {								\
	if((type = get_token(tok,0)) != T_OPEN) {				\
		token_err(type,"Expecting left delimiter, got %s\n",tok);	\
	}									\
	right = getright(*tok);							\
}

#define	rightdelim {								\
	if (type != T_CLOSE)							\
		token_err(type,"Expecting right delimeter, got %s\n",tok);	\
	post_command();							\
}


#define USAGE	"prefer [-p database -s style -n -o key -r] [filename]\n"

#ifdef	_NFILE
#define	FILENUM	_NFILE
#else
#define	FILENUM	20
#endif



/*
 * main - process prefer command line 
 */

main(argcount,arglist)
int argcount;
char **arglist;
{

/*
 * set up signals, get options flags, and process command line arguments;
 * make path names;  setup I/O
 */	
	if(signal(SIGINT, SIG_IGN) != SIG_IGN)
		signal(SIGINT, cleanup);
	signal(SIGQUIT, cleanup);
	argc= argcount-1;
	argv= arglist+1;

	strcpy(stylename,DSTYLE);
	strcpy(INDEX,DINPUT);

	flags();
	strcat(INDEX,".i");

	switch(argc) {
	case 0:
		readtext = stdin;
		break;
	case 1:
		if((readtext = fopen(*argv,"r")) == NULL)
			errexit(1,"error opening textfile %s\n",*argv);
		break;
	default:
		errexit(1,USAGE);
	}
	writetext = stdout;

/*
 * now filter text for prefer commands
 */
	scan_text();
	exit(0);

}


/*
 * get option flags
 *
 * -n	set nroff flag (default troff)
 * -o	set sort flag for listing entire bibliographies (default sequence)
 * -p	pathname of database file (default BIBFILE)
 * -r	set rp (released paper) flag (default tm)
 * -s	set style (default att)
 */

# define    operand     (strlen(*argv+2)==0 ? (argv++,argc--,*argv) : *argv+2)

flags()
{   
    for (; argc>0 && *argv[0]=='-';  argc--,argv++) {
	switch ((*argv)[1]) {   
	    case 'c':
	    case 'l':	fprintf(stderr,USAGE);
			break;
	case 'm':	prmult = 1;
			break;
	    case 'n':	nroff = 1;
			break;
	    case 'o':	sort = 1;
			strcpy(sortkey,operand);
			break;
            case 'p':   strcpy(INDEX,operand);
                        break;
	    case 'r':	rp = 1;
			break;
	    case 's':	strcpy(stylename,operand);
			break;
            default:    errexit(1, USAGE);
        }
    }
}


/*
 * scan_text
 *
 * filter text for prefer commands (beginning with '|') and pass
 * these off to do_command
 */

scan_text()
{
	register int c, lastc;
	char space[maxstr];

	lastc = 0;
	for(;;) {

	/*
	 * if EOF, put out last saved character and space;
	 * finish processing references; close awk
	 */
		if((c = gobble(space)) == EOF) {
			if(lastc) putc(lastc,writetext);
			lastc = 0;
			if(*space) fputs(space, writetext);
			if(awkstate == OK) {
				biblio(END);
				continue;
			}
			break;
		}

	/*
	 * if a '|' then call do_command to process it
	 */
		if(c == BAR) {

			/* checkfor preceding punctuation */

			if(lastc == 0134 && !*space) {	/* backslash */
				putc(lastc,writetext);
				lastc = c;
				continue;
			}
			if(!ismypunct(lastc)) {
				if(lastc) {
					putc(lastc,writetext);
					lastc = 0;
				}
			}
			do_command(lastc,space);
			lastc = 0;
			continue;
		}

		if(lastc) putc(lastc,writetext);
		if(*space) fputs(space,writetext);
		lastc = c;
	}
	if(awkstate != NONE)
		awkclose(readbib,writebib);
}


/*
 * parse and execute prefer commands:
 *	|reference(...)
 *	|reference_style(..)
 *	|reference_database(...)
 *	|reference_list(...)
 *	|reference_placement
 */

do_command(ppunc,white)
int ppunc;	/* punctuation preceding command */
char *white;	/* white space preceding command */
{
	register  int	cite_list,	/* flag "in a citation list" */
			c;		/* for reading characters */
	char		space[maxstr];	/* gobbled space goes here */

/*
 * '|' has already been read; now read command and dispatch
 */
	cite_list = 0;
	for(;;) {

	/*
	 * if space after BAR; finish citation list put out white space and punc
	 */
		c=gobble(space);
		if(*space) {
			if(cite_list) {
				end_citations(ppunc);
			}
			else {
				if(ppunc) putc(ppunc,writetext);
				if(*white) fputs(white,writetext);
			}
			putc(BAR,writetext);
			fputs(space,writetext);
			if(c != EOF) ungetc(c,readtext);
			return;
		}
	/*
	 * at EOF, put out any citation list
	 */
		if(c == EOF) {
			if(cite_list) {	
				end_citations(ppunc);
				cite_list == 0;
			}
			putc(BAR,writetext);
			return;
		}
		else
			ungetc(c,readtext);
			

	/*
	 * process one or more citations
	 */
		get_token(tok,0);
		if(strcmp(tok,REF_CITE) == 0) {
			*white = NULL;
			citation();
			cite_list = 1;
			if((c = gobble(white)) == BAR)
				continue;
			else if (*white || !ismypunct(c)) {
				end_citations(ppunc);
				if(*white) fputs(white,writetext);
				if(c != EOF) ungetc(c,readtext);
			}
			else {
				if(ppunc) putc(ppunc,writetext);
				end_citations(c);
			}
			return;
		}
	
	/*
	 * end a list of citations or if no citation, put out punctuation
	 * and white space
	 */
		if(cite_list)  end_citations(ppunc);
		else if (ppunc) putc(ppunc,writetext);
		if (*white) {
			fputs(white,writetext);
		}
			

	/*
 	 * process style, placement, list, or db commands
	 */
		if(strcmp(tok,REF_STYLE) == 0) {
			style();
			return;
		}

		else if(strcmp(tok,REF_PLACE) == 0) {
			if(awkstate == OK) biblio(NORMAL);
			if(pass != 2) post_command();
			return;
		}

		else if(strcmp(tok,REF_LIST) == 0) {
			bib_list();
			return;
		}

		else if(strcmp(tok,REF_INCLUDE) == 0) {
			dbinclude();
			return;
		}

		else if(strcmp(tok,REF_DB) == 0) {
			database();
			return;
		}

	/*
	 * no command so, put out bar and token
	 */
		else {
			putc(BAR,writetext);
			fputs(tok,writetext);
			return;
		}
	}

}


/*
 * ismypunct - test for puctuation
 */

ismypunct(c)
int c;
{
	switch(c) {
	case '.':
	case ',':
	case ';':
	case ':':
	case '?':
	case '!':
	case '"':
	case '\'':
	case '(':
		return(1);
	default:
		return(0);
	}
}


/*
 * getright - given right delimeter, return left
 */

getright(left)
char left;
{
	switch(left) {
	case '(':
		return(')');
	case '{':
		return('}');
	case '[':
		return(']');
	case '<':
		return('>');
	}
	return(0);
}


/*
 * gobble - gobble up space to the the next non-space character
 *	return space and char
 */

gobble(space)
char *space;			/* gobbled space returned here */
{
	register int c;		/* non-space character returned here */
	register char *t;

	t = space;
	while(isspace(c = mygetc(readtext))) {
		*t++ = c;
	}
	*t = '\0';
	return(c);
}

/*
 * gulp string up thru newline 
 */

char *
gulpnl(space)
char *space;
{
	register char *w;
	for(w=space; *w;) {
		if(*w++ == '\n')
			break;
	}
	return(w);
}		


post_command()
{
	register int c;		/* non-space character returned here */
	while((c = mygetc(readtext)) != EOF) {
		if (c == '\n')
			break;
		else if(!isspace(c)) {
			errexit(1,"prefer command must end with newline\n");
		}
	}
}
	
/*
 * token
 *
 * tokenizer - returns:
 *	T_COMMAND	anything after a |
 *	T_REFER		a line starting with % to next % or right delim
 *	T_WORDS		anthing
 *	T_OPEN		left delimeter
 *	T_CLOSE		right delimeter
 *	T_WORD		anything else (non-space)
 */

TOKEN
get_token(value,right)
char *value;			/* token value */
char right;			/* expected right delimeter */
{
	register int c;		/* current character */
	register char *v;	/* current position in *value */
				/* state of tokenizer */

	enum {BEGIN, ANYTHING, REFER, COMMAND, WORD, ENDREFER, CONTINUE} state;


	if(right == 0) state = BEGIN;
	else state = ANYTHING;
	v = value;

	while((c = mygetc(readtext)) != EOF) {
		switch(state) {

		case BEGIN:
			switch(c) {
			case '|':
				*v = '\0';
				ungetc(c,readtext);
				return(T_COMMAND);
			case '(':
			case '[':
			case '{':
			case '<':
				*v++ = c;
				*v = '\0';
				return(T_OPEN);
			case ' ':
			case '\n':
			case '\t':
				continue;
			default:
				*v++ = c;
				state = COMMAND;
				continue;
			}

		case ANYTHING:
			switch(c) {
			case ' ':
			case '\n':
			case '\t':
				continue;
			case '%':
				*v++ = c;
				state = REFER;
				continue;
			case ')':
			case ']':
			case '}':
			case '>':
				if(c == right) {
					*v++ = c;
					*v = '\0';
					return(T_CLOSE);
				}
			default:
				*v++ = c;
				state = WORD;
				continue;
			}
		
					
		case COMMAND:
			switch(c) {
			case '|':
			case ' ':
			case '\n':
			case '\t':
			case '(':
			case '[':
			case '{':
			case '<':
				*v = '\0';
				ungetc(c,readtext);
				return(T_COMMAND);
			default:
				*v++ = c;
				continue;
			}
	

		case WORD:
			switch(c) {
			case ' ':
			case '\n':
			case '\t':
			case '%':
				*v = '\0';
				ungetc(c,readtext);
				return(T_WORD);
			case ')':
			case ']':
			case '}':
			case '>':
				if(c == right) {
					*v = '\0';
					ungetc(c,readtext);
					return(T_WORD);
				}
			default:
				*v++ = c;
				continue;
			}
			

		case REFER:
			switch(c) {
			case '\n':
				state = ENDREFER;
				continue;
			case '\\':
				state = CONTINUE;
				continue;
			case ')':
			case ']':
			case '}':
			case '>':
				if(c == right) {
					*v++ = '\n';
					*v = '\0';
					ungetc(c,readtext);
					return(T_REFER);
				}
			default:
				*v++ = c;
				continue;
			}

		case CONTINUE:
			state = REFER;
			if(c != '\n') {
				*v++ = '\\';
				*v++ = c;
			}
			continue;
				

		case ENDREFER:
			ungetc(c,readtext);
			if(c == '%') {
				*v++ = '\n';
				*v = '\0';
				return(T_REFER);
			}
			if(!isspace(c)) *v++ = ' ';
			state = REFER;
			continue;
		}
	}
	haveeof++;
	return(T_EOF);
}


/*
 * token_err - print error message and exit
 */ 

token_err(type,s,tok)
TOKEN type;		/* token type in error */
char	*s,		/* error message */
	*tok;		/* error token */
{
	if(type == T_EOF)
		errexit(1, "Unexpected EOF\n");
	else
		errexit(1,s,tok);
}


/*
 * start_awk
 * open awk and handshake to determine one pass or two
 * (listing an entire bibliography is always one pass)
 */

start_awk(bflag)
bibflag bflag;			/* flag complete bibliography listing (as 
					to normal citations and reference list */
{
	char awkcmd[maxstr];	/* line to send to awk */
	struct stat buf;	/* for stating style file */

/*
 * open up awk style script
 * checking to see style is one of the standards or user supplied
 */

	strcpy(awkcmd,stylename);
	if(stat(awkcmd,&buf)) {
		strcpy(awkcmd,libpath(PLIB));
		strcat(awkcmd,STYLEDIR);
		strcat(awkcmd,stylename);
	}
	awkopen(awkcmd,&readbib,&writebib);


/*
 * handshake, sending appropriate flags
 */
	fputs(LETS_GO,writebib);
	if(nroff)
		fputs(" nroff",writebib);
	else
		fputs(" troff",writebib);
	if(rp)
		fputs(" rp",writebib);
	else
		fputs(" tm",writebib);
	putc('\n',writebib);
	fflush(writebib);

/*
 * read back flag from awk that tells 1 pass or 2;
 * divert text to temp file if two pass
 */
	if(fgets(awkcmd,maxstr,readbib) == NULL)
		errexit(1,"start_awk: EOF or error from awk\n");
	if(strcmp(awkcmd,TWOPASS) == 0) {
		if(bflag != BIBLIST) {
			pass = 1;
			savefile = writetext;
			divertname = mktemp(PREFERTMP);
			writetext = fopen(divertname,"w");
		}
	}
	else pass = 0;
	awkstate = OK;
}


/*
 * citation - process |reference():
 *	use search keys to lookup a complete reference in a bibliographic
 *	database; supplement citation with additional %refer lines;
 *	send all this to an awk for formatting
 */

citation()
{
	char	keys[MAXTOK];		/* store keys here */
	int missing;			/* missing flag */

/*
 * signal start of citation to awk
 */
	if(awkstate != OK) start_awk(NORMAL);
	missing = 0;
	if(pass != 2) {
		fputs(BEGIN_CITE,writebib);
		putc('\n',writebib);
	}

/*
 * after left delmeter, get keys and search for corresponding reference
 */
	leftdelim;
	*keys = '\0';
	while((type = get_token(tok,right)) == T_WORD ) {
		strcat(keys,tok);
		strcat(keys," ");
	}
	if(*keys != '\0') {
		missing = getref(keys);
	}

/*
 * send %lines to awk to supplement citation
 */
	if(type == T_REFER) {
		refline();
		while((type = get_token(tok,right)) == T_REFER) {
			refline();
		}
	}

/*
 * get right delimeter and signal the end of citation
 */
	if (type != T_CLOSE)
		token_err(type,"Expecting right delimeter, got %s\n",tok);

	fputs(END_CITE,writebib);
	if (pass == 2)
		putc('2',writebib);
	putc('\n',writebib);
	fflush(writebib);

		/* if two pass, write marker in diverted text */

	if(pass == 1 && missing == 0)
		fprintf(writetext,"|%s[]",REF_CITE);
}


/*
 * pass a %refline to awk, processing %also lines appropriately
 */

refline()
{
	if(strncmp(ALSOBEGIN,tok,strlen(ALSOBEGIN)) == 0) {
		fputs(tok,writebib);
		while((type = get_token(tok,right)) == T_REFER) {
			if(strncmp(ALSOEND,tok,strlen(ALSOEND)) == 0) {
				fputs(tok,writebib);
				fflush(writebib);
				break;
			}
			else if(strncmp(ALSO,tok,strlen(ALSO)) == 0)
				seealso(tok);
			else fputs(tok,writebib);
		}
	}
	else if(strncmp(ALSO,tok,strlen(ALSO)) == 0) {
		fputs(ALSOBEGIN,writebib);
		putc('\n',writebib);
		seealso(tok);
		fputs(ALSOEND,writebib);
		putc('\n',writebib);
		fflush(writebib);
	}
	else
		fputs(tok,writebib);
}


/*
 * get reference corresponding to keywords and send it to awk
 */

int
getref(keys)
char *keys;
{
	char	*ref;			/* points to complete reference */

	if((ref = locate(keys, INDEX)) == NULL) {
		fprintf(stderr,"Can't open database for %s\n",keys);
		fprintf(writebib,"%s not_found\n",TYPE);
		return(1);
	}
	if(*ref != NULL) {
		if(multiple(ref)) {
			fprintf(stderr,"multiple references for %s\n",keys);
		}
		refwrite(ref);
		free(ref);
		return(0);
 
	}
	else {
		fprintf(stderr,"no reference found for %s\n",keys);
		fprintf(writebib,"%s not_found\n",TYPE);
		free(ref);
		return(1);
	}
}


/*
 * check for multiple new references signalled by two newlines in a row
 */

multiple(ref)
char *ref;
{
	register char *s;

	for(s = ref; *s; s++) {
		if (*s == '\n') {
			if(strncmp(s,"\n\n%",3) == 0) {
			if(prmult) fprintf(stderr, "multiple references>>%s<<\n", ref);
				*(++s) = '\0';
				return(1);
			}
		}
	}
	return(0);
}

/*
 * refwrite - write out a single reference to awk
 */ 


refwrite(ref)
char *ref;
{
	register char *s, c;
	char *t, sav;
	int also;

	also = 0;
	for(s = ref; *s; s++) {
		if(*s == '\n') {
			c = *(s+1);
			if((c  != '%') && c) {
				if(!isspace(c)) {
					fputc(' ',writebib);
					continue;
				}
				continue;
			}
		}
		if(*s == '%') {
			if(strncmp(ALSOBEGIN,s,strlen(ALSOBEGIN)) == 0)
				also++;
			else if(strncmp(ALSOEND,s,strlen(ALSOEND)) == 0)
				also--;
			else if(strncmp(ALSO,s,strlen(ALSO)) == 0) {
				for(t = s+1; *t && *t != '%'; ) {
					if(*t++  == '\n')
						if(*t == '\n')
							break;
				}
				sav = *t;
				*t = '\0';
				if(also == 0) {
					fputs(ALSOBEGIN,writebib);
					putc('\n',writebib);
				}
				seealso(s);
				if(also == 0) {
					fputs(ALSOEND,writebib);
					putc('\n',writebib);
				}
				fflush(writebib);
				*t = sav;
				s = t-1;
				continue;
			}
		}
		putc(*s,writebib);
	}
}



/*
 * seealso - hunt for keywords in the "see also list"
 */

seealso(tok)
register char *tok;
{
	char key[MAXTOK];
	register char *k;

	*key = '\0';
	if(k = strtok(tok+strlen(ALSO)+1," \t\n")) {
		strcpy(key,k);
		strcat(key," ");
		while(k = strtok((char *)0," \t\n")) {
			strcat(key,k);
			strcat(key," ");
		}
		if(*key != '\0') {
			getref(key);
		}
	}
	
}
		


/*
 * end_citations - end a list of citations: read back citation
 * list from awk
 */

end_citations(punc)
int punc;	/* punctuation following or preceeding citation list */
{
	register int l, k, j;

	fputs(END_LIST,writebib);
	if(pass == 1)
		putc('1',writebib);
	if(punc)
		fprintf(writebib," %c",(char)punc);
	putc('\n',writebib);
	fflush(writebib);

	while(fgets(awkline,MAXTOK,readbib)) {
		l = strlen(awkline);
		if(l >= (j=strlen(END_LIST)+1)) {
			k = l-j;
			if(strncmp(END_LIST,&awkline[k],j-1) == 0) {
				if(k != 0) {
					awkline[k] = '\0';
					fputs(awkline,writetext);
				}
				return;
			}
		}
		fputs(awkline,writetext);
	}
	errexit(1,"end_citations: EOF or error from awk\n");
}


/*
 * style - process |reference_style()
 * 	select a reference formatting style and set style flags
 */

style()
{

/*
 * get left delimeter, then style name and options, then right delimeter
 */
	leftdelim;
	if ((type = get_token(tok,right)) == T_WORD) {
		if(strcmp(tok,"same")) {
			strcpy(stylename,tok);
		}
		while ((type = get_token(tok,right)) == T_WORD) {
			if(strcmp(tok,"nroff") == 0) {
				nroff = 1;
			}
			else if(strcmp(tok,"troff") == 0) {
				nroff = 0;
			}
			else if(strcmp(tok,"rp") == 0) {
				rp = 1;
			}
			else if(strcmp(tok,"tm") == 0) {
				rp = 0;
			}
			else if(strcmp(tok,"sequence") == 0) {
				sort = 0;
			}
			else if(strcmp(tok,"sort") == 0) {
				sort = 1;
				strcpy(sortkey,tok);
			}
			else {
				sort = 1;
				strcpy(sortkey,tok);
			}
		}
	}
	else {
		token_err(type,"Expecting style name, got %s\n", tok);
	}

	rightdelim;

/*
 * finish previous citations before switching styles
 */
	if(awkstate == OK)
		biblio(END);
}


/*
 * database - process |reference_database
 * 	select a new bibliographic database
 */
 
database()
{

/* 
 * after left delimeter, get name of database, then right delim
 */
	leftdelim;
	if ((type = get_token(tok,right)) != T_WORD) {
		token_err(type,"Expecting name of database file, got %s\n",tok);
	}
	strcpy(INDEX,tok);
	strcat(INDEX,".i");
	type = get_token(tok,right);
	rightdelim;

}


/*
 * bib_list - process |reference_list()
 *	format a bibliographic database
 *	first finish processing for previous citations
 */

bib_list()
{
	static char dbstr[MAXTOK];	/* names of one or more bibs */
	
/*
 * after left delimeter get bibliography name (can skip on second pass),
 * then right delimeter
 */
	leftdelim;
	if(pass != 2) {
		*dbstr = '\0';
		while ((type = get_token(tok,right)) == T_WORD) {
			strcat(dbstr,tok);
			strcat(dbstr," ");
		}
	}
	else type = get_token(tok,right);
	rightdelim;

/* 
 * format any previous citations then put out complete bibliography
 * (unless the second pass has just begun)
 */
	if(awkstate == OK) {
		biblio(BIBLIST);
		if(pass != 2) printbib(dbstr);
	}
	else {
		printbib(dbstr);
	}
}


/*
 * biblio - process reference_placement OR
 *	finish processing citations before EOF, style change, or list command
 */

biblio(bflag)
bibflag bflag;
{

/*
 * signal bibliography to awk
 */
	fputs(BIBLIO,writebib);
	if(pass == 1)
		putc('1',writebib);
	if(bflag == END || bflag == BIBLIST)
		fputs(" no_bib",writebib);
	putc('\n',writebib);
	fflush(writebib);
	
	switch(pass) {

	/*
	 * write out bib mark to diverted text, close, and reopen
	 */
	case 1:
		pass = 2;
		if(bflag != END) {
			putc(BAR,writetext);
			if(bflag == BIBLIST)
				fprintf(writetext,"%s<>",REF_LIST);
			else
				fputs(REF_PLACE,writetext);
			putc('\n',writetext);
		}
		fclose(writetext);
		writetext = savefile;
		savefile = readtext;
		readtext = fopen(divertname,"r");
		haveeof = 0;
		return;

	/*
	 * close and unlink text and resume reading in original file
	 */
	case 2:
		fclose(readtext);
		unlink(divertname);
		readtext = savefile;
		haveeof = 0;
		pass = 0;

	/*
 	 * get formatted bibliography from awk
	 */
	case 0:
		while(fgets(awkline,MAXTOK,readbib)) {
			if(strcmp("%END_BIB\n",awkline) == 0) {
				awkclose(readbib,writebib);
				return;
			}
			fputs(awkline,writetext);
		}
		errexit(1,"biblio: EOF or error from awk\n");
	}
}


/*
 * refread - read a single reference from a bibliography file
 */
char *
refread(bfile)
FILE *bfile;
{
	static char *buf = NULL;
	static unsigned size;
	register char *bp;
	int c, nextc;

	if(buf == NULL) {
		if((buf = malloc(MAXTOK)) == NULL)
			errexit(1, "malloc error in refread\n");
		size = MAXTOK;
	}
	bp = buf;
	while(isspace(c = getc(bfile))) {
	}
	if(c == EOF)
		return(NULL);
	*bp++ = c;
	while(c = getc(bfile)) {
		if(bp-buf == size-1) {
			if((buf = realloc(buf,size+MAXTOK)) == NULL)
				errexit(1, "malloc error in refread\n");
			bp = buf+size-1;
			size += MAXTOK;
		}
		if(c == EOF)
			break;
		*bp++ = c;
		if(c == '\n') {
			if((nextc = getc(bfile)) == '\n') {
				ungetc(nextc,bfile);
				break;
			}
			ungetc(nextc,bfile);
		}
	}

	if(bp == buf)
		return(NULL);
	if(*(bp-1) != '\n')
		*bp++ = '\n';
	*bp = '\0';
	return(buf);
}

		
/*
 * printbib - list one or more bibliographic databases
 */
printbib(bibstr)
char *bibstr;			/* name of database */
{
	register char *bp, *bn;	/* ptrs to bibstr */
	FILE *bib;		/* file ptr to database */
	register char *ref;	/* points to a complete reference */

/*
 * start awk, signalling that this is an entire bib (as opposed to citations)
 */
	start_awk(BIBLIST);

/*
 * for each database; open the file, read each reference,
 * sending BEGIN and END markers
 */
	for(bp = bibstr; *bp; bp = bn) {
		if((bn = strchr(bp,' ')) != NULL)
			*bn++ = '\0';
		else break;
		if((bib = fopen(bp,"r")) == NULL) {
			fprintf(stderr,"cannot open database file %s\n",bp);
			continue;
		}

		strcpy(INDEX,bp);
		strcat(INDEX,".i");

		while(ref = refread(bib)) {
			fputs(BEGIN_CITE,writebib);
			putc('\n',writebib);
			refwrite(ref);
			fputs(END_CITE,writebib);
			putc('\n',writebib);
		}
		fflush(writebib);
		fclose(bib);
	}
	fflush(writebib);

/*
 * signal awk to format the bibliography, read it back, and print it out
 */
	fputs(WHOLEBIB,writebib);
	if(sort) {
		putc(' ',writebib);
		fputs(sortkey,writebib);
	}
	putc('\n',writebib);
	fflush(writebib);

	while(fgets(awkline,MAXTOK,readbib)) {
		if(strcmp("%END_BIB\n",awkline) == 0) {
			awkclose(readbib,writebib);
			return;
		}
		fputs(awkline,writetext);
	}
	errexit(1,"wholebib: EOF or error from awk\n");
}


/*
 * dbinclude - include entries from one or more bibliography databases
 *		in the reference section without citing in the text
 */
dbinclude()
{
	static char dbstr[MAXTOK];	/* names of one or more bibs */
	register char *bp, *bn;	/* ptrs to bibstr */
	FILE *bib;		/* file ptr to database */
	register char *ref;	/* points to a complete reference */
	char saveindex[maxstr];	/* for saving name of current index */
	int includecnt;		/* count number of included biblio entries */
	
/*
 * after left delimeter get bibliography name (or include count on second pass),
 * then right delimeter
 */
	leftdelim;
	*dbstr = '\0';
	while ((type = get_token(tok,right)) == T_WORD) {
		strcat(dbstr,tok);
		strcat(dbstr," ");
	}
	rightdelim;

	if(pass != 2) {
	
		includecnt = 0;
		strcpy(saveindex,INDEX);	/* save the current INDEX and start awk */
		if(awkstate != OK) start_awk(NORMAL);
	/*
	 * for each database; open the file, read each reference,
	 * sending BEGIN and END markers
	 */
		for(bp = dbstr; *bp; bp = bn) {
			if((bn = strchr(bp,' ')) != NULL)
				*bn++ = '\0';
			else break;
			if((bib = fopen(bp,"r")) == NULL) {
				fprintf(stderr,"cannot open database file %s\n",bp);
				continue;
			}

			strcpy(INDEX,bp);
			strcat(INDEX,".i");

			while(ref = refread(bib)) {
				fputs(BEGIN_CITE,writebib);
				putc('\n',writebib);
				refwrite(ref);
				fputs("%no_cite\n",writebib);
				fputs(END_CITE,writebib);
				putc('\n',writebib);
				includecnt++;
			}
			fflush(writebib);
			fclose(bib);
		}
		end_citations('\0');
		strcpy(INDEX,saveindex);
		if(pass == 1) {
			fprintf(writetext,"|%s[%d]\n",REF_INCLUDE,includecnt);
		}
	}
	else {
		fputs(DBINCLUDE,writebib);
		putc(' ',writebib);
		fputs(dbstr,writebib);	/* number of includes */
		putc('\n',writebib);
		fflush(writebib);
	}
}


mygetc(file)
FILE *file;
{
	int c;

	if(haveeof)
		return(EOF);
	else
		if((c = getc(file)) == EOF)
			haveeof++;
		return(c);
}

/*
 * errexit - print error message to stderr, unlink temporary file, exit
 */

errexit(n,s1,s2,s3)
int n;
char *s1;
{
	fprintf(stderr,s1,s2,s3);
	unlink(divertname);
	if(awkstate != NONE)
		awkclose(readbib,writebib);
	exit(n);
}


/*
 * awkopen - open up pipes to awk process
 */

#define	RDR	0
#define	WTR	1

awkopen(awkfile,readawk,writeawk)
char *awkfile;
FILE **readawk,**writeawk;
{
	int
		i,			/* temp for closing fd's not used */
		fromawk[2],		/* pipe file descriptors from awk */
		toawk[2],
		pid			/* pipe file descriptors to awk */
	;
	char	temp[maxstr];

/*
 * *** take care of forming pipes ***
 */
	if (pipe(fromawk) < 0)
		errexit(1, "can't open pipe from awk\n");
	if (pipe(toawk) < 0)
		errexit(1, "can't open pipe to awk\n");

/*
 * *** make awk child ***
 */
	if ((pid = fork()) == 0) {
		close(fromawk[RDR]);
		dup2(fromawk[WTR], 1);
		close(toawk[WTR]);
		dup2(toawk[RDR], 0);
		for (i=3; i<FILENUM; i++)
			close(i);

		strcpy(temp,libpath(PLIB));
		strcat(temp,SLASH);
		strcat(temp,AWK);
/*
		execlp(temp, AWK, "-R", awkfile, 0);
*/
		execlp(temp, AWK, "-f", awkfile, 0);
		errexit(1, "could not exec awk with script %s\n",awkfile);
	}
	if (pid == -1)
		errexit(1, "could not fork awk\n");

/*
 * *** make out stream pointers ***
 */
	close(fromawk[WTR]);
	*readawk = fdopen(fromawk[RDR], "r");
	close(toawk[RDR]);
	*writeawk = fdopen(toawk[WTR], "w");

}


/*
 * awkclose - close pipes to awk
 */

awkclose(read,write)
FILE *read, *write;
{
	fclose(write);
	fclose(read);
	awkstate = NONE;
}


/*
 * cleanup = after signal, get rid of temp file and close awk
 */

cleanup()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	unlink(divertname);
	if(awkstate != NONE)
		awkclose(readbib,writebib);
	exit(1);
}
