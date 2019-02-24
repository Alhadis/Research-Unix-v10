/*
 *	Messages exchanged between this program and system-V rexec'd one
 *
 * The general form of a message is:
 *	a fixed size control portion
 *	a variable size data portion
 * The first two bytes of the control message give a length of the
 * variable portion. First byte is low 8 bits, second byte high bits.
 *
 */

struct rem_req {
	short	r_length ;		/* length of variable data portion */
	char	r_type ;		/* message type, see defines below */
	char	r_seq ;			/* message sequence number */
	short	r_uid ;			/* effective uid of sender */
	short	r_gid ;			/* groupid of sender */
	short	r_file ;		/* primary file to be referenced */
					/* (either a directory or regular,
					 *  depending of type of request) */
	short	r_mode ;		/* file open mode, or new mode */

	union {
	   struct {		/* ROPEN type message */
		short	r_arg ;		/* creation mode */
		short	r_flag ;	/* set according to defines below */
			} ropen ;
	   struct {		/* RREAD type message */
		short	r_X1 ;		/* filler */
		unsigned short	r_count ;/* number of bytes to read */
		long	r_offset ;	/* offset in file to start */
		long	r_lastr ;	/* last logical block read */
			} rread ;
	   struct {		/* RWRITE type message */
		short	r_X1 ;		/* filler */
		unsigned short	r_count ;/* number of bytes to transfer */
					/* (NOTE - better be same as r_length) */
		long	r_offset ;	/* offset in file to start */
		long	r_limit ;	/* max block number allowed */
			} rwrite ;
	   struct {		/* RUTIME type message */
		short	r_tod ;		/*  1 = use system clock */
		short	r_X1 ;
		long	r_atime ;	/* access time */
		long	r_mtime ;	/* modification time */
			} rutime ;
	   struct {		/* RCHOWN type message */
		short	r_nuid ;	/* new user id */
		short	r_ngid ;	/* new group id */
			} rchown ;
	   struct {		/* RMKNOD type message */
		short	r_dev ;		/* major/minor number */
			} rmknod ;
	   struct {		/* RLINK type message */
		short	r_dir1 ;	/* base of search for second path */
		short	r_nlen1 ;	/* length of second pathname */
					/* NOTE - first is r_length-r_nlen1 */
			} rlink ;
	   struct {		/* RIOCTL type message */
		short	r_cmd ;		/* command type to driver */
		short	r_arg ;		/* argument/user memory address */
			} rioctl ;
	} r_var ;
} ;


/*
 *	Messages originated by us
 *
 * General format is identical to those to us
 *
 */

struct rem_reply {
	short	s_length ;		/* length of variable data following */
	char	s_type ;		/* type of message */
	char	s_error ;		/* returned error code */
			/* from here on, these fields may be UNION'ed
			 * because no single message uses all of them.
			 * However, the standard control message length
			 * is generous enough that it doesn't matter
			 */
	short	s_file ;		/* returned file index */
	short	s_uid ;			/* owner of opened file */  
	short	s_gid ;			/* groupid of opened file */
	short	s_mode ;		/* mode of opened file */
	unsigned short	s_resid ;	/* residual data remaining on RREAD */
	unsigned short	s_count ;	/* data written on RWRITE */
	long	s_lastr ;		/* last logical block written RWRITE */
	long	s_size ;		/* file size RSEEK */
} ;

/*
 *	Message types
 *	originated by rexec'd program
 *	(requests to us to do some I/O for it)
 */
#define	RMOUNT	'a'
#define	RCOPEN	'b'
#define	RREAD	'c'
#define	RWRITE	'd'
#define	RCHDIR	'g'
#define	RSTAT	'h'
#define	RFSTAT	'i'
#define	RUTIME	'j'
#define	RCHMOD	'k'
#define	RCHOWN	'l'
#define	RMKNOD	'm'
#define	RACCESS	'n'
#define	RLINK	'o'
#define	RUNLINK	'p'
#define	RUMOUNT	'q'
#define	RSEEK	't'
#define	RCANCEL	'u'
#define	RSYNC	'v'
#define	RIOCTL	'y'
#define	RCLOSEF	'z'

/*
 *		Originated by us
 */
#define	RSIGNAL	'A'


/*
 *	Misc other defines
 */

/* 
 *	bits in r_var.ropen.r_flag
 */
#define	RREG	02		/* must be regular file */
#define	REXEC	04		/* open for execution */

/*
 *	structure-to-string and string-to-structure formats
 */
#define	F_REMREQ	"sbbssssssll"

#define	F_REMREPLY	"sbbssssssll"

/*
 *	size of string passed as control portion
 */
#define	REMSIZE		24

