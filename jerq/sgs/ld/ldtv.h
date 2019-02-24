/*
 * static char ID_ldtvh[] = "@(#) ldtv.h: 1.2 3/13/83";
 */

typedef struct tvinfo TVINFO;

	/* Structure for tv information. */

struct tvinfo
{
	OUTSECT	*tvosptr ;	/* We'll need this for outsclst. */
#if FLEXNAMES
	char	*tvfnfill;	/* Name of fill function. */
#else
	char	tvfnfill[8] ;	/* Name of fill function. */
#endif
	TVENTRY	tvfill ;	/* fill values for tv */
	char	*tvinflnm ;	/* input file name. */
	int	tvinlnno ;	/* input line number. */
	int	tvlength ;	/* number of entries */
	long	tvbndadr ;	/* bond address, -1L if none */
	int	tvrange[2];	/* index of first and last tv slot */
} ;


extern TVINFO tvspec ;		/*  Structure of information needed about */
				/*  the transfer vector (and the .tables */
				/*  section).  Part of this structure is */
				/*  added to either outsclst or bondlist; */
				/*  so these must not be freed before the */
				/*  last use of tvspec. */

typedef struct tvassign TVASSIGN;

	/* Structure for user directed assignment of tv slots */

struct tvassign
{
	TVASSIGN *nxtslot;	/* pointer to next assignment in list	*/
#if FLEXNAMES
	char *funname;		/* name of function assigned to		*/
#else
	char funname[8];	/* name of function assigned to		*/
#endif
	int slot;		/* slot index (not offset)		*/
} ;

extern	TVASSIGN *tvslot1;	/* ptr to head of slot list 		*/
extern	int	 tvslotcnt;	/* number of assigned slots		*/
