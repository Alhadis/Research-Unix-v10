/*
 *	Note that any mods made to this enum must be matched by mods made
 *	to the token dope table 'tkdopes' defined in 'readonly.c'.
 */
typedef enum
{
	tk_and,		/* & */
	tk_andand,	/* && */
	tk_auto,	/* auto */
	tk_bar,		/* | */
	tk_barbar,	/* || */
	tk_break,	/* break */
	tk_case,	/* case */
	tk_cchar,	/* constant char */
	tk_cdouble,	/* constant double */
	tk_cfloat,	/* constant float */
	tk_char,	/* char */
	tk_cint,	/* constant int */
	tk_clong,	/* constant long */
	tk_colon,	/* : */
	tk_comma,	/* , */
	tk_continue,	/* continue */
	tk_default,	/* default */
	tk_do,		/* do */
	tk_dot,		/* . */
	tk_double,	/* double */
	tk_else,	/* else */
	tk_enum,	/* enum */
	tk_eof,		/* eof */
	tk_eq,		/* = */
	tk_eqeq,	/* == */
	tk_extern,	/* extern */
	tk_float,	/* float */
	tk_for,		/* for */
	tk_goto,	/* goto */
	tk_grt,		/* > */
	tk_grteq,	/* >= */
	tk_grtgrt,	/* >> */
	tk_if,		/* if */
	tk_int,		/* int */
	tk_less,	/* < */
	tk_lesseq,	/* <= */
	tk_lessless,	/* << */
	tk_lsminus,	/* <- */
	tk_long,	/* long */
	tk_maxof,	/* maxof */
	tk_minof,	/* minof */
	tk_minus,	/* - */
	tk_minusgrt,	/* -> */
	tk_minusminus,	/* -- */
	tk_name,	/* name */
	tk_not,		/* ! */
	tk_noteq,	/* != */
	tk_null,	/* null */
	tk_offcurly,	/* } */
	tk_offround,	/* ) */
	tk_offsquare,	/* ] */
	tk_oncurly,	/* { */
	tk_onround,	/* ( */
	tk_onsquare,	/* [ */
	tk_percent,	/* % */
	tk_plus,	/* + */
	tk_plusplus,	/* ++ */
	tk_question,	/* ? */
	tk_register,	/* register */
	tk_return,	/* return */
	tk_semicolon,	/* ; */
	tk_set,		/* set */
	tk_short,	/* short */
	tk_sizeof,	/* sizeof */
	tk_slash,	/* / */
	tk_static,	/* static */
	tk_string,	/* ".." */
	tk_struct,	/* struct */
	tk_switch,	/* switch */
	tk_tilda,	/* ~ */
	tk_times,	/* * */
	tk_typedef,	/* typedef */
	tk_union,	/* union */
	tk_unsigned,	/* unsigned */
	tk_uparrow,	/* ^ */
	tk_void,	/* void */
	tk_while,	/* while */
}
	tokens;

typedef	set tokens	tkset;

struct token
{
	tokens	t_what;
	int	t_lineno;
	st_node	*t_filename;
	data	t_value;
	token	*t_next;
};

extern token	*tok_list;

#if	!defined(TOKEN_CALLS) && !defined(TOKEN_INTRIN)
#define	get_token()	(tok_list = (ttmp = tok_list == NULL ? tok_fill() : tok_list)->t_next, ttmp)
#define	unget_token(p)	(ttmp = tok_list, (tok_list = (p))->t_next = ttmp)
#else
extern token	*get_token();
extern void	unget_token();
#endif

#define	sync(t)		(global_filename = (t)->t_filename, global_lineno = (t)->t_lineno)

#define	TOK_STASH	64
