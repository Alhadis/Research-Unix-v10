/*
 * static char ID[] = "@(#) sdp.h: 1.1 3/16/82";
 */
/* take spot audit */
#define AUDITNOW	2

/* turn audits off */
#define AUDITOFF	0

/* turn audits on */
#define AUDITON		1

/* error return from non pointer-valued functions */
#define	ERROR		(-1)

/* error return from id valued functions */

#define IDERROR		(-1l)

/* mnemonic to indicate an impossible itemid */
#define	IDNULL		(-2l)

/* turn error messages off */
#define MESGOFF		0

/* turn error messages on */
#define MESGON		1

/* successful return from non pointer-valued functions */
#define SUCCESS		1

/* permissions to change item for use */
#define	WRTN		2

/* permissions not to change item for use */
#define RNLY		0

/* macro for error conditions */
#define ERR_RET(f,r)	{FATAL(f) ; return(r) ;}
/*
 * If the user includes <stdio.h> ahead of "sdp.h," then sdp_hist will be
 * defined correctly as pointer to structure FILE; otherwise, sdp_hist will
 * be define as pointer to char.
 */

#ifdef FILE
#define MY_FILE	FILE
#else
#define MY_FILE	char
#endif

typedef	long	ITEMID ;
typedef	int	FD ;
typedef	long	BOUNDARY ;

struct SPACE				/* the structure to hold header file	*/
{
	long	version ;		/* version number in form YYMMDD	*/
	unsigned page_size ;		/* number of bytes in a page		*/
	ITEMID	maxpage ;		/* highest byte allotted (page aligned)	*/
	ITEMID	curpage ;		/* id to current page during connection	*/
	unsigned curleft ;		/* number of bytes left in current page	*/
	int	perms ;			/* whether changes are enabled		*/
	int	mode ;			/* creation mode			*/
	int	numbfile ;		/* number of files comprising space	*/
	struct	FILEINFO *fileinfo ;	/* pointer to file information array	*/
	int	numbstax ;		/* number of allottment stacks	 	*/
	struct	STAXINFO *staxinfo ;	/* pointer to stack information array	*/
	char	*head_name ;		/* name of address space		*/
	ITEMID	known_id ;		/* for user				*/
	struct	ENVIRON *environ;	/* pointer to environment		*/
	struct	LIBFILE *lib_file ;	/* pointer to file library structure	*/
	struct	SPACE *next_space ;	/* link to next space			*/
	long	sp1_hash ;		/* hash code for audits			*/
	long	sp2_hash ;		/* hash code for audits			*/
} ;

struct STATS				/* structure to hold paging statistics	*/
{
	long	uses ;			/* number of calls to sdp_use		*/
	long	faults ;		/* number of page faults		*/
	long	writes ;		/* number of faults causing writes	*/
	int	maxuse ;		/* largest number of frames in use	*/
	int	curuse ;		/* current number of frames in use	*/
} ;

struct ENVIRON				/* structure to describe memory setup 	*/
{
	int	num_frames ;		/* number of frames for swapping	*/
	unsigned frame_size ;		/* size of frames in bytes		*/
	int	empties ;		/* start of list of empty frames	*/
	int	cnct_count ;		/* spaces connected to this environment	*/
	struct	MANAGER *manp ;		/* pointer to array frame managers	*/
	struct	LIBMEM *lib_mem ;	/* pointer to memory library structure	*/
	struct	ENVIRON *next_environ ;	/* link to next environment		*/
	struct	STATS stats ;		/* holds paging statistics		*/
	long	en_hash ;		/* hash code for audits			*/
} ;

struct FILEINFO				/* structure to hold file information	*/
{
	FD	desc ;			/* file descriptor			*/
	long	leng ;			/* length of file in bytes		*/
} ;

struct STAXINFO				/* structure to hold stack information	*/
{
	ITEMID	top ;			/* itemid to top of stack		*/
	long	size ;			/* size of items in stack		*/
	long	depth ;			/* number of items in stack		*/
} ;

struct MANAGER				/* structure to manage frames		*/
{
	struct	SPACE *space ;		/* space identifier			*/
	ITEMID	page_id ;		/* byte offset in space			*/
	long	usecount ;		/* number of use calls on this id	*/
	int	ref_chng ;		/* reference/change flag		*/
	int	forward ;		/* index of next manager		*/
	int	backward ;		/* index of previous manager		*/
	char	*frame_pntr ;		/* pointer to frame to be managed	*/
	long	mn_hash ;		/* hash code for audits			*/
} ;

struct LIBFILE				/* pointers to file library functions	*/
{
	char * (*libbuildname)() ;	/* pointer to name build function 	*/
	int (*libclose)() ;		/* pointer to file close function	*/
	int (*libcreate)() ;		/* pointer to file create function	*/
	FD  (*libopen)() ;		/* pointer to file open function	*/
	int (*libread)() ;		/* pointer to file read function	*/
	int (*libseek)() ;		/* pointer to file position function	*/
	int (*libunlink)() ;		/* pointer to file delete function	*/
	int (*libwrite)() ;		/* pointer to file write function	*/
} ;

struct LIBMEM				/* pointers to memory library functions */
{
	int * (*liballoc)() ;		/* pointer to memory allocation function*/
	int   (*libfree)() ;		/* pointer to memory free function	*/
} ;

extern ITEMID		sdp_allot() ;
extern int		sdp_audit() ;
extern int		sdp_change() ;
extern struct SPACE 	*sdp_connect() ;
extern int		sdp_destroy() ;
extern int		sdp_disconnect() ;
extern int		sdp_generate() ;
extern MY_FILE	 	*sdp_hist() ;
extern struct ENVIRON 	*sdp_house() ;
extern int		sdp_redeem() ;
extern struct STATS	sdp_stats() ;
extern int		sdp_sync() ;
extern int		sdp_unuse() ;
extern BOUNDARY 	*sdp_use() ;
extern int		sdp_vacate() ;

extern int		mesg_flag ;
extern	char		*llaerrfile ;
extern ITEMID 		*known_idptr ;
