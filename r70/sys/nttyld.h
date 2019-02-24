/*
 * new tty line discipline processing
 */

struct nttyld {

	struct	queue *nt_outq;		/* for echoed characters */
	struct	queue *nt_readq;	/* for processed input characters */

#define NT_NIN	256
	char	nt_in[NT_NIN];	/* cooked mode input buffer */
	short	nt_nin;		/* current number of characters in nt_in */

	short	nt_flags;	/* ioctl [gs]ettable modes */
	short	nt_state;	/* internal state, not visible externally */
	char	nt_col;		/* printing column of device */
	char	nt_delct;	/* number of delimiters in read queue */

	char	nt_erase;	/* erase character */
	char	nt_kill;	/* kill character */
	struct	tchars nt_tchr;
#define nt_intrc	nt_tchr.t_intrc
#define nt_quitc	nt_tchr.t_quitc
#define nt_startc	nt_tchr.t_startc
#define nt_stopc	nt_tchr.t_stopc
#define nt_eofc		nt_tchr.t_eofc
#define nt_brkc		nt_tchr.t_brkc

	/*
	 * local:
	 */
	short	nt_trash;	/* nt_nin after last write() data processed */
	short	nt_rocol;	/* nt_col when nt_nin==0 */
	short	nt_local;	/* local mode bits */
	short	nt_lstate;	/* local state bits */

	struct	ltchars nt_ltchr;
#define nt_suspc	nt_ltchr.t_suspc
#define nt_dsuspc	nt_ltchr.t_dsuspc
#define nt_rprntc	nt_ltchr.t_rprntc
#define nt_flushc	nt_ltchr.t_flushc
#define nt_werasc	nt_ltchr.t_werasc
#define nt_lnextc	nt_ltchr.t_lnextc
#define nt_urotc	nt_luchr.t_urotc
};


/*
 * values for nt_state:
 */
#define NT_STOP		001	/* stopped by stop character */
#define NT_ESC		002	/* have just seen \ or lnext character */
#define NT_CR		004	/* turning LF into CRLF */
#define NT_USE		010	/* nttyld structure in use */
#define NT_CASE		020	/* adding \ for upper-case-only terminals */
#define NT_ECHO		040	/* echoed something for ntin */


/*
 * default local special characters (struct ltchars)
 */
#define CSUSP	CTRL('z')
#define CDSUSP	CTRL('y')
#define CRPRNT	CTRL('r')
#define CFLUSH	CTRL('o')
#define CWERAS	CTRL('w')
#define CLNEXT	CTRL('v')


/*
 * Ioctl message format.
 */

struct ntioc {
	int	command;

	union {
		struct sgttyb	sg;
		struct tchars	tchr;
		struct ltchars	ltchr;
		short		local;
		char		chr[16];
	} arg;
};
