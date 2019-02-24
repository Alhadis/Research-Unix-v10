/*	@(#) structs.h: 1.1 5/22/83	*/

#define NSECS	20	/* maximum number of defined sections */
#define	FFUNC	10	/* maximum number of named functions	*/
#define	TEXT	0
#define DATA	1

typedef struct scnlist SCNLIST;
typedef struct funclist FUNCLIST;
#if !OLIST
typedef struct nfunction NFUNC;
#endif

/*
 * The linked list of scnlist's describes which sections are
 * to be disassembled
 */

struct scnlist
{
	SCNHDR		shdr;
	SCNLIST		*snext;
	unsigned short	scnum;
	int		stype;		/* disassemble as text or data   */
	FUNCLIST	*funcs;		/* the list of functions defined */
					/* in this section		 */
};

/*
 * A list of functions is associated with each section. This list is
 * used for printing the names of the functions and resyncing.
 */

struct funclist
{
	char		*funcnm;
	long		faddr;		/* address of the function	*/
	long		fcnindex;	/* index of the function in the symbol table */
	FUNCLIST	*nextfunc;
#if OLIST
	long		fend;		/* address of the end of the function */
#endif
};

/*
 * If the -F option is specified, an array of nfunctions is set up
 * contiaing information about the functions
 */

#if !OLIST
struct nfunction
{
	char	*funcnm;
	long	faddr;
	long	fcnindex;
	long	fsize;
	int	found;
	unsigned short	fscnum;
};
#endif
