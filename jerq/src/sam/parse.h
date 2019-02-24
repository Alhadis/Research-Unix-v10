typedef struct Addr Addr;
struct Addr{
	char	type;	/* # (char addr), l (line addr), / ? . $ + - , ; */
	union{
		Regexp	*aure;
		Addr	*auprev;	/* left side of , and ; */
	}au;
	Posn	num;
	Addr	*next;			/* or right side of , and ; */
};
#define	are	au.aure
#define	aprev	au.auprev
typedef struct Cmd Cmd;
struct Cmd{
	Addr	*addr;			/* address (range of text) */
	Regexp	*re;			/* regular expression for e.g. 'x' */
	union{
		Cmd	*cucmd;		/* target of x, g, {, etc. */
		String	*cutext;	/* text of a, c, i; rhs of s */
		Addr	*cuaddr;	/* address for m, t */
	}cu;
	Cmd	*next;			/* pointer to next element in {} */
	short	num;
	ushort	flag;			/* whatever */
	ushort	cmdc;			/* command character; 'x' etc. */
};
#define	ccmd	cu.cucmd
#define	ctext	cu.cutext
#define	caddr	cu.cuaddr
extern struct cmdtab{
	ushort	cmdc;		/* command character */
	uchar	text;		/* takes a textual argument? */
	uchar	regexp;		/* takes a regular expression? */
	uchar	addr;		/* takes an address (m or t)? */
	uchar	defcmd;		/* default command; 0==>none */
	uchar	defaddr;	/* default address */
	uchar	count;		/* takes a count e.g. s2/// */
	char	*token;		/* takes text terminated by one of these */
	int	(*fn)();	/* function to call with parse tree */
}cmdtab[];
enum Defaddr{	/* default addresses */
	aNo,
	aDot,
	aAll
};
extern Regexp	*getregexp();
extern Addr	*newaddr();
