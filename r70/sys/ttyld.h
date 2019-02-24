/*
 * TTY line discipline processing
 */
struct ttyld
{
	short	t_flags;	/* mode, settable by ioctl call */
	short	t_state;	/* internal state, not visible externally */
	char	t_delct;	/* number of delimiters in input q */
	char	t_col;		/* printing column of device */
	char	t_erase;	/* erase character */
	char	t_kill;		/* kill character */
	struct	tchars t_chr;
};

#define	CTRL(c)	((c)&037)

/* default special characters */
#define	CERASE	'\010'
#define	CEOT	CTRL('d')
#define	CKILL	'@'
#define	CQUIT	034		/* FS, cntl shift L */
#define	CINTR	0177		/* DEL */
#define	CSTOP	CTRL('s')
#define	CSTART	CTRL('q')
#define	CBRK	0377

/* define partab character types */
#define	ORDINARY	0
#define	CONTROL		1
#define	BACKSPACE	2
#define	NEWLINE		3
#define	TAB		4
#define	VTAB		5
#define	RETURN		6

/* states */
#define	TTSTOP	01		/* stopped by CTL S */
#define	TTESC	02		/* have just seen \ */
#define	TTCR	04		/* turning LF into CRLF */
#define	TTUSE	010		/* this structure in use */
#define	TTBLOCK	020		/* have sent tandem STOP */
