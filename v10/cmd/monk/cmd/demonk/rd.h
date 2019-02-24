#define register

/* database files */

#ifdef NOT_DEMONK
#define	DB_PATH		"../db"			/* default */
#endif

#define	SLASH_STRING	"/"
#define	COMPRESSED	1
#define	STANDARD	2

#define	DB_ATTRIBUTE	"attrib"
#define	DB_GLOBAL	"global"
#define	DB_MACRO	"macro"

#define	DB_COMP_EXT	".comp"		/* extension for compressed files */
#define	DB_SAMPLE_EXT	".sample"	/* extension for type samples */

/* hash tables: _TABLE is table number, _ENTRIES is maximum # of entries */
					/* database tables */
#define	ATT_TABLE	0	/* attributes - indent, blank.lines, new.page */
#define	ATT_ENTRIES	200
#define	ENV_TABLE	2	/* environments, associates -		      */
#define	ENV_ENTRIES	200	/*		paragraph, address, chapter   */
					/* run-time tables */
#define	STACK_TABLE	3	/* stacking attributes - restore environments */
#define	STACK_ENTRIES	100
#define	DEFINE_TABLE	1	/* definitions - read token, substitute key */
#define	DEFINE_ENTRIES	200
#define	VAR_TABLE	4	/* monk saves strings, numbers for db author*/
#define	VAR_ENTRIES	100

#define	SETUP_TIME	0
#define	RUN_TIME	1

/* database_setup keywords: definition primitives */

#define	SAFESIZ		BUFSIZ*4
					/* options in attribute definition */
#define	STACKING	"stack"		/* attribute is stacking */
#define	DEFAULT		"default"	/* initialization of stack */
#define	INITIALIZE	"init"		/* init stack & output attrib case */

#define	COLON		':'		/* internal environments begin with : */
#define	INIT_STACK	":i_stack"		/* initialize internal stack */
#define	INIT_TROFF	":init_troff"		/* pass to initialize troff */
#define	MONK_TEXT	":common" 		/* monk - beg&end usertext */
#define	MONK_FILE	":common_newfile" 	/*      - beg&end each file */
#define	DOC_TEXT	":document"		/* doc - beg&end of usertext */
#define	MONK_SAVE	":common_save"		/* monk - counters for parts */
#define	MONK_RESTORE	":common_restore"
#define	DOC_SAVE	":document_save"	/* doc - counters for parts */	
#define	DOC_RESTORE	":document_restore"
#define	U_NL_NL		":common_blankline"	/* handles multiple user nls */
#define	U_NL_WHITE	":common_newline_whitespace"
				/* handles whitespace following newline */

#define	SPECIAL_IN_TROFF	'_'	/* first character of all special */
#define	FILENAME	"__FILE__"
#define	LINENUMBER	"__LINE__"
#define	PARENT		"__PARENT__"
#define	BLANKSTRING	" "

#define	DEF_ARGUMENT	'$'
#define	REGEX_CHAR	'$'
#define	XPAND_CHAR	'*'
#define	FIXED_ARG	2
#define	VAR_ARG		3
#define	REGEX_ARG	4
#define	XPAND_ARG	999

#define	EMBED_IN_TROFF	'|'
#define	FOR_LOOP	"for"
#define FOR_ARGS	"in"
#define	TOP_OF_FOR_LOOP	"|for "		/* used for reconstruction - must match
					EMBED_IN_TROFF, FOR_LOOP and FOR_ARGS */
#define IF		"if"
#define	IF_VALUE	"ifvalue"
#define	IF_NOTVALUE	"ifnotvalue"

#define	MONKSAVE	"monksave"	/* store string from user text */
#define	MONKREMEMBER	"monkremember"	/* process and write string to output */

#define	PUSH		"push"
#define	POP		"pop"

#define	ASSOCIATE	"associate"
#define	ATTRIBUTE	"attribute"
#define	COMMENT		"comment"
#define	DEFINE		"define"
#define	ENVIRONMENT	"environment"
#define	ASSOC_ENV	"associate or environment"
#define	INSERT		"insert"
#define	MACRO		"macro"
#define	PART		"part"
#define	SOURCE		"source"

/* integer equivalents for commands */
#define	NOT_ACOMMAND	0
#define	N_BEGIN		1	/* BEGIN and END are user_text commands */
#define	N_END		2
#define	INFO_COMMANDS	3	/* top of file commands >= 3 */
#define	N_ASSOCIATE	4	/* definitions:	associate */
#define	N_ATTRIBUTE	5	/* 		attribute */
#define	N_ENVIRONMENT	6	/* 		environment */
#define	N_MACRO		7	/* 		macro */
#define	N_COMMENT	8
#define	N_DEFINE	9
#define	N_IF		10
#define	N_IF_VALUE	11
#define	N_IF_NOTVALUE	12
#define	N_FOR_LOOP	13
#define	N_PUSH		14
#define	N_POP		15
#define	N_SOURCE	16	/* insert a file without processing monk cmds */
#define	MAKE_COMMANDS	17	/* commands which find document type >= 11 */
#define	N_INSERT	17	/* insert a file processing monk commands */
#define	N_MAKE		18	/* specify document type */
#define	N_PART		19	/* specify parent */

/* database_use keywords */

#define	BEGIN		"begin"
#define	END		"end"
#define	MAKE		"make"

#define	BEGIN_PRIMITIVE	'|'
#define	LIST_SEP	';'
#define	ITEM_SEP	','
#define	NEW_LINE	'\n'
#define	BLANK		' '
#define	TAB		'\t'

#define	NSUBSTITUTE	4
				/* argument specifies source of text
					0 = donot write it out */
#define	NO_WRITE	0
#define	U_TEXT		1	/* source for text - enduser file */
#define	MONK_DB		2	/* source for text - monk databases */
#define	FORCE		3	/* unconditionally spew text (omit test \n\n) */

				/* specifies to whom messages are sent */
#define	U_MSG		001	/* messages to enduser
					must agree with U_MSG in warn.c */
#define	U_REDEF_MSG	001	/* redefinition message to enduser */
#define	U_SYNTX_MSG	002	/* syntax error messages to enduser */
				/* when next character is not as expected */
#define	GOBBLE		010	/* gobble text looking for character */
#define	MESSAGE		020	/* print error messages as specified in 007 */

#define	NO_SPACE	FALSE	/* space permitted in parsing: b4 token */
#define	OK_SPACE	TRUE

#define	END_NOW		0	/* when to end a sub-environment */
#define	END_AT_END	1

#define	TRUE		1
#define	FALSE		0
#define	EXIT_ON_FAIL	1
#define	EXIT		1
#define	NO_EXIT		0
#define	NOT(x)		(int) x - 1

#define	STR_SIZE	48

#define	ATOI		060

#define	TOP_LEVEL	1
#define	SUB_LEVEL	-1

/* iswhite: isspace but not '\n' */
#define	iswhite(c)		(c == BLANK || c == TAB)

#ifdef FIGURE_IT_OUT
#define	mygetc(file)		((c = getc(file)) == '\n') ?\
					linenumber(INCREMENT), c : c

#define	myungetc(c, file)	(c == '\n') ?\
					linenumber(DECREMENT),ungetc(c, file) :\
					ungetc(c, file)
#endif

/* hash data for attribute definition */

struct attrib_info {				/* see dbcompress.h */
	short	stacking;
	struct attrib_case	*firstcase;
};

struct attrib_case {			/* see dbcompress.h */
	short	nvalues;	/* number of arguments - $* counts as one */
	short	expanding;	/* are there any cases with expanding values? */
	short	special;	/* are there any special cookies in troff,
						e.g. __LINE__, __FILE__ */
	short	allocated;
	char	*troff;
	struct value	*value;
	struct attrib_case	*next;
};

/* hash data for each database definition:
	struct definition pointing to linked list of def_elements
 */
struct value {			/* see dbcompress.h */
	char	*value;			/* value identifier */
	short	type;			/* fixed, variable, expanding value */
	struct value	*next;
};

struct def_element {			/* see dbcompress.h */
	char	*attribute;
	char	*troff;
	struct value	*value;
	short	special;
	short allocated;
	short	stacking;
	struct cond_def_el	*cdl;
	struct def_element	*next;
};

struct definition {			/* see dbcompress.h */
	char	*name;
	struct value	*values;
	struct def_element	*begin_def;
	struct def_element	*end_def;
	struct definition	*sub_def;
	struct environment	*instance;	/* envir of this def */
};

/* state information(absolute values) stacked on entering environment;
	on leaving an environment absolute troff strings from previous environment
	used to restore that previous environment
 */

struct state {
	char	*attribute;		/* attribute to restore, e.g. `indent' */
	struct value	*value;		/* absolute argument, eg. `30' */
	short	allocated;		/* space allocated by malloc */
	struct state	*previous;	/* previous instance of this attribute
								to restore */
	struct state	*next;		/* next state in this environment */
};

struct statestack {
	struct state	*first;
	struct state	*last;
};

struct environment {
	struct definition	*def;
	struct state	*state_list;
	struct environment	*lastinstance;	/* last of this envir envname */
	struct environment	*previous;	/* previous envir in time */
	char	*envname;
	char	*filename;
	short	linenumber;	
	char	how_to_end;			/* how to end current envir */
};

struct conditional {
	short	type;
	char	*attribute;
	struct value	*value;
	char	close_delim;
	char	*newp;
};

struct cond_def_el {
	short	type;
	char	*attribute;
	struct value	*value;
	struct def_element	*next_on_fail;
};

struct loop {
	char	loopchar;
	struct value	*args;
	struct value	*current;
	char	close_delim;
	char	*newp;
};

struct buffer {
	char	*start;
	char	*current;
	char	*empty;
	char	*end;
};

struct valu_str {
	struct value	*value;
	char	*newp;
};

struct page_layout {
	short	top_margin;
	short	bottom_margin;
	short	left_margin;
	short	right_margin;
	char	*l_page_header;
	char	*c_page_header;
	char	*r_page_header;
	char	*l_page_footer;
	char	*c_page_footer;
	char	*r_page_footer;
};

struct got_token {
	char	*newp;
	char	delim;
};

struct strings {
	char	*token;
	char	*newp;
};

struct	add_att {
	char	*defname;
	struct value	*value;
	struct add_att	*next;
};

struct	init_def {
	char	*attribute;
	short	stacking;
	struct add_att	*add_att;
};

FILE	*fopenncheck(), *fopendb(), *parentopen();
struct attrib_case	*match_case();
struct def_element	*copy_def_el(), *mk_def_el(), *merge_def_el();
struct def_element	*trans_list(), *xpand_loop_valu();
struct definition	*get_envir();
struct definition	*read_list();
struct environment	 *init_envir(), *do_envcommands(), *make_type();
struct environment	*r_userfile(), *r_usertop(), *r_usertext();
struct environment	*force_envir(), *begin_file(), *end_file();
struct environment	*init_db_env(), *notacommand();
struct environment	*begin_envir(), *assoc_envir(), *do_subenv();
struct environment	*mk_envir(), *do_envir(),*do_end_envir(), *end_envir();
struct init_def	*add_att_type();
struct conditional	*isitaconditional();
struct cond_def_el	*readacond();
struct definition	*match_assoc();
struct loop	*readaloop(), *isitaloop();
struct strings	*parse_token(), *strarg(), *rename_arg();
struct value	*make_value(), *read_values(), *copy_valu();
struct valu_str	*parse_values();
short	match_arg(), cmp_valu();
char	*fillin_loop_valu(), *replace_args(), *bufmalloc(), *read_dbtype();
char	*r_fmtcommands(), *get_file_name(), *add_path();
char	*read_token(), *read_until(), *checkputs(), *checkif(), *trans_state();
char	*r_buftoken(), *r_bufarg(), *read_filename(), *read_env();
char	*str_atype(), *str_aspecial(), *str_avalue(),*str_vtype();
char	*whichargs(), *xpall(), *copy_text_loop(), *get_parent_name();
char	*mymalloc(), *myrealloc(), *strconcat(), *tmpstrcat();
void	make_case(), bubble_case(), dodef(), nstates();
void	warn_user(), warn_db(), warn_me();
void	nl_nlgobble(), map_args(), u_nl_nl(), u_nl_white(), myungetc();
void	read_attribute(), read_comment(), read_define();
void	read_macro(), read_word();
void	copy_userfile(), get_path(), notstacking(), make_doc();
void	clear_parent_name(), read_databases(), do_dbcommands();
char	*strcpy(), *strncpy(), *strcat();
