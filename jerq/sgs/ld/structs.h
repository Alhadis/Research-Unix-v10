/*
 * static char ID_structsh[] = "@(#) structs.h: 1.8 12/1/83";
 */

#include "filehdr.h"
#include "scnhdr.h"
#include "syms.h"


typedef union act_item ACTITEM;
typedef struct infile INFILE;
typedef struct gsym SYMTAB;
typedef struct gaux AUXTAB;
typedef struct region REGION;
typedef struct anode ANODE;
typedef struct memtype MEMTYPE;
typedef struct insect INSECT;
typedef struct outsect OUTSECT;
typedef union exprnode ENODE;
typedef struct listown LISTOWN;

#if pdp11
typedef long ADDRESS;
#else
typedef unsigned long ADDRESS;
#endif

#define SYMSIZ sizeof(SYMTAB)
#define AUXSIZ sizeof(AUXTAB)


struct listown {
	char *head;
	char *tail;
	};

#ifndef ITEMID
#define ITEMID long		/* size of a SDP access id */
#endif



/*
 * The following are the possible types of Action_Items
 */

#define AILDFILE 3		/* Load a Input Object File	*/
#define AILDLBRY 4		/* Load a Library		*/
#define AIEVEXPR 5		/* Evaluate an Expression	*/
#define AIDFNSCN 6		/* Define an Output Section	*/
#define AIADDSCN 7		/* Add Input Sect to Output Section  */
#define AIADFILE 8		/* Add file to Output Section	*/
#define AIBOND 9		/* Bond output section to address */
#define AIDFOWNR 10		/* Allocate section within owner */
#define AIOWNATR 11		/* Alloc sect within owner with given attributes*/
#define AIDFNGRP 12		/* Define a "GROUP" */
#define AIDFNSYM 13		/* Define or undefine a symbol */
/*eject*/
union act_item {
	/*
	 * This structure is used for types
	 *		AILDFILE and AILDLBRY
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		char *aifilnam;		/* file name  */
		} ldlbry;
	/*
	 * This structure is used for type
	 *		AIADFILE
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		char *aifilnam;		/* file name  */
		short ainadscs;		/* no. of ADSCN actions to follow*/
		short aifill2;		/* fill value to be used for ADSCNs */
		char aifilflg;		/* flag indicating that fill was set */
		} adfile;
	/*
	 * This structure is used for types
	 *		AIDFNSCN and AIDFNGRP
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		char ainame[8];		/* name of output section  */
		short aifill;		/* value with which to fill "holes" */
		char aiowname[8];	/* name of owner, only if specified */
		ADDRESS aibndadr;	/* bond address, only if specified  */
		short aiattown;		/* attr. of owner, only if specified */
		long aialign;		/* no. of bits for alignment */
		long aiblock;		/* no. of bits for "file" alignment */
		char aifillfg;		/* flag indicating that fill was set */
		char aisctype;		/* section type  (see scnhdr.h) */
		struct listown sectspec;	/* "head" of SECTSPEC List*/
		} dfnscn;
	/*
	 * This structure is used for type
	 *		AIADDSCN
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		char ainame[8];		/* name of input section  */
		short aifill;		/* value with which to fill if uninitialized */
		char *aiscfile;		/* file name containing input section */
		} addscn;
	/*
	 * This structure is used for type
	 *		AIEVEXPR
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		ENODE *aiexptr;		/* ptr to expression node */
		} evexpr;
	/*
	 * This structure is used for type
	 *		AIDFNSYM
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		char *aisymbol;		/* symbol name  */
		short aideflag;		/* symbol def/undef flag */
		long aidefval;		/* symbol def value */
		} dfnsym;
	/*
	 * This structure is used for type
	 *		AIBOND
	 */
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		OUTSECT *aioutsec;	/* ptr to output section */
		ADDRESS aiadrbnd;
		} bond;
	/*
	 * This structure is used for type
	 *		AIDFOWNR
	 */  
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		OUTSECT *aioutsec;	/* ptr to output section */
		char ainamown[8];
		} dfownr;
	/*
	 * This structure is used for type
	 *		AIOWNATR
	 */  
	struct {
		ACTITEM  *ainext;	/* ptr to next act_item on list */
		short aitype;
		char *aiinflnm;		/* ifile that contained directive */
		short aiinlnno;		/* line number of directive */
		OUTSECT *aioutsec;	/* ptr to output section */
		short aiownatt;
		} ownatr;
	};
/*eject*/
/*
 * Structure for REGIONs
 */

struct region {
		REGION *rgnext;		/* ptr to next region on REGLIST */
		char rgname[8];		/* name of region */
		ADDRESS rgorig;
		ADDRESS rglength;
		ADDRESS rgvaddr;		/* virtual address */
		ANODE *rgaddrhd;	/* "head" of list of addr nodes */
		ANODE *rgaddrtl;	/* "tail" of list of addr nodes */
		};




/*
 * Address nodes (ANODEs) are linked off of REGIONs containing
 * an entry for every configured piece of memory.
 */

struct anode {
	ANODE *adnext;			/* ptr to next ANODE */
	ANODE *adprev;			/* ptr to previous ANODE */
	int adtype;
	ADDRESS adpaddr;		/* physical address */
	ADDRESS adsize;
	OUTSECT *adscnptr;		/* used only for type == SECT */
	MEMTYPE *admemp;		/* ptr to owning memory type  */
	REGION  *adregp;		/* ptr to owning region	      */
	};

/*
 * Possible values for adtype
 *	ADAVAIL : anode represents (a fragment of) unallocated
 *			configured virtual memory
 *	ADSECT  : anode represents an allocated output section
 *	ADPAD	: anode represents a file-padding output section
 */

#define ADAVAIL	1
#define ADSECT	2
#define ADPAD	4
/*eject*/
/*
 * Structure for Memory Type
 */

struct memtype {
		MEMTYPE *mtnext;	/* ptr to next memtype on MEMLIST */
		char mtname[8];		/* name of memtype */
		ADDRESS mtorig;
		ADDRESS mtlength;
		int mtattr;		/* attributes of memory type */
		ANODE *mtaddrhd,	/* start of owned memory nodes */
		      *mtaddrtl;	/* end of same		       */
		};
/*eject*/
/*
 * Input Section Header Structure
 */

struct insect {
	struct  scnhdr ishdr;
	INSECT *isnext;			/* next input sect on INSEC List */
	INFILE *isfilptr;		/* ptr to input file */
	OUTSECT *isoutsec;		/* ptr to output section */
	INSECT *isincnxt;		/* ptr to next input sect on INCLUDE List*/
	long isdispl;			/* Displacement w.r.t. output section */
	long isrldisp;			/* Displacement of reloc entries
					   w.r.t. reloc of output section */
	long islndisp;			/* Displacement of linno entries
					   w.r.t. linno of output section */
	long isnewvad;			/* "relocated" virtual addr of sect*/
	long isnewpad;			/* "relocated" physical addr of sect*/
	short isfill;			/* Fill value, if bss section 	*/
	short isecnum;			/* input section number */
	char isfillfg;			/* flag indicating fill was set */
	};




/*
 * Output Section Header Structure
 */

struct outsect {
	struct  scnhdr oshdr;
	int ossecnum;			/* Section Number */
	short osfill;			/* Fill value for holes in section */
	INSECT *osinclhd;		/* List of input sects included in
					   this output section */
	INSECT *osincltl; 
	OUTSECT *osnext;		/* Next section on OUTSEC List */
	long osalign;			/* no. of bits for alignment */
	long osblock;			/* no. of bits for "file" alignment */
	OUTSECT *osdsnext;		/* next section on DSECT list */
	char osflags;			/* flags */
	};
/*
 * Possible values for osflags
 *	FILL	: flag indicating fill was set
 *	OSDSECT : flag indicating at least one input section was a dummy section
 *	OSREG	: at least one of the input sections was a regular section
 *	OSINFO	: at least one of the input sections was an info section
 *	OSCOPY	: at least one of the input sections was a copy section
 */
#define FILL	1
#define OSDSECT	2
#define OSREG	4
#define OSINFO	8
#define OSCOPY	16





/*
 * Symbol Table Entry structure for global symbols and auxiliary
 * entries
 */

struct gsym {
		SYMENT sment;		/* symbol table entry		*/
		INSECT *smscnptr;	/* input section ptr	*/
		ITEMID smmyacid;	/* SDP id of this entry		*/
		ITEMID smchainid;	/* SDP id ptr to collision chain */
		ITEMID smauxid;		/* SDP id to aux entry list	*/
		long smnewval;		/* symbol value after relocation*/
		long smoutndx;		/* index of symbol in output sym tab*/
		long smnewvirt;		/* "new" virtual value or offset */
		int smnamelength;
		char smlocflg;		/* flag indicating this entry is to be
					   written in the "local" part of table
					   (for example, functions) 	*/
		char smundmsg;		/* flag for undefined symbols to
					   indicate that error msg was printed
					   (see plocsyms in output.c)	*/
		};



struct gaux {
		AUXENT axent;		/* aux entry for a symbol	 */
		int axord;		/* position of aux entry in the list */
		ITEMID axmyacid;	/* SDP id of this entry		 */
		ITEMID axchainid;	/* SDP id to next aux entry in list */
		};
/*eject*/
/*
 * Nodes in the Expression Tree
 */

union exprnode {
	struct {
		char exop;		/* Operator of the node */
		ENODE *exleft;		/* left expr ptr for binary operator*/
					/* only left ptr used for unary ops */
		ENODE *exright;		/* right expr ptr for binary operator*/
		} gnode;
	/*
	 * These structures are used for
	 *		op == NAME  and  op == DOT
	 *
	 * The first is generated during PASS 1 (parsing), and converted
	 * into the second form in PASS 2
	 */
	struct {
		char exop;		/* Operator of the node */
		char symbol[9];         /* NAME */
		} pnnode;
	struct {
		char exop;		/* Operator of the node */
		ITEMID exsymptr;	/* SDP access id of symbol table entry*/
		} nnode;
	/*
	 * This structure is used for
	 *		op == INT
	 */
	struct {
		char exop;		/* Operator of the node */
		long exvalue;
		} vnode;
	};
/*eject*/
/*
 * Input File structure in INFILIST
 */

struct infile {
	INFILE *flnext;			/* Next file on INFILIST */
	char *flname;			/* pointer to file name */
	long flnlsyms;			/* No. local sym entries in input file*/
	long flsymndx;			/* index into output symbol table
					   for local syms from this file */
	INSECT *flishead;		/* head of INSEC list for this file */
	INSECT *flistail;		/* tail of INSEC list for this file */
	long  flfiloff;			/* used for archive libraries: offset
					   form the beginning of the library
					   to this member 		     */
#if FLEXNAMES
	char	*flstrings;
#endif
	int   flfilndx;			/* used for archive libraries: # of
					   files to skip before this file*/
#if DMERT || XL
	char flplib;			/* to indicate the file is DMERT's
					   public library		  */
#endif
	};
