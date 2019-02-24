#
/* op codes */

#define OK	128
#define DEFINE	129
#define REDEFINE	130
#define DELETE	131
#define EOM	133
#define XMT	135
#define POP	137
#define CDIR	138
#define CMODE	139
#define OPEN	140
#define CLOSE	141
#define SEEK	142
#define READ	143
#define WRITE	144
#define TRUNCATE	145
#define LOGIN	146
#define QUIT	147
#define LIST	148
#define STATUS	149
#define TIME	150
#define UNIQUE	151
#define APPEND	152
#define ASSIGN	154
#define FCREATE	155
#define IF	156
#define FI	157
#define SKIP	158
#define DCREATE	159
#define UNLESS	161

# define OPNUM 34		/* number of legal op codes */


struct dentry{
	char	d_val;		/*ms byte of value*/
	char	d_type;		/*type see below*/
	int	d_val1;		/*ls word of value*/
	char	d_name[12];	/*file name filled with zero*/
} 
entry;

#define NTYPE	1		/*name - variable length ending in zero*/
#define WTYPE	2		/*wstore - 1 word*/
#define ETYPE	4		/*error - 2 words*/
#define RTYPE	5		/*file reference - 1 word*/
#define XTYPE	6		/*default argument*/
#define STYPE	7		/*status 69 bytes*/
#define DTYPE	8		/*directory ID - 2 words*/
#define FTYPE	9		/*file ID - 2 words*/
#define IITYPE	10		/*integer - 2 words*/
#define ITYPE	11		/*integer - 1 word*/
#define LTYPE	14		/*label - 1 char*/
#define ATYPE	15		/*account - 2 words*/
#define MAXTYPE 16		


#define ACCDEV	03000	/*device code for accounts*/


/* error codes */

#define ESIG	100		/*illegal signal code*/
#define EEOM	101		/*fell off end of message*/
#define EOP	102		/*illegal op code*/
#define EARGT	103		/*illegal arg type*/
#define EWADDR	104		/*illegal w-store addr*/
#define EEXIST	105		/*already exists*/
#define EUNDEF	106		/*does not exist*/
#define EFREF	107		/*fref on non-open file*/
#define EPWD	108		/*unknown user*/
#define EFAIL	109		/*failed for unspecified reason*/
#define EISACC	110		/*illegal use of account*/
#define EUNAV	111		/*file store not available*/
#define ELOG	112		/*user not logged in*/


struct sentry{
	int	s_val;
	int	s_val1;
	int	s_type;
}
stack[32];

#define VARNO	32		/*number of items in wstore*/
struct sentry wstore[VARNO];


struct link{
	int	*l_link;	/*label for return from macro*/
	char	*l_mp;		/*current position in message*/
	char	*l_ms;		/*start position of message*/
} 
mlink[4];

struct	utmp{
	char	u_name[8];
	char	u_tty;
	char	u_ifill;
	int	u_time[2];
	int	u_ufill;
} 
utmp;

struct {
	int word[1];
};
struct {
	char byte[1];
};

#define OPENS	15
#define NSIZ	128

int buf[256];
char nbuf[NSIZ];
char mbuf[514];
char uniqb[128];
char	*up;
int	statbuf[50];
struct {
	char even, odd;
};
char resp[100];
char openf[OPENS];
char argspec[OPNUM][3];
char nodef[MAXTYPE];
char unixid[128];

int	fn;			/*tiu file*/
int	chan;			/*tiu channel number*/
char	*rp;			/*current position in response*/
char 	*mp;			/*current position in message*/
char	*mlast;			/*end of message*/
struct sentry *sp;		/*top of stack*/
struct sentry *asp;		/*misc. stack pointer*/
struct link *lp;		/*top of link list*/
int 	ig;			/*label for SKIP, -1 for IF*/
int	sum;			/*sumcheck of last file transfer*/
int	length;			/*length of last file transfer*/
int	login;			/*set when logged in*/
int	msiz;			/*maximum size of message to user*/
int	panic;			/*nz after xmt timeout*/
int	trace;			/*non-zero if trace print*/
struct	{			/*info returned from wait()*/
	char	w_pno;		/*process number*/
	char	w_stat;		/*return status*/
}
pstat;
int	uid;
char	*tiu;
struct{
	char	lobyte;
	char	hibyte;
};
#define DELAY	60
#define ROOTUSR	0
#define EGOOD	0
#define EBAD	1
int	status;
struct ustat{
	int	mode;
	int	type;
	int	ftype;
	int	uid;
	int	gid;
	long	size;
	long	modtime;
	long	acctime;
}us;
			/* File Access permissions */
#define R	04
#define W	02
#define X	01
			/* Types of files */
#define FFILE	0
#define CHR	1
#define DIR	2
#define BLK	3
#define REC	4
char	*netpwd;
