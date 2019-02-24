#define snchan 1	
/* Spider channel to file system - but
also change "/dev/tiu/d?" in nfs1.c */
#define DSZ 16			/*directory entry size*/
#define DEPTH 15

int	vflag;		/* flag for key arguments */
int	nflg;		/* make file temporoary on file store */
int	iflag;
int	xflag;


int	tme[2];		/*  modified time of file  */
char	buf[512];
char	cmdbuf[512];
char	*cbp;
#define NARG	9
char	rply [128];
char	nam[DEPTH * 14 + DSZ + 2];
			/* stack for extracting or deleting directory */
struct xstk{
	char *first;	/* pointer to first title for direct. */
	char *pos;
	char *lst;	/* pter to last \*/
}stack[DEPTH];
struct stk{
	int	sff;
	char	*pos;
	int	pad;
};
struct xstk *xsp;
int	dirbuf[50];
int	ubuf[50];
int	statbuf[40];



int	nchar;
int	fn;			/*Spider network file id*/

char	*terr ;

char	op			/*op code*/;
char	pseudoop;
char	**eargv;
char	*np;		/*pointer into nam[]*/
char	*bp		/*pointer into buf[]*/;
char	*tp		/*pointer into *argv[]*/;
char	*fsp;		/* pointer to arg-modified for the filestore */
struct stk *sp;		/*pointer into stack[]*/
int	ff;			/*file being processed*/
int	code;		/*dev/tiu  parameter*/
int	wchar;
int	sum;		/*check sum of data bytes*/
int	length;			/*number of bytes transmitted*/

char	*stat1 ;

char	*ng ;

char	*nxt ;
char	*last ;


#define NTYPE	1		/* vtype for name item  */
#define WTYPE	2		/* vtype for w-store reference */
#define ETYPE	4		/* vtype for 2-word error code */
#define RTYPE	5		/* vtype for file reference */
#define XTYPE	6		/* default operand */
#define STYPE	7		/* vtype for status item */
#define DTYPE	8		/* vtype for directory entry */
#define FTYPE	9		/* vtype for file entry */
#define IITYPE	10		/* vtype for 2-word integer entry */
#define ITYPE	11		/* vtype of 1-word integer */
#define LTYPE	14		/* vtype for skip label */
#define ATYPE	15		/* vtype for account */
#define MAXTYPE	15		/* maximum value of vtype */


				/*values of user op codes */

#define OK	128
#define DEFINE	129
#define REDEFINE	130
#define DELETE	131
#define EOM	133
#define XMT	135
#define POP	137
#define CDIR	138
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
#define UNIQUE	151
#define ASSIGN	154
#define FCREATE	155
#define IF	156
#define FI	157
#define SKIP	158
#define DCREATE	159
#define UNLESS	161

# define OPNUM 33		/* number of legal op codes */

int	diroff;
int	unix;
/*
 * Current format of File Store I-node
 */
struct spstat {
	int	f_dev;
	int	f_number;
	int	f_mode;
	char	f_nlink;
	char	f_uid;
	char	f_state;
	char	f_size0;
	char	*f_size1;
	char	f_seq;
	char	f_vol;
	int	f_accnt[2];
	int	f_perm[3];
	int	f_addr[8];
	int	f_spare[8];
	int	f_dmp[2];
	long	f_acctime;
	long	f_modtime;
};
int	command,filemode,pmode;
#define DELAY 60
#define GOOD 1
#define BAD	0
#define TWLOOP	0
#define TREAD	1
#define TCHECK	2
#define TSTATUS	3
#define TSENDC	4
struct {
	char	lobyte;
	char	hibyte;
};
#define IFMT	060000
#define IFDIR	040000
#define EXPLICIT	2
/*
 * Returns from gstat
 */
#define ORDFILE		0
#define DIRECTORY 	1
#define NOTFOUND	-1

#define YES		1
#define NO		0
/*
 * Spider Network commands
 */
#define SPIDERWRITE	0
#define SPIDERREAD	1
#define SPIDERCHECK	2
#define SPIDERSTATUS	3
#define SPIDERCOM	4
#define SCOM	3
#define SDATA	1
#define SEOF	2
#define SPART	4
#define CHILD	0
struct dent{
	int	d_ino;
	char	d_name[14];
};
int	pid;
#define TEMPFILE 	02000
#define BIGFILE	01000
#define RONLY	0
#define STDOUT	1
int	updread;
#define TRUE	1
#define FALSE	0
#define UPDATE	1
#define NOUPDATE 0
int	byte;
int	simulate;
char	*mach;
int	debug;
#define RPIPE	0
#define WPIPE	1
#define EBAD	0
#define OUTPUT	1
			/* Compressed version of stat */
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
#define CHR	1
#define DIR	2
#define BLK	3
#define REC	4
char	*arg[20];
struct{
	int	hiword;
	int	loword;
};
#define dbg if(debug)fprintf
